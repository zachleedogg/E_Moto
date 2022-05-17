#include "mcu_dbc.h"
#include "utils.h"
/**********************************************************
 * dash NODE MESSAGES
 */
#define CAN_dash_status_ID 0x701

static CAN_message_S CAN_dash_status={
	.canID = CAN_dash_status_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_dash_status_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_dash_status);
}
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

uint16_t CAN_dash_status_heartBeat_get(void){
	return get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_HEARTBEAT_OFFSET, CAN_DASH_STATUS_HEARTBEAT_RANGE);
}
uint16_t CAN_dash_status_state_get(void){
	return get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_STATE_OFFSET, CAN_DASH_STATUS_STATE_RANGE);
}
uint16_t CAN_dash_status_killButton_get(void){
	return get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_KILLBUTTON_OFFSET, CAN_DASH_STATUS_KILLBUTTON_RANGE);
}
uint16_t CAN_dash_status_ignButton_get(void){
	return get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_IGNBUTTON_OFFSET, CAN_DASH_STATUS_IGNBUTTON_RANGE);
}
uint16_t CAN_dash_status_modeButton_get(void){
	return get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_MODEBUTTON_OFFSET, CAN_DASH_STATUS_MODEBUTTON_RANGE);
}
uint16_t CAN_dash_status_selectButton_get(void){
	return get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_SELECTBUTTON_OFFSET, CAN_DASH_STATUS_SELECTBUTTON_RANGE);
}
uint16_t CAN_dash_status_driveMode_get(void){
	return get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_DRIVEMODE_OFFSET, CAN_DASH_STATUS_DRIVEMODE_RANGE);
}

#define CAN_dash_command_ID 0x702

static CAN_message_S CAN_dash_command={
	.canID = CAN_dash_command_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_dash_command_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_dash_command);
}
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

uint16_t CAN_dash_command_ignitionRequest_get(void){
	return get_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_IGNITIONREQUEST_OFFSET, CAN_DASH_COMMAND_IGNITIONREQUEST_RANGE);
}
uint16_t CAN_dash_command_killRequest_get(void){
	return get_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_KILLREQUEST_OFFSET, CAN_DASH_COMMAND_KILLREQUEST_RANGE);
}
uint16_t CAN_dash_command_batteryEjectRequest_get(void){
	return get_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_BATTERYEJECTREQUEST_OFFSET, CAN_DASH_COMMAND_BATTERYEJECTREQUEST_RANGE);
}
uint16_t CAN_dash_command_lightsRequest_get(void){
	return get_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_LIGHTSREQUEST_OFFSET, CAN_DASH_COMMAND_LIGHTSREQUEST_RANGE);
}
uint16_t CAN_dash_command_hornRequest_get(void){
	return get_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_HORNREQUEST_OFFSET, CAN_DASH_COMMAND_HORNREQUEST_RANGE);
}

#define CAN_dash_data1_ID 0x1806e5f5

static CAN_message_S CAN_dash_data1={
	.canID = CAN_dash_data1_ID,
	.canXID = 0x1,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_dash_data1_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_dash_data1);
}
#define CAN_DASH_DATA1_SPEED_RANGE 16
#define CAN_DASH_DATA1_SPEED_OFFSET 0
#define CAN_DASH_DATA1_ODOMETER_RANGE 16
#define CAN_DASH_DATA1_ODOMETER_OFFSET 16
#define CAN_DASH_DATA1_TRIPA_RANGE 16
#define CAN_DASH_DATA1_TRIPA_OFFSET 32
#define CAN_DASH_DATA1_TRIPB_RANGE 16
#define CAN_DASH_DATA1_TRIPB_OFFSET 48

uint16_t CAN_dash_data1_speed_get(void){
	return get_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_SPEED_OFFSET, CAN_DASH_DATA1_SPEED_RANGE);
}
uint16_t CAN_dash_data1_odometer_get(void){
	return get_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_ODOMETER_OFFSET, CAN_DASH_DATA1_ODOMETER_RANGE);
}
uint16_t CAN_dash_data1_tripA_get(void){
	return get_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_TRIPA_OFFSET, CAN_DASH_DATA1_TRIPA_RANGE);
}
uint16_t CAN_dash_data1_tripB_get(void){
	return get_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_TRIPB_OFFSET, CAN_DASH_DATA1_TRIPB_RANGE);
}

#define CAN_dash_data2_ID 0x704

static CAN_message_S CAN_dash_data2={
	.canID = CAN_dash_data2_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_dash_data2_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_dash_data2);
}
#define CAN_DASH_DATA2_RUNNINGTIME_RANGE 16
#define CAN_DASH_DATA2_RUNNINGTIME_OFFSET 0
#define CAN_DASH_DATA2_ODOMETER_RANGE 16
#define CAN_DASH_DATA2_ODOMETER_OFFSET 16
#define CAN_DASH_DATA2_TRIPA_RANGE 16
#define CAN_DASH_DATA2_TRIPA_OFFSET 32
#define CAN_DASH_DATA2_TRIPB_RANGE 16
#define CAN_DASH_DATA2_TRIPB_OFFSET 48

uint16_t CAN_dash_data2_runningTime_get(void){
	return get_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_RUNNINGTIME_OFFSET, CAN_DASH_DATA2_RUNNINGTIME_RANGE);
}
uint16_t CAN_dash_data2_odometer_get(void){
	return get_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_ODOMETER_OFFSET, CAN_DASH_DATA2_ODOMETER_RANGE);
}
uint16_t CAN_dash_data2_tripA_get(void){
	return get_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_TRIPA_OFFSET, CAN_DASH_DATA2_TRIPA_RANGE);
}
uint16_t CAN_dash_data2_tripB_get(void){
	return get_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_TRIPB_OFFSET, CAN_DASH_DATA2_TRIPB_RANGE);
}

/**********************************************************
 * mcu NODE MESSAGES
 */
#define CAN_mcu_status_ID 0x711

static CAN_payload_S CAN_mcu_status_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_mcu_status={
	.canID = CAN_mcu_status_ID,
	.canXID = 0x0,
	.payload = &CAN_mcu_status_payload,
	.canMessageStatus = 0
};

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

void CAN_mcu_status_state_set(uint16_t state){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_STATE_OFFSET, CAN_MCU_STATUS_STATE_RANGE, state);
}
void CAN_mcu_status_throttleMode_set(uint16_t throttleMode){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEMODE_OFFSET, CAN_MCU_STATUS_THROTTLEMODE_RANGE, throttleMode);
}
void CAN_mcu_status_highBeam_set(uint16_t highBeam){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HIGHBEAM_OFFSET, CAN_MCU_STATUS_HIGHBEAM_RANGE, highBeam);
}
void CAN_mcu_status_lowBeam_set(uint16_t lowBeam){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_LOWBEAM_OFFSET, CAN_MCU_STATUS_LOWBEAM_RANGE, lowBeam);
}
void CAN_mcu_status_brakeLight_set(uint16_t brakeLight){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKELIGHT_OFFSET, CAN_MCU_STATUS_BRAKELIGHT_RANGE, brakeLight);
}
void CAN_mcu_status_tailLight_set(uint16_t tailLight){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TAILLIGHT_OFFSET, CAN_MCU_STATUS_TAILLIGHT_RANGE, tailLight);
}
void CAN_mcu_status_horn_set(uint16_t horn){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HORN_OFFSET, CAN_MCU_STATUS_HORN_RANGE, horn);
}
void CAN_mcu_status_turnSignalFR_set(uint16_t turnSignalFR){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFR_OFFSET, CAN_MCU_STATUS_TURNSIGNALFR_RANGE, turnSignalFR);
}
void CAN_mcu_status_turnSignalFL_set(uint16_t turnSignalFL){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFL_OFFSET, CAN_MCU_STATUS_TURNSIGNALFL_RANGE, turnSignalFL);
}
void CAN_mcu_status_turnSignalRR_set(uint16_t turnSignalRR){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRR_OFFSET, CAN_MCU_STATUS_TURNSIGNALRR_RANGE, turnSignalRR);
}
void CAN_mcu_status_turnSignalRL_set(uint16_t turnSignalRL){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRL_OFFSET, CAN_MCU_STATUS_TURNSIGNALRL_RANGE, turnSignalRL);
}
void CAN_mcu_status_GBSALL_set(uint16_t GBSALL){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_GBSALL_OFFSET, CAN_MCU_STATUS_GBSALL_RANGE, GBSALL);
}
void CAN_mcu_status_contactor_set(uint16_t contactor){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_CONTACTOR_OFFSET, CAN_MCU_STATUS_CONTACTOR_RANGE, contactor);
}
void CAN_mcu_status_chargePort_set(uint16_t chargePort){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_CHARGEPORT_OFFSET, CAN_MCU_STATUS_CHARGEPORT_RANGE, chargePort);
}
void CAN_mcu_status_brakeSwitchFront_set(uint16_t brakeSwitchFront){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHFRONT_OFFSET, CAN_MCU_STATUS_BRAKESWITCHFRONT_RANGE, brakeSwitchFront);
}
void CAN_mcu_status_brakeSwitchRear_set(uint16_t brakeSwitchRear){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHREAR_OFFSET, CAN_MCU_STATUS_BRAKESWITCHREAR_RANGE, brakeSwitchRear);
}
void CAN_mcu_status_throttleVal_set(uint16_t throttleVal){
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEVAL_OFFSET, CAN_MCU_STATUS_THROTTLEVAL_RANGE, throttleVal);
}
void CAN_mcu_status_send(void){
	CAN_write(CAN_mcu_status);
}

#define CAN_mcu_command_ID 0x712

static CAN_payload_S CAN_mcu_command_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_mcu_command={
	.canID = CAN_mcu_command_ID,
	.canXID = 0x0,
	.payload = &CAN_mcu_command_payload,
	.canMessageStatus = 0
};

#define CAN_MCU_COMMAND_DCDC_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_DCDC_ENABLE_OFFSET 0
#define CAN_MCU_COMMAND_EV_CHARGER_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_EV_CHARGER_ENABLE_OFFSET 1
#define CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_OFFSET 2

void CAN_mcu_command_DCDC_enable_set(uint16_t DCDC_enable){
	set_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_DCDC_ENABLE_OFFSET, CAN_MCU_COMMAND_DCDC_ENABLE_RANGE, DCDC_enable);
}
void CAN_mcu_command_ev_charger_enable_set(uint16_t ev_charger_enable){
	set_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_EV_CHARGER_ENABLE_OFFSET, CAN_MCU_COMMAND_EV_CHARGER_ENABLE_RANGE, ev_charger_enable);
}
void CAN_mcu_command_motor_controller_enable_set(uint16_t motor_controller_enable){
	set_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_OFFSET, CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_RANGE, motor_controller_enable);
}
void CAN_mcu_command_send(void){
	CAN_write(CAN_mcu_command);
}

#define CAN_mcu_motorControllerRequest_ID 0x700

static CAN_payload_S CAN_mcu_motorControllerRequest_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_mcu_motorControllerRequest={
	.canID = CAN_mcu_motorControllerRequest_ID,
	.canXID = 0x0,
	.payload = &CAN_mcu_motorControllerRequest_payload,
	.canMessageStatus = 0
};

#define CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_RANGE 8
#define CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_OFFSET 0

void CAN_mcu_motorControllerRequest_requestType_set(uint16_t requestType){
	set_bits((size_t*)CAN_mcu_motorControllerRequest.payload, CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_OFFSET, CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_RANGE, requestType);
}
void CAN_mcu_motorControllerRequest_send(void){
	CAN_write(CAN_mcu_motorControllerRequest);
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

/**********************************************************
 * motorcontroller NODE MESSAGES
 */
#define CAN_motorcontroller_motorStatus_ID 0x731

static CAN_message_S CAN_motorcontroller_motorStatus={
	.canID = CAN_motorcontroller_motorStatus_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_motorcontroller_motorStatus_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_motorcontroller_motorStatus);
}
#define CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORSPEED_RANGE 8
#define CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORSPEED_OFFSET 0
#define CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORCURRENT_RANGE 8
#define CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORCURRENT_OFFSET 8
#define CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEA_RANGE 8
#define CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEA_OFFSET 16
#define CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEB_RANGE 8
#define CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEB_OFFSET 24
#define CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEC_RANGE 8
#define CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEC_OFFSET 32
#define CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEA_RANGE 8
#define CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEA_OFFSET 40
#define CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEB_RANGE 8
#define CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEB_OFFSET 48
#define CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEC_RANGE 8
#define CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEC_OFFSET 56

uint16_t CAN_motorcontroller_motorStatus_motorSpeed_get(void){
	return get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORSPEED_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORSPEED_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_motorCurrent_get(void){
	return get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORCURRENT_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORCURRENT_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_IphaseA_get(void){
	return get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEA_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEA_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_IphaseB_get(void){
	return get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEB_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEB_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_IphaseC_get(void){
	return get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEC_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEC_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_VphaseA_get(void){
	return get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEA_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEA_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_VphaseB_get(void){
	return get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEB_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEB_RANGE);
}
uint16_t CAN_motorcontroller_motorStatus_VphaseC_get(void){
	return get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEC_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEC_RANGE);
}

#define CAN_motorcontroller_response_ID 0x700

static CAN_message_S CAN_motorcontroller_response={
	.canID = CAN_motorcontroller_response_ID,
	.canXID = 0x0,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_motorcontroller_response_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_motorcontroller_response);
}
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE1_RANGE 8
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE1_OFFSET 0
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE2_RANGE 8
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE2_OFFSET 8
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE3_RANGE 8
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE3_OFFSET 16
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE4_RANGE 8
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE4_OFFSET 24
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE5_RANGE 8
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE5_OFFSET 32
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE6_RANGE 8
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE6_OFFSET 40
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE7_RANGE 8
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE7_OFFSET 48
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE8_RANGE 8
#define CAN_MOTORCONTROLLER_RESPONSE_BYTE8_OFFSET 56

uint16_t CAN_motorcontroller_response_byte1_get(void){
	return get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE1_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE1_RANGE);
}
uint16_t CAN_motorcontroller_response_byte2_get(void){
	return get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE2_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE2_RANGE);
}
uint16_t CAN_motorcontroller_response_byte3_get(void){
	return get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE3_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE3_RANGE);
}
uint16_t CAN_motorcontroller_response_byte4_get(void){
	return get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE4_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE4_RANGE);
}
uint16_t CAN_motorcontroller_response_byte5_get(void){
	return get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE5_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE5_RANGE);
}
uint16_t CAN_motorcontroller_response_byte6_get(void){
	return get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE6_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE6_RANGE);
}
uint16_t CAN_motorcontroller_response_byte7_get(void){
	return get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE7_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE7_RANGE);
}
uint16_t CAN_motorcontroller_response_byte8_get(void){
	return get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE8_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE8_RANGE);
}

/**********************************************************
 * charger NODE MESSAGES
 */
/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init(void) {
	CAN_configureMailbox(&CAN_dash_status);
	CAN_configureMailbox(&CAN_dash_command);
	CAN_configureMailbox(&CAN_dash_data1);
	CAN_configureMailbox(&CAN_dash_data2);
	CAN_configureMailbox(&CAN_bms_status);
	CAN_configureMailbox(&CAN_bms_status_2);
	CAN_configureMailbox(&CAN_motorcontroller_motorStatus);
	CAN_configureMailbox(&CAN_motorcontroller_response);
}

void CAN_send_10ms(void){
	CAN_mcu_status_send();
	CAN_mcu_command_send();
}

void CAN_send_1ms(void){
	CAN_mcu_motorControllerRequest_send();
}
