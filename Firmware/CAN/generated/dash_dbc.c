#include "dash_dbc.h"
#include "utils.h"
/**********************************************************
 * dash NODE MESSAGES
 */
#define CAN_dash_status_ID 0x701

static CAN_payload_S CAN_dash_status_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_dash_status={
	.canID = CAN_dash_status_ID,
	.canXID = 0x0,
	.payload = &CAN_dash_status_payload,
	.canMessageStatus = 0
};

#define CAN_DASH_STATUS_HEARTBEAT_RANGE 4
#define CAN_DASH_STATUS_HEARTBEAT_OFFSET 0
#define CAN_DASH_STATUS_STATE_RANGE 3
#define CAN_DASH_STATUS_STATE_OFFSET 4
#define CAN_DASH_STATUS_KILLBUTTON_RANGE 2
#define CAN_DASH_STATUS_KILLBUTTON_OFFSET 7
#define CAN_DASH_STATUS_IGNBUTTON_RANGE 2
#define CAN_DASH_STATUS_IGNBUTTON_OFFSET 9
#define CAN_DASH_STATUS_MODEBUTTON_RANGE 2
#define CAN_DASH_STATUS_MODEBUTTON_OFFSET 11
#define CAN_DASH_STATUS_SELECTBUTTON_RANGE 2
#define CAN_DASH_STATUS_SELECTBUTTON_OFFSET 13
#define CAN_DASH_STATUS_DRIVEMODE_RANGE 3
#define CAN_DASH_STATUS_DRIVEMODE_OFFSET 15

void CAN_dash_status_heartBeat_set(uint16_t heartBeat){
	set_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_HEARTBEAT_OFFSET, CAN_DASH_STATUS_HEARTBEAT_RANGE, heartBeat);
}
void CAN_dash_status_state_set(uint16_t state){
	set_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_STATE_OFFSET, CAN_DASH_STATUS_STATE_RANGE, state);
}
void CAN_dash_status_killButton_set(uint16_t killButton){
	set_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_KILLBUTTON_OFFSET, CAN_DASH_STATUS_KILLBUTTON_RANGE, killButton);
}
void CAN_dash_status_ignButton_set(uint16_t ignButton){
	set_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_IGNBUTTON_OFFSET, CAN_DASH_STATUS_IGNBUTTON_RANGE, ignButton);
}
void CAN_dash_status_modeButton_set(uint16_t modeButton){
	set_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_MODEBUTTON_OFFSET, CAN_DASH_STATUS_MODEBUTTON_RANGE, modeButton);
}
void CAN_dash_status_selectButton_set(uint16_t selectButton){
	set_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_SELECTBUTTON_OFFSET, CAN_DASH_STATUS_SELECTBUTTON_RANGE, selectButton);
}
void CAN_dash_status_driveMode_set(uint16_t driveMode){
	set_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_DRIVEMODE_OFFSET, CAN_DASH_STATUS_DRIVEMODE_RANGE, driveMode);
}
void CAN_dash_status_send(void){
	CAN_write(CAN_dash_status);
}

#define CAN_dash_command_ID 0x702

static CAN_payload_S CAN_dash_command_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_dash_command={
	.canID = CAN_dash_command_ID,
	.canXID = 0x0,
	.payload = &CAN_dash_command_payload,
	.canMessageStatus = 0
};

#define CAN_DASH_COMMAND_IGNITIONREQUEST_RANGE 1
#define CAN_DASH_COMMAND_IGNITIONREQUEST_OFFSET 0
#define CAN_DASH_COMMAND_KILLREQUEST_RANGE 1
#define CAN_DASH_COMMAND_KILLREQUEST_OFFSET 1
#define CAN_DASH_COMMAND_BATTERYEJECTREQUEST_RANGE 1
#define CAN_DASH_COMMAND_BATTERYEJECTREQUEST_OFFSET 2
#define CAN_DASH_COMMAND_LIGHTSREQUEST_RANGE 1
#define CAN_DASH_COMMAND_LIGHTSREQUEST_OFFSET 3
#define CAN_DASH_COMMAND_HORNREQUEST_RANGE 1
#define CAN_DASH_COMMAND_HORNREQUEST_OFFSET 4

void CAN_dash_command_ignitionRequest_set(uint16_t ignitionRequest){
	set_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_IGNITIONREQUEST_OFFSET, CAN_DASH_COMMAND_IGNITIONREQUEST_RANGE, ignitionRequest);
}
void CAN_dash_command_killRequest_set(uint16_t killRequest){
	set_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_KILLREQUEST_OFFSET, CAN_DASH_COMMAND_KILLREQUEST_RANGE, killRequest);
}
void CAN_dash_command_batteryEjectRequest_set(uint16_t batteryEjectRequest){
	set_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_BATTERYEJECTREQUEST_OFFSET, CAN_DASH_COMMAND_BATTERYEJECTREQUEST_RANGE, batteryEjectRequest);
}
void CAN_dash_command_lightsRequest_set(uint16_t lightsRequest){
	set_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_LIGHTSREQUEST_OFFSET, CAN_DASH_COMMAND_LIGHTSREQUEST_RANGE, lightsRequest);
}
void CAN_dash_command_hornRequest_set(uint16_t hornRequest){
	set_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_HORNREQUEST_OFFSET, CAN_DASH_COMMAND_HORNREQUEST_RANGE, hornRequest);
}
void CAN_dash_command_send(void){
	CAN_write(CAN_dash_command);
}

#define CAN_dash_data1_ID 0x1806e5f5

static CAN_payload_S CAN_dash_data1_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_dash_data1={
	.canID = CAN_dash_data1_ID,
	.canXID = 0x1,
	.payload = &CAN_dash_data1_payload,
	.canMessageStatus = 0
};

#define CAN_DASH_DATA1_SPEED_RANGE 16
#define CAN_DASH_DATA1_SPEED_OFFSET 0
#define CAN_DASH_DATA1_ODOMETER_RANGE 16
#define CAN_DASH_DATA1_ODOMETER_OFFSET 16
#define CAN_DASH_DATA1_TRIPA_RANGE 16
#define CAN_DASH_DATA1_TRIPA_OFFSET 32
#define CAN_DASH_DATA1_TRIPB_RANGE 16
#define CAN_DASH_DATA1_TRIPB_OFFSET 48

void CAN_dash_data1_speed_set(uint16_t speed){
	set_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_SPEED_OFFSET, CAN_DASH_DATA1_SPEED_RANGE, speed);
}
void CAN_dash_data1_odometer_set(uint16_t odometer){
	set_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_ODOMETER_OFFSET, CAN_DASH_DATA1_ODOMETER_RANGE, odometer);
}
void CAN_dash_data1_tripA_set(uint16_t tripA){
	set_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_TRIPA_OFFSET, CAN_DASH_DATA1_TRIPA_RANGE, tripA);
}
void CAN_dash_data1_tripB_set(uint16_t tripB){
	set_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_TRIPB_OFFSET, CAN_DASH_DATA1_TRIPB_RANGE, tripB);
}
void CAN_dash_data1_send(void){
	CAN_write(CAN_dash_data1);
}

#define CAN_dash_data2_ID 0x704

static CAN_payload_S CAN_dash_data2_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_dash_data2={
	.canID = CAN_dash_data2_ID,
	.canXID = 0x0,
	.payload = &CAN_dash_data2_payload,
	.canMessageStatus = 0
};

#define CAN_DASH_DATA2_RUNNINGTIME_RANGE 16
#define CAN_DASH_DATA2_RUNNINGTIME_OFFSET 0
#define CAN_DASH_DATA2_ODOMETER_RANGE 16
#define CAN_DASH_DATA2_ODOMETER_OFFSET 16
#define CAN_DASH_DATA2_TRIPA_RANGE 16
#define CAN_DASH_DATA2_TRIPA_OFFSET 32
#define CAN_DASH_DATA2_TRIPB_RANGE 16
#define CAN_DASH_DATA2_TRIPB_OFFSET 48

void CAN_dash_data2_runningTime_set(uint16_t runningTime){
	set_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_RUNNINGTIME_OFFSET, CAN_DASH_DATA2_RUNNINGTIME_RANGE, runningTime);
}
void CAN_dash_data2_odometer_set(uint16_t odometer){
	set_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_ODOMETER_OFFSET, CAN_DASH_DATA2_ODOMETER_RANGE, odometer);
}
void CAN_dash_data2_tripA_set(uint16_t tripA){
	set_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_TRIPA_OFFSET, CAN_DASH_DATA2_TRIPA_RANGE, tripA);
}
void CAN_dash_data2_tripB_set(uint16_t tripB){
	set_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_TRIPB_OFFSET, CAN_DASH_DATA2_TRIPB_RANGE, tripB);
}
void CAN_dash_data2_send(void){
	CAN_write(CAN_dash_data2);
}

/**********************************************************
 * mcu NODE MESSAGES
 */
#define CAN_mcu_status_ID 0x711

static CAN_message_S CAN_mcu_status={
	.canID = CAN_mcu_status_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_mcu_status_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_mcu_status);
}
#define CAN_MCU_STATUS_STATE_RANGE 3
#define CAN_MCU_STATUS_STATE_OFFSET 0
#define CAN_MCU_STATUS_THROTTLEMODE_RANGE 3
#define CAN_MCU_STATUS_THROTTLEMODE_OFFSET 3
#define CAN_MCU_STATUS_HIGHBEAM_RANGE 1
#define CAN_MCU_STATUS_HIGHBEAM_OFFSET 6
#define CAN_MCU_STATUS_LOWBEAM_RANGE 1
#define CAN_MCU_STATUS_LOWBEAM_OFFSET 7
#define CAN_MCU_STATUS_BRAKELIGHT_RANGE 1
#define CAN_MCU_STATUS_BRAKELIGHT_OFFSET 8
#define CAN_MCU_STATUS_TAILLIGHT_RANGE 1
#define CAN_MCU_STATUS_TAILLIGHT_OFFSET 9
#define CAN_MCU_STATUS_HORN_RANGE 1
#define CAN_MCU_STATUS_HORN_OFFSET 10
#define CAN_MCU_STATUS_TURNSIGNALFR_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALFR_OFFSET 11
#define CAN_MCU_STATUS_TURNSIGNALFL_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALFL_OFFSET 12
#define CAN_MCU_STATUS_TURNSIGNALRR_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALRR_OFFSET 13
#define CAN_MCU_STATUS_TURNSIGNALRL_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALRL_OFFSET 14
#define CAN_MCU_STATUS_GBSALL_RANGE 1
#define CAN_MCU_STATUS_GBSALL_OFFSET 15
#define CAN_MCU_STATUS_CONTACTOR_RANGE 1
#define CAN_MCU_STATUS_CONTACTOR_OFFSET 16
#define CAN_MCU_STATUS_CHARGEPORT_RANGE 1
#define CAN_MCU_STATUS_CHARGEPORT_OFFSET 17
#define CAN_MCU_STATUS_BRAKESWITCHFRONT_RANGE 1
#define CAN_MCU_STATUS_BRAKESWITCHFRONT_OFFSET 18
#define CAN_MCU_STATUS_BRAKESWITCHREAR_RANGE 1
#define CAN_MCU_STATUS_BRAKESWITCHREAR_OFFSET 19
#define CAN_MCU_STATUS_THROTTLEVAL_RANGE 8
#define CAN_MCU_STATUS_THROTTLEVAL_OFFSET 20

uint16_t CAN_mcu_status_state_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_STATE_OFFSET, CAN_MCU_STATUS_STATE_RANGE);
}
uint16_t CAN_mcu_status_throttleMode_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEMODE_OFFSET, CAN_MCU_STATUS_THROTTLEMODE_RANGE);
}
uint16_t CAN_mcu_status_highBeam_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HIGHBEAM_OFFSET, CAN_MCU_STATUS_HIGHBEAM_RANGE);
}
uint16_t CAN_mcu_status_lowBeam_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_LOWBEAM_OFFSET, CAN_MCU_STATUS_LOWBEAM_RANGE);
}
uint16_t CAN_mcu_status_brakeLight_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKELIGHT_OFFSET, CAN_MCU_STATUS_BRAKELIGHT_RANGE);
}
uint16_t CAN_mcu_status_tailLight_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TAILLIGHT_OFFSET, CAN_MCU_STATUS_TAILLIGHT_RANGE);
}
uint16_t CAN_mcu_status_horn_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HORN_OFFSET, CAN_MCU_STATUS_HORN_RANGE);
}
uint16_t CAN_mcu_status_turnSignalFR_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFR_OFFSET, CAN_MCU_STATUS_TURNSIGNALFR_RANGE);
}
uint16_t CAN_mcu_status_turnSignalFL_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFL_OFFSET, CAN_MCU_STATUS_TURNSIGNALFL_RANGE);
}
uint16_t CAN_mcu_status_turnSignalRR_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRR_OFFSET, CAN_MCU_STATUS_TURNSIGNALRR_RANGE);
}
uint16_t CAN_mcu_status_turnSignalRL_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRL_OFFSET, CAN_MCU_STATUS_TURNSIGNALRL_RANGE);
}
uint16_t CAN_mcu_status_GBSALL_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_GBSALL_OFFSET, CAN_MCU_STATUS_GBSALL_RANGE);
}
uint16_t CAN_mcu_status_contactor_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_CONTACTOR_OFFSET, CAN_MCU_STATUS_CONTACTOR_RANGE);
}
uint16_t CAN_mcu_status_chargePort_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_CHARGEPORT_OFFSET, CAN_MCU_STATUS_CHARGEPORT_RANGE);
}
uint16_t CAN_mcu_status_brakeSwitchFront_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHFRONT_OFFSET, CAN_MCU_STATUS_BRAKESWITCHFRONT_RANGE);
}
uint16_t CAN_mcu_status_brakeSwitchRear_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHREAR_OFFSET, CAN_MCU_STATUS_BRAKESWITCHREAR_RANGE);
}
uint16_t CAN_mcu_status_throttleVal_get(void){
	return get_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEVAL_OFFSET, CAN_MCU_STATUS_THROTTLEVAL_RANGE);
}

#define CAN_mcu_command_ID 0x712

static CAN_message_S CAN_mcu_command={
	.canID = CAN_mcu_command_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_mcu_command_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_mcu_command);
}
#define CAN_MCU_COMMAND_DCDC_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_DCDC_ENABLE_OFFSET 0
#define CAN_MCU_COMMAND_EV_CHARGER_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_EV_CHARGER_ENABLE_OFFSET 1
#define CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_OFFSET 2

uint16_t CAN_mcu_command_DCDC_enable_get(void){
	return get_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_DCDC_ENABLE_OFFSET, CAN_MCU_COMMAND_DCDC_ENABLE_RANGE);
}
uint16_t CAN_mcu_command_ev_charger_enable_get(void){
	return get_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_EV_CHARGER_ENABLE_OFFSET, CAN_MCU_COMMAND_EV_CHARGER_ENABLE_RANGE);
}
uint16_t CAN_mcu_command_motor_controller_enable_get(void){
	return get_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_OFFSET, CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_RANGE);
}

/**********************************************************
 * bms NODE MESSAGES
 */
#define CAN_bms_status_ID 0x721

static CAN_message_S CAN_bms_status={
	.canID = CAN_bms_status_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_bms_status_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_bms_status);
}
#define CAN_BMS_STATUS_STATE_RANGE 3
#define CAN_BMS_STATUS_STATE_OFFSET 0
#define CAN_BMS_STATUS_SOC_RANGE 8
#define CAN_BMS_STATUS_SOC_OFFSET 3
#define CAN_BMS_STATUS_PACKVOLTAGE_RANGE 16
#define CAN_BMS_STATUS_PACKVOLTAGE_OFFSET 11
#define CAN_BMS_STATUS_PACKCURRENT_RANGE 16
#define CAN_BMS_STATUS_PACKCURRENT_OFFSET 27
#define CAN_BMS_STATUS_MINTEMP_RANGE 8
#define CAN_BMS_STATUS_MINTEMP_OFFSET 43
#define CAN_BMS_STATUS_MAXTEMP_RANGE 8
#define CAN_BMS_STATUS_MAXTEMP_OFFSET 51

uint16_t CAN_bms_status_state_get(void){
	return get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_STATE_OFFSET, CAN_BMS_STATUS_STATE_RANGE);
}
uint16_t CAN_bms_status_SOC_get(void){
	return get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_SOC_OFFSET, CAN_BMS_STATUS_SOC_RANGE);
}
uint16_t CAN_bms_status_packVoltage_get(void){
	return get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_PACKVOLTAGE_OFFSET, CAN_BMS_STATUS_PACKVOLTAGE_RANGE);
}
uint16_t CAN_bms_status_packCurrent_get(void){
	return get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_PACKCURRENT_OFFSET, CAN_BMS_STATUS_PACKCURRENT_RANGE);
}
uint16_t CAN_bms_status_minTemp_get(void){
	return get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_MINTEMP_OFFSET, CAN_BMS_STATUS_MINTEMP_RANGE);
}
uint16_t CAN_bms_status_maxTemp_get(void){
	return get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_MAXTEMP_OFFSET, CAN_BMS_STATUS_MAXTEMP_RANGE);
}

#define CAN_bms_status_2_ID 0x722

static CAN_message_S CAN_bms_status_2={
	.canID = CAN_bms_status_2_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_bms_status_2_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_bms_status_2);
}
#define CAN_BMS_STATUS_2_DCDC_STATE_RANGE 1
#define CAN_BMS_STATUS_2_DCDC_STATE_OFFSET 0
#define CAN_BMS_STATUS_2_DCDC_FAULT_RANGE 1
#define CAN_BMS_STATUS_2_DCDC_FAULT_OFFSET 1
#define CAN_BMS_STATUS_2_DCDC_VOLTAGE_RANGE 8
#define CAN_BMS_STATUS_2_DCDC_VOLTAGE_OFFSET 2
#define CAN_BMS_STATUS_2_DCDC_CURRENT_RANGE 8
#define CAN_BMS_STATUS_2_DCDC_CURRENT_OFFSET 10
#define CAN_BMS_STATUS_2_EV_CHARGER_STATE_RANGE 1
#define CAN_BMS_STATUS_2_EV_CHARGER_STATE_OFFSET 18
#define CAN_BMS_STATUS_2_EV_CHARGER_FAULT_RANGE 1
#define CAN_BMS_STATUS_2_EV_CHARGER_FAULT_OFFSET 19
#define CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_RANGE 8
#define CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_OFFSET 20
#define CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_RANGE 8
#define CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_OFFSET 28
#define CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_RANGE 1
#define CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_OFFSET 36
#define CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_RANGE 1
#define CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_OFFSET 37
#define CAN_BMS_STATUS_2_HV_BUS_VOLTAGE_RANGE 8
#define CAN_BMS_STATUS_2_HV_BUS_VOLTAGE_OFFSET 38

uint16_t CAN_bms_status_2_DCDC_state_get(void){
	return get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_STATE_OFFSET, CAN_BMS_STATUS_2_DCDC_STATE_RANGE);
}
uint16_t CAN_bms_status_2_DCDC_fault_get(void){
	return get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_FAULT_OFFSET, CAN_BMS_STATUS_2_DCDC_FAULT_RANGE);
}
uint16_t CAN_bms_status_2_DCDC_voltage_get(void){
	return get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_DCDC_VOLTAGE_RANGE);
}
uint16_t CAN_bms_status_2_DCDC_current_get(void){
	return get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_CURRENT_OFFSET, CAN_BMS_STATUS_2_DCDC_CURRENT_RANGE);
}
uint16_t CAN_bms_status_2_EV_charger_state_get(void){
	return get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_STATE_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_STATE_RANGE);
}
uint16_t CAN_bms_status_2_EV_charger_fault_get(void){
	return get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_FAULT_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_FAULT_RANGE);
}
uint16_t CAN_bms_status_2_EV_charger_voltage_get(void){
	return get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_RANGE);
}
uint16_t CAN_bms_status_2_EV_charger_current_get(void){
	return get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_RANGE);
}
uint16_t CAN_bms_status_2_HV_precharge_state_get(void){
	return get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_OFFSET, CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_RANGE);
}
uint16_t CAN_bms_status_2_HV_contactor_state_get(void){
	return get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_OFFSET, CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_RANGE);
}
uint16_t CAN_bms_status_2_HV_bus_voltage_get(void){
	return get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_BUS_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_HV_BUS_VOLTAGE_RANGE);
}

#define CAN_bms_cellVoltages_ID 0x725

static CAN_message_S CAN_bms_cellVoltages={
	.canID = CAN_bms_cellVoltages_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_bms_cellVoltages_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_bms_cellVoltages);
}
#define CAN_BMS_CELLVOLTAGES_MULTIPLEX_RANGE 8
#define CAN_BMS_CELLVOLTAGES_MULTIPLEX_OFFSET 0
#define CAN_BMS_CELLVOLTAGES_CELL1_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL1_OFFSET 8
#define CAN_BMS_CELLVOLTAGES_CELL2_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL2_OFFSET 20
#define CAN_BMS_CELLVOLTAGES_CELL3_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL3_OFFSET 32
#define CAN_BMS_CELLVOLTAGES_CELL4_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL4_OFFSET 44

uint16_t CAN_bms_cellVoltages_MultiPlex_get(void){
	return get_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_MULTIPLEX_OFFSET, CAN_BMS_CELLVOLTAGES_MULTIPLEX_RANGE);
}
uint16_t CAN_bms_cellVoltages_cell1_get(void){
	return get_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL1_OFFSET, CAN_BMS_CELLVOLTAGES_CELL1_RANGE);
}
uint16_t CAN_bms_cellVoltages_cell2_get(void){
	return get_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL2_OFFSET, CAN_BMS_CELLVOLTAGES_CELL2_RANGE);
}
uint16_t CAN_bms_cellVoltages_cell3_get(void){
	return get_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL3_OFFSET, CAN_BMS_CELLVOLTAGES_CELL3_RANGE);
}
uint16_t CAN_bms_cellVoltages_cell4_get(void){
	return get_bits((size_t*)CAN_bms_cellVoltages.payload, CAN_BMS_CELLVOLTAGES_CELL4_OFFSET, CAN_BMS_CELLVOLTAGES_CELL4_RANGE);
}

/**********************************************************
 * motorcontroller NODE MESSAGES
 */
/**********************************************************
 * charger NODE MESSAGES
 */
/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init(void) {
	CAN_configureMailbox(&CAN_mcu_status);
	CAN_configureMailbox(&CAN_mcu_command);
	CAN_configureMailbox(&CAN_bms_status);
	CAN_configureMailbox(&CAN_bms_status_2);
	CAN_configureMailbox(&CAN_bms_cellVoltages);
}

void CAN_send_10ms(void){
	CAN_dash_status_send();
	CAN_dash_command_send();
	CAN_dash_data1_send();
	CAN_dash_data2_send();
}
