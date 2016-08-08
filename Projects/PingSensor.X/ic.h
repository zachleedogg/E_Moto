/* 
 * File:   ic.h
 * Author: Zachary Levenberg
 *
 * Created on August 6, 2016, 9:28 AM
 */

#ifndef IC_H
#define	IC_H

#include <stdint.h>

typedef enum {
    IC_MODE_DISABLE,
    IC_MODE_ENABLE,
} ic_mode;

/*Select Pin for Input Capture*/

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
 * Init an Input Capture module on the specified Pin
 * @param newICPin from #define list above
 * @return success (1) or failure (0)
 */
uint8_t ic_Init(ic_pin_number newICPin);

/**
 * Set a module to Enabled or Disabled state
 * @param thisModule from pin list above
 * @param mode either IC_MODULE_ENABLE or IC_MODULE_DISABLE
 * @return 
 */
uint8_t ic_moduleMode(ic_pin_number thisModule, uint8_t mode);

/**
 * Check to see if pulse width has been captured
 * @return a 4-bit field corresponding to each IC module
 */
uint8_t ic_Ready(void);

/**
 * get the captured time from a specified module
 * @param thisModule a module from the #define list above
 * @return the 16bit time in Peripheral Clock Ticks
 */
uint16_t ic_getTime(ic_pin_number thisModule);


#endif	/* IC_H */

