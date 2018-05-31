/* 
 * File:   Output Compare
 * Author: Zachary Levenberg
 * Comments: This is a PWM library
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BOLT_OC_H
#define	BOLT_OC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

/*****************************************************************************
 * Use these DEFINEs to select PWM pins
 */
//#define PWM_PIN_RP35 7
//#define PWM_PIN_RP36 11
//#define PWM_PIN_RP20 12
//#define PWM_PIN_RP37 14
//#define PWM_PIN_RP38 15
//#define PWM_PIN_RP39 16
//#define PWM_PIN_RP40 17
//#define PWM_PIN_RP41 18
//#define PWM_PIN_RP42 21
//#define PWM_PIN_RP43 22

/*Select pins for TX*/

/*Not all of these are available on every device, check pin-out sheet*/

typedef enum _oc_pin_number {
    PWM_PIN_RP20,
    PWM_PIN_RP35,
    PWM_PIN_RP36,
    PWM_PIN_RP37,
    PWM_PIN_RP38,
    PWM_PIN_RP39,
    PWM_PIN_RP40,
    PWM_PIN_RP41,
    PWM_PIN_RP42,
    PWM_PIN_RP43,
    PWM_PIN_RP54,
    PWM_PIN_RP55,
    PWM_PIN_RP56,
    PWM_PIN_RP57,
    PWM_PIN_RP100,
    PWM_PIN_RP101,
    NUMBER_OF_OC_PINS,
} oc_pin_number;


/**
 * 
 * @param pin: assigns a PWM output to pins
 * 7, 11, 12, 14, 15, 16, 17, 18, 21 or 22
 * @return success (1) if the module has been added, failure (0) if
 * the pin is invalid or there are no more modules available
 */
uint8_t pwmOCinit(oc_pin_number pin);


/**
 * 
 * @param pin: select to which pin to write the duty cycle
 * @param dutyCycle: select the percent duty cycle from 0-100
 * @return success (1) if pin is written, or (0) if duty is out of range
 */
uint8_t pwmOCwriteDuty(oc_pin_number pin, uint16_t dutyCycle);

#endif	/* BOLT_OC_H */

