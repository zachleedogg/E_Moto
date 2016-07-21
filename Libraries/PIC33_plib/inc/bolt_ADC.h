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


typedef enum _adc_pin_number {
    AN0,
    AN1,
    AN2,
    AN3,
    AN4,
    AN5,
#ifdef ANSELC
    AN6,
    AN7,
    AN8,
#endif
    NUMBER_OF_ADC_PINS,
} adc_pin_number;

/**
 * Initialize the module
 */
void ADC_Init(void);

/**
 * Set pin as an anolog input
 * @param newPin from the enum list above
 * @return success or failure
 */
uint8_t ADC_SetPin(adc_pin_number newPin);

/**
 * Get the Converted Analog value
 * @param thisPin from the assigned pins
 * @return The 10 bit value
 */
uint16_t ADC_GetValue(adc_pin_number thisPin);

#endif	/* BOLT_ADC_H */

