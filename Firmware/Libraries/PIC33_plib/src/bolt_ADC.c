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

typedef struct _ADC_module_S {
    uint8_t ADC_numberActivePins;
    uint16_t ADC_activePinBits;
} ADC_module_S;

static ADC_module_S ADC_thisModule = {
    .ADC_activePinBits = 0,
    .ADC_numberActivePins = 0,
};

volatile uint16_t * const ADC_buffer[16] = {
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

static uint8_t isInitialized = 0;
// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

static void ADC_selectPin(ADC_pinNumber_E thisPin, uint8_t state);

void ADC_Init(void) {
    /* Initialize ADC module only once*/
    if (isInitialized == 0) {
        isInitialized = 1;
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
        _ADCS = 0x0F; /*ADC Clock Period (TAD = 228ns = (ADCS + 1)/(SYSCLOCK/2)) Should be for 72ns...*/
        _SAMC = 0x0F; /*Sample for t = SAMC*TAD before converting*/

        /*Config Reg 4*/
        AD1CON4 = 0x0000;

        /*Select ANx inputs to scan*/
        AD1CSSH = 0x0000;
        AD1CSSL = 0x0000;

        /* Assign MUXA inputs */
        AD1CHS0bits.CH0SA = 0; // CH0SA bits ignored for CH0 +ve input selection
        AD1CHS0bits.CH0NA = 0; // Select VREF- for CH0 -ve input
    }

    /* Do not Enable ADC module or interrupt, wait to add the pins. */
}

uint8_t ADC_SetPin(ADC_pinNumber_E newPin) {
    uint16_t pinMask = 1 << newPin;
    if (ADC_thisModule.ADC_activePinBits & pinMask) {
        return 1;
    }
    ADC_OFF(); /*Turn off ADC*/
    /*set pin as an analog input*/
    ADC_selectPin(newPin, 1);
    ADC_thisModule.ADC_numberActivePins++; /*add number of inputs*/
    set(ADC_thisModule.ADC_activePinBits, pinMask); /*set new pin to active state*/
    AD1CSSL = ADC_thisModule.ADC_activePinBits; /*add pin to input sweep*/
    AD1CON2bits.SMPI = ADC_thisModule.ADC_numberActivePins - 1; /*Number of pins to sweep*/
    ADC_ON();
    return 0;
}

uint8_t ADC_RemovePin(ADC_pinNumber_E thisPin) {
    uint16_t pinMask = 1 << thisPin;
    if ((ADC_thisModule.ADC_activePinBits & pinMask) == 0) {
        return 1;
    }
    ADC_OFF(); /*Turn off ADC*/
    /*disable analog input*/
    ADC_selectPin(thisPin, 0);
    ADC_thisModule.ADC_numberActivePins--; /*subtract number of inputs*/
    clear(ADC_thisModule.ADC_activePinBits, pinMask); /*set new pin to inactive state*/
    AD1CSSL = ADC_thisModule.ADC_activePinBits; /*remove pin from input sweep*/
    if (ADC_thisModule.ADC_numberActivePins == 0) { /*do not enable module if no pins selected*/
        ADC_OFF();
    } else {
        AD1CON2bits.SMPI = ADC_thisModule.ADC_numberActivePins - 1; /*Number of pins to sweep*/
        ADC_ON();
    }
    return 0;
}

uint16_t ADC_GetValue(ADC_pinNumber_E thisPin) {
    uint16_t ADC_activePinBitsBeforeThisPin = 0;
    uint16_t mask = 0x0001;
    /*For each input, read ADC1BUFx value into correct pin buffer*/
    uint8_t i = 0;
    for (i = 0; i < thisPin; i++) {
        if (ADC_thisModule.ADC_activePinBits & (mask << i)) {
            ADC_activePinBitsBeforeThisPin++;
        }
    }
    //    char strgg[256] = {};
    //    sprintf(strgg, "adc %d val %d\n", ADC_activePinBitsBeforeThisPin, *ADC_buffer[ADC_activePinBitsBeforeThisPin]);
    //    Uart1Write(strgg);
    return *ADC_buffer[ADC_activePinBitsBeforeThisPin];
}

static void ADC_selectPin(ADC_pinNumber_E thisPin, uint8_t state) {
    switch (thisPin) {
#ifdef _ANSA0
        case AN0:
            _ANSA0 = state;
            _TRISA0 = 1;
            break;
#endif
#ifdef _ANSA1
        case AN1:
            _ANSA1 = state;
            _TRISA1 = 1;
            break;
#endif
#ifdef _ANSB0
        case AN2:
            _ANSB0 = state;
            _TRISB0 = 1;
            break;
#endif
#ifdef _ANSB1
        case AN3:
            _ANSB1 = state;
            _TRISB1 = 1;
            break;
#endif
#ifdef _ANSB2
        case AN4:
            _ANSB2 = state;
            _TRISB2 = 1;
            break;
#endif
#ifdef _ANSB3
        case AN5:
            _ANSB3 = state;
            _TRISB3 = 1;
            break;
#endif
#ifdef _ANSC0
        case AN6:
            _ANSC0 = state;
            _TRISC0 = 1;
            break;
#endif
#ifdef _ANSC1
        case AN7:
            _ANSC1 = state;
            _TRISC1 = 1;
            break;
#endif
#ifdef _ANSC2
        case AN8:
            _ANSC2 = state;
            _TRISC2 = 1;
            break;
#endif
#ifdef _ANSA11
        case AN9:
            _ANSA11 = state;
            _TRISA11 = 1;
            break;
#endif
#ifdef _ANSA12
        case AN10:
            _ANSA12 = state;
            _TRISA12 = 1;
            break;
#endif
#ifdef _ANSC11
        case AN11:
            _ANSC11 = state;
            _TRISC11 = 1;
            break;
#endif
#ifdef _ANSE12
        case AN12:
            _ANSE12 = state;
            _TRISE12 = 1;
            break;
#endif
#ifdef _ANSE13
        case AN13:
            _ANSE13 = state;
            _TRISE13 = 1;
            break;
#endif
#ifdef _ANSE14
        case AN14:
            _ANSE14 = state;
            _TRISE14 = 1;
            break;
#endif
#ifdef _ANSE15
        case AN15:
            _ANSE15 = state;
            _TRISE15 = 1;
            break;
#endif
        default:
            break;
    }
}