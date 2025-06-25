/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EV_CHARGER_H
#define	EV_CHARGER_H

#include <stdint.h>

void EV_Charger_Init(void);
void EV_CHARGER_Run_10ms(void);

void EV_CHARGER_set_charge_voltage(float volts);
void EV_CHARGER_set_charge_current(float current);
float EV_CHARGER_get_charge_current();
float EV_CHARGER_get_charge_voltage();
uint8_t EV_CHARGER_get_bms_request_charge(void);

#endif	/* EV_CHARGER_H */

