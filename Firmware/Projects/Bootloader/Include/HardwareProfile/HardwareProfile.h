/********************************************************************
 FileName:      HardwareProfile.h
 Dependencies:  See INCLUDES section
 Processor:     dsPIC33E/PIC24E 
 Hardware:      The code is natively intended to be used on the 
                  following hardware platforms: 
                  Explorer 16
                  USB Starter Kit
                  The firmware may be modified for use on other 
                  platforms by editing this file (HardwareProfile.h)
 Compiler:  	Microchip C30 
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************/


#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

#if defined(DEMO_BOARD_EXPLORER_16)
	#include "HardwareProfile_dsPIC33E_PIC24E_PIM_Explorer_16.h"	
#elif defined(DEMO_BOARD_USB_STARTER_KIT) // Same can be used for Ethernet Starterkit
	#include "HardwareProfile_dsPIC33E_PIC24E_USB_StarterKit.h"
#else

	/* Note ****: User has to define board type depending on the development board. 
	To do this, in the MPLAB IDE navigate to menu Project->Build Options->Project.
	Select "MPLAB C30" tab. Select categories as "General" from the dropdown list box.
	Click ADD button and define the DEMO_BOARD under "Preprocessor Macros".*/
	
	#error ("Demo board is either not defined or not defined properly. \
			 Supported values for this macro are BOARD_EXPLORER_16/ BOARD_USB_STARTER_KIT.");
#endif

#endif  //HARDWARE_PROFILE_H
