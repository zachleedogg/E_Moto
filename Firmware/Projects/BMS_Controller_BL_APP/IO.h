/* 
 * File:   IO.h
 * Author: kid group
 *
 * Created on March 10, 2018, 10:04 PM
 */

#ifndef IO_H
#define	IO_H

#include "stdint.h"
#include "pins.h"


/**
 * SET_xxxx will set a digital output to the desired state
 * @param state: HIGH, LOW or TOGGLE
 */
void SET_DEBUG_LED_EN(uint8_t state);
void SET_SW_EN(uint8_t state);
void SET_CAN_SLEEP_EN(uint8_t state);

/**
 * GET_xxxx will return the state of a digital input or output
 * @return: HIGH or LOW
 */
uint8_t GET_DEBUG_LED_EN(void);
uint8_t GET_SW_EN(void);
uint8_t GET_CAN_SLEEP_EN(void);


/**
 * GET_VOLTAGE_xxx will return the converted voltage (including divider) in milliVolts
 * @return Voltage in milliVolts
 */
uint16_t GET_VOLTAGE_VBAT(void);


/**
 * IO_GET_xxx_FAULT returns the Fault Status of a given output. If the fault
 * bit is set, the output has been automatically turned off, and will not reset 
 * until the unit is power cycled or reset.
 * @return HIGH = Faulted, Low = Not Faulted
 */


#endif	/* IO_H */

