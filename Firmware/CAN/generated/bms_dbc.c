#include "bms_dbc.h"
#include "utils.h"
/**********************************************************
 * dash NODE MESSAGES
 */
/**********************************************************
 * mcu NODE MESSAGES
 */
#define CAN_mcu_status_ID 0x711

static CAN_message_S CAN_mcu_status={
	.canID = CAN_mcu_status_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_mcu_status_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_mcu_status);
}
#define CAN_MCU_STATUS_STATE_RANGE 3
#define CAN_MCU_STATUS_STATE_OFFSET 0
#define CAN_MCU_STATUS_THROTTLEMODE_RANGE 3
#define CAN_MCU_STATUS_THROTTLEMODE_OFFSET 3
#define CAN_MCU_STATUS_HIGHBEAM_RANGE 1
#define CAN_MCU_STATUS_HIGHBEAM_OFFSET 6
#define CAN_MCU_STATUS_LOWBEAM_RANGE 1
#define CAN_MCU_STATUS_LOWBEAM_OFFSET 7
#define CAN_MCU_STATUS_BRAKELIGHT_RANGE 1
#define CAN_MCU_STATUS_BRAKELIGHT_OFFSET 8
#define CAN_MCU_STATUS_TAILLIGHT_RANGE 1
#define CAN_MCU_STATUS_TAILLIGHT_OFFSET 9
#define CAN_MCU_STATUS_HORN_RANGE 1
#define CAN_MCU_STATUS_HORN_OFFSET 10
#define CAN_MCU_STATUS_TURNSIGNALFR_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALFR_OFFSET 11
#define CAN_MCU_STATUS_TURNSIGNALFL_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALFL_OFFSET 12
#define CAN_MCU_STATUS_TURNSIGNALRR_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALRR_OFFSET 13
#define CAN_MCU_STATUS_TURNSIGNALRL_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALRL_OFFSET 14
#define CAN_MCU_STATUS_GBSALL_RANGE 1
#define CAN_MCU_STATUS_GBSALL_OFFSET 15
#define CAN_MCU_STATUS_CONTACTOR_RANGE 1
#define CAN_MCU_STATUS_CONTACTOR_OFFSET 16
#define CAN_MCU_STATUS_CHARGEPORT_RANGE 1
#define CAN_MCU_STATUS_CHARGEPORT_OFFSET 17
#define CAN_MCU_STATUS_BRAKESWITCHFRONT_RANGE 1
#define CAN_MCU_STATUS_BRAKESWITCHFRONT_OFFSET 18
#define CAN_MCU_STATUS_BRAKESWITCHREAR_RANGE 1
#define CAN_MCU_STATUS_BRAKESWITCHREAR_OFFSET 19
#define CAN_MCU_STATUS_THROTTLEVAL_RANGE 8
#define CAN_MCU_STATUS_THROTTLEVAL_OFFSET 20

uint16_t CAN_mcu_status_state_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_STATE_OFFSET, CAN_MCU_STATUS_STATE_RANGE);
}
uint16_t CAN_mcu_status_throttleMode_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEMODE_OFFSET, CAN_MCU_STATUS_THROTTLEMODE_RANGE);
}
uint16_t CAN_mcu_status_highBeam_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HIGHBEAM_OFFSET, CAN_MCU_STATUS_HIGHBEAM_RANGE);
}
uint16_t CAN_mcu_status_lowBeam_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_LOWBEAM_OFFSET, CAN_MCU_STATUS_LOWBEAM_RANGE);
}
uint16_t CAN_mcu_status_brakeLight_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKELIGHT_OFFSET, CAN_MCU_STATUS_BRAKELIGHT_RANGE);
}
uint16_t CAN_mcu_status_tailLight_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TAILLIGHT_OFFSET, CAN_MCU_STATUS_TAILLIGHT_RANGE);
}
uint16_t CAN_mcu_status_horn_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HORN_OFFSET, CAN_MCU_STATUS_HORN_RANGE);
}
uint16_t CAN_mcu_status_turnSignalFR_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFR_OFFSET, CAN_MCU_STATUS_TURNSIGNALFR_RANGE);
}
uint16_t CAN_mcu_status_turnSignalFL_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFL_OFFSET, CAN_MCU_STATUS_TURNSIGNALFL_RANGE);
}
uint16_t CAN_mcu_status_turnSignalRR_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRR_OFFSET, CAN_MCU_STATUS_TURNSIGNALRR_RANGE);
}
uint16_t CAN_mcu_status_turnSignalRL_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRL_OFFSET, CAN_MCU_STATUS_TURNSIGNALRL_RANGE);
}
uint16_t CAN_mcu_status_GBSALL_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_GBSALL_OFFSET, CAN_MCU_STATUS_GBSALL_RANGE);
}
uint16_t CAN_mcu_status_contactor_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_CONTACTOR_OFFSET, CAN_MCU_STATUS_CONTACTOR_RANGE);
}
uint16_t CAN_mcu_status_chargePort_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_CHARGEPORT_OFFSET, CAN_MCU_STATUS_CHARGEPORT_RANGE);
}
uint16_t CAN_mcu_status_brakeSwitchFront_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHFRONT_OFFSET, CAN_MCU_STATUS_BRAKESWITCHFRONT_RANGE);
}
uint16_t CAN_mcu_status_brakeSwitchRear_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHREAR_OFFSET, CAN_MCU_STATUS_BRAKESWITCHREAR_RANGE);
}
uint16_t CAN_mcu_status_throttleVal_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEVAL_OFFSET, CAN_MCU_STATUS_THROTTLEVAL_RANGE);
}

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
#define CAN_bms_status_ID 0x721

static CAN_payload_S CAN_bms_status_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_status={
	.canID = CAN_bms_status_ID,
	.canXID = 0x0,
	.payload = &CAN_bms_status_payload,
	.canMessageStatus = 0
};

#define CAN_BMS_STATUS_STATE_RANGE 3
#define CAN_BMS_STATUS_STATE_OFFSET 0
#define CAN_BMS_STATUS_SOC_RANGE 8
#define CAN_BMS_STATUS_SOC_OFFSET 3
#define CAN_BMS_STATUS_PACKVOLTAGE_RANGE 16
#define CAN_BMS_STATUS_PACKVOLTAGE_OFFSET 11
#define CAN_BMS_STATUS_PACKCURRENT_RANGE 16
#define CAN_BMS_STATUS_PACKCURRENT_OFFSET 27
#define CAN_BMS_STATUS_MINTEMP_RANGE 8
#define CAN_BMS_STATUS_MINTEMP_OFFSET 43
#define CAN_BMS_STATUS_MAXTEMP_RANGE 8
#define CAN_BMS_STATUS_MAXTEMP_OFFSET 51

void CAN_bms_status_state_set(uint16_t state){
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_STATE_OFFSET, CAN_BMS_STATUS_STATE_RANGE, state);
}
void CAN_bms_status_SOC_set(uint16_t SOC){
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_SOC_OFFSET, CAN_BMS_STATUS_SOC_RANGE, SOC);
}
void CAN_bms_status_packVoltage_set(uint16_t packVoltage){
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_PACKVOLTAGE_OFFSET, CAN_BMS_STATUS_PACKVOLTAGE_RANGE, packVoltage);
}
void CAN_bms_status_packCurrent_set(uint16_t packCurrent){
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_PACKCURRENT_OFFSET, CAN_BMS_STATUS_PACKCURRENT_RANGE, packCurrent);
}
void CAN_bms_status_minTemp_set(uint16_t minTemp){
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_MINTEMP_OFFSET, CAN_BMS_STATUS_MINTEMP_RANGE, minTemp);
}
void CAN_bms_status_maxTemp_set(uint16_t maxTemp){
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_MAXTEMP_OFFSET, CAN_BMS_STATUS_MAXTEMP_RANGE, maxTemp);
}
void CAN_bms_status_send(void){
	CAN_write(CAN_bms_status);
}

#define CAN_bms_status_2_ID 0x722

static CAN_payload_S CAN_bms_status_2_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_status_2={
	.canID = CAN_bms_status_2_ID,
	.canXID = 0x0,
	.payload = &CAN_bms_status_2_payload,
	.canMessageStatus = 0
};

#define CAN_BMS_STATUS_2_DCDC_STATE_RANGE 1
#define CAN_BMS_STATUS_2_DCDC_STATE_OFFSET 0
#define CAN_BMS_STATUS_2_DCDC_FAULT_RANGE 1
#define CAN_BMS_STATUS_2_DCDC_FAULT_OFFSET 1
#define CAN_BMS_STATUS_2_DCDC_VOLTAGE_RANGE 8
#define CAN_BMS_STATUS_2_DCDC_VOLTAGE_OFFSET 2
#define CAN_BMS_STATUS_2_DCDC_CURRENT_RANGE 8
#define CAN_BMS_STATUS_2_DCDC_CURRENT_OFFSET 10
#define CAN_BMS_STATUS_2_EV_CHARGER_STATE_RANGE 1
#define CAN_BMS_STATUS_2_EV_CHARGER_STATE_OFFSET 18
#define CAN_BMS_STATUS_2_EV_CHARGER_FAULT_RANGE 1
#define CAN_BMS_STATUS_2_EV_CHARGER_FAULT_OFFSET 19
#define CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_RANGE 8
#define CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_OFFSET 20
#define CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_RANGE 8
#define CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_OFFSET 28
#define CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_RANGE 1
#define CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_OFFSET 36
#define CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_RANGE 1
#define CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_OFFSET 37
#define CAN_BMS_STATUS_2_HV_BUS_VOLTAGE_RANGE 8
#define CAN_BMS_STATUS_2_HV_BUS_VOLTAGE_OFFSET 38

void CAN_bms_status_2_DCDC_state_set(uint16_t DCDC_state){
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_STATE_OFFSET, CAN_BMS_STATUS_2_DCDC_STATE_RANGE, DCDC_state);
}
void CAN_bms_status_2_DCDC_fault_set(uint16_t DCDC_fault){
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_FAULT_OFFSET, CAN_BMS_STATUS_2_DCDC_FAULT_RANGE, DCDC_fault);
}
void CAN_bms_status_2_DCDC_voltage_set(uint16_t DCDC_voltage){
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_DCDC_VOLTAGE_RANGE, DCDC_voltage);
}
void CAN_bms_status_2_DCDC_current_set(uint16_t DCDC_current){
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_CURRENT_OFFSET, CAN_BMS_STATUS_2_DCDC_CURRENT_RANGE, DCDC_current);
}
void CAN_bms_status_2_EV_charger_state_set(uint16_t EV_charger_state){
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_STATE_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_STATE_RANGE, EV_charger_state);
}
void CAN_bms_status_2_EV_charger_fault_set(uint16_t EV_charger_fault){
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_FAULT_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_FAULT_RANGE, EV_charger_fault);
}
void CAN_bms_status_2_EV_charger_voltage_set(uint16_t EV_charger_voltage){
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_RANGE, EV_charger_voltage);
}
void CAN_bms_status_2_EV_charger_current_set(uint16_t EV_charger_current){
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_RANGE, EV_charger_current);
}
void CAN_bms_status_2_HV_precharge_state_set(uint16_t HV_precharge_state){
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_OFFSET, CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_RANGE, HV_precharge_state);
}
void CAN_bms_status_2_HV_contactor_state_set(uint16_t HV_contactor_state){
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_OFFSET, CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_RANGE, HV_contactor_state);
}
void CAN_bms_status_2_HV_bus_voltage_set(uint16_t HV_bus_voltage){
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_BUS_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_HV_BUS_VOLTAGE_RANGE, HV_bus_voltage);
}
void CAN_bms_status_2_send(void){
	CAN_write(CAN_bms_status_2);
}

#define CAN_bms_boot_response_ID 0xa2

static CAN_payload_S CAN_bms_boot_response_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_boot_response={
	.canID = CAN_bms_boot_response_ID,
	.canXID = 0x0,
	.payload = &CAN_bms_boot_response_payload,
	.canMessageStatus = 0
};

#define CAN_BMS_BOOT_RESPONSE_CODE_RANGE 4
#define CAN_BMS_BOOT_RESPONSE_CODE_OFFSET 0
#define CAN_BMS_BOOT_RESPONSE_TYPE_RANGE 4
#define CAN_BMS_BOOT_RESPONSE_TYPE_OFFSET 4
#define CAN_BMS_BOOT_RESPONSE_BYTE1_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE1_OFFSET 8
#define CAN_BMS_BOOT_RESPONSE_BYTE2_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE2_OFFSET 16
#define CAN_BMS_BOOT_RESPONSE_BYTE3_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE3_OFFSET 24
#define CAN_BMS_BOOT_RESPONSE_BYTE4_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE4_OFFSET 32
#define CAN_BMS_BOOT_RESPONSE_BYTE5_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE5_OFFSET 40
#define CAN_BMS_BOOT_RESPONSE_BYTE6_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE6_OFFSET 48
#define CAN_BMS_BOOT_RESPONSE_BYTE7_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE7_OFFSET 56

void CAN_bms_boot_response_code_set(uint16_t code){
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_CODE_OFFSET, CAN_BMS_BOOT_RESPONSE_CODE_RANGE, code);
}
void CAN_bms_boot_response_type_set(uint16_t type){
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_TYPE_OFFSET, CAN_BMS_BOOT_RESPONSE_TYPE_RANGE, type);
}
void CAN_bms_boot_response_byte1_set(uint16_t byte1){
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE1_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE1_RANGE, byte1);
}
void CAN_bms_boot_response_byte2_set(uint16_t byte2){
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE2_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE2_RANGE, byte2);
}
void CAN_bms_boot_response_byte3_set(uint16_t byte3){
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE3_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE3_RANGE, byte3);
}
void CAN_bms_boot_response_byte4_set(uint16_t byte4){
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE4_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE4_RANGE, byte4);
}
void CAN_bms_boot_response_byte5_set(uint16_t byte5){
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE5_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE5_RANGE, byte5);
}
void CAN_bms_boot_response_byte6_set(uint16_t byte6){
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE6_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE6_RANGE, byte6);
}
void CAN_bms_boot_response_byte7_set(uint16_t byte7){
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE7_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE7_RANGE, byte7);
}
void CAN_bms_boot_response_send(void){
	CAN_write(CAN_bms_boot_response);
}

#define CAN_bms_charger_request_ID 0x1806e5f4

static CAN_payload_S CAN_bms_charger_request_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_charger_request={
	.canID = CAN_bms_charger_request_ID,
	.canXID = 0x1,
	.payload = &CAN_bms_charger_request_payload,
	.canMessageStatus = 0
};

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

void CAN_bms_charger_request_output_voltage_high_byte_set(uint16_t output_voltage_high_byte){
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE, output_voltage_high_byte);
}
void CAN_bms_charger_request_output_voltage_low_byte_set(uint16_t output_voltage_low_byte){
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_RANGE, output_voltage_low_byte);
}
void CAN_bms_charger_request_output_current_high_byte_set(uint16_t output_current_high_byte){
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_RANGE, output_current_high_byte);
}
void CAN_bms_charger_request_output_current_low_byte_set(uint16_t output_current_low_byte){
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_RANGE, output_current_low_byte);
}
void CAN_bms_charger_request_start_charge_request_set(uint16_t start_charge_request){
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_OFFSET, CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_RANGE, start_charge_request);
}
void CAN_bms_charger_request_charge_mode_set(uint16_t charge_mode){
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_OFFSET, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_RANGE, charge_mode);
}
void CAN_bms_charger_request_byte_7_set(uint16_t byte_7){
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_7_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_7_RANGE, byte_7);
}
void CAN_bms_charger_request_byte_8_set(uint16_t byte_8){
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_8_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_8_RANGE, byte_8);
}
void CAN_bms_charger_request_send(void){
	CAN_write(CAN_bms_charger_request);
}

#define CAN_bms_cellVoltages_ID 0x725

static CAN_payload_S CAN_bms_cellVoltages_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_cellVoltages={
	.canID = CAN_bms_cellVoltages_ID,
	.canXID = 0x0,
	.payload = &CAN_bms_cellVoltages_payload,
	.canMessageStatus = 0
};

#define CAN_BMS_CELLVOLTAGES_MULTIPLEX_RANGE 8
#define CAN_BMS_CELLVOLTAGES_MULTIPLEX_OFFSET 0
#define CAN_BMS_CELLVOLTAGES_CELL1_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL1_OFFSET 8
#define CAN_BMS_CELLVOLTAGES_CELL2_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL2_OFFSET 20
#define CAN_BMS_CELLVOLTAGES_CELL3_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL3_OFFSET 32
#define CAN_BMS_CELLVOLTAGES_CELL4_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL4_OFFSET 44

void CAN_bms_cellVoltages_MultiPlex_set(uint16_t MultiPlex){
	set_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_MULTIPLEX_OFFSET, CAN_BMS_CELLVOLTAGES_MULTIPLEX_RANGE, MultiPlex);
}
void CAN_bms_cellVoltages_cell1_set(uint16_t cell1){
	set_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL1_OFFSET, CAN_BMS_CELLVOLTAGES_CELL1_RANGE, cell1);
}
void CAN_bms_cellVoltages_cell2_set(uint16_t cell2){
	set_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL2_OFFSET, CAN_BMS_CELLVOLTAGES_CELL2_RANGE, cell2);
}
void CAN_bms_cellVoltages_cell3_set(uint16_t cell3){
	set_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL3_OFFSET, CAN_BMS_CELLVOLTAGES_CELL3_RANGE, cell3);
}
void CAN_bms_cellVoltages_cell4_set(uint16_t cell4){
	set_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL4_OFFSET, CAN_BMS_CELLVOLTAGES_CELL4_RANGE, cell4);
}
void CAN_bms_cellVoltages_send(void){
	CAN_write(CAN_bms_cellVoltages);
}

/**********************************************************
 * motorcontroller NODE MESSAGES
 */
/**********************************************************
 * charger NODE MESSAGES
 */
#define CAN_charger_status_ID 0x18ff50e5

static CAN_message_S CAN_charger_status={
	.canID = CAN_charger_status_ID,
	.canXID = 0x1,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_charger_status_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_charger_status);
}
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

uint16_t CAN_charger_status_output_voltage_high_byte_get(void){
	return get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE);
}
uint16_t CAN_charger_status_output_voltage_low_byte_get(void){
	return get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_RANGE);
}
uint16_t CAN_charger_status_output_current_high_byte_get(void){
	return get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_RANGE);
}
uint16_t CAN_charger_status_output_current_low_byte_get(void){
	return get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_RANGE);
}
uint16_t CAN_charger_status_hardware_error_get(void){
	return get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_HARDWARE_ERROR_OFFSET, CAN_CHARGER_STATUS_HARDWARE_ERROR_RANGE);
}
uint16_t CAN_charger_status_charger_overtemp_error_get(void){
	return get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_OFFSET, CAN_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_RANGE);
}
uint16_t CAN_charger_status_input_voltage_error_get(void){
	return get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_OFFSET, CAN_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_RANGE);
}
uint16_t CAN_charger_status_battery_detect_error_get(void){
	return get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BATTERY_DETECT_ERROR_OFFSET, CAN_CHARGER_STATUS_BATTERY_DETECT_ERROR_RANGE);
}
uint16_t CAN_charger_status_communication_error_get(void){
	return get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_COMMUNICATION_ERROR_OFFSET, CAN_CHARGER_STATUS_COMMUNICATION_ERROR_RANGE);
}
uint16_t CAN_charger_status_byte7_get(void){
	return get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BYTE7_OFFSET, CAN_CHARGER_STATUS_BYTE7_RANGE);
}
uint16_t CAN_charger_status_byte8_get(void){
	return get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BYTE8_OFFSET, CAN_CHARGER_STATUS_BYTE8_RANGE);
}

/**********************************************************
 * boot_host NODE MESSAGES
 */
#define CAN_boot_host_bms_ID 0xa1

static CAN_message_S CAN_boot_host_bms={
	.canID = CAN_boot_host_bms_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_boot_host_bms_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_boot_host_bms);
}
#define CAN_BOOT_HOST_BMS_CODE_RANGE 4
#define CAN_BOOT_HOST_BMS_CODE_OFFSET 0
#define CAN_BOOT_HOST_BMS_TYPE_RANGE 4
#define CAN_BOOT_HOST_BMS_TYPE_OFFSET 4
#define CAN_BOOT_HOST_BMS_BYTE1_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE1_OFFSET 8
#define CAN_BOOT_HOST_BMS_BYTE2_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE2_OFFSET 16
#define CAN_BOOT_HOST_BMS_BYTE3_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE3_OFFSET 24
#define CAN_BOOT_HOST_BMS_BYTE4_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE4_OFFSET 32
#define CAN_BOOT_HOST_BMS_BYTE5_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE5_OFFSET 40
#define CAN_BOOT_HOST_BMS_BYTE6_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE6_OFFSET 48
#define CAN_BOOT_HOST_BMS_BYTE7_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE7_OFFSET 56

uint16_t CAN_boot_host_bms_code_get(void){
	return get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_CODE_OFFSET, CAN_BOOT_HOST_BMS_CODE_RANGE);
}
uint16_t CAN_boot_host_bms_type_get(void){
	return get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_TYPE_OFFSET, CAN_BOOT_HOST_BMS_TYPE_RANGE);
}
uint16_t CAN_boot_host_bms_byte1_get(void){
	return get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE1_OFFSET, CAN_BOOT_HOST_BMS_BYTE1_RANGE);
}
uint16_t CAN_boot_host_bms_byte2_get(void){
	return get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE2_OFFSET, CAN_BOOT_HOST_BMS_BYTE2_RANGE);
}
uint16_t CAN_boot_host_bms_byte3_get(void){
	return get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE3_OFFSET, CAN_BOOT_HOST_BMS_BYTE3_RANGE);
}
uint16_t CAN_boot_host_bms_byte4_get(void){
	return get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE4_OFFSET, CAN_BOOT_HOST_BMS_BYTE4_RANGE);
}
uint16_t CAN_boot_host_bms_byte5_get(void){
	return get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE5_OFFSET, CAN_BOOT_HOST_BMS_BYTE5_RANGE);
}
uint16_t CAN_boot_host_bms_byte6_get(void){
	return get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE6_OFFSET, CAN_BOOT_HOST_BMS_BYTE6_RANGE);
}
uint16_t CAN_boot_host_bms_byte7_get(void){
	return get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE7_OFFSET, CAN_BOOT_HOST_BMS_BYTE7_RANGE);
}

void CAN_DBC_init(void) {
	CAN_configureMailbox(&CAN_mcu_status);
	CAN_configureMailbox(&CAN_mcu_command);
	CAN_configureMailbox(&CAN_charger_status);
	CAN_configureMailbox(&CAN_boot_host_bms);
}

void CAN_send_10ms(void){
	CAN_bms_status_send();
	CAN_bms_status_2_send();
	CAN_bms_cellVoltages_send();
}

void CAN_send_1000ms(void){
	CAN_bms_charger_request_send();
}
