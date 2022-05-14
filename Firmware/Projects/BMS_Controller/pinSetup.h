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

#define DEBUG_LED_EN PIN_DEF(B,6)
#define SW_EN PIN_DEF(B,14)
#define CAN_SLEEP_EN PIN_DEF(B,15)

/*ANALOG*/
#define V12_MONITOR_AI AN0


/*PWM*/

/*COMUNICATION*/
//#define UART_TX RP120_OUT
//#define UART_RX RPI121_IN
#define CAN_TX RP36_OUT
#define CAN_RX RPI20_IN

#define TFT_LCD_CS PIN_DEF(B,10)
#define TFT_LCD_DC PIN_DEF(B,11)
#define TFT_LCD_RESET PIN_DEF(B,12)

#define TOUCH_X0 PIN_DEF(A,2)
#define TOUCH_Y0 PIN_DEF(A,3)
#define TOUCH_X1 PIN_DEF(B,0)
#define TOUCH_Y1 PIN_DEF(B,1)
#define TOUCH_AN_X AN3 /*THIS PIN MUST BE SAME AS Y1*/
#define TOUCH_AN_Y AN2 /*THIS PIN MUST BE SAME AS X1*/

/**
 * @PinSetup_Init will initialize all pins for their intended usage as defined above
 */
void PinSetup_Init(void);


#endif	/* PINSETUP_H */

