/**********************************************************************
* © 2005 Microchip Technology Inc.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33Fxxxx/PIC24Hxxxx
* Compiler:        MPLAB® C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
*
* REVISION HISTORY:
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Settu D 			03/15/06  First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Code Tested on:
* Explorer 16 Demo board with dsPIC33FJ256GP710 controller &
* Explorer 16 Demo board with  PIC24HJ256GP610 controller 
* The Processor starts with the External Crystal without PLL enabled and then the Clock is switched to PLL Mode.
**********************************************************************/
#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#elif defined(__PIC24H__)
#include "p24Hxxxx.h"
#endif

#include "typeDef.h"
#include "rtspApi.h"
#include "testData.h"


//-----------------------------------------------------------------------------
//Macros for Configuration Fuse Registers:
//-----------------------------------------------------------------------------
//Invoke macros to set up  device configuration fuse registers.
//The fuses will select the oscillator source, power-up timers, watch-dog
//timers etc. The macros are defined within the device
//header files. The configuration fuse registers reside in Flash memory.
// External Oscillator
_FOSCSEL(FNOSC_PRI);			// Primary (XT, HS, EC) Oscillator 
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_XT);  
								// Clock Switching is enabled and Fail Safe Clock Monitor is disabled
								// OSC2 Pin Function: OSC2 is Clock Output
								// Primary Oscillator Mode: XT Crystanl

// Internal FRC Oscillator
//_FOSCSEL(FNOSC_FRCPLL );		// FRC Oscillator with PLL
//_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF  & POSCMD_NONE); 
								// Clock Switching and Fail Safe Clock Monitor is disabled
								// OSC2 Pin Function: OSC2 is Clock Output
								// Primary Oscillator Mode: Disabled


_FWDT(FWDTEN_OFF);              // Watchdog Timer Enabled/disabled by user software


//-----------------------------------------------------------------------------
int pageMirrorBuff[64*8];

// RTSP variable 
u16 nvmAdr,nvmAdru,nvmAdrPageAligned,nvmRow,nvmSize;

// Data to be written to Flash
i16	MyRowDataInRam[64] = { 0,1,2,3,4,5,6,7,8,9,
					   10,11,12,13,14,15,16,17,18,19,
					   20,21,22,23,24,25,26,27,28,29,
					   30,31,32,33,34,35,36,37,38,39,
					   40,41,42,43,44,45,46,47,48,49,
					   50,51,52,53,54,55,56,57,58,59,
					   60,61,62,63};

i16 main(void)
{
i16 i,temp;

	PSVPAG = 0;
	CORCONbits.PSV=1;

// Configure Oscillator to operate the device at 40Mhz
// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
// Fosc= 8M*40/(2*2)=80Mhz for 8M input clock
	PLLFBD=38;					// M=40
	CLKDIVbits.PLLPOST=0;		// N1=2
	CLKDIVbits.PLLPRE=0;		// N2=2
	OSCTUN=0;					// Tune FRC oscillator, if FRC is used

// Disable Watch Dog Timer
	RCONbits.SWDTEN=0;

// Clock switch to incorporate PLL
	__builtin_write_OSCCONH(0x03);		// Initiate Clock Switch to Primary
													// Oscillator with PLL (NOSC=0b011)
	__builtin_write_OSCCONL(0x01);		// Start clock switching
	while (OSCCONbits.COSC != 0b011);	// Wait for Clock switch to occur

// Wait for PLL to lock
	while(OSCCONbits.LOCK!=1) {};		

	nvmAdru=__builtin_tblpage(&MyRowData5InFlash[0]);
	nvmAdr=__builtin_tbloffset(&MyRowData5InFlash[0]);
	nvmAdrPageAligned=nvmAdr & 0xFC00;			// Get the Flash Page Aligned address
	nvmRow=((nvmAdr>>7) & 7);					// Row in the page	 				
	nvmSize=64;		

// Read the page and place the data into pageMirrorBuf array 
	temp = flashPageRead(nvmAdru,nvmAdrPageAligned, pageMirrorBuff);

// Modify the pageMirrorBuf array
	temp = flashPageModify(nvmRow, nvmSize, MyRowDataInRam, pageMirrorBuff);

// Erase the page in Flash
	temp = flashPageErase(nvmAdru,nvmAdrPageAligned);

// Program the page with modified data
	temp = flashPageWrite(nvmAdru,nvmAdrPageAligned,pageMirrorBuff);

// User can add code here to verify that flash is programmed correctly */

// Clear Page Mirror Buffer
	for (i=0; i<(64*8);i++) {
		pageMirrorBuff[i]=0;
	}

//Read the page and place the data into pageMirrorBuf array 
	temp = flashPageRead(nvmAdru,nvmAdrPageAligned, pageMirrorBuff);


	while(1); 
}


void __attribute__((interrupt, no_auto_psv)) _DefaultInterrupt(void)
{
	while(1) ClrWdt()
}

