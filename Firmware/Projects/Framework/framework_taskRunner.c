/*Framework Includes*/
#include "framework.h"
#include "framework_configure.h"


/*******************************************************************************
 * Debugging
 * ****************************************************************************/
#define DEBUG 1
#ifdef DEBUG
#include <stdio.h>
#include "bolt_uart.h"
static uint8_t debugEnable = 1;
#define framework_taskRunner_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
#define framework_taskRunner_print(...)
#endif




void FRAMEWORK_TASKRUNNER_init(void) {
    framework_taskRunner_print("Service: %s\n\n", "TaskRunner, Event: init");
}

inline void FRAMEWORK_TASKRUNNER_1ms(void) {

}

inline void FRAMEWORK_TASKRUNNER_10ms(void) {

}

inline void FRAMEWORK_TASKRUNNER_100ms(void) {

}

inline void FRAMEWORK_TASKRUNNER_1000ms(void) {
    framework_taskRunner_print("Service: %s\n\n", "TaskRunner, Event: 1000ms");
}

void FRAMEWORK_TASKRUNNER_debug(uint8_t state){
    debugEnable = state;
}