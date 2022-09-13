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
#include "mcc_generated_files/clock.h"
#include "bolt_sleep.h"
#include <xc.h>

// *****************************************************************************
// *****************************************************************************
// Section: Module Defines and variables
// *****************************************************************************
// *****************************************************************************


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
    pwmOCinit(PWM_1_OUT);
    pwmOCinit(PWM_2_OUT);

    /*UART*/
    Uart1INIT(UART_TX, UART_RX, UART_BAUD_115200, CLOCK_SystemFrequencyGet());
    /*CAN*/
    CAN_Init(CAN_TX, CAN_RX, CAN_BAUD_500k, CAN_DISABLE, CLOCK_SystemFrequencyGet());
    
    //set wakeup sources
    setWakeUp(PIN, IGNITION_SWITCH_IN);
    /*CAN is wake source already in CAN_Init()*/

}


