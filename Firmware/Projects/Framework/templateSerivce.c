#include "templateService.h"
#include "Defines.h"
#include "framework.h"

/*******************************************************************************
 * Debugging
 * ****************************************************************************/
#define DEBUG 1
#if DEBUG
#include <stdio.h>
#include "bolt_uart.h"
static uint8_t debugEnable = 1;
#define templateService_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
#define templateService_print(...)
#endif
/*******************************************************************************
 * STATE MACHINE SETUP
 * ****************************************************************************/

#define TEMPLATE_SERVICE_STATES(state)\
state(init) /* init state for startup code */ \
state(welcomeState) /* have fun drawing on the screen */ \
state(lockedState) /* unlock the bike with your passcode */ \
state(homeState) /* select from the menu on the home screen */ \
state(runningState) /* see your complete dashboard in running mode */ \
state(batteryState) \
state(statisticState) /*See you ride stats */ \

/*Creates an enum of states suffixed with _state*/
#define STATE_FORM(WORD) WORD##_state,

typedef enum {
    TEMPLATE_SERVICE_STATES(STATE_FORM)
    NUMBER_OF_STATES
} TEMPLATE_SERVICE_states_E;

/*creates a string-ified list of state string names*/
static const char __attribute__((__unused__)) * const StateStrings[] = {
    TEMPLATE_SERVICE_STATES(STRING_FORM)
};

/* function-ifies the state list*/
#define FUNCTION_FORM(WORD) static TEMPLATE_SERVICE_states_E WORD(Event ThisEvent);
#define FUNC_PTR_FORM(WORD) WORD,
TEMPLATE_SERVICE_STATES(FUNCTION_FORM)
typedef TEMPLATE_SERVICE_states_E(*statePtr)(Event);
static statePtr theState[] = {TEMPLATE_SERVICE_STATES(FUNC_PTR_FORM)};

/*Set new state function*/
static TEMPLATE_SERVICE_states_E setStateTo(statePtr thisState);

/*State variables*/
static TEMPLATE_SERVICE_states_E prevState = init_state; /* initialize previous state */
static TEMPLATE_SERVICE_states_E curState = init_state; /* initialize current state */

/*******************************************************************************
 * USER SPACE
 * ****************************************************************************/

/*******************************************************************************
 * STATE MACHINE BEGINS HERE
 * ****************************************************************************/
Event templateService(Event ThisEvent) {
    /*Debugging print statement*/
    templateService_print("Service: %s,   State: %s,   Event: %s %d\n\n", ServiceStrings[templateService_SERVICE], StateStrings[curState], EventStrings[ThisEvent.EventType], ThisEvent.EventParam);

    /*Call the state machine functions*/
    TEMPLATE_SERVICE_states_E nextState = theState[curState](ThisEvent);

    /* This only happens during state transition
     * State transitions thus have priority over posting new events
     * State transitions always consist of an exit event to curState and entry event to nextState */
    if (nextState != curState) {
        templateService_print("Transistioning\n");
        ThisEvent.EventType = NO_EVENT;
        templateService(EXIT);
        prevState = curState;
        curState = nextState;
        templateService(ENTRY);
    }

    return ThisEvent;
}

/*******************************************************************************
 *                            I N I T I A L I Z E
 * @param ThisEvent
 * @return 
 */
static TEMPLATE_SERVICE_states_E init(Event ThisEvent) {
    TEMPLATE_SERVICE_states_E nextState = curState;
    if (ThisEvent.EventType == INIT_EVENT) {
        /*Initialization stuff here*/

        nextState = setStateTo(welcomeState);
    }
    return nextState;
}

/*******************************************************************************
 *                             W E L C O M E
 * @param ThisEvent
 * @return 
 */
static TEMPLATE_SERVICE_states_E welcomeState(Event ThisEvent) {
    TEMPLATE_SERVICE_states_E nextState = curState;
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
    return nextState;
}

/*******************************************************************************
 *                              L O C K E D
 * @param ThisEvent
 * @return 
 */
static TEMPLATE_SERVICE_states_E lockedState(Event ThisEvent) {
    TEMPLATE_SERVICE_states_E nextState = curState;
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
    return nextState;
}

/*******************************************************************************
 *                            H O M E
 * @param ThisEvent
 * @return 
 */
static TEMPLATE_SERVICE_states_E homeState(Event ThisEvent) {

    TEMPLATE_SERVICE_states_E nextState = curState;

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
    return nextState;
}

/*******************************************************************************
 *                         R U N N I N G
 * @param ThisEvent
 * @return 
 */
static TEMPLATE_SERVICE_states_E runningState(Event ThisEvent) {

    TEMPLATE_SERVICE_states_E nextState = curState;

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
    return nextState;
}

/*******************************************************************************
 *                        B A T T E R Y
 * @param ThisEvent
 * @return 
 */
static TEMPLATE_SERVICE_states_E batteryState(Event ThisEvent) {

    TEMPLATE_SERVICE_states_E nextState = curState;

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
    return nextState;
}

/*******************************************************************************
 *                      S T A T I S T I C S
 * @param ThisEvent
 * @return 
 */
static TEMPLATE_SERVICE_states_E statisticState(Event ThisEvent) {
    TEMPLATE_SERVICE_states_E nextState = curState;

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
    return nextState;
}

/*******************************************************************************
 * PRIVATE HELPER FUNCTIONS
 * ****************************************************************************/

static TEMPLATE_SERVICE_states_E setStateTo(statePtr thisState) {
    TEMPLATE_SERVICE_states_E nextState = 0;
    for (nextState = 0; nextState < NUMBER_OF_STATES; nextState++) {
        if ((uint16_t) theState[nextState] == (uint16_t) thisState) {
            break;
        }
    }
    if (nextState == NUMBER_OF_STATES) {
        nextState = 0;
    }
    return nextState;
}

void template_Debug(uint8_t state) {
#if DEBUG
    debugEnable = state;
#endif
}
