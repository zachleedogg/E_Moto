#include "movingAverage.h"

uint16_t takeAverage(movingAverage_S *x, uint16_t value){
    x->sum-=(uint32_t)(x->buffer[x->index]);
    x->buffer[x->index++] = value;
    x->sum += (uint32_t)value;
    if(x->index >= x->windowSize){
        x->index = 0;
    }
    return (uint16_t)(x->sum/((uint16_t)x->windowSize));
}
