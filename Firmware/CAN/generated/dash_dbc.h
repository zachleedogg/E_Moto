#ifndef dash_DBC_H
#define dash_DBC_H

#include <stdint.h>
#include "bolt_CAN.h"

typedef enum{
    dash,
    mcu,
    bms,
    motorcontroller,
    charger,
    boot_host,
} CAN_nodes_E;


/**********************************************************
 * dash NODE MESSAGES
 */
#define CAN_dash_status_ID 0x701

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

void CAN_dash_status_heartBeat_set(uint16_t heartBeat);
void CAN_dash_status_state_set(uint16_t state);
void CAN_dash_status_killButton_set(uint16_t killButton);
void CAN_dash_status_ignButton_set(uint16_t ignButton);
void CAN_dash_status_modeButton_set(uint16_t modeButton);
void CAN_dash_status_selectButton_set(uint16_t selectButton);
void CAN_dash_status_driveMode_set(uint16_t driveMode);

void CAN_dash_status_send(void);


#define CAN_dash_command_ID 0x702

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

void CAN_dash_command_ignitionRequest_set(uint16_t ignitionRequest);
void CAN_dash_command_killRequest_set(uint16_t killRequest);
void CAN_dash_command_batteryEjectRequest_set(uint16_t batteryEjectRequest);
void CAN_dash_command_lightsRequest_set(uint16_t lightsRequest);
void CAN_dash_command_hornRequest_set(uint16_t hornRequest);

void CAN_dash_command_send(void);


#define CAN_dash_data1_ID 0x1806e5f5

#define CAN_DASH_DATA1_SPEED_RANGE 16
#define CAN_DASH_DATA1_SPEED_OFFSET 0
#define CAN_DASH_DATA1_ODOMETER_RANGE 16
#define CAN_DASH_DATA1_ODOMETER_OFFSET 16
#define CAN_DASH_DATA1_TRIPA_RANGE 16
#define CAN_DASH_DATA1_TRIPA_OFFSET 32
#define CAN_DASH_DATA1_TRIPB_RANGE 16
#define CAN_DASH_DATA1_TRIPB_OFFSET 48

void CAN_dash_data1_speed_set(uint16_t speed);
void CAN_dash_data1_odometer_set(uint16_t odometer);
void CAN_dash_data1_tripA_set(uint16_t tripA);
void CAN_dash_data1_tripB_set(uint16_t tripB);

void CAN_dash_data1_send(void);


#define CAN_dash_data2_ID 0x704

#define CAN_DASH_DATA2_RUNNINGTIME_RANGE 16
#define CAN_DASH_DATA2_RUNNINGTIME_OFFSET 0
#define CAN_DASH_DATA2_ODOMETER_RANGE 16
#define CAN_DASH_DATA2_ODOMETER_OFFSET 16
#define CAN_DASH_DATA2_TRIPA_RANGE 16
#define CAN_DASH_DATA2_TRIPA_OFFSET 32
#define CAN_DASH_DATA2_TRIPB_RANGE 16
#define CAN_DASH_DATA2_TRIPB_OFFSET 48

void CAN_dash_data2_runningTime_set(uint16_t runningTime);
void CAN_dash_data2_odometer_set(uint16_t odometer);
void CAN_dash_data2_tripA_set(uint16_t tripA);
void CAN_dash_data2_tripB_set(uint16_t tripB);

void CAN_dash_data2_send(void);





/**********************************************************
 * mcu NODE MESSAGES
 */
#define CAN_mcu_status_ID 0x711

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

uint8_t CAN_mcu_status_checkDataIsFresh(void);

uint16_t CAN_mcu_status_state_get(void);
uint16_t CAN_mcu_status_throttleMode_get(void);
uint16_t CAN_mcu_status_highBeam_get(void);
uint16_t CAN_mcu_status_lowBeam_get(void);
uint16_t CAN_mcu_status_brakeLight_get(void);
uint16_t CAN_mcu_status_tailLight_get(void);
uint16_t CAN_mcu_status_horn_get(void);
uint16_t CAN_mcu_status_turnSignalFR_get(void);
uint16_t CAN_mcu_status_turnSignalFL_get(void);
uint16_t CAN_mcu_status_turnSignalRR_get(void);
uint16_t CAN_mcu_status_turnSignalRL_get(void);
uint16_t CAN_mcu_status_GBSALL_get(void);
uint16_t CAN_mcu_status_contactor_get(void);
uint16_t CAN_mcu_status_chargePort_get(void);
uint16_t CAN_mcu_status_brakeSwitchFront_get(void);
uint16_t CAN_mcu_status_brakeSwitchRear_get(void);
uint16_t CAN_mcu_status_throttleVal_get(void);


#define CAN_mcu_command_ID 0x712

#define CAN_MCU_COMMAND_DOSOMTHINGELSE_RANGE 1
#define CAN_MCU_COMMAND_DOSOMTHINGELSE_OFFSET 0

uint8_t CAN_mcu_command_checkDataIsFresh(void);

uint16_t CAN_mcu_command_doSomthingElse_get(void);


#define CAN_mcu_motorControllerRequest_ID 0x700

#define CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_RANGE 8
#define CAN_MCU_MOTORCONTROLLERREQUEST_REQUESTTYPE_OFFSET 0

uint8_t CAN_mcu_motorControllerRequest_checkDataIsFresh(void);

uint16_t CAN_mcu_motorControllerRequest_requestType_get(void);





/**********************************************************
 * bms NODE MESSAGES
 */
#define CAN_bms_status_ID 0x721

#define CAN_BMS_STATUS_STATE_RANGE 3
#define CAN_BMS_STATUS_STATE_OFFSET 0
#define CAN_BMS_STATUS_SOC_RANGE 7
#define CAN_BMS_STATUS_SOC_OFFSET 3
#define CAN_BMS_STATUS_PACKVOLTAGE_RANGE 12
#define CAN_BMS_STATUS_PACKVOLTAGE_OFFSET 10
#define CAN_BMS_STATUS_PACKCURRENT_RANGE 10
#define CAN_BMS_STATUS_PACKCURRENT_OFFSET 22
#define CAN_BMS_STATUS_MINTEMP_RANGE 12
#define CAN_BMS_STATUS_MINTEMP_OFFSET 32
#define CAN_BMS_STATUS_MAXTEMP_RANGE 12
#define CAN_BMS_STATUS_MAXTEMP_OFFSET 44

uint8_t CAN_bms_status_checkDataIsFresh(void);

uint16_t CAN_bms_status_state_get(void);
uint16_t CAN_bms_status_SOC_get(void);
uint16_t CAN_bms_status_packVoltage_get(void);
uint16_t CAN_bms_status_packCurrent_get(void);
uint16_t CAN_bms_status_minTemp_get(void);
uint16_t CAN_bms_status_maxTemp_get(void);


#define CAN_bms_charger_request_ID 0x1806e5f4

#define CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET 0
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET 8
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_HIGH_BYTE_OFFSET 16
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_OUTPUT_CURRENT_LOW_BYTE_OFFSET 24
#define CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_START_CHARGE_REQUEST_OFFSET 32
#define CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_CHARGE_MODE_OFFSET 40
#define CAN_BMS_CHARGER_REQUEST_PACKCURRENT_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_PACKCURRENT_OFFSET 48
#define CAN_BMS_CHARGER_REQUEST_BYTE_7_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_BYTE_7_OFFSET 56
#define CAN_BMS_CHARGER_REQUEST_BYTE_8_RANGE 8
#define CAN_BMS_CHARGER_REQUEST_BYTE_8_OFFSET 64

uint8_t CAN_bms_charger_request_checkDataIsFresh(void);

uint16_t CAN_bms_charger_request_output_voltage_high_byte_get(void);
uint16_t CAN_bms_charger_request_output_voltage_low_byte_get(void);
uint16_t CAN_bms_charger_request_output_current_high_byte_get(void);
uint16_t CAN_bms_charger_request_output_current_low_byte_get(void);
uint16_t CAN_bms_charger_request_start_charge_request_get(void);
uint16_t CAN_bms_charger_request_charge_mode_get(void);
uint16_t CAN_bms_charger_request_packCurrent_get(void);
uint16_t CAN_bms_charger_request_byte_7_get(void);
uint16_t CAN_bms_charger_request_byte_8_get(void);


#define CAN_bms_cellVoltages_ID 0x724

#define CAN_BMS_CELLVOLTAGES_MULTIPLEX_RANGE 4
#define CAN_BMS_CELLVOLTAGES_MULTIPLEX_OFFSET 0
#define CAN_BMS_CELLVOLTAGES_CELL1_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL1_OFFSET 4
#define CAN_BMS_CELLVOLTAGES_CELL2_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL2_OFFSET 16
#define CAN_BMS_CELLVOLTAGES_CELL3_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL3_OFFSET 28
#define CAN_BMS_CELLVOLTAGES_CELL4_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL4_OFFSET 40
#define CAN_BMS_CELLVOLTAGES_CELL5_RANGE 12
#define CAN_BMS_CELLVOLTAGES_CELL5_OFFSET 52

uint8_t CAN_bms_cellVoltages_checkDataIsFresh(void);

uint16_t CAN_bms_cellVoltages_MultiPlex_get(void);
uint16_t CAN_bms_cellVoltages_cell1_get(void);
uint16_t CAN_bms_cellVoltages_cell2_get(void);
uint16_t CAN_bms_cellVoltages_cell3_get(void);
uint16_t CAN_bms_cellVoltages_cell4_get(void);
uint16_t CAN_bms_cellVoltages_cell5_get(void);





/**********************************************************
 * motorcontroller NODE MESSAGES
 */
#define CAN_motorcontroller_motorStatus_ID 0x731

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

uint8_t CAN_motorcontroller_motorStatus_checkDataIsFresh(void);

uint16_t CAN_motorcontroller_motorStatus_motorSpeed_get(void);
uint16_t CAN_motorcontroller_motorStatus_motorCurrent_get(void);
uint16_t CAN_motorcontroller_motorStatus_IphaseA_get(void);
uint16_t CAN_motorcontroller_motorStatus_IphaseB_get(void);
uint16_t CAN_motorcontroller_motorStatus_IphaseC_get(void);
uint16_t CAN_motorcontroller_motorStatus_VphaseA_get(void);
uint16_t CAN_motorcontroller_motorStatus_VphaseB_get(void);
uint16_t CAN_motorcontroller_motorStatus_VphaseC_get(void);


#define CAN_motorcontroller_response_ID 0x700

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

uint8_t CAN_motorcontroller_response_checkDataIsFresh(void);

uint16_t CAN_motorcontroller_response_byte1_get(void);
uint16_t CAN_motorcontroller_response_byte2_get(void);
uint16_t CAN_motorcontroller_response_byte3_get(void);
uint16_t CAN_motorcontroller_response_byte4_get(void);
uint16_t CAN_motorcontroller_response_byte5_get(void);
uint16_t CAN_motorcontroller_response_byte6_get(void);
uint16_t CAN_motorcontroller_response_byte7_get(void);
uint16_t CAN_motorcontroller_response_byte8_get(void);





/**********************************************************
 * charger NODE MESSAGES
 */



/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init();



#endif /*dash_DBC_H*/
