/* 
 * File:   OutputCompare
 * Author: Zach Levenberg
 * Comments: This module is intended for use as PWM module
 * Revision history: 
 */

#include "bolt_OC.h" // include processor files - each processor file is guarded.  

#define NUMBER_OC_MODULES 4

#define AVAILABLE 0
#define UNAVAILABLE 1

typedef struct _OCmodule {
    uint8_t moduleNumber;
    uint8_t ppsMap;
    uint8_t Pin;
    uint8_t status;
    uint8_t duty;
    uint16_t freq;
} OCmodule;

static uint16_t OCMasterPeriod = 2000;

static OCmodule modules[NUMBER_OC_MODULES];

/*Peripheral Pin Select Mapping*/
#define OC1_PPS_MAP 0b00010000
#define OC2_PPS_MAP 0b00010001
#define OC3_PPS_MAP 0b00010010
#define OC4_PPS_MAP 0b00010011

uint8_t ppsMapArr[NUMBER_OC_MODULES] = {OC1_PPS_MAP, OC2_PPS_MAP, OC3_PPS_MAP, OC4_PPS_MAP};

/**
 * pwm init, there are only 4 modules.
 * @param pin RP pin number
 * @return succes or failure
 */
uint8_t pwmOCinit(oc_pin_number pin) {

    OCmodule currentModule = {}; /*variable to hold module to assign*/
    uint8_t i = 0;

    /*check the pin for available modules*/
    for (i = 0; i < NUMBER_OC_MODULES; i++) {/*check modules in use*/
        if (modules[i].status == AVAILABLE) {
            modules[i].status = UNAVAILABLE;
            modules[i].moduleNumber = i;
            modules[i].Pin = pin;
            modules[i].ppsMap = ppsMapArr[i];
            modules[i].duty = 0;
            modules[i].freq = OCMasterPeriod;
            currentModule = modules[i];
            break;
        }
    }

    if (i == NUMBER_OC_MODULES) {
        return 0; /*all modules are in use*/
    }

    /*set the pinmap to the correct pin*/
    switch (pin) {
#ifdef _RP20R
        case PWM_PIN_RP20:
            _RP20R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP35R
        case PWM_PIN_RP35:
            _RP35R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP36R
        case PWM_PIN_RP36:
            _RP36R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP37R
        case PWM_PIN_RP37:
            _RP37R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP38R
        case PWM_PIN_RP38:
            _RP38R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP39R
        case PWM_PIN_RP39:
            _RP39R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP40R
        case PWM_PIN_RP40:
            _RP40R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP41R
        case PWM_PIN_RP41:
            _RP41R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP42R
        case PWM_PIN_RP42:
            _RP42R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP43R
        case PWM_PIN_RP43:
            _RP43R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP54R
        case PWM_PIN_RP54:
            _RP54R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP55R
        case PWM_PIN_RP55:
            _RP55R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP56R
        case PWM_PIN_RP56:
            _RP56R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP57R
        case PWM_PIN_RP57:
            _RP57R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP100R
        case PWM_PIN_RP100:
            _RP100R = currentModule.ppsMap;
            break;
#endif
#ifdef _RP101R
        case PWM_PIN_RP101:
            _RP101R = currentModule.ppsMap;
            break;
#endif

        default:
            return 0; /*not a valid pin*/
            break;
    }

    /*configure the module*/
    switch (currentModule.moduleNumber) {
        case 0:
            OC1CON1 = 0; /* It is a good practice to clear off the control bits initially */
            OC1CON2 = 0;
            OC1CON1bits.OCTSEL = 0x07; /* selects the periph clock as the input to the OC module */
            OC1CON1bits.OCM = 0b110; /* This selects and starts the PWM mode */
            OC1R = 0; /*duty cycle*/
            OC1RS = OCMasterPeriod; /*period*/
            OC1CON2bits.SYNCSEL = 0x1F; /*sync source is itslef*/
            break;
        case 1:
            OC2CON1 = 0; /* It is a good practice to clear off the control bits initially */
            OC2CON2 = 0;
            OC2CON1bits.OCTSEL = 0x07; /* selects the periph clock as the input to the OC module */
            OC2CON1bits.OCM = 0b110; /* This selects and starts the PWM mode */
            OC2R = 0; /*duty cycle*/
            OC2RS = OCMasterPeriod; /*period*/
            OC2CON2bits.SYNCSEL = 0x1F; /*sync source is itslef*/
            break;
        case 2:
            OC3CON1 = 0; /* It is a good practice to clear off the control bits initially */
            OC3CON2 = 0;
            OC3CON1bits.OCTSEL = 0x07; /* selects the periph clock as the input to the OC module */
            OC3CON1bits.OCM = 0b110; /* This selects and starts the PWM mode */
            OC3R = 0; /*duty cycle*/
            OC3RS = OCMasterPeriod; /*period*/
            OC3CON2bits.SYNCSEL = 0x1F; /*sync source is itslef*/
            break;
        case 3:
            OC4CON1 = 0; /* It is a good practice to clear off the control bits initially */
            OC4CON2 = 0;
            OC4CON1bits.OCTSEL = 0x07; /* selects the periph clock as the input to the OC module */
            OC4CON1bits.OCM = 0b110; /* This selects and starts the PWM mode */
            OC4R = 0; /*duty cycle*/
            OC4RS = OCMasterPeriod; /*period*/
            OC4CON2bits.SYNCSEL = 0x1F; /*sync source is itslef*/
            break;
    }
    return 1;
}

uint8_t pwmOCwriteDuty(oc_pin_number pin, uint16_t dutyCycle) {
    if (dutyCycle > 100) {
        dutyCycle = 100; /*invalid dutycycle*/
    }
    
    uint8_t i = 0;
    uint8_t currentModule = 0;
    for (i = 0; i < NUMBER_OC_MODULES; i++) {
        if (modules[i].Pin == pin) {
            currentModule = modules[i].moduleNumber;
            modules[i].duty = dutyCycle;
            break;
        }
    }
    if (i == NUMBER_OC_MODULES) {
        return 0; /*invalid pin*/
    }


    switch (currentModule) {
        case 0:
            OC1R = (((uint32_t) modules[0].duty)*((uint32_t) modules[0].freq)) / 100; /*duty cycle*/
            break;
        case 1:
            OC2R = (((uint32_t) modules[1].duty)*((uint32_t) modules[1].freq)) / 100; /*duty cycle*/
            break;
        case 2:
            OC3R = (((uint32_t) modules[2].duty)*((uint32_t) modules[2].freq)) / 100; /*duty cycle*/
            break;
        case 3:
            OC4R = (((uint32_t) modules[3].duty)*((uint32_t) modules[3].freq)) / 100; /*duty cycle*/
            break;
        default:
            return 0; /*invalid pin*/
    }
    return 1;
}

uint8_t pwmOCwriteFreq(oc_pin_number pin, uint16_t frequency) {
    uint8_t i = 0;
    uint8_t currentModule = 0;
    for (i = 0; i < NUMBER_OC_MODULES; i++) {
        if (modules[i].Pin == pin) {
            currentModule = modules[i].moduleNumber;
            modules[i].freq = frequency;
            break;
        }
    }
    if (i == NUMBER_OC_MODULES) {
        return 0; /*invalid pin*/
    }

    switch (currentModule) {
        case 0:
            OC1RS = modules[0].freq; /*duty cycle*/
            OC1R = (((uint32_t) modules[0].duty)*((uint32_t) modules[0].freq)) / 100; /*duty cycle*/
            break;
        case 1:
            OC2RS = modules[1].freq; /*duty cycle*/
            OC2R = (((uint32_t) modules[1].duty)*((uint32_t) modules[1].freq)) / 100; /*duty cycle*/
            break;
        case 2:
            OC3RS = modules[2].freq; /*duty cycle*/
            OC3R = (((uint32_t) modules[2].duty)*((uint32_t) modules[2].freq)) / 100; /*duty cycle*/
            break;
        case 3:
            OC4RS = modules[3].freq; /*duty cycle*/
            OC4R = (((uint32_t) modules[3].duty)*((uint32_t) modules[3].freq)) / 100; /*duty cycle*/
            break;
        default:
            return 0; /*invalid pin*/
    }
    return 1;
}



