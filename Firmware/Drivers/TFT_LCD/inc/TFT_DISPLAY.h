/* 
 * File:   TFT_DISPLAY.h
 * Author: Zachary S. Levenberg
 * Created on September 10, 2016, 8:48 PM
 */
#ifndef TFT_DISPLAY_H
#define	TFT_DISPLAY_H

/*******************************************************************************
 * Include
 * ****************************************************************************/
#include <string.h>
#include <stdint.h>
#include "TFT_LCD.h"
#include "TFT_TOUCH.h"
#include "ASCII_5X7.h"

/*******************************************************************************
 * Defines & Datatypes
 * ****************************************************************************/

/*******************************************************************************
 * Function Prototypes
 * ****************************************************************************/

/**
 * 
 * @param string
 * @param x
 * @param y
 * @param color
 * @param size
 * @return 
 */
uint8_t  TFT_DISPLAY_place_button(const char* string, uint8_t x, uint8_t y, uint16_t color, uint8_t size);

/**
 * 
 * @return 
 */
uint8_t TFT_DISPLAY_button_handler(void);

/**
 * 
 */
void TFT_DISPLAY_destroy_buttons(void);



#endif	/* TFT_DISPLAY_H */

