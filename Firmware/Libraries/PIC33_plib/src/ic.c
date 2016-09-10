/* 
 * File:   ic.c
 * Author: Zachary Levenberg
 *
 * Created on August 6, 2016, 9:27 AM
 */

#include "ic.h"
#include <xc.h>

/******************************************************************************
 * MODULE DEFINES
 ******************************************************************************/
#define AVAILABLE 0
#define UNAVAILABLE 1

#define RISING 0
#define FALLING 1

typedef enum {
    IC_MODULE1,
    IC_MODULE2,
    IC_MODULE3,
    IC_MODULE4,
    NUMBER_IC_MODULES,
} ic_module_number;



/*******************************************************************************
 * MODULE VARIABLES
 ******************************************************************************/
typedef struct _icReadyBits {
    uint8_t ICmodule1 : 1;
    uint8_t ICmodule2 : 1;
    uint8_t ICmodule3 : 1;
    uint8_t ICmodule4 : 1;
} icReadyBits;

static union {
    icReadyBits theBitField;
    uint8_t theInt;
} icReadyStatus;

static uint16_t icTime[NUMBER_IC_MODULES] = {};

typedef struct _ic_module {
    uint8_t moduleNumber;
    uint8_t ppsMap;
    ic_pin_number Pin;
    uint8_t availability;
    ic_status status;
    uint16_t time;
} ic_module;

static ic_module modules[NUMBER_IC_MODULES];


/*******************************************************************************
 * MODULE FUNCTIONS
 ******************************************************************************/

/**
 * 
 * @param newICPin
 * @return 
 */
uint8_t ic_Init(ic_pin_number newICPin) {
    uint8_t returnVal = 0;

    /*check the pin for available modules*/
    uint8_t i = 0;
    for (i = 0; i < NUMBER_IC_MODULES; i++) {/*check modules in use*/
        if (modules[i].availability == AVAILABLE) {
            modules[i].availability = UNAVAILABLE;
            modules[i].moduleNumber = i;
            modules[i].Pin = newICPin;
            modules[i].status = IDLE;
            break;
        }
    }

    /*set the pinmap to the correct pin and configure the module*/
    switch (i) {
        case IC_MODULE1:
            _IC1R = modules[IC_MODULE1].Pin;
            IC1CON1bits.ICTSEL = 0b111; /*Use peripheral clock source*/
            IC1CON1bits.ICM = 0b001; /*Capture on every rising and falling edge*/
            IC1CON2bits.SYNCSEL = 0b00000; /*Input Source is Software*/
            IC1CON2bits.ICTRIG = 0b00; /*no input trigger trigger*/
            IC1CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
            _IC1IP = 4;
            _IC1IE = 1;
            break;

        case IC_MODULE2:
            _IC2R = modules[IC_MODULE2].Pin;
            IC2CON1bits.ICTSEL = 0b111; /*Use peripheral clock source*/
            IC2CON1bits.ICM = 0b001; /*Capture on every rising and falling edge*/
            IC2CON2bits.SYNCSEL = 0b00000; /*Input Source is Software*/
            IC2CON2bits.ICTRIG = 0b01; /*Input Source is the timer trigger*/
            IC2CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
            _IC2IP = 4;
            _IC2IE = 1;
            break;

        case IC_MODULE3:
            _IC3R = modules[IC_MODULE3].Pin;
            IC3CON1bits.ICTSEL = 0b111; /*Use peripheral clock source*/
            IC3CON1bits.ICM = 0b001; /*Capture on every rising and falling edge*/
            IC3CON2bits.SYNCSEL = 0b00000; /*Input Source is Software*/
            IC3CON2bits.ICTRIG = 0b01; /*Input Source is the timer trigger*/
            IC3CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
            _IC3IP = 4;
            _IC3IE = 1;
            break;

        case IC_MODULE4:
            _IC4R = modules[IC_MODULE4].Pin;
            IC4CON1bits.ICTSEL = 0b111; /*Use peripheral clock source*/
            IC4CON1bits.ICM = 0b001; /*Capture on every rising and falling edge*/
            IC4CON2bits.SYNCSEL = 0b00000; /*Input Source is Software*/
            IC4CON2bits.ICTRIG = 0b01; /*Input Source is the timer trigger*/
            IC4CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
            _IC4IP = 4;
            _IC4IE = 1;
            break;

        default:
            returnVal = 1; /*not a valid pin*/
            break;
    }

    return returnVal;
}

/**
 * 
 * @return 
 */
uint8_t ic_Ready(void) {
    return icReadyStatus.theInt;
}

/**
 * 
 * @param thisModule
 * @return 
 */
uint16_t ic_getTime(ic_pin_number thisModule) {
    /*check the pin for available modules*/
    uint16_t time = 0;
    uint8_t i = 0;
    for (i = 0; i < NUMBER_IC_MODULES; i++) {/*check modules in use*/
        if ((modules[i].Pin == thisModule) && (icReadyStatus.theInt & (1 << i))) {
            icReadyStatus.theInt &= ~(1 << i); /*Clear ready bit*/
            time = modules[i].time; /*return the Time*/
            break;
        }
    }
    /*invalid pin or no new data*/
    return time;
}

/**
 * 
 * @param thisModule
 * @param mode
 * @return 
 */
uint8_t ic_moduleMode(ic_pin_number thisModule, uint8_t mode) {

    /*Set returnVal to success*/
    uint8_t returnVal = 0;

    /*check the pin for available modules*/
    uint8_t i = 0;
    for (i = 0; i < NUMBER_IC_MODULES; i++) {/*check modules in use*/
        if (modules[i].Pin == thisModule) {
            break;
        }
    }

    switch (i) {
        case IC_MODULE1:
            IC1CON1bits.ICM = mode; /*Disable the Module*/
            break;
        case IC_MODULE2:
            IC2CON1bits.ICM = mode; /*Disable the Module*/
            break;
        case IC_MODULE3:
            IC3CON1bits.ICM = mode; /*Disable the Module*/
            break;
        case IC_MODULE4:
            IC4CON1bits.ICM = mode; /*Disable the Module*/
            break;
        default:
            returnVal = 1; /*Not a valid pin*/
            break;
    }
    return returnVal;
}

//void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void) {
//    /*clear the interrupt*/
//    _IC1IF = 0;
//
//    testVar = 1;
//    switch (modules[1].status) {
//        case IDLE:
//            IC1CON2bits.TRIGSTAT = 1; /*Release the ICx Timer from reset*/
//            /*clear the 0x0000 reading*/
//            uint16_t temp = IC1BUF;
//            modules[1].status = RUNNING;
//            break;
//        case RUNNING:
//            /*get the rise time*/
//            modules[1].time = IC1BUF;
//            modules[1].status = IDLE;
//            IC1CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
//            break;
//        default:
//            break;
//    }
//
//    /*set the ready bit*/
//    icReadyStatus.theBitField.ICmodule1 = 1;
//}

void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void) {
    /*clear the interrupt*/
    _IC2IF = 0;

    /*get the rise time and the fall time*/
    uint16_t start = IC2BUF;
    uint16_t end = IC2BUF;

    /*Caluclate time with wrap-around protection*/
    if (start < end) {
        icTime[IC_MODULE2] = end - start;
    } else {
        icTime[IC_MODULE2] = 65535 - start + end;
    }

    /*set the ready bit*/
    icReadyStatus.theBitField.ICmodule2 = 1;
}

void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void) {
    /*clear the interrupt*/
    _IC3IF = 0;

    /*get the rise time and the fall time*/
    uint16_t start = IC3BUF;
    uint16_t end = IC3BUF;

    /*Caluclate time with wrap-around protection*/
    if (start < end) {
        icTime[IC_MODULE3] = end - start;
    } else {
        icTime[IC_MODULE3] = 65535 - start + end;
    }

    /*set the ready bit*/
    icReadyStatus.theBitField.ICmodule3 = 1;
}

void __attribute__((__interrupt__, auto_psv)) _IC4Interrupt(void) {
    /*clear the interrupt*/
    _IC4IF = 0;

    /*get the rise time and the fall time*/
    uint16_t start = IC4BUF;
    uint16_t end = IC4BUF;

    /*Caluclate time with wrap-around protection*/
    if (start < end) {
        icTime[IC_MODULE4] = end - start;
    } else {
        icTime[IC_MODULE4] = 65535 - start + end;
    }

    /*set the ready bit*/
    icReadyStatus.theBitField.ICmodule4 = 1;
}

