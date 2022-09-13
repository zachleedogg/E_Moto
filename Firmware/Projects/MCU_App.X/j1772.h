/* 
 * File:   j1772.h
 * Author: kid group
 *
 * Created on June 17, 2018, 7:46 AM
 */

#ifndef J1772_H
#define	J1772_H

#include <stdint.h>

typedef enum prox_status_E {
    J1772_DISCONNECTED,
    J1772_CONNECTED,
    J1772_REQUEST_DISCONNECT,
    J1772_SNA_PROX
} prox_status_E;

void j1772Control_Init(void);

void j1772Control_Run_cont(void);

void j1772Control_Run_100ms(void);

void j1772Control_Halt(void);

prox_status_E j1772getProxState(void);

uint8_t j1772getPilotCurrent(void);

#endif	/* J1772_H */

