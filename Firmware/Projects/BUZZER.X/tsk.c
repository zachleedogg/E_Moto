#include "tsk.h"					// For this modules definitions
#include "tsk_cfg.h"
#include "SysTick.h"
#include <xc.h>


#include "bolt_OC.h"
#include "pins.h"
#include "pitches.h"

#define DEBUG_LED_EN PIN_DEF(B,6)
#define BUZZER PWM_PIN_RP36

void Tsk_init(void) {

    /* Configure all AD pins as digital */
#ifdef ANSELA
    ANSELA = 0x0000;
#endif
#ifdef ANSELB
    ANSELB = 0x0000;
#endif
#ifdef ANSELC
    ANSELC = 0x0000;
#endif
#ifdef ANSELD
    ANSELD = 0x0000;
#endif
#ifdef ANSELE
    ANSELE = 0x0000;
#endif
#ifdef ANSELF
    ANSELF = 0x0000;
#endif
#ifdef ANSELG
    ANSELG = 0x0000;
#endif


    /* Set the PWM's off */
#ifdef IOCON1
    IOCON1bits.PENH = 0;
    IOCON1bits.PENL = 0;
#endif
#ifdef IOCON2
    IOCON2bits.PENH = 0;
    IOCON2bits.PENL = 0;
#endif
#ifdef IOCON3
    IOCON3bits.PENH = 0;
    IOCON3bits.PENL = 0;
#endif

    PINS_direction(DEBUG_LED_EN, OUTPUT);
    pwmOCinit(BUZZER);
    pwmOCwriteDuty(BUZZER, 50);

}

void Tsk(void) {

}

void Tsk_10ms(void) {

    static uint32_t i = 0;
    static uint8_t time = 0;

    pwmOCwriteFreq(BUZZER, RickMelody[i]*2);

    if (time++ == 10*RickTempo[i]) {
        time = 0;
        i++;
    }
    
    if(i>=RickLenth){
        i=0;
        time=0;
    }


}

void Tsk_100ms(void) {




}

void Tsk_1000ms(void) {
    PINS_write(DEBUG_LED_EN, TOGGLE);





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
/*** End of File **************************************************************/

