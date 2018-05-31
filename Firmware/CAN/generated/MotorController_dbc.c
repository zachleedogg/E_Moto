#include "motorcontroller_dbc.h"

/*Private struct for keeping TX message data within message struct*/
typedef struct {
	CAN_message_S txM;
	CAN_payload_S txP;
} CAN_message_TX_S;


/**********************************************************
 * dash NODE MESSAGES
 */
static CAN_message_TX_S CAN_dash_status={
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
	CAN_dash_status_S signals;
} CAN_dash_status_U;

static CAN_dash_status_U * CAN_dash_status_P;

uint16_t CAN_dash_status_heartBeat_get(void){
	return CAN_dash_status_P->signals.heartBeat;
}
uint16_t CAN_dash_status_state_get(void){
	return CAN_dash_status_P->signals.state;
}
uint16_t CAN_dash_status_killButton_get(void){
	return CAN_dash_status_P->signals.killButton;
}
uint16_t CAN_dash_status_ignButton_get(void){
	return CAN_dash_status_P->signals.ignButton;
}
uint16_t CAN_dash_status_modeButton_get(void){
	return CAN_dash_status_P->signals.modeButton;
}
uint16_t CAN_dash_status_selectButton_get(void){
	return CAN_dash_status_P->signals.selectButton;
}
uint16_t CAN_dash_status_driveMode_get(void){
	return CAN_dash_status_P->signals.driveMode;
}

static CAN_message_TX_S CAN_dash_command={
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
	CAN_dash_command_S signals;
} CAN_dash_command_U;

static CAN_dash_command_U * CAN_dash_command_P;

uint16_t CAN_dash_command_ignitionRequest_get(void){
	return CAN_dash_command_P->signals.ignitionRequest;
}
uint16_t CAN_dash_command_killRequest_get(void){
	return CAN_dash_command_P->signals.killRequest;
}
uint16_t CAN_dash_command_batteryEjectRequest_get(void){
	return CAN_dash_command_P->signals.batteryEjectRequest;
}
uint16_t CAN_dash_command_lightsRequest_get(void){
	return CAN_dash_command_P->signals.lightsRequest;
}
uint16_t CAN_dash_command_hornRequest_get(void){
	return CAN_dash_command_P->signals.hornRequest;
}

static CAN_message_TX_S CAN_dash_data1={
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
static CAN_message_TX_S CAN_mcu_status={
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
	CAN_mcu_status_S signals;
} CAN_mcu_status_U;

static CAN_mcu_status_U * CAN_mcu_status_P;

uint16_t CAN_mcu_status_state_get(void){
	return CAN_mcu_status_P->signals.state;
}
uint16_t CAN_mcu_status_throttleMode_get(void){
	return CAN_mcu_status_P->signals.throttleMode;
}
uint16_t CAN_mcu_status_highBeam_get(void){
	return CAN_mcu_status_P->signals.highBeam;
}
uint16_t CAN_mcu_status_brake_get(void){
	return CAN_mcu_status_P->signals.brake;
}
uint16_t CAN_mcu_status_horn_get(void){
	return CAN_mcu_status_P->signals.horn;
}
uint16_t CAN_mcu_status_GBSALL_get(void){
	return CAN_mcu_status_P->signals.GBSALL;
}
uint16_t CAN_mcu_status_contactor_get(void){
	return CAN_mcu_status_P->signals.contactor;
}
uint16_t CAN_mcu_status_chargePort_get(void){
	return CAN_mcu_status_P->signals.chargePort;
}
uint16_t CAN_mcu_status_throttleVal_get(void){
	return CAN_mcu_status_P->signals.throttleVal;
}

static CAN_message_TX_S CAN_mcu_command={
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
	CAN_mcu_command_S signals;
} CAN_mcu_command_U;

static CAN_mcu_command_U * CAN_mcu_command_P;

uint16_t CAN_mcu_command_doSomthingElse_get(void){
	return CAN_mcu_command_P->signals.doSomthingElse;
}

static CAN_message_TX_S CAN_mcu_motorStatus={
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
	CAN_mcu_motorStatus_S signals;
} CAN_mcu_motorStatus_U;

static CAN_mcu_motorStatus_U * CAN_mcu_motorStatus_P;

uint16_t CAN_mcu_motorStatus_motorSpeed_get(void){
	return CAN_mcu_motorStatus_P->signals.motorSpeed;
}
uint16_t CAN_mcu_motorStatus_motorCurrent_get(void){
	return CAN_mcu_motorStatus_P->signals.motorCurrent;
}
uint16_t CAN_mcu_motorStatus_IphaseA_get(void){
	return CAN_mcu_motorStatus_P->signals.IphaseA;
}
uint16_t CAN_mcu_motorStatus_IphaseB_get(void){
	return CAN_mcu_motorStatus_P->signals.IphaseB;
}
uint16_t CAN_mcu_motorStatus_IphaseC_get(void){
	return CAN_mcu_motorStatus_P->signals.IphaseC;
}
uint16_t CAN_mcu_motorStatus_VphaseA_get(void){
	return CAN_mcu_motorStatus_P->signals.VphaseA;
}
uint16_t CAN_mcu_motorStatus_VphaseB_get(void){
	return CAN_mcu_motorStatus_P->signals.VphaseB;
}
uint16_t CAN_mcu_motorStatus_VphaseC_get(void){
	return CAN_mcu_motorStatus_P->signals.VphaseC;
}

static CAN_message_TX_S CAN_mcu_motorControllerRequest={
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
	CAN_mcu_motorControllerRequest_S signals;
} CAN_mcu_motorControllerRequest_U;

static CAN_mcu_motorControllerRequest_U * CAN_mcu_motorControllerRequest_P;

uint16_t CAN_mcu_motorControllerRequest_requestType_get(void){
	return CAN_mcu_motorControllerRequest_P->signals.requestType;
}




/**********************************************************
 * bms NODE MESSAGES
 */
static CAN_message_TX_S CAN_bms_status={
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
	CAN_bms_status_S signals;
} CAN_bms_status_U;

static CAN_bms_status_U * CAN_bms_status_P;

uint16_t CAN_bms_status_state_get(void){
	return CAN_bms_status_P->signals.state;
}
uint16_t CAN_bms_status_SOC_get(void){
	return CAN_bms_status_P->signals.SOC;
}
uint16_t CAN_bms_status_packVoltage_get(void){
	return CAN_bms_status_P->signals.packVoltage;
}
uint16_t CAN_bms_status_packCurrent_get(void){
	return CAN_bms_status_P->signals.packCurrent;
}
uint16_t CAN_bms_status_minTemp_get(void){
	return CAN_bms_status_P->signals.minTemp;
}
uint16_t CAN_bms_status_maxTemp_get(void){
	return CAN_bms_status_P->signals.maxTemp;
}

static CAN_message_TX_S CAN_bms_cellVoltages={
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
	CAN_bms_cellVoltages_S signals;
} CAN_bms_cellVoltages_U;

static CAN_bms_cellVoltages_U * CAN_bms_cellVoltages_P;

uint16_t CAN_bms_cellVoltages_MultiPlex_get(void){
	return CAN_bms_cellVoltages_P->signals.MultiPlex;
}
uint16_t CAN_bms_cellVoltages_cell1_get(void){
	return CAN_bms_cellVoltages_P->signals.cell1;
}
uint16_t CAN_bms_cellVoltages_cell2_get(void){
	return CAN_bms_cellVoltages_P->signals.cell2;
}
uint16_t CAN_bms_cellVoltages_cell3_get(void){
	return CAN_bms_cellVoltages_P->signals.cell3;
}
uint16_t CAN_bms_cellVoltages_cell4_get(void){
	return CAN_bms_cellVoltages_P->signals.cell4;
}
uint16_t CAN_bms_cellVoltages_cell5_get(void){
	return CAN_bms_cellVoltages_P->signals.cell5;
}




/**********************************************************
 * motorcontroller NODE MESSAGES
 */
static CAN_message_TX_S CAN_motorcontroller_response={
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

void CAN_DBC_init() {
	CAN_configureMailbox(&CAN_dash_status.txM);
	CAN_dash_status_P = (CAN_dash_status_U*) CAN_dash_status.txM.payload;

	CAN_configureMailbox(&CAN_dash_command.txM);
	CAN_dash_command_P = (CAN_dash_command_U*) CAN_dash_command.txM.payload;

	CAN_configureMailbox(&CAN_dash_data1.txM);
	CAN_dash_data1_P = (CAN_dash_data1_U*) CAN_dash_data1.txM.payload;

	CAN_configureMailbox(&CAN_dash_data2.txM);
	CAN_dash_data2_P = (CAN_dash_data2_U*) CAN_dash_data2.txM.payload;

	CAN_configureMailbox(&CAN_mcu_status.txM);
	CAN_mcu_status_P = (CAN_mcu_status_U*) CAN_mcu_status.txM.payload;

	CAN_configureMailbox(&CAN_mcu_command.txM);
	CAN_mcu_command_P = (CAN_mcu_command_U*) CAN_mcu_command.txM.payload;

	CAN_configureMailbox(&CAN_mcu_motorStatus.txM);
	CAN_mcu_motorStatus_P = (CAN_mcu_motorStatus_U*) CAN_mcu_motorStatus.txM.payload;

	CAN_configureMailbox(&CAN_mcu_motorControllerRequest.txM);
	CAN_mcu_motorControllerRequest_P = (CAN_mcu_motorControllerRequest_U*) CAN_mcu_motorControllerRequest.txM.payload;

	CAN_configureMailbox(&CAN_bms_status.txM);
	CAN_bms_status_P = (CAN_bms_status_U*) CAN_bms_status.txM.payload;

	CAN_configureMailbox(&CAN_bms_cellVoltages.txM);
	CAN_bms_cellVoltages_P = (CAN_bms_cellVoltages_U*) CAN_bms_cellVoltages.txM.payload;

	CAN_motorcontroller_response.txM.payload = &CAN_motorcontroller_response.txP;
	CAN_motorcontroller_response_P = (CAN_motorcontroller_response_U*) & CAN_motorcontroller_response.txP;

}
