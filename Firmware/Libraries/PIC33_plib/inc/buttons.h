/* 
 * File:   buttons.h
 * Author: Zach
 *
 * Created on November 26, 2016, 5:22 PM
 */

#ifndef BUTTONS_H
#define	BUTTONS_H

#include "pins.h"

typedef enum {
    BUTTONS_STATUS_OFF,
    BUTTONS_STATUS_ON
} BUTTONS_status_E;

typedef enum {
    BUTTONS_NEGATIVE_LOGIC,
    BUTTONS_POSITIVE_LOGIC
} BUTTONS_logic_E;

typedef struct {
    uint8_t debounce;
    const uint8_t threshold;
    const BUTTONS_logic_E polarity;
    BUTTONS_status_E status;
    PINS_pin_S pin;
} BUTTONS_object_S;

BUTTONS_status_E BUTTONS_run(BUTTONS_object_S *thisButton);



#endif	/* BUTTONS_H */

