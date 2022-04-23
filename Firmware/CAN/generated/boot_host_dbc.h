#ifndef boot_host_DBC_H
#define boot_host_DBC_H

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
#define CAN_bms_boot_response_ID 0xa2

#define CAN_BMS_BOOT_RESPONSE_CODE_RANGE 4
#define CAN_BMS_BOOT_RESPONSE_CODE_OFFSET 0
#define CAN_BMS_BOOT_RESPONSE_TYPE_RANGE 4
#define CAN_BMS_BOOT_RESPONSE_TYPE_OFFSET 4
#define CAN_BMS_BOOT_RESPONSE_BYTE1_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE1_OFFSET 8
#define CAN_BMS_BOOT_RESPONSE_BYTE2_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE2_OFFSET 16
#define CAN_BMS_BOOT_RESPONSE_BYTE3_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE3_OFFSET 24
#define CAN_BMS_BOOT_RESPONSE_BYTE4_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE4_OFFSET 32
#define CAN_BMS_BOOT_RESPONSE_BYTE5_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE5_OFFSET 40
#define CAN_BMS_BOOT_RESPONSE_BYTE6_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE6_OFFSET 48
#define CAN_BMS_BOOT_RESPONSE_BYTE7_RANGE 8
#define CAN_BMS_BOOT_RESPONSE_BYTE7_OFFSET 56

uint8_t CAN_bms_boot_response_checkDataIsFresh(void);

uint16_t CAN_bms_boot_response_code_get(void);
uint16_t CAN_bms_boot_response_type_get(void);
uint16_t CAN_bms_boot_response_byte1_get(void);
uint16_t CAN_bms_boot_response_byte2_get(void);
uint16_t CAN_bms_boot_response_byte3_get(void);
uint16_t CAN_bms_boot_response_byte4_get(void);
uint16_t CAN_bms_boot_response_byte5_get(void);
uint16_t CAN_bms_boot_response_byte6_get(void);
uint16_t CAN_bms_boot_response_byte7_get(void);





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
#define CAN_boot_host_bms_ID 0xa1

#define CAN_BOOT_HOST_BMS_CODE_RANGE 4
#define CAN_BOOT_HOST_BMS_CODE_OFFSET 0
#define CAN_BOOT_HOST_BMS_TYPE_RANGE 4
#define CAN_BOOT_HOST_BMS_TYPE_OFFSET 4
#define CAN_BOOT_HOST_BMS_BYTE1_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE1_OFFSET 8
#define CAN_BOOT_HOST_BMS_BYTE2_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE2_OFFSET 16
#define CAN_BOOT_HOST_BMS_BYTE3_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE3_OFFSET 24
#define CAN_BOOT_HOST_BMS_BYTE4_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE4_OFFSET 32
#define CAN_BOOT_HOST_BMS_BYTE5_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE5_OFFSET 40
#define CAN_BOOT_HOST_BMS_BYTE6_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE6_OFFSET 48
#define CAN_BOOT_HOST_BMS_BYTE7_RANGE 8
#define CAN_BOOT_HOST_BMS_BYTE7_OFFSET 56

void CAN_boot_host_bms_code_set(uint16_t code);
void CAN_boot_host_bms_type_set(uint16_t type);
void CAN_boot_host_bms_byte1_set(uint16_t byte1);
void CAN_boot_host_bms_byte2_set(uint16_t byte2);
void CAN_boot_host_bms_byte3_set(uint16_t byte3);
void CAN_boot_host_bms_byte4_set(uint16_t byte4);
void CAN_boot_host_bms_byte5_set(uint16_t byte5);
void CAN_boot_host_bms_byte6_set(uint16_t byte6);
void CAN_boot_host_bms_byte7_set(uint16_t byte7);

void CAN_boot_host_bms_send(void);


void CAN_DBC_init();



#endif /*boot_host_DBC_H*/
