#include "TFT_TOUCH.h"
#include "TFT_LCD.h"

#define HIEGHT_LOWER 120
#define HIEGHT_UPPER 875

#define WIDTH_LOWER 120
#define WIDTH_UPPER 925


static pin_number _x0 = 0;
static pin_number _x1 = 0;
static pin_number _y0 = 0;
static pin_number _y1 = 0;
static adc_pin_number an_X = 0;
static adc_pin_number an_Y = 0;

static uint8_t toggler = 1;

static touchData prevSample = {};
static touchData thisScreenData = {};
static uint8_t debounce = 0;

void setXpins();
void setYpins();
uint8_t inRange(uint16_t item1, uint16_t item2, uint8_t range);
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

void TFT_TOUCH_INIT(pin_number x0, pin_number x1, pin_number y0, pin_number y1, adc_pin_number an_x, adc_pin_number an_y) {
    _x0 = x0;
    _x1 = x1;
    _y0 = y0;
    _y1 = y1;
    an_X = an_x;
    an_Y = an_y;
    ADC_Init();
    setXpins();
}

touchData TFT_TOUCH_run(void) {

    thisScreenData.status = IDLE;
    if (toggler) {
        toggler = 0;
        thisScreenData.xPos = TFT_TOUCH_get_x_pos();
        if (thisScreenData.xPos < HIEGHT_LOWER || thisScreenData.xPos > HIEGHT_UPPER) {
            thisScreenData.xPos = 0;
        }
    } else {
        toggler = 1;
        thisScreenData.yPos = TFT_TOUCH_get_y_pos(); ///2;
        if (thisScreenData.yPos < WIDTH_LOWER || thisScreenData.yPos > WIDTH_UPPER) {
            thisScreenData.yPos = 0;
        } else {
            /*If the reading seems stable, then consider the read as current position*/
            if (inRange(thisScreenData.xPos, prevSample.xPos, TFT_TOUCH_SENSITIVITY) &&
                    inRange(thisScreenData.yPos, prevSample.yPos, TFT_TOUCH_SENSITIVITY)) {
                thisScreenData.status = TOUCHING;
            } else { /*Debounce stray data, if too many stray dots, switch to IDLE mode*/
                thisScreenData.status = IDLE;
            }
            prevSample.xPos = thisScreenData.xPos;
            prevSample.yPos = thisScreenData.yPos;
            thisScreenData.xPos = map(thisScreenData.xPos, HIEGHT_LOWER, HIEGHT_UPPER,0, TFT_LCD_height());
            thisScreenData.yPos = map(thisScreenData.yPos, WIDTH_LOWER, WIDTH_UPPER,0, TFT_LCD_width());
        }
    }
    return thisScreenData;
}

uint16_t TFT_TOUCH_get_x_pos(void) {
    uint16_t returnVal = ADC_GetValue(an_X);
    setYpins();
    return returnVal;
}

uint16_t TFT_TOUCH_get_y_pos(void) {
    uint16_t returnVal = ADC_GetValue(an_Y);
    setXpins();
    return returnVal;
}

void setXpins(void) {
    /*Set Y pins Hi-Z*/
    IO_setPinDir(_y0, INPUT);
    IO_setPinDir(_y1, INPUT);
    ADC_RemovePin(an_Y);
    /*Turn on AN pin X and enable x pins*/
    ADC_SetPin(an_X);
    IO_setPinDir(_x0, OUTPUT);
    IO_pinWrite(_x0, HIGH);
    IO_setPinDir(_x1, OUTPUT);
    IO_pinWrite(_x1, LOW);
}

void setYpins(void) {
    /*Set X pins Hi-Z*/
    IO_setPinDir(_x0, INPUT);
    IO_setPinDir(_x1, INPUT);
    ADC_RemovePin(an_X);
    /*Turn on AN pin Y and enable y pins*/
    ADC_SetPin(an_Y);
    IO_setPinDir(_y0, OUTPUT);
    IO_pinWrite(_y0, LOW);
    IO_setPinDir(_y1, OUTPUT);
    IO_pinWrite(_y1, HIGH);
}

uint8_t inRange(uint16_t item1, uint16_t item2, uint8_t range) {
    uint8_t returnVal = 0;
    uint8_t temp = 0;
    if (item1 > item2) {
        temp = item1 - item2;
    } else {
        temp = item2 - item1;
    }
    if (temp < range) {
        returnVal = 1;
    }
    return returnVal;
}

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
  return (uint16_t)((uint32_t)(x - in_min) * (uint32_t)(out_max - out_min) / (uint32_t)((in_max - in_min) + out_min));
}

