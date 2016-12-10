/*Framework Includes*/
#include "framework_taskRunner.h"
#include "framework.h"
#include "Defines.h"


/*******************************************************************************
 * Debugging
 * ****************************************************************************/
#ifdef DEBUG
#include <stdio.h>
#include "bolt_uart.h"
static uint8_t debugEnable = 1;
#define framework_taskRunner_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
#define framework_taskRunner_print(...)
#endif


void FRAMEWORK_TASKRUNNER_init(void) {

    /*Init I2C module*/
    i2c1_Init(BAUD_400K);
    i2c_SetDeviceID(DEFINES_EEPROM_1_ADDRESS);
    i2c_SetAddress(0x00, ADDRESS_SPACE_8_BIT);
    //i2c1_Write("doggbrohampussyt", 16);

    /*Init Ping sensor*/
    ping_Init(DEFINES_PING_RIGHT_ECHO, DEFINES_PING_RIGHT_TRIGGER, DEFINES_PING_LEFT_ECHO, DEFINES_PING_LEFT_TRIGGER);

    /*Init Touch Screen Sensors*/
    TFT_TOUCH_INIT(DEFINES_TOUCH_X0, DEFINES_TOUCH_X1, DEFINES_TOUCH_Y0, DEFINES_TOUCH_Y1, DEFINES_TOUCH_AN_X, DEFINES_TOUCH_AN_Y);

    /*Init Digital input pins*/
    PINS_pin_S thispin = DEFINES_SW2;
    PINS_direction(thispin.port, thispin.pin, INPUT);
    PINS_pullUp(thispin.port, thispin.pin, HIGH);

    thispin = DEFINES_SW3;
    PINS_direction(thispin.port, thispin.pin, INPUT);
    PINS_pullUp(thispin.port, thispin.pin, HIGH);

    thispin = DEFINES_5V_SW_RAIL;
    PINS_direction(thispin.port, thispin.pin, OUTPUT);
    PINS_write(thispin.port, thispin.pin, HIGH);

    thispin = DEFINES_LED_DRIVER_BRIGHTNESS;
    PINS_direction(thispin.port, thispin.pin, OUTPUT);
    PINS_write(thispin.port, thispin.pin, LOW);

    /*Init 16bit LCD driver*/
    ledDriverInit(DEFINES_LED_DRIVER_CLOCK, DEFINES_LED_DRIVER_DATA, DEFINES_LED_DRIVER_LATCH);
    ledDriverWrite(0, 0xFFFF);

    ADC_SetPin(VBATT_PROT_MONITOR);
}

inline void FRAMEWORK_TASKRUNNER_1ms(void) {

    /*Run touch screen Stuff
     Throw an event during every touch and throw a single event for the
     release of touch from the screen.*/
    static TFT_TOUCH_touchDataStatus_E touchStatus = IDLE;
    if (TFT_TOUCH_run() == TOUCHED) {
        touchStatus = TOUCHED;
        Event newEvent;
        newEvent.EventParam = TOUCHED;
        newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
        newEvent.EventType = TFT_TOUCH_EVENT;
        newEvent.Service = touchScreenService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
    } else if (touchStatus == TOUCHED) {
        touchStatus = IDLE;
        Event newEvent;
        newEvent.EventParam = RELEASED;
        newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
        newEvent.EventType = TFT_TOUCH_EVENT;
        newEvent.Service = touchScreenService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
    }

    /*Initialize Switch 2 and debounce using BUTTONS_run()*/
    static BUTTONS_object_S sw2 = {
        .debounce = 0,
        .pin = DEFINES_SW2,
        .polarity = BUTTONS_NEGATIVE_LOGIC,
        .status = BUTTONS_STATUS_OFF,
        .threshold = 32,
    };
    BUTTONS_status_E tempStatus = sw2.status;
    BUTTONS_status_E newStatus = BUTTONS_run(&sw2);
    if (newStatus != tempStatus) {
        Event newEvent;
        newEvent.EventParam = newStatus;
        newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
        newEvent.EventType = BUTTON_SW2_PRESS_EVENT;
        newEvent.Service = touchScreenService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
    }

    /*Initialize Switch 3 and debounce using BUTTONS_run()*/
    static BUTTONS_object_S sw3 = {
        .debounce = 0,
        .pin = DEFINES_SW3,
        .polarity = BUTTONS_NEGATIVE_LOGIC,
        .status = BUTTONS_STATUS_OFF,
        .threshold = 32,
    };
    tempStatus = sw3.status;
    newStatus = BUTTONS_run(&sw3);
    if (newStatus != tempStatus) {
        Event newEvent;
        newEvent.EventParam = newStatus;
        newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
        newEvent.EventType = BUTTON_SW3_PRESS_EVENT;
        newEvent.Service = touchScreenService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
    }
    
    if(Uart1RXdataReady()){
        Event newEvent;
        newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
        newEvent.EventType = DEBUG_MESSAGE_EVENT;
        newEvent.Service = debuggerService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
    }

}

inline void FRAMEWORK_TASKRUNNER_10ms(void) {

    /*Run the LED sweep*/
//    static uint16_t tog = 1;
//    static uint8_t direction = 0;
//    ledDriverWrite(0, tog);
//    if (direction == 0) {
//        tog = tog << 1;
//    } else {
//        tog = tog >> 1;
//    }
//    if (tog == 0x8000) {
//        direction = 1;
//    } else if (tog == 0x0001) {
//        direction = 0;
//    }

}

inline void FRAMEWORK_TASKRUNNER_100ms(void) {

    Event newEvent;
    /* Run the Ping sensors*/
    ping_Run();
    
    newEvent.EventParam = PING_getDistance(1) | (PING_getDistance(0)<<8);
    newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
    newEvent.EventType = PING_SENSOR_EVENT;
    newEvent.Service = touchScreenService_SERVICE;
    FRAMEWORK_postEvent(newEvent);

}

inline void FRAMEWORK_TASKRUNNER_1000ms(void) {

    Event newEvent;

    /*Get VBATT RAW  ADC voltage*/
    newEvent.EventParam = ADC_GetValue(VBATT_PROT_MONITOR);
    newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
    newEvent.EventType = BATTERY_12V_LEVEL_EVENT;
    newEvent.Service = touchScreenService_SERVICE;
    FRAMEWORK_postEvent(newEvent);
}