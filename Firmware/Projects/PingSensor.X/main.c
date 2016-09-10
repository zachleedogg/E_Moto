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
#include "bolt_pins.h"
#include "ic.h"

#define TEST_LED IO_PIN_RB15
#define PING_TRIGGER IO_PIN_RB14
static uint8_t ic_stat = 0;
static uint16_t ic_time = 0;
static uint8_t ic_count = 0;



static uint16_t count = 0;
static uint8_t flag = 0;

static uint16_t count2 = 0;
static uint8_t flag2 = 0;




static const char helloString[] = "EAT MY SHORTS";

static const char* numbers[10];

/*
 * 
 */
int main(void) {

    Micro_Init();
    clockInit(FREQ_80MHZ, EXTERNAL);
    IO_setPinDir(TEST_LED, OUTPUT);
    IO_setPinDir(PING_TRIGGER, OUTPUT);
    IO_pinWrite(PING_TRIGGER, LOW);
    Uart1Init(RP36_TX, RP20_UART_RX, BAUD115200);

    Uart1Write("Hello, welcome to the PING sensor program\n\n");

    T1_Interrupt_Init(1, 3);



    _IC1R = 0x2D;
    //    IC1CON1bits.ICTSEL = 0b111; /*Use peripheral clock source*/
    //    IC1CON1bits.ICM = 0b001; /*Capture on every rising and falling edge*/
    //    IC1CON2bits.SYNCSEL = 0b00000; /*Input Source is Software*/
    //    IC1CON2bits.ICTRIG = 0b00; /*no input trigger trigger*/
    //    IC1CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
    //    _IC1IP = 4;
    //    _IC1IE = 1;
    //    IFS0bits.IC1IF = 0;           // Clear the IC1 interrupt status flag



    while (IC1CON1bits.ICBNE) { //clear buffer
        uint16_t temp = IC1BUF;
    }
    while (IC2CON1bits.ICBNE) { //clear buffer
        uint16_t temp = IC2BUF;
    }



    IC2CON1bits.ICSIDL = 0; // Input capture will continue to operate in CPU idle mode
    IC2CON1bits.ICTSEL = 0b111; // Peripheral (FP) is the clock source for the IC1 module
    IC2CON1bits.ICI = 0; // Interrupt on every capture event
    IC2CON2bits.IC32 = 1; // Cascade module operation is disabled
    IC2CON2bits.ICTRIG = 1; // Input source used to synchronize the input capture timer of  
    IC2CON2bits.SYNCSEL = 0; // No Sync or Trigger source for the IC1 module
    IC2CON2bits.TRIGSTAT = 0; // IC1TMR has not been triggered and is being held clear


    IC1CON1bits.ICSIDL = 0; // Input capture will continue to operate in CPU idle mode
    IC1CON1bits.ICTSEL = 0b111; // Peripheral (FP) is the clock source for the IC1 module
    IC1CON1bits.ICI = 0; // Interrupt on every capture event
    IC1CON2bits.IC32 = 1; // Cascade module operation is disabled
    IC1CON2bits.ICTRIG = 1; // Input source used to synchronize the input capture timer of  
    IC1CON2bits.SYNCSEL = 0; // No Sync or Trigger source for the IC1 module
    IC1CON2bits.TRIGSTAT = 0; // IC1TMR has not been triggered and is being held clear


    IEC0bits.IC1IE = 1; // Enable IC1 interrupts
    IFS0bits.IC1IF = 0; // Clear Flag
    IPC0bits.IC1IP = 1; // Set module interrupt priority as 1

    IC2CON1bits.ICM = 0b001; // Enable Capture Mode on every edge
    IC1CON1bits.ICM = 0b001; // Enable Capture Mode on every edge



    while (1) {

        if (flag) {
            flag = 0;

        }

        if (flag2) {
            flag2 = 0;

        }


    }
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {

    /*Put your code here*/


    count++;
    count2++;
    if (count2 == 10) {
        Uart1Write("Set Pin HIGH\n");
        /*Make pin go Hi-Z*/
        //IO_setPinDir(PING_TRIGGER, INPUT);
        IO_pinWrite(PING_TRIGGER, HIGH);
    }
    if (count2 == 20) {
        Uart1Write("Pull Pin LOW\n");
        /*Pull Pin Low*/
        //IO_setPinDir(PING_TRIGGER, OUTPUT);
        IO_pinWrite(PING_TRIGGER, LOW);
        ic_stat = IDLE;
    }
    if (count2 == 1000) {
        IO_pinToggle(TEST_LED);
        count2 = 0;
        flag2 = 1;
    }
    if (count == 1) {
        flag = 1;
        count = 0;
    }

    /* clear timer x interrupt */
    _T1IF = 0;
}

void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void) {
    /*clear the interrupt*/
    _IC1IF = 0;



    switch (ic_stat) {
        case IDLE:

            ic_count = 0;
            /*clear the 0x0000 reading*/
            uint16_t temp = 0;
            //while (IC1CON1bits.ICBNE || IC2CON1bits.ICBNE) {
            temp = IC1BUF;
            temp = IC2BUF;
            //}
            char string[10];
            //sprintf(string, "%u\n", temp);
            Uart1Write("\nTstart --- ");
            //Uart1Write(string);
            ic_stat = RUNNING;
            IC2CON1bits.ICM = 0b001; // Enable Capture Mode on every edge
            IC1CON2bits.TRIGSTAT = 1; /*Release the ICx Timer from reset*/
            IC2CON2bits.TRIGSTAT = 1;
            break;
        case RUNNING:
            //TFT_LCD_writeString(numbers[1], 100, 200, TFT_LCD_RED, TFT_LCD_CYAN);
            /*get the rise time*/
        {
            uint16_t temp, temp2;
            //while (IC1CON1bits.ICBNE) {
            temp = IC1BUF;
            temp2 = IC2BUF;
            //}
            uint32_t time = (temp | ((uint32_t) temp2 << 16));
            uint32_t dist = time;
            dist = dist / 40;
            dist = dist / 148;
            char string[50];
            sprintf(string, "\n1: %u\n2: %u\n%lu\nDist: %lu\n", temp, temp2, time, dist);
            Uart1Write("Tend --- ");
            Uart1Write(string);
            ic_stat = IDLE;
            IC1CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
            IC2CON2bits.TRIGSTAT = 0;
            IC2CON1bits.ICM = 0b000; // Enable Capture Mode on every edge
        }
            break;
        default:
            break;
    }
}