/******************************************************************************
 * Includes
 *******************************************************************************/
#include "TFT_LCD.h"
#include "DISPLAY_INTERFACE.h"
#include "SysTick.h"

/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/

#define STATE_MACHINE_STATES(state)\
state(init) /* init state for startup code */ \
state(welcome) /* have fun drawing on the screen */ \
state(locked) /* unlock the bike with your passcode */ \
state(home) /* select from the menu on the home screen */ \
state(running) /* see your complete dashboard in running mode */ \
state(statistic) /*See you ride stats */ \
state(lastRide) /*See you ride stats */ \
state(battery) /* Monitor your battery health */ \
state(idle) /* Monitor your battery health */ \

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

/*Constant messages for the lcd screen*/
#define LCD_BACKGROUND TFT_LCD_WHITE
static const char message_helloWorld[] = "WELCOME TO THE E-MOTO!";
static const char message_instructions[] = "Draw a dick on the screen to begin";
static const char message_enterPasscode[] = "Enter Secret Passcode";
static const char message_bikeUnlocked[] = "Congrats, you unlocked the bike!";
static const char message_runningMode[] = "Press Ignition to Start";
static const char message_statistics[] = "Performance Statistics";
static const char message_lastRide[] = "Last Ride Data";
static const char message_battery[] = "Battery Usage";
static const char message_idle[] = "Idle";
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
            TFT_LCD_fillBackground(LCD_BACKGROUND);
            break;
        case RUN:
            nextState = welcome_state;
            break;
        default:
            break;
    }
    
}

void welcome(STATE_MACHINE_entry_types_E entry_type){
    static uint32_t timeNow = 0;
       switch(entry_type){
        case ENTRY:
            timeNow = SysTick_Get();
            TFT_LCD_writeString(message_helloWorld, TFT_LCD_CENTER, 100, LCD_BACKGROUND, TFT_LCD_CYAN, 3);
            TFT_LCD_writeString(message_instructions, TFT_LCD_CENTER, 150, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            break;
        case EXIT:
            break;
        case RUN:
            if (SysTick_Get()-timeNow > 1000){
                nextState = locked_state;
            }
            break;
        default:
            break;
    }

    
}

void locked(STATE_MACHINE_entry_types_E entry_type){
        switch(entry_type){
        case ENTRY:
            TFT_LCD_writeString(message_enterPasscode, TFT_LCD_CENTER, 175, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            break;
        case EXIT:
            TFT_LCD_fillBackground(LCD_BACKGROUND);
            break;
        case RUN:
            DISPLAY_INTERFACE_draw(TFT_LCD_GREEN);
            break;
        default:
            break;
    }

}

void home(STATE_MACHINE_entry_types_E entry_type){
    
}

void running(STATE_MACHINE_entry_types_E entry_type){
    
}

void statistic(STATE_MACHINE_entry_types_E entry_type){
    
}

void lastRide(STATE_MACHINE_entry_types_E entry_type){
    
}

void battery(STATE_MACHINE_entry_types_E entry_type){
    
}

void idle(STATE_MACHINE_entry_types_E entry_type){
    
}


/*** End of File **************************************************************/



