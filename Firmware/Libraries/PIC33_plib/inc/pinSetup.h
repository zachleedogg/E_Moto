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

void SET_DEBUG_LED_EN(uint8_t state);
void SET_DEBUG_PIN_EN(uint8_t state);
void SET_BMS_CONTROLLER_EN(uint8_t state);
void SET_J1772_CONTROLLER_EN(uint8_t state);
void SET_MOTOR_CONTROLLER_EN(uint8_t state);
void SET_CHARGE_CONTROLLER_EN(uint8_t state);
void SET_PILOT_EN(uint8_t state);
void SET_BRAKE_LIGHT_EN(uint8_t state);
void SET_TAILLIGHT_EN(uint8_t state);
void SET_HEADLIGHT_HI_EN(uint8_t state);
void SET_HEADLIGHT_LO_EN(uint8_t state);
void SET_CHARGER_CONTACTOR_EN(uint8_t state);
void SET_DCDC_CONTACTOR_EN(uint8_t state);
void SET_BATTERY_CONTACTOR_EN(uint8_t state);
void SET_PRECHARGE_RELAY_EN(uint8_t state);
void SET_HORN_EN(uint8_t state);
void SET_AUX_PORT_EN(uint8_t state);
void SET_TURN_SIGNAL_FR_EN(uint8_t state);
void SET_TURN_SIGNAL_RR_EN(uint8_t state);
void SET_TURN_SIGNAL_FL_EN(uint8_t state);
void SET_TURN_SIGNAL_RL_EN(uint8_t state);
void SET_HEATED_GRIPS_EN(uint8_t state);
void SET_HEATED_SEAT_EN(uint8_t state);
void SET_SW_EN(uint8_t state);
void SET_DIAG_EN(uint8_t state);
void SET_DIAG_SELECT_EN(uint8_t state);
void SET_DCDC_EN(uint8_t state);
void SET_BATT_EN(uint8_t state);
void SET_CAN_SLEEP_EN(uint8_t state);
void SET_IC_CONTROLLER_SLEEP_EN(uint8_t state);
void SET_PUMP_1_EN(uint8_t state);
void SET_FAN_1_EN(uint8_t state);


uint8_t GET_DEBUG_LED_EN(void);
uint8_t GET_DEBUG_PIN_EN(void);
uint8_t GET_BMS_CONTROLLER_EN(void);
uint8_t GET_J1772_CONTROLLER_EN(void);
uint8_t GET_MOTOR_CONTROLLER_EN(void);
uint8_t GET_CHARGE_CONTROLLER_EN(void);
uint8_t GET_PILOT_EN(void);
uint8_t GET_BRAKE_LIGHT_EN(void);
uint8_t GET_TAILLIGHT_EN(void);
uint8_t GET_HEADLIGHT_HI_EN(void);
uint8_t GET_HEADLIGHT_LO_EN(void);
uint8_t GET_CHARGER_CONTACTOR_EN(void);
uint8_t GET_DCDC_CONTACTOR_EN(void);
uint8_t GET_BATTERY_CONTACTOR_EN(void);
uint8_t GET_PRECHARGE_RELAY_EN(void);
uint8_t GET_HORN_EN(void);
uint8_t GET_AUX_PORT_EN(void);
uint8_t GET_TURN_SIGNAL_FR_EN(void);
uint8_t GET_TURN_SIGNAL_RR_EN(void);
uint8_t GET_TURN_SIGNAL_FL_EN(void);
uint8_t GET_TURN_SIGNAL_RL_EN(void);
uint8_t GET_HEATED_GRIPS_EN(void);
uint8_t GET_HEATED_SEAT_EN(void);
uint8_t GET_SW_EN(void);
uint8_t GET_DIAG_EN(void);
uint8_t GET_DIAG_SELECT_EN(void);
uint8_t GET_DCDC_EN(void);
uint8_t GET_BATT_EN(void);
uint8_t GET_CAN_SLEEP_EN(void);
uint8_t GET_IC_CONTROLLER_SLEEP_EN(void);
uint8_t GET_PUMP_1_EN(void);
uint8_t GET_FAN_1_EN(void);
uint8_t GET_IC_CONTROLLER_nFAULT(void);
uint8_t GET_DCDC_FAULT(void);
uint8_t GET_BATT_FAULT(void);
uint8_t GET_TURN_SIGNAL_L_STATUS(void);
uint8_t GET_TURN_SIGNAL_R_STATUS(void);
uint8_t GET_CONTACT_1_2_STATUS(void);
uint8_t GET_CONTACT_3_4_STATUS(void);
uint8_t GET_SPARE_SWITCH_1_IN(void);
uint8_t GET_SPARE_SWITCH_2_IN(void);
uint8_t GET_BRAKE_SWITCH_1_IN(void);
uint8_t GET_BRAKE_SWITCH_2_IN(void);
uint8_t GET_IGNITION_SWITCH_IN(void);
uint8_t GET_KILL_SWITCH_IN(void);
uint8_t GET_TURN_LEFT_SWITCH_IN(void);
uint8_t GET_TURN_RIGHT_SWITCH_IN(void);
uint8_t GET_BRIGHTS_SWITCH_IN(void);
uint8_t GET_HORN_SWITCH_IN(void);
uint8_t GET_KICKSTAND_SWITCH_IN(void);

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

void PinSetup_Init(void);


#endif	/* PINSETUP_H */

