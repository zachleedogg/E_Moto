/*******************************************************************************
 *
 * NOW_Wait() blocking real time function in ezbl_lib
 *
 ******************************************************************************/

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

    .equ    SR_C, 0x0   ; SR<C> is bit 0
    .equ    SR_Z, 0x1   ; SR<Z> is bit 1
    .equ    SR_OV, 0x2  ; SR<OV> is bit 2
    .equ    SR_N, 0x3   ; SR<N> is bit 3

/**
 * Waits the specified number of NOW counts in a blocking loop doing nothing.
 *   
 * Delays longer than 65535 NOW counts (~0.936ms @ 70 MIPS, 4.096ms @ 16
 * MIPS) will spend the time in Idle() power saving mode until <= 65535 NOW
 * counts remain.
 *
 * @param waitTime Number of NOW counts to wait. Use any fraction or multiple
 *                 of NOW_second, NOW_millisecond, NOW_microsecond variables to
 *                 specify absolute human time frames.
 *
 * @return NOW_32() return value (the present time, now stale by ~13
 *         instruction cycles).
 */
;unsigned long NOW_Wait(unsigned long waitTime);
;w1:w0                                w1:w0
    .pushsection .text.NOW_Wait, code
    .global _NOW_Wait
    .type   _NOW_Wait, @function
    .extern _NOW_32
_NOW_Wait:
    push.d  w8
    push.d  w10
    mov.d   w0, w8      ; w8 = waitTime
    call    _NOW_32
    mov.d   w0, w10     ; w10 = (startTime = NOW_32())
    clr     w3

0:  cp0     w3          ; > 65535 NOW counts of wait time remaining? If so, do Idle() power saving until <= 65535.
    btss    SR, #SR_Z
    pwrsav  #1      ; Idle

1:  call    _NOW_32
    sub     w0, w10, w4 ; NOW_32() - startTime
    subb    w1, w11, w5
    sub     w8, w4, w2  ; waitTime - (NOW_32() - startTime)
    subb    w9, w5, w3
    bra     C, 0b
    
    pop.d   w10
    pop.d   w8
    return

    .size _NOW_Wait, . - _NOW_Wait
    .popsection
