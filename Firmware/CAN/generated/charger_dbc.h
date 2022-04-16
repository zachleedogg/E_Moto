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
} CAN_nodes_E;


/**********************************************************
 * dash NODE MESSAGES
 */
#define CAN_dash_data1_ID 0x1806e5f5
typedef struct{
	uint16_t speed: 16;
	uint16_t odometer: 16;
	uint16_t tripA: 16;
	uint16_t tripB: 16;
} CAN_dash_data1_S;

uint16_t CAN_dash_data1_speed_get(void);
uint16_t CAN_dash_data1_odometer_get(void);
uint16_t CAN_dash_data1_tripA_get(void);
uint16_t CAN_dash_data1_tripB_get(void);

#define CAN_dash_data2_ID 0x704
typedef struct{
	uint16_t runningTime: 16;
	uint16_t odometer: 16;
	uint16_t tripA: 16;
	uint16_t tripB: 16;
} CAN_dash_data2_S;

uint16_t CAN_dash_data2_runningTime_get(void);
uint16_t CAN_dash_data2_odometer_get(void);
uint16_t CAN_dash_data2_tripA_get(void);
uint16_t CAN_dash_data2_tripB_get(void);




/**********************************************************
 * mcu NODE MESSAGES
 */
#define CAN_mcu_command_ID 0x712
typedef struct{
	uint16_t doSomthingElse: 1;
} CAN_mcu_command_S;

uint16_t CAN_mcu_command_doSomthingElse_get(void);

#define CAN_mcu_motorControllerRequest_ID 0x700
typedef struct{
	uint16_t requestType: 8;
} CAN_mcu_motorControllerRequest_S;

uint16_t CAN_mcu_motorControllerRequest_requestType_get(void);




/**********************************************************
 * bms NODE MESSAGES
 */
#define CAN_bms_charger_request_ID 0x1806e5f4
typedef struct{
	uint16_t output_voltage_high_byte: 8;
	uint16_t output_voltage_low_byte: 8;
	uint16_t output_current_high_byte: 8;
	uint16_t output_current_low_byte: 8;
	uint16_t start_charge_request: 8;
	uint16_t charge_mode: 8;
	uint16_t packCurrent: 8;
	uint16_t byte_7: 8;
	uint16_t byte_8: 8;
} CAN_bms_charger_request_S;

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
typedef struct{
	uint16_t motorSpeed: 8;
	uint16_t motorCurrent: 8;
	uint16_t IphaseA: 8;
	uint16_t IphaseB: 8;
	uint16_t IphaseC: 8;
	uint16_t VphaseA: 8;
	uint16_t VphaseB: 8;
	uint16_t VphaseC: 8;
} CAN_motorcontroller_motorStatus_S;

uint16_t CAN_motorcontroller_motorStatus_motorSpeed_get(void);
uint16_t CAN_motorcontroller_motorStatus_motorCurrent_get(void);
uint16_t CAN_motorcontroller_motorStatus_IphaseA_get(void);
uint16_t CAN_motorcontroller_motorStatus_IphaseB_get(void);
uint16_t CAN_motorcontroller_motorStatus_IphaseC_get(void);
uint16_t CAN_motorcontroller_motorStatus_VphaseA_get(void);
uint16_t CAN_motorcontroller_motorStatus_VphaseB_get(void);
uint16_t CAN_motorcontroller_motorStatus_VphaseC_get(void);

#define CAN_motorcontroller_response_ID 0x700
typedef struct{
	uint16_t byte1: 8;
	uint16_t byte2: 8;
	uint16_t byte3: 8;
	uint16_t byte4: 8;
	uint16_t byte5: 8;
	uint16_t byte6: 8;
	uint16_t byte7: 8;
	uint16_t byte8: 8;
} CAN_motorcontroller_response_S;

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
typedef struct{
	uint16_t output_voltage_high_byte: 8;
	uint16_t output_voltage_low_byte: 8;
	uint16_t output_current_high_byte: 8;
	uint16_t output_current_low_byte: 8;
	uint16_t hardware_error: 1;
	uint16_t charger_overtemp_error: 1;
	uint16_t input_voltage_error: 1;
	uint16_t battery_detect_error: 1;
	uint16_t communication_error: 1;
	uint16_t byte7: 8;
	uint16_t byte8: 8;
} CAN_charger_charger_status_S;

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


void CAN_DBC_init();



#endif /*charger_DBC_H*/
