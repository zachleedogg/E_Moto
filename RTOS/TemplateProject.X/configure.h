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
EVENT(FAIL) /* Runtime failure in the state machine */ \
EVENT(INIT) /* The transition out of init state */ \
EVENT(ENTRY) /* Put any entry code for a state when this state is thrown */ \
EVENT(EXIT) /* Exit code for states triggered by this event */ \
EVENT(TIMEUP) /* The timer-up event for timers */ \

/***********************************************************************************************
 List of Background Event Checkers
 ***********************************************************************************************/
#define EVENTCHECKER_HEADER "EventCheckers.h"
/*list the background function names in EventCheckers.h*/
#define EVENT_CHECK_FUNCS 

/***********************************************************************************************
 List of Services
 ***********************************************************************************************/
/*Put the name of any service */
#define SERVICE_LIST(SERVICE) \
SERVICE(FSM_TEMPLATE_SERVICE) /*Main state machine, This is always first (the default Service)*/ \


/*Put the name of each service*/
#define SERVICES FSM_TEMPLATE

/* file name of each service IN THE SAME ORDER AS ABOVE*/
#define SERVICE_1 "FSM_Template.h"
//#define SERVICE_2
//#define SERVICE_3
//#define SERVICE_4 
//#define SERVICE_5 
//#define SERVICE_6 
//#define SERVICE_7 
//#define SERVICE_8 

/***********************************************************************************************
 * Timers
 ***********************************************************************************************/

/*Select to which Service a timer posts a TimeUp Event, 0 is the Default Service*/
#define TIMER_0 0
#define TIMER_1 0
#define TIMER_2 0
#define TIMER_3 0
#define TIMER_4 0
#define TIMER_5 0
#define TIMER_6 0
#define TIMER_7 0
#define TIMER_8 0
#define TIMER_9 0
#define TIMER_10 0
#define TIMER_11 0
#define TIMER_12 0
#define TIMER_13 0
#define TIMER_14 0
#define TIMER_15 0





#define ENUM_FORM(WORD) WORD,

/* enum-ifies the event list*/
typedef enum{
    EVENT_LIST(ENUM_FORM)
    EVENTCOUNT
} EventType_t;

/* enum-ifies the event list*/
typedef enum{
    SERVICE_LIST(ENUM_FORM)
    SERVICECOUNT
} ServiceType_t;

/* string-ifies the event list */
#define STRING_FORM(WORD) #WORD,
static char *EventStrings[] = {
    EVENT_LIST(STRING_FORM)
};

/* The Event type */
typedef struct Event_t{
    EventType_t EventType;
    uint16_t EventParam;
    uint8_t EventPriority;
    uint8_t Service;
} Event;





#endif	/* CONFIGURE_H */

