/******************************************************************************
 * Includes
 *******************************************************************************/
#include "IgnitionControl.h"
#include "IO.h"

/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/
#define KILL_SWITCH_DEBOUNCE_TIME 10
#define KILL_SWITCH_HOLD_TIME 100
/******************************************************************************
 * Configuration
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/

/******************************************************************************
 * Variable Declarations
 *******************************************************************************/
NEW_BUTTON(ignitionButton, KILL_SWITCH_DEBOUNCE_TIME, KILL_SWITCH_HOLD_TIME, IO_GET_IGNITION_SWITCH_IN);
NEW_BUTTON(killButton, KILL_SWITCH_DEBOUNCE_TIME, KILL_SWITCH_HOLD_TIME, IO_GET_KILL_SWITCH_IN);

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/

void IgnitionControl_Init(void) {
    //Assume button is in RUN position so that we don't just go to sleep right away.
    buttonSetState(killButton, BUTTON_NOT_PRESSED);
}

void IgnitionControl_Run_10ms(void) {
    buttonRun(ignitionButton);
    buttonRun(killButton);
}

ButtonStatus_E IgnitionControl_getKillStatus(void) {
    return buttonGetState(killButton);
}

ButtonStatus_E IgnitionControl_getIgnitionStatus(void) {
    return buttonGetState(ignitionButton);
}


void IgnitionControl_Halt(void) {
    Nop();
}

/*** End of File **************************************************************/




