/******************************************************************************
 * Includes
 *******************************************************************************/
#include "TFT_LCD.h"
#include "DISPLAY_INTERFACE.h"
#include "SysTick.h"
#include "dash_dbc.h"

#include <stdio.h>
#include <string.h>

/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/

#define STATE_MACHINE_STATES(state)\
state(init) /* init state for startup code */ \
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
static STATE_MACHINE_states_E nextState = init_state; /* initialize current state */

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/


/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void StateMachine_Init(void){

}

void StateMachine_Run(void){

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

void init(STATE_MACHINE_entry_types_E entry_type){
    switch(entry_type){
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

void standby(STATE_MACHINE_entry_types_E entry_type){
        switch(entry_type){
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

void running(STATE_MACHINE_entry_types_E entry_type){
        switch(entry_type){
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

void charging(STATE_MACHINE_entry_types_E entry_type){
        switch(entry_type){
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

void sleep(STATE_MACHINE_entry_types_E entry_type){
        switch(entry_type){
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

/*** End of File **************************************************************/



