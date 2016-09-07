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

void TFT_TOUCH_INIT(pin_number x0, pin_number x1, pin_number y0, pin_number y1, adc_pin_number an_x, adc_pin_number an_y);

uint16_t TFT_TOUCH_get_x_pos(void);

uint16_t TFT_TOUCH_get_y_pos(void);

#endif	/* TFT_TOUCH_H */

