#include "MotorController_dbc.h"

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
