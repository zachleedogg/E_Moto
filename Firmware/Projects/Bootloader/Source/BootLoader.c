/*********************************************************************
 *
 *                  dsPIC33E/PIC24E Bootloader
 *
 *********************************************************************
 * FileName:        Bootloader.c
 * Dependencies:
 * Processor:       dsPIC33E/PIC24E 
 *
 * Compiler:        MPLAB C30
 *                  MPLAB IDE
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its dsPIC3E/PIC24E Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip dsPIC3E/PIC24E Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * $Id:  $
 * $Name: $
 *
 **********************************************************************/
#include "..\Include\GenericTypeDefs.h"
#include "..\Include\HardwareProfile\HardwareProfile.h"
#include "..\Include\Bootloader.h"
#include "..\Include\FrameWork\Framework.h"
#include <stdlib.h>
#if defined(__PIC24E__)
#include <p24Exxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif



_FOSCSEL(FNOSC_PRIPLL & IESO_ON)
_FOSC(POSCMD_HS & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSECME)
_FWDT(FWDTEN_OFF);
_FPOR(FPWRT_PWR1 & BOREN_ON & ALTI2C1_OFF & ALTI2C2_OFF);
_FICD(ICS_PGD2 & RSTPRI_AF & JTAGEN_OFF);


#define SWITCH_PRESSED 0



BOOL CheckTrigger(void);
void JumpToApp(void);
BOOL ValidAppPresent(void);
void InitializeBoard(void);

/********************************************************************
 * Function: 	main()
 *
 * Precondition: 
 *
 * Input: 		None.
 *
 * Output:		None.
 *
 * Side Effects:	None.
 *
 * Overview: 	Main entry function. If there is a trigger or 
 *				if there is no valid application, the device 
 *				stays in firmware upgrade mode.
 *
 *			
 * Note:		 	None.
 ********************************************************************/
INT main(void) {

    UINT16 timer = 0;
    UINT8 active = 0;

    InitializeBoard();

    // Initialize the transport layer - UART/USB/Ethernet
    TRANS_LAYER_Init();

    PutChar('h');
    PutChar('e');
    PutChar('l');
    PutChar('l');
    PutChar('o');



    while (!FRAMEWORK_ExitFirmwareUpgradeMode()) // Be in loop till framework recieves "run application" command from PC
    {
        // Enter firmware upgrade mode.
        // Be in loop, looking for commands from PC

        UINT8 temp = TRANS_LAYER_Task(); // Run Transport layer tasks
        if(temp){
            active = 1;
        }
        FRAMEWORK_FrameWorkTask(); // Run frame work related tasks (Handling Rx frame, process frame and so on)
        // Blink LED (Indicates the user that bootloader is running).
        BlinkLED();
        if (checkTimer() & (active == 0)) {
                timer++;
            }
        if ((timer >= 2) & ValidAppPresent()) {
            break;
        }

    }
    // Close trasnport layer.
    TRANS_LAYER_Close();



    // No trigger + valid application = run application.
    JumpToApp();

    return 0;
}

/********************************************************************
 * Function: 	InitializeBoard()
 *
 * Precondition: 
 *
 * Input: 		None.
 *
 * Output:		None
 *
 * Side Effects:	None.
 *
 * Overview: 	Intializes the controller to operate at 60MIPS.
                Initializes USB peripheral, Timer and LEDs.
 *
 *			
 * Note:		 	None.
 ********************************************************************/
void InitializeBoard(void) {
    // Setup configuration
    PLLFBD = 58; // M=60         

    CLKDIVbits.PLLPOST = 0; // N1=2
    CLKDIVbits.PLLPRE = 0; // N2=2
    OSCTUN = 0; // Tune FRC oscillator, if FRC is used

    // Disable Watch Dog Timer
    RCONbits.SWDTEN = 0;

    // Clock switching to incorporate PLL
    __builtin_write_OSCCONH(0x03); // Initiate Clock Switch to Primary

    // Oscillator with PLL (NOSC=0b011)
    __builtin_write_OSCCONL(0x01); // Start clock switching
    while (OSCCONbits.COSC != 0b011);

    // Wait for Clock switch to occur	
    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1) {
    };

#if defined(TRANSPORT_LAYER_USB)
    //USB h/w initialization.
    ACLKCON3 = 0x24C1;
    ACLKDIV3 = 0x7;

    ACLKCON3bits.ENAPLL = 1;
    while (ACLKCON3bits.APLLCK != 1);
#endif    
    // Timer setup for LED blinking.
    T1CONbits.TON = 0;
    T1CONbits.TGATE = 0;
    T1CONbits.TCKPS = 3;
    T1CONbits.TCS = 0;
    PR1 = 0xFFFF;
    T1CONbits.TON = 1;


    InitLED();

}

/********************************************************************
 * Function: 	CheckTrigger()
 *
 * Precondition: 
 *
 * Input: 		None.
 *
 * Output:		TRUE: If triggered
                FALSE: No trigger
 *
 * Side Effects:	None.
 *
 * Overview: 	Checks if there is a trigger to enter 
                firmware upgrade mode.
 *
 *			
 * Note:		 	None.
 ********************************************************************/
BOOL CheckTrigger(void) {
    UINT SwitchStatus;


    SwitchStatus = ReadSwitchStatus();
    if (SwitchStatus == SWITCH_PRESSED) {
        // Switch is pressed
        return TRUE;
    } else {
        // Switch is not pressed.
        return FALSE;
    }
}

/********************************************************************
 * Function: 	JumpToApp()
 *
 * Precondition: 
 *
 * Input: 		None.
 *
 * Output:		
 *
 * Side Effects:	No return from here.
 *
 * Overview: 	Jumps to application.
 *
 *			
 * Note:		 	None.
 ********************************************************************/
void JumpToApp(void) {
    void (*fptr)(void);
    fptr = (void (*)(void))USER_APP_RESET_ADDRESS;
    fptr();
}

/********************************************************************
 * Function: 	ValidAppPresent()
 *
 * Precondition: 
 *
 * Input: 		None.
 *
 * Output:		TRUE: If application is valid.
 *
 * Side Effects:	None.
 *
 * Overview: 	Logic: Check application vector has 
                some value other than "0xFFFFFF"
 *
 *			
 * Note:		 	None.
 ********************************************************************/
BOOL ValidAppPresent(void) {
    volatile DWORD AppPtr;

    TBLPAG = 0x00;

    AppPtr = ((DWORD) __builtin_tblrdh(USER_APP_RESET_ADDRESS) << 16);
    AppPtr = AppPtr | ((DWORD) __builtin_tblrdl(USER_APP_RESET_ADDRESS));

    if (AppPtr == 0xFFFFFF) {
        return FALSE;
    } else {
        return TRUE;
    }
}


/*********************End of File************************************/


