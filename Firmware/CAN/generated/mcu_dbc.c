#include "mcu_dbc.h"




/**********************************************************
 * dash NODE MESSAGES
 */
static CAN_message_S CAN_dash_status={
	.canID = CAN_dash_status_ID,
	.canXID = 0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_dash_status_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_dash_status);
}

uint16_t CAN_dash_status_heartBeat_get(void){
	return get_bits(CAN_dash_status.payload, CAN_DASH_STATUS_HEARTBEAT_OFFSET, CAN_DASH_STATUS_HEARTBEAT_RANGE);
}
uint16_t CAN_dash_status_state_get(void){
	return get_bits(CAN_dash_status.payload, CAN_DASH_STATUS_STATE_OFFSET, CAN_DASH_STATUS_STATE_RANGE);
}
uint16_t CAN_dash_status_killButton_get(void){
	return get_bits(CAN_dash_status.payload, CAN_DASH_STATUS_KILLBUTTON_OFFSET, CAN_DASH_STATUS_KILLBUTTON_RANGE);
}
uint16_t CAN_dash_status_ignButton_get(void){
	return get_bits(CAN_dash_status.payload, CAN_DASH_STATUS_IGNBUTTON_OFFSET, CAN_DASH_STATUS_IGNBUTTON_RANGE);
}
uint16_t CAN_dash_status_modeButton_get(void){
	return get_bits(CAN_dash_status.payload, CAN_DASH_STATUS_MODEBUTTON_OFFSET, CAN_DASH_STATUS_MODEBUTTON_RANGE);
}
uint16_t CAN_dash_status_selectButton_get(void){
	return get_bits(CAN_dash_status.payload, CAN_DASH_STATUS_SELECTBUTTON_OFFSET, CAN_DASH_STATUS_SELECTBUTTON_RANGE);
}
uint16_t CAN_dash_status_driveMode_get(void){
	return get_bits(CAN_dash_status.payload, CAN_DASH_STATUS_DRIVEMODE_OFFSET, CAN_DASH_STATUS_DRIVEMODE_RANGE);
}


static CAN_message_S CAN_dash_command={
	.canID = CAN_dash_command_ID,
	.canXID = 0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_dash_command_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_dash_command);
}

uint16_t CAN_dash_command_ignitionRequest_get(void){
	return get_bits(CAN_dash_command.payload, CAN_DASH_COMMAND_IGNITIONREQUEST_OFFSET, CAN_DASH_COMMAND_IGNITIONREQUEST_RANGE);
}
uint16_t CAN_dash_command_killRequest_get(void){
	return get_bits(CAN_dash_command.payload, CAN_DASH_COMMAND_KILLREQUEST_OFFSET, CAN_DASH_COMMAND_KILLREQUEST_RANGE);
}
uint16_t CAN_dash_command_batteryEjectRequest_get(void){
	return get_bits(CAN_dash_command.payload, CAN_DASH_COMMAND_BATTERYEJECTREQUEST_OFFSET, CAN_DASH_COMMAND_BATTERYEJECTREQUEST_RANGE);
}
uint16_t CAN_dash_command_lightsRequest_get(void){
	return get_bits(CAN_dash_command.payload, CAN_DASH_COMMAND_LIGHTSREQUEST_OFFSET, CAN_DASH_COMMAND_LIGHTSREQUEST_RANGE);
}
uint16_t CAN_dash_command_hornRequest_get(void){
	return get_bits(CAN_dash_command.payload, CAN_DASH_COMMAND_HORNREQUEST_OFFSET, CAN_DASH_COMMAND_HORNREQUEST_RANGE);
}


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
static CAN_payload_S CAN_mcu_status_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_mcu_status={
	.canID = CAN_mcu_status_ID,
	.canXID = 0,
	.payload = &CAN_mcu_status_payload,
	.canMessageStatus = 0
};

void CAN_mcu_status_state_set(uint16_t state){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_STATE_OFFSET, CAN_MCU_STATUS_STATE_RANGE, state);
}
void CAN_mcu_status_throttleMode_set(uint16_t throttleMode){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEMODE_OFFSET, CAN_MCU_STATUS_THROTTLEMODE_RANGE, throttleMode);
}
void CAN_mcu_status_highBeam_set(uint16_t highBeam){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_HIGHBEAM_OFFSET, CAN_MCU_STATUS_HIGHBEAM_RANGE, highBeam);
}
void CAN_mcu_status_lowBeam_set(uint16_t lowBeam){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_LOWBEAM_OFFSET, CAN_MCU_STATUS_LOWBEAM_RANGE, lowBeam);
}
void CAN_mcu_status_brakeLight_set(uint16_t brakeLight){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKELIGHT_OFFSET, CAN_MCU_STATUS_BRAKELIGHT_RANGE, brakeLight);
}
void CAN_mcu_status_tailLight_set(uint16_t tailLight){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_TAILLIGHT_OFFSET, CAN_MCU_STATUS_TAILLIGHT_RANGE, tailLight);
}
void CAN_mcu_status_horn_set(uint16_t horn){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_HORN_OFFSET, CAN_MCU_STATUS_HORN_RANGE, horn);
}
void CAN_mcu_status_turnSignalFR_set(uint16_t turnSignalFR){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFR_OFFSET, CAN_MCU_STATUS_TURNSIGNALFR_RANGE, turnSignalFR);
}
void CAN_mcu_status_turnSignalFL_set(uint16_t turnSignalFL){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFL_OFFSET, CAN_MCU_STATUS_TURNSIGNALFL_RANGE, turnSignalFL);
}
void CAN_mcu_status_turnSignalRR_set(uint16_t turnSignalRR){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRR_OFFSET, CAN_MCU_STATUS_TURNSIGNALRR_RANGE, turnSignalRR);
}
void CAN_mcu_status_turnSignalRL_set(uint16_t turnSignalRL){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRL_OFFSET, CAN_MCU_STATUS_TURNSIGNALRL_RANGE, turnSignalRL);
}
void CAN_mcu_status_GBSALL_set(uint16_t GBSALL){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_GBSALL_OFFSET, CAN_MCU_STATUS_GBSALL_RANGE, GBSALL);
}
void CAN_mcu_status_contactor_set(uint16_t contactor){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_CONTACTOR_OFFSET, CAN_MCU_STATUS_CONTACTOR_RANGE, contactor);
}
void CAN_mcu_status_chargePort_set(uint16_t chargePort){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_CHARGEPORT_OFFSET, CAN_MCU_STATUS_CHARGEPORT_RANGE, chargePort);
}
void CAN_mcu_status_brakeSwitchFront_set(uint16_t brakeSwitchFront){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHFRONT_OFFSET, CAN_MCU_STATUS_BRAKESWITCHFRONT_RANGE, brakeSwitchFront);
}
void CAN_mcu_status_brakeSwitchRear_set(uint16_t brakeSwitchRear){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHREAR_OFFSET, CAN_MCU_STATUS_BRAKESWITCHREAR_RANGE, brakeSwitchRear);
}
void CAN_mcu_status_throttleVal_set(uint16_t throttleVal){
	set_bits(CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEVAL_OFFSET, CAN_MCU_STATUS_THROTTLEVAL_RANGE, throttleVal);
}

void CAN_mcu_status_send(void){
	CAN_write(CAN_mcu_status);
}

static CAN_payload_S CAN_mcu_command_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_mcu_command={
	.canID = CAN_mcu_command_ID,
	.canXID = 0,
	.payload = &CAN_mcu_command_payload,
	.canMessageStatus = 0
};

void CAN_mcu_command_doSomthingElse_set(uint16_t doSomthingElse){
	set_bits(CAN_mcu_command.payload, CAN_MCU_COMMAND_DOSOMTHINGELSE_OFFSET, CAN_MCU_COMMAND_DOSOMTHINGELSE_RANGE, doSomthingElse);
}

void CAN_mcu_command_send(void){
	CAN_write(CAN_mcu_command);
}

static CAN_payload_S CAN_mcu_motorControllerRequest_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_mcu_motorControllerRequest={
	.canID = CAN_mcu_motorControllerRequest_ID,
	.canXID = 0,
	.payload = &CAN_mcu_motorControllerRequest_payload,
	.canMessageStatus = 0
};

void CAN_mcu_motorControllerRequest_requestType_set(uint16_t requestType){
	set_bits(CAN_mcu_motorControllerRequest.payload, CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_OFFSET, CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_RANGE, requestType);
}

void CAN_mcu_motorControllerRequest_send(void){
	CAN_write(CAN_mcu_motorControllerRequest);
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
	CAN_configureMailbox(&CAN_dash_status);
	CAN_configureMailbox(&CAN_dash_command);
	CAN_configureMailbox(&CAN_dash_data1);
	CAN_configureMailbox(&CAN_dash_data2);
	CAN_configureMailbox(&CAN_bms_status);
	CAN_configureMailbox(&CAN_motorcontroller_motorStatus);
	CAN_configureMailbox(&CAN_motorcontroller_response);
}
