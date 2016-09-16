#include <stdio.h>

#include "FSM_TEMPLATE.h"
#include "Defines.h"
#include "configure.h"
#include "framework.h"

/*******************************************************************************
 * Debugging
 * ****************************************************************************/
static uint8_t debugEnable = 0;
#define FSM_TEMPLATE_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}

/*******************************************************************************
 * STATE MACHINE SETUP
 * ****************************************************************************/

#define FSM_TEMPLATE_State_list(state)\
state(init) /* init state for startup code */ \
state(someState) /* locked state before safemode */ \
state(anotherState) /* welcome state before locked */ \
state(yetAnotherState) /* locked state before safemode */ \

/*Creates an enum of states suffixed with _state*/
#define STATE_FORM(WORD) WORD##_state,

typedef enum {
    FSM_TEMPLATE_State_list(STATE_FORM)
    NUMBER_OF_STATES
} FSM_TEMPLATE_State_t;

/*creates a string-ified list of state string names*/
static const char __attribute__((unused)) * StateStrings[] = {
    FSM_TEMPLATE_State_list(STRING_FORM)
};

/* function-ifies the state list*/
#define FUNCTION_FORM(WORD) FSM_TEMPLATE_State_t WORD(Event ThisEvent);
#define FUNC_PTR_FORM(WORD) WORD,
FSM_TEMPLATE_State_list(FUNCTION_FORM)
typedef FSM_TEMPLATE_State_t(*statePtr)(Event);
statePtr theState[] = {FSM_TEMPLATE_State_list(FUNC_PTR_FORM)};


static FSM_TEMPLATE_State_t prevState = init_state; /* initialize previous state */
static FSM_TEMPLATE_State_t curState = init_state; /* initialize current state */

/*******************************************************************************
 * USER SPACE
 * ****************************************************************************/
/*Constant messages for the lcd screen*/
static const char helloString[] = "WELCOME TO THE E-MOTO";
static const char intruction[] = "Touch anywhere to wake up the bike";
static const char passwordMessage[] = "Enter Secret Passcode";
static const char unlocked[] = "Congrats, you unlocked the bike!";

/*Password Stuff*/
#define PASSCODE_SIZE 10
static uint8_t code[PASSCODE_SIZE + 1] = {};
static uint8_t codeIndex = 0;
static const uint8_t passcode[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1};

#define NO_CHAR_INPUT 0xFF
#define CHAR_INPUT 0
#define FAILED 1
#define PASSED 2

uint8_t passwordHandler(uint16_t temp);

/*******************************************************************************
 * STATE MACHINE BEGINS HERE
 * ****************************************************************************/
Event FSM_TEMPLATE(Event ThisEvent) {

    /*Debugging print statement*/
    FSM_TEMPLATE_print("Service: %s\tState: %s\tEvent: %s %d\n", ServiceStrings[FSM_TEMPLATE_SERVICE], StateStrings[curState], EventStrings[ThisEvent.EventType], ThisEvent.EventParam);

    /*Call the state machine functions*/
    FSM_TEMPLATE_State_t nextState = theState[curState](ThisEvent);

    /* This only happens during state transition
     * State transitions thus have priority over posting new events
     * State transitions always consist of an exit event to curState and entry event to nextState */
    if (nextState != curState) {
        FSM_TEMPLATE_print("\nTransistioning\n");
        ThisEvent.EventType = NO_EVENT;
        FSM_TEMPLATE(EXIT);
        prevState = curState;
        curState = nextState;
        FSM_TEMPLATE(ENTRY);
    }

    return ThisEvent;
}

/*******************************************************************************
 * 
 * @param ThisEvent
 * @return 
 */
FSM_TEMPLATE_State_t init(Event ThisEvent) {
    FSM_TEMPLATE_State_t nextState = curState;
    if (ThisEvent.EventType == INIT_EVENT) {
        /*Initialization stuff here*/

        /*LCD Init*/
        TFT_LCD_INIT(IO_PIN_RB12, IO_PIN_RB10, IO_PIN_RB11);
        TFT_TOUCH_INIT(IO_PIN_RA0, IO_PIN_RB2, IO_PIN_RA1, IO_PIN_RB3, AN5, AN4);

        /*fill background*/
        TFT_LCD_fillBackground(TFT_LCD_RED);
        TFT_LCD_drawRect(4, 4, TFT_LCD_width() - 4, TFT_LCD_height() - 4, TFT_LCD_BLACK);
        TFT_LCD_drawRect(8, 8, TFT_LCD_width() - 8, TFT_LCD_height() - 8, TFT_LCD_RED);

        nextState = someState_state;
    }
    return nextState;
}

/*******************************************************************************
 * 
 * @param ThisEvent
 * @return 
 */
FSM_TEMPLATE_State_t someState(Event ThisEvent) {
    FSM_TEMPLATE_State_t nextState = curState;
    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            TFT_LCD_writeString(helloString, TFT_LCD_CENTER, 100, TFT_LCD_MAGENTA, TFT_LCD_CYAN, 3);
            TFT_LCD_writeString(intruction, TFT_LCD_CENTER, 120, TFT_LCD_RED, TFT_LCD_CYAN, 2);
            /*Start a touch screen timer*/
            SW_Timer_Set(TOUCH_TIMER, TOUCH_TIME, FSM_TEMPLATE_SERVICE);
            break;
            /*Put custom states below here*/
        case TIMEUP_EVENT:
            switch (ThisEvent.EventParam) {
                case TOUCH_TIMER:
                    SW_Timer_Set(TOUCH_TIMER, TOUCH_TIME, FSM_TEMPLATE_SERVICE);
                    TFT_TOUCH_run();
                    if (TFT_TOUCH_draw(TFT_LCD_GREEN)) {
                        SW_Timer_Set(TRANSITION_TIMER, WAKE_UP_TOUCH_TIME, FSM_TEMPLATE_SERVICE);
                    }
                    break;
                case TRANSITION_TIMER:
                    nextState = anotherState_state;
                    break;
                default:
                    break;
            }

            break;
            /*Put custom states above here*/
        case EXIT_EVENT:
            TFT_LCD_drawRect(8, 8, TFT_LCD_width() - 8, TFT_LCD_height() - 8, TFT_LCD_RED);
            break;
        default:
            break;
    }
    return nextState;
}

/*******************************************************************************
 * 
 * @param ThisEvent
 * @return 
 */
FSM_TEMPLATE_State_t anotherState(Event ThisEvent) {
    FSM_TEMPLATE_State_t nextState = curState;
    switch (ThisEvent.EventType) {
            static uint8_t buttonArray[10];
        case ENTRY_EVENT:
            /*Create a keypad*/
            buttonArray[0] = TFT_DISPLAY_place_button("1", 2, 1, TFT_LCD_GREEN, 4);
            buttonArray[1] = TFT_DISPLAY_place_button("2", 3, 1, TFT_LCD_GREEN, 4);
            buttonArray[2] = TFT_DISPLAY_place_button("3", 4, 1, TFT_LCD_GREEN, 4);
            buttonArray[3] = TFT_DISPLAY_place_button("4", 2, 2, TFT_LCD_GREEN, 4);
            buttonArray[4] = TFT_DISPLAY_place_button("5", 3, 2, TFT_LCD_GREEN, 4);
            buttonArray[5] = TFT_DISPLAY_place_button("6", 4, 2, TFT_LCD_GREEN, 4);
            buttonArray[6] = TFT_DISPLAY_place_button("7", 2, 3, TFT_LCD_GREEN, 4);
            buttonArray[7] = TFT_DISPLAY_place_button("8", 3, 3, TFT_LCD_GREEN, 4);
            buttonArray[8] = TFT_DISPLAY_place_button("9", 4, 3, TFT_LCD_GREEN, 4);
            buttonArray[9] = TFT_DISPLAY_place_button("CLEAR", 1, 3, TFT_LCD_GREEN, 2);
            TFT_LCD_writeString(passwordMessage, TFT_LCD_CENTER, 275, TFT_LCD_RED, TFT_LCD_CYAN, 3);

            /*Start a touch screen timer*/
            SW_Timer_Set(TOUCH_TIMER, TOUCH_TIME, FSM_TEMPLATE_SERVICE);
            break;
            /*Put custom states below here*/
        case TIMEUP_EVENT:
            switch (ThisEvent.EventParam) {
                case TOUCH_TIMER:
                    SW_Timer_Set(TOUCH_TIMER, TOUCH_TIME, FSM_TEMPLATE_SERVICE);
                    TFT_TOUCH_run();
                    /*button handler returns the number of which button was pressed, or 0xFF for SNA*/
                    uint8_t temp = TFT_DISPLAY_button_handler();
                    switch (passwordHandler(temp)) {
                        case CHAR_INPUT:
                            TFT_LCD_writeVariableString((char*) code, TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                            break;
                        case FAILED:
                            TFT_LCD_writeVariableString((char*) code, TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                            TFT_LCD_writeString("          ", TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                            break;
                        case PASSED:
                            TFT_LCD_writeVariableString((char*) code, TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                            nextState = yetAnotherState_state;
                            break;
                        case NO_CHAR_INPUT:
                        default:
                            break;
                    }
                    /*if button has been pressed run it through the password machine*/

                    break;
                default:
                    break;
            }
            break;
            /*Put custom states above here*/
        case EXIT_EVENT:
            TFT_DISPLAY_destroy_buttons();
            TFT_LCD_drawRect(8, 8, TFT_LCD_width() - 8, TFT_LCD_height() - 8, TFT_LCD_RED);
            break;
        default:
            break;
    }
    return nextState;
}

/*******************************************************************************
 * 
 * @param ThisEvent
 * @return 
 */
FSM_TEMPLATE_State_t yetAnotherState(Event ThisEvent) {
    FSM_TEMPLATE_State_t nextState = curState;
    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            TFT_LCD_writeString(unlocked, TFT_LCD_CENTER, 100, TFT_LCD_MAGENTA, TFT_LCD_CYAN, 2);
            //TFT_LCD_goToSleep();
            //TFT_LCD_writeString(intruction, TFT_LCD_CENTER, 100, TFT_LCD_MAGENTA, TFT_LCD_CYAN, 2);
            break;
            /*Put custom states below here*/

            /*Put custom states above here*/
        case EXIT_EVENT:
            break;
        default:
            break;
    }
    return nextState;
}

uint8_t passwordHandler(uint16_t temp) {
    uint8_t returnVal = NO_CHAR_INPUT;
    if (temp == 9) {
        returnVal = FAILED;
        code[0] = codeIndex = 0; /*make string empty*/
    } else if (temp != NO_CHAR_INPUT) {
        returnVal = CHAR_INPUT;
        code[codeIndex++] = temp + 49;
        code[codeIndex] = 0; /*move terminating NULL character up each time*/
        if (codeIndex >= 10) {
            returnVal = PASSED;
            codeIndex = 0;
            uint8_t i = 0;
            for (i = 0; i < 10; i++) {
                if (code[i] != passcode[i] + 48) {
                    returnVal = FAILED;
                    break;
                }
            }
        }
    }
    return returnVal;
}

void clearPassword() {

}