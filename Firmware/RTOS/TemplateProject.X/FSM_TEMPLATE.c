#include <stdio.h>

#include "FSM_TEMPLATE.h"
#include "Defines.h"
#include "configure.h"
#include "framework.h"

/* Uncomment to for custom Debugging */

static uint8_t debugEnable = 1;

#define FSM_TEMPLATE_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}


static FSM_TEMPLATE_State_t prevState = init; /* initialize previous state */
static FSM_TEMPLATE_State_t curState = init; /* initialize current state */

void print1(void);
void print2(void);
void print3(void);
void print4(void);
void print5(void);

void print1(void) {
    Uart1Write("1");
}

void print2(void) {
    Uart1Write("2");
}

void print3(void) {
    Uart1Write("3");
}

void print4(void) {
    Uart1Write("4");
}

void print5(void) {
    Uart1Write("5\n");
}

Event FSM_TEMPLATE(Event ThisEvent) {

    /*Debugging print statement*/
    FSM_TEMPLATE_print("Service: %s\tState: %s\tEvent: %s %d\n", ServiceStrings[FSM_TEMPLATE_SERVICE], StateStrings[curState], EventStrings[ThisEvent.EventType], ThisEvent.EventParam);

    /*Initialize next state so we can check for a transistion*/
    FSM_TEMPLATE_State_t nextState = curState;

    /*State Machine Begins Here.*/
    switch (curState) {
        case init: /* SM starts here */
            if (ThisEvent.EventType == INIT_EVENT) {
                /*Initialization stuff here*/
                scheduler_add(&print1, 10);
                scheduler_add(&print2, 20);
                scheduler_add(&print3, 300);
                scheduler_add(&print4, 150);
                scheduler_add(&print5, 500);

                nextState = someState;
            }
            break;

        case someState: /* initial idle state before ignition */
            switch (ThisEvent.EventType) {
                case ENTRY_EVENT:
                    ThisEvent.EventParam = 100;
                    ThisEvent.EventType = TIMEUP_EVENT;
                    Post(ThisEvent);
                    Post(ThisEvent);
                    Post(ThisEvent);
                    Post(ThisEvent);
                    Post(ThisEvent);
                    ThisEvent.EventType = EXIT_EVENT;
                    ThisEvent.EventPriority = 2;
                    Post(ThisEvent);
                    ThisEvent.EventType = EXIT_EVENT;
                    ThisEvent.EventPriority = 3;
                    ThisEvent.EventParam = 42;
                    Post(ThisEvent);
                    break;
                    /*Put custom states below here*/
                case TIMEUP_EVENT:
                    scheduler_add(&print1, 310);
                    break;
                    /*Put custom states above here*/
                case EXIT_EVENT:
                    break;
                default:
                    break;
            }
            break;

        case anotherState: /* Welcomes the user after the ignition wake up*/
            if (ThisEvent.EventType != NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ENTRY_EVENT:
                        break;
                        /*Put custom states below here*/

                        /*Put custom states above here*/
                    case EXIT_EVENT:
                        break;
                    default:
                        break;
                }
            }
            break;

        case yetAnotherState: /* Welcomes the user after the ignition wake up*/
            if (ThisEvent.EventType != NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ENTRY_EVENT:
                        break;
                        /*Put custom states below here*/

                        /*Put custom states above here*/
                    case EXIT_EVENT:
                        break;
                    default:
                        break;
                }
            }
            break;
    }

    /* This only happens during state transition
     * State transitions thus have priority over posting new events
     * State transitions always consist of an exit event to curState and entry event to nextState */
    if (nextState != curState) {
        FSM_TEMPLATE_print("\nTransistioning\n");
        ThisEvent.EventType = NO_EVENT;
        FSM_TEMPLATE(EXIT);
        prevState = curState;
        curState = nextState;
        FSM_TEMPLATE(ENTRY);
    }

    return ThisEvent;
}
