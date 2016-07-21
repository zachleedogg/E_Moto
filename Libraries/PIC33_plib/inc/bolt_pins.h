
/* 
 * File:   pins.h
 * Author: Zachary Levenberg
 * Comments:
 * Revision history: initial build 2/4/16
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PINS_H
#define	PINS_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>

#ifdef __dsPIC33EP256MC502__
#define NUMBER_PINS_PORTA 5
#define NUMBER_PINS_PORTB 16
#elif __dsPIC33EP256MC504__
#define NUMBER_PINS_PORTA 11
#define NUMBER_PINS_PORTB 16
#define NUMBER_PINS_PORTC 16
#endif

#define HIGH 1
#define LOW 0

#define INPUT 1
#define OUTPUT 0

typedef enum _pin_number {
    IO_PIN_RA0,
    IO_PIN_RA1,
    IO_PIN_RA2,
    IO_PIN_RA3,
    IO_PIN_RA4,
#ifdef __dsPIC33EP256MC504__
    IO_PIN_RA5,/*Does Not Exist*/
    IO_PIN_RA6,/*Does Not Exist*/
    IO_PIN_RA7,
    IO_PIN_RA8,
    IO_PIN_RA9,
    IO_PIN_RA10,
#endif
    IO_PIN_RB0,
    IO_PIN_RB1,
    IO_PIN_RB2,
    IO_PIN_RB3,
    IO_PIN_RB4,
    IO_PIN_RB5,
    IO_PIN_RB6,
    IO_PIN_RB7,
    IO_PIN_RB8,
    IO_PIN_RB9,
    IO_PIN_RB10,
    IO_PIN_RB11,
    IO_PIN_RB12,
    IO_PIN_RB13,
    IO_PIN_RB14,
    IO_PIN_RB15,
#ifdef __dsPIC33EP256MC504__
    IO_PIN_RC0,
    IO_PIN_RC1,
    IO_PIN_RC2,
    IO_PIN_RC3,
    IO_PIN_RC4,
    IO_PIN_RC5,
    IO_PIN_RC6,
    IO_PIN_RC7,
    IO_PIN_RC8,
    IO_PIN_RC9,
    IO_PIN_RC10,
    IO_PIN_RC11,/*Does Not Exist*/
    IO_PIN_RC12,/*Does Not Exist*/
    IO_PIN_RC13,/*Does Not Exist*/
    IO_PIN_RC14,/*Does Not Exist*/
    IO_PIN_RC15,/*Does Not Exist*/
#endif
    NUMBER_OF_PINS,
} pin_number;

typedef enum _io_port {
    IO_PORTA,
    IO_PORTB,
    IO_PORTC,
} io_port;

/**
 * sets pin as input or output
 * @param pin: pin number to select
 * @param direction: ipnut (1) or output (0)
 * @return success (1) or failure (0)
 */
uint8_t IO_setPinDir(pin_number pin, uint8_t direction);

/*
 Function: IO_setPortDir()
 Params: ports: which port, IO_PORTA or IO_PORTB
         direction: INPUT or OUTPUT
 Return: 1 = success, 0 = failure
 Description: sets all pins of an io port to a specific direction
 author: brdgordo; 2/16/16
 */
uint8_t IO_setPortDir(io_port port, uint8_t direction);

/**
 * Sets output pin to high or low
 * @param pin: pin number to select
 * @param value: high (1) or low (0)
 * @return 
 */
uint8_t IO_pinWrite(pin_number pin, uint8_t value);

/*
 Function: IO_portWrite()
 Params: ports: which port, IO_PORTA or IO_PORTB
         value: HIGH or LOW
 Return: 1 = success, 0 = failure
 Description: writes to all pins of an io port
 author: brdgordo; 2/16/16
 */
uint8_t IO_portWrite(io_port port, uint8_t value);

/**
 * Toggle output pin to high or low
 * @param pin: pin number to toggle output
 * @return 
 */
uint8_t IO_pinToggle(pin_number pin);

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

