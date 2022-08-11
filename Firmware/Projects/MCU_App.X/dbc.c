#include "dbc.h"

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
		.canID = 1792,
		.payload = 0
	},
	.txp =
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

void CAN_DASH_status_send(void){
	CAN_write(CAN_DASH_status.txM);
}

static CAN_message_TX_S CAN_DASH_command={
	.txM = 
	{
		.canID = 1793,
		.payload = 0
	},
	.txp =
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

void CAN_DASH_command_battery_latch_set(uint16_t battery_latch){
	CAN_DASH_command_P->signals.battery_latch = battery_latch;
}
void CAN_DASH_command_dothat_set(uint16_t dothat){
	CAN_DASH_command_P->signals.dothat = dothat;
}

uint16_t CAN_DASH_command_battery_latch_get(void){
	return CAN_DASH_command_P->signals.battery_latch;
}
uint16_t CAN_DASH_command_dothat_get(void){
	return CAN_DASH_command_P->signals.dothat;
}

void CAN_DASH_command_send(void){
	CAN_write(CAN_DASH_command.txM);
}




/**********************************************************
 * BODYCONTROLLER NODE MESSAGES
 */
static CAN_message_TX_S CAN_BODYCONTROLLER_status={
	.txM = 
	{
		.canID = 1808,
		.payload = 0
	},
	.txp =
	{
		.word0 = 0,
		.word1 = 0,
		.word2 = 0,
		.word3 = 0
	}
};

typedef union {
	CAN_payload_S packedMessage;
	CAN_BODYCONTROLLER_status_S signals;
} CAN_BODYCONTROLLER_status_U;

static CAN_BODYCONTROLLER_status_U * CAN_BODYCONTROLLER_status_P;

void CAN_BODYCONTROLLER_status_state_set(uint16_t state){
	CAN_BODYCONTROLLER_status_P->signals.state = state;
}

uint16_t CAN_BODYCONTROLLER_status_state_get(void){
	return CAN_BODYCONTROLLER_status_P->signals.state;
}

void CAN_BODYCONTROLLER_status_send(void){
	CAN_write(CAN_BODYCONTROLLER_status.txM);
}

static CAN_message_TX_S CAN_BODYCONTROLLER_command={
	.txM = 
	{
		.canID = 1809,
		.payload = 0
	},
	.txp =
	{
		.word0 = 0,
		.word1 = 0,
		.word2 = 0,
		.word3 = 0
	}
};

typedef union {
	CAN_payload_S packedMessage;
	CAN_BODYCONTROLLER_command_S signals;
} CAN_BODYCONTROLLER_command_U;

static CAN_BODYCONTROLLER_command_U * CAN_BODYCONTROLLER_command_P;

void CAN_BODYCONTROLLER_command_doSomthingElse_set(uint16_t doSomthingElse){
	CAN_BODYCONTROLLER_command_P->signals.doSomthingElse = doSomthingElse;
}

uint16_t CAN_BODYCONTROLLER_command_doSomthingElse_get(void){
	return CAN_BODYCONTROLLER_command_P->signals.doSomthingElse;
}

void CAN_BODYCONTROLLER_command_send(void){
	CAN_write(CAN_BODYCONTROLLER_command.txM);
}




/**********************************************************
 * BMS NODE MESSAGES
 */
static CAN_message_TX_S CAN_BMS_status={
	.txM = 
	{
		.canID = 1824,
		.payload = 0
	},
	.txp =
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

void CAN_BMS_status_state_set(uint16_t state){
	CAN_BMS_status_P->signals.state = state;
}
void CAN_BMS_status_SOC_set(uint16_t SOC){
	CAN_BMS_status_P->signals.SOC = SOC;
}
void CAN_BMS_status_packVoltage_set(uint16_t packVoltage){
	CAN_BMS_status_P->signals.packVoltage = packVoltage;
}
void CAN_BMS_status_packCurrent_set(uint16_t packCurrent){
	CAN_BMS_status_P->signals.packCurrent = packCurrent;
}
void CAN_BMS_status_minTemp_set(uint16_t minTemp){
	CAN_BMS_status_P->signals.minTemp = minTemp;
}
void CAN_BMS_status_maxTemp_set(uint16_t maxTemp){
	CAN_BMS_status_P->signals.maxTemp = maxTemp;
}

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

void CAN_BMS_status_send(void){
	CAN_write(CAN_BMS_status.txM);
}

static CAN_message_TX_S CAN_BMS_cellVoltages={
	.txM = 
	{
		.canID = 1825,
		.payload = 0
	},
	.txp =
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

void CAN_BMS_cellVoltages_MultiPlex_set(uint16_t MultiPlex){
	CAN_BMS_cellVoltages_P->signals.MultiPlex = MultiPlex;
}
void CAN_BMS_cellVoltages_cell1_set(uint16_t cell1){
	CAN_BMS_cellVoltages_P->signals.cell1 = cell1;
}
void CAN_BMS_cellVoltages_cell2_set(uint16_t cell2){
	CAN_BMS_cellVoltages_P->signals.cell2 = cell2;
}
void CAN_BMS_cellVoltages_cell3_set(uint16_t cell3){
	CAN_BMS_cellVoltages_P->signals.cell3 = cell3;
}
void CAN_BMS_cellVoltages_cell4_set(uint16_t cell4){
	CAN_BMS_cellVoltages_P->signals.cell4 = cell4;
}
void CAN_BMS_cellVoltages_cell5_set(uint16_t cell5){
	CAN_BMS_cellVoltages_P->signals.cell5 = cell5;
}

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

void CAN_BMS_cellVoltages_send(void){
	CAN_write(CAN_BMS_cellVoltages.txM);
}

void_DBC_init() {
	CAN_DASH_status.txM.payload = &CAN_DASH_status.txp;
	CAN_DASH_status_P = (CAN_DASH_status_U*) & CAN_DASH_status.txp;

	CAN_DASH_command.txM.payload = &CAN_DASH_command.txp;
	CAN_DASH_command_P = (CAN_DASH_command_U*) & CAN_DASH_command.txp;

	CAN_BODYCONTROLLER_status.txM.payload = &CAN_BODYCONTROLLER_status.txp;
	CAN_BODYCONTROLLER_status_P = (CAN_BODYCONTROLLER_status_U*) & CAN_BODYCONTROLLER_status.txp;

	CAN_BODYCONTROLLER_command.txM.payload = &CAN_BODYCONTROLLER_command.txp;
	CAN_BODYCONTROLLER_command_P = (CAN_BODYCONTROLLER_command_U*) & CAN_BODYCONTROLLER_command.txp;

	CAN_BMS_status.txM.payload = &CAN_BMS_status.txp;
	CAN_BMS_status_P = (CAN_BMS_status_U*) & CAN_BMS_status.txp;

	CAN_BMS_cellVoltages.txM.payload = &CAN_BMS_cellVoltages.txp;
	CAN_BMS_cellVoltages_P = (CAN_BMS_cellVoltages_U*) & CAN_BMS_cellVoltages.txp;

}
