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
    clockInit(FREQ_80MHZ, EXTERNAL);
    FRAMEWORK_timerInit(clockFreq());
    //FRAMEWORK_schedulerInit();
    Uart1Init(UART_TX_RP36, UART_RX_RP20, UART_BAUD_115200);
    
    printf("\n\n\nhello this is the test program for the Template Project\n");
        
    FRAMEWORK_init();
    
    FRAMEWORK_run();

    return 0;
}

