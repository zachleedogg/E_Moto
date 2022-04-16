#include "charger_dbc.h"

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

uint16_t CAN_bms_charger_request_output_voltage_high_byte_get(void){
	return CAN_bms_charger_request_P->signals.output_voltage_high_byte;
}
uint16_t CAN_bms_charger_request_output_voltage_low_byte_get(void){
	return CAN_bms_charger_request_P->signals.output_voltage_low_byte;
}
uint16_t CAN_bms_charger_request_output_current_high_byte_get(void){
	return CAN_bms_charger_request_P->signals.output_current_high_byte;
}
uint16_t CAN_bms_charger_request_output_current_low_byte_get(void){
	return CAN_bms_charger_request_P->signals.output_current_low_byte;
}
uint16_t CAN_bms_charger_request_start_charge_request_get(void){
	return CAN_bms_charger_request_P->signals.start_charge_request;
}
uint16_t CAN_bms_charger_request_charge_mode_get(void){
	return CAN_bms_charger_request_P->signals.charge_mode;
}
uint16_t CAN_bms_charger_request_packCurrent_get(void){
	return CAN_bms_charger_request_P->signals.packCurrent;
}
uint16_t CAN_bms_charger_request_byte_7_get(void){
	return CAN_bms_charger_request_P->signals.byte_7;
}
uint16_t CAN_bms_charger_request_byte_8_get(void){
	return CAN_bms_charger_request_P->signals.byte_8;
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

void CAN_charger_charger_status_output_voltage_high_byte_set(uint16_t output_voltage_high_byte){
	CAN_charger_charger_status_P->signals.output_voltage_high_byte = output_voltage_high_byte;
}
void CAN_charger_charger_status_output_voltage_low_byte_set(uint16_t output_voltage_low_byte){
	CAN_charger_charger_status_P->signals.output_voltage_low_byte = output_voltage_low_byte;
}
void CAN_charger_charger_status_output_current_high_byte_set(uint16_t output_current_high_byte){
	CAN_charger_charger_status_P->signals.output_current_high_byte = output_current_high_byte;
}
void CAN_charger_charger_status_output_current_low_byte_set(uint16_t output_current_low_byte){
	CAN_charger_charger_status_P->signals.output_current_low_byte = output_current_low_byte;
}
void CAN_charger_charger_status_hardware_error_set(uint16_t hardware_error){
	CAN_charger_charger_status_P->signals.hardware_error = hardware_error;
}
void CAN_charger_charger_status_charger_overtemp_error_set(uint16_t charger_overtemp_error){
	CAN_charger_charger_status_P->signals.charger_overtemp_error = charger_overtemp_error;
}
void CAN_charger_charger_status_input_voltage_error_set(uint16_t input_voltage_error){
	CAN_charger_charger_status_P->signals.input_voltage_error = input_voltage_error;
}
void CAN_charger_charger_status_battery_detect_error_set(uint16_t battery_detect_error){
	CAN_charger_charger_status_P->signals.battery_detect_error = battery_detect_error;
}
void CAN_charger_charger_status_communication_error_set(uint16_t communication_error){
	CAN_charger_charger_status_P->signals.communication_error = communication_error;
}
void CAN_charger_charger_status_byte7_set(uint16_t byte7){
	CAN_charger_charger_status_P->signals.byte7 = byte7;
}
void CAN_charger_charger_status_byte8_set(uint16_t byte8){
	CAN_charger_charger_status_P->signals.byte8 = byte8;
}

void CAN_charger_charger_status_send(void){
	CAN_write(CAN_charger_charger_status.txM);
}

void CAN_DBC_init() {
	CAN_configureMailbox(&CAN_dash_data1.txM);
	CAN_dash_data1_P = (CAN_dash_data1_U*) CAN_dash_data1.txM.payload;

	CAN_configureMailbox(&CAN_dash_data2.txM);
	CAN_dash_data2_P = (CAN_dash_data2_U*) CAN_dash_data2.txM.payload;

	CAN_configureMailbox(&CAN_mcu_command.txM);
	CAN_mcu_command_P = (CAN_mcu_command_U*) CAN_mcu_command.txM.payload;

	CAN_configureMailbox(&CAN_mcu_motorControllerRequest.txM);
	CAN_mcu_motorControllerRequest_P = (CAN_mcu_motorControllerRequest_U*) CAN_mcu_motorControllerRequest.txM.payload;

	CAN_configureMailbox(&CAN_bms_charger_request.txM);
	CAN_bms_charger_request_P = (CAN_bms_charger_request_U*) CAN_bms_charger_request.txM.payload;

	CAN_configureMailbox(&CAN_motorcontroller_motorStatus.txM);
	CAN_motorcontroller_motorStatus_P = (CAN_motorcontroller_motorStatus_U*) CAN_motorcontroller_motorStatus.txM.payload;

	CAN_configureMailbox(&CAN_motorcontroller_response.txM);
	CAN_motorcontroller_response_P = (CAN_motorcontroller_response_U*) CAN_motorcontroller_response.txM.payload;

	CAN_charger_charger_status.txM.payload = &CAN_charger_charger_status.txP;
	CAN_charger_charger_status_P = (CAN_charger_charger_status_U*) &CAN_charger_charger_status.txP;

}
