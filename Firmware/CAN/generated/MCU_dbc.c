#include "MCU_dbc.h"

/*Private struct for keeping TX message data within message struct*/
typedef struct {
	CAN_message_S txM;
	CAN_payload_S txP;
} CAN_message_TX_S;


/**********************************************************
 * DASH NODE MESSAGES
 */
static CAN_message_TX_S CAN_DASH_status={
	.txM = 
	{
		.canID = 1793,
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
	CAN_DASH_status_S signals;
} CAN_DASH_status_U;

static CAN_DASH_status_U * CAN_DASH_status_P;

uint16_t CAN_DASH_status_state_get(void){
	return CAN_DASH_status_P->signals.state;
}
uint16_t CAN_DASH_status_killButton_get(void){
	return CAN_DASH_status_P->signals.killButton;
}
uint16_t CAN_DASH_status_ignButton_get(void){
	return CAN_DASH_status_P->signals.ignButton;
}
uint16_t CAN_DASH_status_modeButton_get(void){
	return CAN_DASH_status_P->signals.modeButton;
}
uint16_t CAN_DASH_status_selectButton_get(void){
	return CAN_DASH_status_P->signals.selectButton;
}
uint16_t CAN_DASH_status_driveMode_get(void){
	return CAN_DASH_status_P->signals.driveMode;
}

static CAN_message_TX_S CAN_DASH_command={
	.txM = 
	{
		.canID = 1794,
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
	CAN_DASH_command_S signals;
} CAN_DASH_command_U;

static CAN_DASH_command_U * CAN_DASH_command_P;

uint16_t CAN_DASH_command_ignitionRequest_get(void){
	return CAN_DASH_command_P->signals.ignitionRequest;
}
uint16_t CAN_DASH_command_killRequest_get(void){
	return CAN_DASH_command_P->signals.killRequest;
}
uint16_t CAN_DASH_command_batteryEjectRequest_get(void){
	return CAN_DASH_command_P->signals.batteryEjectRequest;
}
uint16_t CAN_DASH_command_lightsRequest_get(void){
	return CAN_DASH_command_P->signals.lightsRequest;
}
uint16_t CAN_DASH_command_hornRequest_get(void){
	return CAN_DASH_command_P->signals.hornRequest;
}

static CAN_message_TX_S CAN_DASH_data1={
	.txM = 
	{
		.canID = 1795,
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
	CAN_DASH_data1_S signals;
} CAN_DASH_data1_U;

static CAN_DASH_data1_U * CAN_DASH_data1_P;

uint16_t CAN_DASH_data1_speed_get(void){
	return CAN_DASH_data1_P->signals.speed;
}
uint16_t CAN_DASH_data1_odometer_get(void){
	return CAN_DASH_data1_P->signals.odometer;
}
uint16_t CAN_DASH_data1_tripA_get(void){
	return CAN_DASH_data1_P->signals.tripA;
}
uint16_t CAN_DASH_data1_tripB_get(void){
	return CAN_DASH_data1_P->signals.tripB;
}

static CAN_message_TX_S CAN_DASH_data2={
	.txM = 
	{
		.canID = 1796,
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
	CAN_DASH_data2_S signals;
} CAN_DASH_data2_U;

static CAN_DASH_data2_U * CAN_DASH_data2_P;

uint16_t CAN_DASH_data2_runningTime_get(void){
	return CAN_DASH_data2_P->signals.runningTime;
}
uint16_t CAN_DASH_data2_odometer_get(void){
	return CAN_DASH_data2_P->signals.odometer;
}
uint16_t CAN_DASH_data2_tripA_get(void){
	return CAN_DASH_data2_P->signals.tripA;
}
uint16_t CAN_DASH_data2_tripB_get(void){
	return CAN_DASH_data2_P->signals.tripB;
}




/**********************************************************
 * MCU NODE MESSAGES
 */
static CAN_message_TX_S CAN_MCU_status={
	.txM = 
	{
		.canID = 1809,
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
	CAN_MCU_status_S signals;
} CAN_MCU_status_U;

static CAN_MCU_status_U * CAN_MCU_status_P;

void CAN_MCU_status_state_set(uint16_t state){
	CAN_MCU_status_P->signals.state = state;
}
void CAN_MCU_status_throttleMode_set(uint16_t throttleMode){
	CAN_MCU_status_P->signals.throttleMode = throttleMode;
}
void CAN_MCU_status_highBeam_set(uint16_t highBeam){
	CAN_MCU_status_P->signals.highBeam = highBeam;
}
void CAN_MCU_status_brake_set(uint16_t brake){
	CAN_MCU_status_P->signals.brake = brake;
}
void CAN_MCU_status_horn_set(uint16_t horn){
	CAN_MCU_status_P->signals.horn = horn;
}
void CAN_MCU_status_GBSALL_set(uint16_t GBSALL){
	CAN_MCU_status_P->signals.GBSALL = GBSALL;
}
void CAN_MCU_status_contactor_set(uint16_t contactor){
	CAN_MCU_status_P->signals.contactor = contactor;
}
void CAN_MCU_status_chargePort_set(uint16_t chargePort){
	CAN_MCU_status_P->signals.chargePort = chargePort;
}
void CAN_MCU_status_throttleVal_set(uint16_t throttleVal){
	CAN_MCU_status_P->signals.throttleVal = throttleVal;
}

void CAN_MCU_status_send(void){
	CAN_write(CAN_MCU_status.txM);
}

static CAN_message_TX_S CAN_MCU_command={
	.txM = 
	{
		.canID = 1810,
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
	CAN_MCU_command_S signals;
} CAN_MCU_command_U;

static CAN_MCU_command_U * CAN_MCU_command_P;

void CAN_MCU_command_doSomthingElse_set(uint16_t doSomthingElse){
	CAN_MCU_command_P->signals.doSomthingElse = doSomthingElse;
}

void CAN_MCU_command_send(void){
	CAN_write(CAN_MCU_command.txM);
}

static CAN_message_TX_S CAN_MCU_motorStatus={
	.txM = 
	{
		.canID = 1811,
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
	CAN_MCU_motorStatus_S signals;
} CAN_MCU_motorStatus_U;

static CAN_MCU_motorStatus_U * CAN_MCU_motorStatus_P;

void CAN_MCU_motorStatus_motorSpeed_set(uint16_t motorSpeed){
	CAN_MCU_motorStatus_P->signals.motorSpeed = motorSpeed;
}
void CAN_MCU_motorStatus_motorCurrent_set(uint16_t motorCurrent){
	CAN_MCU_motorStatus_P->signals.motorCurrent = motorCurrent;
}
void CAN_MCU_motorStatus_IphaseA_set(uint16_t IphaseA){
	CAN_MCU_motorStatus_P->signals.IphaseA = IphaseA;
}
void CAN_MCU_motorStatus_IphaseB_set(uint16_t IphaseB){
	CAN_MCU_motorStatus_P->signals.IphaseB = IphaseB;
}
void CAN_MCU_motorStatus_IphaseC_set(uint16_t IphaseC){
	CAN_MCU_motorStatus_P->signals.IphaseC = IphaseC;
}
void CAN_MCU_motorStatus_VphaseA_set(uint16_t VphaseA){
	CAN_MCU_motorStatus_P->signals.VphaseA = VphaseA;
}
void CAN_MCU_motorStatus_VphaseB_set(uint16_t VphaseB){
	CAN_MCU_motorStatus_P->signals.VphaseB = VphaseB;
}
void CAN_MCU_motorStatus_VphaseC_set(uint16_t VphaseC){
	CAN_MCU_motorStatus_P->signals.VphaseC = VphaseC;
}

void CAN_MCU_motorStatus_send(void){
	CAN_write(CAN_MCU_motorStatus.txM);
}

static CAN_message_TX_S CAN_MCU_motorControllerRequest={
	.txM = 
	{
		.canID = 1792,
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
	CAN_MCU_motorControllerRequest_S signals;
} CAN_MCU_motorControllerRequest_U;

static CAN_MCU_motorControllerRequest_U * CAN_MCU_motorControllerRequest_P;

void CAN_MCU_motorControllerRequest_requestType_set(uint16_t requestType){
	CAN_MCU_motorControllerRequest_P->signals.requestType = requestType;
}

void CAN_MCU_motorControllerRequest_send(void){
	CAN_write(CAN_MCU_motorControllerRequest.txM);
}




/**********************************************************
 * BMS NODE MESSAGES
 */
static CAN_message_TX_S CAN_BMS_status={
	.txM = 
	{
		.canID = 1825,
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
	CAN_BMS_status_S signals;
} CAN_BMS_status_U;

static CAN_BMS_status_U * CAN_BMS_status_P;

uint16_t CAN_BMS_status_state_get(void){
	return CAN_BMS_status_P->signals.state;
}
uint16_t CAN_BMS_status_SOC_get(void){
	return CAN_BMS_status_P->signals.SOC;
}
uint16_t CAN_BMS_status_packVoltage_get(void){
	return CAN_BMS_status_P->signals.packVoltage;
}
uint16_t CAN_BMS_status_packCurrent_get(void){
	return CAN_BMS_status_P->signals.packCurrent;
}
uint16_t CAN_BMS_status_minTemp_get(void){
	return CAN_BMS_status_P->signals.minTemp;
}
uint16_t CAN_BMS_status_maxTemp_get(void){
	return CAN_BMS_status_P->signals.maxTemp;
}

static CAN_message_TX_S CAN_BMS_cellVoltages={
	.txM = 
	{
		.canID = 1826,
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
	CAN_BMS_cellVoltages_S signals;
} CAN_BMS_cellVoltages_U;

static CAN_BMS_cellVoltages_U * CAN_BMS_cellVoltages_P;

uint16_t CAN_BMS_cellVoltages_MultiPlex_get(void){
	return CAN_BMS_cellVoltages_P->signals.MultiPlex;
}
uint16_t CAN_BMS_cellVoltages_cell1_get(void){
	return CAN_BMS_cellVoltages_P->signals.cell1;
}
uint16_t CAN_BMS_cellVoltages_cell2_get(void){
	return CAN_BMS_cellVoltages_P->signals.cell2;
}
uint16_t CAN_BMS_cellVoltages_cell3_get(void){
	return CAN_BMS_cellVoltages_P->signals.cell3;
}
uint16_t CAN_BMS_cellVoltages_cell4_get(void){
	return CAN_BMS_cellVoltages_P->signals.cell4;
}
uint16_t CAN_BMS_cellVoltages_cell5_get(void){
	return CAN_BMS_cellVoltages_P->signals.cell5;
}




/**********************************************************
 * MotorController NODE MESSAGES
 */
static CAN_message_TX_S CAN_MotorController_response={
	.txM = 
	{
		.canID = 1792,
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
	CAN_MotorController_response_S signals;
} CAN_MotorController_response_U;

static CAN_MotorController_response_U * CAN_MotorController_response_P;

uint16_t CAN_MotorController_response_byte1_get(void){
	return CAN_MotorController_response_P->signals.byte1;
}
uint16_t CAN_MotorController_response_byte2_get(void){
	return CAN_MotorController_response_P->signals.byte2;
}
uint16_t CAN_MotorController_response_byte3_get(void){
	return CAN_MotorController_response_P->signals.byte3;
}
uint16_t CAN_MotorController_response_byte4_get(void){
	return CAN_MotorController_response_P->signals.byte4;
}
uint16_t CAN_MotorController_response_byte5_get(void){
	return CAN_MotorController_response_P->signals.byte5;
}
uint16_t CAN_MotorController_response_byte6_get(void){
	return CAN_MotorController_response_P->signals.byte6;
}
uint16_t CAN_MotorController_response_byte7_get(void){
	return CAN_MotorController_response_P->signals.byte7;
}
uint16_t CAN_MotorController_response_byte8_get(void){
	return CAN_MotorController_response_P->signals.byte8;
}

void CAN_DBC_init() {
	CAN_configureMailbox(&CAN_DASH_status.txM);
	CAN_DASH_status_P = (CAN_DASH_status_U*) CAN_DASH_status.txM.payload;

	CAN_configureMailbox(&CAN_DASH_command.txM);
	CAN_DASH_command_P = (CAN_DASH_command_U*) CAN_DASH_command.txM.payload;

	CAN_configureMailbox(&CAN_DASH_data1.txM);
	CAN_DASH_data1_P = (CAN_DASH_data1_U*) CAN_DASH_data1.txM.payload;

	CAN_configureMailbox(&CAN_DASH_data2.txM);
	CAN_DASH_data2_P = (CAN_DASH_data2_U*) CAN_DASH_data2.txM.payload;

	CAN_MCU_status.txM.payload = &CAN_MCU_status.txP;
	CAN_MCU_status_P = (CAN_MCU_status_U*) & CAN_MCU_status.txP;

	CAN_MCU_command.txM.payload = &CAN_MCU_command.txP;
	CAN_MCU_command_P = (CAN_MCU_command_U*) & CAN_MCU_command.txP;

	CAN_MCU_motorStatus.txM.payload = &CAN_MCU_motorStatus.txP;
	CAN_MCU_motorStatus_P = (CAN_MCU_motorStatus_U*) & CAN_MCU_motorStatus.txP;

	CAN_MCU_motorControllerRequest.txM.payload = &CAN_MCU_motorControllerRequest.txP;
	CAN_MCU_motorControllerRequest_P = (CAN_MCU_motorControllerRequest_U*) & CAN_MCU_motorControllerRequest.txP;

	CAN_configureMailbox(&CAN_BMS_status.txM);
	CAN_BMS_status_P = (CAN_BMS_status_U*) CAN_BMS_status.txM.payload;

	CAN_configureMailbox(&CAN_BMS_cellVoltages.txM);
	CAN_BMS_cellVoltages_P = (CAN_BMS_cellVoltages_U*) CAN_BMS_cellVoltages.txM.payload;

	CAN_configureMailbox(&CAN_MotorController_response.txM);
	CAN_MotorController_response_P = (CAN_MotorController_response_U*) CAN_MotorController_response.txM.payload;

}
