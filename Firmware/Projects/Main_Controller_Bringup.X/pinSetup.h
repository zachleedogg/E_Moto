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

#define DEBUG_LED_EN PIN_DEF(A,0)
#define DEBUG_PIN_EN PIN_DEF(A,1)
#define BMS_CONTROLLER_EN PIN_DEF(A,2)
#define J1772_CONTROLLER_EN PIN_DEF(A,3)
#define MOTOR_CONTROLLER_EN PIN_DEF(A,4)
#define CHARGE_CONTROLLER_EN PIN_DEF(A,5)
#define PILOT_EN PIN_DEF(A,6)
#define TURN_SIGNAL_L_STATUS PIN_DEF(A,9)
#define TURN_SIGNAL_R_STATUS PIN_DEF(A,10)
#define CONTACT_1_2_STATUS PIN_DEF(A,14)
#define CONTACT_3_4_STATUS PIN_DEF(A,15)
#define SPARE_SWITCH_1_IN PIN_DEF(C,2)
#define SPARE_SWITCH_2_IN PIN_DEF(C,3)
#define BRAKE_LIGHT_EN PIN_DEF(D,0)
#define TAILLIGHT_EN PIN_DEF(D,1)
#define HEADLIGHT_HI_EN PIN_DEF(D,2)
#define HEADLIGHT_LO_EN PIN_DEF(D,3)
#define CHARGER_CONTACTOR_EN PIN_DEF(D,4)
#define DCDC_CONTACTOR_EN PIN_DEF(D,5)
#define BATTERY_CONTACTOR_EN PIN_DEF(D,6)
#define PRECHARGE_RELAY_EN PIN_DEF(D,7)
#define HORN_EN PIN_DEF(D,8)
#define AUX_PORT_EN PIN_DEF(D,9)
#define TURN_SIGNAL_FR_EN PIN_DEF(D,10)
#define TURN_SIGNAL_RR_EN PIN_DEF(D,11)
#define TURN_SIGNAL_FL_EN PIN_DEF(D,12)
#define TURN_SIGNAL_RL_EN PIN_DEF(D,13)
#define HEATED_GRIPS_EN PIN_DEF(D,14)
#define HEATED_SEAT_EN PIN_DEF(D,15)
#define SW_EN PIN_DEF(E,0)
#define DIAG_EN PIN_DEF(E,1)
#define DIAG_SELECT_EN PIN_DEF(E,2)
#define DCDC_EN PIN_DEF(E,3)
#define BATT_EN PIN_DEF(E,4)
#define CAN_SLEEP_EN PIN_DEF(E,5)
#define IC_CONTROLLER_SLEEP_EN PIN_DEF(E,6)
#define IC_CONTROLLER_nFAULT PIN_DEF(E,7)
#define DCDC_nFAULT PIN_DEF(E,8)
#define BATT_nFAULT PIN_DEF(E,9)
#define PUMP_1_EN PIN_DEF(F,2)
#define FAN_1_EN PIN_DEF(F,3)
#define BRAKE_SWITCH_1_IN PIN_DEF(F,13)
#define BRAKE_SWITCH_2_IN PIN_DEF(F,12)
#define IGNITION_SWITCH_IN PIN_DEF(G,1)
#define KILL_SWITCH_IN PIN_DEF(G,0)
#define TURN_LEFT_SWITCH_IN PIN_DEF(G,2)
#define TURN_RIGHT_SWITCH_IN PIN_DEF(G,3)
#define BRIGHTS_SWITCH_IN PIN_DEF(G,12)
#define HORN_SWITCH_IN PIN_DEF(G,13)
#define KICKSTAND_SWITCH_IN PIN_DEF(G,14)

/*ANALOG*/
#define FAN_ISENSE_AI AN0
#define THOTTLE_SIGNAL_MONITOR_AI AN1
#define TAILLIGHT_ISENSE_AI AN2
#define HEADLIGHT_ISENSE_AI AN3
#define HORN_ISENSE_AI AN4
#define AUX_PORT_ISENSE_AI AN5
#define LOCK_ISENSE_AI AN6
#define HEATER_ISENSE_AI AN7
#define ECU_2_ISENSE_AI AN8
#define ECU_1_ISENSE_AI AN9
#define V12_MONITOR_AI AN10
#define BATT_ISENSE_AI AN11
#define DCDC_ISENSE_AI AN12
#define IC_CONTROLLER_ISENSE_AI AN13
#define RADIATOR_INPUT_TEMP_AI AN14
#define RADIATOR_OUTPUT_TEMP_AI AN15
#define P12_MONITOR_AI AN16
#define PILOT_MONITOR_AI AN19 //R177 is wrong, should be 560k, not 10k
#define PROXIMITY_MONITOR_AI AN23

/*PWM*/
#define PWM_1_OUT RP100_OUT
#define PWM_2_OUT RP101_OUT

/*COMUNICATION*/
#define UART_TX RP120_OUT
#define UART_RX RPI121_IN
#define CAN_TX RP118_OUT
#define CAN_RX RPI119_IN

/**
 * @PinSetup_Init will initialize all pins for their intened usage as defined above
 */
void PinSetup_Init(void);


#endif	/* PINSETUP_H */
