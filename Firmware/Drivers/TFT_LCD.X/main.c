/* 
 * File:   main.c
 * Author: kid group
 *
 * Created on September 1, 2016, 8:04 AM
 */

#include <stdio.h>
#include <stdint.h>
#include "bolt_init.h"
#include "TFT_LCD_2.h"
#include "TFT_TOUCH.h"
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

static touchData myScreenPosition;


static const char helloString[] = "EAT MY SHORTS";

static const char* numbers[10];

/*
 * 
 */
int main(void) {

    Micro_Init();
    clockInit(FREQ_80MHZ, INTERNAL);
    TFT_LCD_INIT(IO_PIN_RB12, IO_PIN_RB10, IO_PIN_RB11);

    T1_Interrupt_Init(1, 3);

    ADC_Init();



    TFT_TOUCH_INIT(IO_PIN_RA0, IO_PIN_RB0, IO_PIN_RA1, IO_PIN_RB1, AN3, AN2);

    _IC1R = 0x2D;
    //    IC1CON1bits.ICTSEL = 0b111; /*Use peripheral clock source*/
    //    IC1CON1bits.ICM = 0b001; /*Capture on every rising and falling edge*/
    //    IC1CON2bits.SYNCSEL = 0b00000; /*Input Source is Software*/
    //    IC1CON2bits.ICTRIG = 0b00; /*no input trigger trigger*/
    //    IC1CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
    //    _IC1IP = 4;
    //    _IC1IE = 1;
    //    IFS0bits.IC1IF = 0;           // Clear the IC1 interrupt status flag
    IEC0bits.IC1IE = 1; // Enable IC1 interrupts
    IPC0bits.IC1IP = 1; // Set module interrupt priority as 1
    IC1CON1bits.ICSIDL = 0; // Input capture will continue to operate in CPU idle mode
    IC1CON1bits.ICTSEL = 0b111; // Peripheral (FP) is the clock source for the IC1 module
    IC1CON1bits.ICI = 0; // Interrupt on every capture event
    IC1CON1bits.ICBNE = 0; // Input capture is empty
    IC1CON1bits.ICM = 0b001; // Capture mode; every fourth rising edge (Prescaler Capture mode)
    IC1CON2bits.IC32 = 0; // Cascade module operation is disabled
    IC1CON2bits.ICTRIG = 0; // Input source used to synchronize the input capture timer of    
    //    another    module    (Synchronization    mode)
    IC1CON2bits.TRIGSTAT = 0; // IC1TMR has not been triggered and is being held clear
    IC1CON2bits.SYNCSEL = 0; // No Sync or Trigger source for the IC1 modul
    IO_setPinDir(TEST_LED, OUTPUT);
    IO_setPinDir(PING_TRIGGER, OUTPUT);
    IO_pinWrite(PING_TRIGGER, LOW);
    while (IC1CON1bits.ICBNE) {
        uint16_t temp = IC1BUF;
    }
    TFT_LCD_fillBackground(TFT_LCD_BLACK);

    numbers[0] = "0";
    numbers[1] = "1";
    numbers[2] = "2";
    numbers[3] = "3";
    numbers[4] = "4";
    numbers[5] = "5";
    numbers[6] = "6";
    numbers[7] = "7";
    numbers[8] = "8";
    numbers[9] = "9";




    TFT_LCD_drawRect(50, 50, TFT_LCD_WIDTH - 50, TFT_LCD_HEIGHT - 50, TFT_LCD_RED);

    TFT_LCD_writeString(helloString, 100, 100, TFT_LCD_RED, TFT_LCD_CYAN);
    TFT_LCD_writeString(numbers[0], 100, 150, TFT_LCD_RED, TFT_LCD_CYAN);
    TFT_LCD_writeString(numbers[1], 100, 175, TFT_LCD_RED, TFT_LCD_CYAN);


    while (1) {

        if (flag2) {
            flag2 = 0;

        }

        if (flag) {
            flag = 0;
            if (myScreenPosition.status == TOUCHING) {
                TFT_LCD_drawRect(myScreenPosition.xPos, myScreenPosition.yPos,
                        myScreenPosition.xPos + 3, myScreenPosition.yPos + 3, TFT_LCD_GREEN);
            }

            if (myScreenPosition.yPos > 450) {
                asm("RESET");
            }
        }
    }
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {

    /*Put your code here*/


    count++;
    count2++;
    if (count2 == 10) {
        /*Make pin go Hi-Z*/
        //IO_setPinDir(PING_TRIGGER, INPUT);
        IO_pinWrite(PING_TRIGGER, HIGH);
    }
    if (count2 == 11) {
        /*Pull Pin Low*/
        //IO_setPinDir(PING_TRIGGER, OUTPUT);
        IO_pinWrite(PING_TRIGGER, LOW);
    }
    if (count2 == 1000) {
        IO_pinToggle(TEST_LED);
        count2 = 0;
        flag2 = 1;

    }
    if (count == 1) {
        myScreenPosition = TFT_TOUCH_run();
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
            TFT_LCD_writeString(numbers[ic_count++], 100, 200, TFT_LCD_RED, TFT_LCD_CYAN);
            if (ic_count == 10) {
                ic_count = 0;
            }

            //IC1CON2bits.TRIGSTAT = 1; /*Release the ICx Timer from reset*/
            /*clear the 0x0000 reading*/
            while (IC1CON1bits.ICBNE) {
                uint16_t temp = IC1BUF;
            }
            ic_stat = 1;
            break;
        case 1:
            //TFT_LCD_writeString(numbers[1], 100, 200, TFT_LCD_RED, TFT_LCD_CYAN);
            /*get the rise time*/
            while (IC1CON1bits.ICBNE) {
                uint16_t temp = IC1BUF;
            }
            ic_stat = 0;
            IC1CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
            break;
        default:
            break;
    }
}