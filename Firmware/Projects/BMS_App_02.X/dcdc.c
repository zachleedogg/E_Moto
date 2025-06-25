#include "dcdc.h"
#include "IO.h"
#include "movingAverage.h"
#include "bms_dbc.h"
#include <stdint.h>

static uint8_t dcdc_run = 0;
static uint8_t dcdc_state = DCDC_OFF;
NEW_LOW_PASS_FILTER(dcdc_voltage, 10.0, 1000.0);
NEW_LOW_PASS_FILTER(dcdc_current, 10.0, 1000.0);

void DCDC_init(void) {
    dcdc_run = 1;
    dcdc_state = DCDC_OFF;
}

void DCDC_run_1ms(void) {
    if (dcdc_run) {
        float myval = IO_GET_DCDC_OUTPUT_VOLTAGE();
        takeLowPassFilter(dcdc_voltage, myval);
        takeLowPassFilter(dcdc_current, IO_GET_DCDC_CURRENT());
    }
}

void DCDC_run_100ms(void) {
    if (dcdc_run) {
        uint8_t dcdcCommandFromMCU = CAN_mcu_command_DCDC_enable_get();

        switch (dcdc_state) {
            case DCDC_OFF:
                IO_SET_DCDC_EN(LOW);
                if (dcdcCommandFromMCU) {
                    dcdc_state = DCDC_PRECHARGE;
                    IO_SET_PRE_CHARGE_EN(HIGH);
                }
                break;
            case DCDC_PRECHARGE:
                if (dcdc_voltage->accum > 80.0) {
                    dcdc_state = DCDC_ENABLE;
                    IO_SET_DCDC_EN(HIGH);
                    IO_SET_PRE_CHARGE_EN(LOW);
                }
                if (dcdcCommandFromMCU == 0) {
                    dcdc_state = DCDC_OFF;
                    IO_SET_PRE_CHARGE_EN(LOW);
                }
                break;
            case DCDC_ENABLE:
                if (IO_GET_DCDC_FAULT()) {
                    dcdc_state = DCDC_FAULT;
                }
                if (dcdcCommandFromMCU == 0) {
                    dcdc_state = DCDC_OFF;
                }
                break;
            case DCDC_FAULT:
                IO_SET_DCDC_EN(LOW);
                if (dcdcCommandFromMCU == 0) {
                    dcdc_state = DCDC_OFF;
                }
            default:
                break;
        }

    } else {
        IO_SET_DCDC_EN(LOW);
        dcdc_state = DCDC_OFF;
    }
}

void DCDC_halt() {
    dcdc_run = 0;
    dcdc_state = DCDC_OFF;
    IO_SET_DCDC_EN(LOW);
}

DCDC_state_E DCDC_getState(void){
    return dcdc_state;
}

float DCDC_getVoltage(void){
    return dcdc_voltage->accum;
}

float DCDC_getCurrent(void){
    return dcdc_current->accum;
}