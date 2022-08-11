#include "SysTick.h"
#include <stdint.h>
#include <xc.h>

static uint32_t volatile Tick = 0;

uint32_t SysTick_Get(void) {
    return Tick;
}

void SysTick_Init(uint32_t sysClock) {
    uint32_t TicksPerMS = ((sysClock / 2) / 1000);
    uint8_t preScaler = 0;

    if (TicksPerMS > 0x0000FFFF) {
        preScaler = 1;
        TicksPerMS = (TicksPerMS / 8);
    }

    /* Initialize timer */
    T5CONbits.TON = 0;
    T5CON = 0; /* Clear timer config register */
    T5CONbits.TCKPS = preScaler; /* prescaler set to 0 */
    TMR5 = 0x00; /*Clear Timers*/
    PR5 = (uint16_t) TicksPerMS; /*set timer period */

    /* Enable level 1-7 interrupts */
    /* No restoring of previous CPU IPL state performed here */
    INTCON2bits.GIE = 1;

    /* set timer interrupt priority */
    _T5IP = 7;
    /* reset timer interrupt */
    _T5IF = 0;
    /* Enable timer interrupt */
    _T5IE = 1;

    /* Turn timer on */
    T5CONbits.TON = 1;
}

void SysTick_Stop(void){
    /* Turn timer off */
    T5CONbits.TON = 0;
}

void SysTick_Resume(void){
    /* Turn timer on */
    T5CONbits.TON = 1;
}

void __attribute__((__interrupt__, __auto_psv__, __shadow__)) _T5Interrupt(void) {
    /* clear timer interrupt Flag*/
    _T5IF = 0;

    /*Put your code here*/
    Tick++;
}