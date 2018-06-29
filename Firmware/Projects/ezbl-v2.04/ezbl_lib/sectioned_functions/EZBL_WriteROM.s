;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Source File
;
;  Summary:
;    Flash RTSP programming routine for 16-bit devices
;
;  Description:
;    Assembly language optimized Flash programming routine for writing to the 
;    internal Flash memory on the majority of dsPIC33F, dsPIC33E, dsPIC33D, 
;    dsPIC33C, PIC24F, PIC24H, and PIC24E, devices.
;
;*******************************************************************************/
;
;// DOM-IGNORE-BEGIN
;/*******************************************************************************
;  Copyright (C) 2017 Microchip Technology Inc.
;
;  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
;  derivatives created by any person or entity by or on your behalf, exclusively
;  with Microchip's products.  Microchip and its licensors retain all ownership
;  and intellectual property rights in the accompanying software and in all
;  derivatives here to.
;
;  This software and any accompanying information is for suggestion only.  It
;  does not modify Microchip's standard warranty for its products.  You agree
;  that you are solely responsible for testing the software and determining its
;  suitability.  Microchip has no obligation to modify, test, certify, or
;  support the software.
;
;  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
;  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
;  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
;  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
;  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
;
;  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
;  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
;  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
;  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
;  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
;  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
;  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
;  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
;  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
;
;  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
;  TERMS.
;*******************************************************************************/
;// DOM-IGNORE-END

    .equ    SR_C, 0x0   ; SR<C> is bit 0
    .equ    SR_Z, 0x1   ; SR<Z> is bit 1
    .equ    SR_OV, 0x2  ; SR<OV> is bit 2
    .equ    SR_N, 0x3   ; SR<N> is bit 3
    .macro  skip_next_instr
    btsc    w15, #0     ; Skips next instruction since w15 stack pointer can never be odd
    .endm

    ; Writes a literal into the TBLPAG register, using the MOVPAG opcode (when
    ; available), or falling back to a mov #literal, tempReg;
    ; mov tempReg, TBLPAG sequence when unavailable.
    ;
    ; NOTE: tempReg is not used/will not have the literal loaded in it when
    ;       MOVPAG is used.
    .macro  loadTBLPAG literal, tempReg
    .ifdef  ezbl_lib16ch        ; -mcpu=generic-16dsp-ch target
        movpag  \literal, TBLPAG
    .else
        .ifdef  ezbl_lib16ep    ; -mcpu=generic-16bit-ep target
            movpag  \literal, TBLPAG
        .else
            mov     \literal, \tempReg
            mov     \tempReg, TBLPAG
        .endif
    .endif
    .endm

    ; Dummy section so the .sizeof.(.text.EZBLNoProgramRanges), .startof_hi., 
    ; and .startof. builtins used in EZBL_WriteROM() will not trigger a link 
    ; error in the event there are no EZBLNoProgramRanges defined elsewhere in 
    ; the project. The code is written to handle an empty list.
    .pushsection    .text.EZBLNoProgramRanges, code, keep, page
    .popsection


/**
 * Programs an array of bytes into Flash or Config Word memory at the specified
 * Program Address.
 *
 * Before calling this function, set <b>EZBL_NVMKey = 0x03DF</b> to permit
 * the flash write. Any other value in EZBL_NVMKey will cause all function
 * behavior to execute, but without actually writing anything to Flash.
 *
 *
 * The operation blocks for all but the last flash write block's write
 * operation. If this function is called while a prior non-blocking NVM flash
 * erase or write operation is already in progress, the function blocks until
 * the prior operation completes.
 *
 * Interrupts are disabled for a minimal time (estimated 19 to 38 instruction
 * cycles, depending on target device) before starting each Flash
 * word/double word programming operation. This is done by temporarily raising
 * to CPU IPL8.
 *
 * If you don't want interrupts to execute at all for the entire programming
 * sequence, call EZBL_DisableInterrupts() before calling this function.
 *
 * In timing critical applications, an optional
 *     volatile unsigned int * __attribute__((near)) EZBL_IntDisReqReg;
 * global variable should be declared and initialized to point to an IFSx
 * interrupt flags SFR address or RAM location to facilitate synchronization of
 * when precisely this function is allowed to disable interrupts and perform a
 * queued write operation. When enabled, bit 0 (or EZBL_IntDisReqFlagPos) of the
 * SFR/RAM pointer target will be set when EZBL_WriteROM[Ex]() needs to disable
 * interrupts, and then the code will block until something outside the
 * EZBL_WriteROM[Ex]() function clears the same bit.
 *
 * Generally, a time sensitive ISR would/should clear the bit each time it
 * finishes execution, allowing interrupts to be disabled for the NVM operation
 * only when an NVM write operation is least likely to interfere with concurrent
 * application functionality.
 *
 * To specify which bit in (*EZBL_IntDisReqReg) will be used for synchronization
 * (instead of the default of bit 0), at global scope, write:
 *     EZBL_SetSYM(EZBL_IntDisReqFlagPos, <i>i</i>);
 * where '<i>i</i>' is a link-time integer constant between 0 and 15.
 *
 *
 * In Bootloader or other applications where some of the device's program memory
 * map should never be programmed or altered, the applicable address range(s)
 * should be set by using the:
 *     EZBL_SetNoProgramRange(<i>startProgramAddr</i>, <i>endProgramAddr</i>);
 * macro declared at global scope. The start address is inclusive while the end
 * address is exclusive (i.e. the first address that IS programable).
 *
 * This macro will result in all EZBL_WriteROM[Ex]() calls being checked against
 * the given range and piecewise skipped if overlapping.
 * EZBL_SetNoProgramRange() can be instanced multiple times to define
 * discontinuous address ranges that need to be write-protected.
 *
 * Skipping occurs based on the minimum flash write granularity applicable for
 * the device. For example, on devices that can program 48-bit single
 * instruction words, a no-program range of [0x000230, 0x000240), and a
 * EZBL_WriteROM[Ex]() call to program [0x000200, 0x000300) will result in data
 * being programmed to [0x000200, 0x000230) and [0x000240, 0x000300) with the
 * input source data for the no-program range being read but thrown away to
 * maintain logical stream synchronization.
 *
 *
 * This function can be used to program "Volatile" flash based Configuration
 * Words stored in the last page of Flash memory. It will also program
 * "Non-volatile" type Config Words (exist in the 0xF8xxxx address range).
 *
 * This function will program Auxiliary Flash and Inactive Panel Flash if it
 * exists on the device and is not otherwise disallowed due to Config word
 * security settings.
 *
 * On devices that implement flash ECC, you must not attempt to program the same
 * flash word location more than once per erase. Do so will corrupt the internal
 * ECC bits and lead to a subsequent trap exception when attempting to execute
 * or anything on the same flash word. The EZBL_WriteROMOnlyOnce() API can be
 * used to pre-read the flash contents and mask off write data that would lead
 * to an ECC violation.
 *
 * On devices without ECC, programming a flash word a second time might be
 * permissable, but is illegal on many devices because it is outside the
 * specified flash operating characteristics I.e. doing so too many times may
 * degrade the flash cells and invalidate retention/endurance specifications.
 * Programming a flash word 3+ times without erasing it first is not allowed on
 * any device (although this function does not prevent you from doing so).
 *
 * @param destProgAddr
 *   Address in Program Memory to start writing to. This parameter must be
 *   a legal memory address aligned to the native flash word width (0x2 or 0x4
 *   addresses, which equates to 3 or 6 bytes), or, if targeting non-volatile
 *   Config Words, aligned to 3 bytes/0x2 addresses.
 *
 *   As all flash word addresses are even, this implies bit 0 of destProgAddr
 *   must always be '0'.
 *
 * @param srcData
 *   Pointer to the source data in RAM to write to flash or Config Words. The
 *   data to write must be packed in RAM without "phantom bytes", ex:
 *       srcData[0]: first instruction (or first Config Word byte), low byte
 *       srcData[1]: first instruction (or dummy byte for Config Words), middle byte
 *       srcData[2]: first instruction (or dummy byte for Config Words), high byte
 *       srcData[3]: second instruction (or second Config Word byte), low byte
 *       srcData[4]: second instruction (or dummy byte for Config Words), middle byte
 *       srcData[5]: second instruction (or dummy byte for Config Words), high byte
 *
 *   Use EZBL_RemovePhantoms() if your source data must first be converted to
 *   the required packed form.
 *
 * @param byteCount
 *   Number of bytes to write to Flash or Config Words. If the data length is
 *   not an integer multiple of the native Flash word or Config Word write size,
 *   the bytes in the last partial word are padded with 0xFF bytes and still
 *   programmed. This occurs without causing RAM addresses beyond the specified
 *   length from being accessed.
 *
 *   No error checking is done to ensure the destProgAddr + effective number
 *   of addresses needed for byteCount is physically present on the device.
 *   Writing to unimplemented target addresses is normally harmlessly ignored,
 *   but can cause an Address Error Trap upon read-back.
 *
 * @param flags
 *   Bitwise OR'd collection of flags to modify the writing behavior. Available
 *   options are:
 *   <ul>
 *      <li>EZBL_FLAG_CLRWDT (0x0400)           - Clear watchdog before each write</li>
 *      <li>EZBL_FLAG_LATE_INT_EN (0x0800)      - Restore interrupts only after NVMOP completes. By default interrupts are disabled only for the unlock sequence that sets NVMCON<WR>.</li>
 *      <li>EZBL_FLAG_IPARTITION (0x1000)       - Force 0x400000-0x7FFFFF Inactive Partition address range (set bit 22 of destProgAddr)</li>
 *      <li>EZBL_FLAG_FIXED_PARTITION (0x2000)  - Use fixed Partition 1/Partition 2 target address range for destProgAddr instead of Active/Inactive relative addresses (XOR bit 22 of flashWriteAddress with !NVMCON<10>, which is !P2ACTIV on Dual Partition capable devices)</li>
 *   </ul>
 *
 * @param extraNoProgRanges
 *   Number of address pairs at noProgRanges to use for excluding specific
 *   addresses from being written.
 *
 *   Every 1 extraNoProgRanges implies 8 bytes of RAM at *noProgRanges will
 *   be read. Set to 0 if the default .text.EZBLNoProgramRanges section data
 *   should be used without anything extra.
 *
 *   The special value 0xDEAD (-8531) can be given to disable all no-program
 *   range checking, ignoring all contents in .text.EZBLNoProgramRanges and
 *   ignoring the *noProgRanges parameter. Use this with caution as it will
 *   allow overwriting of any device address, including this function and
 *   bootloader code, potentially bricking the device.
 *
 * @param *noProgRanges pointer to array 32-bit address pairs in RAM which
 *                      should be skipped during writing flash if found to
 *                      overlap the dest address write range.
 *
 *                      This parameter is ignored if
 *                      extraNoProgRanges == 0 or 0xDEAD.
 *
 * @return
 *   Returns the next sequential flash word's memory address. In other words:
 *   (destProgAddr + (byteCount+2)/3*2) or
 *   (destProgAddr + (byteCount+5)/6*4) depending on the device and
 *   destination memory range.
 *
 *   The value contained in EZBL_NVMKey is cleared to 0x0000.
 *
 *   NVMCON<WR> and NVMCON<WREN> may be cleared upon return, depending upon if
 *   the final write has completed or not by the time the return code executes.
 *   Other status and control bits in NVMCON, if needed, should be preserved
 *   outside this function as they will also be cleared.
 */
;   unsigned long EZBL_WriteROMEx(unsigned long destProgAddr, const void *srcData, unsigned int byteCount, unsigned int flags, int extraNoProgRanges, unsigned long *noProgRanges);
;   unsigned long EZBL_WriteROM  (unsigned long destProgAddr, const void *srcData, unsigned int byteCount);
;                                               w1:w0                     w2                    w3                      w4         w5                                w6
    .pushsection    .text.EZBL_WriteROMEx, code
    .global         _EZBL_WriteROMEx
    .global         _EZBL_WriteROM
    .type           _EZBL_WriteROMEx, @function
    .type           _EZBL_WriteROM, @function
    .extern         TBLPAG
    .extern         NVMCON
    .extern         _EZBL_NVMKey
    .extern         _EZBL_ROMCompare
    .weak           _EZBL_IntDisReqReg      ; SFR or near RAM variable for synchronizing interrupt disable requests to an ISR
    .weak           _EZBL_IntDisReqFlagPos  ; Bit in a _EZBL_IntDisReqReg register
    .weak           _NVMADR     ; Extern SFR register that doesn't exist on all devices
    .weak           _NVMADRU    ; Extern SFR register that doesn't exist on all devices
    
    ; Stack has #0x03E5 unlock key, TBLPAG save, w8-w14 save, NVMCON, NVMADR, NVMADRU, 6-bytes of NVMDATA buffer, EZBLNoProgramRanges RAM table, EZBL_NVMKey unlock cached data
    reset       ; Guard against possible function entry from Program Counter accidentally falling into executing this function (ex: operating outside Vdd/Clock speed allowable region and device is mis-executing occassional instructions)

; flags for future consideration:
;   NOT IMPLEMENTED - 0x0001 - EZBL_FLAG_SRC_IS_FIFO     - *srcData is pointer to EZBL_FIFO requiring wrapped reads
;   NOT IMPLEMENTED - 0x0002 - EZBL_FLAG_SRC_IS_PGM_ADDR - *srcData points to unsigned long Flash memory address (stored in RAM, real source data is another Flash location that must be read before any overlapping erase/write takes place)
;   NOT IMPLEMENTED - 0x0004 - EZBL_FLAG_SRC_IS_SFR      - *srcData points to byte SFR (ex: U2RXREG) requiring no address increment, but polled data availability
;   NOT IMPLEMENTED - 0x0010 - EZBL_FLAG_AUTO_PAGE_ERASE - Auto-page erase (Erase Page before programing a new Page start boundary address)
;   NOT IMPLEMENTED - 0x0020 - EZBL_FLAG_RD_MODIFY_WR    - Read-erase-modify-write full page contents for EEPROM style emulation writes (or just write Config byte fuses which are EEPROM like and require no erase)
;   NOT IMPLEMENTED - 0x0040 - EZBL_FLAG_ATOMIC_ABORT    - Atomically abort the entire write request if dest address range is not blank (default is not do pre-reading, or with auto-page-erase, skip only individual write blocks which can't match the intended write data after being programmed)
;   NOT IMPLEMENTED - 0x0080 - EZBL_FLAG_BIT_CLR_MASK    - Clear bits only: source data is a bitmask where '1' values represent bits in Flash that need to be changed to '0' values, leaving all other Flash bits unchanged
;   NOT IMPLEMENTED - 0x0100 - EZBL_FLAG_BIT_SET_MASK    - Set bits only: source data is a bitmask where '1' values represent bits in Flash that need to be changed to '1' values, leaving all other Flash bits unchanged. Use this flag only with the Read-erase-modify-write flag as it is not possible to change existing Flash '0' bits back to '1's otherwise.
;   NOT IMPLEMENTED - 0x0200 - EZBL_FLAG_BIT_AND_VFY     - Bitwise AND write data verify strategy (ignore '0' bits which already existed in Flash and which we were told explicitly or implicitly to write as '1's)
;   0x0400 - EZBL_FLAG_CLRWDT          - Clear watchdog before each write
;   0x0800 - EZBL_FLAG_LATE_INT_EN     - Restore interrupts only after NVMOP completes
;   0x1000 - EZBL_FLAG_IPARTITION      - Force 0x400000-0x7FFFFF Inactive Partition address range (set bit 22 of destProgAddr)
;   0x2000 - EZBL_FLAG_FIXED_PARTITION - Use fixed Partition 1/Partition 2 target address range for destProgAddr instead of Active/Inactive relative addresses (XOR bit 22 of destProgAddr with !NVMCON<10>, which is !P2ACTIV on Dual Partition capable devices)
;   0x8000 - _WRITE_TRANSPARENT_LATCH (INTERNAL ONLY) - Write latches are transparent inline (must set TBLPAG to destination page address, not 0xFA)    
    .equ    EZBL_FLAG_SRC_IS_FIFO,      0
    .equ    EZBL_FLAG_SRC_IS_PGM_ADDR,  1
    .equ    EZBL_FLAG_SRC_IS_SFR,       2
    .equ    EZBL_FLAG_AUTO_PAGE_ERASE,  4
    .equ    EZBL_FLAG_RD_MODIFY_WR,     5
    .equ    EZBL_FLAG_ATOMIC_ABORT,     6
    .equ    EZBL_FLAG_BIT_CLR_MASK,     7
    .equ    EZBL_FLAG_BIT_SET_MASK,     8
    .equ    EZBL_FLAG_BIT_AND_VFY,      9
    .equ    EZBL_FLAG_CLRWDT,           10
    .equ    EZBL_FLAG_LATE_INT_EN,      11
    .equ    EZBL_FLAG_IPARTITION,       12
    .equ    EZBL_FLAG_FIXED_PARTITION,  13
    .equ    _WRITE_TRANSPARENT_LATCH,   15
    
;   unsigned long EZBL_WriteROMEx(unsigned long destProgAddr, const void *srcData, unsigned int byteCount, unsigned int flags, unsigned int extraNoProgRanges, unsigned long *noProgRanges);
;                                               w1:w0                     w2                    w3                      w4                  w5                                w6

    
;   unsigned long EZBL_WriteROM(unsigned long destProgAddr, const void *srcData, unsigned int byteCount);
;                                             w1:w0                     w2                    w3                        w4 = flags (0x0000 for native EZBL_WriteROM() call)

;     w1:w0 = destProgAddr (then temporaries)
;        w2 = *srcData pointer
;        w3 = byteCount
;        w4 = flags (0x0000 for native EZBL_WriteROM() call)
;        w5 = extraNoProgramRanges, then unused
;        w6 = *noProgramRanges, then temp/transient SFR write pointer
;        w7 = temp/transient stack RAM read/write pointer
;        w8 = EZBLNoProgramRanges table base start address in RAM
;        w9 = Permanent (extraNoProgramRanges + EZBLNoProgramRanges)*2
;   w10-w12 = general purpose/temp
;       w13 = Write block address size (0x2 or 0x4 for Flash Word, Flash Double Word, respectively. Bigger potentially for row writes (PIC24FxxKxxx series devices).
;       w14 = Frame pointer (points to 
;                   NVMCON/NVMADR/NVMADRU/8-bytes for NVMDATA buffer (unpacked to 4 bytes per instruction word),
;                   EZBLNoProgramRanges table,
;                   EZBL_NVMKey unlock save value on stack)
_EZBL_WriteROM:     ; unsigned long EZBL_WriteROM(unsigned long destProgAddr, const void *srcData, unsigned int byteCount);
    mul.uu      w4, #0, w4      ; No flags when EZBL_WriteROM() called
_EZBL_WriteROMEx:   ; ;   unsigned long EZBL_WriteROMEx(unsigned long destProgAddr, const void *srcData, unsigned int byteCount, unsigned int flags, unsigned int extraNoProgRanges, unsigned long *noProgRanges);
;    ; Debug Code
;    push        [w6++]
;    push        [w6--]
;    push        w6
;    push        w5
;    push        w4
;    push        w3
;    push        w2
;    push.d      w0
;    .pushsection    .const, psv, page
;formatStr:
;    .asciz     "\n    EZBL_WriteROMEx(%06lX, %04X, %04X, %04X, %04X, %04X), *noProgRanges = %06lX);"
;    .popsection
;    mov         #formatStr, w7
;    push        w7
;    sub         w15, #24, w0
;    call        _EZBL_printf
;    mov         _EZBL_STDOUT, w0
;    setm        w2
;    setm        w3
;    call        _EZBL_FIFOFlush
;    mov         [w15-4], w1
;    mov         [w15-6], w0
;    mov         [w15-8], w2
;    mov         [w15-10], w3
;    mov         [w15-12], w4
;    mov         [w15-14], w5
;    mov         [w15-16], w6
;    sub         w15, #20, w15


    cp0         w3              ; Early return for a 0 byte write
    bra         Z, earlyAbortPass
    lsr         w1, #8, w7      ; Early test for illegal target write address > 0x00FFFFFF
    bra         NZ, earlyAbortFail  ; if(destProgAddr >= 0x01000000) write failing return value

    mov         #0x03E5, w7     ; Save partial NVMKEY unlocking material (EZBL_NVMKey must also have externally been set to 0x03DF)
    push        w7              
    push        TBLPAG          ; Save TBLPAG on stack
    push.d      w8
    push.d      w10
    push.d      w12

    btst        w1, #7          ; Don't force address bit 22 if we are targetting 0x800000-0xFFFFFF
    bra         NZ, 1f
    btst        w4, #EZBL_FLAG_FIXED_PARTITION
    bra         Z, 0f
    btsc        NVMCON, #10     ; NVMCON<10> has to be P2ACTIV bit on Dual Partition devices
    btg         w1, #6
0:  btsc        w4, #EZBL_FLAG_IPARTITION
    bset        w1, #6         ; Make address >= 0x400000

1:  mov         #0x4003, w11    ; Set NVMCON<WREN> and NVMCON<NVMOP> for writing to Flash using single Flash Word writes (24-bits at a time, only present on some devices)
    mov         #0x2, w13       ; w13 = 0x2 Address increment
    bset        w4, #_WRITE_TRANSPARENT_LATCH   ; Single instruction word write methods require target write address TBLPAG value
    mov         #_NVMADR, w12
    cp0         w12
    bra         Z, 1f
    bclr        w4, #_WRITE_TRANSPARENT_LATCH   ; Other methods use table latches at 0xFA0000+
    mov         #0x4000, w11    ; Set NVMCON<WREN> and NVMCON<NVMOP> for writing to "non-volatile" Config bytes using single Non-volatile Config Word byte program operation (8-bits written at a time, 0x2 address increment)
    add.b       w1, #0x08, [w15]; Is Target address 0xF8xxxx? indicating a dsPIC33EP512MU810 style fuse based Config byte
    bra         Z, 1f
    mov         #0x4, w13       ; w13 = 0x4 Address increment
    mov         #0x4001, w11    ; Set NVMCON<WREN> and NVMCON<NVMOP> for writing to Flash using Flash Double Word writes (48-bits at a time, only present on some devices)

    ; Allocate space on the stack for a full expanded input block
1:  lnk         #6              ; Allocate 6 bytes on the stack for NVMCON, NVMADR, NVMADRU
    sl          w13, w12        ; w12 = size of expanded flash word
    add         w15, w12, w15   ; Add 8 (4) more bytes to hold a full expanded flash word after NVMADRU

    mov         w14, w7
    mov         w11, [w7++]     ; NVMCON position = computed NVMCON value above
    mov.d       w0, [w7++]      ; NVMADRU:NVMADR positions = destProgAddr

    mov         w15, w8         ; w8 = permanent start address for RAM table
    clr         w9              ; w9 = permanent total size in RAM for all NoProgramRanges (set to 0x0000 for w5 = 0xDEAD case)
    mov         #0xDEAD, w12    ; Skip stack table space allocation if No-NoProgramRanges protection
    cp          w5, w12
    bra         Z, 4f
    sl          w5, #2, w5      ; w5 = extraNoProgramRanges * 4*sizeof(mov.w) bytes/range
    bra         Z, 3f
    dec         w5, w5
    repeat      w5              ; Copy (unsigned long rangeStartAddr), (unsigned long rangeEndAddr) to stack no prog position
    mov         [w6++], [w15++]
    ; extraNoProgRanges/*noProgRanges (w5/w6) used up. Now copy .text.EZBLNoProgramRanges table from flash also into RAM (for quick contiguous access that doesn't need prior NVM write to complete when checking)
3:  loadTBLPAG  #.startof_hi.(.text.EZBLNoProgramRanges), w6
    mov         #.startof.(.text.EZBLNoProgramRanges), w5
    mov         #.sizeof.(.text.EZBLNoProgramRanges), w6
0:  btsc        NVMCON, #15     ; Must wait before reading from Flash if non-blocking erase or programming is accessing the same Flash partition/internal array
    bra         0b
    bra         2f
1:  tblrdl      [w5], [w15++]
    tblrdh      [w5++], [w15++]
2:  dec2        w6, w6
    bra         C, 1b
    sub         w15, w8, w9     ; w9 = permanent total size in RAM for all NoProgramRanges

4:  push        _EZBL_NVMKey    ; Save key data on the Top of Stack so it has higher chance of getting corrupted if overclocking/misexecution is happening right now
    clr         _EZBL_NVMKey    ; Kill the static key data
    bra         bytesLeftCheck

    
InputPartialChunk:  ; Put packed input data on stack with 0xFF full chunk padding
    ; Have residual 1-5 bytes left, need to write them too, but with 0xFF padding bytes added
    mov         w15, w11
    dec         w13, w12        ; Make a full block of 0xFFFF bytes on the stack
    repeat      w12             ; Start with a full expanded block of 0xFFFF bytes (more than needed, but no big deal)
    setm        [w15++]
    dec         w3, w0          ; Copy input data
    repeat      w0
    mov.b       [w2++], [w11++]
    sub         w11, w3, w2     ; Move input pointer to the stack buffer
    clr         w3              ; byteCount = 0 so we terminate this time
    sl          w13, w1
    skip_next_instr
    
InputFullChunk: ; Put full unpacked flash word in stack frame word buffer. Phantom bytes set to 0xFF.
    clr         w1
    lsr         w13, w0
1:  repeat      #2                  ; Read 3 bytes to start
    mov.b       [w2++], [w7++]
    setm.b      [w7++]              ; Generate a 0xFF in the high byte of the upper part of the instruction word
    dec         w0, w0
    bra         NZ, 1b
    sub         w15, w1, w15        ; Remove anything from the stack that we may have created in the InputPartialChunk path
    
    ; Test for all 0xFFFF's to skip this write
    dec         w13, w12            ; Set SR<C> = 1 and makes the address size a 3 byte block count
    repeat      w12
    addc        w0, [--w7], [w15]
    bra         C, nextAddress      ; Data is all 0xFF's so skip programming this block
    
    ; Test if this is in a no-write program range (from .text.EZBLNoProgramRanges table contents and extraNoProgRanges/*noProgRanges parameters)
testNoWriteRange:
    mov         [--w7], w11     ; w11:w10 = NVMADRH:NVMADRL target write address (not using mov.d to supress psrd_psrd erratum check in XC16 v1.32b - these are RAM pointers though, so mov.d is actually legal)
    mov         [--w7], w10
    add         w10, w13, w1    ; w12:w1 = next target write address (generally +0x4 or +0x2)
    addc        w11, #0, w12
    sub         w8, #4, w6      ; w6 = table start ptr - 4 (-4 to undo +4 at label 0f when starting loop execution)
    mov         w9, w7          ; w7 = RAM table length loop counter
0:  add         w6, #4, w6      ; adjust w6 for last skipped read/compare
1:  sub         w7, #8, w7
    bra         NC, 2f
    cp          w1, [w6++]      ; (target + 0x4) - startLimitAddress    ; (target + 0x4 <= startLimitAddress is fine)
    cpb         w12, [w6++]     ; No Borrow (C) can be bad, but Z is always fine; N is always fine, NN can be bad
    bra         LE, 0b
    cp          w10, [w6++]     ; target - endLimitAddress      ; Less than is bad (target < endLimitAddress) if the above test indicates (target + 0x4 > startLimitAddress)
    cpb         w11, [w6++]     ; Borrow (NC) can be bad, but Z is always fine, NN is always fine, N can be bad, (C is always fine)
    bra         C, 1b
    bra         nextAddress     ; Bad, skip write
    
2:  mov         #0x0000, w0     ; w0 = Low 16-bits of table latches to write to (mapped latches @ 0xFA0000)
    mov         #0x00FA, w1     ; w1 = High 8-bits of table latches to write to (mapped latches @ 0xFA0000)
    add         w14, #2, w6     ; w6 = temporary pointer to read the destination address from NVMADR onwords at frame pointer
    btsc        w4, #_WRITE_TRANSPARENT_LATCH
    mov.d       [w6++], w0      ; w1:w0 = destination address for inline latches (write high 8-bits to TBLPAG)
    mov         w1, TBLPAG
    lsr         w13, w1         ; w1 = number of instruction words to write
    mov         w14, w7         ; w7 = NVMCON start holding location
    mov         #NVMCON, w6     ; w6 = NVMCON writing pointer

    ; All queueable work complete. Poll until any prior NVM erase/write is 
    ; complete so we can write to the NVMCON/NVMADR[U] registers and "NVMDATA" 
    ; latches.
0:  btsc        NVMCON, #15
    bra         0b

1:  repeat      #2
    mov         [w7++], [w6++]  ; NVMCON, NVMADR, NVMADRU (NVMADR, NVMADRU may be unimplemented locations, so just throw away)
0:  tblwtl      [w7++], [w0]
    tblwth      [w7++], [w0++]
    dec         w1, w1
    bra         NZ, 0b

;    ; Debug code
;    push.d      w0
;    push.d      w2
;    push.d      w4
;    push.d      w6
;    dec2        w0, [w15++]
;    push        TBLPAG
;    .pushsection    .const, psv, page
;formatStr2:
;    .asciz     "\n    EZBL_WriteROMEx() programming %06lX"
;    .popsection
;    mov         #formatStr2, w7
;    push        w7
;    sub         w15, #10, w0
;    call        _EZBL_printf
;    mov         _EZBL_STDOUT, w0
;    setm        w2
;    setm        w3
;    call        _EZBL_FIFOFlush
;    sub         w15, #6, w15
;    pop.d       w6
;    pop.d       w4
;    pop.d       w2
;    pop.d       w0


    ; Generate NVMKEY unlocking material
2:  mov         #0x1326, w10            ; w10 = 0x1326 set up offset
    mov         [w14-18], w1            ; w1 = 0x3E5 (-18 = w14, w13, w12, w11, w10, w9, w8, TBLPAG, #0x03E5)
    mov         [w15-2], w0             ; w0 = 0x3DF originally from EZBL_NVMKey
    mul.uu      w0, w1, w0              ; w0:w1 = 0x3E5*0x3DF = 997*991 = 0x000F137B = 988,027 (these are two prime numbers, so less likely to accidentally yield the correct unlock key)
    sub         w0, w10, w7             ; w7 = 0x137B - 0x1326 = 0x0055
    mov         #_EZBL_IntDisReqReg, w10
    bset        [w10], #_EZBL_IntDisReqFlagPos  ; Writes to w0 when _EZBL_IntDisReqReg symbol not present(it's weak)
    clr         w0                      ; Clears the flag we just set when symbol not present
0:  btsc        [w10], #_EZBL_IntDisReqFlagPos
    bra         0b
    btsc        w4, #EZBL_FLAG_CLRWDT
    clrwdt
    call        _EZBL_DisableInterrupts ; Disable all interrupts by switching to IPL8; Only clobbers w0; Worst 13 cycles
    mov         w7, [w6]                ; NVMKEY = 0x0055
    add         #0x9B, w1               ; w1 = 0x009B + 0x000F = 0x00AA
    mov         w1, [w6]                ; NVMKEY = 0x00AA
    bset        NVMCON, #15             ; Start the programming sequence
    nop                                 ; Two NOPs required after setting NVMCON WR bit
    nop
    btst        w4, #EZBL_FLAG_LATE_INT_EN
0:  btsc        NVMCON, #15
    bra         NZ, 0b
    call        _EZBL_RestoreInterrupts ; Only clobbers w0                                            ; Worst 12 cycles - 38 cycles worst case interrupt disabled time when early interrupt disables are enabled, assuming no hardware blocking for NVMOP

    ; Increment destination address and check for more full 48-bit (24-bit) Flash word chunks of data
nextAddress:
1:  add         w14, #2, w7         ; w7 = Destination address pointer
    clr         w0                  ; Increment destination address by 4 (2) addresses
    add         w13, [w7], [w7++]   ; NVMADR position = 4(2) + NVMADR
    addc        w0,  [w7], [w7++]   ; NVMADRU position = NVMADRU + 0 + C

bytesLeftCheck:
    lsr         w13, w0             ; Make 0x4 == 6, or 0x2 == 3 for address to byte count conversion
    add         w13, w0, w0         ; w0 = w13*1.5 input bytes required per flash word
    sub         w3, w0, w3          ; Check if byteCount >=6 (3) bytes are left to write
    bra         C, InputFullChunk   ; Read another full flash word worth of bytes in, if available
    add         w3, w0, w3          ; Undo the last subtraction since it underflowed
    bra         NZ, InputPartialChunk; Handle any residual bytes that need padding

    ; All bytes programmed: clean up, collect return value, and return
done:
    mov         #0x3FF0, w0         ; Clear NVMCON<WR, WREN, NVMOP> if we can for protection. The hardware will ignore this if it is still busy with the last NVMCON<WR> operation.
    and         NVMCON
    clr         [--w15]             ; Kill EZBL_NVMKey on the stack
    mov         [w14+2], w0         ; Return next Flash address
    mov         [w14+4], w1
    ulnk                            ; Remove TBLPAG, NVMCON, NVMADR/NVMADRU, 6+2 data bytes, and everything above it from the stack
    pop.d       w12
    pop.d       w10
    pop.d       w8
    pop         TBLPAG
    clr         [--w15]             ; Kill 0x03E5 dynamic key parameter on stack
    
earlyAbortFail: ;illegal input parameters - full operation aborted)
earlyAbortPass:
    clr         _EZBL_NVMKey
    return

    .size       _EZBL_WriteROMEx, . - _EZBL_WriteROM + 0x2  ; 0x2 extra for reset guard band instruction before function entry
    .size       _EZBL_WriteROM  , . - _EZBL_WriteROM + 0x2  ; 0x2 extra for reset guard band instruction before function entry
    .popsection

