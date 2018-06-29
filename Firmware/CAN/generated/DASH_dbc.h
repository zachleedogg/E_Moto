#ifndef dash_DBC_H
#define dash_DBC_H

#include <stdint.h>
#include "bolt_CAN.h"

typedef enum{
    dash,
    mcu,
    bms,
    motorcontroller,
} CAN_nodes_E;


/**********************************************************
 * dash NODE MESSAGES
 */
#define CAN_dash_status_ID 0x701
typedef struct{
	uint16_t heartBeat: 4;
	uint16_t state: 3;
	uint16_t killButton: 2;
	uint16_t ignButton: 2;
	uint16_t modeButton: 2;
	uint16_t selectButton: 2;
	uint16_t driveMode: 3;
} CAN_dash_status_S;

void CAN_dash_status_heartBeat_set(uint16_t heartBeat);
void CAN_dash_status_state_set(uint16_t state);
void CAN_dash_status_killButton_set(uint16_t killButton);
void CAN_dash_status_ignButton_set(uint16_t ignButton);
void CAN_dash_status_modeButton_set(uint16_t modeButton);
void CAN_dash_status_selectButton_set(uint16_t selectButton);
void CAN_dash_status_driveMode_set(uint16_t driveMode);

void CAN_dash_status_send(void);


#define CAN_dash_command_ID 0x702
typedef struct{
	uint16_t ignitionRequest: 1;
	uint16_t killRequest: 1;
	uint16_t batteryEjectRequest: 1;
	uint16_t lightsRequest: 1;
	uint16_t hornRequest: 1;
} CAN_dash_command_S;

void CAN_dash_command_ignitionRequest_set(uint16_t ignitionRequest);
void CAN_dash_command_killRequest_set(uint16_t killRequest);
void CAN_dash_command_batteryEjectRequest_set(uint16_t batteryEjectRequest);
void CAN_dash_command_lightsRequest_set(uint16_t lightsRequest);
void CAN_dash_command_hornRequest_set(uint16_t hornRequest);

void CAN_dash_command_send(void);


#define CAN_dash_data1_ID 0x703
typedef struct{
	uint16_t speed: 16;
	uint16_t odometer: 16;
	uint16_t tripA: 16;
	uint16_t tripB: 16;
} CAN_dash_data1_S;

void CAN_dash_data1_speed_set(uint16_t speed);
void CAN_dash_data1_odometer_set(uint16_t odometer);
void CAN_dash_data1_tripA_set(uint16_t tripA);
void CAN_dash_data1_tripB_set(uint16_t tripB);

void CAN_dash_data1_send(void);


#define CAN_dash_data2_ID 0x704
typedef struct{
	uint16_t runningTime: 16;
	uint16_t odometer: 16;
	uint16_t tripA: 16;
	uint16_t tripB: 16;
} CAN_dash_data2_S;

void CAN_dash_data2_runningTime_set(uint16_t runningTime);
void CAN_dash_data2_odometer_set(uint16_t odometer);
void CAN_dash_data2_tripA_set(uint16_t tripA);
void CAN_dash_data2_tripB_set(uint16_t tripB);

void CAN_dash_data2_send(void);





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

#define CAN_mcu_motorStatus_ID 0x713
typedef struct{
	uint16_t motorSpeed: 8;
	uint16_t motorCurrent: 8;
	uint16_t IphaseA: 8;
	uint16_t IphaseB: 8;
	uint16_t IphaseC: 8;
	uint16_t VphaseA: 8;
	uint16_t VphaseB: 8;
	uint16_t VphaseC: 8;
} CAN_mcu_motorStatus_S;

uint16_t CAN_mcu_motorStatus_motorSpeed_get(void);
uint16_t CAN_mcu_motorStatus_motorCurrent_get(void);
uint16_t CAN_mcu_motorStatus_IphaseA_get(void);
uint16_t CAN_mcu_motorStatus_IphaseB_get(void);
uint16_t CAN_mcu_motorStatus_IphaseC_get(void);
uint16_t CAN_mcu_motorStatus_VphaseA_get(void);
uint16_t CAN_mcu_motorStatus_VphaseB_get(void);
uint16_t CAN_mcu_motorStatus_VphaseC_get(void);

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

uint16_t CAN_bms_status_state_get(void);
uint16_t CAN_bms_status_SOC_get(void);
uint16_t CAN_bms_status_packVoltage_get(void);
uint16_t CAN_bms_status_packCurrent_get(void);
uint16_t CAN_bms_status_minTemp_get(void);
uint16_t CAN_bms_status_maxTemp_get(void);

#define CAN_bms_cellVoltages_ID 0x722
typedef struct{
	uint16_t MultiPlex: 4;
	uint16_t cell1: 12;
	uint16_t cell2: 12;
	uint16_t cell3: 12;
	uint16_t cell4: 12;
	uint16_t cell5: 12;
} CAN_bms_cellVoltages_S;

uint16_t CAN_bms_cellVoltages_MultiPlex_get(void);
uint16_t CAN_bms_cellVoltages_cell1_get(void);
uint16_t CAN_bms_cellVoltages_cell2_get(void);
uint16_t CAN_bms_cellVoltages_cell3_get(void);
uint16_t CAN_bms_cellVoltages_cell4_get(void);
uint16_t CAN_bms_cellVoltages_cell5_get(void);




/**********************************************************
 * motorcontroller NODE MESSAGES
 */
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

void CAN_DBC_init();



#endif /*dash_DBC_H*/
