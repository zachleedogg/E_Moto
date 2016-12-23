#include "debuggerService.h"
#include "Defines.h"
#include "framework.h"
#include "touchScreenService.h"
#include "framework_taskRunner.h"

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
state(welcomeState) /* have fun drawing on the screen */ \

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


        nextState = setStateTo(welcomeState);
    }
    return nextState;
}

/*******************************************************************************
 *                             W E L C O M E
 * @param ThisEvent
 * @return 
 */
static DEBUGGER_SERVICE_states_E welcomeState(Event ThisEvent) {
    DEBUGGER_SERVICE_states_E nextState = curState;
    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            break;
            /*Put custom states below here*/
        case DEBUG_MESSAGE_EVENT:
            switch (msg[0]) {
                case 0:
                    FRAMEWORK_Debug(msg[1]);
                    break;
                case 1:
                    touchScreenDebug(msg[1]);
                    break;
                case 2:
                    ledDriverWrite(0, ((uint16_t) msg[2] | ((uint16_t) msg[1] << 8)));
                    debuggerService_print("DEBUGGER RECV: %x\n", ((uint16_t) msg[2] | ((uint16_t) msg[1] << 8)));
                    break;
                case 3:
                    FRAMEWORK_TASKRUNNER_debug(msg[1]);
                    break;
                case 4:
                    debug_Debug(msg[1]);
                    break;
                case 5:
                    PINS_write(DEFINES_5V_SW_RAIL, msg[1]);
                    break;
                case 6:
                    PINS_write(DEFINES_12V_SW_RAIL, msg[1]);
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

void debug_Debug(uint8_t state) {
#if DEBUG
    debugEnable = state;
#endif
}
