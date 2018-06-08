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
#include "tsk.h"					// For this modules definitions
#include "tsk_cfg.h"
#include "SysTick.h"

#include "pinSetup.h"
#include "pins.h"
#include "bolt_uart.h"
#include "SerialDebugger.h"
#include "IO.h"
#include "MCU_dbc.h"
#include "bolt_sleep.h"
#include "SystemControl.h"

/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/

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
    
    PinSetup_Init(); //Pin setup should be first
    IO_Init(); //Init the IO Control Service
    SystemControl_Init(); //Init the System Control Service
    CAN_DBC_init(); //Init the CAN System Service

    setWakeUp(WAKE_ON_UART1, (PINS_pin_S) {0, 0}); //This shouldn't go here
    SET_SW_EN(HIGH); //switch enable must be high during noraml operation, this shouldn't go here....
    SET_IC_CONTROLLER_SLEEP_EN(LOW);
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
    IO_Run(); //Run the IO layer (Handles all digital IO, highside drivers, efuses, etc...)
}

/**
 * Runs every 100ms
 */
void Tsk_100ms(void) {
    SerialConsole_Run(); //Debug Serial Terminal Emulation
    SystemControl_Run(); //Run the System Control layer (Responds to button presses, controls, etc...))
    CAN_MCU_status_send(); //Send CAN message, this should be wrapped up in "CAN_RUN_100ms()" or similar
    CAN_MCU_command_send(); //same
}

/**
 * Runs every 1000ms
 */
void Tsk_1000ms(void) {
    SET_DEBUG_LED_EN(TOGGLE); //Toggle Debug LED at 1Hz for scheduler running status
    CAN_MCU_motorControllerRequest_send(); //move this
}

/**
 * Tsk Halt will stop the system tick, put any sleep related code here.
 * The service that calls Tsk_Halt must be damn sure it is allowed to call
 * this function, also, the caller must subsequently call Tsk_Resume to start
 * the scheduler back up again.
 */
void Tsk_Halt(void) {
    SysTick_Stop(); //does this idea need clean-up? is this the best way?
    SET_SW_EN(LOW); //wrap this in "prepare to sleep" or similar
    SET_IC_CONTROLLER_SLEEP_EN(HIGH);//same
    SET_CAN_SLEEP_EN(HIGH);//same
    SET_DIAG_EN(LOW);//same
    SET_DIAG_SELECT_EN(LOW);//same
    SET_DEBUG_LED_EN(LOW);//same
}

/**
 * Resumes the schedule. Can only be called after a halt.
 */
void Tsk_Resume(void) {
    SysTick_Resume();//same
    SET_SW_EN(HIGH);//same
    SET_IC_CONTROLLER_SLEEP_EN(LOW);//same
    SET_CAN_SLEEP_EN(LOW);//same
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

