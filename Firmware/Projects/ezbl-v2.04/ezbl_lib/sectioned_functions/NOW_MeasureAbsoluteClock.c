/*******************************************************************************
 *
 * Time Counting and Measurement Functions of NOW library (part of EZBL)
 * 
 * Includes routines for measuring absolute time, polled task triggering and 
 * measuring oscillators such as your Fast internal RC oscillator 
 * (FRC), Digitally Controlled Oscillator (DCO), or other clock source.
 * 
 *******************************************************************************
 * FileName:        NOW_MeasureAbsoluteClock.c
 * Dependencies:    Timer 1 (or other 16-bit timer/MCCP/SCCP as compile option)
 * Processor:       PIC24, dsPIC33, PIC32
 * Compiler:        Microchip XC16 v1.21 or higher
 *                  Microchip XC32 v1.40 or higher
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

#include <math.h>


/**
 * Measures an unknown-frequency clock source against another known-frequency 
 * reference clock source and computes the unknown frequency.
 * 
 * @param targetClockType
 *        <p>0x0 = 16-bit Timer/CCP/Counter in an SFR or RAM 
 *                 location. *refClock will be dereferenced as a 
 *                 volatile unsigned short to get "tick" 
 *                 values. In the case of a RAM location, an 
 *                 external ISR of higher priority than the present 
 *                 CPU IPL must write the new values into the given 
 *                 address as this function does not return until 
 *                 the DCO measurements are complete.
 *        <p>0x1 = 32-bit Timer/CCP/Counter in an SFR or RAM. 
 *                 *refClock will be dereferenced as a 
 *                 volatile unsigned long to get "tick" values.
 *        <p>0x2 = 16-bit "tick" via function pointer return.
 *                 *refClock must point to an executable function 
 *                 that takes no parameters and will return an 
 *                 incrementing 16-bit integer type (unsigned int, 
 *                 signed int, unsigned short, unsigned short int, 
 *                 signed short, etc.). Generally, it is possible 
 *                 to use this value even for 32-bit return valued 
 *                 functions, but in such a case, the upper 16-bits 
 *                 of information will always be discarded.
 *        <p>0x3 = 32-bit "tick" via function pointer return.
 *                 *refClock must point to an executable function 
 *                 that takes no parameters and will return an 
 *                 incrementing 16-bit integer type (unsigned int, 
 *                 signed int, unsigned short, unsigned short int, 
 *                 signed short, etc.).
 * 
 * @param targetClock       Pointer to the specified clock resource. See 
 *                          targetClockType parameter definition for exact 
 *                          meaning of this pointer.
 * 
 * @param refClockType      Same as targetClockType, but for the known-frequency 
 *                          reference clock.
 * 
 * @param refClock          Same as targetClock, but for the known-frequency 
 *                          reference clock.
 * 
 * @param refClockFrequency Frequency of the known-frequency refClock, in Hz.
 * 
 * @param desiredPPMPrecision   
 *                          Desired precision in measuring the target clock
 *                          frequency, in units of PPM (Parts Per Million). 
 *                          1000 PPM is equivalent to +/-0.1% clock error 
 *                          (1000/1000000 * 100%). 
 * 
 *                          Higher precision takes longer to measure, 
 *                          potentially with a Gaussian tail, meaning a LOT 
 *                          longer. 
 *                          
 *                          NOTE: if either the reference, and to a lesser 
 *                          extent, target clock frequency is unstable and 
 *                          drifts faster than the desired precision can be 
 *                          achieved, this function could block forever. To 
 *                          avoid this, specify a timeout.
 * 
 * @param milliTimeout      Maximum number of milliseconds allowed to try and 
 *                          achieve the desiredPPMPrecision criteria. To disable 
 *                          the timeout, specify 0.
 * 
 * @return  Present frequency of the targetClock, in cycles-per-second (Hz). If an 
 *          unrecognized parameter is given, 0xFFFFFFFF is returned.
 */
unsigned long NOW_MeasureAbsoluteClock(unsigned int targetClockType, void *targetClock, unsigned int refClockType, void *refClock, unsigned long refClockFrequency, unsigned int desiredPPMPrecision, unsigned int milliTimeout)
{
    unsigned int lookingGood;
    unsigned long refTime, startRefTime, lastRefTime, deltaRef;
    unsigned long targetTime, startTargetTime, lastTargetTime, deltaTarget;
    unsigned long long targetAccumulator;
    unsigned long long refAccumulator;
    unsigned long long refAccumulatorTimeout;
    double targetHz, lastTargetHz;
    double elapsedTotalTime;
    double precisionLimit;
    //unsigned long minSampleTime;
    
    // Function pointers
    unsigned int (*refClockFunc16)(void) = refClock;
    unsigned long (*refClockFunc32)(void) = refClock;
    unsigned int (*targetClockFunc16)(void) = targetClock;
    unsigned long (*targetClockFunc32)(void) = targetClock;
    
    // Return impossibly high 4GHz value if an invalid null pointer is supplied
    if((refClock == 0u) || (targetClock == 0u))
        return 0xFFFFFFFF;
    
    
    // Initialize variables before entering timing sensitive loop
    refAccumulator = 0;
    targetAccumulator = 0;
    lookingGood = 0;
    startRefTime = 0;
    lastRefTime = 0;
    startTargetTime = 0;
    lastTargetTime = 0;
    lastTargetHz = 0;
    refAccumulatorTimeout = (((unsigned long long)milliTimeout)*refClockFrequency + 500u)/1000u;
    precisionLimit = desiredPPMPrecision/1.0e6;
    //minSampleTime = (((unsigned long long)(1000000u - desiredPPMPrecision)) * refClockFrequency)/1000000u;
    
    // Enter timing compare loop
    while(1)
    {
        // Get the reference clock
        switch(refClockType)
        {
            case 0x1:   // 32-bit SFR/RAM/data pointer
                refTime = *((volatile unsigned long*)(refClock));
                break;
            case 0x2:   // 16-bit function pointer
                refTime = refClockFunc16();
                break;
            case 0x3:   // 32-bit function pointer
                refTime = refClockFunc32();
                break;
            default:    // case 0x0 or anything else -> Assume 16-bit  SFR/RAM/data pointer
                refTime = *((volatile unsigned short*)(refClock));
        }

        // Get the target clock to measure
        switch(targetClockType) 
        {
            case 0x1:   // 32-bit SFR/RAM/data pointer
                targetTime = *((volatile unsigned long*)(targetClock));
                break;
            case 0x2:   // 16-bit function pointer
                targetTime = targetClockFunc16();
                break;
            case 0x3:   // 32-bit function pointer
                targetTime = targetClockFunc32();
                break;
            default:    // case 0x0 or anything else -> Assume 16-bit SFR/RAM/data pointer
                targetTime = *((volatile unsigned short*)(targetClock));
        }

        // Initialize history if this is the very start of the test
        if(startRefTime == 0u)
        {
            startRefTime = refTime;
            lastRefTime = refTime;
            startTargetTime = targetTime;
            lastTargetTime = targetTime;
        }
        
        // Don't process anything if the reference timer value hasn't incremented 
        // to give us new knowledge.
        if(refTime <= lastRefTime)
            continue;
        if(refTime <= startRefTime)
            continue;
        
        // Compute commonly used differences, update accumulators
        deltaRef = refTime - lastRefTime;
        deltaTarget = targetTime - lastTargetTime;
        lastRefTime = refTime;
        lastTargetTime = targetTime;
        refAccumulator += deltaRef;
        targetAccumulator += deltaTarget;
        
        // Compute target frequency using everything we know so far
        elapsedTotalTime = ((double)(refAccumulator))/refClockFrequency;
        targetHz =((double)targetAccumulator)/elapsedTotalTime;
        
        // Check for timeout
        if(milliTimeout)
        {
            if(refAccumulator >= refAccumulatorTimeout)
            {
                return targetHz;
            }
        }
        
        // Check if target delta meets the desired precision
        //if(refAccumulator > minSampleTime)          // Must wait long enough to reach desired PPM according to the reference clock
        {
            //if(targetAccumulator > minSampleTime)   // Must also wait long enough to reach the desired PPM according to the approximate target clock
            {
                if((fabs(targetHz - lastTargetHz)/targetHz) < precisionLimit)   // Lastly, filter a few samples to ensure the delta for several iterations are below the PPM limit
                {
                    // We are looking good, but let's collect several such readings 
                    // matching the desired precision to avoid any flukes and 
                    // instead be sure our target delta really has leveled off.
                    if(lookingGood++ >= 16u)
                    {
                        return (unsigned long)targetHz;
                    }
                }
                else if(lookingGood)
                {
                    lookingGood = 0;
                }
            }
        }

        lastTargetHz = targetHz;
    }
}

