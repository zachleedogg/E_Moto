/* 
 * File:   main.c
 * Author: Zach Levenberg
 *
 * Created on June 15, 2016, 5:07 PM
 */

#include <stdio.h>

#include "Defines.h"
#include "framework.h"

/*
 * 
 */
int main(void) {
    Micro_Init();
    clockInit(FREQ_120MHZ, EXTERNAL);

    Uart1Init(DEFINES_UART_TX, DEFINES_UART_RX, DEFINES_UART_BAUD);

    Uart1Write("\n\n\nhello this is the test program for the Template Project\n");

    Init();

    Timer_Init(clockFreq());
    //scheduler_init();

    Run();

    return 0;
}

