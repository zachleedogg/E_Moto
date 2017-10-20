#include "DASH_dbc.h"

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

void CAN_DASH_status_state_set(uint16_t state){
	CAN_DASH_status_P->signals.state = state;
}
void CAN_DASH_status_killButton_set(uint16_t killButton){
	CAN_DASH_status_P->signals.killButton = killButton;
}
void CAN_DASH_status_ignButton_set(uint16_t ignButton){
	CAN_DASH_status_P->signals.ignButton = ignButton;
}
void CAN_DASH_status_modeButton_set(uint16_t modeButton){
	CAN_DASH_status_P->signals.modeButton = modeButton;
}
void CAN_DASH_status_selectButton_set(uint16_t selectButton){
	CAN_DASH_status_P->signals.selectButton = selectButton;
}
void CAN_DASH_status_driveMode_set(uint16_t driveMode){
	CAN_DASH_status_P->signals.driveMode = driveMode;
}

void CAN_DASH_status_send(void){
	CAN_write(CAN_DASH_status.txM);
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

void CAN_DASH_command_ignitionRequest_set(uint16_t ignitionRequest){
	CAN_DASH_command_P->signals.ignitionRequest = ignitionRequest;
}
void CAN_DASH_command_killRequest_set(uint16_t killRequest){
	CAN_DASH_command_P->signals.killRequest = killRequest;
}
void CAN_DASH_command_batteryEjectRequest_set(uint16_t batteryEjectRequest){
	CAN_DASH_command_P->signals.batteryEjectRequest = batteryEjectRequest;
}
void CAN_DASH_command_lightsRequest_set(uint16_t lightsRequest){
	CAN_DASH_command_P->signals.lightsRequest = lightsRequest;
}
void CAN_DASH_command_hornRequest_set(uint16_t hornRequest){
	CAN_DASH_command_P->signals.hornRequest = hornRequest;
}

void CAN_DASH_command_send(void){
	CAN_write(CAN_DASH_command.txM);
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

void CAN_DASH_data1_speed_set(uint16_t speed){
	CAN_DASH_data1_P->signals.speed = speed;
}
void CAN_DASH_data1_odometer_set(uint16_t odometer){
	CAN_DASH_data1_P->signals.odometer = odometer;
}
void CAN_DASH_data1_tripA_set(uint16_t tripA){
	CAN_DASH_data1_P->signals.tripA = tripA;
}
void CAN_DASH_data1_tripB_set(uint16_t tripB){
	CAN_DASH_data1_P->signals.tripB = tripB;
}

void CAN_DASH_data1_send(void){
	CAN_write(CAN_DASH_data1.txM);
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

void CAN_DASH_data2_runningTime_set(uint16_t runningTime){
	CAN_DASH_data2_P->signals.runningTime = runningTime;
}
void CAN_DASH_data2_odometer_set(uint16_t odometer){
	CAN_DASH_data2_P->signals.odometer = odometer;
}
void CAN_DASH_data2_tripA_set(uint16_t tripA){
	CAN_DASH_data2_P->signals.tripA = tripA;
}
void CAN_DASH_data2_tripB_set(uint16_t tripB){
	CAN_DASH_data2_P->signals.tripB = tripB;
}

void CAN_DASH_data2_send(void){
	CAN_write(CAN_DASH_data2.txM);
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

uint16_t CAN_MCU_status_state_get(void){
	return CAN_MCU_status_P->signals.state;
}
uint16_t CAN_MCU_status_throttleMode_get(void){
	return CAN_MCU_status_P->signals.throttleMode;
}
uint16_t CAN_MCU_status_highBeam_get(void){
	return CAN_MCU_status_P->signals.highBeam;
}
uint16_t CAN_MCU_status_brake_get(void){
	return CAN_MCU_status_P->signals.brake;
}
uint16_t CAN_MCU_status_horn_get(void){
	return CAN_MCU_status_P->signals.horn;
}
uint16_t CAN_MCU_status_GBSALL_get(void){
	return CAN_MCU_status_P->signals.GBSALL;
}
uint16_t CAN_MCU_status_contactor_get(void){
	return CAN_MCU_status_P->signals.contactor;
}
uint16_t CAN_MCU_status_chargePort_get(void){
	return CAN_MCU_status_P->signals.chargePort;
}
uint16_t CAN_MCU_status_throttleVal_get(void){
	return CAN_MCU_status_P->signals.throttleVal;
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

uint16_t CAN_MCU_command_doSomthingElse_get(void){
	return CAN_MCU_command_P->signals.doSomthingElse;
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

uint16_t CAN_MCU_motorStatus_motorSpeed_get(void){
	return CAN_MCU_motorStatus_P->signals.motorSpeed;
}
uint16_t CAN_MCU_motorStatus_motorCurrent_get(void){
	return CAN_MCU_motorStatus_P->signals.motorCurrent;
}
uint16_t CAN_MCU_motorStatus_IphaseA_get(void){
	return CAN_MCU_motorStatus_P->signals.IphaseA;
}
uint16_t CAN_MCU_motorStatus_IphaseB_get(void){
	return CAN_MCU_motorStatus_P->signals.IphaseB;
}
uint16_t CAN_MCU_motorStatus_IphaseC_get(void){
	return CAN_MCU_motorStatus_P->signals.IphaseC;
}
uint16_t CAN_MCU_motorStatus_VphaseA_get(void){
	return CAN_MCU_motorStatus_P->signals.VphaseA;
}
uint16_t CAN_MCU_motorStatus_VphaseB_get(void){
	return CAN_MCU_motorStatus_P->signals.VphaseB;
}
uint16_t CAN_MCU_motorStatus_VphaseC_get(void){
	return CAN_MCU_motorStatus_P->signals.VphaseC;
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

uint16_t CAN_MCU_motorControllerRequest_requestType_get(void){
	return CAN_MCU_motorControllerRequest_P->signals.requestType;
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
	CAN_DASH_status.txM.payload = &CAN_DASH_status.txP;
	CAN_DASH_status_P = (CAN_DASH_status_U*) & CAN_DASH_status.txP;

	CAN_DASH_command.txM.payload = &CAN_DASH_command.txP;
	CAN_DASH_command_P = (CAN_DASH_command_U*) & CAN_DASH_command.txP;

	CAN_DASH_data1.txM.payload = &CAN_DASH_data1.txP;
	CAN_DASH_data1_P = (CAN_DASH_data1_U*) & CAN_DASH_data1.txP;

	CAN_DASH_data2.txM.payload = &CAN_DASH_data2.txP;
	CAN_DASH_data2_P = (CAN_DASH_data2_U*) & CAN_DASH_data2.txP;

	CAN_configureMailbox(&CAN_MCU_status.txM);
	CAN_MCU_status_P = (CAN_MCU_status_U*) CAN_MCU_status.txM.payload;

	CAN_configureMailbox(&CAN_MCU_command.txM);
	CAN_MCU_command_P = (CAN_MCU_command_U*) CAN_MCU_command.txM.payload;

	CAN_configureMailbox(&CAN_MCU_motorStatus.txM);
	CAN_MCU_motorStatus_P = (CAN_MCU_motorStatus_U*) CAN_MCU_motorStatus.txM.payload;

	CAN_configureMailbox(&CAN_MCU_motorControllerRequest.txM);
	CAN_MCU_motorControllerRequest_P = (CAN_MCU_motorControllerRequest_U*) CAN_MCU_motorControllerRequest.txM.payload;

	CAN_configureMailbox(&CAN_BMS_status.txM);
	CAN_BMS_status_P = (CAN_BMS_status_U*) CAN_BMS_status.txM.payload;

	CAN_configureMailbox(&CAN_BMS_cellVoltages.txM);
	CAN_BMS_cellVoltages_P = (CAN_BMS_cellVoltages_U*) CAN_BMS_cellVoltages.txM.payload;

	CAN_configureMailbox(&CAN_MotorController_response.txM);
	CAN_MotorController_response_P = (CAN_MotorController_response_U*) CAN_MotorController_response.txM.payload;

}
