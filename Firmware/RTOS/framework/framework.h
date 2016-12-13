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

/* Event defines */
#define EMPTY (Event){NO_EVENT, 0x0000, 0x0000, 0x0000}
#define INIT (Event){INIT_EVENT, 0x0000, 0x0000, 0x0000}
#define EXIT (Event){EXIT_EVENT, 0x0000, 0x0000, 0x0000}
#define ENTRY (Event){ENTRY_EVENT, 0x0000, 0x0000, 0x0000}


/******************************************************************************/
/*Function Prototypes*/
/******************************************************************************/

/**
 * Main framework run function
 * @param clockFreq: System Frequency in Hz
 * @return 
 */
uint8_t FRAMEWORK_run(uint32_t clockFreq);

/**
 * Post an event into the priority queue
 * @param thisEvent: contains type, param, service, and priority
 * @return 
 */
uint8_t FRAMEWORK_postEvent(Event thisEvent);

/**
 * Enable or disable debug messages
 * @param state Enabled (1) or Disabled (0)
 */
void FRAMEWORK_Debug(uint8_t state);

/**
 * Enable or disable debug messages
 * @param state Enabled (1) or Disabled (0)
 */
void FRAMEWORK_TASKRUNNER_debug(uint8_t state);

/**
 * Get the current CPU utilization as a percentage
 * @return an integer between 1 and 100
 */
uint8_t FRAMEWORK_getCPUPercentage(void);

/**
 * Resets the 45-day freerunning timer, should be performed periodically or on 
 * system wake from sleep
 */
void FRAMEWORK_resetTimeNow(void);

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
 * @param Mode: the type of timer to run (continuous or one-shot)
 */
void FRAMEWORK_timerSet(FRAMEWORK_timerNumber_E thisTimer, uint16_t time, FRAMEWORK_serviceType_E service, FRAMEWORK_timerMode_E Mode);

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

#endif	/* FRAMEWORK_H */

