/** 
 * @file tsk.h
 * @brief This module contains continuous tasks.
 *
 *  This is the header file for the definition for tasks that are continuous
 *  and should be ran as often as possible.
 */


/******************************************************************************
 * Includes
 *******************************************************************************/
// For this modules definitions
#include "tsk.h"					
#include "tsk_cfg.h"
#include "SysTick.h"

//Direct low level access
#include "bolt_uart.h"
#include "bolt_sleep.h"
#include "bolt_ADC.h"
#include "bolt_OC.h"



//Task Scheduler system control
#include "pinSetup.h"
#include "StateMachine.h"
#include "SerialDebugger.h"
#include "IO.h"
#include "bms_dbc.h"
#include "can_iso_tp.h"

/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/
#define DEBUG 0
#if DEBUG
#include <stdio.h>
#include "bolt_uart.h"

static uint8_t debugEnable = 1;
#define tskService_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
#define tskService_print(...)
#endif
/******************************************************************************
 * Configuration
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/

/******************************************************************************
 * Variable Declarations
 *******************************************************************************/

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

void Tsk_init(void);

/******************************************************************************
 * Function Declarations
 *******************************************************************************/

/**
 * Tsk_init is only run once ever (until a power on reset) so put pin and module stuff here
 */
void Tsk_init(void) {

    /*Init each module once*/
    PinSetup_Init(); // Pin setup should be first
    CAN_DBC_init(); // Initialize the CAN mailboxes
    StateMachine_Init();
    IO_SET_DEBUG_LED_EN(HIGH);
    IO_SET_SW_EN(HIGH);
    CAN_changeOpMode(CAN_NORMAL);
    pwmOCwriteDuty(CHARGE_PUMP_PWM, 50);
    IO_SET_DCDC_EN(HIGH);
    IO_SET_EV_CHARGER_EN(HIGH);
    
#if DEBUG
    Uart1Write("Hello World, Task Init Done.\n"); //hi
#endif
}

/**
 * Tsk is for continuously running tasks, will run when scheduler is Idle.
 */
void Tsk(void) {


}

/**
 * Runs every 1ms
 */
void Tsk_1ms(void) {
    run_iso_tp_basic();
}


/**
 * Runs every 5ms
 */
void Tsk_5ms(void) {
    StateMachine_Run();
}

/**
 * Runs every 10ms
 */
void Tsk_10ms(void) {
    CAN_send_10ms();

}

/**
 * Runs every 100ms
 */
void Tsk_100ms(void) {
    //IO_SET_DEBUG_LED_EN(TOGGLE); //Toggle Debug LED at 1Hz for scheduler running status
}

/**
 * Runs every 1000ms
 */
void Tsk_1000ms(void) {
    CAN_send_1000ms();
    uint32_t val = IO_GET_ISOLATION_VOLTAGE();
    uint32_t valA = IO_GET_MUX_1_VOLTAGE();
    uint32_t valB = IO_GET_MUX_2_VOLTAGE();
    uint32_t valC = IO_GET_MUX_3_VOLTAGE();
    uint32_t val2 = IO_GET_DCDC_OUTPUT_VOLTAGE();
    uint32_t val3 = IO_GET_EV_CHARGER_VOLTAGE();
    
    Nop();
    Nop();
}

/**
 * Tsk Halt will stop the system tick, put any sleep related code here.
 * The service that calls Tsk_Sleep must be damn sure it is allowed to call
 * this function, also, the caller must subsequently call Tsk_Resume to start
 * the scheduler back up again.
 */
void Tsk_Sleep(void) {
    SysTick_Stop(); //does this idea need clean-up? is this the best way?
    CAN_changeOpMode(CAN_DISABLE);
    IO_SET_SW_EN(LOW);
    IO_SET_DEBUG_LED_EN(LOW); //same
    
    SleepNow(); //Go to sleep
    
    SysTick_Resume();
#if DEBUG
    Uart1Write("waking From Sleep");
#endif
}

/**
 * Tsk Run should be called from Main, once entered, it will never return....
 * @param SystemClock: clock speed in Hz i.e. 120,000,000 = 120MHz.
 */
void Tsk_Run(uint32_t SystemClock) {

    static uint32_t tick = 0; // System tick
    static TaskType *Task_ptr; // Task pointer
    static uint8_t TaskIndex = 0; // Task index
    const uint8_t NumTasks = Tsk_GetNumTasks(); // Number of tasks

    SysTick_Init(SystemClock);

    Task_ptr = Tsk_GetConfig(); // Get a pointer to the task configuration

    Tsk_init();
    // The main while loop.  This while loop will run the program forever
    while (1) {
        tick = SysTick_Get(); // Get current system tick

        // Loop through all tasks.  First, run all continuous tasks.  Then,
        // if the number of ticks since the last time the task was run is
        // greater than or equal to the task interval, execute the task.
        for (TaskIndex = 0; TaskIndex < NumTasks; TaskIndex++) {
            if (Task_ptr[TaskIndex].Interval == 0) {
                // Run continuous tasks.
                (*Task_ptr[TaskIndex].Func)();
            } else if ((tick - Task_ptr[TaskIndex].LastTick) >= Task_ptr[TaskIndex].Interval) {
                (*Task_ptr[TaskIndex].Func)(); // Execute Task

                Task_ptr[TaskIndex].LastTick = tick; // Save last tick the task was ran.
            }
        }// end for


    }// end while(1)
}


/*** End of File **************************************************************/
