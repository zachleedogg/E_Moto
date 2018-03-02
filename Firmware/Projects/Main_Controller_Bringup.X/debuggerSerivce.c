#include "debuggerService.h"
#include "framework.h"
#include "templateService.h"

#include <string.h>
#include <stdio.h>
#include "pins.h"
#include "bolt_ADC.h"
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
#define LS 5863588
#define GPIO_SET 156940064
#define GPIO_GET 156926996
#define I2C_SET 9287727
#define I2C_GET 9274659
#define SPI 193506033
#define ADC 193486029

const unsigned long hash(const char *str);

/*******************************************************************************
 * STATE MACHINE BEGINS HERE
 * ****************************************************************************/
Event debuggerService(Event ThisEvent) {

    /*Call the state machine functions*/
    DEBUGGER_SERVICE_states_E nextState = theState[curState](ThisEvent);

    /* This only happens during state transition
     * State transitions thus have priority over posting new events
     * State transitions always consist of an exit event to curState and entry event to nextState */
    if (nextState != curState) {
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
            FRAMEWORK_timerSet(0, 1, debuggerService_SERVICE, CONTINUOUS);
            break;
            /*If a UART command is sent, the first character selects a service,
             * and the second char enables or disables debug printing*/
        case TIMEUP_EVENT:
            /*Debugging print statement*/
            if (Uart1RXdataReady()) {
                unsigned char msg[64] = {};
                Uart1Read(msg);
                debuggerService_print("User: %s\n", msg);

                /*Parse cmd string by spaces*/
                char cmd[16];
                char port;
                int pin;
                int state;
                sscanf(msg, "%16s %*s", cmd);
                unsigned long newHash = hash(cmd);
                switch (newHash) {
                    case GPIO_SET:
                        debuggerService_print("running GPIO_SET\n");
                        sscanf(msg, "%*s %1c%2d %1d", &port, &pin, &state);
                        if (sscanf(msg, "%*s %c%d %d", &port, &pin, &state) == 3) {
                            PINS_pin_S thisPin;
                            thisPin.pin = pin;
                            thisPin.port = (uint8_t) port - 97;
                            PINS_write(thisPin, state);
                        } else {
                            debuggerService_print("GPIO_SET failed\n");
                        }
                        break;
                    case GPIO_GET:
                        debuggerService_print("running GPIO_GET\n");
                        if (sscanf(msg, "%*s %1c%2d", &port, &pin) == 2) {
                            PINS_pin_S thisPin;
                            thisPin.pin = pin;
                            thisPin.port = (uint8_t) port - 97;
                            debuggerService_print("PIN %c%d state: %d\n", port, pin, PINS_read(thisPin));
                        } else {
                            debuggerService_print("GPIO_GET failed\n");
                        }
                        break;
                    case I2C_SET:
                        debuggerService_print("running I2C_SET\n");
                        break;
                    case I2C_GET:
                        debuggerService_print("running I2C_GET\n");
                        break;
                    case SPI:
                        debuggerService_print("running SPI\n");
                    case ADC:
                        debuggerService_print("running ADC\n");
                        if (sscanf(msg, "%*s %2d", &pin) == 1) {
                            debuggerService_print("ADC AN%d value: %d\n", pin, ADC_GetValue(pin));
                        } else {
                            debuggerService_print("ADC failed\n");
                        }
                        debuggerService_print("running ADC\n");
                        break;
                    case LS:
                        debuggerService_print("Available commands:\ngpioget ax x\ngpioset ax\n")
                        break;
                    default:
                        debuggerService_print("invalid command: %s\n", cmd);
                        break;
                }

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

const unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}