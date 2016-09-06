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
#include "bolt_pins.h"


static uint8_t count = 0;
static uint8_t color = 0;
static uint8_t flag = 0;

static uint8_t helloString[] = "EAT MY SHORTS";

/*
 * 
 */
int main(void) {

    Micro_Init();
    clockInit(FREQ_80MHZ, INTERNAL);
    TFT_LCD_INIT(IO_PIN_RB12, IO_PIN_RB10, IO_PIN_RB11);
    IO_setPinDir(IO_PIN_RB15, OUTPUT);
    T1_Interrupt_Init(50, 3);


    TFT_LCD_fillBackground(TFT_LCD_BLACK);


    TFT_LCD_drawRect(50, 50, TFT_LCD_WIDTH-50, TFT_LCD_HEIGHT-50, TFT_LCD_RED);
    
    TFT_LCD_writeString(helloString, 100,100);


    while (1) {


        if (flag) {
            flag = 0;
            int i = 0;
            //TFT_LCD_fillBackground(0x114d);

        }


    }

}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {

    /*Put your code here*/


    count++;
    if (count == 20) {
        IO_pinToggle(IO_PIN_RB15);
        flag = 1;
        count = 0;
        color++;
        if (color == 5) {
            color = 0;
        }

    }

    /* clear timer x interrupt */
    _T1IF = 0;
}