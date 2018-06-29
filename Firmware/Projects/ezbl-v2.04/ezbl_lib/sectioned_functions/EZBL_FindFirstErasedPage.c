/*******************************************************************************
  Easy Bootloader for PIC24/dsPIC and PIC32MM (ezbl_lib)

  Summary:
    Implements the EZBL_FindFirstErasedPage() ezbl_lib API

  Description:
    Sequentially searches Flash addresses starting at the specified address and
    looks for a Flash page that contains no data (is all '1's).
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
 * Finds the first Flash page containing all 0xFFFFFF contents (i.e. is 
 * completely erased).
 * 
 * @param startAddress Program Flash address to start searching from. The page 
 *                     that contains this address is tested first.
 * @param searchDirection 
 *                     > 1 Searching will ascend to bigger addresses and 
 *                     stop at the maximum legal Flash address applicable on the 
 *                     device or Flash partition (for devices in a Dual 
 *                     Partition mode or with Auxiliary Flash) that the 
 *                     startAddress resides on.
 * 
 *                     == 0 Reserved
 *                     
 *                     <= -1 Searching will descend to lower addresses until 
 *                     address 0x000000 is reached or the first legal address of 
 *                     the startAddress's physical partition.
 *
 * @return             Address of the first address of the first Flash Page 
 *                     encountered that is entirely blank. If no erased Flash 
 *                     pages are found, 0xFFFFFFFF is returned.
 */
unsigned long EZBL_FindFirstErasedPage(unsigned long startAddress, int searchDirection)
{
    unsigned long terminateLimit;
    unsigned long auxFlashAddr;

    // Align to the start of a page for testing
    startAddress &= ~((unsigned long)(EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR) - 1u));

    // Check get the base address of Auxiliary Flash, if it exists. (0x000000 when it doesn't exist)
    auxFlashAddr = EZBL_WeakSYM32(EZBL_AUX_FLASH_BASE);

    // Enumerate all ordinary pages sequentially growing. 
    if(searchDirection >= 0u)
    {
        terminateLimit = EZBL_SYM32(EZBL_MAIN_FLASH_END_ADDRESS);
        if(auxFlashAddr && (startAddress >= auxFlashAddr))
        {
            terminateLimit = EZBL_WeakSYM32(EZBL_AUX_FLASH_END_ADDRESS);
        }
        else if(startAddress & 0x400000)
        {
            terminateLimit |= 0x400000;  // Copy bit 22 over if we are Dual Partition
        }

        do
        {
            // Return this page if it erased
            if(EZBL_IsPageErased(startAddress))
            {
                return startAddress;
            }
            startAddress += EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR);
        } while(startAddress < terminateLimit);
        
    }
    else
    {
        // Enumerate all ordinary pages sequentially shrinking.
        terminateLimit = 0;
        if(auxFlashAddr && (startAddress >= auxFlashAddr))
        {
            terminateLimit = auxFlashAddr;
        }
        else if(startAddress & 0x400000)
        {
            terminateLimit |= 0x400000;  // Copy bit 22 over if we are Dual Partition
        }


        startAddress += EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR); // Adjust start address since we shall pre-decrement in the loop
        do
        {
            startAddress -= EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR);

            // Return this page if it erased
            if(EZBL_IsPageErased(startAddress))
            {
                return startAddress;
            }
        } while(startAddress != terminateLimit);
    }
    
    // No erased pages found. Return 0xFFFFFFFF error code
    return 0xFFFFFFFF;
}
