/* 
 * File:   main.c
 * Author: kid group
 *
 * Created on September 1, 2016, 8:04 AM
 */

#include <stdio.h>
#include <stdint.h>
#include "bolt_init.h"
#include "bolt_uart.h"
#include "pins.h"
#include "ping.h"

#define TEST_LED IO_PIN_RB15
#define DEFINES_PING_LEFT_TRIGGER (PINS_pin_S){PIN_PORTD,8}
#define DEFINES_PING_LEFT_ECHO RPI46_IC
#define DEFINES_PING_RIGHT_TRIGGER (PINS_pin_S){PIN_PORTG,6}
#define DEFINES_PING_RIGHT_ECHO RPI47_IC

#define TIMER1_TIME 10

static uint16_t count = 0;
static uint8_t flag = 0;

static uint16_t count2 = 0;
static uint8_t flag2 = 0;

static const char helloString[] = "EAT MY SHORTS";

/*
 * 
 */
int main(void) {

    Micro_Init();
    clockInit(FREQ_120MHZ, EXTERNAL);
    Uart1Init(UART_TX_RP36, UART_RX_RP20, UART_BAUD_230400);

    Uart1Write("Hello, welcome to the PING sensor program\n\n");

    T1_Interrupt_Init(TIMER1_TIME, 3);
    ping_Init(DEFINES_PING_RIGHT_ECHO, DEFINES_PING_RIGHT_TRIGGER, DEFINES_PING_LEFT_ECHO, DEFINES_PING_LEFT_TRIGGER);

    while (1) {

        if (flag) {
            flag = 0;
        }

        if (flag2) {
            flag2 = 0;
            ping_Run();
            uint8_t disR = PING_getDistance(0);
            uint8_t disL = PING_getDistance(1);

            char string[50];
            sprintf(string, "LEFT: %d    RIGHT: %d\n", disL, disR);
            Uart1Write(string);

        }


    }
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    /* clear timer x interrupt */
    _T1IF = 0;

    /*Put your code here*/
    count++;
    count2++;

    if (count2 == 1000/TIMER1_TIME) {

        count2 = 0;
        flag2 = 1;
    }

    if (count == 100) {
        flag = 1;
        count = 0;
    }
}

//void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void) {
//    /*clear the interrupt*/
//    _IC1IF = 0;
//
//    uint16_t temp1 = 0;
//    uint16_t temp2 = 0;
//
//    switch (ic_stat) {
//        case RESLEASED:
//
//            ic_count = 0;
//            /*clear the 0x0000 reading*/
//
//            //while (IC1CON1bits.ICBNE || IC2CON1bits.ICBNE) {
//            temp1 = IC1BUF;
//            temp1 = IC2BUF;
//            //}
//            //char string[10];
//            //sprintf(string, "%u\n", temp);
//            //Uart1Write("\nTstart --- ");
//            //Uart1Write(string);
//            ic_stat = RUNNING;
//            
//            IC1CON2bits.TRIGSTAT = 1; /*Release the ICx Timer from reset*/
//            IC2CON2bits.TRIGSTAT = 1;
//            break;
//        case RUNNING:
//            //TFT_LCD_writeString(numbers[1], 100, 200, TFT_LCD_RED, TFT_LCD_CYAN);
//            /*get the rise time*/
//            //while (IC1CON1bits.ICBNE) {
//            temp1 = IC1BUF;
//            temp2 = IC2BUF;
//            //}
//            uint32_t time = ((uint32_t) temp1 | ((uint32_t) temp2 << 16));
//            uint32_t dist = time;
//            dist = dist / 40;
//            dist = dist / 148;
//            if (dist > 150) {
//                ic_ignoreNextVal = 1;
//                Uart1Write("INF\n");
//            } else if (ic_ignoreNextVal == 1) {
//                ic_ignoreNextVal = 0;
//                Uart1Write("INF\n");
//            } else {
//                char string[50];
//                sprintf(string, "dist:%lu\n", dist);
//                Uart1Write(string);
//            }
//            ic_stat = DONE;
//            IC1CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
//            IC2CON1bits.ICM = 0b000; /*Reset ICx+1 Timer*/
//            IC2CON1bits.ICM = 0b001; 
//            break;
//        default:
//            while (IC1CON1bits.ICBNE || IC2CON1bits.ICBNE) {
//                temp1 = IC1BUF;
//                temp1 = IC2BUF;
//            }
//            break;
//    }
//}