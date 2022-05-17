#ifndef dash_DBC_H
#define dash_DBC_H

#include <stdint.h>
#include "bolt_CAN.h"
typedef enum{
    dash,
    mcu,
    bms,
    motorcontroller,
    charger,
    boot_host,
} CAN_nodes_E;

/**********************************************************
 * dash NODE MESSAGES
 */
void CAN_dash_status_heartBeat_set(uint16_t heartBeat);
void CAN_dash_status_state_set(uint16_t state);
void CAN_dash_status_killButton_set(uint16_t killButton);
void CAN_dash_status_ignButton_set(uint16_t ignButton);
void CAN_dash_status_modeButton_set(uint16_t modeButton);
void CAN_dash_status_selectButton_set(uint16_t selectButton);
void CAN_dash_status_driveMode_set(uint16_t driveMode);
void CAN_dash_status_send(void);


void CAN_dash_command_ignitionRequest_set(uint16_t ignitionRequest);
void CAN_dash_command_killRequest_set(uint16_t killRequest);
void CAN_dash_command_batteryEjectRequest_set(uint16_t batteryEjectRequest);
void CAN_dash_command_lightsRequest_set(uint16_t lightsRequest);
void CAN_dash_command_hornRequest_set(uint16_t hornRequest);
void CAN_dash_command_send(void);


void CAN_dash_data1_speed_set(uint16_t speed);
void CAN_dash_data1_odometer_set(uint16_t odometer);
void CAN_dash_data1_tripA_set(uint16_t tripA);
void CAN_dash_data1_tripB_set(uint16_t tripB);
void CAN_dash_data1_send(void);


void CAN_dash_data2_runningTime_set(uint16_t runningTime);
void CAN_dash_data2_odometer_set(uint16_t odometer);
void CAN_dash_data2_tripA_set(uint16_t tripA);
void CAN_dash_data2_tripB_set(uint16_t tripB);
void CAN_dash_data2_send(void);


/**********************************************************
 * mcu NODE MESSAGES
 */
uint8_t CAN_mcu_status_checkDataIsFresh(void);
uint16_t CAN_mcu_status_state_get(void);
uint16_t CAN_mcu_status_throttleMode_get(void);
uint16_t CAN_mcu_status_highBeam_get(void);
uint16_t CAN_mcu_status_lowBeam_get(void);
uint16_t CAN_mcu_status_brakeLight_get(void);
uint16_t CAN_mcu_status_tailLight_get(void);
uint16_t CAN_mcu_status_horn_get(void);
uint16_t CAN_mcu_status_turnSignalFR_get(void);
uint16_t CAN_mcu_status_turnSignalFL_get(void);
uint16_t CAN_mcu_status_turnSignalRR_get(void);
uint16_t CAN_mcu_status_turnSignalRL_get(void);
uint16_t CAN_mcu_status_GBSALL_get(void);
uint16_t CAN_mcu_status_contactor_get(void);
uint16_t CAN_mcu_status_chargePort_get(void);
uint16_t CAN_mcu_status_brakeSwitchFront_get(void);
uint16_t CAN_mcu_status_brakeSwitchRear_get(void);
uint16_t CAN_mcu_status_throttleVal_get(void);

uint8_t CAN_mcu_command_checkDataIsFresh(void);
uint16_t CAN_mcu_command_DCDC_enable_get(void);
uint16_t CAN_mcu_command_ev_charger_enable_get(void);
uint16_t CAN_mcu_command_motor_controller_enable_get(void);

/**********************************************************
 * bms NODE MESSAGES
 */
uint8_t CAN_bms_status_checkDataIsFresh(void);
uint16_t CAN_bms_status_state_get(void);
uint16_t CAN_bms_status_SOC_get(void);
uint16_t CAN_bms_status_packVoltage_get(void);
uint16_t CAN_bms_status_packCurrent_get(void);
uint16_t CAN_bms_status_minTemp_get(void);
uint16_t CAN_bms_status_maxTemp_get(void);

uint8_t CAN_bms_status_2_checkDataIsFresh(void);
uint16_t CAN_bms_status_2_DCDC_state_get(void);
uint16_t CAN_bms_status_2_DCDC_fault_get(void);
uint16_t CAN_bms_status_2_DCDC_voltage_get(void);
uint16_t CAN_bms_status_2_DCDC_current_get(void);
uint16_t CAN_bms_status_2_EV_charger_state_get(void);
uint16_t CAN_bms_status_2_EV_charger_fault_get(void);
uint16_t CAN_bms_status_2_EV_charger_voltage_get(void);
uint16_t CAN_bms_status_2_EV_charger_current_get(void);
uint16_t CAN_bms_status_2_HV_precharge_state_get(void);
uint16_t CAN_bms_status_2_HV_contactor_state_get(void);
uint16_t CAN_bms_status_2_HV_bus_voltage_get(void);

uint8_t CAN_bms_cellVoltages_checkDataIsFresh(void);
uint16_t CAN_bms_cellVoltages_MultiPlex_get(void);
uint16_t CAN_bms_cellVoltages_cell1_get(void);
uint16_t CAN_bms_cellVoltages_cell2_get(void);
uint16_t CAN_bms_cellVoltages_cell3_get(void);
uint16_t CAN_bms_cellVoltages_cell4_get(void);

/**********************************************************
 * motorcontroller NODE MESSAGES
 */
/**********************************************************
 * charger NODE MESSAGES
 */
/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init();

void CAN_send_10ms(void);


#endif /*dash_DBC_H*/
