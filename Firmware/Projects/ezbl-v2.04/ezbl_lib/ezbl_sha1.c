/*******************************************************************************
  Easy Bootloader for PIC24 and dsPIC33 SHA-1 Hashing Submodule Source File

  File Name:
    ezbl_sha1.c

  Summary:
    SHA-1 hashing implementation for use in the EZBL library.

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

// Special a filename based macro for including/excluding specific items from
// headers that might conflict with local definitions.
#define EZBL_SHA1_C

#include "ezbl.h"
#include "ezbl_core.h"
#include "ezbl_sha1.h"
#include "fifo.h"           // For FIFO_FEATURES flag definitions


// SHA1 hashing private functions
unsigned int EZBL_SHA1_FIFO_OnData(void *data, unsigned int dataLength);


// Macro for doing bit rotations on 32-bit values
#define leftRotateDWORD(x, n) (((x) << (n)) | ((x) >> (32-(n))))

// Define a macro to efficiently swap endianness of a 32-bit aligned value in
// RAM via a pointer.
// Example:
//      unsigned long data = 0x12345678;
//      unsigned long *ptr = &data;
//      BYTE_ENDIAN_SWAP_32(ptr);
// data will now contain 0x78563412
#define BYTE_ENDIAN_SWAP_32(aligned_pointer)           \
        __asm__("   mov.d %0,w2     \n"                \
                "   exch w2,w3      \n"                \
                "   swap w2         \n"                \
                "   swap w3         \n"                \
                "   mov.d w2,%0       "                \
                : "+g"(*aligned_pointer) : :  "w2", "w3")



// Instantiate SHA1 hash processing FIFO
#define FIFO_NAME                           EZBL_SHA1_FIFO_
#define FIFO_SIZE                           64         // Must be an integer multiple of 64 for SHA1 hash minimum block size
#define FIFO_ON_DATA_THRESHOLD_RESET_VALUE  64
#define FIFO_FEATURES                       FIFO_FEATURE_WRITABLE | FIFO_FEATURE_ON_DATA_LINEAR
#define FIFO_EXTRA_VARS                     unsigned int  opFlags;      /* Bitfield of EZBL_SHA1_* flags defined in ezbl_sha1.h */  \
                                            unsigned long length;       /* Number of bytes seen so far */                           \
                                            unsigned long hash[5];      /* 20-byte SHA1 hash state, memory aligned */

#define FIFO_memcpy(dst,src,len)            EZBL_RAMCopy(dst,src,len)    // Remap FIFO's internal memcpy() use to our own EZBL_RAMCopy() function
#include "fifo.c.h"



// Called automatically whenever threshold of 64 bytes is obtained (minimum SHA1 block size)
unsigned int EZBL_SHA1_FIFO_OnData(void *data, unsigned int dataLength)
{
    unsigned int i, back3, back8, back14;
    unsigned long a, b, c, d, e, f, k, temp;
    unsigned long *w = data; // Endian swapped data

    if(EZBL_SHA1_FIFO_vars.opFlags & EZBL_SHA1_HASH)
    {
        // If this is the first time being called, initialize the hash state variables according to the SHA1 spec.
        if(EZBL_SHA1_FIFO_vars.length == 0u)
        {
            EZBL_SHA1_FIFO_vars.hash[0] = 0x67452301;
            EZBL_SHA1_FIFO_vars.hash[1] = 0xEFCDAB89;
            EZBL_SHA1_FIFO_vars.hash[2] = 0x98BADCFE;
            EZBL_SHA1_FIFO_vars.hash[3] = 0x10325476;
            EZBL_SHA1_FIFO_vars.hash[4] = 0xC3D2E1F0;
            if(EZBL_SHA1_FIFO_vars.opFlags & EZBL_SHA1_ZERO_BLOBS_HASH)
            {
                EZBL_RAMSet(data + 8, 0x00, 20);
            }
        }

        // Need to keep track of the length of data we add to the hash since it is included in the hash itself during the final padding step
        EZBL_SHA1_FIFO_vars.length += dataLength;


        // Set up a, b, c, d, e
        a = EZBL_SHA1_FIFO_vars.hash[0];
        b = EZBL_SHA1_FIFO_vars.hash[1];
        c = EZBL_SHA1_FIFO_vars.hash[2];
        d = EZBL_SHA1_FIFO_vars.hash[3];
        e = EZBL_SHA1_FIFO_vars.hash[4];

        // Set up the w[] vector by swaping byte endian-ness for each 32-bit value
        for(i = 0; i < 16u; i++)
        {
            BYTE_ENDIAN_SWAP_32(w);
            w++;
        }
        w -= 16; // Restore pointer

        back3 = 13;
        back8 = 8;
        back14 = 2;

        // Main mixer loop for 80 operations
        for(i = 0; i < 80u; i++)
        {
            if(i <= 19u)
            {
                f = d ^ (b & (c ^ d)); // f = (b & c) | ((~b) & d); Logically equivalent, but one less operation
                k = 0x5A827999;
            }
            else if(i >= 20u && i <= 39u)
            {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if(i >= 40u && i <= 59u)
            {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else // i >= 60u && i <= 79
            {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            // Calculate the w[] value and store it in the array for future use
            if(i >= 16u)
            {
                w[i & 0x0f] = leftRotateDWORD((w[back3] ^ w[back8] ^ w[back14] ^ w[i & 0x0f]), 1);
                back3 += 1;
                back8 += 1;
                back14 += 1;
                back3 &= 0x0f;
                back8 &= 0x0f;
                back14 &= 0x0f;
            }

            // Calculate the new mixers
            temp = leftRotateDWORD(a, 5) + f + e + k + w[i & 0x0f];
            e = d;
            d = c;
            c = leftRotateDWORD(b, 30);
            b = a;
            a = temp;
        }

        // Add the new hash to the sum
        EZBL_SHA1_FIFO_vars.hash[0] += a;
        EZBL_SHA1_FIFO_vars.hash[1] += b;
        EZBL_SHA1_FIFO_vars.hash[2] += c;
        EZBL_SHA1_FIFO_vars.hash[3] += d;
        EZBL_SHA1_FIFO_vars.hash[4] += e;
    }

    return dataLength;
}


// Perform final padding and protocol specific ending steps. For SHA1 hashing,
// completes any residual blocks of data and computes the final hash output.
// Read the Hash from EZBL_SHA1_FIFO_vars.hash[]. Note that .hash[] is
// defined as 5 unsigned longs, so that they are memory aligned and can be
// read/written to efficiently. Cast to a (char*) if you just want a 20 byte
// array of hash data.
void EZBL_SHA1_FIFO_Flush(void)
{
    unsigned int i;
    unsigned long temp;
    unsigned long bytesInCompleteHash;

    // Save the total number of bytes of data in the hash before we add the
    // 1 bit, padding bytes, and 64-bit message length
    bytesInCompleteHash = EZBL_SHA1_FIFO_vars.length + EZBL_SHA1_FIFO_vars.dataCount;

    // Add one more bit and 7 zeros
    EZBL_SHA1_FIFO_Write8(0x80);

    // If there's less than 64-bits (8 bytes) left for (ml message length)
    // before hitting the 512-bit (64 byte) block size, then we need to zero pad
    // and submit this block before adding in the final bit length
    if(EZBL_SHA1_FIFO_vars.dataCount >= 64u - 8u)
    {
        EZBL_SHA1_FIFO_Fill(0x00, 64 - EZBL_SHA1_FIFO_vars.dataCount);
    }

    // Zero fill all the way up to 448 bits (plus 24 more bits since our block
    // length is only 40 bits (35 used bits) instead of 64)
    EZBL_SHA1_FIFO_Fill(0x00, 64 - 8 + 3 - EZBL_SHA1_FIFO_vars.dataCount);

    // Fill in the size, in bits, in big-endian
    EZBL_SHA1_FIFO_Write8(BYTE3(bytesInCompleteHash) >> (29 - 24)); // Because temp below may overflow, we must specifically compute it here at the big end
    temp = bytesInCompleteHash << 3; // Multiply by 8 to get the bit count
    BYTE_ENDIAN_SWAP_32(&temp);
    EZBL_SHA1_FIFO_Write32(temp);

    // Format the result in big-endian format
    for(i = 0; i < 5u; i++)
    {
        BYTE_ENDIAN_SWAP_32(&EZBL_SHA1_FIFO_vars.hash[i]);
    }
}


/**
 * Returns a SHA-1 hash of all regions except those occupied by the bootloader.
 * 
 * This function does not support Auxiliary Flash or "Non-volatile" 
 * Configuration Bytes (addresses >= 0xF80000).
 * 
 * @param *dest Pointer to a block of RAM to write the computed SHA-1 Hash. The 
 *              bytes are stored in Big Endian byte order to match normal usage 
 *              conventions and Internet RFC standards. You need 20-bytes 
 *              (160-bits) to store the complete hash.
 *
 * @param destLength Number of bytes to write at *dest. This parameter can be 
 *                   less than the 20 bytes/160-bit hash size. In this case, all 
 *                   hash bits are computed as normal, but the lower order bytes 
 *                   of the hash are truncated off and not copied to *dest.
 */
void EZBL_HashApp(void *dest, unsigned int destLength)
{
    unsigned char buffer[3*16];
    unsigned long address;
    unsigned int i;
    unsigned long lastSectorAddress;

    EZBL_SHA1_FIFO_Reset();
    EZBL_SHA1_FIFO_vars.length = 0;
    EZBL_SHA1_FIFO_vars.opFlags = EZBL_SHA1_HASH;

    lastSectorAddress = EZBL_WeakSYM32(EZBL_MAIN_FLASH_END_ADDRESS);   
    if(!lastSectorAddress) // When not provided, fall back to compiler provided __CODE_BASE + __CODE_LENGTH linker symbols. These are not always right since they can be for Aux Flash, and are truncated when Flash Config words exist, so are less ideal.
        lastSectorAddress = ((EZBL_SYM32(_CODE_LENGTH) + EZBL_SYM32(_CODE_BASE)) | 0xFF) + 1u;

    for(address = EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR); address <= lastSectorAddress; address += EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR))
    {
        // Skip sectors in the bootloader region
        if(EZBL_ROMAddressInBootloader(address))
        {
            continue;
        }

        // Read all the other Flash data out of the current page and add it to
        // the hash
        for(i = 0; i < EZBL_SYM(EZBL_BYTES_PER_SECTOR); i += sizeof(buffer))
        {
            EZBL_ReadPackedFlash(buffer, address, i, sizeof(buffer));
            EZBL_SHA1_FIFO_Write(buffer, sizeof(buffer));
        }
    }

    // Add padding and compute final hash
    EZBL_SHA1_FIFO_Flush();

    // Return at most 20 bytes (the maximum bytes the hash generates)
    if(destLength > 20u)
    {
        destLength = 20;
    }
    EZBL_RAMCopy(dest, EZBL_SHA1_FIFO_vars.hash, destLength);
}
