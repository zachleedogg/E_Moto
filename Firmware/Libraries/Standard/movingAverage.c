#include "movingAverage.h"

uint16_t takeMovingAverage(movingAverage_S *x, uint16_t value){
    x->sum-=(uint32_t)(x->buffer[x->index]);
    x->buffer[x->index++] = value;
    x->sum += (uint32_t)value;
    if(x->index >= x->windowSize){
        x->index = 0;
    }
    return (uint16_t)(x->sum/((uint16_t)x->windowSize));
}


uint16_t takeLowPassFilter(lowPassFilter_S *x, uint16_t value){
    x->accum = (uint16_t)((x->alpha * (double)value) + (1.0 - x->alpha) * (double)x->accum);
    return x->accum;
}