/* 
 * File:   Init.h
 * Author: kid group
 *
 * Created on June 11, 2016, 8:03 PM
 */

#include <xc.h>
#include <stdint.h>

#ifndef INIT_H
#define	INIT_H
/**************************************************************************
 *Set the internal or external clock these clocks*/
#define INTERNAL 0
#define EXTERNAL 1

//Internal Clock Only
#define FREQ_32KHZ 31250
#define FREQ_125KHZ 125000
#define FREQ_250KHZ 250000
#define FREQ_500KHZ 500000
#define FREQ_1MHZ 1000000
#define FREQ_2MHZ 2000000
#define FREQ_4MHZ 4000000

//Internal/External
#define FREQ_8MHZ 8000000
#define FREQ_16MHZ 16000000
#define FREQ_32MHZ 32000000
#define FREQ_64MHZ 64000000
#define FREQ_68MHZ 68000000
#define FREQ_70MHZ 70000000
#define FREQ_80MHZ 80000000

/**
 * Initializes Micro and Pins
 */
void Micro_Init(void);

/**
 * Sets the clock speed
 * @param clockSpeed: Use #defines Above for standard Frequencies
 * @param source: INTERNAL or EXTERNAL
 * @return 
 */
uint16_t clockInit(uint32_t clockSpeed, uint8_t source);

/**
 * Gets the clock frequency
 * @return the frequency in Hz
 */
uint32_t clockFreq();


/************************************************************
 TIMER ISRs
 ************************************************************/

/*Replace x with timer number */

//void __attribute__((__interrupt__,__auto_psv__)) _TxInterrupt(void){    
//
//    /*Put your code here*/
//    
//    /* clear timer x interrupt */
//    _TxIF = 0;
//}

/**
 * Timer inerupt Routine
 * @param period in milliseconds
 * @param priority 0 = lowest (off) 7 = highest 
 * @return 
 */
uint8_t T1_Interrupt_Init(uint16_t period, uint8_t priority);

/**
 * Disables the Timer
 */
void T1_Interrupt_Disable(void);

uint8_t T2_Interrupt_Init(uint16_t period, uint8_t priority);

void T2_Interrupt_Disable(void);

uint8_t T3_Interrupt_Init(uint16_t period, uint8_t priority);

void T3_Interrupt_Disable(void);

uint8_t T4_Interrupt_Init(uint16_t period, uint8_t priority);

void T4_Interrupt_Disable(void);



#endif	/* INIT_H */

