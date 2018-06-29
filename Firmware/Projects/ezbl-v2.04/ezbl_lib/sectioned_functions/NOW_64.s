/*******************************************************************************
 *
 * Time Counting and Measurement Functions of NOW library (part of EZBL)
 *
 * Includes routines for measuring absolute time, polled task triggering and
 * measuring oscillators such as your Fast internal RC oscillator
 * (FRC), Digitally Controlled Oscillator (DCO), or other clock source.
 *
 *******************************************************************************
 * FileName:        NOW_64.s
 * Dependencies:    Timer 1 (or other 16-bit timer/MCCP/SCCP as defined by
 *                  run-time NOW_timerSFRPtr)
 * Processor:       PIC24, dsPIC33
 * Compiler:        Microchip XC16 v1.26 or higher
 * Company:         Microchip Technology, Inc.
 * Author:          Howard Schlunder
*******************************************************************************/

/*******************************************************************************
  Copyright (C) 2017 Microchip Technology Inc.

  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
  derivatives created by any person or entity by or on your behalf, exclusively
  with Microchip's products.  Microchip and its licensors retain all ownership
  and intellectual property rights in the accompanying software and in all
  derivatives here to.

  This software and any accompanying information is for suggestion only.  It
  does not modify Microchip's standard warranty for its products.  You agree
  that you are solely responsible for testing the software and determining its
  suitability.  Microchip has no obligation to modify, test, certify, or
  support the software.

  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
  TERMS.
*******************************************************************************/


/**
 * Atomically returns the least significant 32 bits of the current NOW counter
 * value (for NOW_32() calls) or the full 64-bits of the NOW counter (for
 * NOW_64() calls).
 *
 * This function is safe to be called from any IPL or ISRs with the return value
 * still being valid (i.e. reentrancy is allowed). When called from a higher
 * priority ISR or trap handler which blocks the _TxInterrupt() ISR from
 * executing, this function will automatically revert to a polling mode where
 * the _TxInterrupt() will be called as needed to ensure proper 16-bit carry
 * propgataion into bits 16 to 63. This ensures that all calls to
 * NOW_32()/NOW_64() will always be chronologically monotonic. I.e. this
 * function should never return a count that represents an earlier time than any
 * previous calls to NOW_32()/NOW_64() made within the last 2^32 counts (or 2^64
 * counts for NOW_64())
 *
 * Execution time is a fixed 21 or 27 cycles best (and normal) case, including
 * the call and return branches, depending on device. However, if you are very
 * unlucky and the lower 16-bits rolls over during the read (less than 0.01%
 * chance if no interrupt occurs), execution time will grow for a bounded, but
 * non-deterministic time.
 *
 * Both NOW_64() and NOW_32() are implemented as the same function with the C
 * compiler ignoring the upper 32-bits when NOW_32() is called instead of
 * NOW_64().
 *
 * @return 64 or 32-bit NOW counter value, where 1 NOW count is normally equal
 *         to 1 instruction cycle. Use the NOW_second, NOW_millisecond,
 *         NOW_microsecond variable contents, or the NOW_Diff_to_ms() and
 *         NOW_Diff_to_us() functions in order to make physical sense of how
 *         long a NOW count is at run time.
 *
 *         NOTE: the NOW_32() function overflows on the timescale of ~minute
 *         Ex: at 70MIPS, the longest interval you could correctly measure is
 *         61.356 seconds. Use the NOW_64() function if longer durations are
 *         being measured.
 */
    .pushsection .text.NOW_64, code
    .global _NOW_32
    .global _NOW_64
    .type   _NOW_32, @function
    .type   _NOW_64, @function
    .weak   _NOW_timerSFRPtr    ; extern volatile unsigned int * XC16_NEAR __attribute__((persistent)) NOW_timerSFRPtr; NOTE: Declared weak so we don't include the wrong NOW_[Tx/CCPx]_Reset.c implementation.
    .weak   _NOW_timeOffset     ; extern volatile unsigned int __attribute__((persistent)) NOW_timeOffset;
    .weak   _NOW_internalCount
    .equ    countPtr, 6
    .weak   _NOW_ISR            ; Don't actually want to have this weak, but also don't want to force ezbl_lib.a searching, which could find the wrong ISR definition. Instead, NOW_Init() choosing which hardware timer to use but enforce the correct ISR to be included, which exports _NOW_ISR as a linkable symbol.
    .extern _IFS0
    .weak   _NOW_IFSReg
    .weak   _NOW_IFSBit
    .extern _EZBL_CallISR
_NOW_32:    ; unsigned long NOW_32(void);
_NOW_64:    ; unsigned long long NOW_64(void);
    mov     _NOW_timerSFRPtr, w7            ;// Get &TMRx
    clr     w6
0:  mov     _NOW_timeOffset, w5             ;// w5 = NOW_timeOffset
    mov     _NOW_internalCount+countPtr, w4 ;// w4 = &NOW_internalCountPing or &NOW_internalCountPong
    subr    w5, [w7],   w0                  ;// w0 = TMRx SFR value - NOW_timeOffset
    subbr   w6, [w4++], w1                  ;// w1 = NOW_internalCount[15:0]  - 0 - Borrow
    subbr   w6, [w4++], w2                  ;// w2 = NOW_internalCount[31:16] - 0 - Borrow
    subbr   w6, [w4],   w3                  ;// w3 = NOW_internalCount[47:32] - 0 - Borrow
    mov     _NOW_internalCount+countPtr, w5 ; Now check if someone interrupted us and toggled the count read pointer while we were busy using it
    sub     w4, #4, w4
    cp      w4, w5
    bra     NZ, 0b                          ; Start over - read may not be atomically valid

    mov     #_IFS0, w4                      ; Now check if someone has masked the Timer interrupt and an ISR increment should have happened
    add     #_NOW_IFSReg, w4
    btss    [w4], #_NOW_IFSBit
    return                                  ; Nope, no interrupt pending -> we are good to go
    mov     #handle(_NOW_ISR), w0           ; Call the ISR at the current IPL if the IFSybits<TxIF> flag is set (meaning this NOW_32()/NOW_64() function was called at an IPL >= the timer or someone has the interrupt disabled)
    call    _EZBL_CallISR                   ; NOTE: calling the ISR can also dispatch the NOW_TaskDispatcher() routine and any pending tasks. Also note that EZBL_CallISR() is needed since the ISR executes a retfie to return, while we need it to return normally.
    bra     _NOW_64

    .size   _NOW_32, . - _NOW_32
    .size   _NOW_64, . - _NOW_64
