/******************************************************************************
 * Includes
 *******************************************************************************/
#include "LightsControl.h"
#include "IO.h"
#include "MCU_dbc.h"
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

void LightsControl_Init(void) {
    SET_TURN_SIGNAL_FL_EN(LOW);
    SET_TURN_SIGNAL_FR_EN(LOW);
    SET_TURN_SIGNAL_RR_EN(LOW);
    SET_TURN_SIGNAL_RL_EN(LOW);

    SET_HEADLIGHT_LO_EN(LOW);
    SET_HEADLIGHT_HI_EN(LOW);

    SET_TAILLIGHT_EN(LOW);
    SET_BRAKE_LIGHT_EN(LOW);
}

void LightsControl_Run_100ms(void) {

    /**
     * LEFT TURN SIGNAL
     */
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
    
    /**
     * RIGHT TURN SIGNAL
     */
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

    /**
     * HEADLIGHT CONTROL
     */
    SET_HEADLIGHT_LO_EN(GET_SPARE_SWITCH_1_IN());
    SET_HEADLIGHT_HI_EN(GET_BRIGHTS_SWITCH_IN());

    /**
     * TAILLIGHT CONTROL
     */
    SET_TAILLIGHT_EN(GET_BRIGHTS_SWITCH_IN() || GET_SPARE_SWITCH_1_IN());
    SET_BRAKE_LIGHT_EN(GET_BRAKE_SWITCH_1_IN() || GET_BRAKE_SWITCH_1_IN());
    
    /*CAN status messages for Lights. Use the status of the output, not the switch,
     to determine the status of the light*/
    CAN_mcu_status_highBeam_set(GET_HEADLIGHT_HI_EN());
    CAN_mcu_status_lowBeam_set(GET_HEADLIGHT_LO_EN());
    CAN_mcu_status_tailLight_set(GET_TAILLIGHT_EN());
    CAN_mcu_status_brakeLight_set(GET_BRAKE_LIGHT_EN());
    
    CAN_mcu_status_turnSignalFL_set(GET_TURN_SIGNAL_FL_EN());
    CAN_mcu_status_turnSignalFR_set(GET_TURN_SIGNAL_FR_EN());
    CAN_mcu_status_turnSignalRL_set(GET_TURN_SIGNAL_RL_EN());
    CAN_mcu_status_turnSignalRR_set(GET_TURN_SIGNAL_RR_EN());
}

void LightsControl_Halt(void){
    SET_TURN_SIGNAL_FL_EN(LOW);
    SET_TURN_SIGNAL_FR_EN(LOW);
    SET_TURN_SIGNAL_RR_EN(LOW);
    SET_TURN_SIGNAL_RL_EN(LOW);

    SET_HEADLIGHT_LO_EN(LOW);
    SET_HEADLIGHT_HI_EN(LOW);

    SET_TAILLIGHT_EN(LOW);
    SET_BRAKE_LIGHT_EN(LOW);
}

/*** End of File **************************************************************/



