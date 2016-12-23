#include "canService.h"
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
#define canService_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
#define canService_print(...)
#endif
/*******************************************************************************
 * STATE MACHINE SETUP
 * ****************************************************************************/

#define CAN_SERVICE_STATES(state)\
state(init) /* init state for startup code */ \
state(welcomeState) /* have fun drawing on the screen */ \

/*Creates an enum of states suffixed with _state*/
#define STATE_FORM(WORD) WORD##_state,

typedef enum {
    CAN_SERVICE_STATES(STATE_FORM)
    NUMBER_OF_STATES
} CAN_SERVICE_states_E;

/*creates a string-ified list of state string names*/
static const char __attribute__((__unused__)) * const StateStrings[] = {
    CAN_SERVICE_STATES(STRING_FORM)
};

/* function-ifies the state list*/
#define FUNCTION_FORM(WORD) static CAN_SERVICE_states_E WORD(Event ThisEvent);
#define FUNC_PTR_FORM(WORD) WORD,
CAN_SERVICE_STATES(FUNCTION_FORM)
typedef CAN_SERVICE_states_E(*statePtr)(Event);
static statePtr theState[] = {CAN_SERVICE_STATES(FUNC_PTR_FORM)};
static CAN_SERVICE_states_E setStateTo(statePtr thisState);

static CAN_SERVICE_states_E prevState = init_state; /* initialize previous state */
static CAN_SERVICE_states_E curState = init_state; /* initialize current state */

/*******************************************************************************
 * USER SPACE
 * ****************************************************************************/



/*******************************************************************************
 * STATE MACHINE BEGINS HERE
 * ****************************************************************************/
Event canService(Event ThisEvent) {
    /*Debugging print statement*/
    canService_print("Service: %s,   State: %s,   Event: %s %d\n\n", ServiceStrings[canService_SERVICE], StateStrings[curState], EventStrings[ThisEvent.EventType], ThisEvent.EventParam);
    
    /*Call the state machine functions*/
    CAN_SERVICE_states_E nextState = theState[curState](ThisEvent);

    /* This only happens during state transition
     * State transitions thus have priority over posting new events
     * State transitions always consist of an exit event to curState and entry event to nextState */
    if (nextState != curState) {
        canService_print("Transitioning\n");
        ThisEvent.EventType = NO_EVENT;
        canService(EXIT);
        prevState = curState;
        curState = nextState;
        canService(ENTRY);
    }

    return ThisEvent;
}

/*******************************************************************************
 *                            I N I T I A L I Z E
 * @param ThisEvent
 * @return 
 */
static CAN_SERVICE_states_E init(Event ThisEvent) {
    CAN_SERVICE_states_E nextState = curState;
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
static CAN_SERVICE_states_E welcomeState(Event ThisEvent) {
    CAN_SERVICE_states_E nextState = curState;
    CAN_message_S otherMessage = {};
    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            break;
            /*Put custom states below here*/
        case CAN_MESSAGE_RCVD_EVENT:
            
            CAN_Read(&otherMessage);
            canService_print("byte0: %3u, byte1: %3u byte2: %3u byte3: %3u byte4: %3u, byte5: %3u byte6: %3u byte7: %3u\n",
                    otherMessage.byte0, otherMessage.byte1, otherMessage.byte2, otherMessage.byte3,
                    otherMessage.byte4, otherMessage.byte5, otherMessage.byte6, otherMessage.byte7);
            break;

            /*Put custom states above here*/
        case EXIT_EVENT:
            break;
        default:
            break;
    }
    return nextState;
}

/*******************************************************************************
 * HELPER FUNCTIONS
 * ****************************************************************************/

static CAN_SERVICE_states_E setStateTo(statePtr thisState) {
    CAN_SERVICE_states_E nextState = 0;
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

void can_Debug(uint8_t state) {
#if DEBUG
    debugEnable = state;
#endif
}
