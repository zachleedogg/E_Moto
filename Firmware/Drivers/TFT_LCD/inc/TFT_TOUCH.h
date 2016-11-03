/* 
 * File:   TFT_TOUCH.h
 * Author: Zachary S. Levenberg
 * Created on September 6, 2016, 10:20 PM
 */
#ifndef TFT_TOUCH_H
#define	TFT_TOUCH_H

/*******************************************************************************
 * Include
 * ****************************************************************************/
#include <stdint.h>
#include "bolt_ADC.h"
#include "pins.h"

/*******************************************************************************
 * Defines & Datatypes
 * ****************************************************************************/

#define TFT_TOUCH_SENSITIVITY 30 /*Lower is more sensitive*/

typedef enum _TFT_TOUCH_touchDataStatus_E {
    IDLE,
    TOUCHED,
    RELEASED
} TFT_TOUCH_touchDataStatus_E;

typedef struct _TFT_TOUCH_touchData_S {
    uint8_t status;
    uint16_t xPos;
    uint16_t yPos;
} TFT_TOUCH_touchData_S;

/*******************************************************************************
 * Function Definitions
 * ****************************************************************************/

/**
 * 
 * @param x0
 * @param x1
 * @param y0
 * @param y1
 * @param an_x
 * @param an_y
 */
void TFT_TOUCH_INIT(PINS_pin_S x0, PINS_pin_S x1, PINS_pin_S y0, PINS_pin_S y1, ADC_pinNumber_E an_x, ADC_pinNumber_E an_y);

/**
 * 
 * @return 
 */
uint8_t TFT_TOUCH_run(void);

/**
 * 
 * @return 
 */
TFT_TOUCH_touchData_S TFT_TOUCH_get_data(void);

/**
 * Draw Pixel at selected (touched) area of screen.
 * @param color to draw pixel in
 * @return Success 0 or Failure 1.
 */
uint8_t TFT_TOUCH_draw(uint16_t color);

#endif	/* TFT_TOUCH_H */

