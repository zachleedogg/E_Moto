#include "bms_dbc.h"

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
static CAN_message_TX_S CAN_mcu_status={
	.txM = 
	{
		.canID = CAN_mcu_status_ID,
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
uint16_t CAN_mcu_status_lowBeam_get(void){
	return CAN_mcu_status_P->signals.lowBeam;
}
uint16_t CAN_mcu_status_brakeLight_get(void){
	return CAN_mcu_status_P->signals.brakeLight;
}
uint16_t CAN_mcu_status_tailLight_get(void){
	return CAN_mcu_status_P->signals.tailLight;
}
uint16_t CAN_mcu_status_horn_get(void){
	return CAN_mcu_status_P->signals.horn;
}
uint16_t CAN_mcu_status_turnSignalFR_get(void){
	return CAN_mcu_status_P->signals.turnSignalFR;
}
uint16_t CAN_mcu_status_turnSignalFL_get(void){
	return CAN_mcu_status_P->signals.turnSignalFL;
}
uint16_t CAN_mcu_status_turnSignalRR_get(void){
	return CAN_mcu_status_P->signals.turnSignalRR;
}
uint16_t CAN_mcu_status_turnSignalRL_get(void){
	return CAN_mcu_status_P->signals.turnSignalRL;
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
uint16_t CAN_mcu_status_brakeSwitchFront_get(void){
	return CAN_mcu_status_P->signals.brakeSwitchFront;
}
uint16_t CAN_mcu_status_brakeSwitchRear_get(void){
	return CAN_mcu_status_P->signals.brakeSwitchRear;
}
uint16_t CAN_mcu_status_throttleVal_get(void){
	return CAN_mcu_status_P->signals.throttleVal;
}

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
static CAN_message_TX_S CAN_bms_status={
	.txM = 
	{
		.canID = CAN_bms_status_ID,
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
	CAN_bms_status_S signals;
} CAN_bms_status_U;

static CAN_bms_status_U * CAN_bms_status_P;

void CAN_bms_status_state_set(uint16_t state){
	CAN_bms_status_P->signals.state = state;
}
void CAN_bms_status_SOC_set(uint16_t SOC){
	CAN_bms_status_P->signals.SOC = SOC;
}
void CAN_bms_status_packVoltage_set(uint16_t packVoltage){
	CAN_bms_status_P->signals.packVoltage = packVoltage;
}
void CAN_bms_status_packCurrent_set(uint16_t packCurrent){
	CAN_bms_status_P->signals.packCurrent = packCurrent;
}
void CAN_bms_status_minTemp_set(uint16_t minTemp){
	CAN_bms_status_P->signals.minTemp = minTemp;
}
void CAN_bms_status_maxTemp_set(uint16_t maxTemp){
	CAN_bms_status_P->signals.maxTemp = maxTemp;
}

void CAN_bms_status_send(void){
	CAN_write(CAN_bms_status.txM);
}

static CAN_message_TX_S CAN_bms_charger_request={
	.txM = 
	{
		.canID = CAN_bms_charger_request_ID,
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
	CAN_bms_charger_request_S signals;
} CAN_bms_charger_request_U;

static CAN_bms_charger_request_U * CAN_bms_charger_request_P;

void CAN_bms_charger_request_output_voltage_high_byte_set(uint16_t output_voltage_high_byte){
	CAN_bms_charger_request_P->signals.output_voltage_high_byte = output_voltage_high_byte;
}
void CAN_bms_charger_request_output_voltage_low_byte_set(uint16_t output_voltage_low_byte){
	CAN_bms_charger_request_P->signals.output_voltage_low_byte = output_voltage_low_byte;
}
void CAN_bms_charger_request_output_current_high_byte_set(uint16_t output_current_high_byte){
	CAN_bms_charger_request_P->signals.output_current_high_byte = output_current_high_byte;
}
void CAN_bms_charger_request_output_current_low_byte_set(uint16_t output_current_low_byte){
	CAN_bms_charger_request_P->signals.output_current_low_byte = output_current_low_byte;
}
void CAN_bms_charger_request_start_charge_request_set(uint16_t start_charge_request){
	CAN_bms_charger_request_P->signals.start_charge_request = start_charge_request;
}
void CAN_bms_charger_request_charge_mode_set(uint16_t charge_mode){
	CAN_bms_charger_request_P->signals.charge_mode = charge_mode;
}
void CAN_bms_charger_request_packCurrent_set(uint16_t packCurrent){
	CAN_bms_charger_request_P->signals.packCurrent = packCurrent;
}
void CAN_bms_charger_request_byte_7_set(uint16_t byte_7){
	CAN_bms_charger_request_P->signals.byte_7 = byte_7;
}
void CAN_bms_charger_request_byte_8_set(uint16_t byte_8){
	CAN_bms_charger_request_P->signals.byte_8 = byte_8;
}

void CAN_bms_charger_request_send(void){
	CAN_write(CAN_bms_charger_request.txM);
}

static CAN_message_TX_S CAN_bms_cellVoltages={
	.txM = 
	{
		.canID = CAN_bms_cellVoltages_ID,
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
	CAN_bms_cellVoltages_S signals;
} CAN_bms_cellVoltages_U;

static CAN_bms_cellVoltages_U * CAN_bms_cellVoltages_P;

void CAN_bms_cellVoltages_MultiPlex_set(uint16_t MultiPlex){
	CAN_bms_cellVoltages_P->signals.MultiPlex = MultiPlex;
}
void CAN_bms_cellVoltages_cell1_set(uint16_t cell1){
	CAN_bms_cellVoltages_P->signals.cell1 = cell1;
}
void CAN_bms_cellVoltages_cell2_set(uint16_t cell2){
	CAN_bms_cellVoltages_P->signals.cell2 = cell2;
}
void CAN_bms_cellVoltages_cell3_set(uint16_t cell3){
	CAN_bms_cellVoltages_P->signals.cell3 = cell3;
}
void CAN_bms_cellVoltages_cell4_set(uint16_t cell4){
	CAN_bms_cellVoltages_P->signals.cell4 = cell4;
}
void CAN_bms_cellVoltages_cell5_set(uint16_t cell5){
	CAN_bms_cellVoltages_P->signals.cell5 = cell5;
}

void CAN_bms_cellVoltages_send(void){
	CAN_write(CAN_bms_cellVoltages.txM);
}




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

uint16_t CAN_motorcontroller_motorStatus_motorSpeed_get(void){
	return CAN_motorcontroller_motorStatus_P->signals.motorSpeed;
}
uint16_t CAN_motorcontroller_motorStatus_motorCurrent_get(void){
	return CAN_motorcontroller_motorStatus_P->signals.motorCurrent;
}
uint16_t CAN_motorcontroller_motorStatus_IphaseA_get(void){
	return CAN_motorcontroller_motorStatus_P->signals.IphaseA;
}
uint16_t CAN_motorcontroller_motorStatus_IphaseB_get(void){
	return CAN_motorcontroller_motorStatus_P->signals.IphaseB;
}
uint16_t CAN_motorcontroller_motorStatus_IphaseC_get(void){
	return CAN_motorcontroller_motorStatus_P->signals.IphaseC;
}
uint16_t CAN_motorcontroller_motorStatus_VphaseA_get(void){
	return CAN_motorcontroller_motorStatus_P->signals.VphaseA;
}
uint16_t CAN_motorcontroller_motorStatus_VphaseB_get(void){
	return CAN_motorcontroller_motorStatus_P->signals.VphaseB;
}
uint16_t CAN_motorcontroller_motorStatus_VphaseC_get(void){
	return CAN_motorcontroller_motorStatus_P->signals.VphaseC;
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

uint16_t CAN_motorcontroller_response_byte1_get(void){
	return CAN_motorcontroller_response_P->signals.byte1;
}
uint16_t CAN_motorcontroller_response_byte2_get(void){
	return CAN_motorcontroller_response_P->signals.byte2;
}
uint16_t CAN_motorcontroller_response_byte3_get(void){
	return CAN_motorcontroller_response_P->signals.byte3;
}
uint16_t CAN_motorcontroller_response_byte4_get(void){
	return CAN_motorcontroller_response_P->signals.byte4;
}
uint16_t CAN_motorcontroller_response_byte5_get(void){
	return CAN_motorcontroller_response_P->signals.byte5;
}
uint16_t CAN_motorcontroller_response_byte6_get(void){
	return CAN_motorcontroller_response_P->signals.byte6;
}
uint16_t CAN_motorcontroller_response_byte7_get(void){
	return CAN_motorcontroller_response_P->signals.byte7;
}
uint16_t CAN_motorcontroller_response_byte8_get(void){
	return CAN_motorcontroller_response_P->signals.byte8;
}




/**********************************************************
 * charger NODE MESSAGES
 */
static CAN_message_TX_S CAN_charger_charger_status={
	.txM = 
	{
		.canID = CAN_charger_charger_status_ID,
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
	CAN_charger_charger_status_S signals;
} CAN_charger_charger_status_U;

static CAN_charger_charger_status_U * CAN_charger_charger_status_P;

uint16_t CAN_charger_charger_status_output_voltage_high_byte_get(void){
	return CAN_charger_charger_status_P->signals.output_voltage_high_byte;
}
uint16_t CAN_charger_charger_status_output_voltage_low_byte_get(void){
	return CAN_charger_charger_status_P->signals.output_voltage_low_byte;
}
uint16_t CAN_charger_charger_status_output_current_high_byte_get(void){
	return CAN_charger_charger_status_P->signals.output_current_high_byte;
}
uint16_t CAN_charger_charger_status_output_current_low_byte_get(void){
	return CAN_charger_charger_status_P->signals.output_current_low_byte;
}
uint16_t CAN_charger_charger_status_hardware_error_get(void){
	return CAN_charger_charger_status_P->signals.hardware_error;
}
uint16_t CAN_charger_charger_status_charger_overtemp_error_get(void){
	return CAN_charger_charger_status_P->signals.charger_overtemp_error;
}
uint16_t CAN_charger_charger_status_input_voltage_error_get(void){
	return CAN_charger_charger_status_P->signals.input_voltage_error;
}
uint16_t CAN_charger_charger_status_battery_detect_error_get(void){
	return CAN_charger_charger_status_P->signals.battery_detect_error;
}
uint16_t CAN_charger_charger_status_communication_error_get(void){
	return CAN_charger_charger_status_P->signals.communication_error;
}
uint16_t CAN_charger_charger_status_byte7_get(void){
	return CAN_charger_charger_status_P->signals.byte7;
}
uint16_t CAN_charger_charger_status_byte8_get(void){
	return CAN_charger_charger_status_P->signals.byte8;
}

void CAN_DBC_init() {
	CAN_configureMailbox(&CAN_dash_data1.txM);
	CAN_dash_data1_P = (CAN_dash_data1_U*) CAN_dash_data1.txM.payload;

	CAN_configureMailbox(&CAN_dash_data2.txM);
	CAN_dash_data2_P = (CAN_dash_data2_U*) CAN_dash_data2.txM.payload;

	CAN_configureMailbox(&CAN_mcu_status.txM);
	CAN_mcu_status_P = (CAN_mcu_status_U*) CAN_mcu_status.txM.payload;

	CAN_configureMailbox(&CAN_mcu_command.txM);
	CAN_mcu_command_P = (CAN_mcu_command_U*) CAN_mcu_command.txM.payload;

	CAN_configureMailbox(&CAN_mcu_motorControllerRequest.txM);
	CAN_mcu_motorControllerRequest_P = (CAN_mcu_motorControllerRequest_U*) CAN_mcu_motorControllerRequest.txM.payload;

	CAN_bms_status.txM.payload = &CAN_bms_status.txP;
	CAN_bms_status_P = (CAN_bms_status_U*) &CAN_bms_status.txP;

	CAN_bms_charger_request.txM.payload = &CAN_bms_charger_request.txP;
	CAN_bms_charger_request_P = (CAN_bms_charger_request_U*) &CAN_bms_charger_request.txP;

	CAN_bms_cellVoltages.txM.payload = &CAN_bms_cellVoltages.txP;
	CAN_bms_cellVoltages_P = (CAN_bms_cellVoltages_U*) &CAN_bms_cellVoltages.txP;

	CAN_configureMailbox(&CAN_motorcontroller_motorStatus.txM);
	CAN_motorcontroller_motorStatus_P = (CAN_motorcontroller_motorStatus_U*) CAN_motorcontroller_motorStatus.txM.payload;

	CAN_configureMailbox(&CAN_motorcontroller_response.txM);
	CAN_motorcontroller_response_P = (CAN_motorcontroller_response_U*) CAN_motorcontroller_response.txM.payload;

	CAN_configureMailbox(&CAN_charger_charger_status.txM);
	CAN_charger_charger_status_P = (CAN_charger_charger_status_U*) CAN_charger_charger_status.txM.payload;

}
