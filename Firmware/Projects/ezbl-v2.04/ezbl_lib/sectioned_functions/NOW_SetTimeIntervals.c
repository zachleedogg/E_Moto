/*******************************************************************************
 *
 * Time Counting and Measurement Functions of NOW library (part of EZBL)
 * 
 * Includes routines for measuring absolute time, polled task triggering and 
 * measuring oscillators such as your Fast internal RC oscillator 
 * (FRC), Digitally Controlled Oscillator (DCO), or other clock source.
 * 
 *******************************************************************************
 * FileName:        NOW_SetTimeIntervals.c
 * Dependencies:    Timer 1 (or other 16-bit timer/MCCP/SCCP as defined by 
 *                  run-time NOW_timerSFRPtr)
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


// Compiler specific macro for ensuring PIC24/dsPIC assembly instructions can 
// quickly address the NOW_second, NOW_millisecond, NOW_microsecond and
// NOW_timerSFRPtr variables. XC32 doesn't need this, so this macro deletes the
// 'near' attribute.
#if defined(__XC32__)
#define _ASM_ACCESSIBLE
#elif defined(__XC16__)
#define _ASM_ACCESSIBLE     __attribute__((near))
#endif


extern unsigned long _ASM_ACCESSIBLE __attribute__((persistent))  NOW_sec;
extern unsigned long _ASM_ACCESSIBLE __attribute__((persistent))  NOW_ms;
extern unsigned short _ASM_ACCESSIBLE __attribute__((persistent)) NOW_us;


/**
 * Updates the NOW_sec, NOW_ms and NOW_us variables to correspond to the
 * specified timer input clock frequency.
 *
 * @param timerInputFrequency 
 *          Number of timer input clock cycles per second. For example, use 
 *          70000000 if your device is operating at 70 MIPS and using the system 
 *          clock to clock the timer.
 */
void NOW_SetTimeIntervals(unsigned long timerInputFrequency)
{
    NOW_sec = timerInputFrequency;
    NOW_ms  = (timerInputFrequency+500u)/1000u;         // (500/1000) is addition of 0.5ms. I.e. rounds up or down to nearest millisecond count with least error magnitude.
    NOW_us  = (timerInputFrequency+500000u)/1000000u;   // (500000/1000000) is addition of 0.5us. I.e. rounds up or down to nearest microsecond count with least error magnitude.
}
