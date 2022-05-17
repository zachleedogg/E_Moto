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
/**********************************************************
 * mcu NODE MESSAGES
 */
uint8_t CAN_mcu_command_checkDataIsFresh(void);
uint16_t CAN_mcu_command_DCDC_enable_get(void);
uint16_t CAN_mcu_command_ev_charger_enable_get(void);
uint16_t CAN_mcu_command_motor_controller_enable_get(void);

/**********************************************************
 * bms NODE MESSAGES
 */
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
/**********************************************************
 * charger NODE MESSAGES
 */
/**********************************************************
 * boot_host NODE MESSAGES
 */
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

void CAN_send_1ms(void);


#endif /*boot_host_DBC_H*/
