#include <stdio.h>

#include "touchScreenService.h"
#include "Defines.h"
#include "configure.h"
#include "framework.h"

/*******************************************************************************
 * Debugging
 * ****************************************************************************/
static uint8_t debugEnable = 0;
#define touchScreenService_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}

/*******************************************************************************
 * STATE MACHINE SETUP
 * ****************************************************************************/

#define touchScreenService_State_list(state)\
state(init) /* init state for startup code */ \
state(welcomeState) /* have fun drawing on the screen */ \
state(lockedState) /* unlock the bike with your passcode */ \
state(homeState) /* select from the menu on the home screen */ \
state(runningState) /* see your complete dashboard in running mode */ \
state(statisticState) /*See you ride stats */ \
state(batteryState) /* Monitor your battery health */ \

/*Creates an enum of states suffixed with _state*/
#define STATE_FORM(WORD) WORD##_state,

typedef enum {
    touchScreenService_State_list(STATE_FORM)
    NUMBER_OF_STATES
} touchScreenService_State_t;

/*creates a string-ified list of state string names*/
static const char __attribute__((unused)) * StateStrings[] = {
    touchScreenService_State_list(STRING_FORM)
};

/* function-ifies the state list*/
#define FUNCTION_FORM(WORD) touchScreenService_State_t WORD(Event ThisEvent);
#define FUNC_PTR_FORM(WORD) WORD,
touchScreenService_State_list(FUNCTION_FORM)
typedef touchScreenService_State_t(*statePtr)(Event);
statePtr theState[] = {touchScreenService_State_list(FUNC_PTR_FORM)};


static touchScreenService_State_t prevState = init_state; /* initialize previous state */
static touchScreenService_State_t curState = init_state; /* initialize current state */

/*******************************************************************************
 * USER SPACE
 * ****************************************************************************/
/*Constant messages for the lcd screen*/
static const char helloString[] = "WELCOME TO THE E-MOTO";
static const char intruction[] = "Draw a dick on the screen";
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

/*Speedo stuff*/
static uint8_t speedo = 0;

/*******************************************************************************
 * STATE MACHINE BEGINS HERE
 * ****************************************************************************/
Event touchScreenService(Event ThisEvent) {

    /*Debugging print statement*/
    touchScreenService_print("Service: %s\tState: %s\tEvent: %s %d\n", ServiceStrings[touchScreenService_SERVICE], StateStrings[curState], EventStrings[ThisEvent.EventType], ThisEvent.EventParam);

    /*Call the state machine functions*/
    touchScreenService_State_t nextState = theState[curState](ThisEvent);

    /* This only happens during state transition
     * State transitions thus have priority over posting new events
     * State transitions always consist of an exit event to curState and entry event to nextState */
    if (nextState != curState) {
        touchScreenService_print("\nTransistioning\n");
        ThisEvent.EventType = NO_EVENT;
        touchScreenService(EXIT);
        prevState = curState;
        curState = nextState;
        touchScreenService(ENTRY);
    }

    return ThisEvent;
}

/*******************************************************************************
 * 
 * @param ThisEvent
 * @return 
 */
touchScreenService_State_t init(Event ThisEvent) {
    touchScreenService_State_t nextState = curState;
    if (ThisEvent.EventType == INIT_EVENT) {
        /*Initialization stuff here*/

        /*LCD Init*/
        TFT_LCD_INIT(IO_PIN_RB12, IO_PIN_RB10, IO_PIN_RB11);
        TFT_TOUCH_INIT(IO_PIN_RA0, IO_PIN_RB2, IO_PIN_RA1, IO_PIN_RB3, AN5, AN4);

        /*fill background*/
        TFT_LCD_fillBackground(TFT_LCD_RED);
        TFT_LCD_drawRect(4, 4, TFT_LCD_width() - 4, TFT_LCD_height() - 4, TFT_LCD_RED);
        TFT_LCD_drawRect(8, 8, TFT_LCD_width() - 8, TFT_LCD_height() - 8, TFT_LCD_RED);

        nextState = welcomeState_state;
    }
    return nextState;
}

/*******************************************************************************
 * 
 * @param ThisEvent
 * @return 
 */
touchScreenService_State_t welcomeState(Event ThisEvent) {
    touchScreenService_State_t nextState = curState;
    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            TFT_LCD_writeString(helloString, TFT_LCD_CENTER, 100, TFT_LCD_MAGENTA, TFT_LCD_CYAN, 3);
            TFT_LCD_writeString(intruction, TFT_LCD_CENTER, 120, TFT_LCD_RED, TFT_LCD_CYAN, 2);
            /*Start a touch screen timer*/
            SW_Timer_Set(TOUCH_TIMER, TOUCH_TIME, touchScreenService_SERVICE, CONTINUOUS);
            break;
            /*Put custom states below here*/
        case TIMEUP_EVENT:
            switch (ThisEvent.EventParam) {
                case TOUCH_TIMER:
                    /*If screen is being touched*/
                    if (TFT_TOUCH_run()) {
                        /*Draw a pixel for fun and set transition timer*/
                        if (TFT_TOUCH_draw(TFT_LCD_GREEN)) {
                            SW_Timer_Set(TRANSITION_TIMER, WAKE_UP_TOUCH_TIME, touchScreenService_SERVICE, SINGLE_SHOT);
                        }
                    }
                    break;
                case TRANSITION_TIMER:
                    nextState = lockedState_state;
                    break;
                default:
                    break;
            }
            break;
            /*Put custom states above here*/
        case EXIT_EVENT:
            SW_Timer_Stop(TOUCH_TIMER);
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
touchScreenService_State_t lockedState(Event ThisEvent) {

    touchScreenService_State_t nextState = curState;
    static uint8_t buttonArray[10];

    switch (ThisEvent.EventType) {
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
            SW_Timer_Set(TOUCH_TIMER, TOUCH_TIME, touchScreenService_SERVICE, CONTINUOUS);
            break;
            /*Put custom states below here*/
        case TIMEUP_EVENT:
            switch (ThisEvent.EventParam) {
                case TOUCH_TIMER:
                    /*if button has been pressed run it through the password machine*/
                    TFT_TOUCH_run();
                    /*button handler returns the number of which button was pressed, or 0xFF for SNA*/
                    uint8_t passwordResult = TFT_DISPLAY_button_handler();
                    switch (passwordHandler(passwordResult)) {
                        case CHAR_INPUT:
                            /*Write the character to the screen*/
                            TFT_LCD_writeVariableString((char*) code, TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                            break;
                        case FAILED:
                            /*Clear all chars from screen*/
                            TFT_LCD_writeVariableString((char*) code, TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                            TFT_LCD_writeString("          ", TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                            break;
                        case PASSED:
                            /*Write the final char to screen and transistion to next state*/
                            TFT_LCD_writeVariableString((char*) code, TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                            nextState = homeState_state;
                            break;
                        case NO_CHAR_INPUT:
                        default:
                            break;
                    }

                    break;
                default:
                    break;
            }
            break;
            /*Put custom states above here*/
        case EXIT_EVENT:
            SW_Timer_Stop(TOUCH_TIMER);
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
touchScreenService_State_t homeState(Event ThisEvent) {

    touchScreenService_State_t nextState = curState;
    static uint8_t buttonArray[4];

    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            TFT_LCD_writeString(unlocked, TFT_LCD_CENTER, 100, TFT_LCD_MAGENTA, TFT_LCD_CYAN, 2);
            buttonArray[0] = TFT_DISPLAY_place_button("RIDE", 1, 4, TFT_LCD_GREEN, 2);
            buttonArray[1] = TFT_DISPLAY_place_button("STATS", 2, 4, TFT_LCD_GREEN, 2);
            buttonArray[2] = TFT_DISPLAY_place_button("LAST", 3, 4, TFT_LCD_GREEN, 2);
            buttonArray[3] = TFT_DISPLAY_place_button("BATTERY", 4, 4, TFT_LCD_GREEN, 2);
            /*Start a touch screen timer*/
            SW_Timer_Set(TOUCH_TIMER, TOUCH_TIME, touchScreenService_SERVICE, CONTINUOUS);
            break;
            /*Put custom states below here*/
        case TIMEUP_EVENT:
            switch (ThisEvent.EventParam) {
                case TOUCH_TIMER:
                    /*If screen is being touched*/
                    TFT_TOUCH_run();
                    switch (TFT_DISPLAY_button_handler()) {
                        case 0:
                            nextState = runningState_state;
                            break;
                        case 1:
                            nextState = statisticState_state;
                            break;
                        case 2:
                            nextState = statisticState_state;
                            break;
                        case 3:
                            nextState = batteryState_state;
                            break;
                        default:
                            break;
                    }

                    break;
                default:
                    break;
            }
            break;
            /*Put custom states above here*/
        case EXIT_EVENT:
            SW_Timer_Stop(TOUCH_TIMER);
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
touchScreenService_State_t runningState(Event ThisEvent) {

    touchScreenService_State_t nextState = curState;
    static uint8_t buttonArray[2];

    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            buttonArray[0] = TFT_DISPLAY_place_button("MODE", 1, 4, TFT_LCD_GREEN, 2);
            buttonArray[1] = TFT_DISPLAY_place_button("SELECT", 4, 4, TFT_LCD_GREEN, 2);
            /*Start a touch screen timer*/
            SW_Timer_Set(TOUCH_TIMER, TOUCH_TIME, touchScreenService_SERVICE, CONTINUOUS);
            /*Start a Speedo Timer*/
            SW_Timer_Set(SPEEDO_TIMER, SPEEDO_TIME, touchScreenService_SERVICE, CONTINUOUS);
            break;
            /*Put custom states below here*/
        case TIMEUP_EVENT:
            switch (ThisEvent.EventParam) {
                case TOUCH_TIMER:
                    /*If screen is being touched*/
                    TFT_TOUCH_run();
                    switch (TFT_DISPLAY_button_handler()) {
                        case 0:
                            nextState = homeState_state;
                            break;
                        case 1:
                            nextState = statisticState_state;
                            break;
                        default:
                            break;
                    }
                    break;
                case SPEEDO_TIMER:
                    speedo++;
                    if(speedo == 25){
                        speedo = 0;
                    }
                    char tempStr[3];
                    sprintf(tempStr,"%d",speedo);
                    TFT_LCD_writeVariableString(tempStr, TFT_LCD_CENTER, 120, TFT_LCD_RED, TFT_LCD_BLACK, 12);
                    break;
                default:
                    break;
            }
            break;

            /*Put custom states above here*/
        case EXIT_EVENT:
            SW_Timer_Stop(TOUCH_TIMER);
            SW_Timer_Stop(SPEEDO_TIMER);
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
touchScreenService_State_t batteryState(Event ThisEvent) {

    touchScreenService_State_t nextState = curState;
    static uint8_t buttonArray[4];

    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            buttonArray[0] = TFT_DISPLAY_place_button("MODE", 1, 3, TFT_LCD_GREEN, 2);
            buttonArray[1] = TFT_DISPLAY_place_button("SELECT", 4, 3, TFT_LCD_GREEN, 2);
            /*Start a touch screen timer*/
            SW_Timer_Set(TOUCH_TIMER, TOUCH_TIME, touchScreenService_SERVICE, CONTINUOUS);
            break;
            /*Put custom states below here*/
        case TIMEUP_EVENT:
            switch (ThisEvent.EventParam) {
                case TOUCH_TIMER:
                    /*If screen is being touched*/
                    TFT_TOUCH_run();
                    switch (TFT_DISPLAY_button_handler()) {
                        case 0:
                            nextState = runningState_state;
                            break;
                        case 1:
                            nextState = statisticState_state;
                            break;
                        case 2:
                            nextState = batteryState_state;
                            break;
                        default:
                            break;
                    }

                    break;
                case TRANSITION_TIMER:
                    nextState = lockedState_state;
                    break;
                default:
                    break;
            }
            break;
            /*Put custom states above here*/
        case EXIT_EVENT:
            SW_Timer_Stop(TOUCH_TIMER);
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
touchScreenService_State_t statisticState(Event ThisEvent) {

    touchScreenService_State_t nextState = curState;
    static uint8_t buttonArray[4];

    switch (ThisEvent.EventType) {
        case ENTRY_EVENT:
            buttonArray[0] = TFT_DISPLAY_place_button("MODE", 1, 3, TFT_LCD_GREEN, 2);
            buttonArray[1] = TFT_DISPLAY_place_button("SELECT", 4, 3, TFT_LCD_GREEN, 2);
            /*Start a touch screen timer*/
            SW_Timer_Set(TOUCH_TIMER, TOUCH_TIME, touchScreenService_SERVICE, CONTINUOUS);
            break;
            /*Put custom states below here*/
        case TIMEUP_EVENT:
            switch (ThisEvent.EventParam) {
                case TOUCH_TIMER:
                    /*If screen is being touched*/
                    break;
                default:
                    break;
            }
            break;
            /*Put custom states above here*/
        case EXIT_EVENT:
            SW_Timer_Stop(TOUCH_TIMER);
            TFT_DISPLAY_destroy_buttons();
            TFT_LCD_drawRect(8, 8, TFT_LCD_width() - 8, TFT_LCD_height() - 8, TFT_LCD_RED);
            break;
        default:
            break;
    }
    return nextState;
}

uint8_t passwordHandler(uint16_t temp) {
    uint8_t returnVal = NO_CHAR_INPUT;
    /*Button 9 is the clear button*/
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


