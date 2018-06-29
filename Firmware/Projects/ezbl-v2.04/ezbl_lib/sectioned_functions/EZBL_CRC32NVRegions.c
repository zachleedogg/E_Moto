/*******************************************************************************
  Easy Bootloader for PIC24/dsPIC33 and PIC32MM Library Source File (ezbl_lib)

  Summary:
    Computes a CRC32 over a logical region of Flash

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


typedef union
{
    unsigned long long  u64;
    unsigned long       u32[2];
    unsigned short      u16[4];
    unsigned char       u8[8];
} EZBL_UNION_64;


#if defined(__XC16__)

unsigned long EZBL_CRC32Prog(unsigned long crcSeed, unsigned long programAddress, unsigned long byteLength);
unsigned long EZBL_Mul3Div2L(unsigned long addrToByteNumber);
#define EZBL_Mul3Div2L(addrToByteNumber) /* macro for inline compiler multiply and divide when addrToByteNumber is a constant */ \
    (__builtin_constant_p(addrToByteNumber) ? (((unsigned long)(addrToByteNumber))*3ul/2u) : EZBL_Mul3Div2L(addrToByteNumber))
unsigned long long EZBL_ReadTablePair(unsigned long *pgmReadAddr);
unsigned long EZBL_CRC32Prog(unsigned long crcSeed, unsigned long programAddress, unsigned long byteLength);

#elif defined(__XC32__) || defined(__PIC32__)

unsigned long EZBL_CRC32(unsigned long crcSeed, const void *data, unsigned int byteLength);
#define EZBL_CRC32Prog(crcSeed, readAddress, byteLength)     EZBL_CRC32(crcSeed, (const void*)(readAddress), byteLength)
#define EZBL_Mul3Div2L(addrToByteNumber)    ((unsigned long)(addrToByteNumber))  // Don't change the number on PIC32 devices since an address is a just as good as a byte offset on this architecture
static inline unsigned long long __attribute__((always_inline)) EZBL_ReadTablePair(unsigned long *pgmReadAddr)
{
    EZBL_UNION_64 data;
    data.u32[0] = *pgmReadAddr++;
    data.u32[1] = *pgmReadAddr++;
    return data.u64;
}

#endif


/**
 * DEPRECATED: This is a legacy API used with EZBL v1.xx code. This function
 * cannot be used with EZBL v2.00+ bootloaders.
 *
 * Computes a CRC-32 (IEEE 802.3 Ethernet version) over all Flash and 
 * non-volatile addresses containing the Bootloader and reserved data,
 * the Application (any non-Bootloader area), or the entire device's implemented
 * non-volatile memories (both).
 *
 * The .text.EZBLDevSpaceGeometry, .text.EZBLBootSpaceGeometry, and
 * .text.EZBLAppSpaceGeometry sections must be defined in your device linker
 * script for this function to be used. Otherwise a link-time error will be
 * triggered.
 *
 * @param typeOfRegions unsigned integer specifying which regions to include in
 *                      the CRC:
 *                          0 - all non-volatile addresses present on the device
 *                          1 - all bootloader occupied or reserved addresses
 *                          2 - all application occupied or free space addresses
 *                              (everything except bootloader occupied and
 *                              reserved addresses)
 *
 * @return Computed CRC32 over all of the regions occupied by the
 *         specified type.
 */
unsigned long EZBL_CRC32NVRegions(unsigned int typeOfRegions)
{
    EZBL_UNION_64 data;
    unsigned long pgmReadAddr;
    unsigned long crc;
    unsigned int entryAddrSize;

    pgmReadAddr     = __builtin_section_begin(".text.EZBLDevSpaceGeometry");
    entryAddrSize   = __builtin_section_size(".text.EZBLDevSpaceGeometry");
    if(typeOfRegions == 1u)
    {
        pgmReadAddr     = __builtin_section_begin(".text.EZBLNoProgramRanges");
        entryAddrSize   = __builtin_section_size(".text.EZBLNoProgramRanges");
    }
    else if(typeOfRegions == 2u)
    {
        pgmReadAddr     = __builtin_section_begin(".text.EZBLAppSpaceGeometry");
        entryAddrSize   = __builtin_section_size(".text.EZBLAppSpaceGeometry");
    }
   
    crc = 0;
    while(entryAddrSize)
    {
        data.u64 = EZBL_ReadTablePair(&pgmReadAddr);
        data.u32[1] = EZBL_Mul3Div2L(data.u32[1] - data.u32[0]);
        crc = EZBL_CRC32Prog(crc, data.u32[0], data.u32[1]);
        entryAddrSize -= 0x4u;
    }

    return crc;
}
