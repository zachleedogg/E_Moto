#include "TOUCH_SCREEN.h"
#include "pins.h"
#include "movingAverage.h"

/*height and width ADC values experimentally found*/
#define HIEGHT_LOWER 120
#define HIEGHT_UPPER 800
#define WIDTH_LOWER 120
#define WIDTH_UPPER 875

/*Touch debounce sensitivty and filtering weight.*/
#define DEBOUNCE_SIZE 32
#define FILTER_WEIGHT 0.05

/*digital and ADC pin holders*/
static PINS_pin_S _x0;
static PINS_pin_S _x1;
static PINS_pin_S _y0;
static PINS_pin_S _y1;
static ADC_pinNumber_E an_X = 0;
static ADC_pinNumber_E an_Y = 0;

static uint8_t toggler = 1;

static TOUCH_SCREEN_touchData_S thisScreenData = {};

static uint16_t debounce_x = 0;
static uint16_t debounce_y = 0;

NEW_LOW_PASS_FILTER(lpf_x, FILTER_WEIGHT);
NEW_LOW_PASS_FILTER(lpf_y, FILTER_WEIGHT);

static void setXpins();
static void setYpins();
static uint16_t TOUCH_SCREEN_get_x_pos(void);
static uint16_t TOUCH_SCREEN_get_y_pos(void);
static uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

void TOUCH_SCREEN_INIT(PINS_pin_S x0, PINS_pin_S x1, PINS_pin_S y0, PINS_pin_S y1, ADC_pinNumber_E an_x, ADC_pinNumber_E an_y) {
    _x0 = x0;
    _x1 = x1;
    _y0 = y0;
    _y1 = y1;
    an_X = an_x;
    an_Y = an_y;
//    ADC_Init();
    setXpins();
}

//TODO ZACH:
//fix debounce_x to go both ways. Make cleaner
uint8_t TOUCH_SCREEN_run(void) {
    if (toggler) {
        toggler = 0;
        /*get x position*/
        uint16_t temp = TOUCH_SCREEN_get_x_pos();
        /*Filter out boundaries*/
        if (temp < HIEGHT_LOWER || temp > HIEGHT_UPPER) {
            temp = HIEGHT_LOWER;
            thisScreenData.xStatus = IDLE;
            debounce_x = 0;
            lpf_x->accum = HIEGHT_UPPER - HIEGHT_LOWER;
        } else {
            if (thisScreenData.xStatus == IDLE ) {
                debounce_x++;
            }
            /*take moving average*/
            takeLowPassFilter(lpf_x, temp);
        }
        if (debounce_x >= DEBOUNCE_SIZE) {
            thisScreenData.xStatus = TOUCHED;
        }
    } else {
        toggler = 1;
        /*get y position*/
        uint16_t temp = TOUCH_SCREEN_get_y_pos();
        /*filter out boundaries*/
        if (temp < WIDTH_LOWER || temp > WIDTH_UPPER) {
            temp = WIDTH_LOWER;
            thisScreenData.yStatus = IDLE;
            debounce_y = 0;
            lpf_y->accum = HIEGHT_UPPER - HIEGHT_LOWER;
        } else {
            if (thisScreenData.yStatus == IDLE ) {
                debounce_y++;
            }
            /*take moving average*/
            takeLowPassFilter(lpf_y, temp);
        }
        if (debounce_y >= DEBOUNCE_SIZE) {
            thisScreenData.yStatus = TOUCHED;
        }
    }
    
    if ((thisScreenData.yStatus == TOUCHED) && (thisScreenData.xStatus == TOUCHED)) {
        thisScreenData.status = TOUCHED;
    } else {
        thisScreenData.status = IDLE;
    }

    thisScreenData.xPos = map(lpf_x->accum, HIEGHT_LOWER, HIEGHT_UPPER, 0, 320);
    thisScreenData.yPos = map(lpf_y->accum, WIDTH_LOWER, WIDTH_UPPER, 0, 480);
    return thisScreenData.status;
}


TOUCH_SCREEN_touchData_S TOUCH_SCREEN_get_data(void) {
    return thisScreenData;
}

static uint16_t TOUCH_SCREEN_get_x_pos(void) {
    uint16_t returnVal = ADC_GetValue(an_X); /*1.1 to account for pull-down resistors*/
    setYpins();
    return returnVal;
}

static uint16_t TOUCH_SCREEN_get_y_pos(void) {
    uint16_t returnVal = ADC_GetValue(an_Y);/*1.1 to account for pull-down resistors*/
    setXpins();
    return returnVal;
}

static void setXpins(void) {
    /*Set Y pins Hi-Z*/
    ADC_RemovePin(an_Y);
    PINS_direction(_y0, INPUT);
    PINS_direction(_y1, INPUT);
    /*Turn on AN pin X and enable x pins*/
    ADC_SetPin(an_X);
    PINS_direction(_x0, OUTPUT);
    PINS_direction(_x1, OUTPUT);
    PINS_write(_x0, LOW);
    PINS_write(_x1, HIGH);
}

static void setYpins(void) {
    /*Set X pins Hi-Z*/
    ADC_RemovePin(an_X);
    PINS_direction(_x0, INPUT);
    PINS_direction(_x1, INPUT);
    /*Turn on AN pin Y and enable y pins*/
    ADC_SetPin(an_Y);
    PINS_direction(_y0, OUTPUT);
    PINS_direction(_y1, OUTPUT);
    PINS_write(_y0, HIGH);
    PINS_write(_y1, LOW);
}

static uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    return (uint16_t) ((uint32_t) (x - in_min) * (uint32_t) (out_max - out_min) / (uint32_t) ((in_max - in_min) + out_min));
}

