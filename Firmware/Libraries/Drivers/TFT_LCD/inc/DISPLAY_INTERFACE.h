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
 * Places a rectagular button with text. Returns button ID number.
 * @param string
 * @param x
 * @param y
 * @param color
 * @param size
 * @return Button ID number
 */
uint8_t  DISPLAY_INTERFACE_place_button(const char* string, uint8_t x, uint8_t y, uint16_t color, uint8_t size);

/**
 * 
 * @return 
 */
uint8_t DISPLAY_INTERFACE_button_handler(void);

/**
 * deletes all existing buttons.
 */
void DISPLAY_INTERFACE_destroy_buttons(void);

/**
 * Draws a dot where the screen is touched. To be called continuously.
 */
void DISPLAY_INTERFACE_draw(void);

/**
 * 
 * @param enable
 */
void DISPLAY_INTERFACE_enable_drawing(uint8_t enable, uint16_t color);

#endif	/* DISPLAY_INTERFACE_H */

