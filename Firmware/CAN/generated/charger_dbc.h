#ifndef charger_DBC_H
#define charger_DBC_H

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
#define CAN_dash_data1_ID 0x1806e5f5

#define CAN_DASH_DATA1_SPEED_RANGE 16
#define CAN_DASH_DATA1_SPEED_OFFSET 0
#define CAN_DASH_DATA1_ODOMETER_RANGE 16
#define CAN_DASH_DATA1_ODOMETER_OFFSET 16
#define CAN_DASH_DATA1_TRIPA_RANGE 16
#define CAN_DASH_DATA1_TRIPA_OFFSET 32
#define CAN_DASH_DATA1_TRIPB_RANGE 16
#define CAN_DASH_DATA1_TRIPB_OFFSET 48

uint8_t CAN_dash_data1_checkDataIsFresh(void);

uint16_t CAN_dash_data1_speed_get(void);
uint16_t CAN_dash_data1_odometer_get(void);
uint16_t CAN_dash_data1_tripA_get(void);
uint16_t CAN_dash_data1_tripB_get(void);


#define CAN_dash_data2_ID 0x704

#define CAN_DASH_DATA2_RUNNINGTIME_RANGE 16
#define CAN_DASH_DATA2_RUNNINGTIME_OFFSET 0
#define CAN_DASH_DATA2_ODOMETER_RANGE 16
#define CAN_DASH_DATA2_ODOMETER_OFFSET 16
#define CAN_DASH_DATA2_TRIPA_RANGE 16
#define CAN_DASH_DATA2_TRIPA_OFFSET 32
#define CAN_DASH_DATA2_TRIPB_RANGE 16
#define CAN_DASH_DATA2_TRIPB_OFFSET 48

uint8_t CAN_dash_data2_checkDataIsFresh(void);

uint16_t CAN_dash_data2_runningTime_get(void);
uint16_t CAN_dash_data2_odometer_get(void);
uint16_t CAN_dash_data2_tripA_get(void);
uint16_t CAN_dash_data2_tripB_get(void);





/**********************************************************
 * mcu NODE MESSAGES
 */
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
#define CAN_charger_charger_status_ID 0x18ff50e5

#define CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_RANGE 8
#define CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_HIGH_BYTE_OFFSET 0
#define CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_RANGE 8
#define CAN_CHARGER_CHARGER_STATUS_OUTPUT_VOLTAGE_LOW_BYTE_OFFSET 8
#define CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_RANGE 8
#define CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_HIGH_BYTE_OFFSET 16
#define CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_RANGE 8
#define CAN_CHARGER_CHARGER_STATUS_OUTPUT_CURRENT_LOW_BYTE_OFFSET 24
#define CAN_CHARGER_CHARGER_STATUS_HARDWARE_ERROR_RANGE 1
#define CAN_CHARGER_CHARGER_STATUS_HARDWARE_ERROR_OFFSET 32
#define CAN_CHARGER_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_RANGE 1
#define CAN_CHARGER_CHARGER_STATUS_CHARGER_OVERTEMP_ERROR_OFFSET 33
#define CAN_CHARGER_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_RANGE 1
#define CAN_CHARGER_CHARGER_STATUS_INPUT_VOLTAGE_ERROR_OFFSET 34
#define CAN_CHARGER_CHARGER_STATUS_BATTERY_DETECT_ERROR_RANGE 1
#define CAN_CHARGER_CHARGER_STATUS_BATTERY_DETECT_ERROR_OFFSET 35
#define CAN_CHARGER_CHARGER_STATUS_COMMUNICATION_ERROR_RANGE 1
#define CAN_CHARGER_CHARGER_STATUS_COMMUNICATION_ERROR_OFFSET 36
#define CAN_CHARGER_CHARGER_STATUS_BYTE7_RANGE 8
#define CAN_CHARGER_CHARGER_STATUS_BYTE7_OFFSET 37
#define CAN_CHARGER_CHARGER_STATUS_BYTE8_RANGE 8
#define CAN_CHARGER_CHARGER_STATUS_BYTE8_OFFSET 45

void CAN_charger_charger_status_output_voltage_high_byte_set(uint16_t output_voltage_high_byte);
void CAN_charger_charger_status_output_voltage_low_byte_set(uint16_t output_voltage_low_byte);
void CAN_charger_charger_status_output_current_high_byte_set(uint16_t output_current_high_byte);
void CAN_charger_charger_status_output_current_low_byte_set(uint16_t output_current_low_byte);
void CAN_charger_charger_status_hardware_error_set(uint16_t hardware_error);
void CAN_charger_charger_status_charger_overtemp_error_set(uint16_t charger_overtemp_error);
void CAN_charger_charger_status_input_voltage_error_set(uint16_t input_voltage_error);
void CAN_charger_charger_status_battery_detect_error_set(uint16_t battery_detect_error);
void CAN_charger_charger_status_communication_error_set(uint16_t communication_error);
void CAN_charger_charger_status_byte7_set(uint16_t byte7);
void CAN_charger_charger_status_byte8_set(uint16_t byte8);

void CAN_charger_charger_status_send(void);





/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init();



#endif /*charger_DBC_H*/