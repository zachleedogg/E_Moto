/* 
 * File:   pinSetup.c
 * Author: Zach Levenberg
 * Comments: Pin setup for projects
 * Revision history: 3/1/18 initial build
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
#include "pinSetup.h"
#include "bolt_ADC.h"
#include "bolt_uart.h"
#include "bolt_CAN.h"
#include "pins.h"
#include "bolt_OC.h"
#include "bolt_pps.h"
//#include "bolt_sleep.h"
#include <xc.h>

// *****************************************************************************
// *****************************************************************************
// Section: Module Defines and variables
// *****************************************************************************
// *****************************************************************************
/*DIGITAL INPUTS AND OUTPUTS*/

#define DEBUG_LED_EN PIN_DEF(A,0)
#define DEBUG_PIN_EN PIN_DEF(A,1)
#define BMS_CONTROLLER_EN PIN_DEF(A,2)
#define J1772_CONTROLLER_EN PIN_DEF(A,3)
#define MOTOR_CONTROLLER_EN PIN_DEF(A,4)
#define CHARGE_CONTROLLER_EN PIN_DEF(A,5)
#define PILOT_EN PIN_DEF(A,6)
#define TURN_SIGNAL_L_STATUS PIN_DEF(A,9)
#define TURN_SIGNAL_R_STATUS PIN_DEF(A,10)
#define CONTACT_1_2_STATUS PIN_DEF(A,14)
#define CONTACT_3_4_STATUS PIN_DEF(A,15)

#define SPARE_SWITCH_1_IN PIN_DEF(C,2)
#define SPARE_SWITCH_2_IN PIN_DEF(C,3)

#define BRAKE_LIGHT_EN PIN_DEF(D,0)
#define TAILLIGHT_EN PIN_DEF(D,1)
#define HEADLIGHT_HI_EN PIN_DEF(D,2)
#define HEADLIGHT_LO_EN PIN_DEF(D,3)
#define CHARGER_CONTACTOR_EN PIN_DEF(D,4)
#define DCDC_CONTACTOR_EN PIN_DEF(D,5)
#define BATTERY_CONTACTOR_EN PIN_DEF(D,6)
#define PRECHARGE_RELAY_EN PIN_DEF(D,7)
#define HORN_EN PIN_DEF(D,8)
#define AUX_PORT_EN PIN_DEF(D,9)
#define TURN_SIGNAL_FR_EN PIN_DEF(D,10)
#define TURN_SIGNAL_RR_EN PIN_DEF(D,11)
#define TURN_SIGNAL_FL_EN PIN_DEF(D,12)
#define TURN_SIGNAL_RL_EN PIN_DEF(D,13)
#define HEATED_GRIPS_EN PIN_DEF(D,14)
#define HEATED_SEAT_EN PIN_DEF(D,15)

#define SW_EN PIN_DEF(E,0)
#define DIAG_EN PIN_DEF(E,1)
#define DIAG_SELECT_EN PIN_DEF(E,2)
#define DCDC_EN PIN_DEF(E,3)
#define BATT_EN PIN_DEF(E,4)
#define CAN_SLEEP_EN PIN_DEF(E,5)
#define IC_CONTROLLER_SLEEP_EN PIN_DEF(E,6)
#define IC_CONTROLLER_nFAULT PIN_DEF(E,7)
#define DCDC_nFAULT PIN_DEF(E,8)
#define BATT_nFAULT PIN_DEF(E,9)

#define PUMP_1_EN PIN_DEF(F,2)
#define FAN_1_EN PIN_DEF(F,3)
#define BRAKE_SWITCH_1_IN PIN_DEF(F,13)
#define BRAKE_SWITCH_2_IN PIN_DEF(F,12)

#define IGNITION_SWITCH_IN PIN_DEF(G,0)
#define KILL_SWITCH_IN PIN_DEF(G,1)
#define TURN_LEFT_SWITCH_IN PIN_DEF(G,2)
#define TURN_RIGHT_SWITCH_IN PIN_DEF(G,3)
#define BRIGHTS_SWITCH_IN PIN_DEF(G,12)
#define HORN_SWITCH_IN PIN_DEF(G,13)
#define KICKSTAND_SWITCH_IN PIN_DEF(G,14)

/*ANALOG*/
#define FAN_ISENSE_AI AN0
#define THOTTLE_SIGNAL_MONITOR_AI AN1
#define TAILLIGHT_ISENSE_AI AN2
#define HEADLIGHT_ISENSE_AI AN3
#define HORN_ISENSE_AI AN4
#define AUX_PORT_ISENSE_AI AN5
#define LOCK_ISENSE_AI AN6
#define HEATER_ISENSE_AI AN7
#define ECU_2_ISENSE_AI AN8
#define ECU_1_ISENSE_AI AN9
#define V12_MONITOR_AI AN10
#define BATT_ISENSE_AI AN11
#define DCDC_ISENSE_AI AN12
#define IC_CONTROLLER_ISENSE_AI AN13
#define RADIATOR_INPUT_TEMP_AI AN14
#define RADIATOR_OUTPUT_TEMP_AI AN15
#define P12_MONITOR_AI AN16
#define PILOT_MONITOR_AI AN19 //R177 is wrong, should be 560k, not 10k
#define PROXIMITY_MONITOR_AI AN23

/*COMUNICATION*/
#define UART_TX RP120_OUT
#define UART_RX RPI121_IN

#define CAN_TX RP118_OUT
#define CAN_RX RPI119_IN

// *****************************************************************************
// *****************************************************************************
// Section: Function Declarations
// *****************************************************************************
// *****************************************************************************

/**
 * 
 */
void PinSetup_Init(void) {
    /* Configure all AD pins as digital */
#ifdef ANSELA
    ANSELA = 0x0000;
#endif
#ifdef ANSELB
    ANSELB = 0x0000;
#endif
#ifdef ANSELC
    ANSELC = 0x0000;
#endif
#ifdef ANSELD
    ANSELD = 0x0000;
#endif
#ifdef ANSELE
    ANSELE = 0x0000;
#endif
#ifdef ANSELF
    ANSELF = 0x0000;
#endif
#ifdef ANSELG
    ANSELG = 0x0000;
#endif


    /* Set the PWM's off */
#ifdef IOCON1
    IOCON1bits.PENH = 0;
    IOCON1bits.PENL = 0;
#endif
#ifdef IOCON2
    IOCON2bits.PENH = 0;
    IOCON2bits.PENL = 0;
#endif
#ifdef IOCON3
    IOCON3bits.PENH = 0;
    IOCON3bits.PENL = 0;
#endif

    /*Set all digital IO*/
    PINS_direction(DEBUG_LED_EN, OUTPUT);
    PINS_direction(DEBUG_PIN_EN, OUTPUT);
    PINS_direction(BMS_CONTROLLER_EN, OUTPUT);
    PINS_direction(J1772_CONTROLLER_EN, OUTPUT);
    PINS_direction(MOTOR_CONTROLLER_EN, OUTPUT);
    PINS_direction(CHARGE_CONTROLLER_EN, OUTPUT);
    PINS_direction(PILOT_EN, OUTPUT);
    PINS_direction(TURN_SIGNAL_L_STATUS, INPUT);
    PINS_direction(TURN_SIGNAL_R_STATUS, INPUT);
    PINS_direction(CONTACT_1_2_STATUS, INPUT);
    PINS_direction(CONTACT_3_4_STATUS, INPUT);
    PINS_direction(SPARE_SWITCH_1_IN, INPUT);
    PINS_direction(SPARE_SWITCH_2_IN, INPUT);
    PINS_direction(BRAKE_LIGHT_EN, OUTPUT);
    PINS_direction(TAILLIGHT_EN, OUTPUT);
    PINS_direction(HEADLIGHT_HI_EN, OUTPUT);
    PINS_direction(HEADLIGHT_LO_EN, OUTPUT);
    PINS_direction(CHARGER_CONTACTOR_EN, OUTPUT);
    PINS_direction(DCDC_CONTACTOR_EN, OUTPUT);
    PINS_direction(BATTERY_CONTACTOR_EN, OUTPUT);
    PINS_direction(PRECHARGE_RELAY_EN, OUTPUT);
    PINS_direction(HORN_EN, OUTPUT);
    PINS_direction(AUX_PORT_EN, OUTPUT);
    PINS_direction(TURN_SIGNAL_FR_EN, OUTPUT);
    PINS_direction(TURN_SIGNAL_RR_EN, OUTPUT);
    PINS_direction(TURN_SIGNAL_FL_EN, OUTPUT);
    PINS_direction(TURN_SIGNAL_RL_EN, OUTPUT);
    PINS_direction(HEATED_GRIPS_EN, OUTPUT);
    PINS_direction(HEATED_SEAT_EN, OUTPUT);
    PINS_direction(SW_EN, OUTPUT);
    PINS_direction(DIAG_EN, OUTPUT);
    PINS_direction(DIAG_SELECT_EN, OUTPUT);
    PINS_direction(DCDC_EN, OUTPUT);
    PINS_direction(BATT_EN, OUTPUT);
    PINS_direction(CAN_SLEEP_EN, OUTPUT);
    PINS_direction(IC_CONTROLLER_SLEEP_EN, OUTPUT);
    PINS_direction(IC_CONTROLLER_nFAULT, INPUT);
    PINS_direction(DCDC_nFAULT, INPUT);
    PINS_direction(BATT_nFAULT, INPUT);
    PINS_direction(PUMP_1_EN, OUTPUT);
    PINS_direction(FAN_1_EN, OUTPUT);
    PINS_direction(BRAKE_SWITCH_1_IN, INPUT);
    PINS_direction(BRAKE_SWITCH_2_IN, INPUT);
    PINS_direction(IGNITION_SWITCH_IN, INPUT);
    PINS_direction(KILL_SWITCH_IN, INPUT);
    PINS_direction(TURN_LEFT_SWITCH_IN, INPUT);
    PINS_direction(TURN_RIGHT_SWITCH_IN, INPUT);
    PINS_direction(BRIGHTS_SWITCH_IN, INPUT);
    PINS_direction(HORN_SWITCH_IN, INPUT);
    PINS_direction(KICKSTAND_SWITCH_IN, INPUT);

    /*ANALOG*/
    ADC_Init();
    ADC_SetPin(FAN_ISENSE_AI);
    ADC_SetPin(THOTTLE_SIGNAL_MONITOR_AI);
    ADC_SetPin(TAILLIGHT_ISENSE_AI);
    ADC_SetPin(HEADLIGHT_ISENSE_AI);
    ADC_SetPin(HORN_ISENSE_AI);
    ADC_SetPin(AUX_PORT_ISENSE_AI);
    ADC_SetPin(LOCK_ISENSE_AI);
    ADC_SetPin(HEATER_ISENSE_AI);
    ADC_SetPin(ECU_2_ISENSE_AI);
    ADC_SetPin(ECU_1_ISENSE_AI);
    ADC_SetPin(V12_MONITOR_AI);
    ADC_SetPin(BATT_ISENSE_AI);
    ADC_SetPin(DCDC_ISENSE_AI);
    ADC_SetPin(IC_CONTROLLER_ISENSE_AI);
    ADC_SetPin(RADIATOR_INPUT_TEMP_AI);
    ADC_SetPin(RADIATOR_OUTPUT_TEMP_AI);
    ADC_SetPin(P12_MONITOR_AI);
    ADC_SetPin(PILOT_MONITOR_AI);
    ADC_SetPin(PROXIMITY_MONITOR_AI);


    /*PWM*/
    pwmOCinit(PWM_PIN_RP100);
    pwmOCinit(PWM_PIN_RP101);

    /*UART*/
    Uart1INIT(UART_TX, UART_RX, UART_BAUD_115200);
    /*CAN*/
    CAN_Init(CAN_TX, CAN_RX, CAN_BAUD_1M, CAN_NORMAL);

}

void SET_DEBUG_LED_EN(uint8_t state) {
    PINS_write(DEBUG_LED_EN, state);
}

void SET_DEBUG_PIN_EN(uint8_t state) {
    PINS_write(DEBUG_PIN_EN, state);
}

void SET_BMS_CONTROLLER_EN(uint8_t state) {
    PINS_write(BMS_CONTROLLER_EN, state);
}

void SET_J1772_CONTROLLER_EN(uint8_t state) {
    PINS_write(J1772_CONTROLLER_EN, state);
}

void SET_MOTOR_CONTROLLER_EN(uint8_t state) {
    PINS_write(MOTOR_CONTROLLER_EN, state);
}

void SET_CHARGE_CONTROLLER_EN(uint8_t state) {
    PINS_write(CHARGE_CONTROLLER_EN, state);
}

void SET_PILOT_EN(uint8_t state) {
    PINS_write(PILOT_EN, state);
}

void SET_BRAKE_LIGHT_EN(uint8_t state) {
    PINS_write(BRAKE_LIGHT_EN, state);
}

void SET_TAILLIGHT_EN(uint8_t state) {
    PINS_write(TAILLIGHT_EN, state);
}

void SET_HEADLIGHT_HI_EN(uint8_t state) {
    PINS_write(HEADLIGHT_HI_EN, state);
}

void SET_HEADLIGHT_LO_EN(uint8_t state) {
    PINS_write(HEADLIGHT_LO_EN, state);
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
    PINS_write(HORN_EN, state);
}

void SET_AUX_PORT_EN(uint8_t state) {
    PINS_write(AUX_PORT_EN, state);
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
    PINS_write(HEATED_GRIPS_EN, state);
}

void SET_HEATED_SEAT_EN(uint8_t state) {
    PINS_write(HEATED_SEAT_EN, state);
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
    PINS_write(DCDC_EN, state);
}

void SET_BATT_EN(uint8_t state) {
    PINS_write(BATT_EN, state);
}

void SET_CAN_SLEEP_EN(uint8_t state) {
    PINS_write(CAN_SLEEP_EN, state);
}

void SET_IC_CONTROLLER_SLEEP_EN(uint8_t state) {
    PINS_write(IC_CONTROLLER_SLEEP_EN, state);
}

void SET_PUMP_1_EN(uint8_t state) {
    PINS_write(PUMP_1_EN, state);
}

void SET_FAN_1_EN(uint8_t state) {
    PINS_write(FAN_1_EN, state);
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