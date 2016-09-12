/* 
 * File:   TFT_DISPLAY.h
 * Author: Zach
 *
 * Created on September 10, 2016, 8:48 PM
 */

#include <string.h>
#include <stdint.h>
#include "TFT_LCD.h"
#include "TFT_TOUCH.h"
#include "ASCII_5X7.h"


#ifndef TFT_DISPLAY_H
#define	TFT_DISPLAY_H

void TFT_DISPLAY_place_button(const char* string, uint8_t x, uint8_t y, uint16_t color, uint8_t size);

uint8_t TFT_DISPLAY_button_handler();

void TFT_DISPLAY_destroy_buttons();



#endif	/* TFT_DISPLAY_H */

