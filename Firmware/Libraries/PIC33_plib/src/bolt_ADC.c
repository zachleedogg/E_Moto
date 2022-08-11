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
#ifdef AD2CON1
#define ADC2
#endif

#define ON 1
#define OFF 0

#define ADC_1_ON() AD1CON1bits.ADON=1
#define ADC_1_OFF() AD1CON1bits.ADON=0

#ifdef ADC2
#define ADC_2_ON() AD2CON1bits.ADON=1
#define ADC_2_OFF() AD2CON1bits.ADON=0
#endif

#define set(x,y) (x |= (y))
#define clear(x,y) (x &= ~(y))

typedef struct _ADC_module_S {
    uint8_t ADC_numberActivePins;
    uint16_t ADC_activePinBits;
} ADC_module_S;

static ADC_module_S ADC_1_Module = {
    .ADC_activePinBits = 0,
    .ADC_numberActivePins = 0,
};

volatile uint16_t * const ADC_1_buffer[16] = {
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

#ifdef ADC2
static ADC_module_S ADC_2_Module = {
    .ADC_activePinBits = 0,
    .ADC_numberActivePins = 0,
};

volatile uint16_t * const ADC_2_buffer[16] = {
    &ADC2BUF0,
    &ADC2BUF1,
    &ADC2BUF2,
    &ADC2BUF3,
    &ADC2BUF4,
    &ADC2BUF5,
    &ADC2BUF6,
    &ADC2BUF7,
    &ADC2BUF8,
    &ADC2BUF9,
    &ADC2BUFA,
    &ADC2BUFB,
    &ADC2BUFC,
    &ADC2BUFD,
    &ADC2BUFE,
    &ADC2BUFF
};
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Function Definitions
// *****************************************************************************
// *****************************************************************************

static void ADC_selectPin(ADC_pinNumber_E thisPin, uint8_t state);

void ADC_Init(void) {

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
    _ADCS = 0x5F; /*ADC Clock Period (TAD = 228ns = (ADCS + 1)/(SYSCLOCK/2)) Should be for 72ns...*/
    _SAMC = 0x1F; /*Sample for t = SAMC*TAD before converting*/

    /*Config Reg 4*/
    AD1CON4 = 0x0000;

    /*Select ANx inputs to scan*/
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;

    /* Assign MUXA inputs */
    AD1CHS0bits.CH0SA = 0; // CH0SA bits ignored for CH0 +ve input selection
    AD1CHS0bits.CH0NA = 0; // Select VREF- for CH0 -ve input

#ifdef ADC2
    /*Config reg 1*/
    // Enable 10-bit mode, auto-sample and auto-conversion
    //_AD12B = 0; /*10 bit operation*/
    //_SSRCG = 0; /*auto-convert*/
    AD2CON1bits.SSRC = 0b111; /*convert on internal timer*/
    AD2CON1bits.ASAM = 1; /*auto sample*/

    /*Config Reg 2*/
    // Sample inputs alternately using channel scanning on CH0
    AD2CON2bits.CSCNA = 1; /*input scanning on channel 0*/
    AD2CON2bits.SMPI = 0; /*number of pins + 1 to read per interrupt*/

    /*Config Reg 3*/
    AD2CON3bits.ADCS = 0x0F; /*ADC Clock Period (TAD = 228ns = (ADCS + 1)/(SYSCLOCK/2)) Should be for 72ns...*/
    AD2CON3bits.SAMC = 0x0F; /*Sample for t = SAMC*TAD before converting*/

    /*Config Reg 4*/
    AD2CON4 = 0x0000;

    /*Select ANx inputs to scan*/
    AD2CSSL = 0x0000;

    /* Assign MUXA inputs */
    AD2CHS0bits.CH0SA = 0; // CH0SA bits ignored for CH0 +ve input selection
    AD2CHS0bits.CH0NA = 0; // Select VREF- for CH0 -ve input
#endif
    /* Do not Enable ADC module or interrupt, wait to add the pins. */
}

uint8_t ADC_SetPin(ADC_pinNumber_E newPin) {
#ifdef ADC2
    if (newPin > 15) {
        uint16_t pinMask = 1 << (newPin - 16);
        if (ADC_1_Module.ADC_activePinBits & pinMask) {
            return 1;
        }
        ADC_1_OFF(); /*Turn off ADC*/
        /*set pin as an analog input*/
        ADC_selectPin(newPin, 1);
        ADC_1_Module.ADC_numberActivePins++; /*add number of inputs*/
        set(ADC_1_Module.ADC_activePinBits, pinMask); /*set new pin to active state*/
        AD1CSSH = ADC_1_Module.ADC_activePinBits; /*add pin to input sweep*/
        AD1CON2bits.SMPI = ADC_1_Module.ADC_numberActivePins - 1; /*Number of pins to sweep*/
        ADC_1_ON();
        return 0;
    } else {
        uint16_t pinMask = 1 << newPin;
        if (ADC_2_Module.ADC_activePinBits & pinMask) {
            return 1;
        }
        ADC_2_OFF(); /*Turn off ADC*/
        /*set pin as an analog input*/
        ADC_selectPin(newPin, 1);
        ADC_2_Module.ADC_numberActivePins++; /*add number of inputs*/
        set(ADC_2_Module.ADC_activePinBits, pinMask); /*set new pin to active state*/
        AD2CSSL = ADC_2_Module.ADC_activePinBits; /*add pin to input sweep*/
        AD2CON2bits.SMPI = ADC_2_Module.ADC_numberActivePins - 1; /*Number of pins to sweep*/
        ADC_2_ON();
        return 0;
    }
#else
    uint16_t pinMask = 1 << newPin;
    if (ADC_1_Module.ADC_activePinBits & pinMask) {
        return 1;
    }
    ADC_1_OFF(); /*Turn off ADC*/
    /*set pin as an analog input*/
    ADC_selectPin(newPin, 1);
    ADC_1_Module.ADC_numberActivePins++; /*add number of inputs*/
    set(ADC_1_Module.ADC_activePinBits, pinMask); /*set new pin to active state*/
    AD1CSSL = ADC_1_Module.ADC_activePinBits; /*add pin to input sweep*/
    AD1CON2bits.SMPI = ADC_1_Module.ADC_numberActivePins - 1; /*Number of pins to sweep*/
    ADC_1_ON();
    return 0;
#endif
}

uint8_t ADC_RemovePin(ADC_pinNumber_E thisPin) {
    uint16_t pinMask = 1 << thisPin;
    if ((ADC_1_Module.ADC_activePinBits & pinMask) == 0) {
        return 1;
    }
    ADC_1_OFF(); /*Turn off ADC*/
    /*disable analog input*/
    ADC_selectPin(thisPin, 0);
    ADC_1_Module.ADC_numberActivePins--; /*subtract number of inputs*/
    clear(ADC_1_Module.ADC_activePinBits, pinMask); /*set new pin to inactive state*/
    AD1CSSL = ADC_1_Module.ADC_activePinBits; /*remove pin from input sweep*/
    if (ADC_1_Module.ADC_numberActivePins == 0) { /*do not enable module if no pins selected*/
        ADC_1_OFF();
    } else {
        AD1CON2bits.SMPI = ADC_1_Module.ADC_numberActivePins - 1; /*Number of pins to sweep*/
        ADC_1_ON();
    }
    return 0;
}

uint16_t ADC_GetValue(ADC_pinNumber_E thisPin) {
#ifdef ADC2
    if (thisPin > 15) {
        thisPin = thisPin - 16;
        uint16_t ADC_activePinBitsBeforeThisPin = 0;
        uint16_t mask = 0x0001;
        /*For each input, read ADC1BUFx value into correct pin buffer*/
        uint8_t i = 0;
        for (i = 0; i < thisPin; i++) {
            if (ADC_1_Module.ADC_activePinBits & (mask << i)) {
                ADC_activePinBitsBeforeThisPin++;
            }
        }
        return *ADC_1_buffer[ADC_activePinBitsBeforeThisPin];
    } else {
        uint16_t ADC_activePinBitsBeforeThisPin = 0;
        uint16_t mask = 0x0001;
        /*For each input, read ADC2BUFx value into correct pin buffer*/
        uint8_t i = 0;
        for (i = 0; i < thisPin; i++) {
            if (ADC_2_Module.ADC_activePinBits & (mask << i)) {
                ADC_activePinBitsBeforeThisPin++;
            }
        }
        return *ADC_2_buffer[ADC_activePinBitsBeforeThisPin];
    }
#else
    uint16_t ADC_activePinBitsBeforeThisPin = 0;
    uint16_t mask = 0x0001;
    /*For each input, read ADC1BUFx value into correct pin buffer*/
    uint8_t i = 0;
    for (i = 0; i < thisPin; i++) {
        if (ADC_1_Module.ADC_activePinBits & (mask << i)) {
            ADC_activePinBitsBeforeThisPin++;
        }
    }
    uint16_t val = *ADC_1_buffer[ADC_activePinBitsBeforeThisPin];
    return val;
#endif
}

static void ADC_selectPin(ADC_pinNumber_E thisPin, uint8_t state) {
    switch (thisPin) {
#if defined(__dsPIC33EP32GP502__) || defined(__dsPIC33EP256MC502__) || defined(__dsPIC33EP64GP506__)

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
            
#elif defined(__dsPIC33EP512MU810__)
#ifdef _ANSB0
        case AN0:
            _ANSB0 = state;
            _TRISB0 = 1;
            break;
#endif
#ifdef _ANSB1
        case AN1:
            _ANSB1 = state;
            _TRISB1 = 1;
            break;
#endif
#ifdef _ANSB2
        case AN2:
            _ANSB2 = state;
            _TRISB2 = 1;
            break;
#endif
#ifdef _ANSB3
        case AN3:
            _ANSB3 = state;
            _TRISB3 = 1;
            break;
#endif
#ifdef _ANSB4
        case AN4:
            _ANSB4 = state;
            _TRISB4 = 1;
            break;
#endif
#ifdef _ANSB5
        case AN5:
            _ANSB5 = state;
            _TRISB5 = 1;
            break;
#endif
#ifdef _ANSB6
        case AN6:
            _ANSB6 = state;
            _TRISB6 = 1;
            break;
#endif
#ifdef _ANSB7
        case AN7:
            _ANSB7 = state;
            _TRISB7 = 1;
            break;
#endif
#ifdef _ANSB8
        case AN8:
            _ANSB8 = state;
            _TRISB8 = 1;
            break;
#endif
#ifdef _ANSB9
        case AN9:
            _ANSB9 = state;
            _TRISB9 = 1;
            break;
#endif
#ifdef _ANSB10
        case AN10:
            _ANSB10 = state;
            _TRISB10 = 1;
            break;
#endif
#ifdef _ANSB11
        case AN11:
            _ANSB11 = state;
            _TRISB11 = 1;
            break;
#endif
#ifdef _ANSB12
        case AN12:
            _ANSB12 = state;
            _TRISB12 = 1;
            break;
#endif
#ifdef _ANSB13
        case AN13:
            _ANSB13 = state;
            _TRISB13 = 1;
            break;
#endif
#ifdef _ANSB14
        case AN14:
            _ANSB14 = state;
            _TRISB14 = 1;
            break;
#endif
#ifdef _ANSB15
        case AN15:
            _ANSB15 = state;
            _TRISB15 = 1;
            break;
#endif
#ifdef _ANSC1
        case AN16:
            _ANSC1 = state;
            _TRISC1 = 1;
            break;
#endif
#ifdef _ANSC2
        case AN17:
            _ANSC2 = state;
            _TRISC2 = 1;
            break;
#endif
#ifdef _ANSC3
        case AN18:
            _ANSC3 = state;
            _TRISC3 = 1;
            break;
#endif
#ifdef _ANSC4
        case AN19:
            _ANSC4 = state;
            _TRISC4 = 1;
            break;
#endif
#ifdef _ANSA7
        case AN23:
            _ANSA7 = state;
            _TRISA7 = 1;
            break;
#endif
#else
#error No Processor defined in list above. Ensure you add the micro you want because all analog pins are different.
#endif
        default:
            break;
    }
}