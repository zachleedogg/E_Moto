/* 
 * File:   touchScreenService.h
 * Author: Zachary Levenberg
 *
 * Created on June 10, 2016, 10:52 AM
 */

#include "configure.h"
#include "framework.h"
#include <stdint.h>

#ifndef touchScreenService_H
#define	touchScreenService_H

/**
 * This is the state machine function call
 * @param thisEvent takes an Event
 * @return the un-consumed event a NO_EVENT
 */
Event touchScreenService(Event thisEvent);

#endif	/* touchScreenService_H */

