#include <stdio.h>

#include "FSM_TEMPLATE.h"
#include "Defines.h"
#include "configure.h"
#include "framework.h"

/* Uncomment to for custom Debugging */
static uint8_t debugEnable = 1;
#define FSM_TEMPLATE_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}

/* State variables*/
static FSM_TEMPLATE_State_t prevState = init; /* initialize previous state */
static FSM_TEMPLATE_State_t curState = init; /* initialize current state */

/*******************************************************************************
 * USER VARIABLES
 * ****************************************************************************/
static const char helloString[] = "WELCOME TO THE E-MOTO";
static const char intruction[] = "SELECT A BUTTON BELOW";

static char myScreenBuffer[100];

static touchData myScreenPosition;

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
                TFT_LCD_INIT(IO_PIN_RB12, IO_PIN_RB10, IO_PIN_RB11);
                TFT_TOUCH_INIT(IO_PIN_RA0, IO_PIN_RB2, IO_PIN_RA1, IO_PIN_RB3, AN5, AN4);

                TFT_LCD_drawRect(25, 25, TFT_LCD_width() - 25, TFT_LCD_height() - 25, TFT_LCD_RED);

                TFT_LCD_drawRect(10, 10, TFT_LCD_width() / 4 - 10, TFT_LCD_height() / 4 - 10, TFT_LCD_GREEN);
                TFT_LCD_drawRect((TFT_LCD_width() / 4) + 10, 10, TFT_LCD_width() / 2 - 10, TFT_LCD_height() / 4 - 10, TFT_LCD_GREEN);
                TFT_LCD_drawRect(TFT_LCD_width() / 2 + 10, 10, (TFT_LCD_width()*3) / 4 - 10, TFT_LCD_height() / 4 - 10, TFT_LCD_GREEN);
                TFT_LCD_drawRect((TFT_LCD_width()*3) / 4 + 10, 10, TFT_LCD_width() - 10, TFT_LCD_height() / 4 - 10, TFT_LCD_GREEN);

                TFT_LCD_writeString(helloString, (TFT_LCD_width() / 2)-(strlen(helloString)*12 / 2), 100, TFT_LCD_RED, TFT_LCD_CYAN, 2);
                TFT_LCD_writeString(intruction, (TFT_LCD_width() / 2)-(strlen(intruction)*12 / 2), 120, TFT_LCD_RED, TFT_LCD_CYAN, 2);
                sprintf(myScreenBuffer, "number is: %d", 27);
                TFT_LCD_writeVariableString(myScreenBuffer, 100, 175, TFT_LCD_RED, TFT_LCD_CYAN, 2);

                nextState = someState;
            }
            break;

        case someState: /* initial idle state before ignition */
            switch (ThisEvent.EventType) {
                case ENTRY_EVENT:
                    SW_Timer_Set(0,5, FSM_TEMPLATE_SERVICE);
                    break;
                    /*Put custom states below here*/
                case TIMEUP_EVENT:
                    SW_Timer_Set(0,5, FSM_TEMPLATE_SERVICE);
                    myScreenPosition = TFT_TOUCH_run();
                    if (myScreenPosition.status == TOUCHING) {
                        TFT_LCD_drawRect(myScreenPosition.yPos, myScreenPosition.xPos,
                                myScreenPosition.yPos + 3, myScreenPosition.xPos + 3, TFT_LCD_GREEN);
                        sprintf(myScreenBuffer, "Xraw: %4u, Yraw: %4u", myScreenPosition.yPos, myScreenPosition.xPos);
                        TFT_LCD_writeVariableString(myScreenBuffer, 100, 175, TFT_LCD_RED, TFT_LCD_CYAN, 2);
                    }

                    break;
                    /*Put custom states above here*/
                case EXIT_EVENT:
                    break;
                default:
                    break;
            }
            break;

        case anotherState: /* Welcomes the user after the ignition wake up*/
            if (ThisEvent.EventType != NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ENTRY_EVENT:
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

        case yetAnotherState: /* Welcomes the user after the ignition wake up*/
            if (ThisEvent.EventType != NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ENTRY_EVENT:
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
