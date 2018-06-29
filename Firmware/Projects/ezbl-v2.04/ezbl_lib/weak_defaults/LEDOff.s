;/*******************************************************************************
;  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM source file (ezbl_lib)
;
;  Summary:
;    Implements a default LEDOff() API by calling LEDToggle() or reverting to
;    LATA accesses.
;
;*******************************************************************************/
;
;// DOM-IGNORE-BEGIN
;/*******************************************************************************
;  Copyright (C) 2017 Microchip Technology Inc.
;
;  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
;  derivatives created by any person or entity by or on your behalf, exclusively
;  with Microchip's products.  Microchip and its licensors retain all ownership
;  and intellectual property rights in the accompanying software and in all
;  derivatives here to.
;
;  This software and any accompanying information is for suggestion only.  It
;  does not modify Microchip's standard warranty for its products.  You agree
;  that you are solely responsible for testing the software and determining its
;  suitability.  Microchip has no obligation to modify, test, certify, or
;  support the software.
;
;  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
;  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
;  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
;  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
;  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
;
;  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
;  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
;  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
;  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
;  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
;  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
;  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
;  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
;  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
;
;  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
;  TERMS.
;*******************************************************************************/
;// DOM-IGNORE-END


/**
 * Turns off an array of LEDs using a logical NOT-AND of the ledBitField
 * provided. I.e. only '1' bits turn LEDs off, '0' bits have no effect.
 *
 * This function internally calls LEDToggle() twice, so LEDToggle() should be
 * defined to use this ezbl_lib API. If it is not defined, this function
 * accesses (all 16-bits that are implemented in) LATA instead.
 *
 * @param ledBitField Bitmask representing an array of LEDs that should be
 *                    turned off. Each '1' bit in the bit field turns the
 *                    corresponding LED off.
 *
 * @return Bitmask representing the LEDs that were previously off before
 *         NOT-ANDing in the ledBitField value. Bits for LEDs that do not exist
 *         are returned as '1' values in the bitmask.
 */
;unsigned int LEDOff(unsigned int ledBitField);
    .pushsection .text.LEDOff, code
    .weak   _LEDOff
    .type   _LEDOff, @function
    .weak   _LEDClr                     ; Alias to LEDOff()
    .type   _LEDClr, @function
    .extern _LEDToggle
_LEDOff:
_LEDClr:
    push    w0
    clr     w0
    call    _LEDToggle
    and     w0, [--w15], w0             ; return ~LEDToggle(ledBitField & LEDToggle(0));
    call    _LEDToggle
    com     w0, w0
    return

    .size _LEDOff, . - _LEDOff
    .size _LEDClr, . - _LEDClr
    .popsection
