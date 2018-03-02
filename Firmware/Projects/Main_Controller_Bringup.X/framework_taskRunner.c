/*******************************************************************************
 * Includes
 * ****************************************************************************/
#include "framework.h"
#include <xc.h>

/*******************************************************************************
 * Debugging
 * ****************************************************************************/
#define DEBUG 0
#if DEBUG
#include <stdio.h>
#include "bolt_uart.h"
static uint8_t debugEnable = 1;
#define framework_taskRunner_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
static uint8_t debugEnable = 1;
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
    _TRISA0 = 0;
    _TRISD0 = 0;
    _TRISD1 = 0;
    _TRISD2 = 0;
    _TRISD3 = 0;
    _TRISD4 = 0;
    _TRISD5 = 0;
    _TRISD6 = 0;
    _TRISD7 = 0;
    _TRISD8 = 0;
    _TRISD9 = 0;
    _TRISD10 = 0;
    _TRISD11 = 0;
    _TRISD12 = 0;
    _TRISD13 = 0;
    _TRISD14 = 0;
    _TRISD15 = 0;
    
    _RD2 = 1;
    
    _TRISE0 = 0;
    _RE0 = 1;
    _TRISE3 = 0;
    _RE3 = 1;
    
}

/**
 * This function is called periodically is 1ms intervals
 */
inline void FRAMEWORK_TASKRUNNER_1ms(void) {
//    if(_LATA0 == 1){
//        _RA0 = 0;
//    }
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
    //_RA0 = 1;
}

/**
 * Enables or disables the UART debugging print statements
 * @param state 1 (enable) or 0 (disable)
 */
void FRAMEWORK_TASKRUNNER_debug(uint8_t state){
    debugEnable = state;
}