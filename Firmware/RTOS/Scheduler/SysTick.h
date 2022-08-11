#ifndef SYS_TICK_H
#define SYS_TICK_H

#include <stdint.h>

typedef struct {
    uint32_t start_time;
    uint32_t end_value;
} SysTick_Timer_S;

typedef SysTick_Timer_S *SysTick_Timer;


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


/**
 * SysTick_Timer will create a timer vaiable with a given name.
 * @param myTimer name of variable
 */
#define SysTick_Timer_new(x) static SysTick_Timer_S x##_S;\
static SysTick_Timer x = &x##_S

/**
 * @brief SysTick_TimerStart will grab the current time and record the time
 * @param time
 * @param timer stuct 
 */
void SysTick_TimerStart(uint32_t time, SysTick_Timer timer);

/**
 * @brief SysTick_TimeOut will check if the timer is expired
 * @param timer
 * @return 
 */
uint8_t SysTick_TimeOut(SysTick_Timer timer);

#endif