/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  dsPIC33EP512MU810
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.50
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/can1.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/tmr1.h"
#include "can_tp.h"
#include "mcc_generated_files/boot/boot_demo.h"
#include "mcc_generated_files/uart1.h"

/*
                         Main application
 */

uint8_t rxBuffer[512];
uint8_t bootByte[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t dummyByte[] = {1,2,3,4,5,6,7,8};
CAN_MSG_OBJ myMessage;
CAN_MSG_FIELD myField;

uint16_t counter_led;
uint16_t counter_can;

static bool can_flag = false;

void send_boot_message(void);

void TMR1_CallBack(void)
{
    // run the CAN TP tick
    CAN_TP_Tick();
    
    //Blink the Debug LED
    counter_led++;
    if (counter_led == 250){
        DEBUG_LED_Toggle();
        counter_led = 0;
    }
    
    counter_can++;
    if (counter_can == 250){
        //can_flag = true;
        counter_can = 0;
    }


}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    BOOT_DEMO_Initialize();
    CAN_SLEEP_SetLow();
    
    CAN_TP_Initialize();
    CAN_TP_RxMessageBufferSet(rxBuffer, sizeof(rxBuffer)); 
    
    //This is a phy level type of call.  It does not belong here....
    //Can we call from   BOOT_COM_Initialize();???
    CAN1_TransmitEnable(); 
    CAN1_ReceiveEnable();
    
    send_boot_message();
    
    char hello[] = "Bootloader\n";
    uint8_t i = 0;
    for (i=0;i<sizeof(hello);i++){
        UART1_Write(hello[i]);
    }


    while (1)
    {       
        CAN_TP_Tasks();
        BOOT_DEMO_Tasks();
        
        if(can_flag){
            can_flag = false;
            send_boot_message();
            
        }

    }
        
    return 1;
}

void send_boot_message(void){
    //Send a boot ID message
    myField.idType = 0;
    myField.dlc = 0b1000;
    myField.frameType = 0;
    myMessage.msgId = 0xA2;
    myMessage.data = dummyByte;
    myMessage.field = myField;
    CAN1_Transmit(CAN_PRIORITY_LOW, &myMessage);
}
/**
 End of File
*/

