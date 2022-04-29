#include "dash_dbc.h"




/**********************************************************
 * dash NODE MESSAGES
 */
static CAN_payload_S CAN_dash_status_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_dash_status={
	.canID = CAN_dash_status_ID,
	.canXID = 0,
	.payload = &CAN_dash_status_payload,
	.canMessageStatus = 0
};

void CAN_dash_status_heartBeat_set(uint16_t heartBeat){
	set_bits(CAN_dash_status.payload, CAN_DASH_STATUS_HEARTBEAT_OFFSET, CAN_DASH_STATUS_HEARTBEAT_RANGE, heartBeat);
}
void CAN_dash_status_state_set(uint16_t state){
	set_bits(CAN_dash_status.payload, CAN_DASH_STATUS_STATE_OFFSET, CAN_DASH_STATUS_STATE_RANGE, state);
}
void CAN_dash_status_killButton_set(uint16_t killButton){
	set_bits(CAN_dash_status.payload, CAN_DASH_STATUS_KILLBUTTON_OFFSET, CAN_DASH_STATUS_KILLBUTTON_RANGE, killButton);
}
void CAN_dash_status_ignButton_set(uint16_t ignButton){
	set_bits(CAN_dash_status.payload, CAN_DASH_STATUS_IGNBUTTON_OFFSET, CAN_DASH_STATUS_IGNBUTTON_RANGE, ignButton);
}
void CAN_dash_status_modeButton_set(uint16_t modeButton){
	set_bits(CAN_dash_status.payload, CAN_DASH_STATUS_MODEBUTTON_OFFSET, CAN_DASH_STATUS_MODEBUTTON_RANGE, modeButton);
}
void CAN_dash_status_selectButton_set(uint16_t selectButton){
	set_bits(CAN_dash_status.payload, CAN_DASH_STATUS_SELECTBUTTON_OFFSET, CAN_DASH_STATUS_SELECTBUTTON_RANGE, selectButton);
}
void CAN_dash_status_driveMode_set(uint16_t driveMode){
	set_bits(CAN_dash_status.payload, CAN_DASH_STATUS_DRIVEMODE_OFFSET, CAN_DASH_STATUS_DRIVEMODE_RANGE, driveMode);
}

void CAN_dash_status_send(void){
	CAN_write(CAN_dash_status);
}

static CAN_payload_S CAN_dash_command_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_dash_command={
	.canID = CAN_dash_command_ID,
	.canXID = 0,
	.payload = &CAN_dash_command_payload,
	.canMessageStatus = 0
};

void CAN_dash_command_ignitionRequest_set(uint16_t ignitionRequest){
	set_bits(CAN_dash_command.payload, CAN_DASH_COMMAND_IGNITIONREQUEST_OFFSET, CAN_DASH_COMMAND_IGNITIONREQUEST_RANGE, ignitionRequest);
}
void CAN_dash_command_killRequest_set(uint16_t killRequest){
	set_bits(CAN_dash_command.payload, CAN_DASH_COMMAND_KILLREQUEST_OFFSET, CAN_DASH_COMMAND_KILLREQUEST_RANGE, killRequest);
}
void CAN_dash_command_batteryEjectRequest_set(uint16_t batteryEjectRequest){
	set_bits(CAN_dash_command.payload, CAN_DASH_COMMAND_BATTERYEJECTREQUEST_OFFSET, CAN_DASH_COMMAND_BATTERYEJECTREQUEST_RANGE, batteryEjectRequest);
}
void CAN_dash_command_lightsRequest_set(uint16_t lightsRequest){
	set_bits(CAN_dash_command.payload, CAN_DASH_COMMAND_LIGHTSREQUEST_OFFSET, CAN_DASH_COMMAND_LIGHTSREQUEST_RANGE, lightsRequest);
}
void CAN_dash_command_hornRequest_set(uint16_t hornRequest){
	set_bits(CAN_dash_command.payload, CAN_DASH_COMMAND_HORNREQUEST_OFFSET, CAN_DASH_COMMAND_HORNREQUEST_RANGE, hornRequest);
}

void CAN_dash_command_send(void){
	CAN_write(CAN_dash_command);
}

static CAN_payload_S CAN_dash_data1_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_dash_data1={
	.canID = CAN_dash_data1_ID,
	.canXID = 1,
	.payload = &CAN_dash_data1_payload,
	.canMessageStatus = 0
};

void CAN_dash_data1_speed_set(uint16_t speed){
	set_bits(CAN_dash_data1.payload, CAN_DASH_DATA1_SPEED_OFFSET, CAN_DASH_DATA1_SPEED_RANGE, speed);
}
void CAN_dash_data1_odometer_set(uint16_t odometer){
	set_bits(CAN_dash_data1.payload, CAN_DASH_DATA1_ODOMETER_OFFSET, CAN_DASH_DATA1_ODOMETER_RANGE, odometer);
}
void CAN_dash_data1_tripA_set(uint16_t tripA){
	set_bits(CAN_dash_data1.payload, CAN_DASH_DATA1_TRIPA_OFFSET, CAN_DASH_DATA1_TRIPA_RANGE, tripA);
}
void CAN_dash_data1_tripB_set(uint16_t tripB){
	set_bits(CAN_dash_data1.payload, CAN_DASH_DATA1_TRIPB_OFFSET, CAN_DASH_DATA1_TRIPB_RANGE, tripB);
}

void CAN_dash_data1_send(void){
	CAN_write(CAN_dash_data1);
}

static CAN_payload_S CAN_dash_data2_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_dash_data2={
	.canID = CAN_dash_data2_ID,
	.canXID = 0,
	.payload = &CAN_dash_data2_payload,
	.canMessageStatus = 0
};

void CAN_dash_data2_runningTime_set(uint16_t runningTime){
	set_bits(CAN_dash_data2.payload, CAN_DASH_DATA2_RUNNINGTIME_OFFSET, CAN_DASH_DATA2_RUNNINGTIME_RANGE, runningTime);
}
void CAN_dash_data2_odometer_set(uint16_t odometer){
	set_bits(CAN_dash_data2.payload, CAN_DASH_DATA2_ODOMETER_OFFSET, CAN_DASH_DATA2_ODOMETER_RANGE, odometer);
}
void CAN_dash_data2_tripA_set(uint16_t tripA){
	set_bits(CAN_dash_data2.payload, CAN_DASH_DATA2_TRIPA_OFFSET, CAN_DASH_DATA2_TRIPA_RANGE, tripA);
}
void CAN_dash_data2_tripB_set(uint16_t tripB){
	set_bits(CAN_dash_data2.payload, CAN_DASH_DATA2_TRIPB_OFFSET, CAN_DASH_DATA2_TRIPB_RANGE, tripB);
}

void CAN_dash_data2_send(void){
	CAN_write(CAN_dash_data2);
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
static CAN_message_S CAN_bms_status={
	.canID = CAN_bms_status_ID,
	.canXID = 0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_bms_status_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_bms_status);
}

uint16_t CAN_bms_status_state_get(void){
	return get_bits(CAN_bms_status.payload, CAN_BMS_STATUS_STATE_OFFSET, CAN_BMS_STATUS_STATE_RANGE);
}
uint16_t CAN_bms_status_SOC_get(void){
	return get_bits(CAN_bms_status.payload, CAN_BMS_STATUS_SOC_OFFSET, CAN_BMS_STATUS_SOC_RANGE);
}
uint16_t CAN_bms_status_packVoltage_get(void){
	return get_bits(CAN_bms_status.payload, CAN_BMS_STATUS_PACKVOLTAGE_OFFSET, CAN_BMS_STATUS_PACKVOLTAGE_RANGE);
}
uint16_t CAN_bms_status_packCurrent_get(void){
	return get_bits(CAN_bms_status.payload, CAN_BMS_STATUS_PACKCURRENT_OFFSET, CAN_BMS_STATUS_PACKCURRENT_RANGE);
}
uint16_t CAN_bms_status_minTemp_get(void){
	return get_bits(CAN_bms_status.payload, CAN_BMS_STATUS_MINTEMP_OFFSET, CAN_BMS_STATUS_MINTEMP_RANGE);
}
uint16_t CAN_bms_status_maxTemp_get(void){
	return get_bits(CAN_bms_status.payload, CAN_BMS_STATUS_MAXTEMP_OFFSET, CAN_BMS_STATUS_MAXTEMP_RANGE);
}


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


static CAN_message_S CAN_bms_cellVoltages={
	.canID = CAN_bms_cellVoltages_ID,
	.canXID = 0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_bms_cellVoltages_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_bms_cellVoltages);
}

uint16_t CAN_bms_cellVoltages_MultiPlex_get(void){
	return get_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_MULTIPLEX_OFFSET, CAN_BMS_CELLVOLTAGES_MULTIPLEX_RANGE);
}
uint16_t CAN_bms_cellVoltages_cell1_get(void){
	return get_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL1_OFFSET, CAN_BMS_CELLVOLTAGES_CELL1_RANGE);
}
uint16_t CAN_bms_cellVoltages_cell2_get(void){
	return get_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL2_OFFSET, CAN_BMS_CELLVOLTAGES_CELL2_RANGE);
}
uint16_t CAN_bms_cellVoltages_cell3_get(void){
	return get_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL3_OFFSET, CAN_BMS_CELLVOLTAGES_CELL3_RANGE);
}
uint16_t CAN_bms_cellVoltages_cell4_get(void){
	return get_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL4_OFFSET, CAN_BMS_CELLVOLTAGES_CELL4_RANGE);
}
uint16_t CAN_bms_cellVoltages_cell5_get(void){
	return get_bits(CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL5_OFFSET, CAN_BMS_CELLVOLTAGES_CELL5_RANGE);
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



/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init() {
	CAN_configureMailbox(&CAN_mcu_status);
	CAN_configureMailbox(&CAN_mcu_command);
	CAN_configureMailbox(&CAN_mcu_motorControllerRequest);
	CAN_configureMailbox(&CAN_bms_status);
	CAN_configureMailbox(&CAN_bms_charger_request);
	CAN_configureMailbox(&CAN_bms_cellVoltages);
	CAN_configureMailbox(&CAN_motorcontroller_motorStatus);
	CAN_configureMailbox(&CAN_motorcontroller_response);
}