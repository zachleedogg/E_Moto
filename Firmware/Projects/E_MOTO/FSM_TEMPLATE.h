/* 
 * File:   FSM_TEMPLATE.h
 * Author: Zachary Levenberg
 *
 * Created on June 10, 2016, 10:52 AM
 */

#include "configure.h"
#include "framework.h"
#include <stdint.h>

#ifndef FSM_TEMPLATE_H
#define	FSM_TEMPLATE_H



/**
 * This is the state machine function call
 * @param thisEvent takes an Event
 * @return the un-consumed event a NO_EVENT
 */
Event FSM_TEMPLATE(Event thisEvent);

#endif	/* FSM_TEMPLATE_H */

