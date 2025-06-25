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

void clearMovingAverage(movingAverage_S *x){
    x->buffer = 0;
    x->index = 0;
    x->sum = 0;
}


float takeLowPassFilter(lowPassFilter_S *x, float value){
    x->accum = (x->alpha * value) + (1.0 - x->alpha) * x->accum;
    return x->accum;
}

float getLowPassFilter(lowPassFilter_S *x){
    return x->accum;
}

void clearLowPassFilter(lowPassFilter_S *x){
    x->accum = 0;
}