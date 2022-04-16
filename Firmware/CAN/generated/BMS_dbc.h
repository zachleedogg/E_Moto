#ifndef bms_DBC_H
#define bms_DBC_H

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
#define CAN_mcu_status_ID 0x711
typedef struct{
	uint16_t state: 3;
	uint16_t throttleMode: 3;
	uint16_t highBeam: 1;
	uint16_t lowBeam: 1;
	uint16_t brakeLight: 1;
	uint16_t tailLight: 1;
	uint16_t horn: 1;
	uint16_t turnSignalFR: 1;
	uint16_t turnSignalFL: 1;
	uint16_t turnSignalRR: 1;
	uint16_t turnSignalRL: 1;
	uint16_t GBSALL: 1;
	uint16_t contactor: 1;
	uint16_t chargePort: 1;
	uint16_t brakeSwitchFront: 1;
	uint16_t brakeSwitchRear: 1;
	uint16_t throttleVal: 8;
} CAN_mcu_status_S;

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
#define CAN_bms_status_ID 0x721
typedef struct{
	uint16_t state: 3;
	uint16_t SOC: 7;
	uint16_t packVoltage: 12;
	uint16_t packCurrent: 10;
	uint16_t minTemp: 12;
	uint16_t maxTemp: 12;
} CAN_bms_status_S;

void CAN_bms_status_state_set(uint16_t state);
void CAN_bms_status_SOC_set(uint16_t SOC);
void CAN_bms_status_packVoltage_set(uint16_t packVoltage);
void CAN_bms_status_packCurrent_set(uint16_t packCurrent);
void CAN_bms_status_minTemp_set(uint16_t minTemp);
void CAN_bms_status_maxTemp_set(uint16_t maxTemp);

void CAN_bms_status_send(void);


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

void CAN_bms_charger_request_output_voltage_high_byte_set(uint16_t output_voltage_high_byte);
void CAN_bms_charger_request_output_voltage_low_byte_set(uint16_t output_voltage_low_byte);
void CAN_bms_charger_request_output_current_high_byte_set(uint16_t output_current_high_byte);
void CAN_bms_charger_request_output_current_low_byte_set(uint16_t output_current_low_byte);
void CAN_bms_charger_request_start_charge_request_set(uint16_t start_charge_request);
void CAN_bms_charger_request_charge_mode_set(uint16_t charge_mode);
void CAN_bms_charger_request_packCurrent_set(uint16_t packCurrent);
void CAN_bms_charger_request_byte_7_set(uint16_t byte_7);
void CAN_bms_charger_request_byte_8_set(uint16_t byte_8);

void CAN_bms_charger_request_send(void);


#define CAN_bms_cellVoltages_ID 0x723
typedef struct{
	uint16_t MultiPlex: 4;
	uint16_t cell1: 12;
	uint16_t cell2: 12;
	uint16_t cell3: 12;
	uint16_t cell4: 12;
	uint16_t cell5: 12;
} CAN_bms_cellVoltages_S;

void CAN_bms_cellVoltages_MultiPlex_set(uint16_t MultiPlex);
void CAN_bms_cellVoltages_cell1_set(uint16_t cell1);
void CAN_bms_cellVoltages_cell2_set(uint16_t cell2);
void CAN_bms_cellVoltages_cell3_set(uint16_t cell3);
void CAN_bms_cellVoltages_cell4_set(uint16_t cell4);
void CAN_bms_cellVoltages_cell5_set(uint16_t cell5);

void CAN_bms_cellVoltages_send(void);





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

uint16_t CAN_charger_charger_status_output_voltage_high_byte_get(void);
uint16_t CAN_charger_charger_status_output_voltage_low_byte_get(void);
uint16_t CAN_charger_charger_status_output_current_high_byte_get(void);
uint16_t CAN_charger_charger_status_output_current_low_byte_get(void);
uint16_t CAN_charger_charger_status_hardware_error_get(void);
uint16_t CAN_charger_charger_status_charger_overtemp_error_get(void);
uint16_t CAN_charger_charger_status_input_voltage_error_get(void);
uint16_t CAN_charger_charger_status_battery_detect_error_get(void);
uint16_t CAN_charger_charger_status_communication_error_get(void);
uint16_t CAN_charger_charger_status_byte7_get(void);
uint16_t CAN_charger_charger_status_byte8_get(void);

void CAN_DBC_init();



#endif /*bms_DBC_H*/
