;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Source File
;
;  Summary:
;    Assembly language optimized memcpy() replacement function
;
;  Description:
;    Assembly language optimized memcpy() replacement function that can perform
;    better at run time by selectively performing word moves instead of byte
;    mode only moves.
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


/**
 * void EZBL_RAMCopy(void *dest, const void *source, unsigned int length);
 * void *EZBL_RAMCopyEx(void *dest, const void *source, unsigned int length);
 *
 * Copies RAM efficiently, much like memcpy() but with run-time auto-detect of 
 * even aligned source and destination pointers for switch over to word-wise 
 * higher speed copying.
 *
 * Copy performance should be (14 + 1/Byte) instruction cycles for byte mode on
 * dsPIC33E/dsPIC33D/PIC24E devices, including call and return time. For aligned
 * pointers, performance should be (~22 + 0.5/Byte) instruction cycles. Exact 
 * performance may differ if an inline __builtin_memcpy() repeat mov loop is 
 * generated instead.
 * 
 * @param *dest RAM/SFR pointer to write the copied data to. Can be byte
 *              aligned, which will force a byte-wise copy.
 *
 *              This pointer cannot be NULL (0x0000) except when <i>length</i>
 *              is 0.
 *
 *              This pointer may overlap with the *source pointer memory range,
 *              in which case, the copy operation will proceed starting at the
 *              first dest/source byte (or word!) and proceed to the end with
 *              each byte/word being written before the next is read.
 *
 * @param *source SFR/RAM or PSV/Flash pointer to read data from. Can be byte
 *                aligned, which will force a byte-wise copy.
 *
 *                This pointer cannot be NULL (0x0000) except when <i>length</i>
 *                is 0.
 *
 * @param length Number of bytes to copy.
 *    
 *               This value must be <= 16384 on dsPIC30, PIC24F, PIC24H, or
 *               dsPIC33F families (or <= 32768 when dest and source are both
 *               even aligned). 0 is allowed, resulting in no operation.
 *
 *               When the dest and source pointers are both even/word aligned
 *               length still represents a total byte count even though the copy
 *               will proceed using word moves. If length is not a word count
 *               integral, the final element will be moved with a byte 
 *               operation.
 *
 *               If this value is a compile-time constant, the EZBL_RAMCopy() 
 *               call is substituted with a __builtin_memcpy() function call, 
 *               which normally evaluates to a repeat loop with a mov.b 
 *               operation.
 * 
 * @return EZBL_RAMCopy() - void
 *         EZBL_RAMCopyEx() - (void*) pointer, which is *dest incremented by 
 *                            length bytes. The increment is still in bytes, 
 *                            even if a faster word move operation was executed.
 */
	.pushsection    .text.EZBL_RAMCopy, code
	.global         _EZBL_RAMCopy
	.global         _EZBL_RAMCopyEx
    .type           _EZBL_RAMCopy, @function
    .type           _EZBL_RAMCopyEx, @function
_EZBL_RAMCopy:
_EZBL_RAMCopyEx:
	dec     w2, w4      ; length == 0?
	bra     N, 7f
    ior     w0, w1, w3  ; *dest and *source both word aligned?
    btss    w3, #0
    bra     wordCopy

byteCopy:
    repeat  w4
    mov.b   [w1++], [w0++]
7:  return

wordCopy:
    lsr     w2, w3
    dec     w3, w3
    bra     N, 0f           ; Only possible when length == 1
    repeat  w3
    mov     [w1++], [w0++]
    btsc    w2, #0          ; Check if there is a residual byte we need to process
0:  mov.b	[w1++], [w0++]  ; Yes
    return                  ; No

    .size   _EZBL_RAMCopy, . - _EZBL_RAMCopy
    .size   _EZBL_RAMCopyEx, . - _EZBL_RAMCopyEx
    .popsection
