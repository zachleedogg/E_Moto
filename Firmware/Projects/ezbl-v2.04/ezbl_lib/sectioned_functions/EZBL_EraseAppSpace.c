/*******************************************************************************
  Easy Bootloader for PIC24/dsPIC and PIC32MM (ezbl_lib)

  Summary:
    Implements the EZBL_EraseAppSpace() ezbl_lib API

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

//#define EZBL_DEBUG     // Uncomment to allow debugging messages to be printed to stdout

#if !defined(EZBL_DEBUG)
#define EZBL_printf(...)        do{}while(0)
#define EZBL_FIFOFlush(...)     do{}while(0)
#endif

/**
 * Erases all flash pages dedicated to the Application (address ranges contained
 * in the ".text.EZBLAppSpaceGeometry" section) in a piecewise fashion.
 *
 * Set <b>EZBL_NVMKey = 0xFC21</b> before each call to this function.
 *
 * Since erasing flash pages requires a long execution time, this function
 * returns between each page erase operation, allowing housekeeping and other
 * tasks to be serviced before starting another page erase operation.
 *
 * @param *eraseState pointer to an unsigned int to keep internal track of which
 *                    page was last erased/which page will be erased next.
 *
 *                    On first call, set this memory location to 0x0000. Ignore
 *                    the return value on subsequent calls, but return the same
 *                    value when continuing the erase sequence.
 * @return <ul>
 *              <li>1 - Erase sequence was initiated for a block of memory. One
 *                      or more calls to EZBL_EraseAppSpace() are required to
 *                      complete the erase.</li>
 *              <li>0 - Erase of all blocks in the App space geometry are
 *                      complete.</li>
 *         </ul>
 */
int EZBL_EraseAppSpace(unsigned int *eraseState)
{
    EZBL_UNION_64 rd;
    unsigned long rdAddr;
    unsigned int tableAddrSize;
    unsigned int pageNum;

    rdAddr        = __builtin_section_begin(".text.EZBLAppSpaceGeometry");
    tableAddrSize = __builtin_section_size(".text.EZBLAppSpaceGeometry");

    EZBL_printf("\n    .text.EZBLAppSpaceGeometry stored at: [%06lX, %06lX)", rdAddr, rdAddr + tableAddrSize);

    pageNum = 0;
    while(tableAddrSize)
    {
        EZBL_printf("\n    Table value read from %06lX: ", rdAddr);
        rd.u64 = EZBL_ReadTablePair(&rdAddr);
        EZBL_printf("%06lX, %06lX", rd.u32[0], rd.u32[1]);
        tableAddrSize -= 0x4;
        while(rd.u32[0] < rd.u32[1])
        {
            pageNum++;

            if(pageNum > *eraseState)
            {
                *eraseState = pageNum;
                if(!EZBL_IsEraseRestricted(rd.u32[0]))  // Don't erase this address if it is in the .text.EZBLNoEraseRanges section, such as user pages reserved for emulated data EEPROM
                {
                    EZBL_printf("\n    %06lX not erase restricted", rd.data[0]);
                    if(rd.u8[2] >= 0xF8u)               // If we've hit some non-volatile Config bytes, restore their default values and call ourselves done
                    {
                        EZBL_RestoreAppErasable(1);
                        return 1;
                    }
                    else if(!EZBL_IsPageErased(rd.u32[0]))
                    {
                        EZBL_printf("\n    %06lX not erased, going to erase", rd.u32[0]);
                        EZBL_FIFOFlush(EZBL_STDOUT, (unsigned long)-1);
                        // Erase the page (clears EZBL_NVMKey on return)
                        EZBL_NVMKey += 0x03DF - 0xFC21;
                        EZBL_ErasePage(rd.u32[0]);
                        EZBL_printf("\n    %06lX erase done, now calling EZBL_RestoreAppErasable(0);", rd.u32[0]);
                        EZBL_FIFOFlush(EZBL_STDOUT, (unsigned long)-1);
                        EZBL_RestoreAppErasable(0);
                        EZBL_printf("\n    EZBL_RestoreAppErasable(0) complete");
                        EZBL_FIFOFlush(EZBL_STDOUT, (unsigned long)-1);
                        return 1;
                    }
                    EZBL_printf(", but is already erased");
                }
            }
            rd.u32[0] += EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR);
        }
    }

    EZBL_printf("\n    All done erasing pages");
    EZBL_FIFOFlush(EZBL_STDOUT, (unsigned long)-1);
    // No more erasable pages found, do nothing and return end of enum code
    EZBL_NVMKey = 0x0000;
    return 0;
}
