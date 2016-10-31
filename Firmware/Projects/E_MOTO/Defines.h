/* 
 * File:   Defines.h
 * Author: Zach Levenberg
 *
 * Created on June 15, 2016, 5:08 PM
 */

#ifndef DEFINES_H
#define	DEFINES_H

#define USE_UART
#ifdef USE_UART
#include "bolt_uart.h"
#endif
#include "bolt_init.h"
#include "bolt_ADC.h"
#include "TFT_LCD.h"
#include "TFT_TOUCH.h"
#include "TFT_DISPLAY.h"
#include "pins.h"

/*******************************************************************************
 * PIN DEFINITIONS
 */
#define DEFINES_TOUCH_X0 (PINS_pin_S){PIN_PORTA,0}
#define DEFINES_TOUCH_X1 (PINS_pin_S){PIN_PORTB,2}
#define DEFINES_TOUCH_Y0 (PINS_pin_S){PIN_PORTA,1}
#define DEFINES_TOUCH_Y1 (PINS_pin_S){PIN_PORTB,3}
#define DEFINES_TOUCH_AN_X AN5
#define DEFINES_TOUCH_AN_Y AN4

#define DEFINES_TFT_LCD_RESET (PINS_pin_S){PIN_PORTB,12}
#define DEFINES_TFT_LCD_CS (PINS_pin_S){PIN_PORTB,10}
#define DEFINES_TFT_LCD_DC (PINS_pin_S){PIN_PORTB,11}

#define DEFINES_UART_TX UART_TX_RP36
#define DEFINES_UART_RX UART_RX_RP20
#define DEFINES_UART_BAUD BAUD230400


/*******************************************************************************
 * TIMER DEFINES
 */

#define TOUCH_TIMER 0
#define TRANSITION_TIMER 1
#define SPEEDO_TIMER 2

#define TOUCH_TIME 1
#define WAKE_UP_TOUCH_TIME 1500
#define SPEEDO_TIME 250


#endif	/* DEFINES_H */

