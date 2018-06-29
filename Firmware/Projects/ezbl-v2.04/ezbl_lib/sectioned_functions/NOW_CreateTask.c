/*******************************************************************************
 *
 * Time Counting and Measurement Functions of NOW library (part of EZBL)
 * 
 * Includes routines for measuring absolute time, polled task triggering and 
 * measuring oscillators such as your Fast internal RC oscillator 
 * (FRC), Digitally Controlled Oscillator (DCO), or other clock source.
 * 
 ******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#include "../ezbl.h"


EZBL_KeepSYM(NOW_TaskDispatcher);

/**
 * NOW_timerCallbackHead - Internal pointer to the first element of a linked 
 * list for tracking and launching user callback functions created with the 
 * NOW_CreateTask() function. This variable should not be changed, and it is 
 * attributed persistent so the CRT doesn't initialize it to zero either. 
 * Instead, NOW_Init() nullifies it, with NOW_CreateTask(), NOW_EndTask(), and 
 * the dispatching routines handling modification of it.
 */
NOW_TASK XC16_NEAR __attribute__((persistent)) *NOW_timerCallbackHead;



/**
 * Adds, updates, or deletes a "NOW Task" which automatically executes a 
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
 *          expiration. The callback function should take no parameters and 
 *          place nothing on the stack upon return. Normally, this means the 
 *          callback will be declared with a void return signature. However, it 
 *          is legal to use a non-void return function, so long as the return 
 *          value are short enough to be passed in the w0 to w7 temporary 
 *          registers.
 *          (i.e. char/short/int/long/long long/float/double return types and 
 *          their unsigned equivalents, as applicable are allowed). 
 * 
 *          If the callback does return a value, it is ignored.
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
 *                -((unsigned long long)(100u*NOW_millisecond)).
 * 
 * @return NOW_TimerCallbackHead/downstream timerContext linked list is updated.
 */
void NOW_CreateTask(NOW_TASK *timerContext, int (*callbackFunction)(void), signed long long timeout)
{
    unsigned long long now;
    
    // First stop the task from executing if it is already in the queue. We 
    // don't want to risk executing the task while we are in the middle of 
    // reconfiguring it
    NOW_EndTask(timerContext);
    
    // If we were asked to delete an existing timer at the specified 
    // timerContext address, we are done
    if(timeout == 0)
    {
        return;
    }
    
    // Configure the task's scheduling timer data and other context values
    now = NOW_64();
    EZBL_RAMSet(timerContext, 0x00, sizeof(*timerContext));
    timerContext->callbackFunction = callbackFunction;
    if(timeout < 0)
    {
        timeout = -timeout;
        EZBL_RAMCopy(&timerContext->interval, &timeout, 6); // Note: only saving 6 bytes, rather than all 8 for ISR speed. A 48-bit signed timer limits the max repeat interval to once every 8.1445 days @ 200 MHz.
    }
    now += (unsigned long long)timeout;
    EZBL_RAMCopy(&timerContext->timeout, &now, 6);          // Note: only saving 6 bytes, rather than all 8 for ISR speed. A 48-bit signed timer limits the max differential timeout to 8.1445 days @ 200 MHz.
     
    // Put this task at the start of the list to activate it
    timerContext->next = NOW_timerCallbackHead;
    NOW_timerCallbackHead = timerContext;
}
