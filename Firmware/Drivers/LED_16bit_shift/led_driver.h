/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LED_DRIVER_H
#define	LED_DRIVER_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>

#include "pins.h"

/*These are the names of each 10 segment LED arrays*/
typedef enum _ledArray {
    POWER_GAUGE,
    BATT_GAUGE
} ledArray;

/*Select pins for CLK and DIN*/

/*Not all of these are available on every device, check pin-out sheet*/

typedef enum _led_pin_number {
    RP20_LED,
    RP35_LED,
    RP36_LED,
    RP37_LED,
    RP38_LED,
    RP39_LED,
    RP40_LED,
    RP41_LED,
    RP42_LED,
    RP43_LED,
    RP54_LED,
    RP55_LED,
    RP56_LED,
    RP57_LED,
    RP97_LED,
    RP118_LED,
    RP120_LED,
    NUMBER_OF_LED_PINS,
} led_pin_number;

/*************************************************************************
 * Select pins for LE and OE using IO pins library
 * pin_number
 * IO_PIN_RA0
 * IO_PIN_RA1
 * IO_PIN_RA2
 * IO_PIN_RA3
 * IO_PIN_RA4
 * IO_PIN_RB0
 * IO_PIN_RB1
 * IO_PIN_RB2
 * IO_PIN_RB3
 * IO_PIN_RB4
 * IO_PIN_RB5
 * IO_PIN_RB6
 * IO_PIN_RB7
 * IO_PIN_RB8
 * IO_PIN_RB9
 * IO_PIN_RB10
 * IO_PIN_RB11
 * IO_PIN_RB12
 * IO_PIN_RB13
 * IO_PIN_RB14
 * IO_PIN_RB15
 */

/*Bit Mask Patterns for LEDs*/
static const uint16_t sweepPattern[] = {
    0b0000000000,
    0b1000000000,
    0b1100000000,
    0b1110000000,
    0b1111000000,
    0b1111100000,
    0b1111110000,
    0b1111111000,
    0b1111111100,
    0b1111111110,
    0b1111111111
};

/**
 * This module is Initialized using the SPI2 module
 * @param clk Clock pin on SPI 2 module,
 * @param Din Data pin on SPI 2 module,
 * @param Lat Lat pin is a general IO pin
 * @return success or failure
 */
uint8_t ledDriverInit(led_pin_number clk, led_pin_number Din, PINS_pin_S Lat);

/**
 * ledDriverWrite writes a 16 bit pattern to the specified display.
 * There are two displays daisy-chained together, calling the function uses a 
 * read-modify-write operation, hence writing to one driver will not affect 
 * the other.
 * @param thisArray choose between POWER_GAUGE or BATTERY_GUAGE
 * @param pattern specified 10-bit pattern
 * @return success or failure
 */
void ledDriverWrite(ledArray thisArray, uint16_t pattern);

/**
 * checks if the driver is busy
 * @return Busy (1) or Not Busy (0)
 */
uint8_t ledDriverIsBusy(void);

#endif	/* LED_DRIVER_H */

