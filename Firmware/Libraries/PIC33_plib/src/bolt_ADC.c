/* 
 * File:   bolt_adc.h
 * Author: Zach Levenberg
 * Comments: Analog to digital Library for PIC33
 * Revision history: 3/13/16 initial build
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
#include "bolt_ADC.h"
#include "bolt_uart.h"

#include <xc.h>

// *****************************************************************************
// *****************************************************************************
// Section: Module Defines and variables
// *****************************************************************************
// *****************************************************************************

#define ON 1
#define OFF 0

#define ADC_ON() AD1CON1bits.ADON=1
#define ADC_OFF() AD1CON1bits.ADON=0

#define set(x,y) (x |= (y))
#define clear(x,y) (x &= ~(y))

typedef struct _ADCmodule {
    uint8_t numberOfInputs;
    uint16_t activePins;
} ADCmodule;

volatile uint16_t* const adcBuffer[16] = {
    &ADC1BUF0,
    &ADC1BUF1,
    &ADC1BUF2,
    &ADC1BUF3,
    &ADC1BUF4,
    &ADC1BUF5,
    &ADC1BUF6,
    &ADC1BUF7,
    &ADC1BUF8,
    &ADC1BUF9,
    &ADC1BUFA,
    &ADC1BUFB,
    &ADC1BUFC,
    &ADC1BUFD,
    &ADC1BUFE,
    &ADC1BUFF  
};

ADCmodule thisADC;

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

void selectADCpin(adc_pin_number newPin, uint8_t state);

void ADC_Init(void) {
    /* Initialize ADC module */

    /*Config reg 1*/
    // Enable 10-bit mode, auto-sample and auto-conversion
    _AD12B = 0; /*10 bit operation*/
    _SSRCG = 0; /*auto-convert*/
    _SSRC = 0b111; /*convert on internal timer*/
    _ASAM = 1; /*auto sample*/

    /*Config Reg 2*/
    // Sample inputs alternately using channel scanning on CH0
    _CSCNA = 1; /*input scanning on channel 0*/
    _SMPI = 0; /*number of pins + 1 to read per interrupt*/

    /*Config Reg 3*/
    _ADCS = 0x0F; /*ADC Clock Period (TAD = (ADCS + 1)/(SYSCLOCK/2))*/
    _SAMC = 0xF; /*Sample for t = SAMC*TAD before converting*/

    /*Config Reg 4*/
    AD1CON4 = 0x0000;

    /*Select ANx inputs to scan*/
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;

    /* Assign MUXA inputs */
    AD1CHS0bits.CH0SA = 0; // CH0SA bits ignored for CH0 +ve input selection
    AD1CHS0bits.CH0NA = 0; // Select VREF- for CH0 -ve input

    /* Do not Enable ADC module or interrupt, wait to add the pins. */
}

uint8_t ADC_SetPin(adc_pin_number newPin) {
    uint16_t pinMask = 1<<newPin;
    if (thisADC.activePins & pinMask) {
        return 1;
    }
    ADC_OFF(); /*Turn off ADC*/
    /*set pin as an analog input*/
    selectADCpin(newPin, 1);
    thisADC.numberOfInputs++; /*add number of inputs*/
    set(thisADC.activePins, pinMask); /*set new pin to active state*/
    AD1CSSL = thisADC.activePins; /*add pin to input sweep*/
    AD1CON2bits.SMPI = thisADC.numberOfInputs - 1; /*Number of pins to sweep*/
    ADC_ON();
    return 0;
}

uint8_t ADC_RemovePin(adc_pin_number newPin) {
    uint16_t pinMask = 1<<newPin;
    if ((thisADC.activePins & pinMask)== 0) {
        return 1;
    }
    ADC_OFF(); /*Turn off ADC*/
    /*disable analog input*/
    selectADCpin(newPin, 0);
    thisADC.numberOfInputs--; /*subtract number of inputs*/
    clear(thisADC.activePins, pinMask); /*set new pin to inactive state*/
    AD1CSSL = thisADC.activePins; /*add pin to input sweep*/
    AD1CON2bits.SMPI = thisADC.numberOfInputs - 1; /*Number of pins to sweep*/
    ADC_ON();
    return 0;
}

uint16_t ADC_GetValue(adc_pin_number thisPin) {
    uint16_t activePinsBeforeThisPin = 0;
    uint16_t mask = 0x0001;
    /*For each input, read ADC1BUFx value into correct pin buffer*/
    uint8_t i = 0;
    for (i = 0; i < thisPin; i++) {
        if(thisADC.activePins & (mask<<i)){
            activePinsBeforeThisPin++;
        }
    }
    return *adcBuffer[activePinsBeforeThisPin];
}

void selectADCpin(adc_pin_number newPin, uint8_t state){
    switch (newPin) {
#ifdef _ANSA0
        case AN0:
            _ANSA0 = state;
            _RA0 = 1;
            break;
#endif
#ifdef _ANSA1
        case AN1:
            _ANSA1 = state;
            _RA1 = 1;
            break;
#endif
#ifdef _ANSB0
        case AN2:
            _ANSB0 = state;
            _RB0 = 1;
            break;
#endif
#ifdef _ANSB1
        case AN3:
            _ANSB1 = state;
            _RB1 = 1;
            break;
#endif
#ifdef _ANSB2
        case AN4:
            _ANSB2 = state;
            _RB2 = 1;
            break;
#endif
#ifdef _ANSB3
        case AN5:
            _ANSB3 = state;
            _RB3 = 1;
            break;
#endif
#ifdef _ANSC0
        case AN6:
            _ANSC0 = state;
            _RC0 = 1;
            break;
#endif
#ifdef _ANSC1
        case AN7:
            _ANSC1 = state;
            _RC1 = 1;
            break;
#endif
#ifdef _ANSC2
        case AN8:
            _ANSC2 = state;
            _RC2 = 1;
            break;
#endif
#ifdef _ANSA11
        case AN9:
            _ANSA11 = state;
            _RA11 = 1;
            break;
#endif
#ifdef _ANSA12
        case AN10:
            _ANSA12 = state;
            _RA12 = 1;
            break;
#endif
#ifdef _ANSC11
        case AN11:
            _ANSC11 = state;
            _RC11 = 1;
            break;
#endif
#ifdef _ANSE12
        case AN12:
            _ANSE12 = state;
            _RE12 = 1;
            break;
#endif
#ifdef _ANSE13
        case AN13:
            _ANSE13 = state;
            _RE13 = 1;
            break;
#endif
#ifdef _ANSE14
        case AN14:
            _ANSE14 = state;
            _RE14 = 1;
            break;
#endif
#ifdef _ANSE15
        case AN15:
            _ANSE15 = state;
            _RE15 = 1;
            break;
#endif
        default:
            break;
    }
}