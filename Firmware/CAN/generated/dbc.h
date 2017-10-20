#ifndef DBC_H
#define DBC_H

#include <stdint.h>
#include "bolt_CAN.h"

typedef enum{
    DASH,
    BODYCONTROLLER,
    BMS,
} CAN_nodes_E;


/**********************************************************
 * DASH NODE MESSAGES
 */
#define CAN_DASH_status_ID 0x700
typedef struct{
	uint16_t state: 3;
	uint16_t killButton: 2;
	uint16_t ignButton: 2;
	uint16_t modeButton: 2;
	uint16_t selectButton: 2;
} CAN_DASH_status_S;

void CAN_DASH_status_state_set(uint16_t state);
void CAN_DASH_status_killButton_set(uint16_t killButton);
void CAN_DASH_status_ignButton_set(uint16_t ignButton);
void CAN_DASH_status_modeButton_set(uint16_t modeButton);
void CAN_DASH_status_selectButton_set(uint16_t selectButton);

uint16_t CAN_DASH_status_state_get(void);
uint16_t CAN_DASH_status_killButton_get(void);
uint16_t CAN_DASH_status_ignButton_get(void);
uint16_t CAN_DASH_status_modeButton_get(void);
uint16_t CAN_DASH_status_selectButton_get(void);

void CAN_DASH_status_send(void);


#define CAN_DASH_command_ID 0x701
typedef struct{
	uint16_t battery_latch: 1;
	uint16_t dothat: 3;
} CAN_DASH_command_S;

void CAN_DASH_command_battery_latch_set(uint16_t battery_latch);
void CAN_DASH_command_dothat_set(uint16_t dothat);

uint16_t CAN_DASH_command_battery_latch_get(void);
uint16_t CAN_DASH_command_dothat_get(void);

void CAN_DASH_command_send(void);





/**********************************************************
 * BODYCONTROLLER NODE MESSAGES
 */
#define CAN_BODYCONTROLLER_status_ID 0x710
typedef struct{
	uint16_t state: 3;
} CAN_BODYCONTROLLER_status_S;

void CAN_BODYCONTROLLER_status_state_set(uint16_t state);

uint16_t CAN_BODYCONTROLLER_status_state_get(void);

void CAN_BODYCONTROLLER_status_send(void);


#define CAN_BODYCONTROLLER_command_ID 0x711
typedef struct{
	uint16_t doSomthingElse: 1;
} CAN_BODYCONTROLLER_command_S;

void CAN_BODYCONTROLLER_command_doSomthingElse_set(uint16_t doSomthingElse);

uint16_t CAN_BODYCONTROLLER_command_doSomthingElse_get(void);

void CAN_BODYCONTROLLER_command_send(void);





/**********************************************************
 * BMS NODE MESSAGES
 */
#define CAN_BMS_status_ID 0x720
typedef struct{
	uint16_t state: 3;
	uint16_t SOC: 7;
	uint16_t packVoltage: 12;
	uint16_t packCurrent: 10;
	uint16_t minTemp: 12;
	uint16_t maxTemp: 12;
} CAN_BMS_status_S;

void CAN_BMS_status_state_set(uint16_t state);
void CAN_BMS_status_SOC_set(uint16_t SOC);
void CAN_BMS_status_packVoltage_set(uint16_t packVoltage);
void CAN_BMS_status_packCurrent_set(uint16_t packCurrent);
void CAN_BMS_status_minTemp_set(uint16_t minTemp);
void CAN_BMS_status_maxTemp_set(uint16_t maxTemp);

uint16_t CAN_BMS_status_state_get(void);
uint16_t CAN_BMS_status_SOC_get(void);
uint16_t CAN_BMS_status_packVoltage_get(void);
uint16_t CAN_BMS_status_packCurrent_get(void);
uint16_t CAN_BMS_status_minTemp_get(void);
uint16_t CAN_BMS_status_maxTemp_get(void);

void CAN_BMS_status_send(void);


#define CAN_BMS_cellVoltages_ID 0x721
typedef struct{
	uint16_t MultiPlex: 4;
	uint16_t cell1: 12;
	uint16_t cell2: 12;
	uint16_t cell3: 12;
	uint16_t cell4: 12;
	uint16_t cell5: 12;
} CAN_BMS_cellVoltages_S;

void CAN_BMS_cellVoltages_MultiPlex_set(uint16_t MultiPlex);
void CAN_BMS_cellVoltages_cell1_set(uint16_t cell1);
void CAN_BMS_cellVoltages_cell2_set(uint16_t cell2);
void CAN_BMS_cellVoltages_cell3_set(uint16_t cell3);
void CAN_BMS_cellVoltages_cell4_set(uint16_t cell4);
void CAN_BMS_cellVoltages_cell5_set(uint16_t cell5);

uint16_t CAN_BMS_cellVoltages_MultiPlex_get(void);
uint16_t CAN_BMS_cellVoltages_cell1_get(void);
uint16_t CAN_BMS_cellVoltages_cell2_get(void);
uint16_t CAN_BMS_cellVoltages_cell3_get(void);
uint16_t CAN_BMS_cellVoltages_cell4_get(void);
uint16_t CAN_BMS_cellVoltages_cell5_get(void);

void CAN_BMS_cellVoltages_send(void);




#endif /*DBC_H*/
