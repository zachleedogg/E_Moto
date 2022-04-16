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
    uint16_t index;
    const uint16_t windowSize;
    uint32_t sum;
    uint16_t *buffer;
} movingAverage_S;

typedef struct {
    double alpha;
    uint16_t accum;
} lowPassFilter_S;

#define CONCAT(a,b) a##b


/**
 * @brief Creates a moving average object
 * @param x: name of the average object
 * @param y: Window average size
 * @return none
 */
#define NEW_AVERAGE(x,y) \
static uint16_t x##buffer[y] = {};\
static movingAverage_S x##Object = {\
    .index = 0, \
    .windowSize = y, \
    .sum = 0, \
    .buffer = x##buffer \
};\
static movingAverage_S * x = &x##Object\

/**
 * @brief Creates a moving average object
 * @param x: name of the average object
 * @param y: Weight (alpha)
 * @return none
 */
#define NEW_LOW_PASS_FILTER(x,y) \
static lowPassFilter_S x##Object = {\
    .alpha = y, \
    .accum = 0, \
};\
static lowPassFilter_S * x = &x##Object\


/**
 * @brief: Call with Moving Average object repeatedly
 * @param x: Name of the average object
 * @param value: New value to include in the average
 * @return : running average
 */
uint16_t takeMovingAverage(movingAverage_S *x, uint16_t value);

/**
 * @brief: Call with Filtered object repeatedly
 * @param x: Name of the average object
 * @param value: New value to include in the average
 * @return : LPF output
 */
uint16_t takeLowPassFilter(lowPassFilter_S *x, uint16_t value);

#endif	/* MOVINGAVERAGE_H */

