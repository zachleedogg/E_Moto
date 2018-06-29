/*******************************************************************************
  Easy Bootloader for PIC24 and dsPIC33 Library API Source File

  Summary:
    Implements the EZBL_ModifyROM() high level wrapper function which allows
    arbitrary unaligned, byte sized write operations with word and multi-page
    straddling to flash as if it were EEPROM or (really slow) RAM. Does direct
    write when possible or read-erase-modify-write automatically when required.

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

extern int EZBL_ADDRESSES_PER_SECTOR;    // Linker symbol, not a real variable. Take address of it to get the real value.


/**
 * Writes an array of data bytes to flash, preserving all data around the array
 * like a byte writable EEPROM or RAM memory. If required, sequential page
 * read-modify-erase-write operations are performed.
 *
 * To restrict writing/erase-rewriting to FlashHoles and Application space, set
 * <b>EZBL_NVMKey = 0x03DF;</b> prior to calling this function. To force writing
 * to any flash address, including Bootloader reserved ranges, set
 * EZBL_NVMKey = 0xDEAD;
 *
 * To preserve flash endurance, save execution time, and avoid potential data
 * loss on device reset, the destination Flash range is first compared with the
 * input data and the function returns immediately if the destination range
 * already exactly matches the input data.
 *
 * Likewise, if the flash range is blank (contains all 0xFFFFFF_FFFFFF flash
 * double words in the required write addresses), then the data is written
 * directly into position, skipping any page erase steps.
 *
 * If the destination range cannot be written due to existing mismatched data, a
 * full Read-Modify-Erase-Write sequence is performed on a page-by-page basis.
 * This entails allocating a buffer on the stack to contain a full flash erase
 * page worth of data, which is typically 1536 or 3072 bytes long, depending on
 * device (set by EZBL_ADDRESSES_PER_SECTOR linker symbol).
 *
 * Execution time may be quite long and if there is any risk of running out of
 * RAM, the caller should disable interrupts or take other precautions to ensure
 * the required stack RAM space is available.
 *
 * @param destPgmAddr 24-bit flash program space destination address. This
 *                    address must be an even, instruction word aligned, but can
 *                    straddle a native flash double word boundary on devices
 *                    that have ECC or program in 48-bit flash double word
 *                    sized chunks.
 *
 *                    To minimize flash endurance loss and execution time, it is
 *                    best if any object written is allocated to a double word
 *                    aligned address (i.e. multiple of 0x4 addresses). This
 *                    ensures no two objects will share a flash double word and
 *                    require extra erase/write penalties when writing the
 *                    objects via sequential EZBL_ModifyROM() calls.
 *
 * @param srcData Data space pointer to an array of bytes to write. If this
 *                parameter is null, no erase or write operation takes place.
 *
 * @param byteLen Number of bytes to write to flash. 0 is allowed, as are values
 *                less than the native minimum flash programming word size. In
 *                such cases, all existing data in the shared bytes within the
 *                programming word are preserved unchanged.
 *
 * @return Flash contains new data in the specified address range.
 *
 *         EZBL_NVMKey is reset to 0x0000, even if the early data compare test
 *         resulted in skipping of all Flash Erase/Write operations, byteLen is
 *         zero, or srcData is null.
 */
void EZBL_ModifyROM(unsigned long destPgmAddr, void *srcData, unsigned int byteLen)
{
    unsigned int pageAddrSize;
    unsigned int pageSize;
    unsigned int pageLoMask;
    char *pageSave;
    unsigned long pageStartAddr;
    unsigned int pageByteOffset;
    unsigned int bytesOnPage;
    EZBL_UNION_64 rd;
    unsigned long rdAddr;
    unsigned int i;
    unsigned int j;
    int needErase;
    unsigned int unlockKey;

    unlockKey = EZBL_NVMKey + 1u;
    EZBL_NVMKey = 0x0000;

    if(!srcData)    // Abort on null source pointer
        return;

    // Data matches allowing us to skip everything?
    if(EZBL_ROMCompare(destPgmAddr, srcData, byteLen) == 0)
        return;


    pageAddrSize = ((unsigned int)&EZBL_ADDRESSES_PER_SECTOR); // 0x400 or 0x800 normally
    pageLoMask = pageAddrSize-1;
    pageSize = EZBL_Mul3Div2(pageAddrSize);             // Now pageSize is in bytes
    pageSave = (void*)0;                                // Null pointer to start

    pageStartAddr = (~((unsigned long)(pageLoMask))) & destPgmAddr;
    pageByteOffset = EZBL_Mul3Div2(((unsigned int)destPgmAddr) & pageLoMask);
    while(1)
    {
        bytesOnPage = pageSize - pageByteOffset;
        if(bytesOnPage > byteLen)
            bytesOnPage = byteLen;

        if(bytesOnPage == 0u)
            return;


        // Destination range is blank? We could skip read-erase-modify steps if so.
        needErase = 0;
        i = ((unsigned int)destPgmAddr) & 0x3u;
        rdAddr = destPgmAddr - i;  // Align to 0x4 address boundary
        if(i)
            i = 3;
        i += bytesOnPage;
        j = i % 6;
        if(j)
            i += 6u - j;                         // Must also end on a 48-bit boundary
        while(i)
        {
            rd.u64 = EZBL_ReadTablePair(&rdAddr);
            if((rd.u32[0] & rd.u32[1]) != 0xFFFFFFu)
            {
                needErase = 1;
                break;
            }
            i -= 6u;
        }

        if(needErase)
        {
            if(!pageSave)                                                       // Allocate RAM page contents buffer onto stack only one time, and only if required
                pageSave = __builtin_alloca(pageSize);
            EZBL_ReadPackedFlash(pageSave, pageStartAddr, 0, pageSize);         // Read
            EZBL_RAMCopy(&pageSave[pageByteOffset], srcData, bytesOnPage);      // Modify Read Data
            if(unlockKey == (0xDEADu + 1u))
            {
                EZBL_NVMKey += unlockKey - (0xDEADu + 1u) + 0x03DFu;
                EZBL_ErasePage(pageStartAddr);                                  // Erase
                EZBL_NVMKey += unlockKey - (0xDEADu + 1u) + 0x03DFu;
                EZBL_WriteROMEx(pageStartAddr, pageSave, pageSize, 0, unlockKey-1u, 0); // Write anywhere, including Bootloader restricted space
            }
            else if(!EZBL_IsEraseRestricted(pageStartAddr))
            {
                EZBL_NVMKey += unlockKey - 1u;
                EZBL_ErasePage(pageStartAddr);                                  // Erase
                EZBL_NVMKey += unlockKey - 1u;
                EZBL_WriteROM(pageStartAddr, pageSave, pageSize);               // Write
            }
        }
        else
        {
            if(unlockKey == (0xDEADu + 1u))
            {
                EZBL_NVMKey += unlockKey - (0xDEADu + 1u) + 0x03DFu;
                EZBL_WriteROMEx(destPgmAddr, srcData, bytesOnPage, 0, unlockKey-1u, 0); // Write anywhere, including Bootloader restricted space
            }
            else
            {
                EZBL_NVMKey += unlockKey-1u;
                EZBL_WriteROM(destPgmAddr, srcData, bytesOnPage);               // Write only to FlashHoles or Application flash
            }
        }

        destPgmAddr = pageStartAddr + pageAddrSize;                             // Inc destPgmAddr to next page aligned address (incorrect address only for loop termination case)
        srcData = (void*)(((unsigned int)srcData) + bytesOnPage);               // Inc srcData, for compilers that treat addition to void pointers as an unknowable quantity
        byteLen -= bytesOnPage;                                                 // Dec bytes remaining
        pageByteOffset = 0;                                                     // Cheap, saves a EZBL_Mul3Div2() call
    }
}
