/*
 * File:   TFT_LCD.h
 * Author: Zachary Levenberg
 * Created on September 6, 2016, 7:11 PM
 */

#ifndef LCD_SPI_H
#define	LCD_SPI_H

/*******************************************************************************
 * Include
 * ****************************************************************************/
#include <xc.h> 
#include <stdint.h>
#include "pins.h"

/*******************************************************************************
 * Defines & Datatypes
 * ****************************************************************************/

// Color definitions
#define	TFT_LCD_BLACK   0x0000
#define	TFT_LCD_BLUE    0x001F
#define	TFT_LCD_RED     0xF800
#define	TFT_LCD_GREEN   0x07E0
#define TFT_LCD_CYAN    0x07FF
#define TFT_LCD_MAGENTA 0xF81F
#define TFT_LCD_YELLOW  0xFFE0  
#define TFT_LCD_WHITE   0xFFFF

/*Arrangement Defintions*/
#define TFT_LCD_CENTER 240
#define TFT_LCD_MIDLLE 160

typedef enum _TFT_LCD_dataFormat_E {
    DATA,
    COMMAND,
    CONST,
    STRING
} TFT_LCD_dataFormat_E;

typedef enum _TFT_LCD_orientation_E{
    PORTIAT,
    LANDSCAPE
} TFT_LCD_orientation_E;

/*******************************************************************************
 * Function Prototypes
 * ****************************************************************************/

/**
 * 
 * @param reset
 * @param CE
 * @param DC
 */
void TFT_LCD_init(PINS_pin_S reset, PINS_pin_S CE, PINS_pin_S DC);

/**
 * 
 * @param thisWay
 */
void TFT_LCD_setOrientation(TFT_LCD_orientation_E thisWay);

/**
 * 
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param color
 */
void TFT_LCD_drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/**
 * 
 * @param color
 */
void TFT_LCD_fillBackground(uint16_t color);

/**
 * 
 * @param anystring
 * @param x
 * @param y
 * @param fillColor
 * @param textColor
 * @param size
 */
void TFT_LCD_writeString(const char* anystring, uint16_t x, uint16_t y, uint16_t fillColor, uint16_t textColor, uint8_t size);

/**
 * 
 * @param anystring
 * @param x
 * @param y
 * @param fillColor
 * @param textColor
 * @param size
 */
void TFT_LCD_writeVariableString(char * anystring, uint16_t x, uint16_t y, uint16_t fillColor, uint16_t textColor, uint8_t size);

/**
 * 
 */
void TFT_LCD_goToSleep(void);

/**
 * 
 * @return 
 */
uint16_t TFT_LCD_height(void);

/**
 * 
 * @return 
 */
uint16_t TFT_LCD_width(void);

#define HX8357D 0xD

#define HX8357_TFTWIDTH  320
#define HX8357_TFTHEIGHT 480

#define HX8357_NOP     0x00
#define HX8357_SWRESET 0x01
#define HX8357_RDDID   0x04
#define HX8357_RDDST   0x09

#define HX8357_RDPOWMODE  0x0A
#define HX8357_RDMADCTL  0x0B
#define HX8357_RDCOLMOD  0x0C
#define HX8357_RDDIM  0x0D
#define HX8357_RDDSDR  0x0F

#define HX8357_SLPIN   0x10
#define HX8357_SLPOUT  0x11

#define HX8357_INVOFF  0x20
#define HX8357_INVON   0x21
#define HX8357_DISPOFF 0x28
#define HX8357_DISPON  0x29

#define HX8357_CASET   0x2A
#define HX8357_PASET   0x2B
#define HX8357_RAMWR   0x2C
#define HX8357_RAMRD   0x2E

#define HX8357_TEON  0x35
#define HX8357_TEARLINE  0x44

#define HX8357_MADCTL  0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

#define HX8357_COLMOD  0x3A

#define HX8357_SETOSC 0xB0
#define HX8357_SETPWR1 0xB1
#define HX8357_SETRGB 0xB3
#define HX8357D_SETCOM  0xB6

#define HX8357D_SETCYC  0xB4
#define HX8357D_SETC 0xB9

#define HX8357B_SET_PANEL_DRIVING 0xC0
#define HX8357D_SETSTBA 0xC0

#define HX8357_SETPANEL  0xCC

#define HX8357D_SETGAMMA 0xE0

#define HX8357B_SETGAMMA 0xC8
#define HX8357B_SETPANELRELATED  0xE9

#endif	/* LCD_SPI_H */

