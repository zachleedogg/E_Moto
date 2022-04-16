#include "motorcontroller_dbc.h"

/*Private struct for keeping TX message data within message struct*/
typedef struct {
	CAN_message_S txM;
	CAN_payload_S txP;
} CAN_message_TX_S;


/**********************************************************
 * dash NODE MESSAGES
 */
static CAN_message_TX_S CAN_dash_data1={
	.txM = 
	{
		.canID = CAN_dash_data1_ID,
		.canXID = 1,
		.payload = 0
	},
	.txP =
	{
		.word0 = 0,
		.word1 = 0,
		.word2 = 0,
		.word3 = 0
	}
};

typedef union {
	CAN_payload_S packedMessage;
	CAN_dash_data1_S signals;
} CAN_dash_data1_U;

static CAN_dash_data1_U * CAN_dash_data1_P;

uint16_t CAN_dash_data1_speed_get(void){
	return CAN_dash_data1_P->signals.speed;
}
uint16_t CAN_dash_data1_odometer_get(void){
	return CAN_dash_data1_P->signals.odometer;
}
uint16_t CAN_dash_data1_tripA_get(void){
	return CAN_dash_data1_P->signals.tripA;
}
uint16_t CAN_dash_data1_tripB_get(void){
	return CAN_dash_data1_P->signals.tripB;
}

static CAN_message_TX_S CAN_dash_data2={
	.txM = 
	{
		.canID = CAN_dash_data2_ID,
		.canXID = 0,
		.payload = 0
	},
	.txP =
	{
		.word0 = 0,
		.word1 = 0,
		.word2 = 0,
		.word3 = 0
	}
};

typedef union {
	CAN_payload_S packedMessage;
	CAN_dash_data2_S signals;
} CAN_dash_data2_U;

static CAN_dash_data2_U * CAN_dash_data2_P;

uint16_t CAN_dash_data2_runningTime_get(void){
	return CAN_dash_data2_P->signals.runningTime;
}
uint16_t CAN_dash_data2_odometer_get(void){
	return CAN_dash_data2_P->signals.odometer;
}
uint16_t CAN_dash_data2_tripA_get(void){
	return CAN_dash_data2_P->signals.tripA;
}
uint16_t CAN_dash_data2_tripB_get(void){
	return CAN_dash_data2_P->signals.tripB;
}




/**********************************************************
 * mcu NODE MESSAGES
 */
static CAN_message_TX_S CAN_mcu_command={
	.txM = 
	{
		.canID = CAN_mcu_command_ID,
		.canXID = 0,
		.payload = 0
	},
	.txP =
	{
		.word0 = 0,
		.word1 = 0,
		.word2 = 0,
		.word3 = 0
	}
};

typedef union {
	CAN_payload_S packedMessage;
	CAN_mcu_command_S signals;
} CAN_mcu_command_U;

static CAN_mcu_command_U * CAN_mcu_command_P;

uint16_t CAN_mcu_command_doSomthingElse_get(void){
	return CAN_mcu_command_P->signals.doSomthingElse;
}

static CAN_message_TX_S CAN_mcu_motorControllerRequest={
	.txM = 
	{
		.canID = CAN_mcu_motorControllerRequest_ID,
		.canXID = 0,
		.payload = 0
	},
	.txP =
	{
		.word0 = 0,
		.word1 = 0,
		.word2 = 0,
		.word3 = 0
	}
};

typedef union {
	CAN_payload_S packedMessage;
	CAN_mcu_motorControllerRequest_S signals;
} CAN_mcu_motorControllerRequest_U;

static CAN_mcu_motorControllerRequest_U * CAN_mcu_motorControllerRequest_P;

uint16_t CAN_mcu_motorControllerRequest_requestType_get(void){
	return CAN_mcu_motorControllerRequest_P->signals.requestType;
}




/**********************************************************
 * bms NODE MESSAGES
 */



/**********************************************************
 * motorcontroller NODE MESSAGES
 */
static CAN_message_TX_S CAN_motorcontroller_motorStatus={
	.txM = 
	{
		.canID = CAN_motorcontroller_motorStatus_ID,
		.canXID = 0,
		.payload = 0
	},
	.txP =
	{
		.word0 = 0,
		.word1 = 0,
		.word2 = 0,
		.word3 = 0
	}
};

typedef union {
	CAN_payload_S packedMessage;
	CAN_motorcontroller_motorStatus_S signals;
} CAN_motorcontroller_motorStatus_U;

static CAN_motorcontroller_motorStatus_U * CAN_motorcontroller_motorStatus_P;

void CAN_motorcontroller_motorStatus_motorSpeed_set(uint16_t motorSpeed){
	CAN_motorcontroller_motorStatus_P->signals.motorSpeed = motorSpeed;
}
void CAN_motorcontroller_motorStatus_motorCurrent_set(uint16_t motorCurrent){
	CAN_motorcontroller_motorStatus_P->signals.motorCurrent = motorCurrent;
}
void CAN_motorcontroller_motorStatus_IphaseA_set(uint16_t IphaseA){
	CAN_motorcontroller_motorStatus_P->signals.IphaseA = IphaseA;
}
void CAN_motorcontroller_motorStatus_IphaseB_set(uint16_t IphaseB){
	CAN_motorcontroller_motorStatus_P->signals.IphaseB = IphaseB;
}
void CAN_motorcontroller_motorStatus_IphaseC_set(uint16_t IphaseC){
	CAN_motorcontroller_motorStatus_P->signals.IphaseC = IphaseC;
}
void CAN_motorcontroller_motorStatus_VphaseA_set(uint16_t VphaseA){
	CAN_motorcontroller_motorStatus_P->signals.VphaseA = VphaseA;
}
void CAN_motorcontroller_motorStatus_VphaseB_set(uint16_t VphaseB){
	CAN_motorcontroller_motorStatus_P->signals.VphaseB = VphaseB;
}
void CAN_motorcontroller_motorStatus_VphaseC_set(uint16_t VphaseC){
	CAN_motorcontroller_motorStatus_P->signals.VphaseC = VphaseC;
}

void CAN_motorcontroller_motorStatus_send(void){
	CAN_write(CAN_motorcontroller_motorStatus.txM);
}

static CAN_message_TX_S CAN_motorcontroller_response={
	.txM = 
	{
		.canID = CAN_motorcontroller_response_ID,
		.canXID = 0,
		.payload = 0
	},
	.txP =
	{
		.word0 = 0,
		.word1 = 0,
		.word2 = 0,
		.word3 = 0
	}
};

typedef union {
	CAN_payload_S packedMessage;
	CAN_motorcontroller_response_S signals;
} CAN_motorcontroller_response_U;

static CAN_motorcontroller_response_U * CAN_motorcontroller_response_P;

void CAN_motorcontroller_response_byte1_set(uint16_t byte1){
	CAN_motorcontroller_response_P->signals.byte1 = byte1;
}
void CAN_motorcontroller_response_byte2_set(uint16_t byte2){
	CAN_motorcontroller_response_P->signals.byte2 = byte2;
}
void CAN_motorcontroller_response_byte3_set(uint16_t byte3){
	CAN_motorcontroller_response_P->signals.byte3 = byte3;
}
void CAN_motorcontroller_response_byte4_set(uint16_t byte4){
	CAN_motorcontroller_response_P->signals.byte4 = byte4;
}
void CAN_motorcontroller_response_byte5_set(uint16_t byte5){
	CAN_motorcontroller_response_P->signals.byte5 = byte5;
}
void CAN_motorcontroller_response_byte6_set(uint16_t byte6){
	CAN_motorcontroller_response_P->signals.byte6 = byte6;
}
void CAN_motorcontroller_response_byte7_set(uint16_t byte7){
	CAN_motorcontroller_response_P->signals.byte7 = byte7;
}
void CAN_motorcontroller_response_byte8_set(uint16_t byte8){
	CAN_motorcontroller_response_P->signals.byte8 = byte8;
}

void CAN_motorcontroller_response_send(void){
	CAN_write(CAN_motorcontroller_response.txM);
}




/**********************************************************
 * charger NODE MESSAGES
 */
void CAN_DBC_init() {
	CAN_configureMailbox(&CAN_dash_data1.txM);
	CAN_dash_data1_P = (CAN_dash_data1_U*) CAN_dash_data1.txM.payload;

	CAN_configureMailbox(&CAN_dash_data2.txM);
	CAN_dash_data2_P = (CAN_dash_data2_U*) CAN_dash_data2.txM.payload;

	CAN_configureMailbox(&CAN_mcu_command.txM);
	CAN_mcu_command_P = (CAN_mcu_command_U*) CAN_mcu_command.txM.payload;

	CAN_configureMailbox(&CAN_mcu_motorControllerRequest.txM);
	CAN_mcu_motorControllerRequest_P = (CAN_mcu_motorControllerRequest_U*) CAN_mcu_motorControllerRequest.txM.payload;

	CAN_motorcontroller_motorStatus.txM.payload = &CAN_motorcontroller_motorStatus.txP;
	CAN_motorcontroller_motorStatus_P = (CAN_motorcontroller_motorStatus_U*) &CAN_motorcontroller_motorStatus.txP;

	CAN_motorcontroller_response.txM.payload = &CAN_motorcontroller_response.txP;
	CAN_motorcontroller_response_P = (CAN_motorcontroller_response_U*) &CAN_motorcontroller_response.txP;

}
