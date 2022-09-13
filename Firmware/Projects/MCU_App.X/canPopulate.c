/*
 * File:   can_populate.c
 * Author: zachleedogg
 *
 * Created on May 15, 2022, 11:21 PM
 */
#include <math.h>

#include "canPopulate.h"
#include "mcu_dbc.h"
#include "IO.h"


void CAN_populate_1ms(void){
    
Nop();

    
}

void CAN_populate_10ms(void){
    Nop();
}

void CAN_populate_100ms(void){
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
    
    CAN_mcu_status_batt_voltage_set(IO_GET_VOLTAGE_VBAT_SW());
}

void CAN_populate_1000ms(void){
Nop();
}
        
