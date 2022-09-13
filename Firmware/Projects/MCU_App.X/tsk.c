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
#include "can_iso_tp_lite.h"
#include "canPopulate.h"
#include "pinSetup.h"
#include "SerialDebugger.h"
#include "LightsControl.h"
#include "IgnitionControl.h"
#include "HornControl.h"
#include "HeatedGrips.h"
#include "j1772.h"
#include "IO.h"
#include "MCU_dbc.h"
#include "StateMachine.h"

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
    CAN_DBC_init(); //Init the CAN System Service
    StateMachine_Init(); //Init state machine
    
//    IO_SET_SW_EN(HIGH);
//    
//    IO_Efuse_Init(); //Init the Efuse Service
//    LightsControl_Init(); //Init the Lights Control Service
//    HeatedGripControl_Init(); //Init the heated grips
//    IgnitionControl_Init();
//    HornControl_Init();
//    j1772Control_Init();
//
//    IO_SET_IC_CONTROLLER_SLEEP_EN(LOW); //switch enable must be high during normal operation, this shouldn't go here....
//    CAN_changeOpMode(CAN_NORMAL);
//    IO_SET_CAN_SLEEP_EN(LOW);
//    IO_SET_BATT_EN(HIGH);
    

    tskService_print("Hello World, Task Init Done.\n"); //hi
    tskService_print("Reset Reason: %x %x\n",(uint8_t)(RCON>>8), (uint8_t)RCON); 

}

/**
 * Tsk is for continuously running tasks, will run when scheduler is Idle.
 */
void Tsk(void) {
    j1772Control_Run_cont();
}

/**
 * Runs every 10ms
 */
void Tsk_1ms(void) {
    ClrWdt(); 
    
    StateMachine_Run();

    switch(run_iso_tp_basic()){
        case ISO_TP_NONE:
            break;
        case ISO_TP_RESET:
            CAN_changeOpMode(CAN_DISABLE);
            asm ("reset");
            break;
        case ISO_TP_SLEEP:
            Tsk_Sleep();
            break;
        case ISO_TP_IO_CONTROL:
            break;
        default:
            break;
    }
    
    CAN_populate_1ms();
    CAN_send_1ms();
}


/**
 * Runs every 10ms
 */
void Tsk_10ms(void) {
    IO_Efuse_Run_10ms(); //Run the Efuse System
    IgnitionControl_Run_10ms();
    HornControl_Run_10ms(); //Run Horn. Horn is disabled if button is held for too long.
    
    CAN_populate_10ms();
    CAN_send_10ms();
}

/**
 * Runs every 100ms
 */
void Tsk_100ms(void) {
    SerialConsole_Run_100ms(); //Debug Serial Terminal Emulation
    LightsControl_Run_100ms(); //Run the System Lights layer (Responds to button presses, controls, etc...)
    HeatedGripControl_Run_100ms(); //Run Heated Grips. Currently activated by spare sw 2
    j1772Control_Run_100ms(); //Run j1772 Proximity and Pilot Signal Control.
    
    CAN_populate_100ms();
    CAN_send_100ms();
}

/**
 * Runs every 1000ms
 */
void Tsk_1000ms(void) {
    IO_SET_DEBUG_LED_EN(TOGGLE); //Toggle Debug LED at 1Hz for scheduler running status
}

//TODO: Delete this.
void Tsk_Sleep(void){
    Sleep();
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

