/* 
 * File:   debuggerService.h
 * Author: Zachary Levenberg
 *
 * Created on June 10, 2016, 10:52 AM
 */

#include "framework.h"
#include <stdint.h>

#ifndef debuggerService_H
#define	debuggerService_H

/**
 * This is the state machine function call
 * @param thisEvent takes an Event
 * @return the un-consumed event a NO_EVENT
 */
Event debuggerService(Event thisEvent);

void debug_Debug(uint8_t state);

#endif	/* debuggerService_H */

