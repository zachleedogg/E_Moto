/*
 * File:   can_populate.c
 * Author: zachleedogg
 *
 * Created on May 15, 2022, 11:21 PM
 */
#include "canPopulate.h"
#include "mcu_dbc.h"
#include "IO.h"
#include "movingAverage.h"


void CAN_populate_1ms(void) {
    Nop();
}

void CAN_populate_10ms(void) {
    static uint8_t heartbeat = 0;
    CAN_mcu_status_heartbeat_set(heartbeat++);
    if (heartbeat >= 16) {
        heartbeat = 0;
    }
    
    CAN_mcu_command_DCDC_enable_set(1);
}

void CAN_populate_100ms(void) {
    /*CAN status messages for Lights. Use the status of the output, not the switch,
     to determine the status of the light*/
    CAN_mcu_status_highBeam_set(IO_GET_HEADLIGHT_HI_EN());
    CAN_mcu_status_lowBeam_set(IO_GET_HEADLIGHT_LO_EN());
    CAN_mcu_status_tailLight_set(IO_GET_TAILLIGHT_EN());
    CAN_mcu_status_brakeLight_set(IO_GET_BRAKE_LIGHT_EN());
    CAN_mcu_status_turnSignalFL_set(IO_GET_TURN_SIGNAL_FL_EN());
    CAN_mcu_status_turnSignalFR_set(IO_GET_TURN_SIGNAL_FR_EN());
    CAN_mcu_status_turnSignalRL_set(IO_GET_TURN_SIGNAL_RL_EN());
    CAN_mcu_status_turnSignalRR_set(IO_GET_TURN_SIGNAL_RR_EN());
    CAN_mcu_status_killSwitch_set(IO_GET_KILL_SWITCH_IN());
    CAN_mcu_status_ignitionSwitch_set(IO_GET_IGNITION_SWITCH_IN());
    CAN_mcu_status_leftTurnSwitch_set(IO_GET_TURN_LEFT_SWITCH_IN());
    CAN_mcu_status_rightTurnSwitch_set(IO_GET_TURN_RIGHT_SWITCH_IN());
    CAN_mcu_status_lightSwitch_set(IO_GET_BRIGHTS_SWITCH_IN());
    CAN_mcu_status_assSwitch_set(IO_GET_SPARE_SWITCH_2_IN());
    CAN_mcu_status_hornSwitch_set(IO_GET_HORN_SWITCH_IN());
    CAN_mcu_status_batt_voltage_set(IO_GET_VOLTAGE_VBAT());
    NEW_LOW_PASS_FILTER(battery_current, 0.3, 10.0);
    NEW_LOW_PASS_FILTER(dcdc_current, 0.3, 10.0);
    CAN_mcu_status_batt_current_set(takeLowPassFilter(battery_current, IO_GET_CURRENT_BATT()));
    CAN_mcu_status_dcdc_current_set(takeLowPassFilter(dcdc_current, IO_GET_CURRENT_DCDC()));
    CAN_mcu_status_dcdc_fault_set(IO_GET_DCDC_FAULT());
    CAN_mcu_status_batt_fault_set(IO_GET_BATT_FAULT());
}

void CAN_populate_1000ms(void) {
    Nop();
}

