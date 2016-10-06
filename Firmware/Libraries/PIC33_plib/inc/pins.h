#ifndef PINS_H
#define	PINS_H
/* 
 * File:   pins.h
 * Author: Zachary Levenberg
 * Comments:
 * Revision history: initial build 10/2/16
 */

#include <xc.h> // incl

ude processor files - each processor file is guarded.
#include <stdint.h>

#if 1 //def __dsPIC33EP256MC502__

#define NUMBER_PINS_PORTA 5
#define NUMBER_PINS_PORTB 16
#elif __dsPIC33EP256MC504__
#define NUMBER_PINS_PORTA 11
#define NUMBER_PINS_PORTB 16
#define NUMBER_PINS_PORTC 16
#endif

#define HIGH 1
#define LOW 0

typedef enum {
    OUTPUT,
    INPUT
} PIN_Direction_e;

typedef struct {
    port;
    lat;
    tris;
} PIN_pin;


/**
 * sets pin as input or output
 * @param pin: pin number to select
 * @param direction: ipnut (1) or output (0)
 * @return success (1) or failure (0)
 */
#define PIN_Direction(x, y) (x.tris = y);



/**
 * Sets output pin to high or low
 * @param pin: pin number to select
 * @param value: high (1) or low (0)
 * @return 
 */
#define PIN_Write(x, y) (x.port = y);


/**
 * Toggle output pin to high or low
 * @param pin: pin number to toggle output
 * @return 
 */
#define PIN_Toggle(x) (x#.lat ? x#.port=0 : x#.port=1)

PIN_Toggle(dude);

/**
 * reads value from latch
 * @param pin: pin to read from
 * @return success (1) or failure (0)
 */
uint8_t IO_pinRead(pin_number pin);

/**
 * sets internal pull-up resistor
 * @param pin: output pin to pull up
 * @return success (1) or failure (0)
 * @ note can only be called on pin set as input
 */
uint8_t IO_pinPullUpRes(pin_number pin);

/**
 * sets internal pull-down resistor
 * @param pin: output pin to pull up
 * @return success (1) or failure (0)
 * @ note can only be called on pin set as input
 */
uint8_t IO_pinPullDownRes(pin_number pin);

/**
 * Sets Interrupt on pin change
 * @param pin: pin to interrupt on
 * @return success (1) or failure (0)
 */
uint8_t IO_pinInterrupt(pin_number pin);

#endif	/* PINS_H */

