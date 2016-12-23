/* 
 * File:   Defines.h
 * Author: Zach Levenberg
 *
 * Created on June 15, 2016, 5:08 PM
 */

#ifndef DEFINES_H
#define	DEFINES_H

#ifdef USE_UART
#include "bolt_uart.h"
#endif
#include "bolt_init.h"
#include "bolt_ADC.h"
#include "pins.h"
#include "bolt_i2c.h"
#include "buttons.h"
#include "bolt_uart.h"

#include "ping.h"
#include "TFT_LCD.h"
#include "TFT_TOUCH.h"
#include "TFT_DISPLAY.h"
#include "led_driver.h"

#include "..\..\CAN\generated\dbc.h"
#include "bolt_CAN.h"

#define DEFINES_SYSTEM_CLOCK_FREQ FREQ_140MHZ

/*******************************************************************************
 * PIN DEFINITIONS
 */
#ifdef EVAL_1
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
#define DEFINES_UART_BAUD UART_BAUD_230400
#endif
#ifdef EVAL_2
#define DEFINES_TOUCH_X0 (PINS_pin_S){PIN_PORTA,9}
#define DEFINES_TOUCH_X1 (PINS_pin_S){PIN_PORTC,11}
#define DEFINES_TOUCH_Y0 (PINS_pin_S){PIN_PORTE,15}
#define DEFINES_TOUCH_Y1 (PINS_pin_S){PIN_PORTE,14}
#define DEFINES_TOUCH_AN_X AN14 /*THIS PIN MUST BE SAME AS Y1*/
#define DEFINES_TOUCH_AN_Y AN11 /*THIS PIN MUST BE SAME AS X1*/

#define VBATT_PROT_MONITOR AN2

#define DEFINES_TFT_LCD_RESET (PINS_pin_S){PIN_PORTB,1}
#define DEFINES_TFT_LCD_CS (PINS_pin_S){PIN_PORTG,9}
#define DEFINES_TFT_LCD_DC (PINS_pin_S){PIN_PORTC,2}

#define DEFINES_PING_LEFT_TRIGGER (PINS_pin_S){PIN_PORTD,8}
#define DEFINES_PING_LEFT_ECHO RPI46_IC
#define DEFINES_PING_RIGHT_TRIGGER (PINS_pin_S){PIN_PORTG,6}
#define DEFINES_PING_RIGHT_ECHO RPI47_IC

#define DEFINES_SW2 (PINS_pin_S){PIN_PORTB,9}
#define DEFINES_SW3 (PINS_pin_S){PIN_PORTC,6}

#define DEFINES_5V_SW_RAIL (PINS_pin_S){PIN_PORTA,10}
#define DEFINES_12V_SW_RAIL (PINS_pin_S){PIN_PORTA,7}

#define DEFINES_EEPROM_1_ADDRESS 0b1010000

#define DEFINES_LED_DRIVER_CLOCK RP42_LED
#define DEFINES_LED_DRIVER_DATA RP97_LED
#define DEFINES_LED_DRIVER_LATCH (PINS_pin_S){PIN_PORTF,0}
#define DEFINES_LED_DRIVER_BRIGHTNESS (PINS_pin_S){PIN_PORTC,9}

#define DEFINES_UART_TX UART_TX_RP36
#define DEFINES_UART_RX UART_RX_RPI24
#define DEFINES_UART_BAUD UART_BAUD_230400

#define DEFINES_CAN_TX CAN_TX_RP43
#define DEFINES_CAN_RX CAN_RX_RPI44
#define DEFINES_CAN_STBY (PINS_pin_S){PIN_PORTB,13}
#define DEFINES_CAN_BAUD CAN_BAUD_1M
#endif


/*******************************************************************************
 * TIMER DEFINES
 */


#define TRANSITION_TIMER 0
#define SPEEDO_TIMER 2

#define WAKE_UP_TOUCH_TIME 1500
#define SPEEDO_TIME 100


#endif	/* DEFINES_H */

