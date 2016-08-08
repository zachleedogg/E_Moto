/**********************************************************************
* © 2005 Microchip Technology Inc.
*
* FileName:        rtsApi.h
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
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Settu D 			03/15/06  First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
*
**********************************************************************/
#include "typeDef.h"

#ifndef __RTSPAPI_H__
#define __RTSPAPI_H__ 



// Flash Memory is organised into ROWs of 64 instructions or 192 bytes
// RTSP allows the user to erage a PAGE of memory which consists of EIGHT ROWs (512 instructions or 1536byts) at a time.
// RTSP allows the user to program a ROW (64 instructions or 192 bytes) at a time

/*
 * FLASH PAGE ERASE
 *
 * Parameters Definition:
 * nvmAdru:	Selects the upper 8bits of the location to program or erase in program flash memory
 * nvmAdr:  Selects the location to program or erase in program flash memory
*           It must be aligned to 512 instruction boundary, LSB 10bits of address must be zero
 
 * Return Value:
 * Function returns ERROREE (or -1), if it is not successful
 * Function return ZERO, if successful
*/
extern int flashPageErase(u16 nvmAdru, u16 nvmAdr);


/*
 * FLASH PAGE READ
 *
 * Parameters Definition:
 * nvmAdru:	Selects the upper 8bits of the location to read in program flash memory
 * nvmAdr:  Selects the location to read in program flash memory
 *          It must be aligned to 512 instruction boundary, LSB 10bits of address must be zero
 * pageBufPtr: Pointer to the data array in which read data will be stored

 
 * Return Value:
 * Function returns ERROREE (or -1), if it is not successful
 * Function return ZERO, if successful
*/

extern int flashPageRead(u16 nvmAdru, u16 nvmAdr, i16 *pageBufPtr);


/*
 * FLASH PAGE MODIFY
 *
 * Parameters Definition:
 * row:		Selects the row in the the Flash page that will be modified
 * rowBuf:  Selects the location to read in program flash memory
 * pageBufPtr: Pointer to the page data array, that will be modified
 
 * Return Value:
 * Function returns ERROREE (or -1), if it is not successful
 * Function return ZERO, if successful
*/

extern int flashPageModify(u16 row, u16 size, i16 *rowBuf, i16 *pageBufPtr);




/*
 * FLASH PAGE WRITE
 *
 * Parameters Definition:
 * nvmAdru:	Selects the upper 8bits of the location to program or erase in program flash memory
 * nvmAdr:  Selects the location to program or erase in program flash memory
 *          It must be aligned to 512 instruction boundary, LSB 10bits of address must be zero
 * pageBufPtr: Pointer to the data array that needs to be programmed 


 * Return Value:
 * Function returns ERROREE (or -1), if it is not successful
 * Function return ZERO, if successful
*/
extern int flashPageWrite(u16 nvmAdru, u16 nvmAdr, i16 *pageBufPtr);



#endif

