/******************************************************************************
 * Includes
 *******************************************************************************/
#include "SysTick.h"
#include "IO.h"
#include "bolt_CAN.h"
#include "bms_dbc.h"
#include "pinSetup.h"
#include "bolt_sleep.h"
#include "dcdc.h"
#include "can_iso_tp_lite.h"


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

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void StateMachine_Init(void) {
    DCDC_init();
}

void StateMachine_Run(void) {

    switch (isoTP_getCommand()) {
        case ISO_TP_NONE:
            break;
        case ISO_TP_RESET:
            CAN_changeOpMode(CAN_DISABLE);
            IO_SET_SW_EN(LOW);
            {uint32_t i = 0;
            for (i = 0; i < 3000000; i++){
                Nop();
            }}
            asm ("reset");
            break;
        case ISO_TP_SLEEP:
            nextState = sleep_state;
            break;
        case ISO_TP_IO_CONTROL:
            break;
        case ISO_TP_TESTER_PRESENT:
            break;
        default:
            break;
    }

    /* This only happens during state transition
     * State transitions thus have priority over posting new events
     * State transitions always consist of an exit event to curState and entry event to nextState */
    if (nextState != curState) {
        state_functions[curState](EXIT);
        prevState = curState;
        curState = nextState;
        state_functions[curState](ENTRY);
    }
    
    state_functions[curState](RUN);

}

void idle(STATE_MACHINE_entry_types_E entry_type) {
    switch (entry_type) {
        case ENTRY:
            PINS_pullUp(CAN_TX_PIN, HIGH);
            IO_SET_SW_EN(HIGH);
            CAN_changeOpMode(CAN_NORMAL);
            IO_SET_CHARGEPUMP_PWM(50);
            break;
        case EXIT:
            break;
        case RUN:
            //StateMachine_DCDC_helper();
            //StateMachine_precharge_helper();

            if (IO_GET_V12_POWER_STATUS() == 0) {
                nextState = standby_state;
            }
            break;
        default:
            break;
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
            break;
        case RUN:
            if (IO_GET_V12_POWER_STATUS()) {
                nextState = idle_state;
            } else {
                nextState = sleep_state;
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
            DCDC_halt();
            IO_SET_DEBUG_LED_EN(LOW);
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
                //nextState = silent_wake_state;
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



