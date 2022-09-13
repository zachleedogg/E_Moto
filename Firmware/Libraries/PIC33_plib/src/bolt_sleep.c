/*
 * File:   bolt_sleep.c
 * Author: Zach Levenberg
 *
 * Created on February 10, 2016, 10:23 PM
 */

#include "bolt_sleep.h"
#include "pins.h"

typedef struct _wake {
    wake_up_type type;
    PINS_pin_S pin;
} wake;

static wake wakeUps[] = {
    {-1,
        {0, 0}},
    {-1,
        {0, 0}},
    {-1,
        {0, 0}},
    {-1,
        {0, 0}}
};

uint8_t setWakeUp(wake_up_type thisType, PINS_pin_S pin) {
    if (thisType >= NUMBER_OF_WAKE_TYPES) {
        return 0;
    }
    wakeUps[thisType].type = thisType;
    wakeUps[thisType].pin = pin;
    return 1;
}

void SleepNow() {
    int i = 0;
    for (i = 0; i < NUMBER_OF_WAKE_TYPES; i++) {
        switch (wakeUps[i].type) {
            case NO_SEL:
                break;
            case WAKE_ON_UART1:
                U1MODEbits.WAKE = 1;
                break;
            case WAKE_ON_UART2:
                U2MODEbits.WAKE = 1;
                break;
            case WAKE_ON_CAN:
                C1INTEbits.WAKIE = 1;
                break;
            case PIN:
                _CNIF = 0;
                _CNIE = 1;
                PINS_internalRegisters_SetInterrupt(wakeUps[i].pin, 1);
                break;
            default:
                break;
        }
    }

    //Nop();
    Sleep();
    //Nop();

}

/*	Change Notification Interrupt*/
void __attribute__((__interrupt__, auto_psv)) _CNInterrupt(void) {
    _CNIF = 0;
}
