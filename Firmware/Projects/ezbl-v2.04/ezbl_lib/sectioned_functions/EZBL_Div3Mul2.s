;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Library Source File
;
;  Summary:
;    Assembly language optimized helper routines required by the EZBL core API
;
;  Description:
;    Assembly language optimized helper routines required by the EZBL core API
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

    .equ    SR_C, 0x0   ; SR<C> is bit 0
    .equ    SR_Z, 0x1   ; SR<Z> is bit 1
    .equ    SR_OV, 0x2  ; SR<OV> is bit 2
    .equ    SR_N, 0x3   ; SR<N> is bit 3

    .ifdef  ezbl_lib16ch    ; __GENERIC-16DSP-CH target, but can't test this symbol since it has dashes in it
    .equ    DIV_CYCLES, #5
    .else
    .equ    DIV_CYCLES, #17
    .endif


/**
 * Multiplies a 16-bit unsigned integer number by 2/3rds and optionally returns
 * the remainder of the divide by 3 operation (effectively before the multiply
 * by 2). Normally, this function is needed when converting a byte count into a
 * Flash address increment. Each Flash address increment is '2', which represents
 * '3' data bytes. If there are any remainder bytes that cannot fill a whole
 * Flash address increment of 2, then the remainder bytes can be returned via the
 * *remainder pointer. Specify a null pointer if the remainder is not needed.
 *
 * Requires 32 instruction cycles (on dsPIC33E/PIC24E) or 27 cycles (on
 * PIC24/dsPIC33F), including the return, but not the call.
 *
 * @param byteToAddrNumber unsigned 16-bit integer to be multiplied by 2 and
 *                         divided by 3. If your input can be larger than
 *                         16-bits, use the EZBL_Div3Mul2L() function
 *                         instead.
 *
 * @param *remainder    Optional pointer to an unsigned 16-bit integer to store
 *                      the division by 3 remainder. This parameter can be null
 *                      (0x0000) if the remainder is not needed. The remainder
 *                      is computed after the multiplication by 2 has already
 *                      been done to yield perfect precision. Ex:
 *                      1 * (2/3) = 0 Remainder 1, 2 * (2/3) = 1 Remainder 1,
 *                      3 * (2/3) = 2 Remainder 0, 4 * (2/3) = 2 Remainder 2,
 *                      etc.
 *
 * @return unsigned 16-bit integer result of the computation where the remainder
 *         (if any) is truncated off. If the remainder is needed, use the
 *         *remainder input option to retrieve it.
 */
 ; unsigned int EZBL_Div3Mul2(unsigned int byteToAddrNumber, unsigned int *remainder);

    .pushsection    .text.EZBL_Div3Mul2, code
    .global         _EZBL_Div3Mul2
    .type           _EZBL_Div3Mul2, @function
    .extern         SR
_EZBL_Div3Mul2:
    mov     w1, w2      ; w2 = *remainder pointer
    mov     #3, w3      ; Divide byteToAddrNumber by 3
    repeat  #DIV_CYCLES ;   w0 = program address offset scaled by 1/2
    div.u   w0, w3      ;   w1 = /3 remainder
    sl      w0, w0      ; Multiply whole portion of result by 2
    cp0     w2          ; Store remainder if non-null pointer provided
    btss    SR, #SR_Z
    mov     w1, [w2]
    return
    
    .size           _EZBL_Div3Mul2, . - _EZBL_Div3Mul2
    .popsection
