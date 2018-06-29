/*******************************************************************************
 *
 * Time Counting and Measurement Functions of NOW library (part of EZBL)
 * 
 * Includes routines for measuring absolute time, polled task triggering and 
 * measuring oscillators such as your Fast internal RC oscillator 
 * (FRC), Digitally Controlled Oscillator (DCO), or other clock source.
 * 
 *******************************************************************************
 * FileName:        NOW_EndTask.c
 * Dependencies:    NOW_CreateTask.c
 * Processor:       PIC24, dsPIC33, PIC32
 * Compiler:        Microchip XC16 v1.26 or higher
 *                  Microchip XC32 v1.42 or higher
 * Company:         Microchip Technology, Inc.
 * Author:          Howard Schlunder
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Copyright (C) 2016 Microchip Technology Inc.

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


#if defined(__XC16__)
#define XC16_NEAR            __attribute__((near))
#elif defined(__XC32__) || defined(__PIC32__)
#define XC16_NEAR
#endif


typedef struct NOW_TASK NOW_TASK;
struct NOW_TASK
{
    NOW_TASK *next;                 // Pointer to next NOW_TASK in a linked list. 0 (NULL) for the end of the list.
    unsigned short timeout[3];      // NOW time (absolute) specifying when the callbackFunction needs to be called next.
    unsigned short interval[3];     // NOW time 47-bit repeat interval to add to the timeout each time timeout is reached. For one-shot timers, this value is zero and bit 47 is also zero. The interval is in positive time form. For repeat timers, bit 47 is always set for quick testing in the ISR handler.
    int (*callbackFunction)(void);  // Callback function to execute when "NOW_48"()'s return value rolls past timeout.
    unsigned int IPL;               // RESERVED/NOT TESTED. USE AT OWN RISK! CPU Priority Level to execute the callback function at (range 0 to 7). Also includes PC[22:16] if needed.
    volatile int returnSignal;      // Callback function's return value
};


/**
 * NOW_timerCallbackHead - Internal pointer to the first element of a linked 
 * list for tracking and launching user callback functions created with the 
 * NOW_CreateTask() function. This variable should not be changed, and it is 
 * attributed persistent so the CRT doesn't initialize it to zero either. 
 * Instead, NOW_Init() nullifies it, with NOW_CreateTask(), NOW_EndTask(), and 
 * the dispatching routines handling modification of it.
 */
extern NOW_TASK * XC16_NEAR __attribute__((persistent)) NOW_timerCallbackHead;


/**
 * Deletes a specified task from being called again. Works on both one-shot 
 * tasks and repeating ones.
 * 
 * @param taskToStopCalling Pointer to the task to stop calling.
 * @return 0 if the task was not scheduled to be called.
 *         1 if the task was successfully removed from the execution schedule.
 */
int NOW_EndTask(NOW_TASK *taskToStopCalling)
{
    NOW_TASK *left, *right;

    left = (NOW_TASK*)&NOW_timerCallbackHead;   // NOTE!!! This is a pointer to a pointer. The ->next structure element must be the first element in the typedef.
    while(1)
    {
        right = left->next;
        if(right == (void*)0)                   // Not an active task if we reach the end of the chain
            return 0;
        if(right == taskToStopCalling)
        {
            left->next = right->next;           // Found it, hook the last one up to the next one to remove ourself
            return 1;
        }
        left = right;
    }
}
