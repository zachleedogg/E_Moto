/*******************************************************************************
  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM API Source File

  Summary:
    Instances the NOW_sec/NOW_second variable

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
 * Number of NOW counts per second of physical time (assuming the NOW_Reset()
 * macro was called to initialize the NOW timer).
 *
 * If run time clock switching occurs externally and you are using the system or
 * peripheral clock for the timing reference to NOW, you must call the
 * NOW_SetTimeIntervals() function to recompute this variable. Any NOW APIs
 * which include clock switching internally do not carry this requirement since
 * they will automatically update NOW_us, NOW_ms and NOW_sec internally.
 *
 * This variable is typically the same value as the device clock frequency, so
 * will results in no rounding or truncation error.
 *
 * Note that this is an unsigned long value. You must up-cast this to an
 * (unsigned long long) if you are running at a high clock frequency and want to
 * compute an interval that would overflow a 32-bit integer. For example, if the
 * device clock is 70.0MHz, NOW_sec would be 70,000,000. If you want to
 * compute something that is two minutes (120 seconds), the correct number of
 * NOW counts would be 8,400,000,000, which can't fit in a 32-bit variable. For
 * this clock speed, any calculation (including intermediate ones) requiring
 * more than 61.356 seconds will be invalid without an initial cast to 64-bits.
 * The NOW_64() API would typically also be needed when working with long
 * intervals.
 */
unsigned long           NOW_sec       XC16_NEAR __attribute__((persistent));

extern unsigned long    NOW_second    XC16_NEAR __attribute__((persistent, alias("NOW_sec")));      // Alias to NOW_sec. Name retained for historical EZBL version compatibility.


#if defined(__XC16__)
extern unsigned long    NOW_Fcy       XC16_NEAR __attribute__((persistent, alias("NOW_sec")));      // Alias to NOW_sec on 16-bit platforms. NOW_sec/NOW_second/NOW_Fcy/NOW_systemFrequency are all the same variable.

extern unsigned long    NOW_systemFrequency XC16_NEAR __attribute__((persistent, alias("NOW_Fcy")));// Alias to NOW_Fcy. Name retained for historical EZBL version compatibility.
#endif
