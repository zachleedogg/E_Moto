;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Library Source File
;
;  Summary:
;    Assembly language optimized EZBL Library functions
;
;  Description:
;    Assembly language optimized ezbl_lib library API functions. See function
;    documentation below.
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

	.equ	SR_C, 0x0	; SR<C> is bit 0
	.equ	SR_Z, 0x1	; SR<Z> is bit 1
	.equ	SR_OV, 0x2	; SR<OV> is bit 2
	.equ	SR_N, 0x3	; SR<N> is bit 3
    .macro  skip_next_instr
    btsc    w15, #0     ; Skips next instruction since w15 stack pointer can never be odd
    .endm
    .ifdef  ezbl_lib16ch    ; __GENERIC-16DSP-CH target, but can't test this symbol since it has dashes in it
    .equ    DIV_CYCLES, #5
    .else
    .equ    DIV_CYCLES, #17
    .endif

/*
    typedef struct tagEZBL_MISMATCH_STATUS
    {
        unsigned long pgmAddress;   // Program space address starting the mismatch; aligned to instruction word boundaries.
        void *dataAddress;          // Pointer to data space corresponding to the program space address that mismatched. The value returned will always point to the low byte of the 24-bit instruction word that triggered comparison mismatch, even if the true byte that mismatched is in the middle or upper byte locations of the program word.
        unsigned long pgmData;      // 24-bit instruction word that was read from Flash/program space that caused the mismatch. Bits 24-31 of this value are always 0x00.
        unsigned long compareData;  // 24-bits (or less) of RAM data that was read from the *cmpData pointer corresponding to the mismatch. When less than 24-bits of compare data were provided, the middle/upper bytes of compare data is returned as 0xFF padding bytes. Bits 24-31 of this value are always 0x00.
    } EZBL_MISMATCH_STATUS;

    typedef enum tagEZBL_COMPARE_FLAGS
    {
        EZBL_FLAG_EQUALS = 0x0000,          // Evaluate binary equivalence where 1's must equal 1's and 0's must equal 0's.
        EZBL_FLAG_ZEROS = 0x0001,           // Evaluate equivalence only considering 0's in RAM must be 0's in program space. 1's in RAM are don't care positions in Flash.
        EZBL_FLAG_ONES  = 0x0002,           // Evaluate equivalence only considering 1's in RAM must be 1's in program space. 0's in RAM are don't care positions in Flash.
        EZBL_FLAG_16BIT = 0x0004,           // Evaluate equivalence only considering the lower 16-bits of each 24-bit instruction word. Mismatches in the high byte are read, but ignored.
        EZBL_FLAG_8BIT  = 0x0008,           // Evaluate equivalence only considering the lower 8-bits of each 24-bit instruction word. Mismatches in the high bytes are read, but ignored.
// NOT IMPLEMENTED -       EZBL_SINGLE_CMP_VAL = 0x0010,       // *cmpData pointer points to a single 24-bit value to compare against and the same value will be used repeatedly for all program space instruction words read. Value @ pointer should be 0xFFFFFF to do an all '1's blank check.
        EZBL_FLAG_IPARTITION = 0x1000,
        EZBL_FLAG_FIXED_PARTITION = 0x2000
    } EZBL_COMPARE_FLAGS;
*/


/**
 * Byte-wise compares Flash/program memory to a data array, returning the first
 * difference encountered. This function is similar to the memcmp() C stdlib
 * function (and EZBL_RAMCompare()), but takes a 23-bit Program memory
 * address/symbol value as one of the input arguments instead of two RAM
 * pointers.
 *
 * @param pgmAddress
 *      Unsigned long program memory address for the first array to read from.
 *      This must be an even aligned value. The value 0x000000 means program
 *      Flash address 0x000000 and is not a null value. Data is read from Flash
 *      in packed form (3-bytes per 0x2 addresses and not a dummy zero in every
 *      4th byte location).
 *
 *      No address checking is done to confirm validity of the program address.
 *      Attempting to read from an unimplemented Flash address will normally
 *      result in a Address Error Trap.
 *
 * @param *cmpData Pointer to a byte array, located in RAM or data space
 *                 (including PSV/EDS) to compare against.
 *
 * @param length Maximum number of bytes to compare before returning.
 *
 * @param flags A bitwise OR of the following flag constants:
 *    <ul>
 *      <li>EZBL_FLAG_EQUALS (0x0000): Compare for exact binary equality. All
 *                  bits must match.
 *                  <br>Logically, this tests cmpData - pgmData == 0.</li>
 *      <li>EZBL_FLAG_ZEROS (0x0001): Compare cleared bits only. All '0' bits in
                    *cmpData must have a '0' bit in the corresponding program
 *                  space cell, whereas all '1' bits in *cmpData are don't care
 *                  bits in Flash and can read back as either '1' or '0' and
 *                  still be a match.
 *                  <br>Logically, this tests, (~cmpData) & pgmData == 0.</li>
 *      <li>EZBL_FLAG_ONES (0x0002): Compare set bits only. All '1' bits in
                    *cmpData must have a '1' bit in the corresponding program
 *                  space cell, whereas all '0' bits in the cmpData array are
 *                  don't care values in the Flash.
 *                  <br>Logically, this tests, (~cmpData) | pgmData == 0.</li>
 *      <li>EZBL_FLAG_16BIT (0x0004): Check only lower 16-bits of each
 *                  instruction word. Bits 16-23 of each program word are don't
 *                  care values on read-back, but there must still be dummy
 *                  equivalent cmdData locations for them. The lower 16-bits are
 *                  tested according to the binary EQUALS/ZEROS/ONES matching
 *                  flags.</li>
 *      <li>EZBL_FLAG_8BIT (0x0008): Check only lower 8-bits of each instruction
 *                  word. Bits 8-23 of each program word are don't care values
 *                  on read-back, but there must still be dummy equivalent
 *                  cmpData locations for them. The lower 8-bits are tested
 *                  according to the binary EQUALS/ZEROS/ONES matching flags.</li>
 *      <li>EZBL_FLAG_IPARTITION (0x1000): Force 0x400000-0x7FFFFF Inactive
 *                  Partition address range (set bit 22 of pgmAddress). The
 *                  next/mismatch return addresses will also reflect the
 *                  inactive partition range.</li>
 *      <li>EZBL_FLAG_FIXED_PARTITION (0x2000): Use fixed Partition 1/Partition
 *                  2 address range for pgmAddress instead of Active/Inactive
 *                  relative addresses (XOR bit 22 of pgmAddress with
 *                  !NVMCON<10>, which is !P2ACTIV on Dual Partition capable
 *                  devices).
 *
 *                  The mismatch status program address will reflect the decoded
 *                  address range that was actually compared against.</li>
 *    </ul>
 *
 * @param *mismatchStatus
 *              Pointer to a caller allocated EZBL_MISMATCH_STATUS structure to
 *              receive extended comparison mismatch status data. Set to null if
 *              unneeded.
 *
 *              Values returned should be ignored if EZBL_ROMCompareEx() returns
 *              0, indicating a match.
 *
 * @return 0x0000 if both arrays have matching contents. A non-zero value is
 *         returned if a mismatch is detected.
 *
 *         If <i>length</i> is zero, a 0x0000 matching return value is always
 *         returned without reading from either memory. Unless null,
 *         *mismatchStatus may still be written to with dummy data.
 */
    .pushsection    .text.EZBL_ROMCompare, code
    .global         _EZBL_ROMCompare
    .global         _EZBL_ROMCompareEx
    .type           _EZBL_ROMCompare, @function
    .type           _EZBL_ROMCompareEx, @function
    .extern         TBLPAG

;   0x1000 - EZBL_FLAG_IPARTITION      - Force 0x400000-0x7FFFFF Inactive Partition address range (set bit 22 of flashWriteAddress)
;   0x2000 - EZBL_FLAG_FIXED_PARTITION - Use fixed Partition 1/Partition 2 target address range for flashWriteAddress instead of Active/Inactive relative addresses (XOR bit 22 of flashWriteAddress with !NVMCON<10>, which is !P2ACTIV on Dual Partition capable devices)
    ;.equ    EZBL_FLAG_EQUALS            N/A;0x0000
    .equ    EZBL_FLAG_ZEROS,             0
    .equ    EZBL_FLAG_ONES,              1
    .equ    EZBL_FLAG_16BIT,             2
    .equ    EZBL_FLAG_8BIT,              3
    .equ    EZBL_FLAG_IPARTITION,       12
    .equ    EZBL_FLAG_FIXED_PARTITION,  13
_EZBL_ROMCompare:   ; int EZBL_ROMCompare (unsigned long pgmAddress, const void *cmpData, unsigned int length);

    mul.uu      w4, #0, w4
_EZBL_ROMCompareEx: ;int EZBL_ROMCompareEx(unsigned long pgmAddress, const void *cmpData, unsigned int length, int flags, EZBL_MISMATCH_STATUS *mismatchStatus);
                    ;                                    w1:w0                   w2                    w3          w4                           w5
    push.d      w8              ; w9:w8 going to be used for program space data
    push.d      w10             ; w11:w10 temporaries
    push        TBLPAG

    cp0         w5              ; If *mismatchStatus is null pointer, then set to stack pointer so we can always dump data there without testing it
    btsc        SR, #SR_Z
    mov         w15, w5
    add         w5, #4, w5      ; w5 = pointer to mismatchStatus->dataAddress (true if on the stack or not)

    btst        w1, #7          ; Don't force address bit 22 if we are targetting 0x800000-0xFFFFFF
    bra         NZ, 1f
    btst        w4, #EZBL_FLAG_FIXED_PARTITION
    bra         Z, 0f
    btsc        NVMCON, #10     ; NVMCON<10> has to be P2ACTIV bit on Dual Partition devices
    btg         w1, #6
0:  btsc        w4, #EZBL_FLAG_IPARTITION
    bset        w1, #6         ; Make address >= 0x400000

1:  mov         w1, TBLPAG
    clr         w1
0:  btsc        NVMCON, #15
    bra         0b

    bra         nextCompare
nextCompareWithAddrInc:
    inc2        w0, w0
    btsc        SR, #SR_C
    inc         TBLPAG
nextCompare:
    dec         w3, w3
    bra         NC, done

    mov         w2, [w5++]                      ; Save void *dataAddress
    ; Read up to 3 RAM bytes
    mov         #WREG6+1, w1
    mov         #0xFFFF, w6
    mov         #0x00FF, w7
    mov.b       [w2++], w6
    dec         w3, w3
    bra         NC, readROMBytesMask2
    mov.b       [w2++], [w1]
    dec         w3, w3
    bra         NC, readROMBytesMask1
    mov.b       [w2++], w7
    bra         readROMBytes

readROMBytesMask2:
    bset        w4, #EZBL_FLAG_8BIT
readROMBytesMask1:
    bset        w4, #EZBL_FLAG_16BIT
readROMBytes:
    tblrdl      [w0], w8            ; w9:w8 = 24-bit program word
    tblrdh      [w0], w9
    mov.d       w8, [w5++]          ; Save unsigned long pgmData
    mov.d       w6, [w5++]          ; Save unsigned long compareData
    and         w4, #0x3, [w15]     ; 0x3 = 1<<EZBL_FLAG_ZEROS | 1<<EZBL_FLAG_ONES
    bra         Z, compare          ; Testing ordinary exact match
    com         w6, w6
    com.b       w7, w7
    and         w6, w8, w10
    and         w7, w9, w11
    btsc        w4, #EZBL_FLAG_ZEROS; Testing only all ram '0's match Flash?
    mov.d       w10, w6             ; result = pgmData & ~compareData
    ior         w6, w8, w10         ; w11:w10 = pgmData | ~compareData;
    ior         w7, w9, w11
    btsc        w4, #EZBL_FLAG_ONES ; Test only all RAM '1's match Flash?
    mov.d       w10, w6
    mul.uu      w8, #0, w8          ; w9:w8 = 0x0000_0000

compare:    ; and do masking
    sub         w6, w8, w6
    subb        w7, w9, w7
    and         w4, #0xC, [w15]     ; Need to do any result masking? 0xC = 1<<EZBL_FLAG_16BIT | 1<<EZBL_FLAG_8BIT
    bra         Z, 0f
    clr         w7                  ; Mask off bits 16-23
    btsc        w4, #EZBL_FLAG_8BIT
    and         #0xFF, w6           ; Mask off bits 8-15
0:  sub         w5, #10, w5         ; Reset *mismatchStatus pointer to mismatchStatus->dataAddress
    ior         w6, w7, w1          ; Do final check of all bits to confirm they are zero (match)
    bra         Z, nextCompareWithAddrInc

done:
    mov         TBLPAG, w6
    mov         w6, [--w5]          ; Save pgmAddress high bits
    mov         w0, [--w5]          ; Save pgmAddress low bits
    mov         w1, w0
    pop         TBLPAG              ; Restore TBLPAG
    pop.d       w10
    pop.d       w8
    return

    .size       _EZBL_ROMCompare, . - _EZBL_ROMCompare
    .popsection
    