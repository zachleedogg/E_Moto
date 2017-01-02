/* 
 * File:   DISPLAY_INTERFACE.h
 * Author: Zachary S. Levenberg
 * Created on September 10, 2016, 8:48 PM
 */
#ifndef DISPLAY_INTERFACE_H
#define	DISPLAY_INTERFACE_H

/*******************************************************************************
 * Include
 * ****************************************************************************/
#include <string.h>
#include <stdint.h>
#include "TFT_LCD.h"
#include "TOUCH_SCREEN.h"
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
uint8_t  DISPLAY_INTERFACE_place_button(const char* string, uint8_t x, uint8_t y, uint16_t color, uint8_t size);

/**
 * 
 * @return 
 */
uint8_t DISPLAY_INTERFACE_button_handler(void);

/**
 * 
 */
void DISPLAY_INTERFACE_destroy_buttons(void);

/**
 * Draws a dot where the screen is touched
 * @param color: color to draw
 */
void DISPLAY_INTERFACE_draw(uint16_t color);

#endif	/* DISPLAY_INTERFACE_H */

