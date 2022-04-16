#ifndef motorcontroller_DBC_H
#define motorcontroller_DBC_H

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

void CAN_motorcontroller_motorStatus_motorSpeed_set(uint16_t motorSpeed);
void CAN_motorcontroller_motorStatus_motorCurrent_set(uint16_t motorCurrent);
void CAN_motorcontroller_motorStatus_IphaseA_set(uint16_t IphaseA);
void CAN_motorcontroller_motorStatus_IphaseB_set(uint16_t IphaseB);
void CAN_motorcontroller_motorStatus_IphaseC_set(uint16_t IphaseC);
void CAN_motorcontroller_motorStatus_VphaseA_set(uint16_t VphaseA);
void CAN_motorcontroller_motorStatus_VphaseB_set(uint16_t VphaseB);
void CAN_motorcontroller_motorStatus_VphaseC_set(uint16_t VphaseC);

void CAN_motorcontroller_motorStatus_send(void);


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

void CAN_motorcontroller_response_byte1_set(uint16_t byte1);
void CAN_motorcontroller_response_byte2_set(uint16_t byte2);
void CAN_motorcontroller_response_byte3_set(uint16_t byte3);
void CAN_motorcontroller_response_byte4_set(uint16_t byte4);
void CAN_motorcontroller_response_byte5_set(uint16_t byte5);
void CAN_motorcontroller_response_byte6_set(uint16_t byte6);
void CAN_motorcontroller_response_byte7_set(uint16_t byte7);
void CAN_motorcontroller_response_byte8_set(uint16_t byte8);

void CAN_motorcontroller_response_send(void);





/**********************************************************
 * charger NODE MESSAGES
 */
void CAN_DBC_init();



#endif /*motorcontroller_DBC_H*/
