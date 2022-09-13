/* 
 * File:   IO.h
 * Author: kid group
 *
 * Created on March 10, 2018, 10:04 PM
 */

#ifndef IO_H
#define	IO_H

#include "stdint.h"
#include "pins.h"

/**
 * Initializes Module
 */
void IO_Efuse_Init(void);

/**
 * Run function must be called periodically by scheduler. recommend 1-10ms loop
 */
void IO_Efuse_Run_10ms(void);

/**
 * Disables Module even if RUN is called
 */
void IO_Efuse_Halt(void);

/**
 * IO_SET_xxxx will set a digital output to the desired state
 * @param state: HIGH, LOW or TOGGLE
 */
void IO_SET_DEBUG_LED_EN(uint8_t state);
void IO_SET_DEBUG_PIN_EN(uint8_t state);
void IO_SET_BMS_CONTROLLER_EN(uint8_t state);
void IO_SET_J1772_CONTROLLER_EN(uint8_t state);
void IO_SET_MOTOR_CONTROLLER_EN(uint8_t state);
void IO_SET_CHARGE_CONTROLLER_EN(uint8_t state);
void IO_SET_PILOT_EN(uint8_t state);
void IO_SET_BRAKE_LIGHT_EN(uint8_t state);
void IO_SET_TAILLIGHT_EN(uint8_t state);
void IO_SET_HEADLIGHT_HI_EN(uint8_t state);
void IO_SET_HEADLIGHT_LO_EN(uint8_t state);
void IO_SET_CHARGER_CONTACTOR_EN(uint8_t state);
void IO_SET_DCDC_CONTACTOR_EN(uint8_t state);
void IO_SET_BATTERY_CONTACTOR_EN(uint8_t state);
void IO_SET_PRECHARGE_RELAY_EN(uint8_t state);
void IO_SET_HORN_EN(uint8_t state);
void IO_SET_AUX_PORT_EN(uint8_t state);
void IO_SET_TURN_SIGNAL_FR_EN(uint8_t state);
void IO_SET_TURN_SIGNAL_RR_EN(uint8_t state);
void IO_SET_TURN_SIGNAL_FL_EN(uint8_t state);
void IO_SET_TURN_SIGNAL_RL_EN(uint8_t state);
void IO_SET_HEATED_GRIPS_EN(uint8_t state);
void IO_SET_HEATED_SEAT_EN(uint8_t state);
void IO_SET_SW_EN(uint8_t state);
void IO_SET_DIAG_EN(uint8_t state);
void IO_SET_DIAG_SELECT_EN(uint8_t state);
void IO_SET_DCDC_EN(uint8_t state);
void IO_SET_BATT_EN(uint8_t state);
void IO_SET_CAN_SLEEP_EN(uint8_t state);
void IO_SET_IC_CONTROLLER_SLEEP_EN(uint8_t state);
void IO_SET_PUMP_1_EN(uint8_t state);
void IO_SET_FAN_1_EN(uint8_t state);

/**
 * IO_GET_xxxx will return the state of a digital input or output
 * @return: HIGH or LOW
 */
uint8_t IO_GET_DEBUG_LED_EN(void);
uint8_t IO_GET_DEBUG_PIN_EN(void);
uint8_t IO_GET_BMS_CONTROLLER_EN(void);
uint8_t IO_GET_J1772_CONTROLLER_EN(void);
uint8_t IO_GET_MOTOR_CONTROLLER_EN(void);
uint8_t IO_GET_CHARGE_CONTROLLER_EN(void);
uint8_t IO_GET_PILOT_EN(void);
uint8_t IO_GET_BRAKE_LIGHT_EN(void);
uint8_t IO_GET_TAILLIGHT_EN(void);
uint8_t IO_GET_HEADLIGHT_HI_EN(void);
uint8_t IO_GET_HEADLIGHT_LO_EN(void);
uint8_t IO_GET_CHARGER_CONTACTOR_EN(void);
uint8_t IO_GET_DCDC_CONTACTOR_EN(void);
uint8_t IO_GET_BATTERY_CONTACTOR_EN(void);
uint8_t IO_GET_PRECHARGE_RELAY_EN(void);
uint8_t IO_GET_HORN_EN(void);
uint8_t IO_GET_AUX_PORT_EN(void);
uint8_t IO_GET_TURN_SIGNAL_FR_EN(void);
uint8_t IO_GET_TURN_SIGNAL_RR_EN(void);
uint8_t IO_GET_TURN_SIGNAL_FL_EN(void);
uint8_t IO_GET_TURN_SIGNAL_RL_EN(void);
uint8_t IO_GET_HEATED_GRIPS_EN(void);
uint8_t IO_GET_HEATED_SEAT_EN(void);
uint8_t IO_GET_SW_EN(void);
uint8_t IO_GET_DIAG_EN(void);
uint8_t IO_GET_DIAG_SELECT_EN(void);
uint8_t IO_GET_DCDC_EN(void);
uint8_t IO_GET_BATT_EN(void);
uint8_t IO_GET_CAN_SLEEP_EN(void);
uint8_t IO_GET_IC_CONTROLLER_SLEEP_EN(void);
uint8_t IO_GET_PUMP_1_EN(void);
uint8_t IO_GET_FAN_1_EN(void);
uint8_t IO_GET_IC_CONTROLLER_FAULT(void);
uint8_t IO_GET_DCDC_FAULT(void);
uint8_t IO_GET_BATT_FAULT(void);
uint8_t IO_GET_TURN_SIGNAL_L_STATUS(void);
uint8_t IO_GET_TURN_SIGNAL_R_STATUS(void);
uint8_t IO_GET_CONTACT_1_2_STATUS(void);
uint8_t IO_GET_CONTACT_3_4_STATUS(void);
uint8_t IO_GET_SPARE_SWITCH_1_IN(void);
uint8_t IO_GET_SPARE_SWITCH_2_IN(void);
uint8_t IO_GET_BRAKE_SWITCH_1_IN(void);
uint8_t IO_GET_BRAKE_SWITCH_2_IN(void);
uint8_t IO_GET_IGNITION_SWITCH_IN(void);
uint8_t IO_GET_KILL_SWITCH_IN(void);
uint8_t IO_GET_TURN_LEFT_SWITCH_IN(void);
uint8_t IO_GET_TURN_RIGHT_SWITCH_IN(void);
uint8_t IO_GET_BRIGHTS_SWITCH_IN(void);
uint8_t IO_GET_HORN_SWITCH_IN(void);
uint8_t IO_GET_KICKSTAND_SWITCH_IN(void);

/**
 * IO_GET_CURRENT_xxx will return the Current in milliamps of the desired Output
 * @return Current in mA (milliamps)
 */
uint16_t IO_GET_CURRENT_FAN(void);
uint16_t IO_GET_CURRENT_PUMP(void);
uint16_t IO_GET_CURRENT_TAILLIGHT(void);
uint16_t IO_GET_CURRENT_BRAKELIGHT(void);
uint16_t IO_GET_CURRENT_LOWBEAM(void);
uint16_t IO_GET_CURRENT_HIGHBEAM(void);
uint16_t IO_GET_CURRENT_HORN(void);
uint16_t IO_GET_CURRENT_AUX_PORT(void);
uint16_t IO_GET_CURRENT_HEATED_GRIPS(void);
uint16_t IO_GET_CURRENT_HEATED_SEAT(void);
uint16_t IO_GET_CURRENT_CHARGE_CONTROLLER(void);
uint16_t IO_GET_CURRENT_MOTOR_CONTROLLER(void);
uint16_t IO_GET_CURRENT_BMS_CONTROLLER(void);
uint16_t IO_GET_CURRENT_SPARE_1_CONTROLLER(void);
float IO_GET_CURRENT_BATT(void);
float IO_GET_CURRENT_DCDC(void);
float IO_GET_CURRENT_IC_CONTROLLER(void);

/**
 * IO_GET_VOLTAGE_xxx will return the converted voltage (including divider) in milliVolts
 * @return Voltage in milliVolts
 */
float IO_GET_VOLTAGE_PILOT(void);
float IO_GET_VOLTAGE_PROXIMITY(void);
float IO_GET_VOLTAGE_VBAT(void);
float IO_GET_VOLTAGE_VBAT_SW(void);


/**
 * IO_GET_xxx_FAULT returns the Fault Status of a given output. If the fault
 * bit is set, the output has been automatically turned off, and will not reset 
 * until the unit is power cycled or reset.
 * @return HIGH = Faulted, Low = Not Faulted
 */
uint8_t IO_GET_FAN_FAULT(void);
uint8_t IO_GET_PUMP_FAULT(void);
uint8_t IO_GET_TAILLIGHT_FAULT(void);
uint8_t IO_GET_BRAKELIGHT_FAULT(void);
uint8_t IO_GET_LOWBEAM_FAULT(void);
uint8_t IO_GET_HIGHBEAM_FAULT(void);
uint8_t IO_GET_HORN_FAULT(void);
uint8_t IO_GET_AUX_PORT_FAULT(void);
uint8_t IO_GET_HEATED_GRIPS_FAULT(void);
uint8_t IO_GET_HEATED_SEAT_FAULT(void);
uint8_t IO_GET_CHARGE_CONTROLLER_FAULT(void);
uint8_t IO_GET_MOTOR_CONTROLLER_FAULT(void);
uint8_t IO_GET_BMS_CONTROLLER_FAULT(void);
uint8_t IO_GET_SPARE_1_CONTROLLER_FAULT(void);

#endif	/* IO_H */

