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
 * Converts the difference between two NOW counts to absolute time in
 * milliseconds. This function is primarily intended for instances where you
 * wish to display a time interval to a human in decimal forms. It is, however,
 * not subject to the integral approximations that NOW_millisecond represent, so
 * can be used in other cases when absolute accuracy is critical and the device
 * operating frequency is very low (ex: 32.768kHz).
 *
 * nowCounts must be measured against the same clock frequency in use during
 * invocation of this function. In other words, a wrong result will be returned
 * if your collect two NOW counts at one clock frequency, do a run-time clock 
 * switch, call NOW_SetTimeInterval(), and then pass the difference of the
 * original counts in for the nowCounts parameter.
 *
 * This function can be called from any CPU IPL and is reentrant capable.
 *
 * This function requires two 32x32 unsigned integer divide operations and
 * therefore requires a fair amount of execution time relative to most other
 * NOW APIs. If you just need to compare two timestamps where one value
 * represents the present and another value represents a constant timeout, it is
 * likely more efficient to use the NOW_second and NOW_millisecond variables.
 *
 * @param nowCounts The number of NOW counts to convert. Acceptable range is 0
 *                  to 536,870,911 counts (or up to 2,684 ms @ 200MHz; longer
 *                  for slower clock frequencies). Specifying
 *                  nowCounts > 536,870,911 will return an undefined value. The
 *                  function will safely succeed, however.
 *
 * @return 32-bit unsigned long representing complete milliseconds elapsed.
 *         Rounding is not performed; partial milliseconds are truncated off.
 */  
 ; unsigned long NOW_Diff_to_ms(unsigned long nowCounts);

    .pushsection .text.NOW_Diff_to_ms, code
    .global _NOW_Diff_to_ms
    .type   _NOW_Diff_to_ms, @function
    .weak   _NOW_sec
    .extern ___udivsi3
_NOW_Diff_to_ms:
    mul.uu  w0, #8, w2      ; Multiply time by 8 (and will divide clock rate by 125) to scale for milliseconds
    sl      w1, #3, w1
    push    w2
    ior     w1, w3, [w15++]
    mov     _NOW_sec+0, w0  ; Get 32-bit representation of a second
    mov     _NOW_sec+2, w1
    mov     #125, w2        ; Divide 32-bit clock rate by 125
    clr     w3
    call    ___udivsi3      ; Compute  (clock_freq/125) (must do 32/32 divide because 32/16 divide instruction is limited to 16-bits out)
    mov.d   w0, w2
    pop.d   w0
    call    ___udivsi3      ; Compute  (8*time)/(clock_freq/125)
    return
    
    .size   _NOW_Diff_to_ms, . - _NOW_Diff_to_ms
    .popsection
    