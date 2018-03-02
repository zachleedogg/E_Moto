/* 
 * File:   configure.h
 * Author: kid group
 *
 * Created on June 9, 2016, 8:07 PM
 */
#ifndef CONFIGURE_H
#define	CONFIGURE_H

#include <stdint.h>

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
EVENT(DEBUG_MESSAGE_EVENT) /*Incoming debug message*/ \

/* enum-ifies the event list*/
#define ENUM_FORM(WORD) WORD,

typedef enum {
    EVENT_LIST(ENUM_FORM)
    EVENTCOUNT
} FRAMEWORK_eventType_E;

/* string-ifies the event list */
#define STRING_FORM(WORD) #WORD,
static const char __attribute__((unused)) * const EventStrings[] = {
    EVENT_LIST(STRING_FORM)
};

/***********************************************************************************************
 List of Priority Levels
 ***********************************************************************************************/
typedef enum _priority {
    FRAMEWORK_PRIORITY_1,
    FRAMEWORK_PRIORITY_2,
    FRAMEWORK_PRIORITY_3,
    FRAMEWORK_PRIORITY_LEVELS
} FRAMEWORK_priority_E;

#define FRAMEWORK_QUEUE_SIZE 8

/***********************************************************************************************
 List of Services
 ***********************************************************************************************/

/*Put the name of each service*/
#define SERVICE_LIST(SERVICE) \
SERVICE(templateService) /*Main state machine, This is always first (the default Service)*/ \
SERVICE(debuggerService) /*This debugger service handles the Uart interface to PC */ \


/* file name of each service IN THE SAME ORDER AS ABOVE*/
#define SERVICE_1 "templateService.h"
#define SERVICE_2 "debuggerService.h"
//#define SERVICE_3
//#define SERVICE_4 
//#define SERVICE_5 
//#define SERVICE_6 
//#define SERVICE_7 
//#define SERVICE_8 


/* enum-ifies the service list*/
#define SERVICE_FORM(WORD) WORD##_SERVICE,
#define SERVICES SERVICE_LIST(ENUM_FORM)

typedef enum {
    SERVICE_LIST(SERVICE_FORM)
    SERVICECOUNT
} FRAMEWORK_serviceType_E;

/* string-ifies the service list */
static const char __attribute__((unused)) * const ServiceStrings[] = {
    SERVICE_LIST(STRING_FORM)
};

/* The Event type */
typedef struct Event_t {
    FRAMEWORK_eventType_E EventType;
    uint16_t EventParam;
    FRAMEWORK_priority_E EventPriority;
    FRAMEWORK_serviceType_E Service;
} Event;

#endif	/* CONFIGURE_H */

