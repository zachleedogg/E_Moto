/* 
 * File:   pinSetup.h
 * Author: kid group
 *
 * Created on March 1, 2018, 8:04 AM
 */

#ifndef PINSETUP_H
#define	PINSETUP_H

#include <stdint.h>
#include "pins.h"

/*DIGITAL INPUTS AND OUTPUTS*/

#define DEBUG_LED_EN PIN_DEF(C,5)
#define SW_EN PIN_DEF(B,4)

/*ANALOG*/
#define V12_MONITOR_AI AN0


/*PWM*/

/*COMUNICATION*/
//#define UART_TX RP120_OUT
//#define UART_RX RPI121_IN
#define CAN_TX RP118_OUT
#define CAN_RX RPI119_IN


/**
 * @PinSetup_Init will initialize all pins for their intended usage as defined above
 */
void PinSetup_Init(void);


#endif	/* PINSETUP_H */

