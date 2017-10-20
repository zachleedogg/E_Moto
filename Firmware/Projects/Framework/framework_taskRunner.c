/*******************************************************************************
 * Includes
 * ****************************************************************************/
#include "framework.h"


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


/*******************************************************************************
 * Functions
 * ****************************************************************************/

/**
 * This function is run once at startup
 */
void FRAMEWORK_TASKRUNNER_init(void) {
    framework_taskRunner_print("Service: %s\n\n", "TaskRunner, Event: init");
}

/**
 * This function is called periodically is 1ms intervals
 */
inline void FRAMEWORK_TASKRUNNER_1ms(void) {

}

/**
 * This function is called periodically is 10ms intervals
 */
inline void FRAMEWORK_TASKRUNNER_10ms(void) {

}

/**
 * This function is called periodically is 100ms intervals
 */
inline void FRAMEWORK_TASKRUNNER_100ms(void) {

}

/**
 * This function is called periodically is 1000ms intervals
 */
inline void FRAMEWORK_TASKRUNNER_1000ms(void) {
    framework_taskRunner_print("Service: %s\n\n", "TaskRunner, Event: 1000ms");
}

/**
 * Enables or disables the UART debugging print statements
 * @param state 1 (enable) or 0 (disable)
 */
void FRAMEWORK_TASKRUNNER_debug(uint8_t state){
    debugEnable = state;
}