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

//Outputs
#define DEBUG_LED_EN PIN_DEF(C,5)
#define DEBUG_PIN PIN_DEF(B,14)
#define DCDC_EN PIN_DEF(B,7)
#define EV_CHARGER_EN PIN_DEF(B,8)
#define DISCHARGE_EN PIN_DEF(B,11)
#define PRE_CHARGE_EN PIN_DEF(B,12)
#define SW_EN PIN_DEF(B,4)
#define MUX_A PIN_DEF(E,15)
#define MUX_B PIN_DEF(E,14)
#define MUX_C PIN_DEF(E,13)

//Inputs
#define V5_SW_nFAULT PIN_DEF(A,8)
#define V12_POWER_STATUS PIN_DEF(D,8)
#define DCDC_nFAULT PIN_DEF(D,6)
#define EV_CHARGER_nFAULT PIN_DEF(D,5)

/*ANALOG*/
#define ISOLATION_VOLTAGE_AI AN0
#define HV_BUS_VOLTAGE_AI AN1
#define EV_CHARGER_CURRENT_AI AN2
#define DCDC_OUTPUT_CURRENT_AI AN3
#define EV_CHARGER_VOLTAGE_AI AN4
#define DCDC_OUTPUT_VOLTAGE_AI AN5
#define HIGH_CURRENT_SHUNT_AI AN6
#define LOW_CURRENT_SHUNT_AI AN10
#define TRANSDUCER_INPUT_AI AN9
#define MUX_1_AI AN8
#define MUX_2_AI AN11
#define MUX_3_AI AN12


/*PWM*/
#define CHARGE_PUMP_PWM PWM_PIN_RP41
#define CONTACTOR_1_PWM PWM_PIN_RP57
#define CONTACTOR_2_PWM PWM_PIN_RP56

/*COMUNICATION*/
//Uart
#define UART_TX RP97_OUT
#define UART_RX RPI96_IN

//CAN
#define CAN_TX RP118_OUT
#define CAN_RX RPI119_IN

//SPI
#define SPI_CS PIN_DEF(C,4)


/**
 * @PinSetup_Init will initialize all pins for their intended usage as defined above
 */
void PinSetup_Init(void);


#endif	/* PINSETUP_H */

