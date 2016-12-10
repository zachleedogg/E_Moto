/*Framework Includes*/
#include "framework_configure.h"


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

}

inline void FRAMEWORK_TASKRUNNER_1ms(void) {

}

inline void FRAMEWORK_TASKRUNNER_10ms(void) {

}

inline void FRAMEWORK_TASKRUNNER_100ms(void) {

}

inline void FRAMEWORK_TASKRUNNER_1000ms(void) {
    framework_taskRunner_print("Service: %s\n\n", "TaskRunner 1000ms");
}


