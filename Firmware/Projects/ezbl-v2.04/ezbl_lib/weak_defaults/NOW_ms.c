/*******************************************************************************
  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM API Source File

  Summary:
    Instances the NOW_ms/NOW_millisecond variable

 *******************************************************************************/

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

#if defined(__XC16__)
#define XC16_NEAR            __attribute__((near))
#else
#define XC16_NEAR
#endif



/**
 * Number of NOW counts per millisecond of physical time (0.001 seconds,
 * assuming the NOW_Reset() macro was called to initialize the NOW timer).
 *
 * If run time clock switching occurs externally and you are using the system or
 * peripheral clock for the timing reference to NOW, you must call the
 * NOW_SetTimeIntervals() function to recompute this variable. Any NOW APIs
 * which include clock switching internally do not carry this requirement since
 * they will automatically update NOW_us, NOW_ms, and NOW_sec for you.
 *
 * Note that this variable is an integral number, rounded to result in the least
 * amount of error, either positive or negative. Ex: if the clock
 * frequency is 32.768kHz, one NOW count would represent 1/32768Hz or
 * 0.030517578125 milliseconds. Therefore, NOW_ms, the number of NOW
 * counts/millisecond, would be 32.768 if precision were infinite. Since
 * rounding instead of truncation occurs, this will result in NOW_ms
 * returning 33 instead of 32 (-0.708% error versus +2.344% error).
 */
unsigned long           NOW_ms            XC16_NEAR __attribute__((persistent));

extern unsigned long    NOW_millisecond   XC16_NEAR __attribute__((persistent, alias("NOW_ms"))); // Alias to NOW_ms. Name retained for historical EZBL version compatibility.

