/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CAN_IS_TP_LITE_CONFIG_H
#define	CAN_IS_TP_LITE_CONFIG_H

#include <stdint.h>
#include "bms_dbc.h"

/*Function pint to CAN set functions*/
void (*CAN_boot_response_type_set)(uint16_t) = CAN_bms_boot_response_type_set;
void (*CAN_boot_response_code_set)(uint16_t) = CAN_bms_boot_response_code_set;
void (*CAN_boot_response_byte1_set)(uint16_t) = CAN_bms_boot_response_byte1_set;
void (*CAN_boot_response_byte2_set)(uint16_t) = CAN_bms_boot_response_byte2_set;
void (*CAN_boot_response_byte3_set)(uint16_t) = CAN_bms_boot_response_byte3_set;
void (*CAN_boot_response_byte4_set)(uint16_t) = CAN_bms_boot_response_byte4_set;
void (*CAN_boot_response_byte5_set)(uint16_t) = CAN_bms_boot_response_byte5_set;
void (*CAN_boot_response_byte6_set)(uint16_t) = CAN_bms_boot_response_byte6_set;
void (*CAN_boot_response_byte7_set)(uint16_t) = CAN_bms_boot_response_byte7_set;
void (*CAN_boot_response_dlc_set)(uint8_t) = CAN_bms_boot_response_dlc_set;
void (*CAN_boot_response_send)(void) = CAN_bms_boot_response_send;

/*Function pointer to CAN get functions*/
uint8_t (*CAN_boot_host_checkDataIsFresh)(void) = CAN_boot_host_bms_checkDataIsFresh;
uint16_t (*CAN_boot_host_type_get)(void) = CAN_boot_host_bms_type_get;
uint16_t (*CAN_boot_host_code_get)(void) = CAN_boot_host_bms_code_get;

uint16_t (*CAN_boot_host_getBytesFp[]) (void) = {
    CAN_boot_host_bms_byte1_get,
    CAN_boot_host_bms_byte2_get,
    CAN_boot_host_bms_byte3_get,
    CAN_boot_host_bms_byte4_get,
    CAN_boot_host_bms_byte5_get,
    CAN_boot_host_bms_byte6_get,
    CAN_boot_host_bms_byte7_get,
};

#endif	/* CAN_IS_TP_LITE_CONFIG_H */

