#include "motorcontroller_dbc.h"




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



/**********************************************************
 * motorcontroller NODE MESSAGES
 */
static CAN_payload_S CAN_motorcontroller_motorStatus_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_motorcontroller_motorStatus={
	.canID = CAN_motorcontroller_motorStatus_ID,
	.canXID = 0,
	.payload = &CAN_motorcontroller_motorStatus_payload,
	.canMessageStatus = 0
};

void CAN_motorcontroller_motorStatus_motorSpeed_set(uint16_t motorSpeed){
	set_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORSPEED_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORSPEED_RANGE, motorSpeed);
}
void CAN_motorcontroller_motorStatus_motorCurrent_set(uint16_t motorCurrent){
	set_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORCURRENT_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORCURRENT_RANGE, motorCurrent);
}
void CAN_motorcontroller_motorStatus_IphaseA_set(uint16_t IphaseA){
	set_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEA_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEA_RANGE, IphaseA);
}
void CAN_motorcontroller_motorStatus_IphaseB_set(uint16_t IphaseB){
	set_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEB_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEB_RANGE, IphaseB);
}
void CAN_motorcontroller_motorStatus_IphaseC_set(uint16_t IphaseC){
	set_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEC_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEC_RANGE, IphaseC);
}
void CAN_motorcontroller_motorStatus_VphaseA_set(uint16_t VphaseA){
	set_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEA_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEA_RANGE, VphaseA);
}
void CAN_motorcontroller_motorStatus_VphaseB_set(uint16_t VphaseB){
	set_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEB_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEB_RANGE, VphaseB);
}
void CAN_motorcontroller_motorStatus_VphaseC_set(uint16_t VphaseC){
	set_bits(CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEC_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEC_RANGE, VphaseC);
}

void CAN_motorcontroller_motorStatus_send(void){
	CAN_write(CAN_motorcontroller_motorStatus);
}

static CAN_payload_S CAN_motorcontroller_response_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_motorcontroller_response={
	.canID = CAN_motorcontroller_response_ID,
	.canXID = 0,
	.payload = &CAN_motorcontroller_response_payload,
	.canMessageStatus = 0
};

void CAN_motorcontroller_response_byte1_set(uint16_t byte1){
	set_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE1_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE1_RANGE, byte1);
}
void CAN_motorcontroller_response_byte2_set(uint16_t byte2){
	set_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE2_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE2_RANGE, byte2);
}
void CAN_motorcontroller_response_byte3_set(uint16_t byte3){
	set_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE3_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE3_RANGE, byte3);
}
void CAN_motorcontroller_response_byte4_set(uint16_t byte4){
	set_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE4_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE4_RANGE, byte4);
}
void CAN_motorcontroller_response_byte5_set(uint16_t byte5){
	set_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE5_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE5_RANGE, byte5);
}
void CAN_motorcontroller_response_byte6_set(uint16_t byte6){
	set_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE6_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE6_RANGE, byte6);
}
void CAN_motorcontroller_response_byte7_set(uint16_t byte7){
	set_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE7_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE7_RANGE, byte7);
}
void CAN_motorcontroller_response_byte8_set(uint16_t byte8){
	set_bits(CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE8_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE8_RANGE, byte8);
}

void CAN_motorcontroller_response_send(void){
	CAN_write(CAN_motorcontroller_response);
}




/**********************************************************
 * charger NODE MESSAGES
 */



/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init() {
	CAN_configureMailbox(&CAN_dash_data1);
	CAN_configureMailbox(&CAN_dash_data2);
	CAN_configureMailbox(&CAN_mcu_command);
	CAN_configureMailbox(&CAN_mcu_motorControllerRequest);
}
