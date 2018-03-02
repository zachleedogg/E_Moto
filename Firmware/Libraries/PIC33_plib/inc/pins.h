/* 
 * File:   pins.h
 * Author: Zachary S. Levenberg
 * Revision history: initial build 10/2/16
 */

#ifndef PINS_H
#define	PINS_H
/*******************************************************************************
 * Include
 * ****************************************************************************/
#include <xc.h>
#include <stdint.h>

/*******************************************************************************
 * Defines and Datatypes
 * ****************************************************************************/

#define PIN_CONCAT(A, B) A##B
#define PIN_DEF(A,B) (PINS_pin_S){PIN_PORT##A,B}

typedef enum {
    LOW,
    HIGH,
    TOGGLE
} PINS_internalRegisters_State_E;

typedef enum {
    OUTPUT,
    INPUT
} PINS_direction_E;

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
    PINS_NUMBER_OF_PORTS
} PINS_portNumber_E;

typedef struct {
    PINS_portNumber_E port;
    uint8_t pin;
} PINS_pin_S;

/**
 * 
 * @param port
 * @param pin
 * @param dir
 */
void PINS_direction(PINS_pin_S pin, PINS_direction_E dir);

/**
 * 
 * @param port
 * @param pin
 * @param state
 */
void PINS_write(PINS_pin_S pin, PINS_internalRegisters_State_E state);

/**
 * 
 * @param port
 * @param pin
 * @return 
 */
PINS_internalRegisters_State_E PINS_read(PINS_pin_S pin);

/**
 * 
 * @param port
 * @param pin
 * @param state
 */
void PINS_pullUp(PINS_pin_S pin, PINS_internalRegisters_State_E state);

/**
 * 
 * @param port
 * @param pin
 * @param state
 */
void PINS_pullDown(PINS_pin_S pin, PINS_internalRegisters_State_E state);

/**
 * 
 * @param port
 * @param pin
 * @param state
 */
void PINS_internalRegisters_SetInterrupt(PINS_pin_S pin, PINS_internalRegisters_State_E state);

/**
 * sets pin as input or output
 * @param pin: pin number to select
 * @param direction: input (1) or output (0)
 */
#define PINS_Direction(pin, direction) (PIN_CONCAT(_TRIS, pin) = direction)

/**
 * Sets output pin to high or low
 * @param pin: pin number to select
 * @param value: high (1) or low (0)
 */
#define PINS_Write(pin,state) (PIN_CONCAT(_R,pin) = state)

/**
 * Toggle output pin to high or low
 * @param pin: pin number to toggle output
 */
#define PIN_Toggle(pin) if (PIN_CONCAT(_LAT,pin)) PIN_CONCAT(_R,pin)=0;\
else PIN_CONCAT(_R,pin)=1;\

/**
 * reads value from latch
 * @param pin: pin to read from
 * @return success (1) or failure (0)
 */
#define PINS_Read(pin) (PIN_CONCAT(_LAT,pin)

/**
 * sets internal pull-up resistor
 * @param pin: output pin to pull up
 * @ note can only be called on pin set as input
 */
#define PIN_PullUp(pin) (PIN_CONCAT(_CNPU,pin) = 1)

/**
 * sets internal pull-down resistor
 * @param pin: output pin to pull up
 * @ note can only be called on pin set as input
 */
#define PIN_PullDown(pin) (PIN_CONCAT(_CNPD,pin) = 1)

/**
 * Sets Interrupt on pin change
 * @param pin: pin to interrupt
 */
#define PIN_SetInterrupt(pin) (PIN_CONCAT(_CNEN,pin) = 1)

#endif	/* PINS_H */

