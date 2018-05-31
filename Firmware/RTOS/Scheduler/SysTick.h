#ifndef SYS_TICK_H
#define SYS_TICK_H

#include <stdint.h>


/**
 * @brief SysTick_Init will start the system clock with interupt
 * @param sysClock is the system oscillator frequency
 */
void SysTick_Init(uint32_t sysClock);

/**
 * @brief Stops the Systick Hardware Timer
 */
void SysTick_Stop(void);

/**
 * @brief Resumes the Systick Hardware Timer
 */
void SysTick_Resume(void);

/**
 * @brief SystTick_Get will return the tick value 
 * @return current value of valiable "Tick"
 */
uint32_t SysTick_Get(void);

#endif