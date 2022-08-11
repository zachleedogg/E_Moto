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
uint8_t lights_run = 1;
/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/

void LightsControl_Init(void) {
    lights_run = 1;
    IO_SET_TURN_SIGNAL_FL_EN(LOW);
    IO_SET_TURN_SIGNAL_FR_EN(LOW);
    IO_SET_TURN_SIGNAL_RR_EN(LOW);
    IO_SET_TURN_SIGNAL_RL_EN(LOW);

    IO_SET_HEADLIGHT_LO_EN(LOW);
    IO_SET_HEADLIGHT_HI_EN(LOW);

    IO_SET_TAILLIGHT_EN(LOW);
    IO_SET_BRAKE_LIGHT_EN(LOW);
}

void LightsControl_Run_100ms(void) {
    if(lights_run){

        /**
         * LEFT TURN SIGNAL
         */
        static uint8_t counterLeft = LEFT_TURN_SIGNAL_FREQUENCY;
        if (IO_GET_TURN_LEFT_SWITCH_IN()) {
            if (++counterLeft >= LEFT_TURN_SIGNAL_FREQUENCY) {
                IO_SET_TURN_SIGNAL_FL_EN(TOGGLE);
                IO_SET_TURN_SIGNAL_RL_EN(TOGGLE);
                counterLeft = 0;
            }
        } else {
            IO_SET_TURN_SIGNAL_FL_EN(LOW);
            IO_SET_TURN_SIGNAL_RL_EN(LOW);
            counterLeft = LEFT_TURN_SIGNAL_FREQUENCY;
        }

        /**
         * RIGHT TURN SIGNAL
         */
        static uint8_t counterRight = RIGHT_TURN_SIGNAL_FREQUENCY;
        if (IO_GET_TURN_RIGHT_SWITCH_IN()) {
            if (++counterRight >= RIGHT_TURN_SIGNAL_FREQUENCY) {
                IO_SET_TURN_SIGNAL_FR_EN(TOGGLE);
                IO_SET_TURN_SIGNAL_RR_EN(TOGGLE);
                counterRight = 0;
            }
        } else {
            IO_SET_TURN_SIGNAL_FR_EN(LOW);
            IO_SET_TURN_SIGNAL_RR_EN(LOW);
            counterRight = RIGHT_TURN_SIGNAL_FREQUENCY;
        }

        /**
         * HEADLIGHT CONTROL
         */
        //SET_HEADLIGHT_LO_EN(IO_GET_SPARE_SWITCH_1_IN());
        IO_SET_HEADLIGHT_HI_EN(IO_GET_BRIGHTS_SWITCH_IN());

        /**
         * TAILLIGHT CONTROL
         */
        IO_SET_TAILLIGHT_EN(IO_GET_BRIGHTS_SWITCH_IN() || IO_GET_SPARE_SWITCH_1_IN());
        IO_SET_BRAKE_LIGHT_EN(IO_GET_BRAKE_SWITCH_1_IN() || IO_GET_BRAKE_SWITCH_2_IN());

        /*CAN status messages for Lights. Use the status of the output, not the switch,
         to determine the status of the light*/
        CAN_mcu_status_highBeam_set(IO_GET_HEADLIGHT_HI_EN());
        CAN_mcu_status_lowBeam_set(IO_GET_HEADLIGHT_LO_EN());
        CAN_mcu_status_tailLight_set(IO_GET_TAILLIGHT_EN());
        CAN_mcu_status_brakeLight_set(IO_GET_BRAKE_LIGHT_EN());

        CAN_mcu_status_turnSignalFL_set(IO_GET_TURN_SIGNAL_FL_EN());
        CAN_mcu_status_turnSignalFR_set(IO_GET_TURN_SIGNAL_FR_EN());
        CAN_mcu_status_turnSignalRL_set(IO_GET_TURN_SIGNAL_RL_EN());
        CAN_mcu_status_turnSignalRR_set(IO_GET_TURN_SIGNAL_RR_EN());
    }
}

void LightsControl_Halt(void){
    lights_run = 0;
    IO_SET_TURN_SIGNAL_FL_EN(LOW);
    IO_SET_TURN_SIGNAL_FR_EN(LOW);
    IO_SET_TURN_SIGNAL_RR_EN(LOW);
    IO_SET_TURN_SIGNAL_RL_EN(LOW);

    IO_SET_HEADLIGHT_LO_EN(LOW);
    IO_SET_HEADLIGHT_HI_EN(LOW);

    IO_SET_TAILLIGHT_EN(LOW);
    IO_SET_BRAKE_LIGHT_EN(LOW);
}

/*** End of File **************************************************************/



