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
#include "pins.h"
#include "TFT_DISPLAY.h"
#include "bolt_uart.h"

#define DEFINES_TOUCH_X0 (PINS_pin_s){PIN_PORTA,0}
#define DEFINES_TOUCH_X1 (PINS_pin_s){PIN_PORTB,2}
#define DEFINES_TOUCH_Y0 (PINS_pin_s){PIN_PORTA,1}
#define DEFINES_TOUCH_Y1 (PINS_pin_s){PIN_PORTB,3}

#define DEFINES_TFT_LCD_RESET (PINS_pin_s){PIN_PORTB,12}
#define DEFINES_TFT_LCD_CS (PINS_pin_s){PIN_PORTB,10}
#define DEFINES_TFT_LCD_DC (PINS_pin_s){PIN_PORTB,11}


#define TEST_LED B15

#define FSM_TEMPLATE_print(...) (char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);)

static uint16_t count = 0;
static uint8_t flag = 0;

static uint16_t count2 = 0;
static uint8_t flag2 = 0;

static touchData myScreenPosition;


static const char helloString[] = "WELCOME TO THE E-MOTO";
static const char intruction[] = "SELECT A BUTTON BELOW";

static char myScreenBuffer[100];

/*
 * 
 */
int main(void) {

    Micro_Init();
    clockInit(FREQ_120MHZ, EXTERNAL);
    TFT_LCD_INIT(DEFINES_TFT_LCD_RESET, DEFINES_TFT_LCD_CS, DEFINES_TFT_LCD_DC);
    TFT_TOUCH_INIT(DEFINES_TOUCH_X0, DEFINES_TOUCH_X1, DEFINES_TOUCH_Y0, DEFINES_TOUCH_Y1, AN5, AN4);
    Uart1Init(UART_TX_RP36, UART_RX_RP20, BAUD115200);

    Uart1Write("hello this is the LCD test program\n\n");

    T1_Interrupt_Init(1, 3);
    PIN_direction(TEST_LED, OUTPUT);


    TFT_LCD_fillBackground(TFT_LCD_RED);
    TFT_LCD_drawRect(4, 4, TFT_LCD_width() - 4, TFT_LCD_height() - 4, TFT_LCD_BLACK);
    TFT_LCD_drawRect(8, 8, TFT_LCD_width() - 8, TFT_LCD_height() - 8, TFT_LCD_RED);
    //TFT_LCD_drawRect(100, 100, 200,200, TFT_LCD_BLACK);

    TFT_LCD_writeString(helloString,TFT_LCD_CENTER, 100, TFT_LCD_MAGENTA, TFT_LCD_CYAN, 3);
    //    TFT_LCD_writeString(intruction, TFT_LCD_CENTER, 120, TFT_LCD_RED, TFT_LCD_CYAN, 1);
    //    TFT_LCD_writeString(numbers[0], 100, 150, TFT_LCD_RED, TFT_LCD_CYAN, 2);
    //    sprintf(myScreenBuffer, "number is: %d", 27);
    //    TFT_LCD_writeVariableString(myScreenBuffer, 100, 175, TFT_LCD_RED, TFT_LCD_CYAN, 3);
    //
    //    TFT_DISPLAY_place_button("START", 1, 4, TFT_LCD_GREEN, 1);
    //    TFT_DISPLAY_place_button("STOP", 2, 4, TFT_LCD_MAGENTA, 3);
    //    TFT_DISPLAY_place_button("SUCK", 3, 4, TFT_LCD_YELLOW, 2);
    //    TFT_DISPLAY_place_button("DICK", 4, 3, TFT_LCD_WHITE, 2);



    while (1) {

        if (flag2) {
            flag2 = 0;

        }

        if (flag) {
            flag = 0;
//
            if (TFT_TOUCH_run() == TOUCHING) {
                TFT_TOUCH_draw(TFT_LCD_GREEN);
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
        Uart1Write("uou idiot\n");
        PIN_toggle(TEST_LED)
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