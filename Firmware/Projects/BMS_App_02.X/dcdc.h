/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DCDC_H
#define	DCDC_H

typedef enum {
    DCDC_OFF,
    DCDC_PRECHARGE,
    DCDC_ENABLE,
    DCDC_FAULT
} DCDC_state_E;

void DCDC_init(void);

void DCDC_run_1ms(void);

void DCDC_run_100ms(void);

void DCDC_halt();

DCDC_state_E DCDC_getState(void);

float DCDC_getVoltage(void);

float DCDC_getCurrent(void);

#endif	/* DCDC_H */

