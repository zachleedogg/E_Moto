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
#define LEFT_TURN_SIGNAL_FREQUENCY 5 // x100ms
#define RIGHT_TURN_SIGNAL_FREQUENCY 5 // x100ms
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

    static uint8_t counter500msLeft = LEFT_TURN_SIGNAL_FREQUENCY;
    if (GET_TURN_LEFT_SWITCH_IN()) {
        if (++counter500msLeft >= LEFT_TURN_SIGNAL_FREQUENCY) {
            SET_TURN_SIGNAL_FL_EN(TOGGLE);
            SET_TURN_SIGNAL_RL_EN(TOGGLE);
            counter500msLeft = 0;
        }
    } else {
        SET_TURN_SIGNAL_FL_EN(LOW);
        SET_TURN_SIGNAL_RL_EN(LOW);
        counter500msLeft = LEFT_TURN_SIGNAL_FREQUENCY;
    }

    static uint8_t counter500msRight = RIGHT_TURN_SIGNAL_FREQUENCY;
    if (GET_TURN_RIGHT_SWITCH_IN()) {
        if (++counter500msRight >= RIGHT_TURN_SIGNAL_FREQUENCY) {
            SET_TURN_SIGNAL_FR_EN(TOGGLE);
            SET_TURN_SIGNAL_RR_EN(TOGGLE);
            counter500msRight = 0;
        }
    } else {
        SET_TURN_SIGNAL_FR_EN(LOW);
        SET_TURN_SIGNAL_RR_EN(LOW);
        counter500msRight = RIGHT_TURN_SIGNAL_FREQUENCY;
    }

    SET_HEADLIGHT_LO_EN(GET_SPARE_SWITCH_1_IN());
    SET_HEADLIGHT_HI_EN(GET_HORN_SWITCH_IN());

    SET_TAILLIGHT_EN(GET_KILL_SWITCH_IN());
    SET_BRAKE_LIGHT_EN(GET_HORN_SWITCH_IN());


    SET_HEATED_GRIPS_EN(GET_SPARE_SWITCH_2_IN());
    SET_HEATED_SEAT_EN(GET_SPARE_SWITCH_2_IN());
}

/*** End of File **************************************************************/



