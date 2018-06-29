/*******************************************************************************
  Easy Bootloader for PIC24 and dsPIC33 Library API Source File

  File Name:
    EZBL_CRC32Prog.c

  Summary:
    Computes a IEEE 802.3 Ethernet style CRC32 checksum over a Program Flash 
    address range.

  Description:
    Computes a IEEE 802.3 Ethernet style CRC32 checksum over a Program Flash 
    address range by chunkwise calling the EZBL_ReadPackedFlash() and 
    EZBL_CRC32() EZBL APIs.

 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Copyright (C) 2015 Microchip Technology Inc.

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

unsigned long EZBL_CRC32(unsigned long crcSeed, const void *data, unsigned int byteLength);
#if defined(__XC16__)
void EZBL_ReadPackedFlash(void *dest, unsigned long baseAddress, unsigned int byteOffset, unsigned int byteCount);
#elif defined(__XC32__) || defined(__PIC32__)
void EZBL_RAMCopy(void *dest, const void *source, unsigned int length);
static void inline __attribute__((always_inline, optimize(1))) EZBL_ReadPackedFlash(void *dest, unsigned long baseAddress, unsigned int byteOffset, unsigned int byteCount)
{
    EZBL_RAMCopy(dest, (const void*)((baseAddress | 0x80000000u) + byteOffset), byteCount);
}
#endif


/**
 * Computes or continues a CRC-32 (IEEE 802.3 Ethernet version) over Flash data
 *
 * The polynomial implemented is:
 * x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
 * Commonly this is represented as 0xEDB88320 (or 0x04C11DB7 for big endian).
 *
 * This function needs ~120 bytes peak of temporary stack RAM.
 * 
 * @param crcSeed starting value to use in the shift register. Set to 0x00000000
 *                when starting a new CRC32 computation.
 * @param programAddress Program memory address to start reading from. This must
 *                       be aligned to an instruction boundary (i.e. always even
 *                       numbered) on 16-bit PICs/dsPICs.
 * @param byteLength unsigned long count of how many bytes to include in this
 *                   CRC computation starting at the specified address. This
 *                   need not be an integral multiple of the underlying Flash
 *                   storage instruction or word size.
 *
 * @return 32-bit CRC computed
 */
#if !(defined(__XC32__) || defined(__PIC32__))  // On PIC32 devices, this is implemented as a macro in ezbl.h that remaps to EZBL_CRC32() without an copy to RAM first.
unsigned long EZBL_CRC32Prog(unsigned long crcSeed, unsigned long programAddress, unsigned long byteLength)
{
    unsigned char buffer[16*6];
    unsigned int chunkLen;

    chunkLen = sizeof(buffer);

    while(chunkLen)
    {
        // Read each chunk of the sector from Flash
        while(byteLength >= chunkLen)
        {
            // Read a chunk from Flash
            EZBL_ReadPackedFlash(buffer, programAddress, 0, chunkLen);

            // Add it to the CRC
            crcSeed = EZBL_CRC32(crcSeed, buffer, chunkLen);

            // Increment read position
            programAddress += sizeof(buffer)/3u*2u;

            // Decrement counter
            byteLength -= chunkLen;
        }

        // Shrink the last chunk to match any residual bytes
        chunkLen = (unsigned int)byteLength;
    }
    
    return crcSeed;
}
#endif
