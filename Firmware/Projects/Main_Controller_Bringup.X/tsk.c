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
#include "Efuse.h"
#include "MCU_dbc.h"
#include "bolt_sleep.h"

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

void Tsk_init(void) {
    PinSetup_Init();
    Uart1Write("Hello World");
    EFUSE_Init();
    CAN_init(CAN_BAUD_1M, CAN_NORMAL);
    CAN_DBC_init();
    setWakeUp(WAKE_ON_UART1,(PINS_pin_S){0,0});
}

void Tsk(void) {

}

void Tsk_10ms(void) {
    EFUSE_Run();
}

void Tsk_100ms(void) {
    SerialConsole_Run();
    CAN_MCU_status_send();
    CAN_MCU_command_send();
}

void Tsk_1000ms(void) {
    SET_DEBUG_LED_EN(TOGGLE);
    CAN_MCU_motorControllerRequest_send();
}

void Tsk_RUN(uint32_t SystemClock) {

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

void Tsk_HALT(void){
    SysTick_Stop();
    SET_SW_EN(LOW);
    SET_IC_CONTROLLER_SLEEP_EN(HIGH);
    SET_CAN_SLEEP_EN(HIGH);
    SET_DIAG_EN(LOW);
    SET_DIAG_SELECT_EN(LOW);
    SET_DEBUG_LED_EN(LOW);
}

void Tsk_RESUME(void){
    SysTick_Resume();
    SET_SW_EN(HIGH);
    SET_IC_CONTROLLER_SLEEP_EN(LOW);
    SET_CAN_SLEEP_EN(LOW);
}
/*** End of File **************************************************************/

