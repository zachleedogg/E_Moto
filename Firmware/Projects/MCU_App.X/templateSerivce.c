#include "framework.h"
#include "templateService.h"


/*******************************************************************************
 * Debugging
 * ****************************************************************************/
#define DEBUG 0
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
#define FUNCTION_FORM(WORD) static void WORD(Event ThisEvent);
#define FUNC_PTR_FORM(WORD) WORD,
TEMPLATE_SERVICE_STATES(FUNCTION_FORM)
typedef void(*statePtr)(Event);
static statePtr theState[] = {TEMPLATE_SERVICE_STATES(FUNC_PTR_FORM)};

/*Set new state function*/
static void setStateTo(statePtr thisState);

/*State variables*/
static TEMPLATE_SERVICE_states_E prevState = init_state; /* initialize previous state */
static TEMPLATE_SERVICE_states_E curState = init_state; /* initialize current state */
static TEMPLATE_SERVICE_states_E nextState = init_state; /* initialize current state */

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
    theState[curState](ThisEvent);

    /* This only happens during state transition
     * State transitions thus have priority over posting new events
     * State transitions always consist of an exit event to curState and entry event to nextState */
    if (nextState != curState) {
        templateService_print("Transitioning\n");
        ThisEvent.EventType = NO_EVENT;
        theState[curState](EXIT);
        prevState = curState;
        curState = nextState;
        theState[curState](ENTRY);
    }

    return ThisEvent;
}

/*******************************************************************************
 *                            I N I T I A L I Z E
 * @param ThisEvent
 * @return 
 */
static void init(Event ThisEvent) {

    if (ThisEvent.EventType == INIT_EVENT) {
        /*Initialization stuff here*/

        setStateTo(welcomeState);
    }
}

/*******************************************************************************
 *                             W E L C O M E
 * @param ThisEvent
 * @return 
 */
static void welcomeState(Event ThisEvent) {

    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            setStateTo(lockedState);
            break;
            /*Put custom states below here*/

            /*Put custom states above here*/
        case EXIT_EVENT:
            break;
        default:
            break;
    }
}

/*******************************************************************************
 *                              L O C K E D
 * @param ThisEvent
 * @return 
 */
static void lockedState(Event ThisEvent) {

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

/*******************************************************************************
 *                            H O M E
 * @param ThisEvent
 * @return 
 */
static void homeState(Event ThisEvent) {

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

/*******************************************************************************
 *                         R U N N I N G
 * @param ThisEvent
 * @return 
 */
static void runningState(Event ThisEvent) {

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

/*******************************************************************************
 *                        B A T T E R Y
 * @param ThisEvent
 * @return 
 */
static void batteryState(Event ThisEvent) {

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

/*******************************************************************************
 *                      S T A T I S T I C S
 * @param ThisEvent
 * @return 
 */
static void statisticState(Event ThisEvent) {

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

/*******************************************************************************
 * PRIVATE HELPER FUNCTIONS
 * ****************************************************************************/

static void setStateTo(statePtr thisState) {
    uint8_t stateIndex = 0;
    for (stateIndex = 0; stateIndex < NUMBER_OF_STATES; stateIndex++) {
        if ((uint16_t) theState[stateIndex] == (uint16_t) thisState) {
            break;
        }
    }
    if (stateIndex == NUMBER_OF_STATES) {
        stateIndex = 0;
    }
    nextState = stateIndex;
}

void template_Debug(uint8_t state) {
#if DEBUG
    debugEnable = state;
#endif
}
