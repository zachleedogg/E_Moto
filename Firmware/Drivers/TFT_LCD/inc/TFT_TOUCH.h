/* 
 * File:   TFT_TOUCH.h
 * Author: kid group
 *
 * Created on September 6, 2016, 10:20 PM
 */

#include "stdint.h"

#include "bolt_ADC.h"
#include "bolt_pins.h"

#ifndef TFT_TOUCH_H
#define	TFT_TOUCH_H

#define TFT_TOUCH_SENSITIVITY 30 /*Lower is more sensitive*/

typedef enum _touchDataStatus {
    IDLE,
    TOUCHING,
} touchDataStatus;

typedef struct _touchData {
    uint8_t status;
    uint16_t xPos;
    uint16_t yPos;
} touchData;

void TFT_TOUCH_INIT(pin_number x0, pin_number x1, pin_number y0, pin_number y1, adc_pin_number an_x, adc_pin_number an_y);

uint8_t TFT_TOUCH_run(void);

touchData TFT_TOUCH_get_data(void);

uint8_t TFT_TOUCH_draw(uint16_t color);

#endif	/* TFT_TOUCH_H */

