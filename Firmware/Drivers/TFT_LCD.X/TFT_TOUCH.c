#include "TFT_TOUCH.h"

static pin_number _x0 = 0;
static pin_number _x1 = 0;
static pin_number _y0 = 0;
static pin_number _y1 = 0;
static adc_pin_number an_X = 0;
static adc_pin_number an_Y= 0;

void setXpins();
void setYpins();

void TFT_TOUCH_INIT(pin_number x0, pin_number x1, pin_number y0, pin_number y1, adc_pin_number an_x, adc_pin_number an_y){
    _x0 = x0;
    _x1 = x1;
    _y0 = y0;
    _y1 = y1;
    an_X = an_x;
    an_Y = an_y;
    ADC_Init();
    setXpins();
}

uint16_t TFT_TOUCH_get_x_pos(void){
    uint16_t returnVal = ADC_GetValue(an_X);
    setYpins();
    return returnVal;
}

uint16_t TFT_TOUCH_get_y_pos(void){
    uint16_t returnVal = ADC_GetValue(an_Y);
    setXpins();
    return returnVal;
}

void setXpins(void){
    IO_setPinDir(_y0, INPUT);
    IO_setPinDir(_y1, INPUT);
    ADC_RemovePin(an_Y);
    ADC_SetPin(an_X);
    IO_setPinDir(_x0, OUTPUT);
    IO_pinWrite(_x0, HIGH);
    IO_setPinDir(_x1, OUTPUT);
    IO_pinWrite(_x1, LOW);
}

void setYpins(void){
    IO_setPinDir(_x0, INPUT);
    IO_setPinDir(_x1, INPUT);
    ADC_RemovePin(an_X);
    ADC_SetPin(an_Y);
    IO_setPinDir(_y0, OUTPUT);
    IO_pinWrite(_y0, HIGH);
    IO_setPinDir(_y1, OUTPUT);
    IO_pinWrite(_y1, LOW);
}
