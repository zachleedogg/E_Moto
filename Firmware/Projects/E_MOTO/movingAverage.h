/* 
 * File:   movingAverage.h
 * Author: kid group
 *
 * Created on January 8, 2017, 1:34 PM
 */

#ifndef MOVINGAVERAGE_H
#define	MOVINGAVERAGE_H

#include <stdint.h>

typedef struct {
    uint8_t index;
    const uint8_t windowSize;
    uint16_t sum;
    uint16_t *buffer;
} movingAverage_S;

#define CONCAT(a,b) a##b

#define NEW_AVERAGE(x,y) \
static uint16_t x##buffer[y] = {};\
static movingAverage_S x = {\
    .index = 0, \
    .windowSize = y, \
    .sum = 0, \
    .buffer = x##buffer \
}\

//NEW_AVERAGE(test, 10);


uint16_t TAKE_AVERAGE(movingAverage_S *x, uint16_t value);

#endif	/* MOVINGAVERAGE_H */

