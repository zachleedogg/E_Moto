/* 
 * File:   main.c
 * Author: Zach Levenberg
 *
 * Created on August 6, 2016, 12:55 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "bolt_init.h"

#pragma config GWRP = OFF  // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF  // General Code Segment Code Protect (Code protection is disabled)

/*
 * 
 */
int main(void) {
    Micro_Init();
    clockInit(FREQ_80MHZ,EXTERNAL);
    
    
    
    while(1){;}

    return (EXIT_SUCCESS);
}

