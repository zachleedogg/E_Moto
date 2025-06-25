/* 
 * File:   main.c
 * Author: kid group
 *
 * Created on February 24, 2018, 5:32 PM
 */


#include <xc.h>
#include "system.h"
#include "clock.h"
#include "tsk.h"


/*
 * 
 */
int main(void) {
    
    SYSTEM_Initialize();

    Tsk_Run(CLOCK_SystemFrequencyGet());

    while (1) {
    }

    return (0);
}

