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
//#include "bolt_sleep.h"
#include <xc.h>
#include "clock.h"

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
    PINS_direction(DEBUG_PIN, OUTPUT);
    PINS_direction(DCDC_EN, OUTPUT);
    PINS_direction(EV_CHARGER_EN, OUTPUT);
    PINS_direction(DISCHARGE_EN, OUTPUT);
    PINS_direction(PRE_CHARGE_EN, OUTPUT);
    PINS_direction(MUX_A, OUTPUT);
    PINS_direction(MUX_B, OUTPUT);
    PINS_direction(MUX_C, OUTPUT);
    
    PINS_direction(EV_CHARGER_nFAULT, INPUT);
    PINS_direction(DCDC_nFAULT,INPUT);
    /*ANALOG*/
    ADC_Init();
    ADC_SetPin(ISOLATION_VOLTAGE_AI);
    ADC_SetPin(HV_BUS_VOLTAGE_AI);
    ADC_SetPin(EV_CHARGER_CURRENT_AI);
    ADC_SetPin(DCDC_OUTPUT_CURRENT_AI);
    ADC_SetPin(EV_CHARGER_VOLTAGE_AI);
    ADC_SetPin(DCDC_OUTPUT_VOLTAGE_AI);
    ADC_SetPin(HIGH_CURRENT_SHUNT_AI);
    ADC_SetPin(LOW_CURRENT_SHUNT_AI);
    ADC_SetPin(TRANSDUCER_INPUT_AI);
    ADC_SetPin(MUX_1_AI);
    ADC_SetPin(MUX_2_AI);
    ADC_SetPin(MUX_3_AI);

    /*PWM*/
    pwmOCinit(CHARGE_PUMP_PWM);
    pwmOCwriteFreq(CHARGE_PUMP_PWM, 100); //600kHz
    pwmOCinit(CONTACTOR_1_PWM);
    pwmOCwriteFreq(CONTACTOR_1_PWM, 3000); //10kHz
    pwmOCinit(CONTACTOR_2_PWM);
    pwmOCwriteFreq(CONTACTOR_2_PWM, 3000); //20kHz
    /*UART*/
    //Uart1INIT(UART_TX, UART_RX, UART_BAUD_115200);
    
    /*CAN*/
    CAN_Init(CAN_TX, CAN_RX, CAN_BAUD_500k, CAN_DISABLE, CLOCK_SystemFrequencyGet());
    

}


