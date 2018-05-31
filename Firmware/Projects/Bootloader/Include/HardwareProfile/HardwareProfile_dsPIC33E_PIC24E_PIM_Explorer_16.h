// Copyright (c) 2002-2010,  Microchip Technology Inc.
//
// Microchip licenses this software to you solely for use with Microchip
// products.  The software is owned by Microchip and its licensors, and
// is protected under applicable copyright laws.  All rights reserved.
//
// SOFTWARE IS PROVIDED "AS IS."  MICROCHIP EXPRESSLY DISCLAIMS ANY
// WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL
// MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
// CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR
// EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY
// OR SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED
// TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
// OR OTHER SIMILAR COSTS.
//
// To the fullest extent allowed by law, Microchip and its licensors
// liability shall not exceed the amount of fees, if any, that you
// have paid directly to Microchip to use this software.
//
// MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
// OF THESE TERMS.
#ifndef __HARDWAREPROFILE_PIC32MX_PIM_EXPLORER_16_H__
#define __HARDWAREPROFILE_PIC32MX_PIM_EXPLORER_16_H__


/*******************************************************************/
    /******** USB stack hardware selection options *********************/
    /*******************************************************************/
    //This section is the set of definitions required by the MCHPFSUSB
    //  framework.  These definitions tell the firmware what mode it is
    //  running in, and where it can find the results to some information
    //  that the stack needs.
    //These definitions are required by every application developed with
    //  this revision of the MCHPFSUSB framework.  Please review each
    //  option carefully and determine which options are desired/required
    //  for your application.

//    //#define USE_SELF_POWER_SENSE_IO
//    #define tris_self_power     TRISBbits.TRISB15    // Input
//    #define self_power          1
//
//    //#define USE_USB_BUS_SENSE_IO
//    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
//    #define USB_BUS_SENSE       1 
    
    // UART Tx and Rx pins
    #define TX_TRIS TRISGbits.TRISG8
    #define RX_TRIS TRISGbits.TRISG9


	#define mLED              LATAbits.LATA0
	// Blinks LED 5 on explorer 16 board
	#define BlinkLED() (mLED = ((TMR1 & 0x8000) != 0))
    #define checkTimer() (((TMR1 == 0x8000) != 0)?1:0)
    #define ToggleLED() (mLED = PORTAbits.RA0==1?0:1)
	#define InitLED() (TRISAbits.TRISA0 = 0)
	
	
	// Switch S3 on Explorer 16.
	#define ReadSwitchStatus() (PORTAbits.RA1)
    #define WriteSwitchStatus(x) (LATAbits.LATA1 = x)


// System clock and peripheral clock. 	
   	#define GetSystemClock()    (120000000ul) // 120MHz for 60MIPS operation.
    #define GetPeripheralClock()    (GetSystemClock() / 2)
    
	#if defined(TRANSPORT_LAYER_USB_HOST)
	
		// ******************* MDD File System Required Definitions ********************
		// Select your MDD File System interface type
		// This library currently only supports a single physical interface layer
		// In this example we are going to use the USB so we only need the USB definition
		// *****************************************************************************
		#define USE_USB_INTERFACE               // USB host MSD library
	#endif
   

#endif
