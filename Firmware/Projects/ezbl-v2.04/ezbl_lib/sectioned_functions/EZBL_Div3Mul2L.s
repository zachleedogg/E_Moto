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
 * Multiplies a 32-bit unsigned long number by 2/3rds and optionally returns the
 * remainder of the divide by 3 operation (effectively before the multiply by 2).
 * Normally, this function is needed when converting a byte count into a Flash
 * address increment. Each Flash address increment is '2', which represents '3'
 * data bytes. If there are any remainder bytes that cannot fill a whole Flash
 * address increment of 2, then the remainder bytes can be returned via the
 * *remainder pointer. Specify a null pointer if the remainder is not needed.
 * Multiplication and division is done iteratively with bit shifts.
 *
 * Requires ~570 instruction cycles (on dsPIC33E/PIC24E), up to about ~634 
 * maximum, including the return, but not the call. Fewer cycles are required on 
 * PIC24/dsPIC33F due to shorter by ~173 cycles (~397 total typically) due to 
 * branch delay reduction.
 *
 * @param byteToAddrNumber unsigned 32-bit integer to be multiplied by 2 and
 *                         divided by 3. If your input will always be 16-bits or
 *                         less, use the EZBL_Div3Mul2() function instead for
 *                         greater efficiency.
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
 * @return unsigned 32-bit long integer result of the computation where the
 *         remainder (if any) is truncated off. If the remainder is needed, use
 *         the *remainder input option to retrieve it.
 */
 ; unsigned long EZBL_Div3Mul2L(unsigned long byteToAddrNumber, unsigned int *remainder);
    .pushsection    .text.EZBL_Div3Mul2L, code
    .global         _EZBL_Div3Mul2L
    .type           _EZBL_Div3Mul2L, @function
    .extern         SR
_EZBL_Div3Mul2L:
    ; Remainders always toggle back and fourth between 2 and 1 for each input bit
    ; Adder starts at 0x2AAAAAAA and always shifts right by 1 bit for each input bit
    ; Ex: 2^31/3 is 715,827,882 R2 (0x2AAAAAAA + R2)
    ;     2^30/3 is 357,913,941 R1 (0x15555555 + R1)
    ;     2^29/3 is 178,956,970 R2 (0x0AAAAAAA + R2)
    push.d  w8
    mov     w2, w9      ; w9 = *remainder pointer
    clr     w8          ; w8 = remainder accumulator
    mov     #2, w7      ; w7 = 2-1-2-1 remainder toggler
    mov     #32, w6     ; w6 = bit loop counter
    mov     #0xAAAA, w4 ; w4:w5 = 0x2AAAAAAA
    mov     #0x2AAA, w5
    mov.d   w0, w2      ; w2:w3 = byteToAddrNumber
    mul.uu  w0, #0, w0  ; w0:w1 = output result, 0x00000000 right now to start

1:  sl      w2, w2      ; Check MSbit of number, which is shifting left into the carry bit
    rlc     w3, w3
    bra     NC, 2f      ; Skip addition in this cycle if MSbit is clear
    add     w0, w4, w0
    addc    w1, w5, w1
    add     w8, w7, w8  ; Add remainder to remainder acumulator

2:  lsr     w5, w5      ; Divide adder by 2
    rrc     w4, w4
    xor     w7, #0x3, w7; Toggle remainder from 2 to 1 or 1 to 2
    dec     w6, w6      ; Decrement and check loop counter
    bra     NZ, 1b

    bra     4f

    ; Add whole chunks of remainder accumulator (up to 16 whole chunks will exist)
3:  add     w0, #1, w0  ; Add 1 for the chunk of 3
    addc    w1, #0, w1
4:  sub     w8, #3, w8  ; Take a chunk of 3 away
    bra     C, 3b       ; Keep looping until remainder accumulator underflows

    add     w8, #3, w8  ; Undo remainder underflow
    cp0     w9          ; Store remainder if non-null pointer provided
    btss    SR, #SR_Z
    mov     w8, [w9]
    sl      w0, w0      ; Multiply whole result by 2 to complete the *2/3 operation
    rlc     w1, w1
    pop.d   w8
    return
    
    .size               _EZBL_Div3Mul2L, . - _EZBL_Div3Mul2L
    .popsection
