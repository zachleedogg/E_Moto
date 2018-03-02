/* 
 * File:   bolt_ADC.h
 * Author: kid group
 *
 * Created on March 14, 2016, 12:22 AM
 */

#ifndef BOLT_ADC_H
#define	BOLT_ADC_H

#include <xc.h>
#include <stdint.h>

typedef enum _ADC_pinNumber_E {
    AN0,
    AN1,
    AN2,
    AN3,
    AN4,
    AN5,
    AN6,
    AN7,
    AN8,
    AN9,
    AN10,
    AN11,
    AN12,
    AN13,
    AN14,
    AN15,
    AN16,
    AN17,
    AN18,
    AN19,
    AN20,
    AN21,
    AN22,
    AN23,
    AN24,
    AN25,
    AN26,
    AN27,
    AN28,
    AN29,
    AN30,
    AN31,
    NUMBER_OF_ADC_PINS,
} ADC_pinNumber_E;

/**
 * Initialize the module
 */
void ADC_Init(void);

/**
 * Set pin as an anolog input
 * @param newPin from the enum list above
 * @return success or failure
 */
uint8_t ADC_SetPin(ADC_pinNumber_E newPin);

uint8_t ADC_RemovePin(ADC_pinNumber_E newPin);

/**
 * Get the Converted Analog value
 * @param thisPin from the assigned pins
 * @return The 10 bit value
 */
uint16_t ADC_GetValue(ADC_pinNumber_E thisPin);

#endif	/* BOLT_ADC_H */

