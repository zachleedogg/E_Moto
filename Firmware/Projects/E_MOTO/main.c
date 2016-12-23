/* 
 * File:   main.c
 * Author: Zach Levenberg
 *
 * Created on June 15, 2016, 5:07 PM
 */


#include "Defines.h"
#include "framework.h"

/*
 * 
 */
int main(void) {
    Micro_Init();
    clockInit(DEFINES_SYSTEM_CLOCK_FREQ, EXTERNAL);

    Uart1Init(DEFINES_UART_TX, DEFINES_UART_RX, DEFINES_UART_BAUD);
    Uart1Write("\nE_MOTO TEST\n\n");



    FRAMEWORK_run(clockFreq());

    return 0;
}

