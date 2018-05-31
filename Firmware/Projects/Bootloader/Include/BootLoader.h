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
#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#define SYS_FREQ (60000000L)

//-------User configurable macros begin---------
#define MAJOR_VERSION 1
#define MINOR_VERSION 0

// Following things are not required, since we are block erasing the program FLASH.
/*#define ERASE_INSTRUCTION_SIZE_KB  3 //KB   // 1K instruction word (each word is 3 bytes)
#define APP_FLASH_TOTAL_SIZE_KB    512 //KB
#define APP_FLASH_BASE_ADDRESS 	0x00000
#define APP_FLASH_TOTAL_PAGES   (APP_FLASH_TOTAL_SIZE_KB/ERASE_INSTRUCTION_SIZE_KB)     //Each page is of 1K instruction word in size (ie, 3KB).
*/

/* Put here the address of  the Flash from where the application starts executing */
#define USER_APP_RESET_ADDRESS 		0
//-------User configurable macros end-----------



#if defined(TRANSPORT_LAYER_UART)
	#include "..\Include\Uart\Uart.h"
#elif defined(TRANSPORT_LAYER_USB)
	#include "Include\Usb\Usb_Tasks.h"
#else
	/* Note ****: User has to define TANSPORT_LAYER . 
	To do this, in the MPLAB IDE navigate to menu Project->Build Options->Project.
	Select "MPLAB PIC32 C Compiler" tab. Select categories as "General" from the dropdown list box.
	Click Add button and define the TANSPORT_LAYER under "Preprocessor Macros".*/

	#error ("TRANSPORT_LAYER is either not defined or not defined properly. Supported values for this macro are TL_UART/ TL_USB/ TL_ETH.");
#endif
#endif
