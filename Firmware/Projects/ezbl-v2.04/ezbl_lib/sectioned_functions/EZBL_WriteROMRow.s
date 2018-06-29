;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Source File with external symbol 
;  dependencies
;
;  Summary:
;    Assembly language optimized helper routines required by the EZBL core
;
;  Description:
;    Assembly language optimized helper routines required by the EZBL core
;    and optionally also callable from EZBL optional submodules.
;
;*******************************************************************************/
;
;// DOM-IGNORE-BEGIN
;/*******************************************************************************
;  Copyright (C) 2016 Microchip Technology Inc.
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

	.equ	SR_C, 0x0	; SR<C> is bit 0
	.equ	SR_Z, 0x1	; SR<Z> is bit 1
	.equ	SR_OV, 0x2	; SR<OV> is bit 2
	.equ	SR_N, 0x3	; SR<N> is bit 3


/**
 * Attempts to accelerate Flash programming operations by using Row Programming 
 * hardware, when available. Row Programming ordinarly requires special 
 * destination address alignment on 0x80 (384 bytes) boundaries and with data 
 * lengths that are of equal size. This function, will, however, accept other 
 * addresses and data byte counts and try to pad with all '1's data out to the 
 * start and end addresses of the row(s). You still, however, should try to 
 * match alignment and sizes to the native hardware row size for best 
 * performance. Doing so ensures no address gets programmed more than one or 
 * two times before issuing an erase (limit defined on a per-device basis). Not 
 * all devices support all '1's programming acceleration to skip programming 
 * full Flash words containing all '1's, so the all '1's data padding can still 
 * count against this Flash programming limit for such pad addresses within the 
 * specified row(s).
 *
 * NOTE: This function currently only supports Row Programming on the select 
 *      devices that have 128 write latchs (384 bytes) @ program address 
 *      0xFA0000 and an NVMCON<3:0> NVOP encoding of 0x2 for starting the Row 
 *      Write operation. On devices where this Row Programming hardware 
 *      variation is unavailable, EZBL_WriteROM() will be internally called 
 *      instead, resulting in execution of the desired Flash write operation 
 *      using the slower, but universally supported Flash Word Programming 
 *      method.
 *
 *      You can directly call EZBL_WriteROM() instead of this function if you 
 *      know Row Programming is not available on your target device, since it 
 *      will avoid linking to this second function and shrink your bootloader 
 *      or project code size.
 *   
 * Before calling this function, set EZBL_NVMKey = 0x03DF to permit
 * the Flash write. Any other value in EZBL_NVMKey will cause all function
 * behavior to execute, but without actually writing anything to Flash.
 *
 * The operation blocks during the write operation. If this function is called
 * while a prior non-blocking NVM Flash erase or write operation is already in
 * progress, the function safely blocks until the prior operation completes. This
 * function disables interrupts only for each Flash Row programming operation,
 * and then reenables interrupts immediately after the programing operation.
 * This allows queued interrupts to still execute with minimal extra latency.
 *
 * This function can be used to program "Volatile" Flash based Configuration
 * Words stored in the last page of Flash memory. It CANNOT be used to program
 * "Non-volatile" type Config Words (exist in the 0xF8xxxx address range). Use
 * EZBL_WriteROM() instead when targetting these types of Config Words.
 *
 * This function cannot be used to program EEPROM or Data Flash, if it exists.
 * These memories require different programming sizes and NVMCON control codes.
 *
 * This function will program Auxiliary Flash and Inactive Panel Flash if it
 * exists on the device.
 *
 * On devices that implement Flash ECC, you must not attempt to program any 
 * Flash word location more than once per erase (i.e. all such bytes must be
 * masked as 0xFF if they exist in the destination address range). On devices
 * without Flash ECC, programing a preexisting Flash word a second time might be
 * permissable, but is illegal on many devices because it is outside the
 * specified Flash operating characteristics I.e. doing so may damage the Flash
 * cells and they will no longer meet specified Flash retention and endurance
 * specifications. Programming a Flash word 3+ times without erasing it first is
 * never allowed (although this function does not prevent you from doing so).
 *
 * @param flashWriteAddress
 *   Address in Program Memory to start writing to. This parameter must be
 *   a legal memory address aligned to the native Flash word width (3 or 6 bytes,
 *   which equates to 0x2 or 0x4 addresses). Bit 0 must be cleared. No error 
 *   checking is done for unaligned or addresses outside the implemented 
 *   destination memory. 
 *
 * @param srcData
 *   Pointer to the source data in RAM to write to Flash or Flash-based Volatile 
 *   Config Words. The data to write must be packed in RAM without "phantom bytes", ex:
 *       srcData[0]: first instruction (or first Flash Config Word byte), low byte
 *       srcData[1]: first instruction,  middle byte
 *       srcData[2]: first instruction,  high byte
 *       srcData[3]: second instruction, low byte
 *       srcData[4]: second instruction, middle byte
 *       srcData[5]: second instruction, high byte
 *
 * @param dataByteCount
 *   Number of bytes to write to Flash or Config Words. If the data length is
 *   not an integer multiple of the native Flash word or Config Word write size,
 *   the bytes in the last partial word are padded with 0xFF bytes and still
 *   programmed. This occurs without causing RAM addresses beyond the specified
 *   length from being accessed.
 *
 *   No error checking is done to ensure the flashWriteAddress + effective number
 *   of addresses needed for dataByteCount is physically present on the device.
 *   Care should be taken to ensure an attempt to program outside the legal
 *   address range is not performed as the results are undefined and may brick
 *   the device (ex: possible address wraparound).
 *    
 * @param dataByteCount	Number of bytes of data at *srcData to program into 
 *	Flash. This value can be as small as 0 (function performs no write), 
 *      bigger than a single row (multiple row writes will take place 
 *      internally to program all data sequentially), and can be values that do 
 *      not end an a perfect Flash word size boundary. 0xFF bytes will be 
 *      appended to fill all remaining bytes and addresses to reach the end of 
 *	the Flash Row.
 *
 * @return
 *   Returns the next sequential Flash Row's memory address. In other words:
 *   (flashWriteAddress + (dataByteCount+2)/3*2) + addresses filled with padding
 *   or
 *   (flashWriteAddress + (dataByteCount+2)/3*2) + addresses filled with padding
 *   depending on the device and destination memory range.
 *   or
 *   the next Flash Word address, if fallback to internally calling 
 *   EZBL_WriteROM() Word based programming was performed.
 *
 *   The value contained in EZBL_NVMKey is cleared to 0x0000.
 *
 *   NVMCON<WR> and NVMCON<WREN> are cleared upon return. Any other NVMCON bits
 *   (like NVMPIDL for NVM Power Down In Idle), if they exist, will likely be
 *   cleared and must be saved/restored outside this function.
 *
 * w1:w0 = flashWriteAddress
 * w2 = *srcData
 * w3 = dataByteCount
 */
 ; unsigned long EZBL_WriteROMRow(unsigned long flashWriteAddress, void *srcData, unsigned int dataByteCount);
    .pushsection    .text.EZBL_WriteROMRow, code
    .global         _EZBL_WriteROMRow
    .type           _EZBL_WriteROMRow, @function
    .weak           _NVMADR     ; Extern SFR register that doesn't exist on all devices
    .weak           _NVMADRU    ; Extern SFR register that doesn't exist on all devices
    .weak           _NVMSRCADRL ; Extern SFR register that doesn't exist on all devices
    .weak           _NVMSRCADRH ; Extern SFR register that doesn't exist on all devices
    .extern         _EZBL_WriteROM
    .extern         _EZBL_NVMKey
    .extern         _EZBL_WriteNVMCON
    .extern         SR
    .extern         NVMCON
    .extern         TBLPAG
    
    reset       ; Guard against possible function entry from Program Counter accidentally falling into executing this function (ex: operating outside Vdd/Clock speed allowable region and device is mis-executing occassional instructions)
_EZBL_WriteROMRow:
    mov         #_NVMADR, w7	; Check if transparaent write address latches exist
    cp0         w7
    btsc        SR, #SR_Z
    goto        _EZBL_WriteROM	; They are indeed transparent, fall back to Word Programming.
    mov         #_NVMSRCADRL, w7; Check if newer dsPIC33E bus mastered Row Programming is used, instead of latches at 0xFA0000.
    cp0         w7
    btss        SR, #SR_Z
    goto        _EZBL_WriteROM	; They do exist, fall back to Word Programming
    
1:  btsc        NVMCON, #15     ; Loop, blocking until last NVM operation is complete (WR is clear)
    bra         1b

    push        TBLPAG
    mov         w1, _NVMADRU
    mov         #0xFA, w1
    mov         w1, TBLPAG

    mov         #0xFFFF, w5 ; w5 = 0xFFFF filler data for unaligned locations
    mov         #0xFF00, w6 ; w6 = mask for setting row alignment (used only for set up)
    mov         #0x00FF, w7 ; w7 = mask for checking row completion
    and         w0, w6, w1  ; Compute Row aligned address (low word only needed)
    mov         w1, NVMADR  ; Set the row aligned write address
    sub         w0, w1, w4  ; Compute how many addresses of misalignment we have to fill
    clr         w1          ; w1 = 0x0000, use for programming latch address pointer
    lsr         w4, w4      ; Compute how many instructions of misalignment we have to fill
    bra         Z, 2f       ; Skip 0xFFFFFF filling if there isn't any misalignment
    sub         w4, #1, w4  ; Subtract one instruction so we can use a repeat loop which adds one to the count

    ; Fill the row latches until we reach the user provided flashWriteAddress
    repeat      w4
    tblwtl      w5, [w1++]  ; Fill all low words with 0xFFFF until we reach flashWriteAddress
    repeat      w4
    tblwth      w5, [--w1]  ; Fill all high words with 0xFF

    ; Check if dataByteCount == 0 or if there is any data left
2:  cp0         w3
    bra         Z, 7f       ; No data, go clean up and return
    bra         4f

    ; Now fill rest of 128 instruction latches with data from *srcData (unless we run out, in which case, we need to 0xFFFFFF fill again to reach the end of the row)
3:  tblwtl.b    [w2++], [w1++]
    tblwtl.b    [w2++], [w1--]
    tblwth.b    [w2++], [w1++]
    add         w1, #1, w1
    and         w1, w7, w4
    bra         Z, 6f       ; All 128 latches full, do the write
4:  sub         w3, #3, w3  ; Going to copy in chunks of 3 bytes, check if we have at least 3 bytes
    bra         GEU, 3b     ; Full instruction available


    ; Check if 1 or 2 bytes of data left
    add         w3, #3, w3  ; We pre-decrement by 3, so undo underflow
    bra         Z, 5f       ; No data, perfect boundary -> Go to fill step
    sub         w3, #1, w4  ; Compute repeat value

    ; Copy last 1 or 2 bytes that don't complete an instruction plus 0xFF pre-fill to complete the instruction
    tblwtl      w5, [w1]
    tblwth      w5, [w1]
    repeat      w4
    tblwtl.b    [w2++], [w1++]
    add         w1, #1, w1
    bclr        w1, #0
    and         w1, w7, w4
    bra         Z, 6f           ; All 128 latches full, do the write

5:  ; Generate 0xFFFFF fill to the end of the row
    tblwtl      w5, [w1]
    tblwth      w5, [w1++]
    and         w1, w7, w4
    bra         NZ, 5b          ; Not all latches full, add another 0xFFFFFF fill word

6:  ; All 128 latches full, do the write
    push.d      w2              ; Save current *srcData address and byteCount remaining
    mov         #0x4002, w0     ; Set WREN, clear ERASE (if present) and set NVMOP = 0x2 for Flash Row Program Operation
    setm        w2              ; Stall until complete. Note: this is required on non-blocking writeable hardware whenever dataByteCount necessitates more than one Flash word write since otherwise we would return and start modifying the NVMADR/NVMARDU and/or Table latches while the last erase/write is still underway. The NVM hardware won't automatically handle this.
    mov         #0x03E5, w1     ; Set NVMKey unlocking material (EZBL_NVMKey must have externally been set to 0x03DF)
    push        _EZBL_NVMKey    ; Save a copy of EZBL_NVMKey since we might need it again for more word writes
    call        _EZBL_WriteNVMCON   ; Do the write
    pop         _EZBL_NVMKey    ; EZBL_NVMKey is zeroed by above function, so restore it
    clr         [w15]           ; Dummy write to destroy copy of NVMKey value on the stack
    pop.d       w2              ; Restore *srcData address and byteCount remaining

    ; Set up for doing another row write
    mov         #0xFFFF, w5     ; w5 = 0xFFFF filler data for unaligned locations
    mov         #0x00FF, w7     ; w7 = mask for checking row completion
    clr         w1              ; w1 = 0x0000, use for programming latch address pointer
    mov         #0x100, w0      ; Increment to next row address
    add         _NVMADR
    addc        _NVMADRU         ; Adds 0x100 + C to NVMADRU, but this register can only have 8 bits in it, so MSbit just goes away
    bra         2b              ; Go back and test for more data for another row write


    ; All bytes programmed: clean up, collect return value, and return
7:  clr         _EZBL_NVMKey ; Kill the key data
    pop         TBLPAG
    mov         _NVMADR, w0
    mov         _NVMADRU, w1
    return
    
    .size           _EZBL_WriteROMRow, . - _EZBL_WriteROMRow + 2
    .popsection
    