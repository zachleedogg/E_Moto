/*******************************************************************************
  Multipurpose FIFO Source Library

  Summary:
    Implements a Flash Read back to .blob file formatting routine, writing the 
    output data to an EZBL_FIFO. 

  Description:
    Implements a Flash Read back to .blob file formatting routine, writing the 
    output data to an EZBL_FIFO. 
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Copyright (C) 2016 Microchip Technology Inc.

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

extern volatile unsigned int XC16_NEAR NVMCON;


/**
 * Reads the present Flash contents and writes it in a .blob file format (with 
 * zero SHA-1 hash field data) to a specified EZBL_FIFO.
 * 
 * @param *blobOutputFile Pointer to an EZBL_FIFO to write the Flash contents 
 *        formatted as a .blob file to.
 * 
 * @return Number of bytes written to the *blobOutputFile destination.
 */
unsigned long EZBL_Flash2Blob(EZBL_FIFO *blobOutputFile)
{
    unsigned int i;
    unsigned long recordSize;
    unsigned long recordAddress;
    unsigned long crc32;
    unsigned long buffer[4*3];
    unsigned int  chunkSize;
    unsigned long bytesWritten;
    unsigned long lSize;
    unsigned long readRange[6];
    
    
    readRange[0] = EZBL_WeakSYM32(EZBL_MAIN_FLASH_BASE);
    readRange[1] = EZBL_WeakSYM32(EZBL_MAIN_FLASH_END_ADDRESS);
    readRange[2] = EZBL_WeakSYM32(EZBL_AUX_FLASH_BASE);          // Aux Flash @ 0x7FC000 on dsPIC33EP512MU814 Family
    readRange[3] = EZBL_WeakSYM32(EZBL_AUX_FLASH_END_ADDRESS);   // Aux Flash end @ 0x800000 on dsPIC33EP512MU814 Family
    readRange[4] = EZBL_WeakSYM32(EZBL_CONFIG_BASE);             // "Non-volatile" Config bytes @ 0xF80004 on some devices (not Flash based Config Words)
    readRange[5] = EZBL_WeakSYM32(EZBL_CONFIG_END_ADDRESS);      // "Non-volatile" Config bytes end @ 0xF80012 on some devices (not Flash based Config Words)

#if defined(__XC16__)   // Features are not applicable to PIC32 devices
    if(!readRange[1])
    {
        readRange[1] = ((EZBL_SYM32(_CODE_BASE) + EZBL_SYM32(_CODE_LENGTH)) | 0xFF) + 1u;
    }
    if(EZBL_WeakSYM(_FLASH_PARTITION))
    {
        readRange[2] = readRange[0] | 0x400000ul;
        readRange[3] = readRange[1] | 0x400000ul;
    }
    if(readRange[4] < 0x800000ul)    // Do not dump Flash based config words. These are handled by the main Flash array(s)
    {
        EZBL_RAMSet(&readRange[4], 0x00, sizeof(readRange[4])*2u);
    }
#endif
    
    // Compute sizes for each block and total .blob file size
    lSize = 28ul + 4ul; // .blob header + 4 byte CRC32 footer
    for(i = 1; i < sizeof(readRange)/sizeof(readRange[0]); i+=2u)
    {
        readRange[i] = EZBL_Mul3Div2L(readRange[i] - readRange[i-1]);
        if(!readRange[i])
            continue;
        lSize += 8ul + readRange[i];
    }
    

    // Send whole .blob file header
    EZBL_RAMSet(buffer, 0x00, sizeof(buffer));          // Zero hash bytes
    buffer[0] = lSize;          // Record length field
    buffer[1] = 0x424F4C42u;    // Record address field: 'BLOB'
    bytesWritten = EZBL_FIFOWrite(blobOutputFile, buffer, 28);
    crc32 = EZBL_CRC32(0x00000000, buffer, 28);

    // Send record length/address header, assuming this memory block is present
    for(i = 1; i < sizeof(readRange)/sizeof(readRange[0]); i+=2u)
    {
        if(!readRange[i])
            continue;
        recordAddress = readRange[i-1];
        recordSize = readRange[i] + 8u;
        buffer[0] = recordSize;       // Record length field
        buffer[1] = recordAddress;    // Record address field
        bytesWritten += EZBL_FIFOWrite(blobOutputFile, buffer, 8);
        crc32 = EZBL_CRC32(crc32, buffer, 8);

#if defined(__XC16__)
        if(EZBL_WeakSYM(_FLASH_PARTITION))
        {
            recordAddress ^= ((unsigned long)(NVMCON&(1u<<10)))<<12;
        }
#endif
        // Send record data
        for(chunkSize = sizeof(buffer); readRange[i]; readRange[i] -= chunkSize)
        {
            if(((unsigned long)chunkSize) > readRange[i])
                chunkSize = (unsigned int)readRange[i];
            EZBL_ReadPackedFlash(buffer, recordAddress, 0, chunkSize);
            crc32 = EZBL_CRC32(crc32, buffer, chunkSize);
            bytesWritten += EZBL_FIFOWrite(blobOutputFile, buffer, chunkSize);
            recordAddress += EZBL_Div3Mul2L(sizeof(buffer), 0);
        }
    }

    // Output final CRC32
    bytesWritten += EZBL_FIFOWrite(blobOutputFile, &crc32, 4);  

    return bytesWritten;
}
