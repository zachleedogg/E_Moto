#include "FSM_TEMPLATE.h"
#include "Defines.h"
#include <stdio.h>

/* Uncomment to for custom Debugging */
#define FSM_TEMPLATE_DEBUG

#ifdef FSM_TEMPLATE_DEBUG
#define DEBUG
#define FSM_TEMPLATE_print(...) ({char tempArray[100]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);})
#else
#define FSM_TEMPLATE_print(...) 
#endif

static FSM_TEMPLATE_State_t prevState = init; /* initialize previous state */
static FSM_TEMPLATE_State_t curState = init; /* initialize current state */

Event FSM_TEMPLATE(Event ThisEvent) {
    /*Clear priority and service*/
    ThisEvent.Service = 0;
    ThisEvent.EventPriority = 0;

    /*Debugging print statement*/
    FSM_TEMPLATE_print("%s Event: %s %d\n", StateStrings[curState], EventStrings[ThisEvent.EventType], ThisEvent.EventParam);

    /*Initialize next state so we can check for a transistion*/
    FSM_TEMPLATE_State_t nextState = curState;

    /*State Machine Begins Here.*/
    switch (curState) {
        case init: /* SM starts here */
            if (ThisEvent.EventType == INIT) {
                /*Initialization stuff here*/
                nextState = someState;
            }
            break;

        case someState: /* initial idle state before ignition */
            switch (ThisEvent.EventType) {
                case ENTRY:
                    break;
                    /*Put custom states below here*/

                    /*Put custom states above here*/
                case EXIT:
                    break;
                default:
                    break;
            }
            break;

        case anotherState: /* Welcomes the user after the ignition wake up*/
            if (ThisEvent.EventType != NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ENTRY:
                        break;
                        /*Put custom states below here*/

                        /*Put custom states above here*/
                    case EXIT:
                        break;
                    default:
                        break;
                }
            }
            break;
            
        case yetAnotherState: /* Welcomes the user after the ignition wake up*/
            if (ThisEvent.EventType != NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ENTRY:
                        break;
                        /*Put custom states below here*/

                        /*Put custom states above here*/
                    case EXIT:
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
        FSM_TEMPLATE(EXIT_EVENT);
        prevState = curState;
        curState = nextState;
        FSM_TEMPLATE(ENTRY_EVENT);
    }

    return ThisEvent;
}
