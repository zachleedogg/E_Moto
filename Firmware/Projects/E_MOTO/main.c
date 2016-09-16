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

    Uart1Init(RP36_TX, RP20_UART_RX, BAUD115200);

    Uart1Write("\n\n\nhello this is the test program for the Template Project\n");

    Init();

    Timer_Init(clockFreq());
    scheduler_init();

    Run();

    return 0;
}

