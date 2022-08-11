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
#define CAN_dash_status_interval() 10
void CAN_dash_status_heartBeat_set(uint16_t heartBeat);
void CAN_dash_status_state_set(uint16_t state);
void CAN_dash_status_killButton_set(uint16_t killButton);
void CAN_dash_status_ignButton_set(uint16_t ignButton);
void CAN_dash_status_modeButton_set(uint16_t modeButton);
void CAN_dash_status_selectButton_set(uint16_t selectButton);
void CAN_dash_status_driveMode_set(uint16_t driveMode);
void CAN_dash_status_dlc_set(uint8_t dlc);


void CAN_dash_status_send(void);


#define CAN_dash_command_interval() 10
void CAN_dash_command_ignitionRequest_set(uint16_t ignitionRequest);
void CAN_dash_command_killRequest_set(uint16_t killRequest);
void CAN_dash_command_batteryEjectRequest_set(uint16_t batteryEjectRequest);
void CAN_dash_command_lightsRequest_set(uint16_t lightsRequest);
void CAN_dash_command_hornRequest_set(uint16_t hornRequest);
void CAN_dash_command_dlc_set(uint8_t dlc);


void CAN_dash_command_send(void);


#define CAN_dash_data1_interval() 10
void CAN_dash_data1_speed_set(uint16_t speed);
void CAN_dash_data1_odometer_set(uint16_t odometer);
void CAN_dash_data1_tripA_set(uint16_t tripA);
void CAN_dash_data1_tripB_set(uint16_t tripB);
void CAN_dash_data1_dlc_set(uint8_t dlc);


void CAN_dash_data1_send(void);


#define CAN_dash_data2_interval() 10
void CAN_dash_data2_runningTime_set(uint16_t runningTime);
void CAN_dash_data2_odometer_set(uint16_t odometer);
void CAN_dash_data2_tripA_set(uint16_t tripA);
void CAN_dash_data2_tripB_set(uint16_t tripB);
void CAN_dash_data2_dlc_set(uint8_t dlc);


void CAN_dash_data2_send(void);


/**********************************************************
 * mcu NODE MESSAGES
 */
#define CAN_mcu_status_interval() 10
uint8_t CAN_mcu_status_checkDataIsFresh(void);
uint16_t CAN_mcu_status_heartbeat_get(void);
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

#define CAN_mcu_command_interval() 100
uint8_t CAN_mcu_command_checkDataIsFresh(void);
uint16_t CAN_mcu_command_DCDC_enable_get(void);
uint16_t CAN_mcu_command_ev_charger_enable_get(void);
float CAN_mcu_command_ev_charger_current_get(void);
uint16_t CAN_mcu_command_precharge_enable_get(void);
uint16_t CAN_mcu_command_motor_controller_enable_get(void);

/**********************************************************
 * bms NODE MESSAGES
 */
#define CAN_bms_status_interval() 10
uint8_t CAN_bms_status_checkDataIsFresh(void);
uint16_t CAN_bms_status_state_get(void);
uint16_t CAN_bms_status_SOC_get(void);
float CAN_bms_status_packVoltage_get(void);
float CAN_bms_status_packCurrent_get(void);
float CAN_bms_status_minTemp_get(void);
float CAN_bms_status_maxTemp_get(void);

#define CAN_bms_status_2_interval() 10
uint8_t CAN_bms_status_2_checkDataIsFresh(void);
uint16_t CAN_bms_status_2_DCDC_state_get(void);
uint16_t CAN_bms_status_2_DCDC_fault_get(void);
float CAN_bms_status_2_DCDC_voltage_get(void);
float CAN_bms_status_2_DCDC_current_get(void);
uint16_t CAN_bms_status_2_EV_charger_state_get(void);
uint16_t CAN_bms_status_2_EV_charger_fault_get(void);
float CAN_bms_status_2_EV_charger_voltage_get(void);
float CAN_bms_status_2_EV_charger_current_get(void);
uint16_t CAN_bms_status_2_HV_precharge_state_get(void);
float CAN_bms_status_2_HV_isolation_voltage_get(void);
uint16_t CAN_bms_status_2_HV_contactor_state_get(void);

#define CAN_bms_debug_interval() 10
uint8_t CAN_bms_debug_checkDataIsFresh(void);
uint16_t CAN_bms_debug_bool0_get(void);
uint16_t CAN_bms_debug_bool1_get(void);
uint16_t CAN_bms_debug_bool2_get(void);
uint16_t CAN_bms_debug_bool3_get(void);
float CAN_bms_debug_float1_get(void);
float CAN_bms_debug_float2_get(void);

#define CAN_bms_charger_request_interval() 1000
uint8_t CAN_bms_charger_request_checkDataIsFresh(void);
uint16_t CAN_bms_charger_request_output_voltage_high_byte_get(void);
uint16_t CAN_bms_charger_request_output_voltage_low_byte_get(void);
uint16_t CAN_bms_charger_request_output_current_high_byte_get(void);
uint16_t CAN_bms_charger_request_output_current_low_byte_get(void);
uint16_t CAN_bms_charger_request_start_charge_not_request_get(void);
uint16_t CAN_bms_charger_request_charge_mode_get(void);
uint16_t CAN_bms_charger_request_byte_7_get(void);
uint16_t CAN_bms_charger_request_byte_8_get(void);

#define CAN_bms_cellVoltages_interval() 1000
uint8_t CAN_bms_cellVoltages_checkDataIsFresh(void);
uint16_t CAN_bms_cellVoltages_MultiPlex_get(void);
float CAN_bms_cellVoltages_cell_1_voltage_get(void);
float CAN_bms_cellVoltages_cell_2_voltage_get(void);
float CAN_bms_cellVoltages_cell_3_voltage_get(void);
float CAN_bms_cellVoltages_cell_4_voltage_get(void);

#define CAN_bms_cellTemperaturs_interval() 1000
uint8_t CAN_bms_cellTemperaturs_checkDataIsFresh(void);
uint16_t CAN_bms_cellTemperaturs_MultiPlex_get(void);
float CAN_bms_cellTemperaturs_temp_1_get(void);
float CAN_bms_cellTemperaturs_temp_2_get(void);
float CAN_bms_cellTemperaturs_temp_3_get(void);
float CAN_bms_cellTemperaturs_temp_4_get(void);

/**********************************************************
 * motorcontroller NODE MESSAGES
 */
/**********************************************************
 * charger NODE MESSAGES
 */
/**********************************************************
 * boot_host NODE MESSAGES
 */
#define CAN_boot_host_dash_interval() 1
uint8_t CAN_boot_host_dash_checkDataIsFresh(void);
uint16_t CAN_boot_host_dash_code_get(void);
uint16_t CAN_boot_host_dash_type_get(void);
uint16_t CAN_boot_host_dash_byte1_get(void);
uint16_t CAN_boot_host_dash_byte2_get(void);
uint16_t CAN_boot_host_dash_byte3_get(void);
uint16_t CAN_boot_host_dash_byte4_get(void);
uint16_t CAN_boot_host_dash_byte5_get(void);
uint16_t CAN_boot_host_dash_byte6_get(void);
uint16_t CAN_boot_host_dash_byte7_get(void);

void CAN_DBC_init();

void CAN_send_10ms(void);


#endif /*dash_DBC_H*/
