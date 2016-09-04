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

uint8_t colorarr[5] = {
    0x00,
    0x0A,
    0x2F,
    0xA8,
    0x55
};

static uint8_t colorpt = 0x45;

static uint8_t count = 0;
static uint8_t color = 0;
static uint8_t flag = 0;

/*
 * 
 */
int main(void) {

    Micro_Init();
    clockInit(FREQ_80MHZ, INTERNAL);
    TFT_LCD_INIT(IO_PIN_RB12, IO_PIN_RB10, IO_PIN_RB11);
    IO_setPinDir(IO_PIN_RB15, OUTPUT);
    T1_Interrupt_Init(60, 3);


    TFT_LCD_fillBackground(0x89);

    uint8_t col = 0x2A;
    uint8_t row = 0x2B;
    uint8_t ramw = 0x2C;
    
    TFT_LCD_drawRect(0,0,100,100,0x73);
    TFT_LCD_drawRect(0,200,100,250,0x73);
    TFT_LCD_drawRect(0,300,100,350,0x73);
    TFT_LCD_drawRect(0,400,100,450,0x73);
    

    
    TFT_LCD_drawRect(200,100,250,400,0x21);

    while (1) {


        if (flag) {
            flag = 0;
            int i = 0;
            //writeconst(colorarr[color], 307200);
        }


    }

}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {

    /*Put your code here*/


    count++;
    if (count == 5) {
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