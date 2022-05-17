
#include <stddef.h>

/*
 * File:   ev_charger.c
 * Author: zachleedogg
 *
 * Created on May 15, 2022, 11:21 PM
 */
#include "ev_charger.h"
#include "bms_dbc.h"
#include "IO.h"


void EV_CHARGER_run_100ms(void){
    uint8_t chargeRequest = CAN_mcu_command_ev_charger_enable_get();
            
    static uint32_t last_valid_message = 0;
    if (CAN_mcu_command_checkDataIsFresh()){
        last_valid_message = SysTick_Get();
    }
    uint8_t message_valid = (SysTick_Get() - last_valid_message) < 1000 ? 1 : 0;
    
    if (CAN_mcu_command_DCDC_enable_get() && message_valid){
        IO_SET_EV_CHARGER_EN(HIGH);
        CAN_bms_status_2_EV_charger_state_set(HIGH);
    } else {
        IO_SET_EV_CHARGER_EN(LOW);
        CAN_bms_status_2_EV_charger_state_set(LOW);
    }
    CAN_bms_status_2_EV_charger_voltage_set(IO_GET_DCDC_OUTPUT_VOLTAGE());
    CAN_bms_status_2_EV_charger_current_set(IO_GET_DCDC_CURRENT());
}