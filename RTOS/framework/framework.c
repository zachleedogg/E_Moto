/*This is a Framework for the BOLT MOTORBIKES embedded system*/

#include "framework.h"
#include EVENTCHECKER_HEADER
#include <xc.h>
#include <stdio.h>
#include SERVICE_1
#ifdef SERVICE_2
#include SERVICE_2
#endif
#ifdef SERVICE_3
#include SERVICE_3
#endif
#ifdef SERVICE_4
#include SERVICE_4
#endif
#ifdef SERVICE_5
#include SERVICE_5
#endif
#ifdef SERVICE_6
#include SERVICE_6
#endif
#ifdef SERVICE_7
#include SERVICE_7
#endif


#define QUEUE_SIZE 32
#define PRIORITY_LEVELS 3

/*Event queue*/
typedef struct _queue {
    Event eventQueue[QUEUE_SIZE];
    uint16_t size;
    uint16_t head;
    uint16_t tail;
} queue;



/* Event check function pointer type*/
typedef uint8_t(*EventFunc_t)();
typedef Event(*ServiceFuncList)(Event);
EventFunc_t const EventCheckList[] = {EVENT_CHECK_FUNCS};
ServiceFuncList const ServiceList[] = {SERVICES};
static uint8_t numberofCheckers = (uint8_t)sizeof (EventCheckList) / sizeof (EventFunc_t);
static uint8_t numberofServices = (uint8_t)sizeof (ServiceList) / sizeof (ServiceFuncList);

/*Queue Array*/
static queue QueueList[PRIORITY_LEVELS] = {};

/*Timer Array*/
static const uint8_t TimerArray[] = {TIMER_0, TIMER_1, TIMER_2,
    TIMER_3, TIMER_4, TIMER_5, TIMER_6, TIMER_7, TIMER_8, TIMER_9,
    TIMER_10, TIMER_11, TIMER_12, TIMER_13, TIMER_14, TIMER_15};

uint8_t EnQueue(queue *thisQueue, Event thisEvent);
Event DeQueue(queue *thisQueue);
uint8_t CheckForEvents();

uint8_t Post(Event thisEvent) {
    uint8_t i = 0;
    for (i = 0; i < PRIORITY_LEVELS; i++) {
        if (thisEvent.EventPriority == i) {
            EnQueue(&QueueList[i], thisEvent);
            break;
        }
    }
    return 0;
}

uint8_t Init() {
    Event ThisEvent = INIT_EVENT;
    uint8_t S = 0;

    for (S = 0; S < numberofServices; S++) {/*Check through each service*/
        /*post init event to each service*/
        ServiceList[S](ThisEvent);
    }
    return 1;
}

uint8_t Run() {
    Event ThisEvent;
    int8_t Q = 0;
    uint8_t S = 0;

    //printf("# of Event Checkers: %d,\n# of Services: %d\n", numberofCheckers, numberofServices);

    while (1) {
        for (Q = PRIORITY_LEVELS - 1; Q >= 0; Q--) {/*Check each priority level*/
            while (QueueList[Q].size != 0) {/*If the queue is not empty*/
                ThisEvent = DeQueue(&QueueList[Q]); /*DeQueue the event*/
                for (S = 0; S < numberofServices; S++) {/*Check through each service*/
                    if (ThisEvent.Service == S) {/*post event to the correct service*/
                        ServiceList[S](ThisEvent);
                        break;
                    }
                }
            }
        }
        /* empty queues, check for user events */
        CheckForEvents();
    }
}

uint8_t CheckForEvents() {
    uint8_t EventFound = 0;
    uint8_t i = 0;
    /* Loop through executing checker functions */
    for (i = 0; i < numberofCheckers; i++) {
        EventFound = EventCheckList[i]();
        if (EventFound == 1) { /* Event Found */
            break;
        }
    }
    return EventFound;
}

//uint8_t CheckTimers(){
//    while(TimerQueue.tail != TimerQueue.head) {/*If the Queue is not empty*/
//        Post(TimerQueue.eventQueue[TimerQueue.tail]); /*Dequeue the data and post it*/
//        TimerQueue.eventQueue[TimerQueue.tail++] = EMPTY_EVENT;
//        TimerQueue.size--;
//        if (TimerQueue.tail == QUEUE_SIZE) {/*wrap-around protection*/
//            TimerQueue.tail = 0;
//        }
//
//    }
//}

uint8_t EnQueue(queue *thisQueue, Event thisEvent) {
    thisQueue->eventQueue[thisQueue->head++] = thisEvent; /*Put Data in the Q*/
    thisQueue->size++;
    if (thisQueue->head == QUEUE_SIZE) {/*wrap-around protection*/
        thisQueue->head = 0;
    }
    if (thisQueue->size == QUEUE_SIZE) {
        return 1;
    }
    return 0;
}

Event DeQueue(queue *thisQueue) {
    Event thisEvent = EMPTY_EVENT;
    if (thisQueue->tail != thisQueue->head) {/*If the Queue is not empty*/
        thisEvent = thisQueue->eventQueue[thisQueue->tail]; /*Dequeue the data*/
        thisQueue->eventQueue[thisQueue->tail++] = EMPTY_EVENT;
        thisQueue->size--;
        if (thisQueue->tail == QUEUE_SIZE) {/*wrap-around protection*/
            thisQueue->tail = 0;
        }

    }
    return thisEvent; /*Return the event*/
}


static volatile uint32_t runningTime = 0;
static uint8_t milliseond = 0;

typedef struct _sw_timer {
    uint16_t time;
    uint16_t threshold;
    sw_timer_status status;
} sw_timer;

static sw_timer SW_timers[NUMBER_OF_SW_TIMERS];

/*
 Function: T5_Interrupt_Init()
 Param: period: the peroid at which the interrupt fires, 0-0xFFFF
        priority: the priority of the interrupt, 1-7
 Return: void
 Description: Initializes timer 5 interrupt
 author: brdgordo - 1/28/16
 */
uint8_t Timer_Init(uint32_t clockFreq) {
    uint32_t currentSpeed = clockFreq / 2;

    uint16_t TicksPerMS = (uint16_t) (currentSpeed / 2000);
    /* Initialize timer */
    T5CONbits.TON = 0;

    T5CON = 0; /* Clear timer config register */

    T5CONbits.TCKPS = 0; /* prescaler set to 0 */


    TMR5 = 0x00; /*Clear Timers*/

    PR5 = TicksPerMS; /*set timer period */

    /* Enable level 1-7 interrupts */
    /* No restoring of previous CPU IPL state performed here */
    INTCON2bits.GIE = 1;

    /* set timer interrupt priority */
    _T5IP = 4;

    /* reset timer interrupt */
    _T5IF = 0;

    /* Enable timer interrupt */
    _T5IE = 1;

    /* Turn timer on */
    T5CONbits.TON = 1;
    return 0;
}

void FreeRunningTimerReset(void) {
    runningTime = 0;
}

uint32_t FreeRunningTimer(void) {
    return runningTime;
}

void SW_Timer_Set(sw_timer_number thisTimer, uint16_t time) {
    SW_timers[thisTimer].time = time;
    SW_timers[thisTimer].status = RUNNING;
}

void SW_Timer_Stop(sw_timer_number thisTimer) {
    SW_timers[thisTimer].status = OFF;
}

void SW_Timer_Resume(sw_timer_number thisTimer) {
    SW_timers[thisTimer].status = RUNNING;
}

void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void) {
    /* clear timer x interrupt */
    _T5IF = 0;

    /*This is a 1/2 millisecond timer, so only count every other for milliseconds*/
    milliseond = ~milliseond;

    /*Put your code here*/
    if (!milliseond) {
        runningTime++;
        int i;
        for (i = 0; i < NUMBER_OF_SW_TIMERS; i++) {

            if (SW_timers[i].status == RUNNING) {
                SW_timers[i].time--;
                if (SW_timers[i].time == 0) {
                    SW_timers[i].status = DONE;
                    Event ThisEvent = {};
                    ThisEvent.EventType = TIMEUP;
                    ThisEvent.EventParam = i;
                    ThisEvent.EventPriority = 2;
                    ThisEvent.Service = TimerArray[i];
                    Post(ThisEvent);
                }
            }
        }
    }
}