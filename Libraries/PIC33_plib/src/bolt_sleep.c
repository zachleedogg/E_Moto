/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
#include "sleep.h"
#include "pins.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

typedef struct _wake {
    wake_up_type type;
    pin_number pin;
} wake;

wake wakeUps[4] = {{-1,0},{-1,0},{-1,0},{-1,0}};
// *****************************************************************************
// *****************************************************************************
// Section: User Functions
// *****************************************************************************
// *****************************************************************************

uint8_t setWakeUp(wake_up_type thisType, pin_number pin) {
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
                IO_pinInterrupt(wakeUps[i].pin);
                break;
            default:
                break;
        }
    }
    Sleep();
}