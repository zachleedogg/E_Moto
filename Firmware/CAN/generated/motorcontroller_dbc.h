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
    boot_host,
} CAN_nodes_E;

/**********************************************************
 * dash NODE MESSAGES
 */
/**********************************************************
 * mcu NODE MESSAGES
 */
#define CAN_mcu_command_interval() 100
uint8_t CAN_mcu_command_checkDataIsFresh(void);
uint16_t CAN_mcu_command_DCDC_enable_get(void);
uint16_t CAN_mcu_command_ev_charger_enable_get(void);
float CAN_mcu_command_ev_charger_current_get(void);
uint16_t CAN_mcu_command_precharge_enable_get(void);
uint16_t CAN_mcu_command_motor_controller_enable_get(void);

#define CAN_mcu_motorControllerRequest_interval() 1
uint8_t CAN_mcu_motorControllerRequest_checkDataIsFresh(void);
uint16_t CAN_mcu_motorControllerRequest_requestType_get(void);

/**********************************************************
 * bms NODE MESSAGES
 */
#define CAN_bms_debug_interval() 10
uint8_t CAN_bms_debug_checkDataIsFresh(void);
uint16_t CAN_bms_debug_bool0_get(void);
uint16_t CAN_bms_debug_bool1_get(void);
uint16_t CAN_bms_debug_bool2_get(void);
uint16_t CAN_bms_debug_bool3_get(void);
float CAN_bms_debug_float1_get(void);
float CAN_bms_debug_float2_get(void);

/**********************************************************
 * motorcontroller NODE MESSAGES
 */
#define CAN_motorcontroller_motorStatus_interval() 10
void CAN_motorcontroller_motorStatus_motorSpeed_set(uint16_t motorSpeed);
void CAN_motorcontroller_motorStatus_motorCurrent_set(float motorCurrent);
void CAN_motorcontroller_motorStatus_IphaseA_set(uint16_t IphaseA);
void CAN_motorcontroller_motorStatus_IphaseB_set(uint16_t IphaseB);
void CAN_motorcontroller_motorStatus_IphaseC_set(uint16_t IphaseC);
void CAN_motorcontroller_motorStatus_VphaseA_set(uint16_t VphaseA);
void CAN_motorcontroller_motorStatus_VphaseB_set(uint16_t VphaseB);
void CAN_motorcontroller_motorStatus_VphaseC_set(uint16_t VphaseC);
void CAN_motorcontroller_motorStatus_dlc_set(uint8_t dlc);


void CAN_motorcontroller_motorStatus_send(void);


#define CAN_motorcontroller_response_interval() None
void CAN_motorcontroller_response_byte1_set(uint16_t byte1);
void CAN_motorcontroller_response_byte2_set(uint16_t byte2);
void CAN_motorcontroller_response_byte3_set(uint16_t byte3);
void CAN_motorcontroller_response_byte4_set(uint16_t byte4);
void CAN_motorcontroller_response_byte5_set(uint16_t byte5);
void CAN_motorcontroller_response_byte6_set(uint16_t byte6);
void CAN_motorcontroller_response_byte7_set(uint16_t byte7);
void CAN_motorcontroller_response_byte8_set(uint16_t byte8);
void CAN_motorcontroller_response_dlc_set(uint8_t dlc);


void CAN_motorcontroller_response_send(void);


/**********************************************************
 * charger NODE MESSAGES
 */
/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init();

void CAN_send_10ms(void);


#endif /*motorcontroller_DBC_H*/
