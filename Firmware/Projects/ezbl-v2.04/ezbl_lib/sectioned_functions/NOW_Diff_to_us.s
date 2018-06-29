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

    .ifdef  ezbl_lib16ch    ; __GENERIC-16DSP-CH target, but can't test this symbol since it has dashes in it
    .equ    DIV_CYCLES, #5
    .else
    .equ    DIV_CYCLES, #17
    .endif


/**
 * Converts a time difference in NOW counts to absolute time in microseconds.
 *
 * The time difference in NOW counts should be obtained by subtracting the
 * return value of NOW_16()/NOW_32()/NOW_64() from a prior value returned from
 * the same function.
 *
 * Use this when you wish to display a time interval in human decimal form.
 * This function is subject to less precision loss, as compared to dividing the
 * time difference by NOW_us, when the NOW timebase is only a few MHz or slower.
 *
 * This function can be called from any CPU IPL and is reentrant capable.
 *
 * This function requires one 32x16 unsigned integer hardware divide plus one
 * 32x32 unsigned software integer divide operation and therefore typically
 * requires a few microseconds of execution time.
 *
 * @param nowCounts
 *          The number NOW counts to convert. Supported range is 0 to 67,108,863
 *          counts (up to 335.544ms @ 200MHz; 958.697ms @ 70 MHz; > 4 seconds @
 *          16 MHz). Specifying > 67,108,863 will return an undefined value, but
 *          safely succeed.
 *
 * @return 32-bit unsigned long representing complete microseconds elapsed.
 *         Rounding is not performed; partial microseconds are truncated off.
 *
 *         To generate a rounded result, add (NOW_us/2u) to the nowCounts
 *         parameter before calling this conversion function. This is equivalent
 *         to adding 0.5 to the internal result, which when represented as an
 *         integer, results in fractionals of >= 0.5us being increased to +1us
 *         and fractions of < 0.5 being truncated down to 0.
 */
; unsigned long NOW_Diff_to_us(unsigned long nowCounts);
    .pushsection .text.NOW_Diff_to_us, code
    .global _NOW_Diff_to_us
    .type   _NOW_Diff_to_us, @function
    .weak   _NOW_sec
    .extern ___udivsi3      ; unsigned long __udivsi3(unsigned long numerator, unsigned long denominator); Remainder available in w5:w4.
_NOW_Diff_to_us:
    mov     #64, w2         ; Compute nowCounts*64 and NOW_sec/15625. These scaling factors are chosen because 64*15625 = 1,000,000, which is the correct scalar needed to compute microseconds. 64 allows a relatively large range for the numerator before overflow beyond 32-bits while 15625 offers supperior resolution against the demoniator compared to dividing it by 1M, which would result in 0 for clock rates of less than 1 MHz.
    mul.uu  w1, w2, w6
    mul.uu  w0, w2, w4
    mov     _NOW_sec+0, w0
    mov     _NOW_sec+2, w1
    mov     #15625, w2      ; Divide 32-bit clock rate by 15625.
    repeat  #DIV_CYCLES
    div.ud  w0, w2          ; w0 = (NOW_sec/15625), w1 = remainder
    mul.uu  w0, #1, w2      ; w3:w2 = denominator = {16'b0, (NOW_sec/15625) & 0xFFFF}
    mov     w4, w0
    add     w5, w6, w1      ; w1:w0 = numerator   = cycleDifference*64
    call    ___udivsi3      ; unsigned long __udivsi3(unsigned long numerator, unsigned long denominator); Remainder available in w5:w4. Computes (64*time)/(clock_freq/15,625).
    return

    .size   _NOW_Diff_to_us, .-_NOW_Diff_to_us
    .popsection
