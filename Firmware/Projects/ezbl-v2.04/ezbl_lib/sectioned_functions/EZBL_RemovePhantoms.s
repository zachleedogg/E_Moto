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
 * Copies or rewrites a byte array, removing every 4th byte from the source
 * array. This function is typically used to remove 0x00 "phantom" bytes after
 * every 3 useful bytes.
 *
 * To do the reverse and add phantom bytes, see EZBL_AddPhantoms().
 *
 * @param *dest RAM pointer to write the packed byte array without phantom
 *              padding bytes. The size of this buffer must be at least
 *              (srcDataAddrLen/2*3) bytes large to hold the output.
 *
 *              If dest == srcData, the data will be converted in place. If
 *              dest > srcData with overlap, the output is undefined.
 *              dest < srcData with overlap is accepted.
 *
 * @param *srcData Aligned 32-bit long int array containing the source data that
 *                 will be stripped of upper bytes.
 *
 * @param srcDataAddrLen Size of the srcData array, in program addresses. This
 *                       is equivalent to exactly half of the byte count of
 *                       srcData. This value should always be even as the input
 *                       array is specified in multiples of 4 bytes each.
 *
 * @return (srcDataAddrLen/2*3), which is the number of bytes written to *dest.
 */
;unsigned int EZBL_RemovePhantoms(void *dest, unsigned long *srcData, unsigned int srcDataAddrLen);
    .pushsection    .text.EZBL_RemovePhantoms, code
    .global         _EZBL_RemovePhantoms
    .type           _EZBL_RemovePhantoms, @function
_EZBL_RemovePhantoms:
    lsr     w2, w2
    mov     w2, w7              ; Save word count so we can compute return value (bytes written to dest)
    bra     testRemaining

convertWord:
    mov.b   [w1++], [w0++]
    mov.b   [w1++], [w0++]
    mov.b   [w1++], [w0++]
    inc     w1, w1
testRemaining:
    dec     w2, w2
    bra     C, convertWord

done:
    mul.uu      w7, #3, w0
    return
    
    .size           _EZBL_RemovePhantoms, . - _EZBL_RemovePhantoms
    .popsection
