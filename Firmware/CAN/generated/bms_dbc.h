#ifndef bms_DBC_H
#define bms_DBC_H

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
/**********************************************************
 * mcu NODE MESSAGES
 */
#define CAN_mcu_status_interval() 10
uint8_t CAN_mcu_status_checkDataIsFresh(void);
uint16_t CAN_mcu_status_heartbeat_get(void);
uint16_t CAN_mcu_status_state_get(void);
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
uint16_t CAN_mcu_status_killSwitch_get(void);
uint16_t CAN_mcu_status_ignitionSwitch_get(void);
uint16_t CAN_mcu_status_leftTurnSwitch_get(void);
uint16_t CAN_mcu_status_rightTurnSwitch_get(void);
uint16_t CAN_mcu_status_lightSwitch_get(void);
uint16_t CAN_mcu_status_assSwitch_get(void);
uint16_t CAN_mcu_status_hornSwitch_get(void);
float CAN_mcu_status_batt_voltage_get(void);

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
void CAN_bms_status_state_set(uint16_t state);
void CAN_bms_status_SOC_set(uint16_t SOC);
void CAN_bms_status_packVoltage_set(float packVoltage);
void CAN_bms_status_packCurrent_set(float packCurrent);
void CAN_bms_status_minTemp_set(float minTemp);
void CAN_bms_status_maxTemp_set(float maxTemp);
void CAN_bms_status_dlc_set(uint8_t dlc);


void CAN_bms_status_send(void);


#define CAN_bms_status_2_interval() 10
void CAN_bms_status_2_DCDC_state_set(uint16_t DCDC_state);
void CAN_bms_status_2_DCDC_fault_set(uint16_t DCDC_fault);
void CAN_bms_status_2_DCDC_voltage_set(float DCDC_voltage);
void CAN_bms_status_2_DCDC_current_set(float DCDC_current);
void CAN_bms_status_2_EV_charger_state_set(uint16_t EV_charger_state);
void CAN_bms_status_2_EV_charger_fault_set(uint16_t EV_charger_fault);
void CAN_bms_status_2_EV_charger_voltage_set(float EV_charger_voltage);
void CAN_bms_status_2_EV_charger_current_set(float EV_charger_current);
void CAN_bms_status_2_HV_precharge_state_set(uint16_t HV_precharge_state);
void CAN_bms_status_2_HV_isolation_voltage_set(float HV_isolation_voltage);
void CAN_bms_status_2_HV_contactor_state_set(uint16_t HV_contactor_state);
void CAN_bms_status_2_dlc_set(uint8_t dlc);


void CAN_bms_status_2_send(void);


#define CAN_bms_debug_interval() 10
void CAN_bms_debug_bool0_set(uint16_t bool0);
void CAN_bms_debug_bool1_set(uint16_t bool1);
void CAN_bms_debug_bool2_set(uint16_t bool2);
void CAN_bms_debug_bool3_set(uint16_t bool3);
void CAN_bms_debug_float1_set(float float1);
void CAN_bms_debug_float2_set(float float2);
void CAN_bms_debug_dlc_set(uint8_t dlc);


void CAN_bms_debug_send(void);


#define CAN_bms_boot_response_interval() 
void CAN_bms_boot_response_code_set(uint16_t code);
void CAN_bms_boot_response_type_set(uint16_t type);
void CAN_bms_boot_response_byte1_set(uint16_t byte1);
void CAN_bms_boot_response_byte2_set(uint16_t byte2);
void CAN_bms_boot_response_byte3_set(uint16_t byte3);
void CAN_bms_boot_response_byte4_set(uint16_t byte4);
void CAN_bms_boot_response_byte5_set(uint16_t byte5);
void CAN_bms_boot_response_byte6_set(uint16_t byte6);
void CAN_bms_boot_response_byte7_set(uint16_t byte7);
void CAN_bms_boot_response_dlc_set(uint8_t dlc);


void CAN_bms_boot_response_send(void);


#define CAN_bms_charger_request_interval() 1000
void CAN_bms_charger_request_output_voltage_high_byte_set(uint16_t output_voltage_high_byte);
void CAN_bms_charger_request_output_voltage_low_byte_set(uint16_t output_voltage_low_byte);
void CAN_bms_charger_request_output_current_high_byte_set(uint16_t output_current_high_byte);
void CAN_bms_charger_request_output_current_low_byte_set(uint16_t output_current_low_byte);
void CAN_bms_charger_request_start_charge_not_request_set(uint16_t start_charge_not_request);
void CAN_bms_charger_request_charge_mode_set(uint16_t charge_mode);
void CAN_bms_charger_request_byte_7_set(uint16_t byte_7);
void CAN_bms_charger_request_byte_8_set(uint16_t byte_8);
void CAN_bms_charger_request_dlc_set(uint8_t dlc);


void CAN_bms_charger_request_send(void);


#define CAN_bms_cellVoltages_interval() 1000
void CAN_bms_cellVoltages_MultiPlex_set(uint16_t MultiPlex);
void CAN_bms_cellVoltages_cell_1_voltage_set(float cell_1_voltage);
void CAN_bms_cellVoltages_cell_2_voltage_set(float cell_2_voltage);
void CAN_bms_cellVoltages_cell_3_voltage_set(float cell_3_voltage);
void CAN_bms_cellVoltages_cell_4_voltage_set(float cell_4_voltage);
void CAN_bms_cellVoltages_dlc_set(uint8_t dlc);


void CAN_bms_cellVoltages_send(void);


#define CAN_bms_cellTemperaturs_interval() 1000
void CAN_bms_cellTemperaturs_MultiPlex_set(uint16_t MultiPlex);
void CAN_bms_cellTemperaturs_temp_1_set(float temp_1);
void CAN_bms_cellTemperaturs_temp_2_set(float temp_2);
void CAN_bms_cellTemperaturs_temp_3_set(float temp_3);
void CAN_bms_cellTemperaturs_temp_4_set(float temp_4);
void CAN_bms_cellTemperaturs_dlc_set(uint8_t dlc);


void CAN_bms_cellTemperaturs_send(void);


/**********************************************************
 * motorcontroller NODE MESSAGES
 */
/**********************************************************
 * charger NODE MESSAGES
 */
#define CAN_charger_status_interval() 1000
uint8_t CAN_charger_status_checkDataIsFresh(void);
uint16_t CAN_charger_status_output_voltage_high_byte_get(void);
uint16_t CAN_charger_status_output_voltage_low_byte_get(void);
uint16_t CAN_charger_status_output_current_high_byte_get(void);
uint16_t CAN_charger_status_output_current_low_byte_get(void);
uint16_t CAN_charger_status_hardware_error_get(void);
uint16_t CAN_charger_status_charger_overtemp_error_get(void);
uint16_t CAN_charger_status_input_voltage_error_get(void);
uint16_t CAN_charger_status_battery_detect_error_get(void);
uint16_t CAN_charger_status_communication_error_get(void);
uint16_t CAN_charger_status_byte7_get(void);
uint16_t CAN_charger_status_byte8_get(void);

/**********************************************************
 * boot_host NODE MESSAGES
 */
#define CAN_boot_host_bms_interval() 1
uint8_t CAN_boot_host_bms_checkDataIsFresh(void);
uint16_t CAN_boot_host_bms_code_get(void);
uint16_t CAN_boot_host_bms_type_get(void);
uint16_t CAN_boot_host_bms_byte1_get(void);
uint16_t CAN_boot_host_bms_byte2_get(void);
uint16_t CAN_boot_host_bms_byte3_get(void);
uint16_t CAN_boot_host_bms_byte4_get(void);
uint16_t CAN_boot_host_bms_byte5_get(void);
uint16_t CAN_boot_host_bms_byte6_get(void);
uint16_t CAN_boot_host_bms_byte7_get(void);

void CAN_DBC_init();

void CAN_send_10ms(void);
void CAN_send_1000ms(void);


#endif /*bms_DBC_H*/
