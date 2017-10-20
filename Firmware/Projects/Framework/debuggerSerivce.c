#include "debuggerService.h"
#include "Defines.h"
#include "framework.h"
#include "templateService.h"

/*******************************************************************************
 * Debugging
 * ****************************************************************************/
#define DEBUG 1
#if DEBUG
#include <stdio.h>
#include "bolt_uart.h"
static uint8_t debugEnable = 1;
#define debuggerService_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
#define debuggerService_print(...)
#endif
/*******************************************************************************
 * STATE MACHINE SETUP
 * ****************************************************************************/

#define DEBUGGER_SERVICE_STATES(state)\
state(init) /* init state for startup code */ \
state(debugState) /* have fun drawing on the screen */ \

/*Creates an enum of states suffixed with _state*/
#define STATE_FORM(WORD) WORD##_state,

typedef enum {
    DEBUGGER_SERVICE_STATES(STATE_FORM)
    NUMBER_OF_STATES
} DEBUGGER_SERVICE_states_E;

/*creates a string-ified list of state string names*/
static const char __attribute__((__unused__)) * const StateStrings[] = {
    DEBUGGER_SERVICE_STATES(STRING_FORM)
};

/* function-ifies the state list*/
#define FUNCTION_FORM(WORD) static DEBUGGER_SERVICE_states_E WORD(Event ThisEvent);
#define FUNC_PTR_FORM(WORD) WORD,
DEBUGGER_SERVICE_STATES(FUNCTION_FORM)
typedef DEBUGGER_SERVICE_states_E(*statePtr)(Event);
static statePtr theState[] = {DEBUGGER_SERVICE_STATES(FUNC_PTR_FORM)};
static DEBUGGER_SERVICE_states_E setStateTo(statePtr thisState);

static DEBUGGER_SERVICE_states_E prevState = init_state; /* initialize previous state */
static DEBUGGER_SERVICE_states_E curState = init_state; /* initialize current state */

/*******************************************************************************
 * USER SPACE
 * ****************************************************************************/

static unsigned char msg[20] = {};

/*******************************************************************************
 * STATE MACHINE BEGINS HERE
 * ****************************************************************************/
Event debuggerService(Event ThisEvent) {
    /*Debugging print statement*/
    Uart1Read(msg);
    debuggerService_print("Service: %s...\nASCII: %s\nHEX:", ServiceStrings[debuggerService_SERVICE], msg);
    uint8_t i = 0;
    while (msg[i]) {
        debuggerService_print(" %2x", msg[i]);
        i++;
    }
    debuggerService_print("\n...end\n");

    /*Call the state machine functions*/
    DEBUGGER_SERVICE_states_E nextState = theState[curState](ThisEvent);

    /* This only happens during state transition
     * State transitions thus have priority over posting new events
     * State transitions always consist of an exit event to curState and entry event to nextState */
    if (nextState != curState) {
        debuggerService_print("Transistioning\n");
        ThisEvent.EventType = NO_EVENT;
        debuggerService(EXIT);
        prevState = curState;
        curState = nextState;
        debuggerService(ENTRY);
    }

    return ThisEvent;
}

/*******************************************************************************
 *                            I N I T I A L I Z E
 * @param ThisEvent
 * @return 
 */
static DEBUGGER_SERVICE_states_E init(Event ThisEvent) {
    DEBUGGER_SERVICE_states_E nextState = curState;
    if (ThisEvent.EventType == INIT_EVENT) {
        /*Initialization stuff here*/
        debuggerService_print("Debugger Initializing\n");
        nextState = setStateTo(debugState);
    }
    return nextState;
}

/*******************************************************************************
 *                             D E B U G   S T A T E
 * @param ThisEvent
 * @return 
 */
static DEBUGGER_SERVICE_states_E debugState(Event ThisEvent) {
    DEBUGGER_SERVICE_states_E nextState = curState;
    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            break;
            /*If a UART command is sent, the first character selects a service,
             * and the second char enables or disables debug printing*/
        case DEBUG_MESSAGE_EVENT:
            switch (msg[0]) {
                case 0:
                    FRAMEWORK_Debug(msg[1]);
                    break;
                case 1:
                    DEBUGGER_Debug(msg[1]);
                    break;
                case 2:
                    FRAMEWORK_TASKRUNNER_debug(msg[1]);
                    break;
                case 3:
                    template_Debug(msg[1]);
                    break;
                case 4:
                    break;
                default:
                    break;
            }
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

static DEBUGGER_SERVICE_states_E setStateTo(statePtr thisState) {
    DEBUGGER_SERVICE_states_E nextState = 0;
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

void DEBUGGER_Debug(uint8_t state) {
    debugEnable = state;
}