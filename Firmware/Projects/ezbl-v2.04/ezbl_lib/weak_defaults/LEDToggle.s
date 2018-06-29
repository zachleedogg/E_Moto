;/*******************************************************************************
;  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM source file (ezbl_lib)
;
;  Summary:
;    Implements a default LEDToggle() API that attempts to run-time decode which
;    I/O pins (LATx register) to bit-field toggle. Up to 8 LATx bits are
;    inverted based on the bitmask input.
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

    .equ    SR_C, 0x0   ; SR<C> is bit 0
    .equ    SR_Z, 0x1   ; SR<Z> is bit 1
    .equ    SR_OV, 0x2  ; SR<OV> is bit 2
    .equ    SR_N, 0x3   ; SR<N> is bit 3
    .equ    SR_RA, 0x4  ; SR<RA> is bit 4
    .macro  skip_next_instr
    btsc    w15, #0     ; Skips next instruction since w15 stack pointer can never be odd
    .endm


/**
 * Toggles the state of an array of LEDs using a logical XOR of the ledBitField
 * provided. I.e. only '1' bits toggle an LED state. '0' bits have no effect.
 *
 * To use this function, first call EZBL_DefineLEDMap() at global scope in one 
 * of your top-level project files to create the bit mapping between LED
 * bitfield positions and physical GPIO port pins.
 *
 * This is a wrapper API that internally calls:
 *      EZBL_MapToggle(ledBitField, &EZBL_LEDMap);
 *
 * @param ledBitField A bitmask representing an array of LEDs who's state
 *                    should be toggled. Each '1' bit in the bit field toggles
 *                    the corresponding LED on or off according to the existing
 *                    state in the LATx register (does not read from PORTx).
 *
 * @return Bitmask representing the LEDs that were previously on (as defined in
 *         the LATx register(s)) before XORing in the ledBitField value. Bits
 *         for LEDs that do not exist are returned as '0' values in the
 *         bitmask.
 */
;unsigned int LEDToggle(unsigned int ledBitField);
    .pushsection .text.LEDToggle, code
    .weak   _LEDToggle                  ; Alias to LEDInv();
    .type   _LEDToggle, @function
    .weak   _LEDInv
    .type   _LEDInv, @function
    .extern _EZBL_LEDMap
    .extern _EZBL_MapInv
_LEDInv:
_LEDToggle:         ; unsigned int LEDToggle(unsigned int ledBitField);
    mov     #_EZBL_LEDMap, w1                   ; w1 = EZBL_LEDMap array pointer
    goto    _EZBL_MapInv

    .size _LEDToggle, . - _LEDToggle
    .size _LEDInv, . - _LEDInv
    .popsection
