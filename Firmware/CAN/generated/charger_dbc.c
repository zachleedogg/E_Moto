#include "charger_dbc.h"
#include "utils.h"
/**********************************************************
 * dash NODE MESSAGES
 */
/**********************************************************
 * mcu NODE MESSAGES
 */
#define CAN_mcu_command_ID 0x712

static CAN_message_S CAN_mcu_command={
	.canID = CAN_mcu_command_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_mcu_command_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_mcu_command);
}
#define CAN_MCU_COMMAND_DCDC_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_DCDC_ENABLE_OFFSET 0
#define CAN_MCU_COMMAND_EV_CHARGER_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_EV_CHARGER_ENABLE_OFFSET 1
#define CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_OFFSET 2

uint16_t CAN_mcu_command_DCDC_enable_get(void){
	return get_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_DCDC_ENABLE_OFFSET, CAN_MCU_COMMAND_DCDC_ENABLE_RANGE);
}
uint16_t CAN_mcu_command_ev_charger_enable_get(void){
	return get_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_EV_CHARGER_ENABLE_OFFSET, CAN_MCU_COMMAND_EV_CHARGER_ENABLE_RANGE);
}
uint16_t CAN_mcu_command_motor_controller_enable_get(void){
	return get_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_OFFSET, CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_RANGE);
}

/**********************************************************
 * bms NODE MESSAGES
 */
#define CAN_bms_charger_request_ID 0x1806e5f4

static CAN_message_S CAN_bms_charger_request={
	.canID = CAN_bms_charger_request_ID,
	.canXID = 0x1,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_bms_charger_request_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_bms_charger_request);
}
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET 0
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET 8
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_OFFSET 16
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_OFFSET 24
#define CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_OFFSET 32
#define CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_OFFSET 40
#define CAN_BMS_CHARGER_REQUEST_BYTE_7_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_BYTE_7_OFFSET 48
#define CAN_BMS_CHARGER_REQUEST_BYTE_8_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_BYTE_8_OFFSET 56

uint16_t CAN_bms_charger_request_output_voltage_high_byte_get(void){
	return get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE);
}
uint16_t CAN_bms_charger_request_output_voltage_low_byte_get(void){
	return get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_RANGE);
}
uint16_t CAN_bms_charger_request_output_current_high_byte_get(void){
	return get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_RANGE);
}
uint16_t CAN_bms_charger_request_output_current_low_byte_get(void){
	return get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_RANGE);
}
uint16_t CAN_bms_charger_request_start_charge_request_get(void){
	return get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_OFFSET, CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_RANGE);
}
uint16_t CAN_bms_charger_request_charge_mode_get(void){
	return get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_OFFSET, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_RANGE);
}
uint16_t CAN_bms_charger_request_byte_7_get(void){
	return get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_7_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_7_RANGE);
}
uint16_t CAN_bms_charger_request_byte_8_get(void){
	return get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_8_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_8_RANGE);
}

/**********************************************************
 * motorcontroller NODE MESSAGES
 */
/**********************************************************
 * charger NODE MESSAGES
 */
#define CAN_charger_status_ID 0x18ff50e5

static CAN_payload_S CAN_charger_status_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_charger_status={
	.canID = CAN_charger_status_ID,
	.canXID = 0x1,
	.payload = &CAN_charger_status_payload,
	.canMessageStatus = 0
};

#define CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE 8
#define CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET 0
#define CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_RANGE 8
#define CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET 8
#define CAN_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_RANGE 8
#define CAN_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_OFFSET 16
#define CAN_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_RANGE 8
#define CAN_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_OFFSET 24
#define CAN_CHARGER_STATUS_HARDWARE_ERROR_RANGE 1
#define CAN_CHARGER_STATUS_HARDWARE_ERROR_OFFSET 32
#define CAN_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_RANGE 1
#define CAN_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_OFFSET 33
#define CAN_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_RANGE 1
#define CAN_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_OFFSET 34
#define CAN_CHARGER_STATUS_BATTERY_DETECT_ERROR_RANGE 1
#define CAN_CHARGER_STATUS_BATTERY_DETECT_ERROR_OFFSET 35
#define CAN_CHARGER_STATUS_COMMUNICATION_ERROR_RANGE 1
#define CAN_CHARGER_STATUS_COMMUNICATION_ERROR_OFFSET 36
#define CAN_CHARGER_STATUS_BYTE7_RANGE 8
#define CAN_CHARGER_STATUS_BYTE7_OFFSET 37
#define CAN_CHARGER_STATUS_BYTE8_RANGE 8
#define CAN_CHARGER_STATUS_BYTE8_OFFSET 45

void CAN_charger_status_output_voltage_high_byte_set(uint16_t output_voltage_high_byte){
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE, output_voltage_high_byte);
}
void CAN_charger_status_output_voltage_low_byte_set(uint16_t output_voltage_low_byte){
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_RANGE, output_voltage_low_byte);
}
void CAN_charger_status_output_current_high_byte_set(uint16_t output_current_high_byte){
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_RANGE, output_current_high_byte);
}
void CAN_charger_status_output_current_low_byte_set(uint16_t output_current_low_byte){
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_RANGE, output_current_low_byte);
}
void CAN_charger_status_hardware_error_set(uint16_t hardware_error){
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_HARDWARE_ERROR_OFFSET, CAN_CHARGER_STATUS_HARDWARE_ERROR_RANGE, hardware_error);
}
void CAN_charger_status_charger_overtemp_error_set(uint16_t charger_overtemp_error){
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_OFFSET, CAN_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_RANGE, charger_overtemp_error);
}
void CAN_charger_status_input_voltage_error_set(uint16_t input_voltage_error){
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_OFFSET, CAN_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_RANGE, input_voltage_error);
}
void CAN_charger_status_battery_detect_error_set(uint16_t battery_detect_error){
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BATTERY_DETECT_ERROR_OFFSET, CAN_CHARGER_STATUS_BATTERY_DETECT_ERROR_RANGE, battery_detect_error);
}
void CAN_charger_status_communication_error_set(uint16_t communication_error){
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_COMMUNICATION_ERROR_OFFSET, CAN_CHARGER_STATUS_COMMUNICATION_ERROR_RANGE, communication_error);
}
void CAN_charger_status_byte7_set(uint16_t byte7){
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BYTE7_OFFSET, CAN_CHARGER_STATUS_BYTE7_RANGE, byte7);
}
void CAN_charger_status_byte8_set(uint16_t byte8){
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BYTE8_OFFSET, CAN_CHARGER_STATUS_BYTE8_RANGE, byte8);
}
void CAN_charger_status_send(void){
	CAN_write(CAN_charger_status);
}

/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init(void) {
	CAN_configureMailbox(&CAN_mcu_command);
	CAN_configureMailbox(&CAN_bms_charger_request);
}

void CAN_send_1000ms(void){
	CAN_charger_status_send();
}
