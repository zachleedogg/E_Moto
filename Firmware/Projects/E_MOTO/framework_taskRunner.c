#include "framework_taskRunner.h"
#include "framework.h"

/**/
#include "TFT_TOUCH.h"

typedef struct _TASKRUNNER_timer {
    uint8_t ones;
    uint8_t tens;
    uint8_t hunds;
} TASKRUNNER_timer;
static TASKRUNNER_timer timer = {};

void FRAMEWORK_TASKRUNNER_1ms(void);
void FRAMEWORK_TASKRUNNER_10ms(void);
void FRAMEWORK_TASKRUNNER_100ms(void);
void FRAMEWORK_TASKRUNNER_1000ms(void);

void FRAMEWORK_TASKRUNNER_1ms(void) {
    
    /*Run touch screen Stuff
     Throw an event during any touch activity and throw a final event for the
     release of touch from the screen.*/
    static uint8_t internalTouchStatus = 0;
    if (TFT_TOUCH_run()) {
        internalTouchStatus = 1;
        Event newEvent;
        newEvent.EventParam = 0;
        newEvent.EventPriority = PRIORITY_1;
        newEvent.EventType = TFT_TOUCH_EVENT;
        newEvent.Service = touchScreenService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
    } else if(internalTouchStatus){
        internalTouchStatus = 0;
        Event newEvent;
        newEvent.EventParam = 0;
        newEvent.EventPriority = PRIORITY_1;
        newEvent.EventType = TFT_TOUCH_EVENT;
        newEvent.Service = touchScreenService_SERVICE;
        FRAMEWORK_postEvent(newEvent);
    }
    
    
}

void FRAMEWORK_TASKRUNNER_10ms(void) {
    
    /*Run Button checker at reasonably fast speed. No need to debounce because
     of polling freq combined with Low-Pass filter on input.*/

}

void FRAMEWORK_TASKRUNNER_100ms(void) {

}

void FRAMEWORK_TASKRUNNER_1000ms(void) {

}

Event FRAMEWORK_TASKRUNNER_run(Event emptyEvent) {

    /*1ms tasks here*/
    timer.ones++;
    FRAMEWORK_TASKRUNNER_1ms();

    /*10ms tasks here*/
    if (timer.ones == 10) {
        timer.ones = 0;
        timer.tens++;
        FRAMEWORK_TASKRUNNER_10ms();

        /*100ms tasks here*/
        if (timer.tens == 10) {
            timer.tens = 0;
            timer.hunds++;
            FRAMEWORK_TASKRUNNER_100ms();

            /*1000ms tasks here*/
            if (timer.hunds == 10) {
                timer.hunds = 0;
                FRAMEWORK_TASKRUNNER_1000ms();

            }
        }
    }
    return emptyEvent;
}
