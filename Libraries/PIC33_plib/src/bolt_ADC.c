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


// *****************************************************************************
// *****************************************************************************
// Section: Module Defines and variables
// *****************************************************************************
// *****************************************************************************

#define ADC_ON() AD1CON1bits.ADON=1
#define ADC_OFF() AD1CON1bits.ADON=0

uint8_t uartFlag;

typedef struct _ADCmodule {
    uint8_t numberOfInputs;
    uint8_t pinList[NUMBER_OF_ADC_PINS];
    uint16_t buffer[NUMBER_OF_ADC_PINS];
} ADCmodule;

ADCmodule thisADC;

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************


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
    //_AD1IP = 4; /*Interrupt Priority*/
    //_AD1IE = 1;
    //AD1CON1bits.ADON = 1;
}

void __attribute__((__interrupt__, __auto_psv__)) _AD1Interrupt(void) {

    _AD1IF = 0; // Clear conversion done status bit

    uint8_t currentInput = 0;
    /*For each input, read ADC1BUFx value into correct pin buffer*/
    uint8_t i = 0;
    for (i = 0; i < thisADC.numberOfInputs; i++) {
        /*search for pins that are active*/
        for (; currentInput < NUMBER_OF_ADC_PINS; currentInput++) {
            if (thisADC.pinList[currentInput]) {
                switch (i) {
                    case 0:
                        thisADC.buffer[currentInput] = ADC1BUF0;
                        break;
                    case 1:
                        thisADC.buffer[currentInput] = ADC1BUF1;
                        break;
                    case 2:
                        thisADC.buffer[currentInput] = ADC1BUF2;
                        break;
                    case 3:
                        thisADC.buffer[currentInput] = ADC1BUF3;
                        break;
                    case 4:
                        thisADC.buffer[currentInput] = ADC1BUF4;
                        break;
                    case 5:
                        thisADC.buffer[currentInput] = ADC1BUF5;
                        break;
                    case 6:
                        thisADC.buffer[currentInput] = ADC1BUF6;
                        break;
                    case 7:
                        thisADC.buffer[currentInput] = ADC1BUF7;
                        break;
                    case 8:
                        thisADC.buffer[currentInput] = ADC1BUF8;
                        break;
                    default:
                        break;
                }
                currentInput++;
                break;
            }
        }
    }
    uartFlag = 1;
}

uint8_t ADC_SetPin(adc_pin_number newPin) {
    ADC_OFF(); /*Turn off ADC*/
    /*set pin as an analog input*/
    switch (newPin) {
        case AN0:
            _ANSA0 = 1;
            _RA0 = 1;
            break;
        case AN1:
            _ANSA1 = 1;
            _RA1 = 1;
            break;
        case AN2:
            _ANSB0 = 1;
            _RB0 = 1;
            break;
        case AN3:
            _ANSB1 = 1;
            _RB1 = 1;
            break;
        case AN4:
            _ANSB2 = 1;
            _RB2 = 1;
            break;
        case AN5:
            _ANSB3 = 1;
            _RB3 = 1;
            break;
#ifdef ANSELC
        case AN6:
            _ANSC0 = 1;
            _RC0 = 1;
            break;
        case AN7:
            _ANSC1 = 1;
            _RC1 = 1;
            break;
        case AN8:
            _ANSC2 = 1;
            _RC2 = 1;
            break;
#endif
        default:
            break;
    }
    thisADC.numberOfInputs++; /*add number of inputs*/
    thisADC.pinList[newPin] = 1; /*set new pin to active state*/
    AD1CSSL |= (1 << newPin); /*add pin to input sweep*/
    AD1CON2bits.SMPI = thisADC.numberOfInputs - 1; /*Number of pins to sweep*/
    //_AD1IE = 1;
    ADC_ON();
    return 0;
}

uint16_t ADC_GetValue(adc_pin_number thisPin) {
       uint8_t currentInput = 0;
    /*For each input, read ADC1BUFx value into correct pin buffer*/
    uint8_t i = 0;
    for (i = 0; i < thisADC.numberOfInputs; i++) {
        /*search for pins that are active*/
        for (; currentInput < NUMBER_OF_ADC_PINS; currentInput++) {
            if (thisADC.pinList[currentInput]) {
                switch (i) {
                    case 0:
                        thisADC.buffer[currentInput] = ADC1BUF0;
                        break;
                    case 1:
                        thisADC.buffer[currentInput] = ADC1BUF1;
                        break;
                    case 2:
                        thisADC.buffer[currentInput] = ADC1BUF2;
                        break;
                    case 3:
                        thisADC.buffer[currentInput] = ADC1BUF3;
                        break;
                    case 4:
                        thisADC.buffer[currentInput] = ADC1BUF4;
                        break;
                    case 5:
                        thisADC.buffer[currentInput] = ADC1BUF5;
                        break;
                    case 6:
                        thisADC.buffer[currentInput] = ADC1BUF6;
                        break;
                    case 7:
                        thisADC.buffer[currentInput] = ADC1BUF7;
                        break;
                    case 8:
                        thisADC.buffer[currentInput] = ADC1BUF8;
                        break;
                    default:
                        break;
                }
                currentInput++;
                break;
            }
        }
    }
    uartFlag = 1;
    return thisADC.buffer[thisPin];
}
