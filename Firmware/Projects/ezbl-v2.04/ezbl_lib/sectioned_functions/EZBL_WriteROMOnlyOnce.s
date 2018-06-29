;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Source File
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
    .macro  skip_next_instr
    btsc    w15, #0     ; Skips next instruction since w15 stack pointer can never be odd
    .endm


/**
 * Identical to EZBL_WriteROM()/EZBL_WriteROMEx() except that the target flash
 * range is read first and any already programmed flash words are individually
 * masked to all '1's in the input data to suppress their write effect.
 * 
 * Set <b>EZBL_NVMKey = 0x03DF</b> before calling this function. Any other value
 * will suppress the Flash write.
 * 
 * If the flash already contains the correct data in the specified addresses, or
 * if it is not possible to write the correct data (i.e. requires clearing a 
 * preexisting '0' back to '1'), then the Flash write for the given word (24 or 
 * 48-bits, depending on device) is skipped. srcData must be in RAM and must be 
 * writable. The data can change (changes to 0xFFFFFF or 0xFFFFFFFFFFFF anywhere 
 * a Flash word cannot or does not need to be programmed).
 *
 * @param flags
 *   Bitwise OR'd collection of flags to modify the writing behavior. Available
 *   options are:
 *      EZBL_FLAG_CLRWDT (0x0400)           - Clear watchdog before each write
 *      EZBL_FLAG_LATE_INT_EN (0x0800)      - Restore interrupts only after NVMOP completes. By default interrupts are disabled only for the unlock sequence that sets NVMCON<WR>.
 *      EZBL_FLAG_IPARTITION (0x1000)       - Force 0x400000-0x7FFFFF Inactive Partition address range (set bit 22 of destProgAddr)
 *      EZBL_FLAG_FIXED_PARTITION (0x2000)  - Use fixed Partition 1/Partition 2 target address range for destProgAddr instead of Active/Inactive relative addresses (XOR bit 22 of destProgAddr with !NVMCON<10>, which is !P2ACTIV on Dual Partition capable devices)
 */
 ; unsigned long EZBL_WriteROMOnlyOnceEx(unsigned long destProgAddr, void *srcData, unsigned int byteCount, unsigned int flags, int extraNoProgRanges, unsigned long *noProgRanges);
 ; unsigned long EZBL_WriteROMOnlyOnce  (unsigned long destProgAddr, void *srcData, unsigned int byteCount);
    .pushsection    .text.EZBL_WriteROMOnlyOnce, code
    .global         _EZBL_WriteROMOnlyOnce
    .type           _EZBL_WriteROMOnlyOnce, @function
    .global         _EZBL_WriteROMOnlyOnceEx
    .type           _EZBL_WriteROMOnlyOnceEx, @function
    .extern         NVMADR
    .weak           NVMADR
    .extern         TBLPAG
    .extern         SR
    .extern         _EZBL_WriteROMEx
    .extern         _EZBL_WriteROM

    .equ    EZBL_FLAG_CLRWDT,           10
    .equ    EZBL_FLAG_LATE_INT_EN,      11
    .equ    EZBL_FLAG_IPARTITION,       12
    .equ    EZBL_FLAG_FIXED_PARTITION,  13
    .equ    _UNALIGNED_WRITE,           15  ; internal only temporary flag

    reset           ; Guard against possible function entry from PC accidentally falling into executing this function (ex: operating outside Vdd/Clock speed allowable region and device is mis-executing occasional instructions)
_EZBL_WriteROMOnlyOnce:         ; unsigned long EZBL_WriteROMOnlyOnce  (unsigned long destProgAddr, void *srcData, unsigned int byteCount);
    mul.uu      w4, #0, w4      ;                                                     w1:w0               w2                    w3                      w4         w5                                w6
_EZBL_WriteROMOnlyOnceEx:       ; unsigned long EZBL_WriteROMOnlyOnceEx(unsigned long destProgAddr, void *srcData, unsigned int byteCount, unsigned int flags, int extraNoProgRanges, unsigned long *noProgRanges);
    ; Do address correction for flags
    btst        w1, #7          ; Don't force address bit 22 if we are targetting 0x800000-0xFFFFFF
    bra         NZ, 1f
    btst        w4, #EZBL_FLAG_FIXED_PARTITION
    bra         Z, 0f
    btsc        NVMCON, #10     ; NVMCON<10> has to be P2ACTIV bit on Dual Partition devices
    btg         w1, #6
0:  btsc        w4, #EZBL_FLAG_IPARTITION
    bset        w1, #6         ; Make address >= 0x400000

    ; Mask off non-erased regions and pass to write routine
1:  push.d      w0      ; Save original+flag modified dest address
    push.d      w2      ; Save original w2 = source pointer, w3 = byteCount
    push        TBLPAG
    mov         w1, TBLPAG

    ; Test if 0 bytes to write
    cp0         w3
    bra         Z, 9f

    ; Compute how many bytes/iteration there are to test, and save @ TOS
    mov         #6, w7
    sub         w7, #3, [w15++]
    mov         #NVMADR, w6
    cp0         w6
    bra         Z, 1f

    mov         w7, [w15-2]
    btsc        w0, #1                  ; Check if we need to consider input data that occurs before the given write address, but on the same flash program double word
    bset        w4, #_UNALIGNED_WRITE
    bclr        w0, #1

    ; Do not try to read from flash if NVM erase/write in progress
1:  bclr        w0, #0                  ; Don't let any odd dest address propagate forward
0:  btsc        NVMCON, #15
    bra         0b

loopStart:
    mov         [w15-2], w7 ; Get bytes/flash word
    setm        w1          ; Read 24-bits from Flash
    tblrdh.b    [w0], w1
    tblrdl      [w0], [w15++]
    and         w1, [--w15], [w15++]
    inc2        w0, w0

    ; See if another 24-bits need to be read and tested for this same Flash word
    cp          w7, #6
    bra         NZ, 4f

    ; 48-bit Flash Word: Need to test another instruction
    tblrdh.b    [w0], w1            ; High byte still has 0xFF in it from 3b
    and         w1, [--w15], [w15++]
    tblrdl      [w0], w1
    and         w1, [--w15], [w15++]
    inc2        w0, w0

    ; Advance pointers, check for 64K TBLPAG rollover, and decrement length by one Flash word
4:  cp0         w0
    btsc        SR, #SR_Z
    inc         TBLPAG

    ; Adjust this word's byte count needed for incrementing, if needed
    btsc        w4, #_UNALIGNED_WRITE
    sub         w7, #3, w7
    bclr        w4, #_UNALIGNED_WRITE

    ; Then, test the accumulated data from the read Flash word.
    com         [--w15], [w15]  ; Test accumulated data
    bra         NZ, 5f          ; Fail erase check, mask input data

 1: add         w2, w7, w2  ; Increment data pointer by bytes/word
    sub         w3, w7, w3  ; Decrement data length by bytes/word
    bra         LEU, 8f     ; Terminate if we borrow or reach zero
    bra         loopStart

    ; Failed erased check - mask data
5:  setm.b      [w2++]
    dec         w3, w3      ; Dec RAM bytes available
    bra         Z, 8f
    dec         w7, w7      ; Dec bytes per flash word
    bra         NZ, 5b
    bra         loopStart   ; Masked all data bytes in this word and Check next word

    ; Done checking, restore parameters and go do the properly masked write
8:  dec2        w15, w15    ; Remove temporary bytes/word save value
9:  pop         TBLPAG
    pop.d       w2          ; Reobtain original source data ptr + source len
    pop.d       w0          ; Reobtain original dest address
    goto        _EZBL_WriteROMEx
    .size       _EZBL_WriteROMOnlyOnceEx, . - _EZBL_WriteROMOnlyOnce + 2
    .size       _EZBL_WriteROMOnlyOnce,   . - _EZBL_WriteROMOnlyOnce + 2
    .popsection
