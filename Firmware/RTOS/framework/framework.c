/* 
 * File:   framework.h
 * Author: Zachary Levenberg
 *
 * Created on June 9, 2016, 7:52 PM
 */

#include <xc.h>
#include "framework.h"

/*******************************************************************************
 * Debugging
 * ****************************************************************************/
#ifdef DEBUG
#include <stdio.h>
#include "bolt_uart.h"
static uint8_t debugEnable = 1;
#define framework_print(...) if(debugEnable){char tempArray[20]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
#define framework_print(...)
#endif

/*******************************************************************************
 *******************************************************************************
 * Event Framework Queue
 *******************************************************************************
 *******************************************************************************/
#include "framework_taskRunner.h"
//#include EVENTCHECKER_HEADER
#ifdef SERVICE_1
#include SERVICE_1
#endif
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

#define FRAMEWORK_QUEUE_SIZE 4

/*******************************************************************************
 * Datatypes and Variables
 ******************************************************************************/

/*Event queue*/
typedef struct _framework_queue_S {
    Event eventQueue[FRAMEWORK_QUEUE_SIZE];
    uint16_t size;
    uint16_t head;
    uint16_t tail;
} framework_queue_S;
static volatile framework_queue_S QueueList[FRAMEWORK_PRIORITY_LEVELS] = {};

/*Active Priority Level*/
static volatile int8_t framework_ActivePriorityLevel = -1;

/* Event check function pointer*/
//typedef uint8_t(*framework_EventFunctionList_F)();
//framework_EventFunctionList_F const EventCheckList[] = {EVENT_CHECK_FUNCS};
//static uint8_t numberofCheckers = (uint8_t)sizeof (EventCheckList) / sizeof (framework_EventFunctionList_F);

/* Service function pointer*/
typedef Event(*framwork_serviceFunctionList_F)(Event);
framwork_serviceFunctionList_F const ServiceList[] = {SERVICES FRAMEWORK_TASKRUNNER_run};
static uint8_t numberofServices = (uint8_t)sizeof (ServiceList) / sizeof (framwork_serviceFunctionList_F);

static uint32_t cpuUsage = 0;

typedef struct _average {
    uint16_t cpu_ticks[10];
    uint32_t sum;
    uint8_t index;
} average;
static average cpuUsageAverage = {};

/*******************************************************************************
 * Module Function Prototypes
 ******************************************************************************/
/**
 * FRAMEWORK_run the framwork initialization
 * @return 
 */
static uint8_t FRAMEWORK_init();
uint8_t FRAMEWORK_timerInit(uint32_t clockFreq);


/*Event Queue functions*/
static uint8_t EnQueue(volatile framework_queue_S *thisQueue, Event thisEvent);
static Event DeQueue(volatile framework_queue_S *thisQueue);
//static uint8_t CheckForEvents(void);

/*Timer functions*/
static uint8_t timerHasTicked(void);
static void checkTimers(void);

/*Task Scheduler functions*/
//static uint8_t scheduler_remove(uint32_t time);

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
uint8_t FRAMEWORK_postEvent(Event thisEvent) {
    if ((int8_t) thisEvent.EventPriority > framework_ActivePriorityLevel) {
        framework_ActivePriorityLevel = thisEvent.EventPriority;
    }
    EnQueue(&QueueList[thisEvent.EventPriority], thisEvent);
    return 0;
}

uint8_t FRAMEWORK_run(uint32_t clockFreq) {
    Event ThisEvent;

    FRAMEWORK_init();

    FRAMEWORK_timerInit(clockFreq);

    while (1) {

        /*If priority Level is active, run services.*/
        while (framework_ActivePriorityLevel >= 0) {
            while (QueueList[framework_ActivePriorityLevel].size > 0) {
                /*DeQueue the event and Run the Serice with the Event*/
                ThisEvent = DeQueue(&QueueList[framework_ActivePriorityLevel]);
                Event cleanEvent = ThisEvent;
                cleanEvent.Service = 0;
                cleanEvent.EventPriority = 0;
                ServiceList[ThisEvent.Service](cleanEvent);
            }
            /*when the priority level is clear, reduce the priority*/
            framework_ActivePriorityLevel--;
        }
        /*Background check for events*/
        //CheckForEvents();

        cpuUsage++;

        /*Background check timers*/
        if (timerHasTicked()) {
            static uint16_t dumbSecondCounter = 0;
            dumbSecondCounter++;
            if (dumbSecondCounter == 1000) {
                dumbSecondCounter = 0;
                framework_print("FRAMEWORK CPU_USAGE %lu\n", cpuUsageAverage.sum / 10);
            }
            cpuUsageAverage.sum -= cpuUsageAverage.cpu_ticks[cpuUsageAverage.index];
            cpuUsageAverage.cpu_ticks[cpuUsageAverage.index] = cpuUsage;
            cpuUsageAverage.sum += cpuUsage;
            cpuUsage = 0;
            cpuUsageAverage.index++;
            if (cpuUsageAverage.index >= 10) {
                cpuUsageAverage.index = 0;
            }
            /*run timer service*/
            /* I want to put this here, but the framework runs better with it
             in the Timer ISR. I can't figure out why, of a for-loop of 16 items.*/
            //checkTimers();

            /*run task scheduler*/
            //scheduler_remove(FRAMEWORK_getTimeNow());
        }

    }
}

//static uint8_t CheckForEvents() {
//    uint8_t EventFound = 0;
//    uint8_t i = 0;
//    /* Loop through executing checker functions */
//    for (i = 0; i < numberofCheckers; i++) {
//        EventFound = EventCheckList[i]();
//        if (EventFound == 1) { /* Event Found */
//            break;
//        }
//    }
//    return EventFound;
//}

static uint8_t FRAMEWORK_init(void) {
    Event ThisEvent = INIT;
    uint8_t S = 0;

    FRAMEWORK_TASKRUNNER_init();

    for (S = 0; S < numberofServices; S++) {/*Check through each service*/
        /*post init event to each service*/
        ServiceList[S](ThisEvent);
    }
    return 1;
}

void FRAMEWORK_Debug(uint8_t state){
    debugEnable = state;
}

static uint8_t EnQueue(volatile framework_queue_S *thisQueue, Event thisEvent) {
    thisQueue->eventQueue[thisQueue->head++] = thisEvent; /*Put Data in the Q*/
    thisQueue->size++;
    if (thisQueue->head == FRAMEWORK_QUEUE_SIZE) {/*wrap-around protection*/
        thisQueue->head = 0;
    }
    if (thisQueue->size >= FRAMEWORK_QUEUE_SIZE) {
        framework_print("Framework_Q_Error\n");
        return 1;
    }
    return 0;
}

static Event DeQueue(volatile framework_queue_S *thisQueue) {
    Event thisEvent = EMPTY;
    if (thisQueue->tail != thisQueue->head) {/*If the Queue is not empty*/
        thisEvent = thisQueue->eventQueue[thisQueue->tail]; /*Dequeue the data*/
        thisQueue->eventQueue[thisQueue->tail++] = EMPTY;
        thisQueue->size--;
        if (thisQueue->tail == FRAMEWORK_QUEUE_SIZE) {/*wrap-around protection*/
            thisQueue->tail = 0;
        }

    }
    return thisEvent; /*Return the event*/
}


/******************************************************************************
 *******************************************************************************
 *******************************************************************************
 * Software Timers
 *******************************************************************************
 *******************************************************************************
 *******************************************************************************/

#define STATUS_Q_SIZE 8

/*******************************************************************************
 * Data Structures and variables
 *******************************************************************************/
static volatile uint32_t runningTime = 0;

typedef enum _FRAMEWORK_swTimerStatus_E {
    OFF,
    RUNNING,
    DONE,
} FRAMEWORK_swTimerStatus_E;

typedef struct _FRAMEWORK_swTimer_S {
    uint16_t time;
    uint16_t threshold;
    FRAMEWORK_swTimerStatus_E status;
    FRAMEWORK_serviceType_E service;
    FRAMEWORK_timerMode_E mode;
} FRAMEWORK_swTimer_S;

static volatile FRAMEWORK_swTimer_S SW_timers[NUMBER_OF_SW_TIMERS];

typedef struct _FRAMEWORK_swTimerStatusQueue_S {
    uint8_t queue[STATUS_Q_SIZE];
    uint8_t head;
    uint8_t tail;
} FRAMEWORK_swTimerStatusQueue_S;

static volatile FRAMEWORK_swTimerStatusQueue_S timerStatus = {};

uint8_t halfMillis = 0;

/*******************************************************************************
 * Private Functions
 *******************************************************************************/
static uint8_t timerHasTicked() {
    uint8_t returnVal = 0;
    /*Check if timer has ticked and deQueue timer bit*/
    if (timerStatus.queue[timerStatus.tail] == 1) {
        timerStatus.queue[timerStatus.tail++] = 0;
        /*Wrap around protection*/
        if (timerStatus.tail >= STATUS_Q_SIZE) {
            timerStatus.tail = 0;
        }
        if (timerStatus.tail != timerStatus.head) {
            framework_print("p\n");
        }
        returnVal = 1;
    }
    return returnVal;
}

static void checkTimers(void) {

    /*Check active timers*/
    int i;
    for (i = 0; i < NUMBER_OF_SW_TIMERS; i++) {
        /*If timer is running, decriment timer and post event if expired*/
        if (SW_timers[i].status == RUNNING) {
            SW_timers[i].time--;
            if (SW_timers[i].time == 0) {
                if (SW_timers[i].mode == CONTINUOUS) {
                    SW_timers[i].time = SW_timers[i].threshold;
                } else {
                    SW_timers[i].status = DONE;
                }
                Event ThisEvent;
                ThisEvent.EventType = TIMEUP_EVENT;
                ThisEvent.EventParam = i;
                ThisEvent.EventPriority = FRAMEWORK_PRIORITY_2;
                ThisEvent.Service = SW_timers[i].service;
                FRAMEWORK_postEvent(ThisEvent);
            }
        }
    }
}

/*******************************************************************************
 * Public Functions
 *******************************************************************************/

uint8_t FRAMEWORK_timerInit(uint32_t clockFreq) {
    uint32_t currentSpeed = clockFreq / 2;

    uint32_t TicksPerMS = (currentSpeed / 2000);

    /* Initialize timer */
    T5CONbits.TON = 0;
    T5CON = 0; /* Clear timer config register */
    T5CONbits.TCKPS = 0; /* prescaler set to 0 */
    TMR5 = 0x00; /*Clear Timers*/
    PR5 = (uint16_t) TicksPerMS; /*set timer period */

    /* Enable level 1-7 interrupts */
    /* No restoring of previous CPU IPL state performed here */
    INTCON2bits.GIE = 1;

    /* set timer interrupt priority */
    _T5IP = 7;
    /* reset timer interrupt */
    _T5IF = 0;
    /* Enable timer interrupt */
    _T5IE = 1;

    /* Turn timer on */
    T5CONbits.TON = 1;
    return 0;
}

void FRAMEWORK_resetRunningTimer(void) {
    runningTime = 0;
}

uint32_t FRAMEWORK_getTimeNow(void) {
    return runningTime;
}

void FRAMEWORK_timerSet(FRAMEWORK_timerNumber_E thisTimer, uint16_t time, FRAMEWORK_serviceType_E service, FRAMEWORK_timerMode_E Mode) {
    SW_timers[thisTimer].time = time;
    SW_timers[thisTimer].threshold = time;
    SW_timers[thisTimer].status = RUNNING;
    SW_timers[thisTimer].service = service;
    SW_timers[thisTimer].mode = Mode;
}

void FRAMEWORK_timerStop(FRAMEWORK_timerNumber_E thisTimer) {
    SW_timers[thisTimer].status = OFF;
}

void FRAMEWORK_timerResume(FRAMEWORK_timerNumber_E thisTimer) {
    SW_timers[thisTimer].status = RUNNING;
}

void __attribute__((__interrupt__, __auto_psv__, __shadow__)) _T5Interrupt(void) {
    /* clear timer interrupt Flag*/
    _T5IF = 0;

    /*this interrupt occurs every half millisecond*/
    static uint8_t toggle = 0;
    if (toggle == 0) {
        toggle = 1;
    } else {
        toggle = 0;
        /*Put your code here*/
        runningTime++;
        timerStatus.queue[timerStatus.head++] = 1;
        if (timerStatus.head >= STATUS_Q_SIZE) {
            timerStatus.head = 0;
        }

        /*Check through SW timers*/
        checkTimers();

        /*Every millisecond, run the task scheduler*/
        Event taskEvent;
        taskEvent.EventPriority = FRAMEWORK_PRIORITY_3;
        taskEvent.EventType = NO_EVENT;
        taskEvent.Service = numberofServices - 1;
        FRAMEWORK_postEvent(taskEvent);
    }
}


/******************************************************************************
 *******************************************************************************
 *******************************************************************************
 * Task Scheduler
 *******************************************************************************
 *******************************************************************************
 *******************************************************************************/

//#define SCHEDULE_SIZE 8
//
///*******************************************************************************
// * Data Structures
// ******************************************************************************/
//typedef struct _node {
//    uint32_t time;
//    FRAMEWORK_scheduleTask thisFunction;
//    struct _node * nextNode;
//    struct _node * prevNode;
//} node;
//
//typedef struct _stack {
//    node* stack[SCHEDULE_SIZE];
//    uint8_t stackPtr;
//} stack;
//
//static node scheduleArray[SCHEDULE_SIZE];
//
//static stack scheduleStack;
//
//static node Head;
//
///*******************************************************************************
// * Private Functions
// ******************************************************************************/
//uint8_t push(node* thisNode);
//node* pop(void);
//
///*******************************************************************************
// * Scheduler Functions
// ******************************************************************************/
//void FRAMEWORK_schedulerInit(void) {
//    int i = 0;
//    for (i = 0; i < SCHEDULE_SIZE; i++) {
//        scheduleStack.stack[i] = &scheduleArray[i];
//    }
//    scheduleStack.stackPtr = SCHEDULE_SIZE;
//    Head.nextNode = 0;
//    Head.prevNode = 0;
//    Head.thisFunction = 0;
//    Head.time = 0;
//}
//
//uint8_t FRAMEWORK_schedulerAdd(FRAMEWORK_scheduleTask someFunction, uint32_t time) {
//    time = time + FRAMEWORK_getTimeNow();
//    uint8_t returnVal = 0;
//    /*Pop a new node off the stack*/
//    node* newNode = pop();
//    if (newNode != 0) {
//        /*Search for insertion point*/
//        node* thisNode = &Head;
//        while (thisNode->nextNode != 0 && thisNode->nextNode->time <= time) {
//            thisNode = thisNode->nextNode;
//        }
//        /*Insert new node*/
//        newNode->prevNode = thisNode;
//        newNode->nextNode = thisNode->nextNode;
//        newNode->prevNode->nextNode = newNode;
//        newNode->nextNode->prevNode = newNode;
//        newNode->time = time;
//        newNode->thisFunction = someFunction;
//        returnVal = 1;
//
//    }
//    return returnVal;
//}
//
//uint8_t scheduler_remove(uint32_t time) {
//    uint8_t returnVal = 0;
//    if (Head.nextNode->time > time) {
//        /*Do nothing*/
//    } else {
//
//        node* thisNode = Head.nextNode;
//        if (thisNode == 0) {
//            //do nothing
//        } else {
//            while (thisNode != 0 && thisNode->time <= time) {
//                thisNode->prevNode->nextNode = thisNode->nextNode;
//                thisNode->nextNode->prevNode = thisNode->prevNode;
//                thisNode->thisFunction();
//                push(thisNode); /*put empty node back on stack*/
//                thisNode = thisNode->nextNode;
//            }
//        }
//        returnVal = 1;
//    }
//    return returnVal;
//}
//
//uint8_t push(node* thisNode) {
//    uint8_t returnVal = 0;
//    if (scheduleStack.stackPtr <= SCHEDULE_SIZE) {
//        scheduleStack.stack[scheduleStack.stackPtr++] = thisNode;
//        returnVal = 1;
//    }
//    return returnVal;
//}
//
//node* pop(void) {
//    node* thisNode = 0;
//    if (scheduleStack.stackPtr > 0) {
//        thisNode = scheduleStack.stack[--scheduleStack.stackPtr];
//    }
//    return thisNode;
//}

