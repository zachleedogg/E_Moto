#include "TFT_TOUCH.h"
#include "pins.h"

#define HIEGTH_
#define HIEGHT_LOWER 120
#define HIEGHT_UPPER 875

#define WIDTH_LOWER 120
#define WIDTH_UPPER 925


static PINS_pin_S _x0;
static PINS_pin_S _x1;
static PINS_pin_S _y0;
static PINS_pin_S _y1;

static ADC_pinNumber_E an_X = 0;
static ADC_pinNumber_E an_Y = 0;

static uint8_t toggler = 1;

static TFT_TOUCH_touchData_S thisScreenData = {};

static uint8_t debounce = 0;

#define AVERAGE_SIZE 8

typedef struct _average {
    uint16_t x[AVERAGE_SIZE];
    uint16_t sum_x;
    uint16_t y[AVERAGE_SIZE];
    uint16_t sum_y;
    uint8_t index;
} average;

static average touchAve;

static void setXpins();
static void setYpins();
static uint16_t TFT_TOUCH_get_x_pos(void);
static uint16_t TFT_TOUCH_get_y_pos(void);
static uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

void TFT_TOUCH_INIT(PINS_pin_S x0, PINS_pin_S x1, PINS_pin_S y0, PINS_pin_S y1, ADC_pinNumber_E an_x, ADC_pinNumber_E an_y) {
    _x0 = x0;
    _x1 = x1;
    _y0 = y0;
    _y1 = y1;
    an_X = an_x;
    an_Y = an_y;
    ADC_Init();
    setXpins();
}

//TODO ZACH:
//fix debounce to go both ways. Make cleaner
uint8_t TFT_TOUCH_run(void) {
    if (toggler) {
        toggler = 0;
        /*get x position*/
        uint16_t temp = TFT_TOUCH_get_x_pos();
        /*Filter out boundaries*/
        if (temp < HIEGHT_LOWER || temp > HIEGHT_UPPER) {
            temp = HIEGHT_LOWER;
            thisScreenData.status = IDLE;
            debounce = 0;
        } else {
            debounce++;
        }
        if (debounce == AVERAGE_SIZE) {
            debounce = 0;
            thisScreenData.status = TOUCHED;
        }
        /*take moving average*/
        touchAve.sum_x -= touchAve.x[touchAve.index];
        touchAve.x[touchAve.index] = temp;
        touchAve.sum_x += temp;
    } else {
        toggler = 1;
        /*get y position*/
        uint16_t temp = TFT_TOUCH_get_y_pos();
        /*filter out boundaries*/
        if (temp < WIDTH_LOWER || temp > WIDTH_UPPER) {
            temp = WIDTH_LOWER;
            thisScreenData.status = IDLE;
        }
        /*take moving average*/
        touchAve.sum_y -= touchAve.y[touchAve.index];
        touchAve.y[touchAve.index] = temp;
        touchAve.sum_y += temp;
        
        /*wrap around protection*/
        if (++touchAve.index == AVERAGE_SIZE) {
            touchAve.index = 0;
        }
    }

    thisScreenData.xPos = map(touchAve.sum_x / AVERAGE_SIZE, HIEGHT_LOWER, HIEGHT_UPPER, 0, 320);
    thisScreenData.yPos = map(touchAve.sum_y / AVERAGE_SIZE, WIDTH_LOWER, WIDTH_UPPER, 0, 480);
    return thisScreenData.status;
}


TFT_TOUCH_touchData_S TFT_TOUCH_get_data(void) {
    return thisScreenData;
}

static uint16_t TFT_TOUCH_get_x_pos(void) {
    uint16_t returnVal = ADC_GetValue(an_X);
    setYpins();
    return returnVal;
}

static uint16_t TFT_TOUCH_get_y_pos(void) {
    uint16_t returnVal = ADC_GetValue(an_Y);
    setXpins();
    return returnVal;
}

static void setXpins(void) {
    /*Set Y pins Hi-Z*/
    PINS_direction(_y0, INPUT);
    PINS_direction(_y1, INPUT);
    ADC_RemovePin(an_Y);
    /*Turn on AN pin X and enable x pins*/
    ADC_SetPin(an_X);
    PINS_direction(_x0, OUTPUT);
    PINS_write(_x0, HIGH);
    PINS_direction(_x1, OUTPUT);
    PINS_write(_x1, LOW);
}

static void setYpins(void) {
    /*Set X pins Hi-Z*/
    PINS_direction(_x0, INPUT);
    PINS_direction(_x1, INPUT);
    ADC_RemovePin(an_X);
    /*Turn on AN pin Y and enable y pins*/
    ADC_SetPin(an_Y);
    PINS_direction(_y0, OUTPUT);
    PINS_write(_y0, LOW);
    PINS_direction(_y1, OUTPUT);
    PINS_write(_y1, HIGH);
}

static uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    return (uint16_t) ((uint32_t) (x - in_min) * (uint32_t) (out_max - out_min) / (uint32_t) ((in_max - in_min) + out_min));
}

