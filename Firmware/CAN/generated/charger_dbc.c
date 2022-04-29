#include "charger_dbc.h"




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
static CAN_message_S CAN_bms_charger_request={
	.canID = CAN_bms_charger_request_ID,
	.canXID = 1,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_bms_charger_request_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_bms_charger_request);
}

uint16_t CAN_bms_charger_request_output_voltage_high_byte_get(void){
	return get_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE);
}
uint16_t CAN_bms_charger_request_output_voltage_low_byte_get(void){
	return get_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_RANGE);
}
uint16_t CAN_bms_charger_request_output_current_high_byte_get(void){
	return get_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_RANGE);
}
uint16_t CAN_bms_charger_request_output_current_low_byte_get(void){
	return get_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_RANGE);
}
uint16_t CAN_bms_charger_request_start_charge_request_get(void){
	return get_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_OFFSET, CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_RANGE);
}
uint16_t CAN_bms_charger_request_charge_mode_get(void){
	return get_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_OFFSET, CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_RANGE);
}
uint16_t CAN_bms_charger_request_packCurrent_get(void){
	return get_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_PACKCURRENT_OFFSET, CAN_BMS_CHARGER_REQUEST_PACKCURRENT_RANGE);
}
uint16_t CAN_bms_charger_request_byte_7_get(void){
	return get_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_7_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_7_RANGE);
}
uint16_t CAN_bms_charger_request_byte_8_get(void){
	return get_bits(CAN_bms_charger_request.payload, CAN_BMS_CHARGER_REQUEST_BYTE_8_OFFSET, CAN_BMS_CHARGER_REQUEST_BYTE_8_RANGE);
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
static CAN_payload_S CAN_charger_charger_status_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_charger_charger_status={
	.canID = CAN_charger_charger_status_ID,
	.canXID = 1,
	.payload = &CAN_charger_charger_status_payload,
	.canMessageStatus = 0
};

void CAN_charger_charger_status_output_voltage_high_byte_set(uint16_t output_voltage_high_byte){
	set_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET, CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE, output_voltage_high_byte);
}
void CAN_charger_charger_status_output_voltage_low_byte_set(uint16_t output_voltage_low_byte){
	set_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET, CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_RANGE, output_voltage_low_byte);
}
void CAN_charger_charger_status_output_current_high_byte_set(uint16_t output_current_high_byte){
	set_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_OFFSET, CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_RANGE, output_current_high_byte);
}
void CAN_charger_charger_status_output_current_low_byte_set(uint16_t output_current_low_byte){
	set_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_OFFSET, CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_RANGE, output_current_low_byte);
}
void CAN_charger_charger_status_hardware_error_set(uint16_t hardware_error){
	set_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_HARDWARE_ERROR_OFFSET, CAN_CHARGER_CHARGER_STATUS_HARDWARE_ERROR_RANGE, hardware_error);
}
void CAN_charger_charger_status_charger_overtemp_error_set(uint16_t charger_overtemp_error){
	set_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_OFFSET, CAN_CHARGER_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_RANGE, charger_overtemp_error);
}
void CAN_charger_charger_status_input_voltage_error_set(uint16_t input_voltage_error){
	set_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_OFFSET, CAN_CHARGER_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_RANGE, input_voltage_error);
}
void CAN_charger_charger_status_battery_detect_error_set(uint16_t battery_detect_error){
	set_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_BATTERY_DETECT_ERROR_OFFSET, CAN_CHARGER_CHARGER_STATUS_BATTERY_DETECT_ERROR_RANGE, battery_detect_error);
}
void CAN_charger_charger_status_communication_error_set(uint16_t communication_error){
	set_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_COMMUNICATION_ERROR_OFFSET, CAN_CHARGER_CHARGER_STATUS_COMMUNICATION_ERROR_RANGE, communication_error);
}
void CAN_charger_charger_status_byte7_set(uint16_t byte7){
	set_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_BYTE7_OFFSET, CAN_CHARGER_CHARGER_STATUS_BYTE7_RANGE, byte7);
}
void CAN_charger_charger_status_byte8_set(uint16_t byte8){
	set_bits(CAN_charger_charger_status.payload, CAN_CHARGER_CHARGER_STATUS_BYTE8_OFFSET, CAN_CHARGER_CHARGER_STATUS_BYTE8_RANGE, byte8);
}

void CAN_charger_charger_status_send(void){
	CAN_write(CAN_charger_charger_status);
}




/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init() {
	CAN_configureMailbox(&CAN_dash_data1);
	CAN_configureMailbox(&CAN_dash_data2);
	CAN_configureMailbox(&CAN_mcu_command);
	CAN_configureMailbox(&CAN_mcu_motorControllerRequest);
	CAN_configureMailbox(&CAN_bms_charger_request);
	CAN_configureMailbox(&CAN_motorcontroller_motorStatus);
	CAN_configureMailbox(&CAN_motorcontroller_response);
}