/* 
 * File:   configure.h
 * Author: kid group
 *
 * Created on June 9, 2016, 8:07 PM
 */

#include <stdint.h>

#ifndef CONFIGURE_H
#define	CONFIGURE_H

/***********************************************************************************************
 List of Event Types
 ***********************************************************************************************/
/* Put the names of any events here is this list */
#define EVENT_LIST(EVENT) \
EVENT(NO_EVENT) \
EVENT(FAIL_EVENT) /* Runtime failure in the state machine */ \
EVENT(INIT_EVENT) /* The transition out of init state */ \
EVENT(ENTRY_EVENT) /* Put any entry code for a state when this state is thrown */ \
EVENT(EXIT_EVENT) /* Exit code for states triggered by this event */ \
EVENT(TIMEUP_EVENT) /* The timer-up event for timers */ \

/***********************************************************************************************
 List of Background Event Checkers
 ***********************************************************************************************/
#define EVENTCHECKER_HEADER "EventCheckers.h"
/*list the background function names in EventCheckers.h*/
#define EVENT_CHECK_FUNCS 

/* enum-ifies the event list*/
#define ENUM_FORM(WORD) WORD,
typedef enum{
    EVENT_LIST(ENUM_FORM)
    EVENTCOUNT
} EventType_t;

/* string-ifies the event list */
#define STRING_FORM(WORD) #WORD,
static char __attribute__ ((unused)) *EventStrings[] = {
    EVENT_LIST(STRING_FORM)
};

/***********************************************************************************************
 List of Services
 ***********************************************************************************************/

/*Put the name of each service*/
#define SERVICE_LIST(SERVICE) \
SERVICE(FSM_TEMPLATE) /*Main state machine, This is always first (the default Service)*/ \

/* file name of each service IN THE SAME ORDER AS ABOVE*/
#define SERVICE_1 "FSM_Template.h"
//#define SERVICE_2
//#define SERVICE_3
//#define SERVICE_4 
//#define SERVICE_5 
//#define SERVICE_6 
//#define SERVICE_7 
//#define SERVICE_8 


/* enum-ifies the service list*/
#define SERVICE_FORM(WORD) WORD##_SERVICE,
#define SERVICES SERVICE_LIST(ENUM_FORM)
typedef enum{
    SERVICE_LIST(SERVICE_FORM)
    SERVICECOUNT
} ServiceType_t;

/* string-ifies the service list */
static char __attribute__ ((unused)) *ServiceStrings[] = {
    SERVICE_LIST(STRING_FORM)
};

/* The Event type */
typedef struct Event_t{
    EventType_t EventType;
    uint16_t EventParam;
    uint8_t EventPriority;
    ServiceType_t Service;
} Event;

#endif	/* CONFIGURE_H */

