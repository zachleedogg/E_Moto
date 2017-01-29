#include "movingAverage.h"

uint16_t TAKE_AVERAGE(movingAverage_S *x, uint16_t value){
    x->sum-=x->buffer[x->index];
    x->buffer[x->index++] = value;
    x->sum += value;
    if(x->index >= x->windowSize){
        x->index = 0;
    }
    return x->sum/x->windowSize;
}
