/******************************************************************************
 * Includes
 *******************************************************************************/
#include "button.h"
#include <stdint.h>

/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/

/******************************************************************************
 * Configuration
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/

/******************************************************************************
 * Variable Declarations
 *******************************************************************************/

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void buttonRun(Button * thisButton) {
    // if pin GPIO is equal to 0, start counting, because 0 is a press (negative logic).
    switch (thisButton->state) {

            //Released and Not Pressed are really the same thing, just depends when the button was last checked.
        case BUTTON_NOT_PRESSED:
        case BUTTON_RELEASED:
            //Buttons are active low. If 0, start debouncing.
            if (thisButton->pinFunction() == 1) {
                thisButton->count++;
            } else {
                thisButton->count = 0;
            }
            //Once threshold is met, button is officially pressed.
            if (thisButton->count >= thisButton->debounceTime) {
                thisButton->state = BUTTON_PRESSED;
            }
            break;

        case BUTTON_PRESSED:
        case BUTTON_HELD:
            // If the button continues to be pressed, count towards a "hold"
            if (thisButton->pinFunction() == 1) {
                if (thisButton->count < thisButton->holdTime) {
                    thisButton->count++;
                }

            } else {
                // If there is any noise, go back to standard debounce time and start counting down.
                if (thisButton->count > thisButton->debounceTime) {
                    thisButton->count = thisButton->debounceTime;
                }
                thisButton->count--;
            }

            if (thisButton->count == thisButton->holdTime) {
                thisButton->state = BUTTON_HELD;
            }

            if (thisButton->count == 0) {
                thisButton->state = BUTTON_RELEASED;
            }
    }
}

void buttonSetState(Button * thisButton, ButtonStatus_E state){
    thisButton->state = state;
    thisButton->count = 0;
}

ButtonStatus_E buttonGetState(Button * thisButton){
    ButtonStatus_E returnState = thisButton->state;
    if (thisButton->state == BUTTON_RELEASED){
        thisButton->state = BUTTON_NOT_PRESSED;
    }
    return returnState;
}

/*** End of File **************************************************************/




