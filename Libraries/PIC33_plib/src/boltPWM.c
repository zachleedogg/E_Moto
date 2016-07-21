/*
 * File:   boltPWM.c
 * Author: kid group
 *
 * Created on January 25, 2016, 6:53 PM
 */


#include "xc.h"
#include "boltPWM.h"

uint8_t pwmPinStatus[3] = {0, 0, 0};

uint8_t PWMinit(void) {

    /*Clock divider*/
    PTCON2bits.PCLKDIV = 0b000; /*divide-by-one (max res)*/

    /*Primary master time base PERIOD REGISTER*/
    PTPER = 1000; /*0-16 bits from PTCON2*/

    /*Master DUTY CYCLE register*/
    MDC = 1000; /*16 bits is set to 50%*/

    /*set all outputs to GPIO*/
    IOCON1bits.PENH = 0;
    IOCON1bits.PENL = 0;
    IOCON2bits.PENH = 0;
    IOCON2bits.PENL = 0;
    IOCON3bits.PENH = 0;
    IOCON3bits.PENL = 0;

    return 1;
}

uint8_t PWMpinInit(uint8_t pinNumber, uint16_t dutyCycle) {

    if (dutyCycle > 100) {
        return 0;
    }
    if (pwmPinStatus[pinNumber - 1] == 1) {
        return 0; /*error, pin already Initted*/
    }
    /*disnable module*/
    PTCONbits.PTEN = 0;
    
    uint32_t duty = 0;

    switch (pinNumber) {
        case 1:
            /* Set Duty Cycles */
            duty = (((uint32_t) PTPER)*((uint32_t) dutyCycle)) / 100;
            PDC1 = (uint16_t) duty;

            /*PWM1H is enabled*/
            IOCON1bits.PENH = 1;
            /* Configure Faults */
            FCLCON1 = 0x0003; /*none*/

            pwmPinStatus[0] = 1;

            break;

        case 2:
            /* Set Duty Cycles */
            duty = (((uint32_t) PTPER)*((uint32_t) dutyCycle)) / 100;
            PDC2 = (uint16_t) duty;

            /*PWM1H is enabled*/
            IOCON2bits.PENH = 1;
            /* Configure Faults */
            FCLCON2 = 0x0003; /*none*/

            pwmPinStatus[1] = 1;

            break;
            
        case 3:
            /* Set Duty Cycles */
            duty = (((uint32_t) PTPER)*((uint32_t) dutyCycle)) / 100;
            PDC3 = (uint16_t) duty;

            /*PWM1H is enabled*/
            IOCON3bits.PENH = 1;
            /* Configure Faults */
            FCLCON3 = 0x0003; /*none*/

            pwmPinStatus[2] = 1;

            break;
    }
    /*enable module*/
    PTCONbits.PTEN = 1;

    return 1;
}