;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Library Source File (ezbl_lib)
;
;  Summary:
;    Assembly language optimized EZBL APIs
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


/**
 * Reads consecutive instructions words (24-bits each) from Flash program space,
 * stores them as 32-bit unsigned longs in RAM, incrementing the read
 * address upon return.
 *
 * TBLPAG is internally saved, used and restored before return.
 *
 * Function blocks if an outstanding NVMCON<WR> operation is underway (dual
 * partition devices).
 *
 * The read address range can straddle 64K TBLPAG boundaries.
 *
 * @param *params Pointer to EZBL_UNPACKED_READER structure that contains the
 *                desired read address and memory target for reading sequential
 *                instruction words into. Each 24-bit instruction word is
 *                written as a 32-bit value with 0x00 filler in the upper 8
 *                bits.
 *
 *                Upon return, the params->readPgmAddr element has been
 *                post incremented to the next program address after the data
 *                returned. i.e. params->readPgmAddr += (destLen/4) * 0x2;
 *
 * @return  First instruction word contents from the read (lower 24-bits) and 
 *          0x00 padding in the upper 8-bits.
 *
 *          For read lengths of <= 3 bytes, all 32-bits of the first instruction
 *          word value are returned, even though nothing will be written to the
 *          *dest pointer and the readPgmAddr doesn't increment.
 *
 *          Destination addresses are written to with 0x00 padding in the upper
 *          byte of each 32-bit integer and 24-bits of data from flash in the
 *          lower 3 bytes. This includes the first unsigned long which is also
 *          the function return value.
 *
 *          The read program address is incremented in the params structure
 *          corresponding to the number of unsigned longs read/written (except
 *          for the <= 3 byte case).
 */
    .pushsection    .text.EZBL_ReadUnpacked, code
    .global         _EZBL_ReadUnpacked
    .global         _EZBL_ReadUnpackedInd
    .type           _EZBL_ReadUnpacked, @function
    .type           _EZBL_ReadUnpackedInd, @function
    .extern         TBLPAG
    .extern         NVMCON
    .extern         SR
_EZBL_ReadUnpacked:     ; unsigned long EZBL_ReadUnpacked(unsigned long *dest, unsigned long readPgmAddr, unsigned int destLen);
    ; w3:w2 = readPgmAddr (reordered by compiler)
    ; w0 = *dest
    ; w1 = destLen
    mov.d       w0, w4          ; w4 = *dest, w5 = destLen
    mov         #WREG4, w7      ; w7 = dummy *params structure write back pointer for discarding readPgmAddr back into w5:w4
    bra         directEntry

;typedef struct
;{
;    unsigned long readPgmAddr;  // Instruction word aligned read start address; Self increments by 0x2 addresses per instruction word read
;    unsigned long *dest;        // Aligned RAM pointer to write data to. Null valid only when destLen is <= 4.
;    unsigned int  destLen;      // Number of bytes of space at *dest to fill with program word data. A non-multiple of 4 will be processed only up to the last complete multiple of 4.
;} EZBL_UNPACKED_READER;
_EZBL_ReadUnpackedInd:  ; unsigned long EZBL_ReadUnpackedInd(EZBL_UNPACKED_READER *params);
    ; w0 = EZBL_UNPACKED_READER structure pointer
    add         w0, #8, w7      ; w7 = *params structure pointer for readPgmAddr write back
    mov.d       [--w7], w4      ; w4 = *dest, w5 = destLen
    mov.d       [--w7], w2      ; w3:w2 = readPgmAddr
directEntry:
    mov         TBLPAG, w6
    mov         w3, TBLPAG
0:  btsc        NVMCON, #15     ; Ensure no asynchronous Flash erase/write operations are in progress against the same Flash target. You cannot read from a Flash memory during programming and the CPU will not stall for completion unless you actually branch to it..
    bra         0b

    tblrdl      [w2], w0        ; Get return unsigned long
    tblrdh      [w2], w1
    bra         testLoop

nextRead:
    tblrdl      [w2], [w4++]
    tblrdh      [w2], [w4++]
    inc2        w2, w2
    addc        w3, #0, w3
    mov         w3, TBLPAG
testLoop:
    sub         w5, #4, w5
    bra         C, nextRead

done:
    mov.d       w2, [w7++]  ; Write back incremented readPgmAddr
    mov         w6, TBLPAG
    return
    
    .size   _EZBL_ReadUnpacked,    . - _EZBL_ReadUnpacked
    .size   _EZBL_ReadUnpackedInd, . - _EZBL_ReadUnpacked
    .popsection
