#include "bms_dbc.h"




/**********************************************************
 * dash NODE MESSAGES
 */
static CAN_message_S CAN_dash_data1={
	.canID = CAN_dash_data1_ID,
	.canXID = 1,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_dash_data1_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_dash_data1);
}

uint16_t CAN_dash_data1_speed_get(void){
	return get_bits(CAN_dash_data1.payload, CAN_DASH_DATA1_SPEED_OFFSET, CAN_DASH_DATA1_SPEED_RANGE);
}
uint16_t CAN_dash_data1_odometer_get(void){
	return get_bits(CAN_dash_data1.payload, CAN_DASH_DATA1_ODOMETER_OFFSET, CAN_DASH_DATA1_ODOMETER_RANGE);
}
uint16_t CAN_dash_data1_tripA_get(void){
	return get_bits(CAN_dash_data1.payload, CAN_DASH_DATA1_TRIPA_OFFSET, CAN_DASH_DATA1_TRIPA_RANGE);
}
uint16_t CAN_dash_data1_tripB_get(void){
	return get_bits(CAN_dash_data1.payload, CAN_DASH_DATA1_TRIPB_OFFSET, CAN_DASH_DATA1_TRIPB_RANGE);
}


static CAN_message_S CAN_dash_data2={
	.canID = CAN_dash_data2_ID,
	.canXID = 0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_dash_data2_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_dash_data2);
}

uint16_t CAN_dash_data2_runningTime_get(void){
	return get_bits(CAN_dash_data2.payload, CAN_DASH_DATA2_RUNNINGTIME_OFFSET, CAN_DASH_DATA2_RUNNINGTIME_RANGE);
}
uint16_t CAN_dash_data2_odometer_get(void){
	return get_bits(CAN_dash_data2.payload, CAN_DASH_DATA2_ODOMETER_OFFSET, CAN_DASH_DATA2_ODOMETER_RANGE);
}
uint16_t CAN_dash_data2_tripA_get(void){
	return get_bits(CAN_dash_data2.payload, CAN_DASH_DATA2_TRIPA_OFFSET, CAN_DASH_DATA2_TRIPA_RANGE);
}
uint16_t CAN_dash_data2_tripB_get(void){
	return get_bits(CAN_dash_data2.payload, CAN_DASH_DATA2_TRIPB_OFFSET, CAN_DASH_DATA2_TRIPB_RANGE);
}





/**********************************************************
 * mcu NODE MESSAGES
 */
static CAN_message_S CAN_mcu_status={
	.canID = CAN_mcu_status_ID,
	.canXID = 0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_mcu_status_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_mcu_status);
}

uint16_t CAN_mcu_status_state_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_STATE_OFFSET, CAN_MCU_STATUS_STATE_RANGE);
}
uint16_t CAN_mcu_status_throttleMode_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEMODE_OFFSET, CAN_MCU_STATUS_THROTTLEMODE_RANGE);
}
uint16_t CAN_mcu_status_highBeam_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_HIGHBEAM_OFFSET, CAN_MCU_STATUS_HIGHBEAM_RANGE);
}
uint16_t CAN_mcu_status_lowBeam_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_LOWBEAM_OFFSET, CAN_MCU_STATUS_LOWBEAM_RANGE);
}
uint16_t CAN_mcu_status_brakeLight_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKELIGHT_OFFSET, CAN_MCU_STATUS_BRAKELIGHT_RANGE);
}
uint16_t CAN_mcu_status_tailLight_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_TAILLIGHT_OFFSET, CAN_MCU_STATUS_TAILLIGHT_RANGE);
}
uint16_t CAN_mcu_status_horn_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_HORN_OFFSET, CAN_MCU_STATUS_HORN_RANGE);
}
uint16_t CAN_mcu_status_turnSignalFR_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFR_OFFSET, CAN_MCU_STATUS_TURNSIGNALFR_RANGE);
}
uint16_t CAN_mcu_status_turnSignalFL_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFL_OFFSET, CAN_MCU_STATUS_TURNSIGNALFL_RANGE);
}
uint16_t CAN_mcu_status_turnSignalRR_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRR_OFFSET, CAN_MCU_STATUS_TURNSIGNALRR_RANGE);
}
uint16_t CAN_mcu_status_turnSignalRL_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRL_OFFSET, CAN_MCU_STATUS_TURNSIGNALRL_RANGE);
}
uint16_t CAN_mcu_status_GBSALL_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_GBSALL_OFFSET, CAN_MCU_STATUS_GBSALL_RANGE);
}
uint16_t CAN_mcu_status_contactor_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_CONTACTOR_OFFSET, CAN_MCU_STATUS_CONTACTOR_RANGE);
}
uint16_t CAN_mcu_status_chargePort_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_CHARGEPORT_OFFSET, CAN_MCU_STATUS_CHARGEPORT_RANGE);
}
uint16_t CAN_mcu_status_brakeSwitchFront_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHFRONT_OFFSET, CAN_MCU_STATUS_BRAKESWITCHFRONT_RANGE);
}
uint16_t CAN_mcu_status_brakeSwitchRear_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHREAR_OFFSET, CAN_MCU_STATUS_BRAKESWITCHREAR_RANGE);
}
uint16_t CAN_mcu_status_throttleVal_get(void){
	return get_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEVAL_OFFSET, CAN_MCU_STATUS_THROTTLEVAL_RANGE);
}


static CAN_message_S CAN_mcu_command={
	.canID = CAN_mcu_command_ID,
	.canXID = 0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_mcu_command_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_mcu_command);
}

uint16_t CAN_mcu_command_doSomthingElse_get(void){
	return get_bits(CAN_mcu_command.payload, CAN_MCU_COMMAND_DOSOMTHINGELSE_OFFSET, CAN_MCU_COMMAND_DOSOMTHINGELSE_RANGE);
}


static CAN_message_S CAN_mcu_motorControllerRequest={
	.canID = CAN_mcu_motorControllerRequest_ID,
	.canXID = 0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_mcu_motorControllerRequest_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_mcu_motorControllerRequest);
}

uint16_t CAN_mcu_motorControllerRequest_requestType_get(void){
	return get_bits(CAN_mcu_motorControllerRequest.payload, CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_OFFSET, CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_RANGE);
}





/**********************************************************
 * bms NODE MESSAGES
 */
static CAN_payload_S CAN_bms_status_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_status={
	.canID = CAN_bms_status_ID,
	.canXID = 0,
	.payload = &CAN_bms_status_payload,
	.canMessageStatus = 0
};

void CAN_bms_status_state_set(uint16_t state){
	set_bits(CAN_bms_status.payload, CAN_BMS_STATUS_STATE_OFFSET, CAN_BMS_STATUS_STATE_RANGE, state);
}
void CAN_bms_status_SOC_set(uint16_t SOC){
	set_bits(CAN_bms_status.payload, CAN_BMS_STATUS_SOC_OFFSET, CAN_BMS_STATUS_SOC_RANGE, SOC);
}
void CAN_bms_status_packVoltage_set(uint16_t packVoltage){
	set_bits(CAN_bms_status.payload, CAN_BMS_STATUS_PACKVOLTAGE_OFFSET, CAN_BMS_STATUS_PACKVOLTAGE_RANGE, packVoltage);
}
void CAN_bms_status_packCurrent_set(uint16_t packCurrent){
	set_bits(CAN_bms_status.payload, CAN_BMS_STATUS_PACKCURRENT_OFFSET, CAN_BMS_STATUS_PACKCURRENT_RANGE, packCurrent);
}
void CAN_bms_status_minTemp_set(uint16_t minTemp){
	set_bits(CAN_bms_status.payload, CAN_BMS_STATUS_MINTEMP_OFFSET, CAN_BMS_STATUS_MINTEMP_RANGE, minTemp);
}
void CAN_bms_status_maxTemp_set(uint16_t maxTemp){
	set_bits(CAN_bms_status.payload, CAN_BMS_STATUS_MAXTEMP_OFFSET, CAN_BMS_STATUS_MAXTEMP_RANGE, maxTemp);
}

void CAN_bms_status_send(void){
	CAN_write(CAN_bms_status);
}

static CAN_payload_S CAN_bms_boot_response_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_boot_response={
	.canID = CAN_bms_boot_response_ID,
	.canXID = 0,
	.payload = &CAN_bms_boot_response_payload,
	.canMessageStatus = 0
};

void CAN_bms_boot_response_code_set(uint16_t code){
	set_bits(CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_CODE_OFFSET, CAN_BMS_BOOT_RESPONSE_CODE_RANGE, code);
}
void CAN_bms_boot_response_type_set(uint16_t type){
	set_bits(CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_TYPE_OFFSET, CAN_BMS_BOOT_RESPONSE_TYPE_RANGE, type);
}
void CAN_bms_boot_response_byte1_set(uint16_t byte1){
	set_bits(CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE1_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE1_RANGE, byte1);
}
void CAN_bms_boot_response_byte2_set(uint16_t byte2){
	set_bits(CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE2_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE2_RANGE, byte2);
}
void CAN_bms_boot_response_byte3_set(uint16_t byte3){
	set_bits(CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE3_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE3_RANGE, byte3);
}
void CAN_bms_boot_response_byte4_set(uint16_t byte4){
	set_bits(CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE4_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE4_RANGE, byte4);
}
void CAN_bms_boot_response_byte5_set(uint16_t byte5){
	set_bits(CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE5_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE5_RANGE, byte5);
}
void CAN_bms_boot_response_byte6_set(uint16_t byte6){
	set_bits(CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE6_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE6_RANGE, byte6);
}
void CAN_bms_boot_response_byte7_set(uint16_t byte7){
	set_bits(CAN_bms_boot_response.payload, CAN_BMS_BOOT_RESPONSE_BYTE7_OFFSET, CAN_BMS_BOOT_RESPONSE_BYTE7_RANGE, byte7);
}

void CAN_bms_boot_response_send(void){
	CAN_write(CAN_bms_boot_response);
}

static CAN_payload_S CAN_bms_charger_request_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_charger_request={
	.canID = CAN_bms_charger_request_ID,
	.canXID = 1,
	.payload = &CAN_bms_charger_request_payload,
	.canMessageStatus = 0
};

void CAN_bms_charger_request_output_voltage_high_byte_set(uint16_t output_voltage_high_byte){
	set_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE, output_voltage_high_byte);
}
void CAN_bms_charger_request_output_voltage_low_byte_set(uint16_t output_voltage_low_byte){
	set_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_RANGE, output_voltage_low_byte);
}
void CAN_bms_charger_request_output_current_high_byte_set(uint16_t output_current_high_byte){
	set_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_RANGE, output_current_high_byte);
}
void CAN_bms_charger_request_output_current_low_byte_set(uint16_t output_current_low_byte){
	set_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_RANGE, output_current_low_byte);
}
void CAN_bms_charger_request_start_charge_request_set(uint16_t start_charge_request){
	set_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_OFFSET, CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_RANGE, start_charge_request);
}
void CAN_bms_charger_request_charge_mode_set(uint16_t charge_mode){
	set_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_OFFSET, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_RANGE, charge_mode);
}
void CAN_bms_charger_request_packCurrent_set(uint16_t packCurrent){
	set_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_PACKCURRENT_OFFSET, CAN_BMS_CHARGER_REQUEST_PACKCURRENT_RANGE, packCurrent);
}
void CAN_bms_charger_request_byte_7_set(uint16_t byte_7){
	set_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_7_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_7_RANGE, byte_7);
}
void CAN_bms_charger_request_byte_8_set(uint16_t byte_8){
	set_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_8_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_8_RANGE, byte_8);
}

void CAN_bms_charger_request_send(void){
	CAN_write(CAN_bms_charger_request);
}

static CAN_payload_S CAN_bms_cellVoltages_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_bms_cellVoltages={
	.canID = CAN_bms_cellVoltages_ID,
	.canXID = 0,
	.payload = &CAN_bms_cellVoltages_payload,
	.canMessageStatus = 0
};

void CAN_bms_cellVoltages_MultiPlex_set(uint16_t MultiPlex){
	set_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_MULTIPLEX_OFFSET, CAN_BMS_CELLVOLTAGES_MULTIPLEX_RANGE, MultiPlex);
}
void CAN_bms_cellVoltages_cell1_set(uint16_t cell1){
	set_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL1_OFFSET, CAN_BMS_CELLVOLTAGES_CELL1_RANGE, cell1);
}
void CAN_bms_cellVoltages_cell2_set(uint16_t cell2){
	set_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL2_OFFSET, CAN_BMS_CELLVOLTAGES_CELL2_RANGE, cell2);
}
void CAN_bms_cellVoltages_cell3_set(uint16_t cell3){
	set_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL3_OFFSET, CAN_BMS_CELLVOLTAGES_CELL3_RANGE, cell3);
}
void CAN_bms_cellVoltages_cell4_set(uint16_t cell4){
	set_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL4_OFFSET, CAN_BMS_CELLVOLTAGES_CELL4_RANGE, cell4);
}
void CAN_bms_cellVoltages_cell5_set(uint16_t cell5){
	set_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL5_OFFSET, CAN_BMS_CELLVOLTAGES_CELL5_RANGE, cell5);
}

void CAN_bms_cellVoltages_send(void){
	CAN_write(CAN_bms_cellVoltages);
}




/**********************************************************
 * motorcontroller NODE MESSAGES
 */
static CAN_message_S CAN_motorcontroller_motorStatus={
	.canID = CAN_motorcontroller_motorStatus_ID,
	.canXID = 0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_motorcontroller_motorStatus_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_motorcontroller_motorStatus);
}

uint16_t CAN_motorcontroller_motorStatus_motorSpeed_get(void){
	return get_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORSPEED_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORSPEED_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_motorCurrent_get(void){
	return get_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORCURRENT_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORCURRENT_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_IphaseA_get(void){
	return get_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEA_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEA_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_IphaseB_get(void){
	return get_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEB_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEB_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_IphaseC_get(void){
	return get_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEC_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEC_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_VphaseA_get(void){
	return get_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEA_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEA_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_VphaseB_get(void){
	return get_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEB_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEB_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_VphaseC_get(void){
	return get_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEC_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEC_RANGE);
}


static CAN_message_S CAN_motorcontroller_response={
	.canID = CAN_motorcontroller_response_ID,
	.canXID = 0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_motorcontroller_response_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_motorcontroller_response);
}

uint16_t CAN_motorcontroller_response_byte1_get(void){
	return get_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE1_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE1_RANGE);
}
uint16_t CAN_motorcontroller_response_byte2_get(void){
	return get_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE2_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE2_RANGE);
}
uint16_t CAN_motorcontroller_response_byte3_get(void){
	return get_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE3_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE3_RANGE);
}
uint16_t CAN_motorcontroller_response_byte4_get(void){
	return get_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE4_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE4_RANGE);
}
uint16_t CAN_motorcontroller_response_byte5_get(void){
	return get_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE5_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE5_RANGE);
}
uint16_t CAN_motorcontroller_response_byte6_get(void){
	return get_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE6_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE6_RANGE);
}
uint16_t CAN_motorcontroller_response_byte7_get(void){
	return get_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE7_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE7_RANGE);
}
uint16_t CAN_motorcontroller_response_byte8_get(void){
	return get_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE8_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE8_RANGE);
}





/**********************************************************
 * charger NODE MESSAGES
 */
static CAN_message_S CAN_charger_charger_status={
	.canID = CAN_charger_charger_status_ID,
	.canXID = 1,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_charger_charger_status_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_charger_charger_status);
}

uint16_t CAN_charger_charger_status_output_voltage_high_byte_get(void){
	return get_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE);
}
uint16_t CAN_charger_charger_status_output_voltage_low_byte_get(void){
	return get_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_RANGE);
}
uint16_t CAN_charger_charger_status_output_current_high_byte_get(void){
	return get_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_RANGE);
}
uint16_t CAN_charger_charger_status_output_current_low_byte_get(void){
	return get_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_RANGE);
}
uint16_t CAN_charger_charger_status_hardware_error_get(void){
	return get_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_HARDWARE_ERROR_OFFSET, CAN_CHARGER_CHARGER_STATUS_HARDWARE_ERROR_RANGE);
}
uint16_t CAN_charger_charger_status_charger_overtemp_error_get(void){
	return get_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_OFFSET, CAN_CHARGER_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_RANGE);
}
uint16_t CAN_charger_charger_status_input_voltage_error_get(void){
	return get_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_OFFSET, CAN_CHARGER_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_RANGE);
}
uint16_t CAN_charger_charger_status_battery_detect_error_get(void){
	return get_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_BATTERY_DETECT_ERROR_OFFSET, CAN_CHARGER_CHARGER_STATUS_BATTERY_DETECT_ERROR_RANGE);
}
uint16_t CAN_charger_charger_status_communication_error_get(void){
	return get_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_COMMUNICATION_ERROR_OFFSET, CAN_CHARGER_CHARGER_STATUS_COMMUNICATION_ERROR_RANGE);
}
uint16_t CAN_charger_charger_status_byte7_get(void){
	return get_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_BYTE7_OFFSET, CAN_CHARGER_CHARGER_STATUS_BYTE7_RANGE);
}
uint16_t CAN_charger_charger_status_byte8_get(void){
	return get_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_BYTE8_OFFSET, CAN_CHARGER_CHARGER_STATUS_BYTE8_RANGE);
}





/**********************************************************
 * boot_host NODE MESSAGES
 */
static CAN_message_S CAN_boot_host_bms={
	.canID = CAN_boot_host_bms_ID,
	.canXID = 0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_boot_host_bms_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_boot_host_bms);
}

uint16_t CAN_boot_host_bms_code_get(void){
	return get_bits(CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_CODE_OFFSET, CAN_BOOT_HOST_BMS_CODE_RANGE);
}
uint16_t CAN_boot_host_bms_type_get(void){
	return get_bits(CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_TYPE_OFFSET, CAN_BOOT_HOST_BMS_TYPE_RANGE);
}
uint16_t CAN_boot_host_bms_byte1_get(void){
	return get_bits(CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE1_OFFSET, CAN_BOOT_HOST_BMS_BYTE1_RANGE);
}
uint16_t CAN_boot_host_bms_byte2_get(void){
	return get_bits(CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE2_OFFSET, CAN_BOOT_HOST_BMS_BYTE2_RANGE);
}
uint16_t CAN_boot_host_bms_byte3_get(void){
	return get_bits(CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE3_OFFSET, CAN_BOOT_HOST_BMS_BYTE3_RANGE);
}
uint16_t CAN_boot_host_bms_byte4_get(void){
	return get_bits(CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE4_OFFSET, CAN_BOOT_HOST_BMS_BYTE4_RANGE);
}
uint16_t CAN_boot_host_bms_byte5_get(void){
	return get_bits(CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE5_OFFSET, CAN_BOOT_HOST_BMS_BYTE5_RANGE);
}
uint16_t CAN_boot_host_bms_byte6_get(void){
	return get_bits(CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE6_OFFSET, CAN_BOOT_HOST_BMS_BYTE6_RANGE);
}
uint16_t CAN_boot_host_bms_byte7_get(void){
	return get_bits(CAN_boot_host_bms.payload, CAN_BOOT_HOST_BMS_BYTE7_OFFSET, CAN_BOOT_HOST_BMS_BYTE7_RANGE);
}


void CAN_DBC_init() {
	CAN_configureMailbox(&CAN_dash_data1);
	CAN_configureMailbox(&CAN_dash_data2);
	CAN_configureMailbox(&CAN_mcu_status);
	CAN_configureMailbox(&CAN_mcu_command);
	CAN_configureMailbox(&CAN_mcu_motorControllerRequest);
	CAN_configureMailbox(&CAN_motorcontroller_motorStatus);
	CAN_configureMailbox(&CAN_motorcontroller_response);
	CAN_configureMailbox(&CAN_charger_charger_status);
	CAN_configureMailbox(&CAN_boot_host_bms);
}
