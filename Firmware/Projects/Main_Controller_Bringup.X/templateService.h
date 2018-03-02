/* 
 * File:   templateService.h
 * Author: Zachary Levenberg
 *
 * Created on June 10, 2016, 10:52 AM
 */

#include "framework_configure.h"
#include "framework.h"
#include <stdint.h>

#ifndef templateService_H
#define	templateService_H

void template_Debug(uint8_t state);

/**
 * This is the state machine function call
 * @param thisEvent takes an Event
 * @return the un-consumed event a NO_EVENT
 */
Event templateService(Event thisEvent);

#endif	/* templateService_H */

