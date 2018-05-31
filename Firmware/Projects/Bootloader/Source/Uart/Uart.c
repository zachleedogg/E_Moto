/*********************************************************************
 *
 *                  dsPIC33E/PIC24E Boot Loader
 *
 *********************************************************************
 * FileName:        Uart.c
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
 * (the “Company”) for its PIC32 Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PIC32 Microcontroller products.
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
#if defined(__PIC24E__)
#include <p24Exxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif
#include <stdlib.h>
#include "..\Include\GenericTypeDefs.h"
#include "..\Include\Uart\Uart.h"
#include "..\Include\BootLoader.h"
#include "..\Include\Framework\Framework.h"
#include "..\Include\HardwareProfile\HardwareProfile.h"


static UINT8 TxBuff[255];
BOOL GetChar(UINT8 *byte);
void PutChar(UINT8 txChar);


/********************************************************************
 * Function: 	UartInit()
 *
 * Precondition: 
 *
 * Input: 		PB Clock
 *
 * Output:		None.
 *
 * Side Effects:	None.
 *
 * Overview:     Initializes UART.
 *
 *			
 * Note:		 	None.
 ********************************************************************/

/*void UartInit(void)
{
    TX_TRIS = 0;
    RX_TRIS = 1;    

    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)

        AD1PCFGL = 0xFFFF;
        RPINR19bits.U2RXR = 2;  	// assign RP2 to RX
        RPOR1bits.RP3R = 5;     	// assign RP3 to TX

    #endif

    #if defined(__PIC24FJ256GB110__) || defined(__PIC24FJ256GA110__) || defined(__PIC24FJ256GB210__)
    
        __builtin_write_OSCCONL(OSCCON & 0xbf);
        RPINR19bits.U2RXR = 10; 	// assign RP10 to RX
        RPOR8bits.RP17R = 5;    	// assign RP17 to TX
        __builtin_write_OSCCONL(OSCCON | 0x40); 
		
    #endif
    
    #if defined(__PIC24E__) || defined(__dsPIC33E__)
    __builtin_write_OSCCONL(OSCCON & 0xbf);
        RPOR9bits.RP101R = 3;		//Map U2TX to RF5   
        RPINR19bits.U2RXR = 0;
        RPINR19bits.U2RXR = 0x64;   //Map U2RX to RF4
    __builtin_write_OSCCONL(OSCCON | 0x40); 
    	
    #endif

    #if defined(__PIC24FJ256DA210__)
    
        __builtin_write_OSCCONL(OSCCON & 0xbf);
        _U2RXR = 11;				// Bring RX2 Input to RP11/RD0			
        _RP16R = 5; 				// Bring TX2 output on RP16/RF3
        __builtin_write_OSCCONL(OSCCON | 0x40); 
		
        OSCCON = 0x3302;    		// Enable secondary oscillator
        CLKDIV = 0x0020;    		// Set PLL prescaler (1:1), PLLEN = 1
		
    #endif    
	

    #if defined(__PIC32MX)
        U2BRG = (GetPeripheralClock() / 4 / BAUDRATE) - 1;
    #elif defined(__dsPIC33E__) || defined(__PIC24E__)
        U2BRG = (GetPeripheralClock() / 16 / 38400) - 1;
    #else    
        #if defined(__dsPIC33F__) || defined(__PIC24H__)
            // The processor frequency is set to 40MHz
            #define BRG_TEMP    10 * GetSystemClock() / 8 / BAUDRATE	
        #else
            #define BRG_TEMP    10 * GetPeripheralClock() / 4 / BAUDRATE
        #endif
		
        #if (BRG_TEMP - ((BRG_TEMP / 10) * 10)) >= 5
            U2BRG = BRG_TEMP / 10;
        #else
            U2BRG = BRG_TEMP / 10 - 1;
        #endif
    #endif

    U2MODE = 0;
    U2STA = 0;
    U2MODEbits.UARTEN = 1;
    U2MODEbits.STSEL = 0;
    U2STAbits.UTXEN = 1;

    #ifdef __PIC32MX
    U2STAbits.URXEN = 1;
    #endif
    
     #if !defined(__dsPIC33E__) && !defined(__PIC24E__)
    U2MODEbits.BRGH = 1;
    #endif

    U2STAbits.OERR = 0;
}*/


void UartInit(void) {

    ANSELG = 0x0000;
    // Set Rx and Tx pin I/Os
    TX_TRIS = 1;
    RX_TRIS = 1;
    // Map the UART I/Os to required Pins.
    MAP_UART_PINS_mac();


    // Set UART baudrate.
    UINT16 temp1 = (GetPeripheralClock() / 16 / DEFAULT_BAUDRATE) - 1;
    UxBRG = temp1;

    UxMODEbits.UARTEN = 1;
    UxMODEbits.STSEL = 0;
    UxSTAbits.UTXEN = 1;
    UxMODEbits.BRGH = 0;
    UxSTAbits.OERR = 0;

}

/********************************************************************
 * Function: 	UartClose()
 *
 * Precondition: 
 *
 * Input: 		None
 *
 * Output:		None.
 *
 * Side Effects:	None.
 *
 * Overview:     Closes UART connection.
 *
 *			
 * Note:		 	None.
 ********************************************************************/
void UartClose(void) {
    //TODO: do some closing operation if required.	
}

/********************************************************************
 * Function: 	UartTasks()
 *
 * Precondition: 
 *
 * Input: 		None
 *
 * Output:		True is char received or sent
 *
 * Side Effects:	None.
 *
 * Overview:     Runs some UART routines like receive bytes and transmit bytes.
 *
 *			
 * Note:		 	None.
 ********************************************************************/
UINT8 UartTask(void) {
    UINT8 activeChar = 0;
    UINT TxLen;
    UINT8 Rx;
    UINT8 *ptr;
    // Check any character is received.
    if (GetChar(&Rx)) {
        activeChar = 1;
        // Pass the bytes to frame work.
        FRAMEWORK_BuildRxFrame(&Rx, 1);
    }

    ptr = TxBuff;
    // Get transmit frame from frame work.
    TxLen = FRAMEWORK_GetTransmitFrame(ptr);

    if (TxLen) {
        activeChar = 1;
        // There is something to transmit.
        while (TxLen--) {
            PutChar(*(ptr++));
        }
    }

    return activeChar;
}

/********************************************************************
 * Function: 	GetChar()
 *
 * Precondition: 
 *
 * Input: 		None
 *
 * Output:		True: If there is some data.
 *
 * Side Effects:	None.
 *
 * Overview:     Gets the data from UART RX FIFO.
 *
 *			
 * Note:		 	None.
 ********************************************************************/
BOOL GetChar(UINT8 *byte) {

    // Wait for new data
    if (UxSTAbits.URXDA) {
        *byte = (UINT8) UxRXREG; // get data from UART RX FIFO
        // Clear error flag
        if (UxSTAbits.OERR) {
            UxSTAbits.OERR = 0;
        }
        return TRUE;
    }

    return FALSE;

}

/********************************************************************
 * Function: 	PutChar()
 *
 * Precondition: 
 *
 * Input: 		None
 *
 * Output:		None
 *
 * Side Effects:	None.
 *
 * Overview:     Puts the data into UART tx reg for transmission.
 *
 *			
 * Note:		 	None.
 ********************************************************************/
void PutChar(UINT8 txChar) {
    while (UxSTAbits.UTXBF); // wait for TX buffer to be empty
    UxTXREG = txChar;
}

/***************************************End Of File*************************************/
