/*******************************************************************************
 *
 * NOW Task manipulation routines in ezbl_lib
 *
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

;typedef struct NOW_TASK NOW_TASK;
;struct NOW_TASK
;{
;    NOW_TASK *next;                 // Pointer to next NOW_TASK in a linked list. 0 (NULL) for the end of the list.
;    unsigned short timeout[3];      // NOW time (absolute) specifying when the callbackFunction needs to be called next.
;    unsigned short interval[3];     // NOW time 47-bit repeat interval to add to the timeout each time timeout is reached. For one-shot timers, this value is zero and bit 47 is also zero. The interval is in positive time form. For repeat timers, bit 47 is always set for quick testing in the ISR handler.
;    int (*callbackFunction)(void);  // Callback function to execute when "NOW_48"()'s return value rolls past timeout.
;    unsigned int IPL;               // RESERVED/NOT TESTED. USE AT OWN RISK! CPU Priority Level to execute the callback function at (range 0 to 7). Also includes PC[22:16] if needed.
;    volatile int returnSignal;      // Callback function's return value
;};
    ; NOW_TASK structure element offsets
    .equ        next, 0
    .equ        timeout, 2
    .equ        interval, 8
    .equ        callbackFunc, 14
    .equ        IPL, 16
    .equ        returnSignal, 18
    .equ        NOW_TASK_SIZE, 20


/**
 * Schedules a task for execution at a earlier/later time than presently
 * scheduled.
 *
 * When the task is executed, the repeat interval will be added to the time at
 * execution to compute when the task will be run again (assuming the task
 * repeats). Therefore, this function will pull-in/push-out the whole train of
 * subsequent executions without affecting the repeat period for anything but
 * the present cycle.
 *
 * @param task A pointer to a staticaly or globally allocated NOW_TASK
 *             structure to modify.
 *
 * @param timeFromNow Number of now counts into the future when the task should
 *                    be executed next. Use NOW_second, NOW_millisecond, and
 *                    NOW_microsecond to choose this.
 *
 * @return Number of now counts pulled in (pushed out) from the original task's
 *         scheduled execution time. Time pulled in is represented as a positive
 *         number while negative numbers represent a pushed out time.
 */
    .pushsection .text.NOW_SetNextTaskTime, code
    .global _NOW_SetNextTaskTime
    .type   _NOW_SetNextTaskTime, @function
    .extern _NOW_64
    .weak   _NOW_dispatcherActive
;unsigned long NOW_SetNextTaskTime(NOW_TASK *task, unsigned long timeFromNow);
;                                            w0                  w3:w2          ; NOTE: w1 skipped!
_NOW_SetNextTaskTime:
    push    w3  ; Push big end timeFromNow
    push    w2  ; Now little end timeFromNow
    push    w0  ; *task pointer
    call    _NOW_64
    pop     w7                          ; w7 = *task pointer
    add     w0, [--w15], w0
    addc    w1, [--w15], w1             ; w1:w0 = NOW_32() + timeFromNow
    addc    w2, #0, w2
    btsts   _NOW_dispatcherActive, #0	;// Read value in NOW_dispatcherActive and atomically set it as well so we can prevent the NOW_TaskDispatcher from executing during this critical task update
    mov.d   [++w7], w4                  ; w5:w4 = existing execute time. ++w7 is &task->timeout[0]
    mov     w0, [--w7]
    mov     w1, [++w7]
    mov     w2, [++w7]
    btsc    SR, #SR_Z
    bclr    _NOW_dispatcherActive, #0   ;// Restore original value
    sub     w4, w0, w0                  ; return (old_exec_time - new_exec_time)
    subb    w5, w1, w1
    return  

    .size _NOW_SetNextTaskTime, . - _NOW_SetNextTaskTime
    .popsection
    