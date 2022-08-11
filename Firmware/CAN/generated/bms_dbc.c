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
	.canXID = 0,
	.dlc = 8,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_mcu_status_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_mcu_status);
}
#define CAN_MCU_STATUS_HEARTBEAT_RANGE 4
#define CAN_MCU_STATUS_HEARTBEAT_OFFSET 0
#define CAN_MCU_STATUS_STATE_RANGE 3
#define CAN_MCU_STATUS_STATE_OFFSET 4
#define CAN_MCU_STATUS_THROTTLEMODE_RANGE 3
#define CAN_MCU_STATUS_THROTTLEMODE_OFFSET 7
#define CAN_MCU_STATUS_HIGHBEAM_RANGE 1
#define CAN_MCU_STATUS_HIGHBEAM_OFFSET 10
#define CAN_MCU_STATUS_LOWBEAM_RANGE 1
#define CAN_MCU_STATUS_LOWBEAM_OFFSET 11
#define CAN_MCU_STATUS_BRAKELIGHT_RANGE 1
#define CAN_MCU_STATUS_BRAKELIGHT_OFFSET 12
#define CAN_MCU_STATUS_TAILLIGHT_RANGE 1
#define CAN_MCU_STATUS_TAILLIGHT_OFFSET 13
#define CAN_MCU_STATUS_HORN_RANGE 1
#define CAN_MCU_STATUS_HORN_OFFSET 14
#define CAN_MCU_STATUS_TURNSIGNALFR_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALFR_OFFSET 15
#define CAN_MCU_STATUS_TURNSIGNALFL_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALFL_OFFSET 16
#define CAN_MCU_STATUS_TURNSIGNALRR_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALRR_OFFSET 17
#define CAN_MCU_STATUS_TURNSIGNALRL_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALRL_OFFSET 18
#define CAN_MCU_STATUS_GBSALL_RANGE 1
#define CAN_MCU_STATUS_GBSALL_OFFSET 19
#define CAN_MCU_STATUS_CONTACTOR_RANGE 1
#define CAN_MCU_STATUS_CONTACTOR_OFFSET 20
#define CAN_MCU_STATUS_CHARGEPORT_RANGE 1
#define CAN_MCU_STATUS_CHARGEPORT_OFFSET 21
#define CAN_MCU_STATUS_BRAKESWITCHFRONT_RANGE 1
#define CAN_MCU_STATUS_BRAKESWITCHFRONT_OFFSET 22
#define CAN_MCU_STATUS_BRAKESWITCHREAR_RANGE 1
#define CAN_MCU_STATUS_BRAKESWITCHREAR_OFFSET 23
#define CAN_MCU_STATUS_THROTTLEVAL_RANGE 8
#define CAN_MCU_STATUS_THROTTLEVAL_OFFSET 24

uint16_t CAN_mcu_status_heartbeat_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HEARTBEAT_OFFSET, CAN_MCU_STATUS_HEARTBEAT_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_state_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_STATE_OFFSET, CAN_MCU_STATUS_STATE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_throttleMode_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEMODE_OFFSET, CAN_MCU_STATUS_THROTTLEMODE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_highBeam_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HIGHBEAM_OFFSET, CAN_MCU_STATUS_HIGHBEAM_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_lowBeam_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_LOWBEAM_OFFSET, CAN_MCU_STATUS_LOWBEAM_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_brakeLight_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKELIGHT_OFFSET, CAN_MCU_STATUS_BRAKELIGHT_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_tailLight_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TAILLIGHT_OFFSET, CAN_MCU_STATUS_TAILLIGHT_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_horn_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HORN_OFFSET, CAN_MCU_STATUS_HORN_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_turnSignalFR_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFR_OFFSET, CAN_MCU_STATUS_TURNSIGNALFR_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_turnSignalFL_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFL_OFFSET, CAN_MCU_STATUS_TURNSIGNALFL_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_turnSignalRR_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRR_OFFSET, CAN_MCU_STATUS_TURNSIGNALRR_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_turnSignalRL_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRL_OFFSET, CAN_MCU_STATUS_TURNSIGNALRL_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_GBSALL_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_GBSALL_OFFSET, CAN_MCU_STATUS_GBSALL_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_contactor_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_CONTACTOR_OFFSET, CAN_MCU_STATUS_CONTACTOR_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_chargePort_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_CHARGEPORT_OFFSET, CAN_MCU_STATUS_CHARGEPORT_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_brakeSwitchFront_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHFRONT_OFFSET, CAN_MCU_STATUS_BRAKESWITCHFRONT_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_brakeSwitchRear_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHREAR_OFFSET, CAN_MCU_STATUS_BRAKESWITCHREAR_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_mcu_status_throttleVal_get(void){
	uint16_t data = get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEVAL_OFFSET, CAN_MCU_STATUS_THROTTLEVAL_RANGE);
	return (data * 1.0) + 0;
}

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
#define CAN_bms_status_ID 0x721

static CAN_payload_S CAN_bms_status_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_status={
	.canID = CAN_bms_status_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = &CAN_bms_status_payload,
	.canMessageStatus = 0
};

#define CAN_BMS_STATUS_STATE_RANGE 3
#define CAN_BMS_STATUS_STATE_OFFSET 0
#define CAN_BMS_STATUS_SOC_RANGE 9
#define CAN_BMS_STATUS_SOC_OFFSET 3
#define CAN_BMS_STATUS_PACKVOLTAGE_RANGE 16
#define CAN_BMS_STATUS_PACKVOLTAGE_OFFSET 12
#define CAN_BMS_STATUS_PACKCURRENT_RANGE 16
#define CAN_BMS_STATUS_PACKCURRENT_OFFSET 28
#define CAN_BMS_STATUS_MINTEMP_RANGE 10
#define CAN_BMS_STATUS_MINTEMP_OFFSET 44
#define CAN_BMS_STATUS_MAXTEMP_RANGE 10
#define CAN_BMS_STATUS_MAXTEMP_OFFSET 54

void CAN_bms_status_state_set(uint16_t state){
	uint16_t data_scaled = (state - 0) / 1.0;
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_STATE_OFFSET, CAN_BMS_STATUS_STATE_RANGE, data_scaled);
}
void CAN_bms_status_SOC_set(uint16_t SOC){
	uint16_t data_scaled = (SOC - 0) / 0.1;
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_SOC_OFFSET, CAN_BMS_STATUS_SOC_RANGE, data_scaled);
}
void CAN_bms_status_packVoltage_set(float packVoltage){
	uint16_t data_scaled = (packVoltage - 0) / 0.01;
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_PACKVOLTAGE_OFFSET, CAN_BMS_STATUS_PACKVOLTAGE_RANGE, data_scaled);
}
void CAN_bms_status_packCurrent_set(float packCurrent){
	uint16_t data_scaled = (packCurrent - 0) / 0.01;
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_PACKCURRENT_OFFSET, CAN_BMS_STATUS_PACKCURRENT_RANGE, data_scaled);
}
void CAN_bms_status_minTemp_set(float minTemp){
	uint16_t data_scaled = (minTemp - -40) / 0.1;
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_MINTEMP_OFFSET, CAN_BMS_STATUS_MINTEMP_RANGE, data_scaled);
}
void CAN_bms_status_maxTemp_set(float maxTemp){
	uint16_t data_scaled = (maxTemp - -40) / 0.1;
	set_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_MAXTEMP_OFFSET, CAN_BMS_STATUS_MAXTEMP_RANGE, data_scaled);
}
void CAN_bms_status_dlc_set(uint8_t dlc){
	CAN_bms_status.dlc = dlc;
}
void CAN_bms_status_send(void){
	CAN_write(CAN_bms_status);
}

#define CAN_bms_status_2_ID 0x722

static CAN_payload_S CAN_bms_status_2_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_status_2={
	.canID = CAN_bms_status_2_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = &CAN_bms_status_2_payload,
	.canMessageStatus = 0
};

#define CAN_BMS_STATUS_2_DCDC_STATE_RANGE 1
#define CAN_BMS_STATUS_2_DCDC_STATE_OFFSET 0
#define CAN_BMS_STATUS_2_DCDC_FAULT_RANGE 1
#define CAN_BMS_STATUS_2_DCDC_FAULT_OFFSET 1
#define CAN_BMS_STATUS_2_DCDC_VOLTAGE_RANGE 10
#define CAN_BMS_STATUS_2_DCDC_VOLTAGE_OFFSET 2
#define CAN_BMS_STATUS_2_DCDC_CURRENT_RANGE 10
#define CAN_BMS_STATUS_2_DCDC_CURRENT_OFFSET 12
#define CAN_BMS_STATUS_2_EV_CHARGER_STATE_RANGE 1
#define CAN_BMS_STATUS_2_EV_CHARGER_STATE_OFFSET 22
#define CAN_BMS_STATUS_2_EV_CHARGER_FAULT_RANGE 1
#define CAN_BMS_STATUS_2_EV_CHARGER_FAULT_OFFSET 23
#define CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_RANGE 10
#define CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_OFFSET 24
#define CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_RANGE 10
#define CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_OFFSET 34
#define CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_RANGE 1
#define CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_OFFSET 44
#define CAN_BMS_STATUS_2_HV_ISOLATION_VOLTAGE_RANGE 10
#define CAN_BMS_STATUS_2_HV_ISOLATION_VOLTAGE_OFFSET 45
#define CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_RANGE 1
#define CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_OFFSET 55

void CAN_bms_status_2_DCDC_state_set(uint16_t DCDC_state){
	uint16_t data_scaled = (DCDC_state - 0) / 1.0;
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_STATE_OFFSET, CAN_BMS_STATUS_2_DCDC_STATE_RANGE, data_scaled);
}
void CAN_bms_status_2_DCDC_fault_set(uint16_t DCDC_fault){
	uint16_t data_scaled = (DCDC_fault - 0) / 1.0;
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_FAULT_OFFSET, CAN_BMS_STATUS_2_DCDC_FAULT_RANGE, data_scaled);
}
void CAN_bms_status_2_DCDC_voltage_set(float DCDC_voltage){
	uint16_t data_scaled = (DCDC_voltage - 0) / 0.1;
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_DCDC_VOLTAGE_RANGE, data_scaled);
}
void CAN_bms_status_2_DCDC_current_set(float DCDC_current){
	uint16_t data_scaled = (DCDC_current - 0) / 0.1;
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_CURRENT_OFFSET, CAN_BMS_STATUS_2_DCDC_CURRENT_RANGE, data_scaled);
}
void CAN_bms_status_2_EV_charger_state_set(uint16_t EV_charger_state){
	uint16_t data_scaled = (EV_charger_state - 0) / 1.0;
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_STATE_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_STATE_RANGE, data_scaled);
}
void CAN_bms_status_2_EV_charger_fault_set(uint16_t EV_charger_fault){
	uint16_t data_scaled = (EV_charger_fault - 0) / 1.0;
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_FAULT_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_FAULT_RANGE, data_scaled);
}
void CAN_bms_status_2_EV_charger_voltage_set(float EV_charger_voltage){
	uint16_t data_scaled = (EV_charger_voltage - 0) / 0.1;
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_RANGE, data_scaled);
}
void CAN_bms_status_2_EV_charger_current_set(float EV_charger_current){
	uint16_t data_scaled = (EV_charger_current - 0) / 0.1;
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_RANGE, data_scaled);
}
void CAN_bms_status_2_HV_precharge_state_set(uint16_t HV_precharge_state){
	uint16_t data_scaled = (HV_precharge_state - 0) / 1.0;
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_OFFSET, CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_RANGE, data_scaled);
}
void CAN_bms_status_2_HV_isolation_voltage_set(float HV_isolation_voltage){
	uint16_t data_scaled = (HV_isolation_voltage - 0) / 0.1;
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_ISOLATION_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_HV_ISOLATION_VOLTAGE_RANGE, data_scaled);
}
void CAN_bms_status_2_HV_contactor_state_set(uint16_t HV_contactor_state){
	uint16_t data_scaled = (HV_contactor_state - 0) / 1.0;
	set_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_OFFSET, CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_RANGE, data_scaled);
}
void CAN_bms_status_2_dlc_set(uint8_t dlc){
	CAN_bms_status_2.dlc = dlc;
}
void CAN_bms_status_2_send(void){
	CAN_write(CAN_bms_status_2);
}

#define CAN_bms_debug_ID 0x723

static CAN_payload_S CAN_bms_debug_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_debug={
	.canID = CAN_bms_debug_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = &CAN_bms_debug_payload,
	.canMessageStatus = 0
};

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

void CAN_bms_debug_bool0_set(uint16_t bool0){
	uint16_t data_scaled = (bool0 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL0_OFFSET, CAN_BMS_DEBUG_BOOL0_RANGE, data_scaled);
}
void CAN_bms_debug_bool1_set(uint16_t bool1){
	uint16_t data_scaled = (bool1 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL1_OFFSET, CAN_BMS_DEBUG_BOOL1_RANGE, data_scaled);
}
void CAN_bms_debug_bool2_set(uint16_t bool2){
	uint16_t data_scaled = (bool2 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL2_OFFSET, CAN_BMS_DEBUG_BOOL2_RANGE, data_scaled);
}
void CAN_bms_debug_bool3_set(uint16_t bool3){
	uint16_t data_scaled = (bool3 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL3_OFFSET, CAN_BMS_DEBUG_BOOL3_RANGE, data_scaled);
}
void CAN_bms_debug_float1_set(float float1){
	uint16_t data_scaled = (float1 - 0) / 0.01;
	set_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_FLOAT1_OFFSET, CAN_BMS_DEBUG_FLOAT1_RANGE, data_scaled);
}
void CAN_bms_debug_float2_set(float float2){
	uint16_t data_scaled = (float2 - 0) / 0.01;
	set_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_FLOAT2_OFFSET, CAN_BMS_DEBUG_FLOAT2_RANGE, data_scaled);
}
void CAN_bms_debug_dlc_set(uint8_t dlc){
	CAN_bms_debug.dlc = dlc;
}
void CAN_bms_debug_send(void){
	CAN_write(CAN_bms_debug);
}

#define CAN_bms_boot_response_ID 0xa2

static CAN_payload_S CAN_bms_boot_response_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_boot_response={
	.canID = CAN_bms_boot_response_ID,
	.canXID = 0,
	.dlc = 8,
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
	uint16_t data_scaled = (code - 0) / 1.0;
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_CODE_OFFSET, CAN_BMS_BOOT_RESPONSE_CODE_RANGE, data_scaled);
}
void CAN_bms_boot_response_type_set(uint16_t type){
	uint16_t data_scaled = (type - 0) / 1.0;
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_TYPE_OFFSET, CAN_BMS_BOOT_RESPONSE_TYPE_RANGE, data_scaled);
}
void CAN_bms_boot_response_byte1_set(uint16_t byte1){
	uint16_t data_scaled = (byte1 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE1_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE1_RANGE, data_scaled);
}
void CAN_bms_boot_response_byte2_set(uint16_t byte2){
	uint16_t data_scaled = (byte2 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE2_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE2_RANGE, data_scaled);
}
void CAN_bms_boot_response_byte3_set(uint16_t byte3){
	uint16_t data_scaled = (byte3 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE3_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE3_RANGE, data_scaled);
}
void CAN_bms_boot_response_byte4_set(uint16_t byte4){
	uint16_t data_scaled = (byte4 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE4_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE4_RANGE, data_scaled);
}
void CAN_bms_boot_response_byte5_set(uint16_t byte5){
	uint16_t data_scaled = (byte5 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE5_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE5_RANGE, data_scaled);
}
void CAN_bms_boot_response_byte6_set(uint16_t byte6){
	uint16_t data_scaled = (byte6 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE6_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE6_RANGE, data_scaled);
}
void CAN_bms_boot_response_byte7_set(uint16_t byte7){
	uint16_t data_scaled = (byte7 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE7_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE7_RANGE, data_scaled);
}
void CAN_bms_boot_response_dlc_set(uint8_t dlc){
	CAN_bms_boot_response.dlc = dlc;
}
void CAN_bms_boot_response_send(void){
	CAN_write(CAN_bms_boot_response);
}

#define CAN_bms_charger_request_ID 0x1806e5f4

static CAN_payload_S CAN_bms_charger_request_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_charger_request={
	.canID = CAN_bms_charger_request_ID,
	.canXID = 1,
	.dlc = 8,
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
#define CAN_BMS_CHARGER_REQUEST_START_CHARGE_NOT_REQUEST_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_START_CHARGE_NOT_REQUEST_OFFSET 32
#define CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_OFFSET 40
#define CAN_BMS_CHARGER_REQUEST_BYTE_7_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_BYTE_7_OFFSET 48
#define CAN_BMS_CHARGER_REQUEST_BYTE_8_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_BYTE_8_OFFSET 56

void CAN_bms_charger_request_output_voltage_high_byte_set(uint16_t output_voltage_high_byte){
	uint16_t data_scaled = (output_voltage_high_byte - 0) / 1.0;
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE, data_scaled);
}
void CAN_bms_charger_request_output_voltage_low_byte_set(uint16_t output_voltage_low_byte){
	uint16_t data_scaled = (output_voltage_low_byte - 0) / 1.0;
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_RANGE, data_scaled);
}
void CAN_bms_charger_request_output_current_high_byte_set(uint16_t output_current_high_byte){
	uint16_t data_scaled = (output_current_high_byte - 0) / 1.0;
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_RANGE, data_scaled);
}
void CAN_bms_charger_request_output_current_low_byte_set(uint16_t output_current_low_byte){
	uint16_t data_scaled = (output_current_low_byte - 0) / 1.0;
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_RANGE, data_scaled);
}
void CAN_bms_charger_request_start_charge_not_request_set(uint16_t start_charge_not_request){
	uint16_t data_scaled = (start_charge_not_request - 0) / 1.0;
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_START_CHARGE_NOT_REQUEST_OFFSET, CAN_BMS_CHARGER_REQUEST_START_CHARGE_NOT_REQUEST_RANGE, data_scaled);
}
void CAN_bms_charger_request_charge_mode_set(uint16_t charge_mode){
	uint16_t data_scaled = (charge_mode - 0) / 1.0;
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_OFFSET, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_RANGE, data_scaled);
}
void CAN_bms_charger_request_byte_7_set(uint16_t byte_7){
	uint16_t data_scaled = (byte_7 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_7_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_7_RANGE, data_scaled);
}
void CAN_bms_charger_request_byte_8_set(uint16_t byte_8){
	uint16_t data_scaled = (byte_8 - 0) / 1.0;
	set_bits((size_t*)CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_8_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_8_RANGE, data_scaled);
}
void CAN_bms_charger_request_dlc_set(uint8_t dlc){
	CAN_bms_charger_request.dlc = dlc;
}
void CAN_bms_charger_request_send(void){
	CAN_write(CAN_bms_charger_request);
}

#define CAN_bms_cellVoltages_ID 0x725

static CAN_payload_S CAN_bms_cellVoltages_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_cellVoltages={
	.canID = CAN_bms_cellVoltages_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = &CAN_bms_cellVoltages_payload,
	.canMessageStatus = 0
};

#define CAN_BMS_CELLVOLTAGES_MULTIPLEX_RANGE 4
#define CAN_BMS_CELLVOLTAGES_MULTIPLEX_OFFSET 0
#define CAN_BMS_CELLVOLTAGES_CELL_1_VOLTAGE_RANGE 15
#define CAN_BMS_CELLVOLTAGES_CELL_1_VOLTAGE_OFFSET 4
#define CAN_BMS_CELLVOLTAGES_CELL_2_VOLTAGE_RANGE 15
#define CAN_BMS_CELLVOLTAGES_CELL_2_VOLTAGE_OFFSET 19
#define CAN_BMS_CELLVOLTAGES_CELL_3_VOLTAGE_RANGE 15
#define CAN_BMS_CELLVOLTAGES_CELL_3_VOLTAGE_OFFSET 34
#define CAN_BMS_CELLVOLTAGES_CELL_4_VOLTAGE_RANGE 15
#define CAN_BMS_CELLVOLTAGES_CELL_4_VOLTAGE_OFFSET 49

void CAN_bms_cellVoltages_MultiPlex_set(uint16_t MultiPlex){
	uint16_t data_scaled = (MultiPlex - 0) / 1.0;
	set_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_MULTIPLEX_OFFSET, CAN_BMS_CELLVOLTAGES_MULTIPLEX_RANGE, data_scaled);
}
void CAN_bms_cellVoltages_cell_1_voltage_set(float cell_1_voltage){
	uint16_t data_scaled = (cell_1_voltage - 0) / 0.001;
	set_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL_1_VOLTAGE_OFFSET, CAN_BMS_CELLVOLTAGES_CELL_1_VOLTAGE_RANGE, data_scaled);
}
void CAN_bms_cellVoltages_cell_2_voltage_set(float cell_2_voltage){
	uint16_t data_scaled = (cell_2_voltage - 0) / 0.001;
	set_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL_2_VOLTAGE_OFFSET, CAN_BMS_CELLVOLTAGES_CELL_2_VOLTAGE_RANGE, data_scaled);
}
void CAN_bms_cellVoltages_cell_3_voltage_set(float cell_3_voltage){
	uint16_t data_scaled = (cell_3_voltage - 0) / 0.001;
	set_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL_3_VOLTAGE_OFFSET, CAN_BMS_CELLVOLTAGES_CELL_3_VOLTAGE_RANGE, data_scaled);
}
void CAN_bms_cellVoltages_cell_4_voltage_set(float cell_4_voltage){
	uint16_t data_scaled = (cell_4_voltage - 0) / 0.001;
	set_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL_4_VOLTAGE_OFFSET, CAN_BMS_CELLVOLTAGES_CELL_4_VOLTAGE_RANGE, data_scaled);
}
void CAN_bms_cellVoltages_dlc_set(uint8_t dlc){
	CAN_bms_cellVoltages.dlc = dlc;
}
void CAN_bms_cellVoltages_send(void){
	CAN_write(CAN_bms_cellVoltages);
}

#define CAN_bms_cellTemperaturs_ID 0x726

static CAN_payload_S CAN_bms_cellTemperaturs_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_cellTemperaturs={
	.canID = CAN_bms_cellTemperaturs_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = &CAN_bms_cellTemperaturs_payload,
	.canMessageStatus = 0
};

#define CAN_BMS_CELLTEMPERATURS_MULTIPLEX_RANGE 4
#define CAN_BMS_CELLTEMPERATURS_MULTIPLEX_OFFSET 0
#define CAN_BMS_CELLTEMPERATURS_TEMP_1_RANGE 12
#define CAN_BMS_CELLTEMPERATURS_TEMP_1_OFFSET 4
#define CAN_BMS_CELLTEMPERATURS_TEMP_2_RANGE 12
#define CAN_BMS_CELLTEMPERATURS_TEMP_2_OFFSET 16
#define CAN_BMS_CELLTEMPERATURS_TEMP_3_RANGE 12
#define CAN_BMS_CELLTEMPERATURS_TEMP_3_OFFSET 28
#define CAN_BMS_CELLTEMPERATURS_TEMP_4_RANGE 12
#define CAN_BMS_CELLTEMPERATURS_TEMP_4_OFFSET 40

void CAN_bms_cellTemperaturs_MultiPlex_set(uint16_t MultiPlex){
	uint16_t data_scaled = (MultiPlex - 0) / 1.0;
	set_bits((size_t*)CAN_bms_cellTemperaturs.payload, CAN_BMS_CELLTEMPERATURS_MULTIPLEX_OFFSET, CAN_BMS_CELLTEMPERATURS_MULTIPLEX_RANGE, data_scaled);
}
void CAN_bms_cellTemperaturs_temp_1_set(float temp_1){
	uint16_t data_scaled = (temp_1 - -40) / 0.1;
	set_bits((size_t*)CAN_bms_cellTemperaturs.payload, CAN_BMS_CELLTEMPERATURS_TEMP_1_OFFSET, CAN_BMS_CELLTEMPERATURS_TEMP_1_RANGE, data_scaled);
}
void CAN_bms_cellTemperaturs_temp_2_set(float temp_2){
	uint16_t data_scaled = (temp_2 - -40) / 0.1;
	set_bits((size_t*)CAN_bms_cellTemperaturs.payload, CAN_BMS_CELLTEMPERATURS_TEMP_2_OFFSET, CAN_BMS_CELLTEMPERATURS_TEMP_2_RANGE, data_scaled);
}
void CAN_bms_cellTemperaturs_temp_3_set(float temp_3){
	uint16_t data_scaled = (temp_3 - -40) / 0.1;
	set_bits((size_t*)CAN_bms_cellTemperaturs.payload, CAN_BMS_CELLTEMPERATURS_TEMP_3_OFFSET, CAN_BMS_CELLTEMPERATURS_TEMP_3_RANGE, data_scaled);
}
void CAN_bms_cellTemperaturs_temp_4_set(float temp_4){
	uint16_t data_scaled = (temp_4 - -40) / 0.1;
	set_bits((size_t*)CAN_bms_cellTemperaturs.payload, CAN_BMS_CELLTEMPERATURS_TEMP_4_OFFSET, CAN_BMS_CELLTEMPERATURS_TEMP_4_RANGE, data_scaled);
}
void CAN_bms_cellTemperaturs_dlc_set(uint8_t dlc){
	CAN_bms_cellTemperaturs.dlc = dlc;
}
void CAN_bms_cellTemperaturs_send(void){
	CAN_write(CAN_bms_cellTemperaturs);
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
	.canXID = 1,
	.dlc = 8,
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
	uint16_t data = get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_charger_status_output_voltage_low_byte_get(void){
	uint16_t data = get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_charger_status_output_current_high_byte_get(void){
	uint16_t data = get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_charger_status_output_current_low_byte_get(void){
	uint16_t data = get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_charger_status_hardware_error_get(void){
	uint16_t data = get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_HARDWARE_ERROR_OFFSET, CAN_CHARGER_STATUS_HARDWARE_ERROR_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_charger_status_charger_overtemp_error_get(void){
	uint16_t data = get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_OFFSET, CAN_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_charger_status_input_voltage_error_get(void){
	uint16_t data = get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_OFFSET, CAN_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_charger_status_battery_detect_error_get(void){
	uint16_t data = get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BATTERY_DETECT_ERROR_OFFSET, CAN_CHARGER_STATUS_BATTERY_DETECT_ERROR_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_charger_status_communication_error_get(void){
	uint16_t data = get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_COMMUNICATION_ERROR_OFFSET, CAN_CHARGER_STATUS_COMMUNICATION_ERROR_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_charger_status_byte7_get(void){
	uint16_t data = get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BYTE7_OFFSET, CAN_CHARGER_STATUS_BYTE7_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_charger_status_byte8_get(void){
	uint16_t data = get_bits((size_t*)CAN_charger_status.payload, CAN_CHARGER_STATUS_BYTE8_OFFSET, CAN_CHARGER_STATUS_BYTE8_RANGE);
	return (data * 1.0) + 0;
}

/**********************************************************
 * boot_host NODE MESSAGES
 */
#define CAN_boot_host_bms_ID 0xa1

static CAN_message_S CAN_boot_host_bms={
	.canID = CAN_boot_host_bms_ID,
	.canXID = 0,
	.dlc = 8,
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
	uint16_t data = get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_CODE_OFFSET, CAN_BOOT_HOST_BMS_CODE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_bms_type_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_TYPE_OFFSET, CAN_BOOT_HOST_BMS_TYPE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_bms_byte1_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE1_OFFSET, CAN_BOOT_HOST_BMS_BYTE1_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_bms_byte2_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE2_OFFSET, CAN_BOOT_HOST_BMS_BYTE2_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_bms_byte3_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE3_OFFSET, CAN_BOOT_HOST_BMS_BYTE3_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_bms_byte4_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE4_OFFSET, CAN_BOOT_HOST_BMS_BYTE4_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_bms_byte5_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE5_OFFSET, CAN_BOOT_HOST_BMS_BYTE5_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_bms_byte6_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE6_OFFSET, CAN_BOOT_HOST_BMS_BYTE6_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_bms_byte7_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE7_OFFSET, CAN_BOOT_HOST_BMS_BYTE7_RANGE);
	return (data * 1.0) + 0;
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
	CAN_bms_debug_send();
}

void CAN_send_1000ms(void){
	CAN_bms_charger_request_send();
	CAN_bms_cellVoltages_send();
	CAN_bms_cellTemperaturs_send();
}
