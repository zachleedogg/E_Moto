/* 
 * File:   Efuse.h
 * Author: kid group
 *
 * Created on March 10, 2018, 10:04 PM
 */

#ifndef Efuse_H
#define	Efuse_H

#include "stdint.h"

void EFUSE_Init(void);

void EFUSE_Run(void);

void EFUSE_Halt(void);

uint8_t EFUSE_GET_FAN_FAULT(void);

uint8_t EFUSE_GET_PUMP_FAULT(void);

uint8_t EFUSE_GET_TAILLIGHT_FAULT(void);

uint8_t EFUSE_GET_BRAKELIGHT_FAULT(void);

uint8_t EFUSE_GET_LOWBEAM_FAULT(void);

uint8_t EFUSE_GET_HIGHBEAM_FAULT(void);

uint8_t EFUSE_GET_HORN_FAULT(void);

uint8_t EFUSE_GET_AUX_PORT_FAULT(void);

uint8_t EFUSE_GET_HEATED_GRIPS_FAULT(void);

uint8_t EFUSE_GET_HEATED_SEAT_FAULT(void);

uint8_t EFUSE_GET_CHARGE_CONTROLLER_FAULT(void);

uint8_t EFUSE_GET_MOTOR_CONTROLLER_FAULT(void);

uint8_t EFUSE_GET_BMS_CONTROLLER_FAULT(void);

uint8_t EFUSE_GET_SPARE_1_CONTROLLER_FAULT(void);


#endif	/* Efuse_H */

