/* 
 * File:   sleep.h
 * Author: Zachary Levenberg
 * Comments: zzzzzzzzzzzzzzzzz
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SLEEP_H
#define	SLEEP_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

#include "pins.h"

/*WAKE UP LIST*/
#define WAKE_UP(INT) \
INT(WAKE_ON_UART1)\
INT(WAKE_ON_UART2)\
INT(WAKE_ON_CAN)\
INT(PIN)\

#define WAKE_FORM(STATE) STATE,

typedef enum {
    NO_SEL = -1,
    WAKE_UP(WAKE_FORM)
    NUMBER_OF_WAKE_TYPES
} wake_up_type;



/**
 * setWakeUp sets which module for the micro to Wake Up on.
 * @param type of wake signal from list above
 * @return success or failure
 */
uint8_t setWakeUp(wake_up_type thisType, PINS_pin_S pin);

void SleepNow();

#endif	/* XC_HEADER_TEMPLATE_H */

