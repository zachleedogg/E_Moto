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

/*Screen Locations*/
#define MENU_MESSAGE_Y_POS 50
#define DATA_ROW_OFFSET 100
#define DATA_ROW_HEIGHT 16
#define DATA_ROW_1 (DATA_ROW_OFFSET)
#define DATA_ROW_2 (DATA_ROW_OFFSET+DATA_ROW_HEIGHT)
#define DATA_ROW_3 (DATA_ROW_OFFSET+(2*DATA_ROW_HEIGHT))
#define DATA_ROW_4 (DATA_ROW_OFFSET+(3*DATA_ROW_HEIGHT))
#define DATA_ROW_5 (DATA_ROW_OFFSET+(4*DATA_ROW_HEIGHT))
#define DATA_ROW_6 (DATA_ROW_OFFSET+(5*DATA_ROW_HEIGHT))
#define DATA_ROW_7 (DATA_ROW_OFFSET+(6*DATA_ROW_HEIGHT))
#define DATA_ROW_8 (DATA_ROW_OFFSET+(7*DATA_ROW_HEIGHT))

#define DATA_COLUMN_OFFSET 100
#define DATA_COLUMN_WIDTH (6*20)
#define DATA_COLUMN_1 (DATA_COLUMN_OFFSET)
#define DATA_COLUMN_2 (DATA_COLUMN_OFFSET+DATA_COLUMN_WIDTH)
#define DATA_COLUMN_3 (DATA_COLUMN_OFFSET+(2*DATA_COLUMN_WIDTH))

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
#define PASSWORD "5236"

STATE_MACHINE_STATES(FUNCTION_FORM)
static statePtr state_functions[] = {STATE_MACHINE_STATES(FUNC_PTR_FORM)};

static STATE_MACHINE_states_E prevState = 0; /* initialize previous state */
static STATE_MACHINE_states_E curState = 0; /* initialize current state */
static STATE_MACHINE_states_E nextState = init_state; /* initialize current state */

/*Constant messages for the lcd screen*/
#define LCD_BACKGROUND TFT_LCD_WHITE
static const char message_helloWorld[] = "WELCOME TO THE E-MOTO!";
static const char message_instructions[] = "Draw on the screen to begin";
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
uint8_t passcode_handler(uint8_t input);


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
         DISPLAY_INTERFACE_enable_drawing(1, TFT_LCD_GREEN);
         break;
     case EXIT:
         DISPLAY_INTERFACE_enable_drawing(0, TFT_LCD_GREEN);
         TFT_LCD_fillBackground(LCD_BACKGROUND);
         break;
     case RUN:
         if (SysTick_Get()-timeNow > 3000){
             nextState = locked_state;
         }
         break;
     default:
         break;
    }
}

void locked(STATE_MACHINE_entry_types_E entry_type){
    static uint8_t buttonArray[10];
    uint8_t userInputValue = 0xFF;

    switch(entry_type){
        case ENTRY:
            buttonArray[0] = DISPLAY_INTERFACE_place_button("CLEAR", 4, 2, TFT_LCD_GREEN, 2);
            buttonArray[1] = DISPLAY_INTERFACE_place_button("1", 1, 1, TFT_LCD_GREEN, 4);
            buttonArray[2] = DISPLAY_INTERFACE_place_button("2", 2, 1, TFT_LCD_GREEN, 4);
            buttonArray[3] = DISPLAY_INTERFACE_place_button("3", 3, 1, TFT_LCD_GREEN, 4);
            buttonArray[4] = DISPLAY_INTERFACE_place_button("4", 1, 2, TFT_LCD_GREEN, 4);
            buttonArray[5] = DISPLAY_INTERFACE_place_button("5", 2, 2, TFT_LCD_GREEN, 4);
            buttonArray[6] = DISPLAY_INTERFACE_place_button("6", 3, 2, TFT_LCD_GREEN, 4);
            buttonArray[7] = DISPLAY_INTERFACE_place_button("7", 1, 3, TFT_LCD_GREEN, 4);
            buttonArray[8] = DISPLAY_INTERFACE_place_button("8", 2, 3, TFT_LCD_GREEN, 4);
            buttonArray[9] = DISPLAY_INTERFACE_place_button("9", 3, 3, TFT_LCD_GREEN, 4);
            buttonArray[9] = DISPLAY_INTERFACE_place_button("ENTER", 4, 3, TFT_LCD_GREEN, 2);
            TFT_LCD_writeString(message_enterPasscode, TFT_LCD_CENTER, 250, LCD_BACKGROUND, TFT_LCD_CYAN, 3);
            break;
        case EXIT:
            DISPLAY_INTERFACE_destroy_buttons();
            TFT_LCD_fillBackground(LCD_BACKGROUND);
            break;
        case RUN:
            userInputValue = DISPLAY_INTERFACE_button_handler();
            if(passcode_handler(userInputValue)){
                nextState = home_state;
            };
            break;
        default:
            break;
    }

}

void home(STATE_MACHINE_entry_types_E entry_type){
    static uint8_t buttonArray[10];
    uint8_t userInputValue = 0xFF;
    switch(entry_type){
        case ENTRY:
            TFT_LCD_writeString(message_bikeUnlocked, TFT_LCD_CENTER, 50, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            buttonArray[0] = DISPLAY_INTERFACE_place_button("RIDE", 1, 4, TFT_LCD_GREEN, 2);
            buttonArray[1] = DISPLAY_INTERFACE_place_button("STATS", 2, 4, TFT_LCD_GREEN, 2);
            buttonArray[2] = DISPLAY_INTERFACE_place_button("LAST", 3, 4, TFT_LCD_GREEN, 2);
            buttonArray[3] = DISPLAY_INTERFACE_place_button("BATTERY", 4, 4, TFT_LCD_GREEN, 2);
            break;
        case EXIT:
            DISPLAY_INTERFACE_destroy_buttons();
            TFT_LCD_fillBackground(LCD_BACKGROUND);
            break;
        case RUN:
            userInputValue = DISPLAY_INTERFACE_button_handler();
            switch(userInputValue){
                case 0:
                    nextState = running_state;
                    break;
                case 1:
                    nextState = statistic_state;
                    break;
                case 2:
                    nextState = lastRide_state;
                    break;
                case 3:
                    nextState = battery_state;
                    break;
                default:
                    break;
            }
            
            break;
        default:
            break;
    }
}

void running(STATE_MACHINE_entry_types_E entry_type){
    static uint32_t timeNow = 0;
    static uint16_t speed = 0;
    static uint8_t buttonArray[10];
    uint8_t userInputValue = 0xFF;
    switch(entry_type){
        case ENTRY:
            TFT_LCD_writeString(message_runningMode, TFT_LCD_CENTER, 50, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            buttonArray[0] = DISPLAY_INTERFACE_place_button("RIDE", 1, 4, TFT_LCD_GREEN, 2);
            buttonArray[1] = DISPLAY_INTERFACE_place_button("STATS", 2, 4, TFT_LCD_GREEN, 2);
            buttonArray[2] = DISPLAY_INTERFACE_place_button("LAST", 3, 4, TFT_LCD_GREEN, 2);
            buttonArray[3] = DISPLAY_INTERFACE_place_button("BATTERY", 4, 4, TFT_LCD_GREEN, 2);
            TFT_LCD_writeString("mph", TFT_LCD_CENTER+100, 120, LCD_BACKGROUND, TFT_LCD_BLACK, 6);
            speed = 0;
            break;
        case EXIT:
            DISPLAY_INTERFACE_destroy_buttons();
            TFT_LCD_fillBackground(LCD_BACKGROUND);
            break;
        case RUN:
            if (SysTick_Get()-timeNow > 250){
                uint16_t speedNew = CAN_motorcontroller_motorStatus_motorSpeed_get();
                if(speedNew != speed){
                    speed = speedNew;
                    char speedStr[4];
                    sprintf(speedStr, "%3d", speed);
                    TFT_LCD_writeVariableString(speedStr, TFT_LCD_CENTER-150, 120, LCD_BACKGROUND, TFT_LCD_BLACK, 12);
                }
            }
            userInputValue = DISPLAY_INTERFACE_button_handler();
            switch(userInputValue){
                case 0:
                    nextState = running_state;
                    break;
                case 1:
                    nextState = statistic_state;
                    break;
                case 2:
                    nextState = lastRide_state;
                    break;
                case 3:
                    nextState = battery_state;
                    break;
                default:
                    break;
            }
            
            break;
        default:
            break;
    }
}

void statistic(STATE_MACHINE_entry_types_E entry_type){
    static uint8_t buttonArray[10];
    uint8_t userInputValue = 0xFF;
    switch(entry_type){
        case ENTRY:
            TFT_LCD_writeString(message_statistics, TFT_LCD_CENTER, 50, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            buttonArray[0] = DISPLAY_INTERFACE_place_button("RIDE", 1, 4, TFT_LCD_GREEN, 2);
            buttonArray[1] = DISPLAY_INTERFACE_place_button("STATS", 2, 4, TFT_LCD_GREEN, 2);
            buttonArray[2] = DISPLAY_INTERFACE_place_button("LAST", 3, 4, TFT_LCD_GREEN, 2);
            buttonArray[3] = DISPLAY_INTERFACE_place_button("BATTERY", 4, 4, TFT_LCD_GREEN, 2);
            TFT_LCD_writeString("Odo: 5180mi", DATA_COLUMN_1, DATA_ROW_1, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Economy: 450Wh/mi", DATA_COLUMN_1, DATA_ROW_2, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Ride Time: 197hrs", DATA_COLUMN_1, DATA_ROW_3, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Efficiency: 78%:", DATA_COLUMN_1, DATA_ROW_4, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("12V Battery Voltage:", DATA_COLUMN_1, DATA_ROW_5, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            break;
        case EXIT:
            DISPLAY_INTERFACE_destroy_buttons();
            TFT_LCD_fillBackground(LCD_BACKGROUND);
            break;
        case RUN:
            userInputValue = DISPLAY_INTERFACE_button_handler();
            switch(userInputValue){
                case 0:
                    nextState = running_state;
                    break;
                case 1:
                    nextState = statistic_state;
                    break;
                case 2:
                    nextState = lastRide_state;
                    break;
                case 3:
                    nextState = battery_state;
                    break;
                default:
                    break;
            }
            
            break;
        default:
            break;
    }
}

void lastRide(STATE_MACHINE_entry_types_E entry_type){
    static uint8_t buttonArray[10];
    uint8_t userInputValue = 0xFF;
    switch(entry_type){
        case ENTRY:
            TFT_LCD_writeString(message_lastRide, TFT_LCD_CENTER, 50, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            buttonArray[0] = DISPLAY_INTERFACE_place_button("RIDE", 1, 4, TFT_LCD_GREEN, 2);
            buttonArray[1] = DISPLAY_INTERFACE_place_button("STATS", 2, 4, TFT_LCD_GREEN, 2);
            buttonArray[2] = DISPLAY_INTERFACE_place_button("LAST", 3, 4, TFT_LCD_GREEN, 2);
            buttonArray[3] = DISPLAY_INTERFACE_place_button("BATTERY", 4, 4, TFT_LCD_GREEN, 2);
            TFT_LCD_writeString("Distance: 5180mi", DATA_COLUMN_1, DATA_ROW_1, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Economy: 800Wh/mi", DATA_COLUMN_1, DATA_ROW_2, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Ride Time: 35mins", DATA_COLUMN_1, DATA_ROW_3, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Efficiency: 78%:", DATA_COLUMN_1, DATA_ROW_4, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Average Speed: 54mph", DATA_COLUMN_1, DATA_ROW_5, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Max Speed: 72mph", DATA_COLUMN_1, DATA_ROW_6, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Average Power: 1200W", DATA_COLUMN_1, DATA_ROW_6, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            break;
        case EXIT:
            DISPLAY_INTERFACE_destroy_buttons();
            TFT_LCD_fillBackground(LCD_BACKGROUND);
            break;
        case RUN:
            userInputValue = DISPLAY_INTERFACE_button_handler();
            switch(userInputValue){
                case 0:
                    nextState = running_state;
                    break;
                case 1:
                    nextState = statistic_state;
                    break;
                case 2:
                    nextState = lastRide_state;
                    break;
                case 3:
                    nextState = battery_state;
                    break;
                default:
                    break;
            }
            
            break;
        default:
            break;
    }
}

void battery(STATE_MACHINE_entry_types_E entry_type){
    static uint8_t buttonArray[10];
    uint8_t userInputValue = 0xFF;
    switch(entry_type){
        case ENTRY:
            TFT_LCD_writeString(message_battery, TFT_LCD_CENTER, 50, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            buttonArray[0] = DISPLAY_INTERFACE_place_button("RIDE", 1, 4, TFT_LCD_GREEN, 2);
            buttonArray[1] = DISPLAY_INTERFACE_place_button("STATS", 2, 4, TFT_LCD_GREEN, 2);
            buttonArray[2] = DISPLAY_INTERFACE_place_button("LAST", 3, 4, TFT_LCD_GREEN, 2);
            buttonArray[3] = DISPLAY_INTERFACE_place_button("BATTERY", 4, 4, TFT_LCD_GREEN, 2);
            TFT_LCD_writeString("Cell 0:", DATA_COLUMN_1, DATA_ROW_1, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 1:", DATA_COLUMN_1, DATA_ROW_2, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 2:", DATA_COLUMN_1, DATA_ROW_3, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 3:", DATA_COLUMN_1, DATA_ROW_4, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 4:", DATA_COLUMN_1, DATA_ROW_5, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 5:", DATA_COLUMN_1, DATA_ROW_6, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 6:", DATA_COLUMN_1, DATA_ROW_7, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 7:", DATA_COLUMN_1, DATA_ROW_8, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 8:", DATA_COLUMN_2, DATA_ROW_1, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 9:", DATA_COLUMN_2, DATA_ROW_2, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 10:", DATA_COLUMN_2, DATA_ROW_3, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 11:", DATA_COLUMN_2, DATA_ROW_4, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 12:", DATA_COLUMN_2, DATA_ROW_5, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 13:", DATA_COLUMN_2, DATA_ROW_6, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 14:", DATA_COLUMN_2, DATA_ROW_7, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 15:", DATA_COLUMN_2, DATA_ROW_8, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 16:", DATA_COLUMN_3, DATA_ROW_1, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 17:", DATA_COLUMN_3, DATA_ROW_2, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 18:", DATA_COLUMN_3, DATA_ROW_3, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 19:", DATA_COLUMN_3, DATA_ROW_4, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 20:", DATA_COLUMN_3, DATA_ROW_5, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 21:", DATA_COLUMN_3, DATA_ROW_6, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 22:", DATA_COLUMN_3, DATA_ROW_7, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            TFT_LCD_writeString("Cell 23:", DATA_COLUMN_3, DATA_ROW_8, LCD_BACKGROUND, TFT_LCD_CYAN, 2);
            break;
        case EXIT:
            DISPLAY_INTERFACE_destroy_buttons();
            TFT_LCD_fillBackground(LCD_BACKGROUND);
            break;
        case RUN:
            userInputValue = DISPLAY_INTERFACE_button_handler();
            switch(userInputValue){
                case 0:
                    nextState = running_state;
                    break;
                case 1:
                    nextState = statistic_state;
                    break;
                case 2:
                    nextState = lastRide_state;
                    break;
                case 3:
                    nextState = battery_state;
                    break;
                default:
                    break;
            }
            
            break;
        default:
            break;
    }
}

void idle(STATE_MACHINE_entry_types_E entry_type){
    
}

uint8_t passcode_handler(uint8_t input){
    static char code_buffer[10];
    uint8_t return_val = 0;
    switch(input){
        case 0xFF:
            break;
        case 0:
            code_buffer[0] = 0;
            TFT_LCD_writeVariableString("        ", TFT_LCD_CENTER, 275, LCD_BACKGROUND, TFT_LCD_CYAN, 3);
        case 10:
            if (!strcmp(code_buffer, PASSWORD)){
                return_val = 1;
            }
            break;
        case 1 ... 9:
            if (strlen(code_buffer) > 9){
                break;
            }
            char newval[2];
            sprintf(newval, "%d", input);
            strncat(code_buffer, newval, 1);
            TFT_LCD_writeVariableString(code_buffer, TFT_LCD_CENTER, 275, LCD_BACKGROUND, TFT_LCD_CYAN, 3);
            break;
        default:
            break;
    }
    return return_val;
}

/*** End of File **************************************************************/



