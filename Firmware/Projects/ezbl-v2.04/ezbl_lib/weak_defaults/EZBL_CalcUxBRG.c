/*******************************************************************************
  Easy Bootloader for PIC24/dsPIC and PIC32MM (ezbl_lib)

  Summary:
    Implements the EZBL_CalcUxBRG() ezbl_lib API
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


#if defined(__XC16__) && (defined(__dsPIC33C__) || defined(__dsPIC33CH__) || defined(EZBL_LIB16CH) || defined(U1MODEH))
#define UART_CLK_MULT   2u  // dsPIC33C devices support FCY*2 as a clock input option
#else
#define UART_CLK_MULT   1u
#endif


/**
 * Computes the closest UxBRG value for the specified baud rate, assuming a 16x
 * clock sampling divider (BRGH == 0).
 *
 * The return value will be rounded to minimize total baud error magnitude, so
 * will be up to +/- 0.5 UxBRG steps faster or slower when a perfect match is
 * impossible for the given peripheral input clock.
 *
 * The UxBRG value computed is not written to any SFR.
 *
 * @param peripheralClock Input clock speed to the peripheral (typically system
 *                        clock speed in MIPS, ex: NOW_systemFrequency).
 *
 *                        On dsPIC33C devices, the UART_Reset() API configures
 *                        the UART peripheral to run at Fosc (2*Fcy), but
 *                        peripheralClock should still be specified as Fcy. The
 *                        UxBRG value computed will be computed against 2x this
 *                        peripheralClock parameter value.
 *
 * @param baud Baud rate to compute for, specified in bits per second. Ex:
 *             115200, 230400, 460800, 921600, etc.
 *
 *             If baud is a negative value, the return value will also be
 *             negative, but the magnitude will be correct. Take the 2's
 *             complement before assigning it to a UxBRG register.
 *
 * @return Closest UxBRG value needed to achieve the specified baud rate.
 *
 *         If baud is negative, the correct UxBRG value will be returned in 2's
 *         complemented form.
 *
 *         If baud is 0, 0 is returned without triggering a divide by zero
 *         error.
 */
int __attribute__((weak)) EZBL_CalcUxBRG(unsigned long peripheralClock, long baud)
{
    int complemented;
    unsigned int BRG;

    if(baud == 0)  // Abort if division by zero would occur
        return 0;

    complemented = 0;
    if(((unsigned char*)&baud)[3] & 0x80)   // Is baudRate < 0 for auto-baud with initial baud rate specified
    {
        complemented = 1;
        baud = -baud;
    }
    BRG = (UART_CLK_MULT*peripheralClock + (baud*8u))/(baud*16u) - 1u;    // Note: (baud*8u)/(baud*16u) = 0.5, so this formula does rounding to the BRG value that will cause the lowest absolute error magnitude that is either + or - from the perfect value.
    return complemented ? -BRG : BRG;
}
