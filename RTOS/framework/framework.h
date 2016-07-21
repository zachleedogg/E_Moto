/* 
 * File:   framework.h
 * Author: kid group
 *
 * Created on June 9, 2016, 7:52 PM
 */

#include <stdint.h>
#include "configure.h"

#ifndef FRAMEWORK_H
#define	FRAMEWORK_H

/******************************************************************************/
/*Framework Global Defines*/
/******************************************************************************/

#define TRUE 1
#define FALSE 0

/* Timer defines */
typedef enum _sw_timer_number {
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
} sw_timer_number;

typedef enum _sw_timer_status {
    OFF,
    RUNNING,
    DONE,
} sw_timer_status;

/* Event defines */
#define EMPTY_EVENT (Event){NO_EVENT, 0x0000, 0x0000, 0x0000}
#define INIT_EVENT (Event){INIT, 0x0000, 0x0000, 0x0000}
#define EXIT_EVENT (Event){EXIT, 0x0000, 0x0000, 0x0000}
#define ENTRY_EVENT (Event){ENTRY, 0x0000, 0x0000, 0x0000}


/******************************************************************************/
/*Function Prototypes*/
/******************************************************************************/
/*Run the framwork initialization*/
uint8_t Init();

/*Main framework run function*/
uint8_t Run();

/*Service posting Fucntion*/
uint8_t Post(Event thisEvent);

/*Timer function*/
uint8_t Timer_Init(uint32_t clockFreq);

void FreeRunningTimerReset(void);

uint32_t FreeRunningTimer(void);

void SW_Timer_Set(sw_timer_number thisTimer, uint16_t time);

void SW_Timer_Stop(sw_timer_number thisTimer);

void SW_Timer_Resume(sw_timer_number thisTimer);


#endif	/* FRAMEWORK_H */

