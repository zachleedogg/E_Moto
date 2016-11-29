/* 
 * File:   ping.h
 * Author: kid group
 *
 * Created on September 10, 2016, 11:41 AM
 */

#include <stdint.h>

#ifndef PING_H
#define	PING_H

#include "pins.h"

typedef enum _ic_status {
    CHARGING,
    RESLEASED,
    RUNNING,
    DONE
} ic_status;

/*Not all of these are available on every device, check pin-out sheet*/
typedef enum {
    RP20_IC = 0x14,
    RPI24_IC = 0x18,
    RPI25_IC = 0x19,
    RPI27_IC = 0x1B,
    RPI28_IC = 0x1C,
    RPI32_IC = 0x20,
    RPI33_IC = 0x21,
    RPI34_IC = 0x22,
    RP35_IC = 0x23,
    RP36_IC = 0x24,
    RP37_IC = 0x25,
    RP38_IC = 0x26,
    RP39_IC = 0x27,
    RP40_IC = 0x28,
    RP41_IC = 0x29,
    RP42_IC = 0x2A,
    RP43_IC = 0x2B,
    RPI44_IC = 0x2C,
    RPI45_IC = 0x2D,
    RPI46_IC = 0x2E,
    RPI47_IC = 0x2F,
    RPI51_IC = 0x33,
    RPI52_IC = 0x34,
    RPI53_IC = 0x35,
    RP54_IC = 0x36,
    RP55_IC = 0x37,
    RP56_IC = 0x38,
    RP57_IC = 0x39,
    RPI58_IC = 0x3A,
} ic_pin_number;

/**
 * Initialize the PING SENSOR Driver. REQUIRES CLOCK TO BE INITIALIZED
 * @param rightEchoPin The ECHO pin on the SENSOR
 * @param R_TRIGGER the TRIGGER pin on the SENSOR
 * @param leftEchoPin The ECHO pin on the SENSOR
 * @param leftTriggerPin The TIGGER pin on the SENSOR
 * @return 
 */
uint8_t ping_Init(ic_pin_number rightEchoPin, PINS_pin_S R_TRIGGER, ic_pin_number leftEchoPin, PINS_pin_S leftTriggerPin);

/**
 * ping_Run must be called continuously to provide distance feedback data.
 * If called faster than ~6ms, the module will return the last knowm data
 * @return Distance from ojbect in inches up to 150in, else will return SNA (0xFF)
 */
uint8_t ping_Run(void);

uint8_t PING_getDistanceRight(uint8_t leftOrRight);

#endif	/* PING_H */

