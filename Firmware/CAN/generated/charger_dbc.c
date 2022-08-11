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
	.canXID = 0,
	.dlc = 8,
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
#define CAN_MCU_COMMAND_EV_CHARGER_CURRENT_RANGE 13
#define CAN_MCU_COMMAND_EV_CHARGER_CURRENT_OFFSET 2
#define CAN_MCU_COMMAND_PRECHARGE_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_PRECHARGE_ENABLE_OFFSET 15
#define CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_OFFSET 16

uint16_t CAN_mcu_command_DCDC_enable_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_DCDC_ENABLE_OFFSET, CAN_MCU_COMMAND_DCDC_ENABLE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_command_ev_charger_enable_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_EV_CHARGER_ENABLE_OFFSET, CAN_MCU_COMMAND_EV_CHARGER_ENABLE_RANGE);
	return (data * 1.0) + 0;
}
float CAN_mcu_command_ev_charger_current_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_EV_CHARGER_CURRENT_OFFSET, CAN_MCU_COMMAND_EV_CHARGER_CURRENT_RANGE);
	return (data * 0.1) + 0;
}
uint16_t CAN_mcu_command_precharge_enable_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_PRECHARGE_ENABLE_OFFSET, CAN_MCU_COMMAND_PRECHARGE_ENABLE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_command_motor_controller_enable_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_OFFSET, CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_RANGE);
	return (data * 1.0) + 0;
}

/**********************************************************
 * bms NODE MESSAGES
 */
#define CAN_bms_debug_ID 0x723

static CAN_message_S CAN_bms_debug={
	.canID = CAN_bms_debug_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_bms_debug_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_bms_debug);
}
#define CAN_BMS_DEBUG_BOOL0_RANGE 1
#define CAN_BMS_DEBUG_BOOL0_OFFSET 0
#define CAN_BMS_DEBUG_BOOL1_RANGE 1
#define CAN_BMS_DEBUG_BOOL1_OFFSET 1
#define CAN_BMS_DEBUG_BOOL2_RANGE 1
#define CAN_BMS_DEBUG_BOOL2_OFFSET 2
#define CAN_BMS_DEBUG_BOOL3_RANGE 1
#define CAN_BMS_DEBUG_BOOL3_OFFSET 3
#define CAN_BMS_DEBUG_FLOAT1_RANGE 16
#define CAN_BMS_DEBUG_FLOAT1_OFFSET 4
#define CAN_BMS_DEBUG_FLOAT2_RANGE 16
#define CAN_BMS_DEBUG_FLOAT2_OFFSET 20

uint16_t CAN_bms_debug_bool0_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL0_OFFSET, CAN_BMS_DEBUG_BOOL0_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_debug_bool1_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL1_OFFSET, CAN_BMS_DEBUG_BOOL1_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_debug_bool2_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL2_OFFSET, CAN_BMS_DEBUG_BOOL2_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_debug_bool3_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL3_OFFSET, CAN_BMS_DEBUG_BOOL3_RANGE);
	return (data * 1.0) + 0;
}
float CAN_bms_debug_float1_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_FLOAT1_OFFSET, CAN_BMS_DEBUG_FLOAT1_RANGE);
	return (data * 0.01) + 0;
}
float CAN_bms_debug_float2_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_FLOAT2_OFFSET, CAN_BMS_DEBUG_FLOAT2_RANGE);
	return (data * 0.01) + 0;
}

#define CAN_bms_charger_request_ID 0x1806e5f4

static CAN_message_S CAN_bms_charger_request={
	.canID = CAN_bms_charger_request_ID,
	.canXID = 1,
	.dlc = 8,
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
#define CAN_BMS_CHARGER_REQUEST_START_CHARGE_NOT_REQUEST_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_START_CHARGE_NOT_REQUEST_OFFSET 32
#define CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_OFFSET 40
#define CAN_BMS_CHARGER_REQUEST_BYTE_7_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_BYTE_7_OFFSET 48
#define CAN_BMS_CHARGER_REQUEST_BYTE_8_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_BYTE_8_OFFSET 56

uint16_t CAN_bms_charger_request_output_voltage_high_byte_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_charger_request_output_voltage_low_byte_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_charger_request_output_current_high_byte_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_charger_request_output_current_low_byte_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_charger_request_start_charge_not_request_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_START_CHARGE_NOT_REQUEST_OFFSET, CAN_BMS_CHARGER_REQUEST_START_CHARGE_NOT_REQUEST_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_charger_request_charge_mode_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_OFFSET, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_charger_request_byte_7_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_7_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_7_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_charger_request_byte_8_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_8_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_8_RANGE);
	return (data * 1.0) + 0;
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
	.canXID = 1,
	.dlc = 8,
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
	uint16_t data_scaled = (output_voltage_high_byte - 0) / 1.0;
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE, data_scaled);
}
void CAN_charger_status_output_voltage_low_byte_set(uint16_t output_voltage_low_byte){
	uint16_t data_scaled = (output_voltage_low_byte - 0) / 1.0;
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_RANGE, data_scaled);
}
void CAN_charger_status_output_current_high_byte_set(uint16_t output_current_high_byte){
	uint16_t data_scaled = (output_current_high_byte - 0) / 1.0;
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_RANGE, data_scaled);
}
void CAN_charger_status_output_current_low_byte_set(uint16_t output_current_low_byte){
	uint16_t data_scaled = (output_current_low_byte - 0) / 1.0;
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_RANGE, data_scaled);
}
void CAN_charger_status_hardware_error_set(uint16_t hardware_error){
	uint16_t data_scaled = (hardware_error - 0) / 1.0;
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_HARDWARE_ERROR_OFFSET, CAN_CHARGER_STATUS_HARDWARE_ERROR_RANGE, data_scaled);
}
void CAN_charger_status_charger_overtemp_error_set(uint16_t charger_overtemp_error){
	uint16_t data_scaled = (charger_overtemp_error - 0) / 1.0;
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_OFFSET, CAN_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_RANGE, data_scaled);
}
void CAN_charger_status_input_voltage_error_set(uint16_t input_voltage_error){
	uint16_t data_scaled = (input_voltage_error - 0) / 1.0;
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_OFFSET, CAN_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_RANGE, data_scaled);
}
void CAN_charger_status_battery_detect_error_set(uint16_t battery_detect_error){
	uint16_t data_scaled = (battery_detect_error - 0) / 1.0;
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BATTERY_DETECT_ERROR_OFFSET, CAN_CHARGER_STATUS_BATTERY_DETECT_ERROR_RANGE, data_scaled);
}
void CAN_charger_status_communication_error_set(uint16_t communication_error){
	uint16_t data_scaled = (communication_error - 0) / 1.0;
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_COMMUNICATION_ERROR_OFFSET, CAN_CHARGER_STATUS_COMMUNICATION_ERROR_RANGE, data_scaled);
}
void CAN_charger_status_byte7_set(uint16_t byte7){
	uint16_t data_scaled = (byte7 - 0) / 1.0;
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BYTE7_OFFSET, CAN_CHARGER_STATUS_BYTE7_RANGE, data_scaled);
}
void CAN_charger_status_byte8_set(uint16_t byte8){
	uint16_t data_scaled = (byte8 - 0) / 1.0;
	set_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BYTE8_OFFSET, CAN_CHARGER_STATUS_BYTE8_RANGE, data_scaled);
}
void CAN_charger_status_dlc_set(uint8_t dlc){
	CAN_charger_status.dlc = dlc;
}
void CAN_charger_status_send(void){
	CAN_write(CAN_charger_status);
}

/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init(void) {
	CAN_configureMailbox(&CAN_mcu_command);
	CAN_configureMailbox(&CAN_bms_debug);
	CAN_configureMailbox(&CAN_bms_charger_request);
}

void CAN_send_1000ms(void){
	CAN_charger_status_send();
}
