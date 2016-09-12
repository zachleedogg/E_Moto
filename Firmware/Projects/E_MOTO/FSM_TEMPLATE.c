#include <stdio.h>

#include "FSM_TEMPLATE.h"
#include "Defines.h"
#include "configure.h"
#include "framework.h"

#define PASSCODE_SIZE 10
/*******************************************************************************
 * Debugging
 * ****************************************************************************/
static uint8_t debugEnable = 0;
#define FSM_TEMPLATE_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}

/*******************************************************************************
 * STATE MACHINE VARIABLES
 * ****************************************************************************/
static FSM_TEMPLATE_State_t prevState = init; /* initialize previous state */
static FSM_TEMPLATE_State_t curState = init; /* initialize current state */

/*******************************************************************************
 * USER SPACE
 * ****************************************************************************/
/*Constant messages for the lcd screen*/
static const char helloString[] = "WELCOME TO THE E-MOTO";
static const char intruction[] = "Touch anywhere to wake up the bike";
static const char passwordMessage[] = "Enter Secret Passcode";
static const char unlocked[] = "Congrats, you unlocked the bike!";

/*Password Stuff*/

static uint8_t code[PASSCODE_SIZE + 1] = {};
static uint8_t codeIndex = 0;
static const uint8_t passcode[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1};

#define NO_INPUT 0xFF
#define INPUT 0
#define FAILED 1
#define PASSED 2

uint8_t passwordHandler(uint16_t temp);

/*******************************************************************************
 * STATE MACHINE BEGINS HERE
 * ****************************************************************************/
Event FSM_TEMPLATE(Event ThisEvent) {

    /*Debugging print statement*/
    FSM_TEMPLATE_print("Service: %s\tState: %s\tEvent: %s %d\n", ServiceStrings[FSM_TEMPLATE_SERVICE], StateStrings[curState], EventStrings[ThisEvent.EventType], ThisEvent.EventParam);

    /*Initialize next state so we can check for a transistion*/
    FSM_TEMPLATE_State_t nextState = curState;

    /*State Machine Begins Here.*/
    switch (curState) {
        case init: /* SM starts here */
            if (ThisEvent.EventType == INIT_EVENT) {
                /*Initialization stuff here*/

                /*LCD Init*/
                TFT_LCD_INIT(IO_PIN_RB12, IO_PIN_RB10, IO_PIN_RB11);
                TFT_TOUCH_INIT(IO_PIN_RA0, IO_PIN_RB2, IO_PIN_RA1, IO_PIN_RB3, AN5, AN4);

                /*fill background*/
                TFT_LCD_fillBackground(TFT_LCD_RED);
                TFT_LCD_drawRect(4, 4, TFT_LCD_width() - 4, TFT_LCD_height() - 4, TFT_LCD_BLACK);
                TFT_LCD_drawRect(8, 8, TFT_LCD_width() - 8, TFT_LCD_height() - 8, TFT_LCD_RED);

                /*write a greeting*/
                TFT_LCD_writeString(helloString, TFT_LCD_CENTER, 100, TFT_LCD_MAGENTA, TFT_LCD_CYAN, 3);
                TFT_LCD_writeString(intruction, TFT_LCD_CENTER, 120, TFT_LCD_RED, TFT_LCD_CYAN, 2);

                nextState = someState;
            }
            break;

            /*******************************************************************
             * someState
             ******************************************************************/
        case someState: /* initial idle state before ignition */
            switch (ThisEvent.EventType) {
                case ENTRY_EVENT:
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
                            nextState = anotherState;
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
            break;

            /*******************************************************************
             * anotherState
             ******************************************************************/
        case anotherState: /* Welcomes the user after the ignition wake up*/
            if (ThisEvent.EventType != NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ENTRY_EVENT:
                        /*Create a keypad*/
                        TFT_DISPLAY_place_button("1", 2, 1, TFT_LCD_GREEN, 4);
                        TFT_DISPLAY_place_button("2", 3, 1, TFT_LCD_GREEN, 4);
                        TFT_DISPLAY_place_button("3", 4, 1, TFT_LCD_GREEN, 4);
                        TFT_DISPLAY_place_button("4", 2, 2, TFT_LCD_GREEN, 4);
                        TFT_DISPLAY_place_button("5", 3, 2, TFT_LCD_GREEN, 4);
                        TFT_DISPLAY_place_button("6", 4, 2, TFT_LCD_GREEN, 4);
                        TFT_DISPLAY_place_button("7", 2, 3, TFT_LCD_GREEN, 4);
                        TFT_DISPLAY_place_button("8", 3, 3, TFT_LCD_GREEN, 4);
                        TFT_DISPLAY_place_button("9", 4, 3, TFT_LCD_GREEN, 4);
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
                                    case INPUT:
                                        TFT_LCD_writeVariableString((char*)code, TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                                        break;
                                    case FAILED:
                                        TFT_LCD_writeVariableString((char*)code, TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                                        TFT_LCD_writeString("          ", TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                                        break;
                                    case PASSED:
                                        TFT_LCD_writeVariableString((char*)code, TFT_LCD_CENTER, 250, TFT_LCD_RED, TFT_LCD_CYAN, 3);
                                        nextState = yetAnotherState;
                                        break;
                                    case NO_INPUT:
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
            }
            break;
            /*******************************************************************
             * yetAnotherState
             ******************************************************************/
        case yetAnotherState: /* Welcomes the user after the ignition wake up*/
            if (ThisEvent.EventType != NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ENTRY_EVENT:
                        TFT_LCD_writeString(unlocked, TFT_LCD_CENTER, 100, TFT_LCD_MAGENTA, TFT_LCD_CYAN, 2);
                        break;
                        /*Put custom states below here*/

                        /*Put custom states above here*/
                    case EXIT_EVENT:
                        break;
                    default:
                        break;
                }
            }
            break;
    }

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

uint8_t passwordHandler(uint16_t temp) {
    uint8_t returnVal = NO_INPUT;
    if (temp != NO_INPUT) {
        returnVal = INPUT;
        code[codeIndex++] = temp + 49;
        code[codeIndex] = 0;
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