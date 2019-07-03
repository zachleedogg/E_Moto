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
#include "pins.h"
#include "bolt_uart.h"
#include "bolt_sleep.h"

//Task Scheduler system control
#include "pinSetup.h"
#include "SerialDebugger.h"
#include "LightsControl.h"
#include "IgnitionControl.h"
#include "HornControl.h"
#include "HeatedGrips.h"
#include "j1772.h"
#include "IO.h"
#include "MCU_dbc.h"

/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/
#define DEBUG 1
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
    PinSetup_Init(); //Pin setup should be first
    IO_Efuse_Init(); //Init the Efuse Service
    LightsControl_Init(); //Init the Lights Control Service
    HeatedGripControl_Init(); //Init the heated grips
    CAN_DBC_init(); //Init the CAN System Service
    IgnitionControl_Init();
    HornControl_Init();

    SET_IC_CONTROLLER_SLEEP_EN(LOW); //switch enable must be high during noraml operation, this shouldn't go here....
    SET_CAN_SLEEP_EN(LOW);

    Uart1Write("Hello World, Task Init Done.\n"); //hi
}

/**
 * Tsk is for continuously running tasks, will run when scheduler is Idle.
 */
void Tsk(void) {

}

/**
 * Runs every 10ms
 */
void Tsk_10ms(void) {
    IO_Efuse_Run_10ms(); //Run the Efuse System
    IgnitionControl_Run_10ms();
    HornControl_Run_10ms(); //Run Horn. Horn is disabled if button is held for too long.
}

/**
 * Runs every 100ms
 */
void Tsk_100ms(void) {
    SerialConsole_Run_100ms(); //Debug Serial Terminal Emulation
    LightsControl_Run_100ms(); //Run the System Lights layer (Responds to button presses, controls, etc...)
    HeatedGripControl_Run_100ms(); //Run Heated Grips. Currently activated by spare sw 2
    j1772Control_Run_100ms(); //Run j1772 Proximity and Pilot Signal Control.
    
    CAN_mcu_status_send(); //Send CAN message, this should be wrapped up in "CAN_RUN_100ms()" or similar
    CAN_mcu_command_send(); //same
}

/**
 * Runs every 1000ms
 */
void Tsk_1000ms(void) {
    SET_DEBUG_LED_EN(TOGGLE); //Toggle Debug LED at 1Hz for scheduler running status
    
    CAN_mcu_motorControllerRequest_send(); //move this
}

/**
 * Tsk Halt will stop the system tick, put any sleep related code here.
 * The service that calls Tsk_Sleep must be damn sure it is allowed to call
 * this function, also, the caller must subsequently call Tsk_Resume to start
 * the scheduler back up again.
 */
void Tsk_Sleep(void) {
    SysTick_Stop(); //does this idea need clean-up? is this the best way?
    SET_SW_EN(LOW);
    SET_IC_CONTROLLER_SLEEP_EN(HIGH); //same
    SET_CAN_SLEEP_EN(HIGH); //same
    
    IO_Efuse_Halt();
    LightsControl_Halt();
    HeatedGripControl_Halt();
    HornControl_Halt();
    j1772Control_Halt();
    
    SET_DEBUG_LED_EN(LOW); //same
    setWakeUp(PIN, IGNITION_SWITCH_IN);
    SleepNow(); //Go to sleep
    SysTick_Resume();
    SET_SW_EN(HIGH);
    SET_IC_CONTROLLER_SLEEP_EN(LOW); //same
    SET_CAN_SLEEP_EN(LOW); //same
    Uart1Write("waking From Sleep");
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

