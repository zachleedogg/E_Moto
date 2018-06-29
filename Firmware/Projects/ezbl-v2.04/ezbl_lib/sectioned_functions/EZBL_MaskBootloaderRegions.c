/*******************************************************************************
  Easy Bootloader for PIC24/dsPIC and PIC32MM (ezbl_lib)

  Summary:
    Implements the EZBL_MaskBootloaderRegions() ezbl_lib API

  Description:
    Modifies an array of data specified to start at a given address to never
    write a '0' on top of a Bootloader reserved Flash location.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Copyright (C) 2017 Microchip Technology Inc.

  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
  derivatives created by any person or entity by or on your behalf, exclusively
  with Microchip's products.  Microchip and its licensors retain all ownership
  and intellectual property rights in the accompanying software and in all
  derivatives here to.

  This software and any accompanying information is for suggestion only.  It
  does not modify Microchip's standard warranty for its products.  You agree
  that you are solely responsible for testing the software and determining its
  suitability.  Microchip has no obligation to modify, test, certify, or
  support the software.

  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
  TERMS.
*******************************************************************************/
// DOM-IGNORE-END

#include "../ezbl.h"


/**
 * DEPRECATED: DO NOT USE UNLESS YOU STILL HAVE __EZBL_ROM_USE_TABLE DATA BEING
 * GENERATED IN LINKER SCRIPT.
 *
 * Determines if the given program memory address and length overlaps with any
 * bootloader reserved regions. If they do and the write destination is Flash
 * memory, then the writeData for the overlapping region is changed to all '1's
 * so as not to damage the bootloader when programmed with the EZBL_WriteROM()
 * function. If address overlap exists and the write destination is in a
 * Non-volatile Configuration Words area (addresses of 0xF8xxxx), then the
 * overlapping region is changed to match the bootloader specified values.
 *
 * @param flashWriteAddress Specifies the starting program memory address that
 *                          this write data is intended for.
 *
 * @param *writeData Specifies a pointer to the data planned for write to
 *                   program memory. If this parameter is null (0x0000), no data
 *                   masking is performed. However, the number of bytes that
 *                   would have been masked off is still computed and returned.
 *
 * @param writeByteLen Number of bytes of intended write data that needs to be
 *                     masked off or checked for overlap with bootloader
 *                     reserved regions.
 *
 * @return Returns the byte count that has been masked off (or would have been
 *         masked off if data were provided). A return value of 0 means the
 *         specified write address range did not overlap with any bootloader
 *         reserved memory address regions.
 */
unsigned int EZBL_MaskBootloaderRegions(unsigned long flashWriteAddress, void *writeData, unsigned int writeByteLen)
{
    unsigned long bootBlockStartAddress;// Bootloader address
    unsigned long bootBlockEndAddress;  // Bootloader address
    unsigned long tableReadAddress;     // Where to get bootloader addresses
    unsigned int bootBlockCount;        // Number of bootloader reserved address regions
    unsigned long flashWriteEndAddress; // Target Flash address end (non-inclusive)
    unsigned int writeAddressLen;
    unsigned int writeByteRemainder;
    unsigned char *maskingStartData;
    unsigned int maskingLen;
    unsigned int totalBytesMasked;
    unsigned int i;

    totalBytesMasked = 0;

    // Convert writeByteLen in bytes to Flash program addresses
    // (writeByteLen/3*2) and compute the ending address for our write range
    writeAddressLen = EZBL_Div3Mul2(writeByteLen, &writeByteRemainder);
    if(writeByteRemainder)  // Go for another Flash word if value isn't aligned and a remainder exists
    {
        writeAddressLen += 2;
    }
    flashWriteEndAddress = flashWriteAddress + writeAddressLen;

    // Read the bootloader address out of Flash (table specified in linker script)
    tableReadAddress = EZBL_SYM32(EZBL_ROM_USE_TABLE_BASE);
    bootBlockCount = (unsigned int)EZBL_ReadFlash(tableReadAddress);
    tableReadAddress += sizeof(unsigned int);   // 2 addresses for PIC24/dsPIC, 4 addresses for PIC32
    while(bootBlockCount--)
    {
        // Get a bootloader reserved region start and end program addresses
        bootBlockStartAddress = EZBL_ReadFlash(tableReadAddress);
        tableReadAddress += sizeof(unsigned int);   // 2 addresses for PIC24/dsPIC, 4 addresses for PIC32
        bootBlockEndAddress = EZBL_ReadFlash(tableReadAddress);
        tableReadAddress += sizeof(unsigned int);   // 2 addresses for PIC24/dsPIC, 4 addresses for PIC32

        // Check for overlap with intended write range. If the address doesn't
        // overlap with the bootloader, skip further checking and masking of
        // this range and move to the next immediately.
        if(flashWriteEndAddress <= bootBlockStartAddress)
            continue;
        if(flashWriteAddress >= bootBlockEndAddress)
            continue;

        // Address overlap: compute overlapping RAM pointer address and length
        maskingStartData = writeData;
        maskingLen = writeByteLen;
        if(bootBlockStartAddress > flashWriteAddress)
        {
            i = (unsigned int)EZBL_Mul3Div2((unsigned int)(bootBlockStartAddress - flashWriteAddress));
            maskingStartData += i;
            maskingLen -= i;
        }
        if(bootBlockEndAddress < flashWriteEndAddress)
        {
            maskingLen -= (unsigned int)EZBL_Mul3Div2((unsigned int)(flashWriteEndAddress - bootBlockEndAddress));
        }

        // Sum of total masked length for return
        totalBytesMasked += maskingLen;

        // Mask off bytes, if non-null pointer
        if(writeData)
        {
            // Check if the destination is in "Non-volatile" Config Word space.
            if(((unsigned char *)&flashWriteAddress)[2] != 0xF8u)
            {// Destination is ordinary Flash program memory or Auxiliary Flash -> mask to all '1's
                EZBL_RAMSet(maskingStartData, 0xFF, maskingLen);
            }
        }
    }

    return totalBytesMasked;
}
