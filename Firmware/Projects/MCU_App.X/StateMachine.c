/******************************************************************************
 * Includes
 *******************************************************************************/
#include "bolt_CAN.h"
#include "bolt_sleep.h"

#include "SysTick.h"
#include "LightsControl.h"
#include "IgnitionControl.h"
#include "HornControl.h"
#include "HeatedGrips.h"
#include "j1772.h"
#include "IO.h"

#include <xc.h>
#include <stdio.h>

/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/

#define STATE_MACHINE_STATES(state)\
state(idle)\
state(standby)\
state(silent_wake)\
state(running)\
state(charging)\
state(sleep)\

/*Creates an enum of states suffixed with _state*/
#define STATE_FORM(WORD) WORD##_state,
#define FUNCTION_FORM(WORD) static void WORD(STATE_MACHINE_entry_types_E entry_type);
#define FUNC_PTR_FORM(WORD) WORD,


/******************************************************************************
 * Configuration
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/
typedef enum {
    STATE_MACHINE_STATES(STATE_FORM)
    NUMBER_OF_STATES
} STATE_MACHINE_states_E;

typedef enum {
    ENTRY,
    EXIT,
    RUN
} STATE_MACHINE_entry_types_E;

typedef void(*statePtr)(STATE_MACHINE_entry_types_E);

/******************************************************************************
 * Variable Declarations
 *******************************************************************************/
/*State variables*/
/* function-ifies the state list*/
STATE_MACHINE_STATES(FUNCTION_FORM)
static statePtr state_functions[] = {STATE_MACHINE_STATES(FUNC_PTR_FORM)};

static STATE_MACHINE_states_E prevState = idle_state; /* initialize previous state */
static STATE_MACHINE_states_E curState = idle_state; /* initialize current state */
static STATE_MACHINE_states_E nextState = idle_state; /* initialize current state */

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void StateMachine_Init(void) {
    state_functions[curState](ENTRY);
}

void StateMachine_Run(void) {

    /* This only happens during state transition
     * State transitions thus have priority over posting new events
     * State transitions always consist of an exit event to curState and entry event to nextState */
    if (nextState != curState) {
        state_functions[curState](EXIT);
        prevState = curState;
        curState = nextState;
        state_functions[curState](ENTRY);
    } else {
        state_functions[curState](RUN);
    }
}

void idle(STATE_MACHINE_entry_types_E entry_type) {
    static const uint32_t idleTimer = 5000;
    static uint32_t idleTimerStartTime = 0;
    switch (entry_type) {
        case ENTRY:
            idleTimerStartTime = SysTick_Get();
            //Initialize the board.
            IO_SET_SW_EN(HIGH);
            IO_SET_IC_CONTROLLER_SLEEP_EN(LOW);
            CAN_changeOpMode(CAN_NORMAL);
            IO_SET_CAN_SLEEP_EN(LOW);
            IO_SET_BATT_EN(HIGH); //TODO: check this enable sequence

            //Initialize each application.
            IO_Efuse_Init();
            LightsControl_Init();
            HeatedGripControl_Init();
            IgnitionControl_Init();
            HornControl_Init();
            j1772Control_Init();
            break;

        case EXIT:
            break;

        case RUN:
            // Always update the timer if there is CAN traffic present.
            if (CAN_RxDataIsReady()) {
                idleTimerStartTime = SysTick_Get();
            }
            // Once the timer expires, go to standby and prepare for sleep.
            if ((SysTick_Get() - idleTimerStartTime) > idleTimer) {
                nextState = standby_state;
            }
            if (j1772getProxState() == J1772_CONNECTED) {
                nextState = charging_state;
            }
            break;

        default:
            break;
    }

}

void standby(STATE_MACHINE_entry_types_E entry_type) {
    static const uint32_t standbyTimer = 3000;
    static uint32_t standbyTimerStartTime = 0;
    switch (entry_type) {
        case ENTRY:
            standbyTimerStartTime = SysTick_Get();
            CAN_changeOpMode(CAN_LISTEN);
            break;
        case EXIT:
            break;
        case RUN:
            if (CAN_RxDataIsReady()) {
                nextState = idle_state;
            }
            if ((SysTick_Get() - standbyTimerStartTime) > standbyTimer) {
                nextState = sleep_state;
            }
            break;
        default:
            break;
    }
}

void silent_wake(STATE_MACHINE_entry_types_E entry_type) {
    switch (entry_type) {
        case ENTRY:
            break;
        case EXIT:
            break;
        case RUN:
            break;
        default:
            break;
    }
}

void running(STATE_MACHINE_entry_types_E entry_type) {
    switch (entry_type) {
        case ENTRY:
            break;
        case EXIT:
            break;
        case RUN:
            break;
        default:
            break;
    }
}

void charging(STATE_MACHINE_entry_types_E entry_type) {
    switch (entry_type) {
        case ENTRY:
            break;
        case EXIT:
            break;
        case RUN:
            switch(j1772getProxState()){
                case J1772_DISCONNECTED:
                case J1772_REQUEST_DISCONNECT:
                case J1772_SNA_PROX:
                default:
                    nextState = idle_state;
                    break;
            }
            break;
        default:
            break;
    }
}

void sleep(STATE_MACHINE_entry_types_E entry_type) {
    switch (entry_type) {
        case ENTRY:
            IO_SET_SW_EN(LOW);
            IO_SET_IC_CONTROLLER_SLEEP_EN(HIGH);
            CAN_changeOpMode(CAN_DISABLE);
            IO_SET_CAN_SLEEP_EN(HIGH);
            IO_SET_DEBUG_LED_EN(LOW);

            //stop all apps
            IO_Efuse_Halt();
            LightsControl_Halt();
            HeatedGripControl_Halt();
            HornControl_Halt();
            j1772Control_Halt();
            IgnitionControl_Halt();
            break;

        case EXIT:
            break;
        case RUN:
            SysTick_Stop();
            RCONbits.SWDTEN = 0;
            SleepNow(); //Go to sleep
            RCONbits.SWDTEN = 1;
            SysTick_Resume();
            if (RCONbits.WDTO) {
                nextState = silent_wake_state;
            } else {
                nextState = idle_state;
            }

            break;
        default:
            break;
    }
}

/****Helpers*******************************************************************/


/*** End of File **************************************************************/



