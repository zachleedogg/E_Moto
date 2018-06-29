/******************************************************************************
 * Includes
 *******************************************************************************/
#include "IO.h"
#include "pinSetup.h"
#include "bolt_ADC.h"


/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/
#define MAX_ADC_BITS 1023

#define VBAT_VOLTAGE_CONVERSION 8.234
#define PILOT_VOLTAGE_CONVERSION 6.6
#define PROXIMITY_VOLTAGE_CONVERSION 2.0
/******************************************************************************
 * Configuration
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/

/******************************************************************************
 * Variable Declarations
 *******************************************************************************/

/*Fault Bits*/
static uint8_t FAN_fault = 0;
static uint8_t PUMP_fault = 0;
static uint8_t TAILLIGHT_fault = 0;
static uint8_t BRAKELIGHT_fault = 0;
static uint8_t LOWBEAM_fault = 0;
static uint8_t HIGHBEAM_fault = 0;
static uint8_t HORN_fault = 0;
static uint8_t AUX_PORT_fault = 0;
static uint8_t HEATED_GRIPS_fault = 0;
static uint8_t HEATED_SEAT_fault = 0;
static uint8_t CHARGE_CONTROLLER_fault = 0;
static uint8_t MOTOR_CONTROLLER_fault = 0;
static uint8_t BMS_CONTROLLER_fault = 0;
static uint8_t SPARE_1_CONTROLLER_fault = 0;
static uint8_t BATT_fault = 0;
static uint8_t DCDC_fault = 0;
static uint8_t IC_CONTROLLER_fault = 0;

/*Fault Bits*/
static uint16_t FAN_current = 0;
static uint16_t PUMP_current = 0;
static uint16_t TAILLIGHT_current = 0;
static uint16_t BRAKELIGHT_current = 0;
static uint16_t LOWBEAM_current = 0;
static uint16_t HIGHBEAM_current = 0;
static uint16_t HORN_current = 0;
static uint16_t AUX_PORT_current = 0;
static uint16_t HEATED_GRIPS_current = 0;
static uint16_t HEATED_SEAT_current = 0;
static uint16_t CHARGE_CONTROLLER_current = 0;
static uint16_t MOTOR_CONTROLLER_current = 0;
static uint16_t BMS_CONTROLLER_current = 0;
static uint16_t SPARE_1_CONTROLLER_current = 0;
static uint16_t BATT_current = 0;
static uint16_t DCDC_current = 0;
static uint16_t IC_CONTROLLER_current = 0;


/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void IO_Efuse_Init(void) {
    SET_SW_EN(HIGH);
    SET_DIAG_SELECT_EN(LOW);
    SET_DIAG_EN(HIGH);
}

void IO_Efuse_Run_10ms(void) {
    /*Check value of Diag pin to select which HSDs are being read out*/
    uint8_t diagSelect = GET_DIAG_SELECT_EN();

    if (diagSelect == 0) {

        PUMP_current = GET_CURRENT_PUMP();
        if (PUMP_current >= 1000) {
            PUMP_fault = 1;
            SET_PUMP_1_EN(LOW);
        }

        BRAKELIGHT_current = GET_CURRENT_BRAKELIGHT();
        if (BRAKELIGHT_current >= 1000) {
            BRAKELIGHT_fault = 1;
            SET_BRAKE_LIGHT_EN(LOW);
        }

        HIGHBEAM_current = GET_CURRENT_HIGHBEAM();
        if (HIGHBEAM_current >= MAX_ADC_BITS) {
            HIGHBEAM_fault = 1;
            SET_HEADLIGHT_HI_EN(LOW);
        }

        HEATED_SEAT_current = GET_CURRENT_HEATED_SEAT();
        if (HEATED_SEAT_current >= 1000) {
            HEATED_SEAT_fault = 1;
            SET_HEATED_SEAT_EN(LOW);
        }

        CHARGE_CONTROLLER_current = GET_CURRENT_CHARGE_CONTROLLER();
        if (CHARGE_CONTROLLER_current >= 1000) {
            CHARGE_CONTROLLER_fault = 1;
            SET_CHARGE_CONTROLLER_EN(LOW);
        }

        SPARE_1_CONTROLLER_current = GET_CURRENT_SPARE_1_CONTROLLER();
        if (SPARE_1_CONTROLLER_current >= 1000) {
            SPARE_1_CONTROLLER_fault = 1;
            SET_J1772_CONTROLLER_EN(LOW);
        }

    } else {
        FAN_current = GET_CURRENT_FAN();
        if (FAN_current >= 1000) {
            FAN_fault = 1;
            SET_FAN_1_EN(LOW);
        }

        TAILLIGHT_current = GET_CURRENT_TAILLIGHT();
        if (TAILLIGHT_current >= 1000) {
            TAILLIGHT_fault = 1;
            SET_TAILLIGHT_EN(LOW);
        }

        LOWBEAM_current = GET_CURRENT_LOWBEAM();
        if (LOWBEAM_current >= MAX_ADC_BITS) {
            LOWBEAM_fault = 1;
            SET_HEADLIGHT_LO_EN(LOW);
        }

        HEATED_GRIPS_current = GET_CURRENT_HEATED_GRIPS();
        if (HEATED_GRIPS_current >= 1000) {
            HEATED_GRIPS_fault = 1;
            SET_HEATED_GRIPS_EN(LOW);
        }

        MOTOR_CONTROLLER_current = GET_CURRENT_MOTOR_CONTROLLER();
        if (MOTOR_CONTROLLER_current >= 1000) {
            MOTOR_CONTROLLER_fault = 1;
            SET_MOTOR_CONTROLLER_EN(LOW);
        }

        BMS_CONTROLLER_current = GET_CURRENT_BMS_CONTROLLER();
        if (BMS_CONTROLLER_current >= 1000) {
            BMS_CONTROLLER_fault = 1;
            SET_BMS_CONTROLLER_EN(LOW);
        }
    }

    HORN_current = GET_CURRENT_HORN();
    if (HORN_current >= 1000) {
        HORN_fault = 1;
        SET_HORN_EN(LOW);
    }

    AUX_PORT_current = GET_CURRENT_AUX_PORT();
    if (AUX_PORT_current >= 1000) {
        AUX_PORT_fault = 1;
        SET_AUX_PORT_EN(LOW);
    }

    BATT_fault = GET_BATT_FAULT();

    DCDC_fault = GET_DCDC_FAULT();

    IC_CONTROLLER_fault = GET_IC_CONTROLLER_FAULT();

    /*Toggle Diag Select for next cycle*/
    SET_DIAG_SELECT_EN(TOGGLE);

}

void IO_Efuse_Halt(void) {
    SET_DIAG_SELECT_EN(LOW);
    SET_DIAG_EN(LOW);
    SET_SW_EN(LOW);
}

void SET_DEBUG_LED_EN(uint8_t state) {
    PINS_write(DEBUG_LED_EN, state);
}

void SET_DEBUG_PIN_EN(uint8_t state) {
    PINS_write(DEBUG_PIN_EN, state);
}

void SET_BMS_CONTROLLER_EN(uint8_t state) {
    if (BMS_CONTROLLER_fault == 0) {
        PINS_write(BMS_CONTROLLER_EN, state);
    }
}

void SET_J1772_CONTROLLER_EN(uint8_t state) {
    PINS_write(J1772_CONTROLLER_EN, state);
}

void SET_MOTOR_CONTROLLER_EN(uint8_t state) {
    if (MOTOR_CONTROLLER_fault == 0) {
        PINS_write(MOTOR_CONTROLLER_EN, state);
    }
}

void SET_CHARGE_CONTROLLER_EN(uint8_t state) {
    if (CHARGE_CONTROLLER_fault == 0) {
        PINS_write(CHARGE_CONTROLLER_EN, state);
    }
}

void SET_PILOT_EN(uint8_t state) {
    PINS_write(PILOT_EN, state);
}

void SET_BRAKE_LIGHT_EN(uint8_t state) {
    if (BRAKELIGHT_fault == 0) {
        PINS_write(BRAKE_LIGHT_EN, state);
    }
}

void SET_TAILLIGHT_EN(uint8_t state) {
    if (TAILLIGHT_fault == 0) {
        PINS_write(TAILLIGHT_EN, state);
    }
}

void SET_HEADLIGHT_HI_EN(uint8_t state) {
    if (HIGHBEAM_fault == 0) {
        PINS_write(HEADLIGHT_HI_EN, state);
    }
}

void SET_HEADLIGHT_LO_EN(uint8_t state) {
    if (LOWBEAM_fault == 0) {
        PINS_write(HEADLIGHT_LO_EN, state);
    }
}

void SET_CHARGER_CONTACTOR_EN(uint8_t state) {
    PINS_write(CHARGER_CONTACTOR_EN, state);
}

void SET_DCDC_CONTACTOR_EN(uint8_t state) {
    PINS_write(DCDC_CONTACTOR_EN, state);
}

void SET_BATTERY_CONTACTOR_EN(uint8_t state) {
    PINS_write(BATTERY_CONTACTOR_EN, state);
}

void SET_PRECHARGE_RELAY_EN(uint8_t state) {
    PINS_write(PRECHARGE_RELAY_EN, state);
}

void SET_HORN_EN(uint8_t state) {
    if (HORN_fault == 0) {
        PINS_write(HORN_EN, state);
    }
}

void SET_AUX_PORT_EN(uint8_t state) {
    if (AUX_PORT_fault == 0) {
        PINS_write(AUX_PORT_EN, state);
    }
}

void SET_TURN_SIGNAL_FR_EN(uint8_t state) {
    PINS_write(TURN_SIGNAL_FR_EN, state);
}

void SET_TURN_SIGNAL_RR_EN(uint8_t state) {
    PINS_write(TURN_SIGNAL_RR_EN, state);
}

void SET_TURN_SIGNAL_FL_EN(uint8_t state) {
    PINS_write(TURN_SIGNAL_FL_EN, state);
}

void SET_TURN_SIGNAL_RL_EN(uint8_t state) {
    PINS_write(TURN_SIGNAL_RL_EN, state);
}

void SET_HEATED_GRIPS_EN(uint8_t state) {
    if (HEATED_GRIPS_fault == 0) {
        PINS_write(HEATED_GRIPS_EN, state);
    }
}

void SET_HEATED_SEAT_EN(uint8_t state) {
    if (HEATED_SEAT_fault == 0) {
        PINS_write(HEATED_SEAT_EN, state);
    }
}

void SET_SW_EN(uint8_t state) {
    PINS_write(SW_EN, state);
}

void SET_DIAG_EN(uint8_t state) {
    PINS_write(DIAG_EN, state);
}

void SET_DIAG_SELECT_EN(uint8_t state) {
    PINS_write(DIAG_SELECT_EN, state);
}

void SET_DCDC_EN(uint8_t state) {
    if (DCDC_fault == 0) {
        PINS_write(DCDC_EN, state);
    }
}

void SET_BATT_EN(uint8_t state) {
    if (BATT_fault == 0) {
        PINS_write(BATT_EN, state);
    }
}

void SET_CAN_SLEEP_EN(uint8_t state) {
    PINS_write(CAN_SLEEP_EN, state);
}

void SET_IC_CONTROLLER_SLEEP_EN(uint8_t state) {
    PINS_write(IC_CONTROLLER_SLEEP_EN, state);
}

void SET_PUMP_1_EN(uint8_t state) {
    if (PUMP_fault == 0) {
        PINS_write(PUMP_1_EN, state);
    }
}

void SET_FAN_1_EN(uint8_t state) {
    if (FAN_fault == 0) {
        PINS_write(FAN_1_EN, state);
    }
}

/*INPUTS*/

uint8_t GET_DEBUG_LED_EN(void) {
    return PINS_read(DEBUG_LED_EN);
}

uint8_t GET_DEBUG_PIN_EN(void) {
    return PINS_read(DEBUG_PIN_EN);
}

uint8_t GET_BMS_CONTROLLER_EN(void) {
    return PINS_read(BMS_CONTROLLER_EN);
}

uint8_t GET_J1772_CONTROLLER_EN(void) {
    return PINS_read(J1772_CONTROLLER_EN);
}

uint8_t GET_MOTOR_CONTROLLER_EN(void) {
    return PINS_read(MOTOR_CONTROLLER_EN);
}

uint8_t GET_CHARGE_CONTROLLER_EN(void) {
    return PINS_read(CHARGE_CONTROLLER_EN);
}

uint8_t GET_PILOT_EN(void) {
    return PINS_read(PILOT_EN);
}

uint8_t GET_BRAKE_LIGHT_EN(void) {
    return PINS_read(BRAKE_LIGHT_EN);
}

uint8_t GET_TAILLIGHT_EN(void) {
    return PINS_read(TAILLIGHT_EN);
}

uint8_t GET_HEADLIGHT_HI_EN(void) {
    return PINS_read(HEADLIGHT_HI_EN);
}

uint8_t GET_HEADLIGHT_LO_EN(void) {
    return PINS_read(HEADLIGHT_LO_EN);
}

uint8_t GET_CHARGER_CONTACTOR_EN(void) {
    return PINS_read(CHARGER_CONTACTOR_EN);
}

uint8_t GET_DCDC_CONTACTOR_EN(void) {
    return PINS_read(DCDC_CONTACTOR_EN);
}

uint8_t GET_BATTERY_CONTACTOR_EN(void) {
    return PINS_read(BATTERY_CONTACTOR_EN);
}

uint8_t GET_PRECHARGE_RELAY_EN(void) {
    return PINS_read(PRECHARGE_RELAY_EN);
}

uint8_t GET_HORN_EN(void) {
    return PINS_read(HORN_EN);
}

uint8_t GET_AUX_PORT_EN(void) {
    return PINS_read(AUX_PORT_EN);
}

uint8_t GET_TURN_SIGNAL_FR_EN(void) {
    return PINS_read(TURN_SIGNAL_FR_EN);
}

uint8_t GET_TURN_SIGNAL_RR_EN(void) {
    return PINS_read(TURN_SIGNAL_RR_EN);
}

uint8_t GET_TURN_SIGNAL_FL_EN(void) {
    return PINS_read(TURN_SIGNAL_FL_EN);
}

uint8_t GET_TURN_SIGNAL_RL_EN(void) {
    return PINS_read(TURN_SIGNAL_RL_EN);
}

uint8_t GET_HEATED_GRIPS_EN(void) {
    return PINS_read(HEATED_GRIPS_EN);
}

uint8_t GET_HEATED_SEAT_EN(void) {
    return PINS_read(HEATED_SEAT_EN);
}

uint8_t GET_SW_EN(void) {
    return PINS_read(SW_EN);
}

uint8_t GET_DIAG_EN(void) {
    return PINS_read(DIAG_EN);
}

uint8_t GET_DIAG_SELECT_EN(void) {
    return PINS_read(DIAG_SELECT_EN);
}

uint8_t GET_DCDC_EN(void) {
    return PINS_read(DCDC_EN);
}

uint8_t GET_BATT_EN(void) {
    return PINS_read(BATT_EN);
}

uint8_t GET_CAN_SLEEP_EN(void) {
    return PINS_read(CAN_SLEEP_EN);
}

uint8_t GET_IC_CONTROLLER_SLEEP_EN(void) {
    return PINS_read(IC_CONTROLLER_SLEEP_EN);
}

uint8_t GET_PUMP_1_EN(void) {
    return PINS_read(PUMP_1_EN);
}

uint8_t GET_FAN_1_EN(void) {
    return PINS_read(FAN_1_EN);
}

uint8_t GET_IC_CONTROLLER_FAULT(void) {
    return PINS_read(IC_CONTROLLER_nFAULT) ? 0 : 1;
}

uint8_t GET_DCDC_FAULT(void) {
    return PINS_read(DCDC_nFAULT) ? 0 : 1;
}

uint8_t GET_BATT_FAULT(void) {
    return PINS_read(BATT_nFAULT) ? 0 : 1;
}

uint8_t GET_TURN_SIGNAL_L_STATUS(void) {
    return PINS_read(TURN_SIGNAL_L_STATUS);
}

uint8_t GET_TURN_SIGNAL_R_STATUS(void) {
    return PINS_read(TURN_SIGNAL_R_STATUS);
}

uint8_t GET_CONTACT_1_2_STATUS(void) {
    return PINS_read(CONTACT_1_2_STATUS);
}

uint8_t GET_CONTACT_3_4_STATUS(void) {
    return PINS_read(CONTACT_3_4_STATUS);
}

uint8_t GET_SPARE_SWITCH_1_IN(void) {
    return PINS_read(SPARE_SWITCH_1_IN) ? 0 : 1;
}

uint8_t GET_SPARE_SWITCH_2_IN(void) {
    return PINS_read(SPARE_SWITCH_2_IN) ? 0 : 1;
}

uint8_t GET_BRAKE_SWITCH_1_IN(void) {
    return PINS_read(BRAKE_SWITCH_1_IN) ? 0 : 1;
}

uint8_t GET_BRAKE_SWITCH_2_IN(void) {
    return PINS_read(BRAKE_SWITCH_2_IN) ? 0 : 1;
}

uint8_t GET_IGNITION_SWITCH_IN(void) {
    return PINS_read(IGNITION_SWITCH_IN) ? 0 : 1;
}

uint8_t GET_KILL_SWITCH_IN(void) {
    return PINS_read(KILL_SWITCH_IN) ? 0 : 1;
}

uint8_t GET_TURN_LEFT_SWITCH_IN(void) {
    return PINS_read(TURN_LEFT_SWITCH_IN) ? 0 : 1;
}

uint8_t GET_TURN_RIGHT_SWITCH_IN(void) {
    return PINS_read(TURN_RIGHT_SWITCH_IN) ? 0 : 1;
}

uint8_t GET_BRIGHTS_SWITCH_IN(void) {
    return PINS_read(BRIGHTS_SWITCH_IN) ? 0 : 1;
}

uint8_t GET_HORN_SWITCH_IN(void) {
    return PINS_read(HORN_SWITCH_IN) ? 0 : 1;
}

uint8_t GET_KICKSTAND_SWITCH_IN(void) {
    return PINS_read(KICKSTAND_SWITCH_IN) ? 0 : 1;
}

/*ANALOG*/

uint16_t GET_CURRENT_FAN() {
    return FAN_current;
}

uint16_t GET_CURRENT_PUMP() {
    return PUMP_current;
}

uint16_t GET_CURRENT_TAILLIGHT() {
    return TAILLIGHT_current;
}

uint16_t GET_CURRENT_BRAKELIGHT() {
    return BRAKELIGHT_current;
}

uint16_t GET_CURRENT_LOWBEAM() {
    return LOWBEAM_current;
}

uint16_t GET_CURRENT_HIGHBEAM() {
    return HIGHBEAM_current;
}

uint16_t GET_CURRENT_HORN() {
    return HORN_current;
}

uint16_t GET_CURRENT_AUX_PORT() {
    return AUX_PORT_current;
}

uint16_t GET_CURRENT_HEATED_GRIPS() {
    return HEATED_GRIPS_current;
}

uint16_t GET_CURRENT_HEATED_SEAT() {
    return HEATED_SEAT_current;
}

uint16_t GET_CURRENT_CHARGE_CONTROLLER() {
    return CHARGE_CONTROLLER_current;
}

uint16_t GET_CURRENT_MOTOR_CONTROLLER() {
    return MOTOR_CONTROLLER_current;
}

uint16_t GET_CURRENT_BMS_CONTROLLER() {
    return BMS_CONTROLLER_current;
}

uint16_t GET_CURRENT_SPARE_1_CONTROLLER() {
    return SPARE_1_CONTROLLER_current;
}

uint16_t GET_CURRENT_BATT() {
    return BATT_current;
}

uint16_t GET_CURRENT_DCDC() {
    return DCDC_current;
}

uint16_t GET_CURRENT_IC_CONTROLLER() {
    return IC_CONTROLLER_current;
}

uint16_t GET_VOLTAGE_PILOT(void) {
    return (uint16_t)((((float)ADC_GetValue(PILOT_MONITOR_AI))*3300.0*PILOT_VOLTAGE_CONVERSION)/MAX_ADC_BITS);
}

uint16_t GET_VOLTAGE_PROXIMITY(void) {
    return (uint16_t)((((float)ADC_GetValue(PROXIMITY_MONITOR_AI))*3300.0*PROXIMITY_VOLTAGE_CONVERSION)/MAX_ADC_BITS);
}

uint16_t GET_VOLTAGE_VBAT(void) {
    return (uint16_t)((((float)ADC_GetValue(V12_MONITOR_AI))*3300.0*VBAT_VOLTAGE_CONVERSION)/MAX_ADC_BITS);
}

uint16_t GET_VOLTAGE_VBAT_SW(void) {
    return (uint16_t)((((float)ADC_GetValue(P12_MONITOR_AI))*3300.0*VBAT_VOLTAGE_CONVERSION)/MAX_ADC_BITS);
}


/*FAULT BITS*/

uint8_t IO_GET_CURRENT_FAN_FAULT(void) {
    return FAN_fault;
}

uint8_t IO_GET_CURRENT_PUMP_FAULT(void) {
    return PUMP_fault;
}

uint8_t IO_GET_CURRENT_TAILLIGHT_FAULT(void) {
    return TAILLIGHT_fault;
}

uint8_t IO_GET_CURRENT_BRAKELIGHT_FAULT(void) {
    return BRAKELIGHT_fault;
}

uint8_t IO_GET_CURRENT_LOWBEAM_FAULT(void) {
    return LOWBEAM_fault;
}

uint8_t IO_GET_CURRENT_HIGHBEAM_FAULT(void) {
    return HIGHBEAM_fault;
}

uint8_t IO_GET_CURRENT_HORN_FAULT(void) {
    return HORN_fault;
}

uint8_t IO_GET_CURRENT_AUX_PORT_FAULT(void) {
    return AUX_PORT_fault;
}

uint8_t IO_GET_CURRENT_HEATED_GRIPS_FAULT(void) {
    return HEATED_GRIPS_fault;
}

uint8_t IO_GET_CURRENT_HEATED_SEAT_FAULT(void) {
    return HEATED_SEAT_fault;
}

uint8_t IO_GET_CURRENT_CHARGE_CONTROLLER_FAULT(void) {
    return CHARGE_CONTROLLER_fault;
}

uint8_t IO_GET_CURRENT_MOTOR_CONTROLLER_FAULT(void) {
    return MOTOR_CONTROLLER_fault;
}

uint8_t IO_GET_CURRENT_BMS_CONTROLLER_FAULT(void) {
    return BMS_CONTROLLER_fault;
}

uint8_t IO_GET_CURRENT_SPARE_1_CONTROLLER_FAULT(void) {
    return SPARE_1_CONTROLLER_fault;
}




/*** End of File **************************************************************/


