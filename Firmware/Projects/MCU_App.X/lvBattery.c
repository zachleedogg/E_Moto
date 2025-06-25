#include "lvBattery.h"
#include "IO.h"
#include "movingAverage.h"
/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/
#define LV_BATTERY_CHARGE_FULL_LEVEL 13.5
#define LV_BATTERY_CHARGE_EMPTY_LEVEL 11.0
#define LV_BATTERY_CHARGE_FULL_CURRENT_LEVEL 0.2
/******************************************************************************
 * Configuration
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/

/******************************************************************************
 * Variable Declarations
 *******************************************************************************/
static uint8_t lvBattery_run = 0;
lvBatteryState_E lvBatteryState = LV_BATTERY_NOMINAL;

/* 1Hz Filters on battery voltage and current*/
NEW_LOW_PASS_FILTER(lvBatteryVoltage, 1.0, 100.0);
NEW_LOW_PASS_FILTER(lvBatteryVoltageFast, 10.0, 100.0);
NEW_LOW_PASS_FILTER(dcdcInputCurrent, 1.0, 100.0);
NEW_LOW_PASS_FILTER(lvBatteryCurrent, 1.0, 100.0);

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void lvBattery_Init(void) {
    lvBattery_run = 1;
    lvBatteryState = LV_BATTERY_NOMINAL;
    /*Initialize the fast filter so we can get up and running more quickly*/
    if (IO_GET_SW_EN()) {
        lvBatteryVoltageFast->accum = IO_GET_VOLTAGE_VBAT_SW();
    }
}

void lvBattery_Run_10ms(void) {
    if (lvBattery_run && IO_GET_SW_EN()) {
        /*Get all voltage and current readings*/
        takeLowPassFilter(lvBatteryVoltage, IO_GET_VOLTAGE_VBAT_SW());
        takeLowPassFilter(lvBatteryVoltageFast, IO_GET_VOLTAGE_VBAT_SW());
        takeLowPassFilter(lvBatteryCurrent, IO_GET_CURRENT_BATT());

        /*If current into the battery goes up, and dcdc current into the controller goes up, we are charging!*/
        if (lvBatteryCurrent->accum > LV_BATTERY_CHARGE_FULL_CURRENT_LEVEL &&
                dcdcInputCurrent->accum < 0.0) {
            lvBatteryState = LV_BATTERY_CHARGING;
        }

        /*If current into the battery goes down, and dcdc current into the controller goes up, we are charged!*/
        if (lvBatteryCurrent->accum < LV_BATTERY_CHARGE_FULL_CURRENT_LEVEL && 
                dcdcInputCurrent->accum < 0.0) {
            lvBatteryState = LV_BATTERY_CHARGED;
        }
        
        /*If Battery voltage is greater than empty level, and dcdc current 0, then we are nominal*/
        if (lvBatteryVoltage->accum >= LV_BATTERY_CHARGE_EMPTY_LEVEL && 
                dcdcInputCurrent->accum >= 0.0) {
            lvBatteryState = LV_BATTERY_NOMINAL;
        }

        


    }
}

void lvBattery_Halt(void) {
    lvBattery_run = 0;
}

lvBatteryState_E lvBattery_GetState(void) {
    return lvBatteryState;
}