/*
 * File:   can_populate.c
 * Author: zachleedogg
 *
 * Created on May 15, 2022, 11:21 PM
 */


#include "can_populate.h"
#include "bms_dbc.h"
#include "IO.h"
#include "ltc_6802.h"

void CAN_populate(void){
    
    CAN_bms_status_2_DCDC_state_set(IO_GET_DCDC_EN());
    CAN_bms_status_2_DCDC_fault_set(IO_GET_DCDC_FAULT());
    CAN_bms_status_2_DCDC_voltage_set(IO_GET_DCDC_OUTPUT_VOLTAGE());
    CAN_bms_status_2_DCDC_current_set(IO_GET_DCDC_CURRENT());
    CAN_bms_status_2_EV_charger_state_set(IO_GET_EV_CHARGER_EN());
    CAN_bms_status_2_EV_charger_fault_set(IO_GET_EV_CHARGER_FAULT());
    CAN_bms_status_2_EV_charger_voltage_set(IO_GET_EV_CHARGER_VOLTAGE());
    CAN_bms_status_2_EV_charger_current_set(IO_GET_EV_CHARGER_CURRENT());
    CAN_bms_status_2_HV_precharge_state_set(IO_GET_PRE_CHARGE_EN());
    //CAN_bms_status_2_HV_contactor_state_set(IO_GET_)
    CAN_bms_status_2_HV_bus_voltage_set(IO_GET_HV_BUS_VOLTAGE());
    
    static uint8_t cellVoltageMultiPlex = 0;
    CAN_bms_cellVoltages_MultiPlex_set(cellVoltageMultiPlex++);
    CAN_bms_cellVoltages_cell1_set(LTC6802_get_cell_voltage(1*cellVoltageMultiPlex));
    CAN_bms_cellVoltages_cell2_set(LTC6802_get_cell_voltage(2*cellVoltageMultiPlex));
    CAN_bms_cellVoltages_cell3_set(LTC6802_get_cell_voltage(3*cellVoltageMultiPlex));
    CAN_bms_cellVoltages_cell4_set(LTC6802_get_cell_voltage(4*cellVoltageMultiPlex));
            
    
}
        