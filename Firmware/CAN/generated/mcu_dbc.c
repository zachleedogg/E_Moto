#include "mcu_dbc.h"
#include "utils.h"
/**********************************************************
 * dash NODE MESSAGES
 */
#define CAN_dash_status_ID 0x701

static CAN_message_S CAN_dash_status={
	.canID = CAN_dash_status_ID,
	.canXID = 0,
	.dlc = 8,
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
	uint16_t data = get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_HEARTBEAT_OFFSET, CAN_DASH_STATUS_HEARTBEAT_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_status_state_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_STATE_OFFSET, CAN_DASH_STATUS_STATE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_status_killButton_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_KILLBUTTON_OFFSET, CAN_DASH_STATUS_KILLBUTTON_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_status_ignButton_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_IGNBUTTON_OFFSET, CAN_DASH_STATUS_IGNBUTTON_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_status_modeButton_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_MODEBUTTON_OFFSET, CAN_DASH_STATUS_MODEBUTTON_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_status_selectButton_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_SELECTBUTTON_OFFSET, CAN_DASH_STATUS_SELECTBUTTON_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_status_driveMode_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_status.payload, CAN_DASH_STATUS_DRIVEMODE_OFFSET, CAN_DASH_STATUS_DRIVEMODE_RANGE);
	return (data * 1.0) + 0;
}

#define CAN_dash_command_ID 0x702

static CAN_message_S CAN_dash_command={
	.canID = CAN_dash_command_ID,
	.canXID = 0,
	.dlc = 8,
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
	uint16_t data = get_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_IGNITIONREQUEST_OFFSET, CAN_DASH_COMMAND_IGNITIONREQUEST_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_command_killRequest_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_KILLREQUEST_OFFSET, CAN_DASH_COMMAND_KILLREQUEST_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_command_batteryEjectRequest_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_BATTERYEJECTREQUEST_OFFSET, CAN_DASH_COMMAND_BATTERYEJECTREQUEST_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_command_lightsRequest_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_LIGHTSREQUEST_OFFSET, CAN_DASH_COMMAND_LIGHTSREQUEST_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_command_hornRequest_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_command.payload, CAN_DASH_COMMAND_HORNREQUEST_OFFSET, CAN_DASH_COMMAND_HORNREQUEST_RANGE);
	return (data * 1.0) + 0;
}

#define CAN_dash_data1_ID 0x1806e5f5

static CAN_message_S CAN_dash_data1={
	.canID = CAN_dash_data1_ID,
	.canXID = 1,
	.dlc = 8,
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
	uint16_t data = get_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_SPEED_OFFSET, CAN_DASH_DATA1_SPEED_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_data1_odometer_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_ODOMETER_OFFSET, CAN_DASH_DATA1_ODOMETER_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_data1_tripA_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_TRIPA_OFFSET, CAN_DASH_DATA1_TRIPA_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_data1_tripB_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_data1.payload, CAN_DASH_DATA1_TRIPB_OFFSET, CAN_DASH_DATA1_TRIPB_RANGE);
	return (data * 1.0) + 0;
}

#define CAN_dash_data2_ID 0x704

static CAN_message_S CAN_dash_data2={
	.canID = CAN_dash_data2_ID,
	.canXID = 0,
	.dlc = 8,
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
	uint16_t data = get_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_RUNNINGTIME_OFFSET, CAN_DASH_DATA2_RUNNINGTIME_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_data2_odometer_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_ODOMETER_OFFSET, CAN_DASH_DATA2_ODOMETER_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_data2_tripA_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_TRIPA_OFFSET, CAN_DASH_DATA2_TRIPA_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_dash_data2_tripB_get(void){
	uint16_t data = get_bits((size_t*)CAN_dash_data2.payload, CAN_DASH_DATA2_TRIPB_OFFSET, CAN_DASH_DATA2_TRIPB_RANGE);
	return (data * 1.0) + 0;
}

/**********************************************************
 * mcu NODE MESSAGES
 */
#define CAN_mcu_status_ID 0x711

static CAN_payload_S CAN_mcu_status_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_mcu_status={
	.canID = CAN_mcu_status_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = &CAN_mcu_status_payload,
	.canMessageStatus = 0
};

#define CAN_MCU_STATUS_HEARTBEAT_RANGE 4
#define CAN_MCU_STATUS_HEARTBEAT_OFFSET 0
#define CAN_MCU_STATUS_STATE_RANGE 3
#define CAN_MCU_STATUS_STATE_OFFSET 4
#define CAN_MCU_STATUS_HIGHBEAM_RANGE 1
#define CAN_MCU_STATUS_HIGHBEAM_OFFSET 7
#define CAN_MCU_STATUS_LOWBEAM_RANGE 1
#define CAN_MCU_STATUS_LOWBEAM_OFFSET 8
#define CAN_MCU_STATUS_BRAKELIGHT_RANGE 1
#define CAN_MCU_STATUS_BRAKELIGHT_OFFSET 9
#define CAN_MCU_STATUS_TAILLIGHT_RANGE 1
#define CAN_MCU_STATUS_TAILLIGHT_OFFSET 10
#define CAN_MCU_STATUS_HORN_RANGE 1
#define CAN_MCU_STATUS_HORN_OFFSET 11
#define CAN_MCU_STATUS_TURNSIGNALFR_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALFR_OFFSET 12
#define CAN_MCU_STATUS_TURNSIGNALFL_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALFL_OFFSET 13
#define CAN_MCU_STATUS_TURNSIGNALRR_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALRR_OFFSET 14
#define CAN_MCU_STATUS_TURNSIGNALRL_RANGE 1
#define CAN_MCU_STATUS_TURNSIGNALRL_OFFSET 15
#define CAN_MCU_STATUS_GBSALL_RANGE 1
#define CAN_MCU_STATUS_GBSALL_OFFSET 16
#define CAN_MCU_STATUS_CONTACTOR_RANGE 1
#define CAN_MCU_STATUS_CONTACTOR_OFFSET 17
#define CAN_MCU_STATUS_CHARGEPORT_RANGE 1
#define CAN_MCU_STATUS_CHARGEPORT_OFFSET 18
#define CAN_MCU_STATUS_BRAKESWITCHFRONT_RANGE 1
#define CAN_MCU_STATUS_BRAKESWITCHFRONT_OFFSET 19
#define CAN_MCU_STATUS_BRAKESWITCHREAR_RANGE 1
#define CAN_MCU_STATUS_BRAKESWITCHREAR_OFFSET 20
#define CAN_MCU_STATUS_THROTTLEVAL_RANGE 8
#define CAN_MCU_STATUS_THROTTLEVAL_OFFSET 21
#define CAN_MCU_STATUS_KILLSWITCH_RANGE 1
#define CAN_MCU_STATUS_KILLSWITCH_OFFSET 29
#define CAN_MCU_STATUS_IGNITIONSWITCH_RANGE 1
#define CAN_MCU_STATUS_IGNITIONSWITCH_OFFSET 30
#define CAN_MCU_STATUS_LEFTTURNSWITCH_RANGE 1
#define CAN_MCU_STATUS_LEFTTURNSWITCH_OFFSET 31
#define CAN_MCU_STATUS_RIGHTTURNSWITCH_RANGE 1
#define CAN_MCU_STATUS_RIGHTTURNSWITCH_OFFSET 32
#define CAN_MCU_STATUS_LIGHTSWITCH_RANGE 1
#define CAN_MCU_STATUS_LIGHTSWITCH_OFFSET 33
#define CAN_MCU_STATUS_ASSSWITCH_RANGE 1
#define CAN_MCU_STATUS_ASSSWITCH_OFFSET 34
#define CAN_MCU_STATUS_HORNSWITCH_RANGE 1
#define CAN_MCU_STATUS_HORNSWITCH_OFFSET 35
#define CAN_MCU_STATUS_BATT_VOLTAGE_RANGE 8
#define CAN_MCU_STATUS_BATT_VOLTAGE_OFFSET 36

void CAN_mcu_status_heartbeat_set(uint16_t heartbeat){
	uint16_t data_scaled = (heartbeat - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HEARTBEAT_OFFSET, CAN_MCU_STATUS_HEARTBEAT_RANGE, data_scaled);
}
void CAN_mcu_status_state_set(uint16_t state){
	uint16_t data_scaled = (state - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_STATE_OFFSET, CAN_MCU_STATUS_STATE_RANGE, data_scaled);
}
void CAN_mcu_status_highBeam_set(uint16_t highBeam){
	uint16_t data_scaled = (highBeam - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HIGHBEAM_OFFSET, CAN_MCU_STATUS_HIGHBEAM_RANGE, data_scaled);
}
void CAN_mcu_status_lowBeam_set(uint16_t lowBeam){
	uint16_t data_scaled = (lowBeam - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_LOWBEAM_OFFSET, CAN_MCU_STATUS_LOWBEAM_RANGE, data_scaled);
}
void CAN_mcu_status_brakeLight_set(uint16_t brakeLight){
	uint16_t data_scaled = (brakeLight - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKELIGHT_OFFSET, CAN_MCU_STATUS_BRAKELIGHT_RANGE, data_scaled);
}
void CAN_mcu_status_tailLight_set(uint16_t tailLight){
	uint16_t data_scaled = (tailLight - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TAILLIGHT_OFFSET, CAN_MCU_STATUS_TAILLIGHT_RANGE, data_scaled);
}
void CAN_mcu_status_horn_set(uint16_t horn){
	uint16_t data_scaled = (horn - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HORN_OFFSET, CAN_MCU_STATUS_HORN_RANGE, data_scaled);
}
void CAN_mcu_status_turnSignalFR_set(uint16_t turnSignalFR){
	uint16_t data_scaled = (turnSignalFR - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFR_OFFSET, CAN_MCU_STATUS_TURNSIGNALFR_RANGE, data_scaled);
}
void CAN_mcu_status_turnSignalFL_set(uint16_t turnSignalFL){
	uint16_t data_scaled = (turnSignalFL - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALFL_OFFSET, CAN_MCU_STATUS_TURNSIGNALFL_RANGE, data_scaled);
}
void CAN_mcu_status_turnSignalRR_set(uint16_t turnSignalRR){
	uint16_t data_scaled = (turnSignalRR - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRR_OFFSET, CAN_MCU_STATUS_TURNSIGNALRR_RANGE, data_scaled);
}
void CAN_mcu_status_turnSignalRL_set(uint16_t turnSignalRL){
	uint16_t data_scaled = (turnSignalRL - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_TURNSIGNALRL_OFFSET, CAN_MCU_STATUS_TURNSIGNALRL_RANGE, data_scaled);
}
void CAN_mcu_status_GBSALL_set(uint16_t GBSALL){
	uint16_t data_scaled = (GBSALL - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_GBSALL_OFFSET, CAN_MCU_STATUS_GBSALL_RANGE, data_scaled);
}
void CAN_mcu_status_contactor_set(uint16_t contactor){
	uint16_t data_scaled = (contactor - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_CONTACTOR_OFFSET, CAN_MCU_STATUS_CONTACTOR_RANGE, data_scaled);
}
void CAN_mcu_status_chargePort_set(uint16_t chargePort){
	uint16_t data_scaled = (chargePort - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_CHARGEPORT_OFFSET, CAN_MCU_STATUS_CHARGEPORT_RANGE, data_scaled);
}
void CAN_mcu_status_brakeSwitchFront_set(uint16_t brakeSwitchFront){
	uint16_t data_scaled = (brakeSwitchFront - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHFRONT_OFFSET, CAN_MCU_STATUS_BRAKESWITCHFRONT_RANGE, data_scaled);
}
void CAN_mcu_status_brakeSwitchRear_set(uint16_t brakeSwitchRear){
	uint16_t data_scaled = (brakeSwitchRear - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BRAKESWITCHREAR_OFFSET, CAN_MCU_STATUS_BRAKESWITCHREAR_RANGE, data_scaled);
}
void CAN_mcu_status_throttleVal_set(uint16_t throttleVal){
	uint16_t data_scaled = (throttleVal - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_THROTTLEVAL_OFFSET, CAN_MCU_STATUS_THROTTLEVAL_RANGE, data_scaled);
}
void CAN_mcu_status_killSwitch_set(uint16_t killSwitch){
	uint16_t data_scaled = (killSwitch - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_KILLSWITCH_OFFSET, CAN_MCU_STATUS_KILLSWITCH_RANGE, data_scaled);
}
void CAN_mcu_status_ignitionSwitch_set(uint16_t ignitionSwitch){
	uint16_t data_scaled = (ignitionSwitch - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_IGNITIONSWITCH_OFFSET, CAN_MCU_STATUS_IGNITIONSWITCH_RANGE, data_scaled);
}
void CAN_mcu_status_leftTurnSwitch_set(uint16_t leftTurnSwitch){
	uint16_t data_scaled = (leftTurnSwitch - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_LEFTTURNSWITCH_OFFSET, CAN_MCU_STATUS_LEFTTURNSWITCH_RANGE, data_scaled);
}
void CAN_mcu_status_rightTurnSwitch_set(uint16_t rightTurnSwitch){
	uint16_t data_scaled = (rightTurnSwitch - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_RIGHTTURNSWITCH_OFFSET, CAN_MCU_STATUS_RIGHTTURNSWITCH_RANGE, data_scaled);
}
void CAN_mcu_status_lightSwitch_set(uint16_t lightSwitch){
	uint16_t data_scaled = (lightSwitch - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_LIGHTSWITCH_OFFSET, CAN_MCU_STATUS_LIGHTSWITCH_RANGE, data_scaled);
}
void CAN_mcu_status_assSwitch_set(uint16_t assSwitch){
	uint16_t data_scaled = (assSwitch - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_ASSSWITCH_OFFSET, CAN_MCU_STATUS_ASSSWITCH_RANGE, data_scaled);
}
void CAN_mcu_status_hornSwitch_set(uint16_t hornSwitch){
	uint16_t data_scaled = (hornSwitch - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_HORNSWITCH_OFFSET, CAN_MCU_STATUS_HORNSWITCH_RANGE, data_scaled);
}
void CAN_mcu_status_batt_voltage_set(float batt_voltage){
	uint16_t data_scaled = (batt_voltage - 0) / 0.1;
	set_bits((size_t*)CAN_mcu_status.payload, CAN_MCU_STATUS_BATT_VOLTAGE_OFFSET, CAN_MCU_STATUS_BATT_VOLTAGE_RANGE, data_scaled);
}
void CAN_mcu_status_dlc_set(uint8_t dlc){
	CAN_mcu_status.dlc = dlc;
}
void CAN_mcu_status_send(void){
	CAN_write(CAN_mcu_status);
}

#define CAN_mcu_command_ID 0x712

static CAN_payload_S CAN_mcu_command_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_mcu_command={
	.canID = CAN_mcu_command_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = &CAN_mcu_command_payload,
	.canMessageStatus = 0
};

#define CAN_MCU_COMMAND_DCDC_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_DCDC_ENABLE_OFFSET 0
#define CAN_MCU_COMMAND_EV_CHARGER_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_EV_CHARGER_ENABLE_OFFSET 1
#define CAN_MCU_COMMAND_EV_CHARGER_CURRENT_RANGE 13
#define CAN_MCU_COMMAND_EV_CHARGER_CURRENT_OFFSET 2
#define CAN_MCU_COMMAND_PRECHARGE_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_PRECHARGE_ENABLE_OFFSET 15
#define CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_RANGE 1
#define CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_OFFSET 16

void CAN_mcu_command_DCDC_enable_set(uint16_t DCDC_enable){
	uint16_t data_scaled = (DCDC_enable - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_DCDC_ENABLE_OFFSET, CAN_MCU_COMMAND_DCDC_ENABLE_RANGE, data_scaled);
}
void CAN_mcu_command_ev_charger_enable_set(uint16_t ev_charger_enable){
	uint16_t data_scaled = (ev_charger_enable - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_EV_CHARGER_ENABLE_OFFSET, CAN_MCU_COMMAND_EV_CHARGER_ENABLE_RANGE, data_scaled);
}
void CAN_mcu_command_ev_charger_current_set(float ev_charger_current){
	uint16_t data_scaled = (ev_charger_current - 0) / 0.1;
	set_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_EV_CHARGER_CURRENT_OFFSET, CAN_MCU_COMMAND_EV_CHARGER_CURRENT_RANGE, data_scaled);
}
void CAN_mcu_command_precharge_enable_set(uint16_t precharge_enable){
	uint16_t data_scaled = (precharge_enable - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_PRECHARGE_ENABLE_OFFSET, CAN_MCU_COMMAND_PRECHARGE_ENABLE_RANGE, data_scaled);
}
void CAN_mcu_command_motor_controller_enable_set(uint16_t motor_controller_enable){
	uint16_t data_scaled = (motor_controller_enable - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_command.payload, CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_OFFSET, CAN_MCU_COMMAND_MOTOR_CONTROLLER_ENABLE_RANGE, data_scaled);
}
void CAN_mcu_command_dlc_set(uint8_t dlc){
	CAN_mcu_command.dlc = dlc;
}
void CAN_mcu_command_send(void){
	CAN_write(CAN_mcu_command);
}

#define CAN_mcu_motorControllerRequest_ID 0x700

static CAN_payload_S CAN_mcu_motorControllerRequest_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_mcu_motorControllerRequest={
	.canID = CAN_mcu_motorControllerRequest_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = &CAN_mcu_motorControllerRequest_payload,
	.canMessageStatus = 0
};

#define CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_RANGE 8
#define CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_OFFSET 0

void CAN_mcu_motorControllerRequest_requestType_set(uint16_t requestType){
	uint16_t data_scaled = (requestType - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_motorControllerRequest.payload, CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_OFFSET, CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_RANGE, data_scaled);
}
void CAN_mcu_motorControllerRequest_dlc_set(uint8_t dlc){
	CAN_mcu_motorControllerRequest.dlc = dlc;
}
void CAN_mcu_motorControllerRequest_send(void){
	CAN_write(CAN_mcu_motorControllerRequest);
}

#define CAN_mcu_boot_response_ID 0xa2

static CAN_payload_S CAN_mcu_boot_response_payload __attribute__((aligned(sizeof(CAN_payload_S))));
static CAN_message_S CAN_mcu_boot_response={
	.canID = CAN_mcu_boot_response_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = &CAN_mcu_boot_response_payload,
	.canMessageStatus = 0
};

#define CAN_MCU_BOOT_RESPONSE_CODE_RANGE 4
#define CAN_MCU_BOOT_RESPONSE_CODE_OFFSET 0
#define CAN_MCU_BOOT_RESPONSE_TYPE_RANGE 4
#define CAN_MCU_BOOT_RESPONSE_TYPE_OFFSET 4
#define CAN_MCU_BOOT_RESPONSE_BYTE1_RANGE 8
#define CAN_MCU_BOOT_RESPONSE_BYTE1_OFFSET 8
#define CAN_MCU_BOOT_RESPONSE_BYTE2_RANGE 8
#define CAN_MCU_BOOT_RESPONSE_BYTE2_OFFSET 16
#define CAN_MCU_BOOT_RESPONSE_BYTE3_RANGE 8
#define CAN_MCU_BOOT_RESPONSE_BYTE3_OFFSET 24
#define CAN_MCU_BOOT_RESPONSE_BYTE4_RANGE 8
#define CAN_MCU_BOOT_RESPONSE_BYTE4_OFFSET 32
#define CAN_MCU_BOOT_RESPONSE_BYTE5_RANGE 8
#define CAN_MCU_BOOT_RESPONSE_BYTE5_OFFSET 40
#define CAN_MCU_BOOT_RESPONSE_BYTE6_RANGE 8
#define CAN_MCU_BOOT_RESPONSE_BYTE6_OFFSET 48
#define CAN_MCU_BOOT_RESPONSE_BYTE7_RANGE 8
#define CAN_MCU_BOOT_RESPONSE_BYTE7_OFFSET 56

void CAN_mcu_boot_response_code_set(uint16_t code){
	uint16_t data_scaled = (code - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_boot_response.payload, CAN_MCU_BOOT_RESPONSE_CODE_OFFSET, CAN_MCU_BOOT_RESPONSE_CODE_RANGE, data_scaled);
}
void CAN_mcu_boot_response_type_set(uint16_t type){
	uint16_t data_scaled = (type - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_boot_response.payload, CAN_MCU_BOOT_RESPONSE_TYPE_OFFSET, CAN_MCU_BOOT_RESPONSE_TYPE_RANGE, data_scaled);
}
void CAN_mcu_boot_response_byte1_set(uint16_t byte1){
	uint16_t data_scaled = (byte1 - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_boot_response.payload, CAN_MCU_BOOT_RESPONSE_BYTE1_OFFSET, CAN_MCU_BOOT_RESPONSE_BYTE1_RANGE, data_scaled);
}
void CAN_mcu_boot_response_byte2_set(uint16_t byte2){
	uint16_t data_scaled = (byte2 - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_boot_response.payload, CAN_MCU_BOOT_RESPONSE_BYTE2_OFFSET, CAN_MCU_BOOT_RESPONSE_BYTE2_RANGE, data_scaled);
}
void CAN_mcu_boot_response_byte3_set(uint16_t byte3){
	uint16_t data_scaled = (byte3 - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_boot_response.payload, CAN_MCU_BOOT_RESPONSE_BYTE3_OFFSET, CAN_MCU_BOOT_RESPONSE_BYTE3_RANGE, data_scaled);
}
void CAN_mcu_boot_response_byte4_set(uint16_t byte4){
	uint16_t data_scaled = (byte4 - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_boot_response.payload, CAN_MCU_BOOT_RESPONSE_BYTE4_OFFSET, CAN_MCU_BOOT_RESPONSE_BYTE4_RANGE, data_scaled);
}
void CAN_mcu_boot_response_byte5_set(uint16_t byte5){
	uint16_t data_scaled = (byte5 - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_boot_response.payload, CAN_MCU_BOOT_RESPONSE_BYTE5_OFFSET, CAN_MCU_BOOT_RESPONSE_BYTE5_RANGE, data_scaled);
}
void CAN_mcu_boot_response_byte6_set(uint16_t byte6){
	uint16_t data_scaled = (byte6 - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_boot_response.payload, CAN_MCU_BOOT_RESPONSE_BYTE6_OFFSET, CAN_MCU_BOOT_RESPONSE_BYTE6_RANGE, data_scaled);
}
void CAN_mcu_boot_response_byte7_set(uint16_t byte7){
	uint16_t data_scaled = (byte7 - 0) / 1.0;
	set_bits((size_t*)CAN_mcu_boot_response.payload, CAN_MCU_BOOT_RESPONSE_BYTE7_OFFSET, CAN_MCU_BOOT_RESPONSE_BYTE7_RANGE, data_scaled);
}
void CAN_mcu_boot_response_dlc_set(uint8_t dlc){
	CAN_mcu_boot_response.dlc = dlc;
}
void CAN_mcu_boot_response_send(void){
	CAN_write(CAN_mcu_boot_response);
}

/**********************************************************
 * bms NODE MESSAGES
 */
#define CAN_bms_status_ID 0x721

static CAN_message_S CAN_bms_status={
	.canID = CAN_bms_status_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_bms_status_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_bms_status);
}
#define CAN_BMS_STATUS_STATE_RANGE 3
#define CAN_BMS_STATUS_STATE_OFFSET 0
#define CAN_BMS_STATUS_SOC_RANGE 9
#define CAN_BMS_STATUS_SOC_OFFSET 3
#define CAN_BMS_STATUS_PACKVOLTAGE_RANGE 16
#define CAN_BMS_STATUS_PACKVOLTAGE_OFFSET 12
#define CAN_BMS_STATUS_PACKCURRENT_RANGE 16
#define CAN_BMS_STATUS_PACKCURRENT_OFFSET 28
#define CAN_BMS_STATUS_MINTEMP_RANGE 10
#define CAN_BMS_STATUS_MINTEMP_OFFSET 44
#define CAN_BMS_STATUS_MAXTEMP_RANGE 10
#define CAN_BMS_STATUS_MAXTEMP_OFFSET 54

uint16_t CAN_bms_status_state_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_STATE_OFFSET, CAN_BMS_STATUS_STATE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_status_SOC_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_SOC_OFFSET, CAN_BMS_STATUS_SOC_RANGE);
	return (data * 0.1) + 0;
}
float CAN_bms_status_packVoltage_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_PACKVOLTAGE_OFFSET, CAN_BMS_STATUS_PACKVOLTAGE_RANGE);
	return (data * 0.01) + 0;
}
float CAN_bms_status_packCurrent_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_PACKCURRENT_OFFSET, CAN_BMS_STATUS_PACKCURRENT_RANGE);
	return (data * 0.01) + 0;
}
float CAN_bms_status_minTemp_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_MINTEMP_OFFSET, CAN_BMS_STATUS_MINTEMP_RANGE);
	return (data * 0.1) + -40;
}
float CAN_bms_status_maxTemp_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status.payload, CAN_BMS_STATUS_MAXTEMP_OFFSET, CAN_BMS_STATUS_MAXTEMP_RANGE);
	return (data * 0.1) + -40;
}

#define CAN_bms_status_2_ID 0x722

static CAN_message_S CAN_bms_status_2={
	.canID = CAN_bms_status_2_ID,
	.canXID = 0,
	.dlc = 8,
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
#define CAN_BMS_STATUS_2_DCDC_VOLTAGE_RANGE 10
#define CAN_BMS_STATUS_2_DCDC_VOLTAGE_OFFSET 2
#define CAN_BMS_STATUS_2_DCDC_CURRENT_RANGE 10
#define CAN_BMS_STATUS_2_DCDC_CURRENT_OFFSET 12
#define CAN_BMS_STATUS_2_EV_CHARGER_STATE_RANGE 1
#define CAN_BMS_STATUS_2_EV_CHARGER_STATE_OFFSET 22
#define CAN_BMS_STATUS_2_EV_CHARGER_FAULT_RANGE 1
#define CAN_BMS_STATUS_2_EV_CHARGER_FAULT_OFFSET 23
#define CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_RANGE 10
#define CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_OFFSET 24
#define CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_RANGE 10
#define CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_OFFSET 34
#define CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_RANGE 1
#define CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_OFFSET 44
#define CAN_BMS_STATUS_2_HV_ISOLATION_VOLTAGE_RANGE 10
#define CAN_BMS_STATUS_2_HV_ISOLATION_VOLTAGE_OFFSET 45
#define CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_RANGE 1
#define CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_OFFSET 55

uint16_t CAN_bms_status_2_DCDC_state_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_STATE_OFFSET, CAN_BMS_STATUS_2_DCDC_STATE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_status_2_DCDC_fault_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_FAULT_OFFSET, CAN_BMS_STATUS_2_DCDC_FAULT_RANGE);
	return (data * 1.0) + 0;
}
float CAN_bms_status_2_DCDC_voltage_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_DCDC_VOLTAGE_RANGE);
	return (data * 0.1) + 0;
}
float CAN_bms_status_2_DCDC_current_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_DCDC_CURRENT_OFFSET, CAN_BMS_STATUS_2_DCDC_CURRENT_RANGE);
	return (data * 0.1) + 0;
}
uint16_t CAN_bms_status_2_EV_charger_state_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_STATE_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_STATE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_status_2_EV_charger_fault_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_FAULT_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_FAULT_RANGE);
	return (data * 1.0) + 0;
}
float CAN_bms_status_2_EV_charger_voltage_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_VOLTAGE_RANGE);
	return (data * 0.1) + 0;
}
float CAN_bms_status_2_EV_charger_current_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_OFFSET, CAN_BMS_STATUS_2_EV_CHARGER_CURRENT_RANGE);
	return (data * 0.1) + 0;
}
uint16_t CAN_bms_status_2_HV_precharge_state_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_OFFSET, CAN_BMS_STATUS_2_HV_PRECHARGE_STATE_RANGE);
	return (data * 1.0) + 0;
}
float CAN_bms_status_2_HV_isolation_voltage_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_ISOLATION_VOLTAGE_OFFSET, CAN_BMS_STATUS_2_HV_ISOLATION_VOLTAGE_RANGE);
	return (data * 0.1) + 0;
}
uint16_t CAN_bms_status_2_HV_contactor_state_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_status_2.payload, CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_OFFSET, CAN_BMS_STATUS_2_HV_CONTACTOR_STATE_RANGE);
	return (data * 1.0) + 0;
}

#define CAN_bms_debug_ID 0x723

static CAN_message_S CAN_bms_debug={
	.canID = CAN_bms_debug_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_bms_debug_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_bms_debug);
}
#define CAN_BMS_DEBUG_BOOL0_RANGE 1
#define CAN_BMS_DEBUG_BOOL0_OFFSET 0
#define CAN_BMS_DEBUG_BOOL1_RANGE 1
#define CAN_BMS_DEBUG_BOOL1_OFFSET 1
#define CAN_BMS_DEBUG_BOOL2_RANGE 1
#define CAN_BMS_DEBUG_BOOL2_OFFSET 2
#define CAN_BMS_DEBUG_BOOL3_RANGE 1
#define CAN_BMS_DEBUG_BOOL3_OFFSET 3
#define CAN_BMS_DEBUG_FLOAT1_RANGE 16
#define CAN_BMS_DEBUG_FLOAT1_OFFSET 4
#define CAN_BMS_DEBUG_FLOAT2_RANGE 16
#define CAN_BMS_DEBUG_FLOAT2_OFFSET 20

uint16_t CAN_bms_debug_bool0_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL0_OFFSET, CAN_BMS_DEBUG_BOOL0_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_debug_bool1_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL1_OFFSET, CAN_BMS_DEBUG_BOOL1_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_debug_bool2_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL2_OFFSET, CAN_BMS_DEBUG_BOOL2_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_bms_debug_bool3_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_BOOL3_OFFSET, CAN_BMS_DEBUG_BOOL3_RANGE);
	return (data * 1.0) + 0;
}
float CAN_bms_debug_float1_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_FLOAT1_OFFSET, CAN_BMS_DEBUG_FLOAT1_RANGE);
	return (data * 0.01) + 0;
}
float CAN_bms_debug_float2_get(void){
	uint16_t data = get_bits((size_t*)CAN_bms_debug.payload, CAN_BMS_DEBUG_FLOAT2_OFFSET, CAN_BMS_DEBUG_FLOAT2_RANGE);
	return (data * 0.01) + 0;
}

/**********************************************************
 * motorcontroller NODE MESSAGES
 */
#define CAN_motorcontroller_motorStatus_ID 0x731

static CAN_message_S CAN_motorcontroller_motorStatus={
	.canID = CAN_motorcontroller_motorStatus_ID,
	.canXID = 0,
	.dlc = 8,
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
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORSPEED_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORSPEED_RANGE);
	return (data * 1.0) + 0;
}
float CAN_motorcontroller_motorStatus_motorCurrent_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORCURRENT_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_MOTORCURRENT_RANGE);
	return (data * 0.01) + 0;
}
uint16_t CAN_motorcontroller_motorStatus_IphaseA_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEA_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEA_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_motorStatus_IphaseB_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEB_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEB_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_motorStatus_IphaseC_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEC_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_IPHASEC_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_motorStatus_VphaseA_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEA_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEA_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_motorStatus_VphaseB_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEB_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEB_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_motorStatus_VphaseC_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_motorStatus.payload, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEC_OFFSET, CAN_MOTORCONTROLLER_MOTORSTATUS_VPHASEC_RANGE);
	return (data * 1.0) + 0;
}

#define CAN_motorcontroller_response_ID 0x6ff

static CAN_message_S CAN_motorcontroller_response={
	.canID = CAN_motorcontroller_response_ID,
	.canXID = 0,
	.dlc = 8,
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
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE1_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE1_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_response_byte2_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE2_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE2_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_response_byte3_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE3_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE3_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_response_byte4_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE4_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE4_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_response_byte5_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE5_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE5_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_response_byte6_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE6_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE6_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_response_byte7_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE7_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE7_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_motorcontroller_response_byte8_get(void){
	uint16_t data = get_bits((size_t*)CAN_motorcontroller_response.payload, CAN_MOTORCONTROLLER_RESPONSE_BYTE8_OFFSET, CAN_MOTORCONTROLLER_RESPONSE_BYTE8_RANGE);
	return (data * 1.0) + 0;
}

/**********************************************************
 * charger NODE MESSAGES
 */
/**********************************************************
 * boot_host NODE MESSAGES
 */
#define CAN_boot_host_mcu_ID 0xa3

static CAN_message_S CAN_boot_host_mcu={
	.canID = CAN_boot_host_mcu_ID,
	.canXID = 0,
	.dlc = 8,
	.payload = 0,
	.canMessageStatus = 0
};

uint8_t CAN_boot_host_mcu_checkDataIsFresh(void){
	return CAN_checkDataIsFresh(&CAN_boot_host_mcu);
}
#define CAN_BOOT_HOST_MCU_CODE_RANGE 4
#define CAN_BOOT_HOST_MCU_CODE_OFFSET 0
#define CAN_BOOT_HOST_MCU_TYPE_RANGE 4
#define CAN_BOOT_HOST_MCU_TYPE_OFFSET 4
#define CAN_BOOT_HOST_MCU_BYTE1_RANGE 8
#define CAN_BOOT_HOST_MCU_BYTE1_OFFSET 8
#define CAN_BOOT_HOST_MCU_BYTE2_RANGE 8
#define CAN_BOOT_HOST_MCU_BYTE2_OFFSET 16
#define CAN_BOOT_HOST_MCU_BYTE3_RANGE 8
#define CAN_BOOT_HOST_MCU_BYTE3_OFFSET 24
#define CAN_BOOT_HOST_MCU_BYTE4_RANGE 8
#define CAN_BOOT_HOST_MCU_BYTE4_OFFSET 32
#define CAN_BOOT_HOST_MCU_BYTE5_RANGE 8
#define CAN_BOOT_HOST_MCU_BYTE5_OFFSET 40
#define CAN_BOOT_HOST_MCU_BYTE6_RANGE 8
#define CAN_BOOT_HOST_MCU_BYTE6_OFFSET 48
#define CAN_BOOT_HOST_MCU_BYTE7_RANGE 8
#define CAN_BOOT_HOST_MCU_BYTE7_OFFSET 56

uint16_t CAN_boot_host_mcu_code_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_mcu.payload, CAN_BOOT_HOST_MCU_CODE_OFFSET, CAN_BOOT_HOST_MCU_CODE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_mcu_type_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_mcu.payload, CAN_BOOT_HOST_MCU_TYPE_OFFSET, CAN_BOOT_HOST_MCU_TYPE_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_mcu_byte1_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_mcu.payload, CAN_BOOT_HOST_MCU_BYTE1_OFFSET, CAN_BOOT_HOST_MCU_BYTE1_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_mcu_byte2_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_mcu.payload, CAN_BOOT_HOST_MCU_BYTE2_OFFSET, CAN_BOOT_HOST_MCU_BYTE2_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_mcu_byte3_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_mcu.payload, CAN_BOOT_HOST_MCU_BYTE3_OFFSET, CAN_BOOT_HOST_MCU_BYTE3_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_mcu_byte4_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_mcu.payload, CAN_BOOT_HOST_MCU_BYTE4_OFFSET, CAN_BOOT_HOST_MCU_BYTE4_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_mcu_byte5_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_mcu.payload, CAN_BOOT_HOST_MCU_BYTE5_OFFSET, CAN_BOOT_HOST_MCU_BYTE5_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_mcu_byte6_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_mcu.payload, CAN_BOOT_HOST_MCU_BYTE6_OFFSET, CAN_BOOT_HOST_MCU_BYTE6_RANGE);
	return (data * 1.0) + 0;
}
uint16_t CAN_boot_host_mcu_byte7_get(void){
	uint16_t data = get_bits((size_t*)CAN_boot_host_mcu.payload, CAN_BOOT_HOST_MCU_BYTE7_OFFSET, CAN_BOOT_HOST_MCU_BYTE7_RANGE);
	return (data * 1.0) + 0;
}

void CAN_DBC_init(void) {
	CAN_configureMailbox(&CAN_dash_status);
	CAN_configureMailbox(&CAN_dash_command);
	CAN_configureMailbox(&CAN_dash_data1);
	CAN_configureMailbox(&CAN_dash_data2);
	CAN_configureMailbox(&CAN_bms_status);
	CAN_configureMailbox(&CAN_bms_status_2);
	CAN_configureMailbox(&CAN_bms_debug);
	CAN_configureMailbox(&CAN_motorcontroller_motorStatus);
	CAN_configureMailbox(&CAN_motorcontroller_response);
	CAN_configureMailbox(&CAN_boot_host_mcu);
}

void CAN_send_10ms(void){
	CAN_mcu_status_send();
}

void CAN_send_100ms(void){
	CAN_mcu_command_send();
}

void CAN_send_1ms(void){
	CAN_mcu_motorControllerRequest_send();
}
