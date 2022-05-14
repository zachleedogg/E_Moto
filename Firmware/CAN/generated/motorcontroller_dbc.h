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
uint8_t CAN_mcu_command_checkDataIsFresh(void);
uint16_t CAN_mcu_command_DCDC_enable_get(void);
uint16_t CAN_mcu_command_ev_charger_enable_get(void);
uint16_t CAN_mcu_command_motor_controller_enable_get(void);

uint8_t CAN_mcu_motorControllerRequest_checkDataIsFresh(void);
uint16_t CAN_mcu_motorControllerRequest_requestType_get(void);

/**********************************************************
 * bms NODE MESSAGES
 */
/**********************************************************
 * motorcontroller NODE MESSAGES
 */
void CAN_motorcontroller_motorStatus_motorSpeed_set(uint16_t motorSpeed);
void CAN_motorcontroller_motorStatus_motorCurrent_set(uint16_t motorCurrent);
void CAN_motorcontroller_motorStatus_IphaseA_set(uint16_t IphaseA);
void CAN_motorcontroller_motorStatus_IphaseB_set(uint16_t IphaseB);
void CAN_motorcontroller_motorStatus_IphaseC_set(uint16_t IphaseC);
void CAN_motorcontroller_motorStatus_VphaseA_set(uint16_t VphaseA);
void CAN_motorcontroller_motorStatus_VphaseB_set(uint16_t VphaseB);
void CAN_motorcontroller_motorStatus_VphaseC_set(uint16_t VphaseC);
void CAN_motorcontroller_motorStatus_send(void);


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
/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init();

void CAN_send_10ms(void);


#endif /*motorcontroller_DBC_H*/
