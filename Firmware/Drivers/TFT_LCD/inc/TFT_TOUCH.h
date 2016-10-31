/* 
 * File:   TFT_TOUCH.h
 * Author: kid group
 *
 * Created on September 6, 2016, 10:20 PM
 */

#include <stdint.h>

#include "bolt_ADC.h"
#include "pins.h"

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

void TFT_TOUCH_INIT(PINS_pin_S x0, PINS_pin_S x1, PINS_pin_S y0, PINS_pin_S y1, ADC_pinNumber_E an_x, ADC_pinNumber_E an_y) ;

uint8_t TFT_TOUCH_run(void);

touchData TFT_TOUCH_get_data(void);

uint8_t TFT_TOUCH_draw(uint16_t color);

#endif	/* TFT_TOUCH_H */

