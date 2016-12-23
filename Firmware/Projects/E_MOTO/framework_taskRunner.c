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
    TFT_TOUCH_INIT(DEFINES_TOUCH_X0, DEFINES_TOUCH_X1, DEFINES_TOUCH_Y0, DEFINES_TOUCH_Y1, DEFINES_TOUCH_AN_X, DEFINES_TOUCH_AN_Y);

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

    CAN_Configure(DEFINES_CAN_TX, DEFINES_CAN_RX, DEFINES_CAN_BAUD, CAN_NORMAL);
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

    /*Switch 2 and debounce using BUTTONS_run()*/
    BUTTONS_status_E tempStatus = sw2.status;
    BUTTONS_status_E newStatus = BUTTONS_run(&sw2);
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

    if (CAN_RxDataIsReady()) {
        Event newEvent;
        newEvent.EventPriority = FRAMEWORK_PRIORITY_2;
        newEvent.EventType = CAN_MESSAGE_RCVD_EVENT;
        newEvent.Service = canService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
    }

    /*Run the LED sweep*/
    static float sinner = 0;
    static int timerpi = 1;
    static counter = 0;
    counter++;
    /*LED Breathing Thing*/
    uint8_t temp = (uint8_t) (fabs(sin(sinner))*16.4);
    sinner += .0033;
    if (timerpi == temp) {
        
        int i = 0;
        for (i = 0; i < counter; i++) {
            framework_taskRunner_print("-");
        }
        framework_taskRunner_print("\n");
        counter = 0;
        static uint16_t tog = 0x8000;
        static uint8_t direction = 0;
        ledDriverWrite(0, tog);
        if (direction == 0) {
            timerpi++;
            tog = tog >> 1;
        } else {
            timerpi--;
            tog = tog << 1;
        }
        if (tog == 0x8000) {
            direction = 0;
            sinner = 0;
        } else if (tog == 0x0001) {
            direction = 1;
        }
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
    newEvent.EventParam = ADC_GetValue(VBATT_PROT_MONITOR);
    newEvent.EventPriority = FRAMEWORK_PRIORITY_1;
    newEvent.EventType = BATTERY_12V_LEVEL_EVENT;
    newEvent.Service = touchScreenService_SERVICE;
    FRAMEWORK_postEvent(newEvent);

    static uint8_t mynum = 0;
    mynum++;
    const char str[] = "Hello  ";
    CAN_message_S newMessage = {
        .nodeID = 0x4,
        .messageID = 0x2,
        .frequency = 0x7,
        .byte0 = str[mynum % 7],
        .byte1 = str[(mynum + 1) % 7],
        .byte2 = str[(mynum + 2) % 7],
        .byte3 = str[(mynum + 3) % 7],
        .byte4 = str[(mynum + 4) % 7],
        .byte5 = str[(mynum + 5) % 7],
        .byte6 = str[(mynum + 6) % 7],
        .byte7 = mynum,
    };

    CAN_write(&newMessage);

}

void FRAMEWORK_TASKRUNNER_debug(uint8_t state) {
    debugEnable = state;
}