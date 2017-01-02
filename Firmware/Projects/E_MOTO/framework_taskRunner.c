/*Framework Includes*/
#include "framework_taskRunner.h"
#include "framework.h"
#include "Defines.h"
#include <math.h>


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
/*******************************************************************************
 * USER SPACE
 * ****************************************************************************/
static BUTTONS_object_S sw2 = {
    .debounce = 0,
    .pin = DEFINES_SW2,
    .polarity = BUTTONS_NEGATIVE_LOGIC,
    .status = BUTTONS_STATUS_OFF,
    .threshold = 32,
};

static BUTTONS_object_S sw3 = {
    .debounce = 0,
    .pin = DEFINES_SW3,
    .polarity = BUTTONS_NEGATIVE_LOGIC,
    .status = BUTTONS_STATUS_OFF,
    .threshold = 32,
};

/*******************************************************************************
 * TASK SCHEDULES
 * ****************************************************************************/
void FRAMEWORK_TASKRUNNER_init(void) {

    /*Init I2C module*/
    i2c1_Init(BAUD_400K);
    i2c_SetDeviceID(DEFINES_EEPROM_1_ADDRESS);
    i2c_SetAddress(0x00, ADDRESS_SPACE_8_BIT);
    //i2c1_Write("doggbrohampussyt", 16);

    /*Init Ping sensor*/
    ping_Init(DEFINES_PING_RIGHT_ECHO, DEFINES_PING_RIGHT_TRIGGER, DEFINES_PING_LEFT_ECHO, DEFINES_PING_LEFT_TRIGGER);

    /*Init Touch Screen Sensors*/
    TOUCH_SCREEN_INIT(DEFINES_TOUCH_X0, DEFINES_TOUCH_X1, DEFINES_TOUCH_Y0, DEFINES_TOUCH_Y1, DEFINES_TOUCH_AN_X, DEFINES_TOUCH_AN_Y);

    /*Init Digital input pins*/

    PINS_direction(sw2.pin, INPUT);
    PINS_pullUp(sw2.pin, HIGH);

    PINS_direction(sw3.pin, INPUT);
    PINS_pullUp(sw3.pin, HIGH);

    /*Init Digital Output pins*/
    PINS_direction(DEFINES_5V_SW_RAIL, OUTPUT);
    PINS_write(DEFINES_5V_SW_RAIL, HIGH);

    PINS_direction(DEFINES_LED_DRIVER_BRIGHTNESS, OUTPUT);
    PINS_write(DEFINES_LED_DRIVER_BRIGHTNESS, LOW);

    PINS_direction(DEFINES_CAN_STBY, OUTPUT);
    PINS_write(DEFINES_CAN_STBY, LOW);

    /*Init 16bit LCD driver*/
    ledDriverInit(DEFINES_LED_DRIVER_CLOCK, DEFINES_LED_DRIVER_DATA, DEFINES_LED_DRIVER_LATCH);
    ledDriverWrite(0, 0xFFFF);

    ADC_SetPin(VBATT_PROT_MONITOR);

    CAN_init(DEFINES_CAN_TX, DEFINES_CAN_RX, DEFINES_CAN_BAUD, CAN_NORMAL);
}

inline void FRAMEWORK_TASKRUNNER_1ms(void) {

    /*Run touch screen Stuff
     Throw an event during every touch and throw a single event for the
     release of touch from the screen.*/
    static TOUCH_SCREEN_touchDataStatus_E touchStatus = IDLE;
    if (TOUCH_SCREEN_run() == TOUCHED) {
        touchStatus = TOUCHED;
        Event newEvent;
        newEvent.EventParam = TOUCHED;
        newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
        newEvent.EventType = TOUCH_SCREEN_EVENT;
        newEvent.Service = touchScreenService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
    } else if (touchStatus == TOUCHED) {
        touchStatus = IDLE;
        Event newEvent;
        newEvent.EventParam = RELEASED;
        newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
        newEvent.EventType = TOUCH_SCREEN_EVENT;
        newEvent.Service = touchScreenService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
    }

    BUTTONS_status_E tempStatus;
    BUTTONS_status_E newStatus;

    /*Switch 2 and debounce using BUTTONS_run()*/
    tempStatus = sw2.status;
    newStatus = BUTTONS_run(&sw2);
    if (newStatus != tempStatus) {
        Event newEvent;
        newEvent.EventParam = newStatus;
        newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
        newEvent.EventType = BUTTON_SW2_PRESS_EVENT;
        newEvent.Service = touchScreenService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
        framework_taskRunner_print("SW2: %d\n", newStatus);
    }

    /*Initialize Switch 3 and debounce using BUTTONS_run()*/

    tempStatus = sw3.status;
    newStatus = BUTTONS_run(&sw3);
    if (newStatus != tempStatus) {
        Event newEvent;
        newEvent.EventParam = newStatus;
        newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
        newEvent.EventType = BUTTON_SW3_PRESS_EVENT;
        newEvent.Service = touchScreenService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
        framework_taskRunner_print("SW3: %d\n", newStatus);
    }

    if (Uart1RXdataReady()) {
        Event newEvent;
        newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
        newEvent.EventType = DEBUG_MESSAGE_EVENT;
        newEvent.Service = debuggerService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
    }

    /*Run the LED sweep*/
    static float sinner = 0;
    static double val = 0.0;
    static uint8_t lastVal = 0;
    static uint8_t counter = 0;
    static uint16_t ticker = 0;
    counter++;
    ticker++;
    /*LED Breathing Thing*/
    val = (fabs(sin(sinner))*16.25);
    uint8_t temp = (uint8_t)val;
    sinner += .00314159;
    if (lastVal != temp) {
        lastVal = temp;

        framework_taskRunner_print("LED: %02d, VAL: %f", temp, val);
        uint8_t i = 0;
        for (i = 0; i < counter; i++) {
            framework_taskRunner_print(".");
        }
        counter = 0;
        framework_taskRunner_print("\n");
        ledDriverWrite(0, 0xFFFF << 16-temp);
    }
    if(ticker==1000){
        ticker=0;
        sinner=0;
    }
}

inline void FRAMEWORK_TASKRUNNER_10ms(void) {



}

inline void FRAMEWORK_TASKRUNNER_100ms(void) {

    Event newEvent;
    /* Run the Ping sensors*/
    ping_Run();

    newEvent.EventParam = PING_getDistance(1) | (PING_getDistance(0) << 8);
    newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
    newEvent.EventType = PING_SENSOR_EVENT;
    newEvent.Service = touchScreenService_SERVICE;
    FRAMEWORK_postEvent(newEvent);

}

inline void FRAMEWORK_TASKRUNNER_1000ms(void) {

    framework_taskRunner_print("CPU usage: %d\n", FRAMEWORK_getCPUPercentage());

    Event newEvent;

    /*Get VBATT RAW  ADC voltage*/
    float batV = 4.84 * 3.3 * ((float) ADC_GetValue(VBATT_PROT_MONITOR)) / 1024.0;
    framework_taskRunner_print("Battery VOltage: %f, Raw ADC: %d\n", batV, ADC_GetValue(VBATT_PROT_MONITOR));
    newEvent.EventParam = ADC_GetValue(VBATT_PROT_MONITOR);
    newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
    newEvent.EventType = BATTERY_12V_LEVEL_EVENT;
    newEvent.Service = touchScreenService_SERVICE;
    FRAMEWORK_postEvent(newEvent);


}

void FRAMEWORK_TASKRUNNER_debug(uint8_t state) {
    debugEnable = state;
}