#ifndef mcu_DBC_H
#define mcu_DBC_H

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

uint16_t CAN_dash_status_heartBeat_get(void);
uint16_t CAN_dash_status_state_get(void);
uint16_t CAN_dash_status_killButton_get(void);
uint16_t CAN_dash_status_ignButton_get(void);
uint16_t CAN_dash_status_modeButton_get(void);
uint16_t CAN_dash_status_selectButton_get(void);
uint16_t CAN_dash_status_driveMode_get(void);

#define CAN_dash_command_ID 0x702
typedef struct{
	uint16_t ignitionRequest: 1;
	uint16_t killRequest: 1;
	uint16_t batteryEjectRequest: 1;
	uint16_t lightsRequest: 1;
	uint16_t hornRequest: 1;
} CAN_dash_command_S;

uint16_t CAN_dash_command_ignitionRequest_get(void);
uint16_t CAN_dash_command_killRequest_get(void);
uint16_t CAN_dash_command_batteryEjectRequest_get(void);
uint16_t CAN_dash_command_lightsRequest_get(void);
uint16_t CAN_dash_command_hornRequest_get(void);

#define CAN_dash_data1_ID 0x703
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

void CAN_mcu_status_state_set(uint16_t state);
void CAN_mcu_status_throttleMode_set(uint16_t throttleMode);
void CAN_mcu_status_highBeam_set(uint16_t highBeam);
void CAN_mcu_status_lowBeam_set(uint16_t lowBeam);
void CAN_mcu_status_brakeLight_set(uint16_t brakeLight);
void CAN_mcu_status_tailLight_set(uint16_t tailLight);
void CAN_mcu_status_horn_set(uint16_t horn);
void CAN_mcu_status_turnSignalFR_set(uint16_t turnSignalFR);
void CAN_mcu_status_turnSignalFL_set(uint16_t turnSignalFL);
void CAN_mcu_status_turnSignalRR_set(uint16_t turnSignalRR);
void CAN_mcu_status_turnSignalRL_set(uint16_t turnSignalRL);
void CAN_mcu_status_GBSALL_set(uint16_t GBSALL);
void CAN_mcu_status_contactor_set(uint16_t contactor);
void CAN_mcu_status_chargePort_set(uint16_t chargePort);
void CAN_mcu_status_brakeSwitchFront_set(uint16_t brakeSwitchFront);
void CAN_mcu_status_brakeSwitchRear_set(uint16_t brakeSwitchRear);
void CAN_mcu_status_throttleVal_set(uint16_t throttleVal);

void CAN_mcu_status_send(void);


#define CAN_mcu_command_ID 0x712
typedef struct{
	uint16_t doSomthingElse: 1;
} CAN_mcu_command_S;

void CAN_mcu_command_doSomthingElse_set(uint16_t doSomthingElse);

void CAN_mcu_command_send(void);


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

void CAN_mcu_motorStatus_motorSpeed_set(uint16_t motorSpeed);
void CAN_mcu_motorStatus_motorCurrent_set(uint16_t motorCurrent);
void CAN_mcu_motorStatus_IphaseA_set(uint16_t IphaseA);
void CAN_mcu_motorStatus_IphaseB_set(uint16_t IphaseB);
void CAN_mcu_motorStatus_IphaseC_set(uint16_t IphaseC);
void CAN_mcu_motorStatus_VphaseA_set(uint16_t VphaseA);
void CAN_mcu_motorStatus_VphaseB_set(uint16_t VphaseB);
void CAN_mcu_motorStatus_VphaseC_set(uint16_t VphaseC);

void CAN_mcu_motorStatus_send(void);


#define CAN_mcu_motorControllerRequest_ID 0x700
typedef struct{
	uint16_t requestType: 8;
} CAN_mcu_motorControllerRequest_S;

void CAN_mcu_motorControllerRequest_requestType_set(uint16_t requestType);

void CAN_mcu_motorControllerRequest_send(void);





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



#endif /*mcu_DBC_H*/
