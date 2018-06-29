/*******************************************************************************
 *
 * Time Counting and Measurement Functions of NOW library (part of EZBL)
 *
 * Includes routines for measuring absolute time, polled task triggering and
 * measuring oscillators such as your Fast internal RC oscillator
 * (FRC), Digitally Controlled Oscillator (DCO), or other clock source.
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

    ; Make a strong reference to the NOW_TaskDispatcher() function so it doesn't
    ; go unreferenced and the the NOW Timer interrupt will call it.
    .extern _NOW_TaskDispatcher
    .pushsection .info.EZBL_KeepSYM, info, keep
    .word   _NOW_TaskDispatcher
    .popsection



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
 * Adds, updates, or deletes a timer task which automatically executes a
 * function after the specified time elapses. The timer can be a one-shot event
 * or an automatically self-repeating timer.
 *
 * @param timerContext A pointer to a caller allocated NOW_TASK
 *          structure. If the pointer matches one previously given to
 *          NOW_CreateTimer(), but for which the task has not expired yet, this
 *          API will update the existing context structure with the latest
 *          callbackFunction pointer and timeout parameters, effectively
 *          replacing the existing timer.
 *
 *          NOTE: the pointed to memory MUST stay resident until the callback is
 *          executed (for one-shot mode). If the timer is a self-repeating type,
 *          the pointed to memory MUST stay resident at all times. Normally,
 *          the timerContext structure should be allocated as static or at
 *          global scope to ensure this requirement is met.
 *
 * @param callbackFunction Pointer to a callback function to execute upon timer
 *          expiration. The callback function should take (void) parameters and
 *          return an (int).
 *
 *          The return value does nothing inside the NOW APIs, except get stored
 *          in the timerContext structure for retrieval by the main() loop or
 *          other tasks by calling NOW_GetTaskReturnCode(). Due to the
 *          asynchronous nature of the task, the return code must be treated as
 *          a "lossy" communications method much like how two reads from a PORTx
 *          register can completely miss a short voltage spike on an I/O pin
 *          that falls in between the two read events.
 *
 * @param timeout
 *          >= 1: The number of NOW counts to wait until expiring a one-shot
 *                timer and executing the callback. This value is relative to
 *                the present time, so '1' always means: execute the callback
 *                right away, not when NOW_64()'s return value rolls past the
 *                value 0x1. Using a value of 'NOW_second' means: execute the
 *                callback one second in the future.
 *
 *                In this one-shot timer mode, the timerContext memory will be
 *                freed immediately prior to calling the callbackFunction. It is
 *                therefore legal to restart another timer (possibly for
 *                the same callback function) from within the callback code and
 *                using the same timerContext memory.
 *
 *          == 0: Any existing timer task at the timerContext memory is ended.
 *                The timerContext memory is freed and can be reused for other
 *                purposes after this NOW_CreateTimer() API returns. The
 *                callbackFunction parameter is ignored.
 *
 *                If you wish to create a timer that executes immediately,
 *                specify timeout as '1' instead.
 *
 *          < 0:  Creates a self-repeating timer for executing the callback. The
 *                value specified is negated (2's complemented) in order to set
 *                the repeat inteveral, in NOW counts. For example, to create a
 *                periodic callback event every 100 milliseconds, and being
 *                called for the first time 100 milliseconds from now, specify:
 *                -(100u*NOW_ms).
 *
 * @return NOW_TimerCallbackHead/downstream timerContext linked list is updated.
 */
    .pushsection .text.NOW_CreateTask, code
    .global _NOW_CreateTask
    .type   _NOW_CreateTask, @function
    .extern _NOW_timerCallbackHead
    .extern _NOW_64
    .extern _NOW_EndTask

;void NOW_CreateTaskEx(NOW_TASK *timerContext, unsigned long longFuncAddr, unsigned int callbackIPL, signed long long timeout);
;                               w0             w3:w2                          w1                        w7:w4
;_NOW_CreateTaskEx:     ;// Commeted out as this IPL assignment feature is not tested and is expected to cause coherency problems if used right now.
;    sl      w1, #8, w1
;    ior     w3, w1, [w15++] ; *callbackFunction high + IPL
;    push    w2              ; *callbackFunction low
;    bra     0f
  
;void NOW_CreateTask(NOW_TASK *timerContext, int (*callbackFunction)(void), signed long long timeout);
;                              w0                  w1                                        w7:w4
_NOW_CreateTask:
    clr     [w15++]         ; *callbackFunction high + IPL
    push    w1              ; *callbackFunction
0:  push    w0              ; *timerContext
    push    w6              ; timeout<47:32>
    push    w5              ; timeout<31:16>
    push    w4              ; timeout<15:0>

    call    _NOW_EndTask    ; NOW_EndTask(timerContext);
    call    _NOW_64         ; w3:w0 = NOW_64();

    clr     w7
    sub     w7, [--w15], w4
    subb    w7, [--w15], w5
    subb    w7, [--w15], w6
    pop     w3              ; w3 = &(NOW_TASK->next)
    bra     Z, done
    mov     #_NOW_timerCallbackHead, w7
    mov     [w7], [w3++]    ; timerContext->next = NOW_timerCallbackHead;       // Prep for task inseration at the beginning of the list
    bra     N, oneShotPositiveTimeout

    add     w0, w4, [w3++]  ; NOW_TASK->timeout = NOW_48 + (-timeout)
    addc    w1, w5, [w3++]
    addc    w2, w6, [w3++]
1:  mov.d   w4, [w3++]      ; NOW_TASK->interval = (-timeout)
    mov     w6, [w3++]
    pop     [w3++]          ; callbackFunc<15:0>
    pop     [w3++]          ; callbackFunc<23:16> + IPL
    mov     #0x8000, w0
    mov     w0, [w3++]      ; returnSignal = 0x8000; // -32768
    sub     w3, #NOW_TASK_SIZE, [w7]    ; NOW_timerCallbackHead = timerContext; // Activates the task
    return

oneShotPositiveTimeout:
    sub     w0, w4, [w3++]  ; timeout = NOW_48 - (-timeout)
    subb    w1, w5, [w3++]
    subb    w2, w6, [w3++]
    mul.uu  w4, #0, w4      ; interval = 0
    clr     w6
    bra     1b

done:
    sub     w15, #4, w15    ; Remove unused calbackFunction (high and low) + IPL from stack
    return

    .size _NOW_CreateTask, . - _NOW_CreateTask
    .popsection
