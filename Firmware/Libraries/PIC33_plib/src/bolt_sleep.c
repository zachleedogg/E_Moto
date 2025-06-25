/*
 * File:   bolt_sleep.c
 * Author: Zach Levenberg
 *
 * Created on February 10, 2016, 10:23 PM
 */

#include "bolt_sleep.h"
#include <xc.h>

void SleepNow() {
    _CNIF = 0;
    _CNIE = 1;

    Nop();
    Sleep();
    Nop();
    _CNIF = 0;
}

/*	Change Notification Interrupt*/
void __attribute__((__interrupt__, auto_psv)) _CNInterrupt(void) {
    _CNIF = 0;
}
