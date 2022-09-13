/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    boot_demo.c

  @Summary:
    This is the boot_demo.c file generated using 16-bit Bootloader

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  16-bit Bootloader - 1.19.1
        Device            :  dsPIC33EP512MU810
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36B
        MPLAB             :  MPLAB X v5.15
*/
/*
Copyright (c) [2012-2022] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/
#include "../memory/flash.h"
#include "boot_process.h"

#include <stdbool.h>
#include <stdint.h>
#include "boot_config.h"
#include "boot_application_header.h"
#include "boot_process.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/can1.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/uart1.h"

#define DOWNLOADED_IMAGE    0u
#define EXECUTION_IMAGE     0u


static uint16_t bootloadTimeOutTime = 5000; //ms
static uint32_t bootloadLastTime = 0;

void BOOT_DEMO_Initialize(void)
{    
    TMR1_SoftwareCounterClear();
    bootloadLastTime = 0;
    if(RCONbits.SWR == 1 || RCONbits.WDTO == 1 || RCONbits.TRAPR || RCONbits.EXTR){
        bootloadTimeOutTime = 5000;
    } else {
        bootloadTimeOutTime = 250;
    }
    
    char hello[] = "Reset Reason: ";
    uint8_t i = 0;
    for (i=0;i<sizeof(hello);i++){
        UART1_Write(hello[i]);
    }
    
    uint8_t low = (uint8_t)RCON;
    uint8_t high = (uint8_t)(RCON>>8);
    
    for (i=0;i<8;i++){
        UART1_Write(((high >> (7-i)) & 0x01)+48);
    }
        for (i=0;i<8;i++){
        UART1_Write(((low >> (7-i)) & 0x01)+48);
    }
    UART1_Write(0xA);//newline
}


void BOOT_DEMO_Tasks(void)
{
    enum BOOT_COMMAND_RESULT res = BOOT_ProcessCommand();
    //UART1_Write(res+);//newline
    if (res == BOOT_COMMAND_SUCCESS){
        bootloadLastTime = TMR1_SoftwareCounterGet();
    }

    if (BOOT_Verify()){
        if (TMR1_SoftwareCounterGet() - bootloadLastTime > bootloadTimeOutTime){
            /* NOTE: Before starting the application, all interrupts used
             * by the bootloader must be disabled. Add code here to return
             * the peripherals/interrupts to the reset state before starting
             * the application code. */
             //#warning "All interrupt sources and peripherals should be disabled before starting the application.  Add any code required here to disable all interrupts and peripherals used in the bootloader."

            TMR1_Stop();
            CAN_SLEEP_SetHigh();
            CAN1_OperationModeSet(CAN_CONFIGURATION_MODE);
            CAN1_OperationModeSet(CAN_DISABLE_MODE);
            BOOT_StartApplication();
        }
    }

}