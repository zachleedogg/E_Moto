/* 
 * File:   pins.h
 * Author: Zachary Levenberg
 * Comments:
 * Revision history: initial build 10/2/16
 */

#include <xc.h>
#include <stdint.h>

#ifndef PINS_H
#define	PINS_H

#define PIN_CONCAT(A, B) A##B

/************************************************
 * PINS
 * 
 * Any pin argument may be in the form A1, B5, F7, etc...
 * If the target micro does not have this pin, the build will
 * fail at compile time
 */

typedef enum {
    LOW,
    HIGH,
    TOGGLE
} PINS_state_e;

typedef enum {
    OUTPUT,
    INPUT
} PINS_direction_e;

typedef enum {
#ifdef PORTA
    PIN_PORTA,
#endif
#ifdef PORTB
    PIN_PORTB,
#endif
#ifdef PORTC
    PIN_PORTC,
#endif
#ifdef PORTD
    PIN_PORTD,
#endif
#ifdef PORTE
    PIN_PORTE,
#endif
#ifdef PORTF
    PIN_PORTF,
#endif
#ifdef PORTG
    PIN_PORTG,
#endif
    NUMBER_OF_PORTS
} PINS_portNumber_e;

typedef struct {
    PINS_portNumber_e port;
    uint8_t pin;
} PINS_pin_s;

void PIN_Direction(PINS_portNumber_e port, uint8_t pin, PINS_direction_e dir);
void PIN_Write(PINS_portNumber_e port, uint8_t pin, PINS_state_e state);
PINS_state_e PIN_Read(PINS_portNumber_e port, uint8_t pin);
void PIN_Pulll_Up(PINS_portNumber_e port, uint8_t pin, PINS_state_e state);
void PIN_Pulll_Down(PINS_portNumber_e port, uint8_t pin, PINS_state_e state);
void PINS_set_Interrupt(PINS_portNumber_e port, uint8_t pin, PINS_state_e state);

/**
 * sets pin as input or output
 * @param pin: pin number to select
 * @param direction: input (1) or output (0)
 */
#define PIN_direction(pin, direction) (PIN_CONCAT(_TRIS, pin) = direction)

/**
 * Sets output pin to high or low
 * @param pin: pin number to select
 * @param value: high (1) or low (0)
 */
#define PIN_write(pin,state) (PIN_CONCAT(_R,pin) = state)

/**
 * Toggle output pin to high or low
 * @param pin: pin number to toggle output
 */
#define PIN_toggle(pin) if (PIN_CONCAT(_LAT,pin)) PIN_CONCAT(_R,pin)=0;\
else PIN_CONCAT(_R,pin)=1;\

/**
 * reads value from latch
 * @param pin: pin to read from
 * @return success (1) or failure (0)
 */
#define PIN_read(pin) (PIN_CONCAT(_LAT,pin)

/**
 * sets internal pull-up resistor
 * @param pin: output pin to pull up
 * @ note can only be called on pin set as input
 */
#define PIN_pullUp(pin) (PIN_CONCAT(_CNPU,pin) = 1)

/**
 * sets internal pull-down resistor
 * @param pin: output pin to pull up
 * @ note can only be called on pin set as input
 */
#define PIN_pullDown(pin) (PIN_CONCAT(_CNPD,pin) = 1)

/**
 * Sets Interrupt on pin change
 * @param pin: pin to interrupt
 */
#define PIN_interrupt(pin) (PIN_CONCAT(_CNEN,pin) = 1)

#endif	/* PINS_H */

