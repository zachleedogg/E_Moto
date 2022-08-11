/******************************************************************************
 * Includes
 *******************************************************************************/
#include "SysTick.h"
#include "IO.h"
#include "bolt_CAN.h"
#include "bms_dbc.h"
#include "pinSetup.h"

#include <stdio.h>
#include <string.h>

/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/

#define STATE_MACHINE_STATES(state)\
state(idle) /* init state for startup code */ \
state(standby)\
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

static STATE_MACHINE_states_E prevState = 0; /* initialize previous state */
static STATE_MACHINE_states_E curState = 0; /* initialize current state */
static STATE_MACHINE_states_E nextState = standby_state; /* initialize current state */

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/
void StateMachine_DCDC_helper(void);
void StateMachine_precharge_helper(void);
/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void StateMachine_Init(void) {

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

void standby(STATE_MACHINE_entry_types_E entry_type) {
    switch (entry_type) {
        case ENTRY:
            CAN_changeOpMode(CAN_DISABLE);
            IO_SET_SW_EN(LOW);
            PINS_pullUp(CAN_TX_PIN, LOW);
            IO_SET_DCDC_EN(LOW);
            IO_SET_EV_CHARGER_EN(LOW);
            IO_SET_CHARGEPUMP_PWM(0);
            break;
        case EXIT:
            PINS_pullUp(CAN_TX_PIN, HIGH);
            IO_SET_SW_EN(HIGH);
            CAN_changeOpMode(CAN_NORMAL);
            IO_SET_CHARGEPUMP_PWM(50);
            break;
        case RUN:
            if (IO_GET_V12_POWER_STATUS()) {
                nextState = idle_state;
            }
            break;
        default:
            break;
    }
}

void idle(STATE_MACHINE_entry_types_E entry_type) {
    switch (entry_type) {
        case ENTRY:
            break;
        case EXIT:
            break;
        case RUN:
            StateMachine_DCDC_helper();
            StateMachine_precharge_helper();

            if (!IO_GET_V12_POWER_STATUS()) {
                nextState = standby_state;
            }
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
            break;
        default:
            break;
    }
}

void sleep(STATE_MACHINE_entry_types_E entry_type) {
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

/****Helpers*******************************************************************/

void StateMachine_DCDC_helper(void) {
    IO_SET_DCDC_EN(CAN_mcu_command_DCDC_enable_get());
}

void StateMachine_precharge_helper(void) {
    IO_SET_PRE_CHARGE_EN(CAN_mcu_command_precharge_enable_get());
}

/*** End of File **************************************************************/



