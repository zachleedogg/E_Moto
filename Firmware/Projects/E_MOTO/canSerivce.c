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
static uint8_t debugEnable = 0;
#define canService_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
#define canService_print(...)
#endif
/*******************************************************************************
 * STATE MACHINE SETUP
 * ****************************************************************************/

#define CAN_SERVICE_STATES(state)\
state(init) /* init state for startup code */ \
state(canSleeping) /* have fun drawing on the screen */ \
state(canSilent) /* have fun drawing on the screen */ \
state(canActive) /* have fun drawing on the screen */ \

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

        CAN_configureMailbox(DEFINES_CAN_MSG_1, DEFINES_CAN_MSG_1_ID);
        CAN_configureMailbox(DEFINES_CAN_MSG_2, DEFINES_CAN_MSG_2_ID);
        CAN_configureMailbox(DEFINES_CAN_MSG_3, DEFINES_CAN_MSG_3_ID);
        CAN_configureMailbox(DEFINES_CAN_MSG_4, DEFINES_CAN_MSG_4_ID);

        nextState = setStateTo(canActive);
    }
    return nextState;
}

/*******************************************************************************
 *                             S L E E P I N G
 * @param ThisEvent
 * @return 
 */
static CAN_SERVICE_states_E canSleeping(Event ThisEvent) {
    CAN_SERVICE_states_E nextState = curState;
    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            break;
            /*Put custom states below here*/
        case TIMEUP_EVENT:

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
 *                             S I L E N T
 * @param ThisEvent
 * @return 
 */
static CAN_SERVICE_states_E canSilent(Event ThisEvent) {
    CAN_SERVICE_states_E nextState = curState;

    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            break;
            /*Put custom states below here*/
        case TIMEUP_EVENT:

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
 *                             A C T I V E
 * @param ThisEvent
 * @return 
 */
static CAN_SERVICE_states_E canActive(Event ThisEvent) {
    CAN_SERVICE_states_E nextState = curState;

    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            FRAMEWORK_timerSet(14, 250, canService_SERVICE, CONTINUOUS);
            break;
            /*Put custom states below here*/
        case TIMEUP_EVENT:
        {
            static uint8_t mynum = 0;
            mynum++;
            
            CAN_DASH_status_U thisStatus;
            thisStatus.state = mynum;

            CAN_message_S newMessage = {
                .nodeID = 0x4,
                .messageID = 0x2,
                .frequency = 0x7,
                .word0 = thisStatus.packedMessage.word0,
            };

            CAN_write(&newMessage);
        }
            break;
            /*Put custom states above here*/
        case EXIT_EVENT:
            FRAMEWORK_timerStop(14);
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
