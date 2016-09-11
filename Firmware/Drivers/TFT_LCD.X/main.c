/* 
 * File:   main.c
 * Author: kid group
 *
 * Created on September 1, 2016, 8:04 AM
 */

#include <stdio.h>
#include <stdint.h>
#include "bolt_init.h"
#include "TFT_LCD.h"
#include "bolt_pins.h"


#define TEST_LED IO_PIN_RB15


static uint16_t count = 0;
static uint8_t flag = 0;

static uint16_t count2 = 0;
static uint8_t flag2 = 0;

static touchData myScreenPosition;


static const char helloString[] = "WELCOME TO THE E-MOTO";
static const char intruction[] = "SELECT A BUTTON BELOW";

static char myScreenBuffer[100];

static const char* numbers[10];

/*
 * 
 */
int main(void) {

    Micro_Init();
    clockInit(FREQ_80MHZ, INTERNAL);
    TFT_LCD_INIT(IO_PIN_RB12, IO_PIN_RB10, IO_PIN_RB11);
    TFT_TOUCH_INIT(IO_PIN_RA0, IO_PIN_RB2, IO_PIN_RA1, IO_PIN_RB3, AN5, AN4);

    T1_Interrupt_Init(1, 3);
    IO_setPinDir(TEST_LED, OUTPUT);

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




    TFT_LCD_drawRect(25, 25, TFT_LCD_width() - 25, TFT_LCD_height() - 25, TFT_LCD_RED);
    
    TFT_LCD_drawRect(10, 10, TFT_LCD_width()/4 -10, TFT_LCD_height()/4 - 10, TFT_LCD_GREEN);
    TFT_LCD_drawRect((TFT_LCD_width()/4)+10, 10, TFT_LCD_width()/2 -10, TFT_LCD_height()/4 - 10, TFT_LCD_GREEN);
    TFT_LCD_drawRect(TFT_LCD_width()/2 +10, 10, (TFT_LCD_width()*3)/4 - 10, TFT_LCD_height()/4 - 10, TFT_LCD_GREEN);
    TFT_LCD_drawRect((TFT_LCD_width()*3)/4 + 10, 10, TFT_LCD_width() - 10, TFT_LCD_height()/4 - 10, TFT_LCD_GREEN);

    TFT_LCD_writeString(helloString, (TFT_LCD_width()/2)-(strlen(helloString)*12/2), 100, TFT_LCD_RED, TFT_LCD_CYAN, 2);
    TFT_LCD_writeString(intruction, (TFT_LCD_width()/2)-(strlen(intruction)*12/2), 120, TFT_LCD_RED, TFT_LCD_CYAN, 2);
    TFT_LCD_writeString(numbers[0], 100, 150, TFT_LCD_RED, TFT_LCD_CYAN, 2);
    sprintf(myScreenBuffer, "number is: %d", 27);
    TFT_LCD_writeVariableString(myScreenBuffer, 100, 175, TFT_LCD_RED, TFT_LCD_CYAN, 2);


    while (1) {

        if (flag2) {
            flag2 = 0;

        }

        if (flag) {
            flag = 0;
            if (myScreenPosition.status == TOUCHING) {
                TFT_LCD_drawRect(myScreenPosition.yPos, myScreenPosition.xPos,
                        myScreenPosition.yPos + 3, myScreenPosition.xPos + 3, TFT_LCD_GREEN);
                sprintf(myScreenBuffer, "Xraw: %u, Yraw: %u", myScreenPosition.yPos, myScreenPosition.xPos);
                TFT_LCD_writeVariableString(myScreenBuffer, 100, 175, TFT_LCD_RED, TFT_LCD_CYAN, 2);
            }


            if (myScreenPosition.yPos > 450) {
                //asm("RESET");
            }
        }
    }
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {

    /*Put your code here*/


    count++;
    count2++;

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