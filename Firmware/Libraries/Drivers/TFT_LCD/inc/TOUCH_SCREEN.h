/* 
 * File:   TOUCH_SCREEN.h
 * Author: Zachary S. Levenberg
 * Created on September 6, 2016, 10:20 PM
 */
#ifndef TOUCH_SCREEN_H
#define	TOUCH_SCREEN_H

/*******************************************************************************
 * Include
 * ****************************************************************************/
#include <stdint.h>
#include "bolt_ADC.h"
#include "pins.h"

/*******************************************************************************
 * Defines & Datatypes
 * ****************************************************************************/

#define TOUCH_SCREEN_SENSITIVITY 30 /*Lower is more sensitive*/

typedef enum _TOUCH_SCREEN_touchDataStatus_E {
    IDLE,
    TOUCHED,
} TOUCH_SCREEN_touchDataStatus_E;

typedef struct _TOUCH_SCREEN_touchData_S {
    uint8_t status;
    uint16_t xPos;
    uint16_t yPos;
    uint8_t xStatus;
    uint8_t yStatus;
} TOUCH_SCREEN_touchData_S;

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
void TOUCH_SCREEN_INIT(PINS_pin_S x0, PINS_pin_S x1, PINS_pin_S y0, PINS_pin_S y1, ADC_pinNumber_E an_x, ADC_pinNumber_E an_y);

/**
 * 
 * @return 
 */
uint8_t TOUCH_SCREEN_run(void);

/**
 * 
 * @return 
 */
TOUCH_SCREEN_touchData_S TOUCH_SCREEN_get_data(void);

#endif	/* TOUCH_SCREEN_H */

