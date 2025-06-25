/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LV_BATTERY_H
#define	LV_BATTERY_H

#include <stdint.h>

typedef enum {
    LV_BATTERY_NOMINAL,
    LV_BATTERY_CHARGED,
    LV_BATTERY_CHARGING,
    LV_BATTERY_CHARGE_NEEDED,
} lvBatteryState_E;

typedef enum {
    LV_BATTERY_CHARGE_STATE_CHARGE_NEEDED,
    LV_BATTERY_CHARGE_STATE_CHARGING,
    LV_BATTERY_CHARGE_STATE_CHARGED
} lvBatteryChargeState_E;

typedef enum {
    LV_BATTERY_HEALTH_STATE_DISCHARGED,
    LV_BATTERY_HEALTH_STATE_NOMINAL,
    LV_BATTERY_HEALTH_STATE_CHARGED
} lvBatteryHealthState_E;


void lvBattery_Init(void);

void lvBattery_Run_10ms(void);

void lvBattery_Halt(void);

lvBatteryState_E lvBattery_GetState(void);


#endif	/* LV_BATTERY_H */

