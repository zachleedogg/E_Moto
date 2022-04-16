/* 
 * File:   pinSetup.c
 * Author: Zach Levenberg
 * Comments: Pin setup for projects
 * Revision history: 3/1/18 initial build
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
#include "pinSetup.h"
#include "bolt_ADC.h"
#include "bolt_uart.h"
#include "bolt_CAN.h"
#include "pins.h"
#include "bolt_OC.h"
#include "bolt_pps.h"
#include "TFT_LCD.h"
#include "TOUCH_SCREEN.h"
//#include "bolt_sleep.h"
#include <xc.h>

// *****************************************************************************
// *****************************************************************************
// Section: Module Defines and variables
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Function Declarations
// *****************************************************************************
// *****************************************************************************

/**
 * 
 */
void PinSetup_Init(void) {
    /* Configure all AD pins as digital */
#ifdef ANSELA
    ANSELA = 0x0000;
#endif
#ifdef ANSELB
    ANSELB = 0x0000;
#endif
#ifdef ANSELC
    ANSELC = 0x0000;
#endif
#ifdef ANSELD
    ANSELD = 0x0000;
#endif
#ifdef ANSELE
    ANSELE = 0x0000;
#endif
#ifdef ANSELF
    ANSELF = 0x0000;
#endif
#ifdef ANSELG
    ANSELG = 0x0000;
#endif


    /* Set the PWM's off */
#ifdef IOCON1
    IOCON1bits.PENH = 0;
    IOCON1bits.PENL = 0;
#endif
#ifdef IOCON2
    IOCON2bits.PENH = 0;
    IOCON2bits.PENL = 0;
#endif
#ifdef IOCON3
    IOCON3bits.PENH = 0;
    IOCON3bits.PENL = 0;
#endif

    /*Set all digital IO*/
    PINS_direction(DEBUG_LED_EN, OUTPUT);
    PINS_direction(SW_EN, OUTPUT);
    PINS_direction(CAN_SLEEP_EN, OUTPUT);
    
    /*ANALOG*/
    ADC_Init();
    ADC_SetPin(V12_MONITOR_AI);

    /*PWM*/

    /*UART*/
//    Uart1INIT(UART_TX, UART_RX, UART_BAUD_115200);
    
    /*CAN*/
    CAN_Init(CAN_TX, CAN_RX, CAN_BAUD_500k, CAN_NORMAL);
    
    /*LCD Screen INIT*/
    TOUCH_SCREEN_INIT(TOUCH_X0, TOUCH_X1, TOUCH_Y0, TOUCH_Y1, TOUCH_AN_X, TOUCH_AN_Y);
    TFT_LCD_init(TFT_LCD_RESET, TFT_LCD_CS, TFT_LCD_DC);

}


