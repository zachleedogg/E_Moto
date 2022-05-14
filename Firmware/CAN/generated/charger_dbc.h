#ifndef charger_DBC_H
#define charger_DBC_H

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
uint8_t CAN_mcu_command_checkDataIsFresh(void);
uint16_t CAN_mcu_command_DCDC_enable_get(void);
uint16_t CAN_mcu_command_ev_charger_enable_get(void);
uint16_t CAN_mcu_command_motor_controller_enable_get(void);

/**********************************************************
 * bms NODE MESSAGES
 */
uint8_t CAN_bms_charger_request_checkDataIsFresh(void);
uint16_t CAN_bms_charger_request_output_voltage_high_byte_get(void);
uint16_t CAN_bms_charger_request_output_voltage_low_byte_get(void);
uint16_t CAN_bms_charger_request_output_current_high_byte_get(void);
uint16_t CAN_bms_charger_request_output_current_low_byte_get(void);
uint16_t CAN_bms_charger_request_start_charge_request_get(void);
uint16_t CAN_bms_charger_request_charge_mode_get(void);
uint16_t CAN_bms_charger_request_packCurrent_get(void);
uint16_t CAN_bms_charger_request_byte_7_get(void);
uint16_t CAN_bms_charger_request_byte_8_get(void);

/**********************************************************
 * motorcontroller NODE MESSAGES
 */
/**********************************************************
 * charger NODE MESSAGES
 */
void CAN_charger_status_output_voltage_high_byte_set(uint16_t output_voltage_high_byte);
void CAN_charger_status_output_voltage_low_byte_set(uint16_t output_voltage_low_byte);
void CAN_charger_status_output_current_high_byte_set(uint16_t output_current_high_byte);
void CAN_charger_status_output_current_low_byte_set(uint16_t output_current_low_byte);
void CAN_charger_status_hardware_error_set(uint16_t hardware_error);
void CAN_charger_status_charger_overtemp_error_set(uint16_t charger_overtemp_error);
void CAN_charger_status_input_voltage_error_set(uint16_t input_voltage_error);
void CAN_charger_status_battery_detect_error_set(uint16_t battery_detect_error);
void CAN_charger_status_communication_error_set(uint16_t communication_error);
void CAN_charger_status_byte7_set(uint16_t byte7);
void CAN_charger_status_byte8_set(uint16_t byte8);
void CAN_charger_status_send(void);


/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init();

void CAN_send_1000ms(void);


#endif /*charger_DBC_H*/
