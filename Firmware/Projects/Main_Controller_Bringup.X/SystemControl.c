/******************************************************************************
 * Includes
 *******************************************************************************/
#include "SystemControl.h"
#include "IO.h"
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

void SystemControl_Init(void) {
    SET_TURN_SIGNAL_FL_EN(LOW);
    SET_TURN_SIGNAL_FR_EN(LOW);
    SET_TURN_SIGNAL_RR_EN(LOW);
    SET_TURN_SIGNAL_RL_EN(LOW);

    SET_HEADLIGHT_LO_EN(LOW);
    SET_HEADLIGHT_HI_EN(LOW);

    SET_TAILLIGHT_EN(LOW);
    SET_BRAKE_LIGHT_EN(LOW);

    SET_HEATED_GRIPS_EN(LOW);
    SET_HEATED_SEAT_EN(LOW);

}

void SystemControl_Run(void) {

    static uint8_t counter500msLeft = 5;
    if (GET_TURN_LEFT_SWITCH_IN()) {
        if (++counter500msLeft > 5) {
            SET_TURN_SIGNAL_FL_EN(TOGGLE);
            SET_TURN_SIGNAL_RL_EN(TOGGLE);
            counter500msLeft = 0;
        } else {
            SET_TURN_SIGNAL_FL_EN(LOW);
            SET_TURN_SIGNAL_RL_EN(LOW);
            counter500msLeft = 5;
        }
    }
    
    static uint8_t counter500msRight = 5;
    if (GET_TURN_RIGHT_SWITCH_IN()) {
        if (++counter500msRight > 5) {
            SET_TURN_SIGNAL_FR_EN(TOGGLE);
            SET_TURN_SIGNAL_RR_EN(TOGGLE);
            counter500msRight = 0;
        } else {
            SET_TURN_SIGNAL_FR_EN(LOW);
            SET_TURN_SIGNAL_RR_EN(LOW);
            counter500msRight = 5;
        }
    }
    SET_TURN_SIGNAL_FR_EN(GET_TURN_RIGHT_SWITCH_IN());
    SET_TURN_SIGNAL_RR_EN(GET_TURN_RIGHT_SWITCH_IN());

    SET_HEADLIGHT_LO_EN(GET_SPARE_SWITCH_1_IN());
    SET_HEADLIGHT_HI_EN(GET_HORN_SWITCH_IN());

    //SET_TAILLIGHT_EN(GET_SPARE_SWITCH_1_IN() || GET_BRIGHTS_SWITCH_IN());
    //SET_BRAKE_LIGHT_EN(GET_BRAKE_SWITCH_1_IN() || GET_BRAKE_SWITCH_2_IN());

    SET_TAILLIGHT_EN(GET_KILL_SWITCH_IN());
    SET_BRAKE_LIGHT_EN(GET_SPARE_SWITCH_1_IN());


    SET_HEATED_GRIPS_EN(GET_SPARE_SWITCH_2_IN());
    SET_HEATED_SEAT_EN(GET_SPARE_SWITCH_2_IN());
}

/*** End of File **************************************************************/



