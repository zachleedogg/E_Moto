/*
 * File:   LEDToggle.c
 * Author: Howard Schlunder
 *
 * Implements a default/weak LEDToggle() function that will be used if no such
 * function exists locally in the project. This implementation reads/writes to
 * the least significant 8 bits of LATA.
 */

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
#define XC16_NEAR                       __attribute__((near))
#elif defined(__XC32__) || defined(__PIC32__)
#define XC16_NEAR
#endif

extern volatile unsigned int XC16_NEAR LATA;


/**
 *  Toggles the state of an array of LEDs using a logical XOR of the ledBitField
 *  provided. I.e. only '1' bits toggle an LED state. '0' bits have no effect.
 *
 *  @param ledBitField A bitmask representing an array of LEDs who's state
 *                     should be toggled. Each '1' bit in the bit field toggles
 *                     the corresponding LED on or off according to the existing
 *                     state in the LATx register (does not read from PORTx).
 *
 *  @return Bitmask representing the LEDs that were previously on (as defined in
 *          the LATx register(s)) before XORing in the ledBitField value. Bits
 *          for LEDs that do not exist are returned as '0' values in the
 *          bitmask.
 */
unsigned int __attribute__((weak)) LEDToggle(unsigned int ledBitField)
{
    unsigned int ret = 0;

    *((unsigned char*)&ret) = *((volatile unsigned char*)&LATA);
    *((unsigned char*)&LATA) ^= ledBitField;

    return ret;
}
