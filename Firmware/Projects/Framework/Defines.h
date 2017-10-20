/* 
 * File:   Defines.h
 * Author: Zach Levenberg
 *
 * Created on June 15, 2016, 5:08 PM
 */

#ifndef DEFINES_H
#define	DEFINES_H

/*******************************************************************************
 * Library Includes
 */
#include "bolt_init.h"
#include "bolt_uart.h"


/*******************************************************************************
 * System Defines
 */
#define DEFINES_SYSTEM_CLOCK_FREQ FREQ_120MHZ
#define DEFINES_SYSTEM_CLOCK_SOURCE EXTERNAL


/*******************************************************************************
 * PIN DEFINITIONS
 */

#define DEFINES_UART_TX UART_TX_RP36
#define DEFINES_UART_RX UART_RX_RPI24
#define DEFINES_UART_BAUD UART_BAUD_230400



/*******************************************************************************
 * TIMER DEFINES
 */


#define TRANSITION_TIMER 0
#define SPEEDO_TIMER 2

#define WAKE_UP_TOUCH_TIME 1500
#define SPEEDO_TIME 100


#endif	/* DEFINES_H */

