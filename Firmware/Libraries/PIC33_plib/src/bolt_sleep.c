/*
 * File:   bolt_sleep.c
 * Author: Zach Levenberg
 *
 * Created on February 10, 2016, 10:23 PM
 */

#include "bolt_sleep.h"
#include "bolt_pins.h"

typedef struct _wake {
    wake_up_type type;
    PINS_pin_S pin;
} wake;

wake wakeUps[4] = {
    {-1, 0},
    {-1, 0},
    {-1, 0},
    {-1, 0}
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
                PINS_internalRegisters_SetInterrupt(wakeUps[i].pin, 1);
                break;
            default:
                break;
        }
    }
#ifdef PMD1
    //PMD1 = 0xFFFF;
    PMD1bits.C1MD = 0;
    PMD1bits.T5MD = 0;
#endif
#ifdef PMD2
    PMD2 = 0xFFFF;
#endif
#ifdef PMD3
    PMD3 = 0xFFFF;
#endif
#ifdef PMD4
    PMD4 = 0xFFFF;
#endif
#ifdef PMD5
    PMD5 = 0xFFFF;
#endif
#ifdef PMD6
    PMD6 = 0xFFFF;
#endif
#ifdef PMD7
    PMD7 = 0xFFFF;
#endif

    Nop();
    Sleep();
    Nop();

#ifdef PMD1
    PMD1 = 0x0000;
#endif
#ifdef PMD2
    PMD2 = 0x0000;
#endif
#ifdef PMD3
    PMD3 = 0x0000;
#endif
#ifdef PMD4
    PMD4 = 0x0000;
#endif
#ifdef PMD5
    PMD5 = 0x0000;
#endif
#ifdef PMD6
    PMD6 = 0x0000;
#endif
#ifdef PMD7
    PMD7 = 0x0000;
#endif

}