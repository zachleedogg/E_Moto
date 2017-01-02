/* 
 * File:   canService.h
 * Author: Zachary Levenberg
 *
 * Created on December 22, 2016, 10:52 AM
 */

#include "framework.h"
#include <stdint.h>

#ifndef canService_H
#define	canService_H

/**
 * This is the state machine function call
 * @param thisEvent takes an Event
 * @return the un-consumed event a NO_EVENT
 */
Event canService(Event thisEvent);

void can_Debug(uint8_t state);

#endif	/* canService_H */

