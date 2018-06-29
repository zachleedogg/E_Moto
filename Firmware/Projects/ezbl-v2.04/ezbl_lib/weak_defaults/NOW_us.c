/*******************************************************************************
  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM API Source File

  Summary:
    Instances the NOW_us/NOW_microsecond variable

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
 * Number of NOW counts per microsecond of physical time (0.000001 seconds,
 * assuming the NOW_Reset() macro is called with a suitably fast timer clock
 * to provide microsecond scale resolution).
 *
 * NOTE: Unlike NOW_sec and NOW_ms, this value is a 16-bit unsigned short. When
 * attempting to measure or time a longer interval with a fast timer base
 * frequency (ex: 937u*NOW_us @ 70 MIPS), ensure that final/intermediate
 * arithmetic is applied using an up-casted (unsigned long) NOW_us value to
 * avoid overflow. Ex: (937ul*NOW_us).
 *
 * 31/32kHz LPRC and 32.768kHz SOSC clocks have a period of over 30us, so DO NOT
 * USE NOW_us with such clocks. The value would always be zero. Use a NOW_ms or
 * NOW_sec fractional value instead.
 *
 * If run time clock switching occurs externally and you are using the system or
 * peripheral clock for the timing reference to NOW, you must call the
 * NOW_SetTimeIntervals() function to recompute this variable. Any NOW APIs
 * which include clock switching internally do not have this requirement as
 * they will automatically update NOW_us, NOW_ms and NOW_sec internally.
 *
 * Note that this variable is an integral number, rounded to result in the least
 * amount of error, either positive or negative. Ex: if the clock
 * frequency is 1.500001MHz, one NOW count would represent 1/1500001Hz or 0.6667
 * microseconds. Therefore, NOW_us, the number of NOW
 * counts/microsecond, would be 1.500001 if precision were infinite. Since
 * rounding instead of truncation occurs, this will result in NOW_us
 * returning 2 instead of 1 (-33.33324% error instead of +33.33338%).
 *
 * Be very cautious using this value if your clock frequency is under 500kHz. In
 * this case, 0 will be returned, which could cause a divide by zero exception
 * when divided or 100% error condition when multiplied.
 */
unsigned short          NOW_us          XC16_NEAR __attribute__((persistent));

extern unsigned short   NOW_microsecond XC16_NEAR __attribute__((persistent, alias("NOW_us")));   // Alias to NOW_us. Name retained for historical EZBL version compatibility.

