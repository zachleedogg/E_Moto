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


static uint8_t count = 0;
static uint8_t flag = 0;

static uint8_t count2 = 0;
static uint8_t flag2 = 0;

static uint16_t xpos = 0;
static uint16_t xlast = 0;
static uint16_t ypos = 0;
static uint16_t ylast = 0;

static uint8_t toggle = 1;


static const char helloString[] = "EAT MY SHORTS";

static const char* numbers[10];

/*
 * 
 */
int main(void) {

    Micro_Init();
    clockInit(FREQ_80MHZ, INTERNAL);
    TFT_LCD_INIT(IO_PIN_RB12, IO_PIN_RB10, IO_PIN_RB11);
    IO_setPinDir(IO_PIN_RB15, OUTPUT);
    T1_Interrupt_Init(1, 3);

    ADC_Init();

    TFT_TOUCH_INIT(IO_PIN_RA0, IO_PIN_RB0, IO_PIN_RA1, IO_PIN_RB1, AN3, AN2);


    TFT_LCD_fillBackground(TFT_LCD_BLACK);

    numbers[0] = "apple";
    numbers[1] = "bobby";
    numbers[2] = "cunto";
    numbers[3] = "dickk";
    numbers[4] = "eleph";
    numbers[5] = "fucku";
    numbers[6] = "godda";
    numbers[7] = "hella";
    numbers[8] = "items";
    numbers[9] = "juunk";




    TFT_LCD_drawRect(50, 50, TFT_LCD_WIDTH - 50, TFT_LCD_HEIGHT - 50, TFT_LCD_RED);

    TFT_LCD_writeString(helloString, 100, 100);
    TFT_LCD_writeString(numbers[0], 100, 150);
    TFT_LCD_writeString(numbers[1], 100, 175);


    while (1) {


        if (flag) {
            flag = 0;

        }

        if (flag2) {
            flag2 = 0;
            if (toggle) {
                toggle = 0;
                xpos = (TFT_TOUCH_get_x_pos()*45)/100 - 75;
            } else {
                toggle = 1;
                ypos = (TFT_TOUCH_get_y_pos()*50)/100;
            }

            TFT_LCD_drawRect(xpos, ypos, xpos + 3, ypos + 3, TFT_LCD_GREEN);
            if(ypos > 410){
                asm ("RESET");
            }
            //            uint16_t tempo = temp / 10;
            //            switch (tempo) {
            //                case 0 ... 10:
            //                    TFT_LCD_writeString(numbers[0], 100, 300);
            //                    break;
            //                case 11 ... 20:
            //                    TFT_LCD_writeString(numbers[1], 100, 300);
            //                    break;
            //                case 21 ... 30:
            //                    TFT_LCD_writeString(numbers[2], 100, 300);
            //                    break;
            //                case 31 ... 40:
            //                    TFT_LCD_writeString(numbers[3], 100, 300);
            //                    break;
            //                case 41 ... 50:
            //                    TFT_LCD_writeString(numbers[4], 100, 300);
            //                    break;
            //                case 51 ... 60:
            //                    TFT_LCD_writeString(numbers[5], 100, 300);
            //                    break;
            //                case 61 ... 70:
            //                    TFT_LCD_writeString(numbers[6], 100, 300);
            //                    break;
            //                case 71 ... 80:
            //                    TFT_LCD_writeString(numbers[7], 100, 300);
            //                    break;
            //                case 81 ... 90:
            //                    TFT_LCD_writeString(numbers[8], 100, 300);
            //                    break;
            //                case 91 ... 100:
            //                    TFT_LCD_writeString(numbers[9], 100, 300);
            //                    break;
            //                default:
            //                    TFT_LCD_writeString(helloString, 100, 100);
            //                    break;
            //        }
        }
    }
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {

    /*Put your code here*/


    count++;
    count2++;
    if (count2 == 1) {
        count2 = 0;
        flag2 = 1;
    }
    if (count == 50) {
        IO_pinToggle(IO_PIN_RB15);
        flag = 1;
        count = 0;
    }

    /* clear timer x interrupt */
    _T1IF = 0;
}