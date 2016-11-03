/* 
 * File:   framework.h
 * Author: Zachary Levenberg
 *
 * Created on June 9, 2016, 7:52 PM
 */
#ifndef FRAMEWORK_H
#define	FRAMEWORK_H
#include <stdint.h>
#include "framework_configure.h"

/*******************************************************************************
 * Defines & Datatypes
 * ****************************************************************************/

#define TRUE 1
#define FALSE 0

/* Timer defines */
typedef enum _FRAMEWORK_timerNumber_E {
    TIMER0,
    TIMER1,
    TIMER2,
    TIMER3,
    TIMER4,
    TIMER5,
    TIMER6,
    TIMER7,
    TIMER8,
    TIMER9,
    TIMER10,
    TIMER11,
    TIMER12,
    TIMER13,
    TIMER15,
    NUMBER_OF_SW_TIMERS,
} FRAMEWORK_timerNumber_E;

typedef enum _FRAMEWORK_timerMode_E {
    SINGLE_SHOT,
    CONTINUOUS
} FRAMEWORK_timerMode_E;

/*Function pointer protoype for runtime scheduler*/
typedef void(*FRAMEWORK_scheduleTask)();

/* Event defines */
#define EMPTY (Event){NO_EVENT, 0x0000, 0x0000, 0x0000}
#define INIT (Event){INIT_EVENT, 0x0000, 0x0000, 0x0000}
#define EXIT (Event){EXIT_EVENT, 0x0000, 0x0000, 0x0000}
#define ENTRY (Event){ENTRY_EVENT, 0x0000, 0x0000, 0x0000}


/******************************************************************************/
/*Function Prototypes*/
/******************************************************************************/

/**
 * FRAMEWORK_run the framwork initialization
 * @return 
 */
uint8_t FRAMEWORK_init();


/**
 * Main framework run function
 * @return 
 */
uint8_t FRAMEWORK_run();

/**
 * Post an event into the priority queue
 * @param thisEvent: contains type, param, service, and priority
 * @return 
 */
uint8_t FRAMEWORK_postEvent(Event thisEvent);

/**
 * Initializes the framework millisecond timer
 * @param clockFreq: from clockfreq() return value
 * @return 
 */
uint8_t FRAMEWORK_timerInit(uint32_t clockFreq);

/**
 * Resets the 45-day freerunning timer, should be performed periodically or on 
 * system wake from sleep
 */
void FRAMEWORK_resetRunningTimer(void);

/**
 * Returns the value of the freerunning timer in milliseconds
 * @return 
 */
uint32_t FRAMEWORK_getTimeNow(void);

/**
 * Sets a one-shot timer and throws a TIMEUP Event upon completion
 * @param thisTimer: Timer number to set (0-15)
 * @param time: the time in milliseconds
 * @param service: the service in which the TIMEUP Event is posted to.
 */
void FRAMEWORK_timerSet(FRAMEWORK_timerNumber_E thisTimer, uint16_t time, ServiceType_t service, FRAMEWORK_timerMode_E Mode);

/**
 * Halts a timer before it expires
 * @param thisTimer: timer to be halted (0-15)
 */
void FRAMEWORK_timerStop(FRAMEWORK_timerNumber_E thisTimer);

/**
 * Resumes a timer that has been halted
 * @param thisTimer: timer to be resumed (0-15)
 */
void FRAMEWORK_timerResume(FRAMEWORK_timerNumber_E thisTimer);


/**
 * Initialize the run-time task scheduler
 */
void FRAMEWORK_schedulerInit(void);

/**
 * Adds a task to the scheduler
 * @param someFunction: pointer to the function to be scheduled, must be a
 * void function and take no arguments
 * @param time: time in milliseconds from current time to execute the task
 * @return 
 */
uint8_t FRAMEWORK_schedulerAdd(FRAMEWORK_scheduleTask someFunction, uint32_t time);

#endif	/* FRAMEWORK_H */

