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

/**************************************************************************************************
 Define states here
 **************************************************************************************************/
#define FSM_TEMPLATE_States_List(state)\
state(init) /* init state for startup code */ \
state(someState) /* locked state before safemode */ \
state(anotherState) /* welcome state before locked */ \
state(yetAnotherState) /* locked state before safemode */ \

typedef enum{
    FSM_TEMPLATE_States_List(ENUM_FORM)
}FSM_TEMPLATE_State_t;

static const char *StateStrings[] = {
    FSM_TEMPLATE_States_List(STRING_FORM)
};

/**
 * This is the state machine function call
 * @param thisEvent takes an Event
 * @return the un-consumed event a NO_EVENT
 */
Event FSM_TEMPLATE(Event thisEvent);

#endif	/* FSM_TEMPLATE_H */

