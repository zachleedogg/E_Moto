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
    .macro  skip_next_instr
    btsc    w15, #0     ; Skips next instruction since w15 stack pointer can never be odd
    .endm


/**
 * Copies a byte array, generating a 0x00 "phantom" padding byte after every 3
 * input bytes (24-bit instruction word). This function converts packed RAM data
 * into a format more suitable for Flash/program space use.
 *
 * The EZBL_RemovePhantoms() function can be used to reverse the processes,
 * which skips every 4th input byte.
 *
 * @param *dest RAM pointer to write the unpacked byte array with phantom bytes 
 *              added. The size of this buffer must be at least
 *              ((srcLen + 2)/3 * 4) bytes large to hold the output.
 *
 * @param *srcData RAM pointer to a byte array that has packed data (3 bytes per
 *                 0x2 program address, no 4th 0x00 "phantom" byte).
 *
 *                 If a srcLen is specified that does not end on a 24-bit
 *                 instruction word boundary (i.e. srcLen % 3 != 0), then 0xFF
 *                 filler bytes are generated in the output array (plus final
 *                 0x00 phantom byte) to make a complete instruction word.
 *
 * @param srcLen Number of bytes to unpack with phantom bytes at the *srcData
 *               location.
 *
 * @return ((srcLen + 2)/3 * 2), which is the number of program addresses now
 *         represented with the phantom bytes present. This is also exactly half
 *         of the number of bytes written to *dest, inclusive of the newly
 *         generated phantom bytes.
 */
 ; unsigned int EZBL_AddPhantomsEx(void *dest, void *srcData, unsigned int srcLen, unsigned int destPgmAddrLSbits);
 ; Same as EZBL_AddPhantoms(), but pads+aligns the output to the first program address boundary that can actually be written for the given device.
    .pushsection    .text.EZBL_AddPhantoms, code
    .global         _EZBL_AddPhantomsEx
    .global         _EZBL_AddPhantoms
    .type           _EZBL_AddPhantomsEx, @function
    .type           _EZBL_AddPhantoms, @function
_EZBL_AddPhantomsEx:    ; unsigned int EZBL_AddPhantomsEx(void *dest, void *srcData, unsigned int srcLen, unsigned int destPgmAddrLSbits);
    mov     w0, w5      ; w5 = *dest save value for computing number of program addresses written to dest
    mov     #NVMADR, w6
    cp0     w6
    bra     Z, noProblem
    btss    w3, #1
    bra     noProblem
    repeat  #2
    setm.b  [w0++]

noProblem:
    btsc    w3, #0
    setm.b  [w0++]
    skip_next_instr

_EZBL_AddPhantoms:   ; unsigned int EZBL_AddPhantoms(void *dest, void *srcData, unsigned int srcLen);
    mov     w0, w5      ; w5 = *dest save value for computing number of program addresses written to dest
    mov     #3, w7

newPhantom:
    mov     w7, w6
noPhantom:
    dec     w2, w2
    bra     NC, finalWord
    mov.b   [w1++], [w0++]
    dec     w6, w6
    bra     NZ, noPhantom
    clr.b   [w0++]
    bra     newPhantom

finalWord:
    cp      w6, #3
    bra     Z, done     ; Just wrote phantom, so done
0:  setm.b  [w0++]      ; Add 0xFF padding to fill the 24-bit instruction word
    dec     w6, w6
    bra     NZ, 0b
    clr.b   [w0++]      ; Now add final 0x00 phantom

done:
    sub     w0, w5, w0  ; Compute return value (number of program addresses written to *dest)
    lsr     w0, w0      ;   Divide by 2 to make this program addresses rather than bytes
    return

    .size           _EZBL_AddPhantoms, . - _EZBL_AddPhantoms
    .popsection
