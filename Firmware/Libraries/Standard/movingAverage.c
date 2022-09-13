#include "movingAverage.h"

float takeMovingAverage(movingAverage_S *x, float value){
    x->sum-=(x->buffer[x->index]);
    x->buffer[x->index++] = value;
    x->sum += value;
    if(x->index >= x->windowSize){
        x->index = 0;
    }
    return x->sum/(x->windowSize);
}


float takeLowPassFilter(lowPassFilter_S *x, float value){
    x->accum = (x->alpha * value) + (1.0 - x->alpha) * x->accum;
    return x->accum;
}