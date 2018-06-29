/*******************************************************************************
  Easy Bootloader for PIC24 and dsPIC33 Internal Definitions

  File Name:
    ezbl_core.h

  Summary:
    OBSOLETE; DO NOT INCLUDE IN NEW CODE

  Description:
    OBSOLETE; DO NOT INCLUDE IN NEW CODE

    These are internal functions of the bootloader core. You should not #include
    this file in any source files unless you are writing a EZBL submodule.
    For ordinary application use of the EZBL bootloader, #include "ezbl.h"
    instead.
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

#ifndef EZBL_CORE_H
#define	EZBL_CORE_H




// Macro for ConCatenating two tokens without making them strings first. This
// is useful for prefixing letters or words onto functions, variables, and
// other symbol names. Ex: You can declare a function prefix of
// 'FIFO_' to automatically be added using:
// #define MODULE_PREFIX  FIFO_
//      void CC(MODULE_PREFIX,Reset)(int parameter1, char parameter2)
// The preprocessor will resolve this into:
//      void FIFO_Reset(int parameter1, char parameter2)
#if !defined(CC)
#define CC_INNER(left,right)            left##right
#define CC(left,right)                  CC_INNER(left,right)
#endif

// Macros for concatenating three tokens together without making them strings
// first. This is just like the two-token CC() macro, but for case where you
// want both a prefix and suffix.
// Ex: you can write 1 to the T2CONbits.ON register bit using:
// #define TIMER_SELECT  2
//     CCC(T,TIMER_SELECT,CONbits).ON = 1;
// The preprocessor will resolve this into:
//     T2CONbits.ON = 1;
#if !defined(CCC)
#define CCC_INNER(left, middle, right)  left##middle##right
#define CCC(left, middle, right)        CCC_INNER(left, middle, right)
#endif

// Macros for converting a macro value into a string representation of the
// macro value. This is needed for concatenating macro contents to other strings.
// Using STRINGIFY_INNER() directly does not allow macro expansion.
// Using STRINGIFY() does perform macro expansion of value before passing
// to the inner version.
// As an example, consider this code:
//  #define APP_VERSION_MAJOR    3
//  #define APP_VERSION_MINOR    14
//      UARTPrintROMString("Firmware version: " STRINGIFY(APP_VERSION_MAJOR) "." STRINGIFY(APP_VERSION_MINOR) "\r\n");
// Here, the version macros are integer compile-time constants. With
// stringification, the UARTPrintROMString() function doesn't need any logic to
// do binary to ASCII number conversion like the heavyweight printf() function.
#if !defined(STRINGIFY)
#define STRINGIFY_INNER(x)              #x
#define STRINGIFY(value)                STRINGIFY_INNER(value)
#endif


#define BYTE0(value)                    (((unsigned char*)&value)[0])
#define BYTE1(value)                    (((unsigned char*)&value)[1])
#define BYTE2(value)                    (((unsigned char*)&value)[2])
#define BYTE3(value)                    (((unsigned char*)&value)[3])





#if !defined(NULL)
#define NULL 0u
#endif


// Normal defines for stuff ordinarily found in the device header file. Since we
// want to ensure we aren't linking against anything that might not exist, we
// are purposely not including xc.h or any device header in ezbl*.c files so
// that undefined tokens won't compile.
#define Nop()       __builtin_nop()
#define ClrWdt()    {__asm__ volatile ("clrwdt" : : : "memory");}
#define Sleep()     {__asm__ volatile ("pwrsav #0" : : : "memory");}
#define Idle()      {__asm__ volatile ("pwrsav #1" : : : "memory");}
#define BootSwp()   {__asm__ volatile ("bootswp" : : : "memory");}     // 1111 1110  001u uuuu  uuuu uuuu
#define Break()     {__asm__ volatile ("break" : : : "memory");}       // 1101 1010  01uu uuuu  uuuu uuuu
#define Reset()     {__asm__ volatile ("reset" : : : "memory");}       // 1111 1110 0000 0000 0000 0000





// Declare existence of some useful pseudo-constant values specified in the 
// linker script(s). These symbols are normally defined in the ezbl_xxx.gld 
// file.
//
// Since these are link time symbols, not actual constants stored in
// Flash, use the EZBL_SYM32() macro to retrieve their address/value.
//extern int __attribute__((space(prog))) EZBL_INSTRUCTION_ADDRESS_MASK;      // Ex: 0x07FF, a mask for obtaining the address bits for an instruction on a sector
//extern int __attribute__((space(prog))) EZBL_BYTES_PER_SECTOR;              // Number of bytes in the minimum erase block size of the Flash
//extern int __attribute__((space(prog))) EZBL_BYTES_PER_FLASH_WORD;          // 3 or 6 on 16-bit devices
//extern int __attribute__((space(prog))) EZBL_ADDRESSES_PER_SECTOR;
//extern int __attribute__((space(prog))) EZBL_FLASH_SECTOR_COUNT;            // Deprecated, do not use for new code. Count of how many sectors are in the main flash array
//extern int __attribute__((space(prog))) EZBL_BYTES_PER_SECTOR_MAP;          // Deprecated, do not use for new code. Number of bytes needed to create a bitmap of the entire Flash organization
//extern int __attribute__((space(prog))) EZBL_ADDRESSES_PER_SECTOR_MAP;      // Deprecated, do not use for new code. Approximate number of addresses needed to store a bitmap of the Flash. This is approximate because it is rounded up in the event EZBL_BYTES_PER_SECTOR_MAP isn't integer divisible by the instruction width.
//extern int __attribute__((space(prog))) EZBL_NVMOP_PAGE_ERASE;              // Deprecated, do not use for new code. Actual value to write to NVMCON to erase the minimum amount of data
//extern int __attribute__((space(prog))) EZBL_NVMOP_MIN_PROGRAM;             // Deprecated, do not use for new code. Actual value to write to NVMCON to program the least amount of data
//extern int __attribute__((space(prog))) EZBL_MIN_PROGRAM_BYTES;             // Bytes
//extern int __attribute__((space(prog))) EZBL_MIN_PROGRAM_ADDRESSES;         // Addresses (Bytes/3*2)
//extern int __attribute__((space(prog))) EZBL_MIN_PROGRAM_INSTRUCTIONS;      // Instructions (Bytes/3)
//extern int __attribute__((space(prog))) EZBL_ADDRESS_TO_SECTOR_MAP_SHIFT;
//extern int __attribute__((space(prog))) EZBL_LAST_SAFE_SECTOR_ADDRESS;     // Highest sector address we can erase safely without worrying about clobbering sensitive configuration fuse information (i.e. last sector if fuses not in main Flash, second to last sector if fuses are on the last sector)
//extern int __attribute__((space(prog))) EZBL_SECTOR_ADDRESS_MASK;          // Ex: 0x00FFF800, a mask for obtaining the address bits for the beginning of the current sector.
//extern int __attribute__((space(prog))) EZBL_LAST_SECTOR_ADDRESS;          // Address of the last sector in the main flash array
//extern int __attribute__((space(prog))) EZBL_MAIN_FLASH_END_ADDRESS;       // Address just past the end of the implemented Flash array
extern int __attribute__((space(prog), weak)) EZBL_BYTES_PER_REC_HEADER;       /* See JOURNAL_REC_HEADER structure type */
extern int __attribute__((space(prog), weak)) EZBL_BYTES_PER_REC_DEFINE_STORE; /* See REC_DEFINE_STORE structure type */
extern int __attribute__((space(prog), weak)) EZBL_BYTES_PER_REC_INVALIDATE;   /* See REC_INVALIDATE structure type */
extern int __attribute__((space(prog), weak)) EZBL_ADDRESSES_PER_REC_DEFINE_STORE;
extern int __attribute__((space(prog), weak)) EZBL_ADDRESSES_PER_REC_INVALIDATE;
extern int __attribute__((space(prog), weak)) EZBL_CONFIG_ALLOW_ERASE_FUSES;// Not implemented
extern int __attribute__((space(prog), weak)) EZBL_RESERVED_BIT_ADDRESS;   // Address of a special Flash word that always contains a `0' bit someplace in it (use EZBL_RESERVED_BIT_MASK to find it)
extern int __attribute__((space(prog), weak)) EZBL_RESERVED_BIT_MASK;      // Reserved bit mask if in the EZBL_RESERVED_BIT_ADDRESS config word
extern int __attribute__((space(prog), weak)) EZBL_JOURNAL_BASE;           // Addresses for the journal; defined weak so they still link but return 0 if there is no journal enabled.
extern int __attribute__((space(prog), weak)) EZBL_JOURNAL_SECTOR2;        // Addresses for the journal; defined weak so they still link but return 0 if there is no journal enabled.
extern int __attribute__((space(prog), weak)) EZBL_JOURNAL_END;            // Addresses for the journal; defined weak so they still link but return 0 if there is no journal enabled.
extern int __attribute__((space(prog), weak)) EZBL_Reset;                  // Bootloader entry point function address
extern int __attribute__((space(prog), weak)) EZBL_CONFIG_SECTOR_ADDRESS;  // May not be a sector address if config stored in fuses



// SFRs registers that need to be hooked up to the actual SFR addresses by the
// linker.
extern volatile unsigned int NVMCON;
extern volatile unsigned int TBLPAG;
extern volatile unsigned int INTCON2;
extern volatile unsigned int __attribute__((weak))  NVMADR;	    // Only exists on some parts. Defining as weak will cause &NVMADR to return NULL when not present.
extern volatile unsigned int __attribute__((weak))  NVMADRU;	// Only exists on some parts. Defining as weak will cause &NVMADRU to return NULL when not present.
extern volatile unsigned int __attribute__((weak))  NVMSRCADRL;	// Only exists on some parts. Defining as weak will cause &NVMADR to return NULL when not present.



// State bitmask flags definitions for EZBL_PGM_FIFO_vars.opFlags variable. These are only used for multi-store operation.
#define EZBL_PGM_ERROR_WRITING           0x0001u
#define EZBL_PGM_NO_FREE_SPACE           0x0002u
#define EZBL_PGM_MAP_SECTORS             0x0004u     // Keep track of destination address seen, mapping to EZBL_sectorBitmapRAM2[].
#define EZBL_PGM_TARGET_RESET_VECTOR     0x0008u     // Address must be 0x000000
#define EZBL_PGM_TARGET_FUSES            0x0010u     // Do not actually write anything; Fuse writing not implemented
#define EZBL_PGM_TARGET_STORE0           0x0020u     // Only allow writing to store0 (no bootloader, journal, or occupied inactive store addresses)
#define EZBL_PGM_TARGET_INACTIVE_STORE   0x0040u     // Only allow writing to an inactive store (no bootloader, journal, or occupied store0 addresses)
#define EZBL_PGM_TARGET_JOURNAL          0x0080u     // Only allow writing to the journal (no bootloader, or occupied inactive or active store0 addresses)
#define EZBL_PGM_TARGET_ANY              (EZBL_PGM_TARGET_RESET_VECTOR | EZBL_PGM_TARGET_FUSES | EZBL_PGM_TARGET_STORE0 | EZBL_PGM_TARGET_INACTIVE_STORE | EZBL_PGM_TARGET_JOURNAL)     // Any address besides the bootloader
// If no target specified, the all writes will be ignored (although address incrementing will still occur for address mapping purposes)


// State bitmask flags definitions for EZBL_BLOB_FIFO_vars.opFlags variable. These are only used for multi-store operation.
#define EZBL_BLOB_DONE               0x0001u // Don't need more data anymore
#define EZBL_BLOB_INCOMING           0x0002u // Blob entering from communications medium or inactive store; output will be a store0 (if BLOB_WRITE turned on). Decryption, decompression, decoding will be applied, if turned on.  If this flag is clear, then it is assumed store0 type data is entering and encode/compress/encrypt operations will be applied as applicable; output would be an inactive store1+ (if BLOB_WRITE is on)
#define EZBL_BLOB_HASH               0x0004u // Compute SHA1 hash over the data, blob data, or processed blob data
#define EZBL_BLOB_MAP_SECTORS        0x0008u // Fill EZBL_sectorBitmap[] with the encoded sectors used or EZBL_sectorBitmap2[] with the decoded sectors needed/used
//#define EZBL_BLOB_CRYPT              0x0010u // Decrypt the incoming data (occurs first, if applicable); or Encrypt the outgoing data (occurs after compression, if applicable)
//#define EZBL_BLOB_PRESS              0x0020u // Decompress the incoming data (occurs after decryption, if applicable); or Compress the outgoing data (occurs after encoding, if applicable)
//#define EZBL_BLOB_CODE               0x0040u // Decode the incoming data from blob format to raw store0 format (occurs after decryption and decompression, if applicable); or Encode the output data (occurs first, if applicable)
#define EZBL_BLOB_WRITE              0x0080u // Write the outgoing data to Flash (data is thrown away otherwise - needed for computing hashes and bitmaps before committing anything)
#define EZBL_BLOB_FAIL               0x0100u // An error occurred in the data stream or during write of the output. This blob or data is unusable and subsequent data will be ignored.


// Enum for FIFO selection with PumpStoreIntoFIFO() function. These are only used for multi-store operation.
typedef enum
{
    COM = 0,
    PGM,
    BLOB,
    SHA1,
    ERASE // Erase the store (not a real FIFO)
} EZBL_FIFO_TYPES;


#if !defined(EZBL_MULTISTORE_C)   // Access to certain struct members is needed in alternate source files, but declaring a struct extern first and then locally in the instancing .c file gives a redefinition compiler error. Therefore, we must not let the instancing .c file see this struct definition.
extern struct
{
    volatile unsigned int dataCount;    // Use EZBL_ATOMIC_ADD() and EZBL_ATOMIC_SUB() to change this value
    unsigned char *headPtr;             // Head pointer for writing data into the fifoRAM (via the EZBL_PGM_FIFO_Write*() functions)
    unsigned char *tailPtr;             // Tail pointer for reading data from the fifoRAM (via the EZBL_PGM_FIFO_Read*() functions; also EZBL_PGM_FIFO_Peek*() functions if peeking is enabled)
    unsigned int dataThreshold;         // Value to compare against dataCount for issuing OnData() callback
    unsigned int opFlags;               // Bitfield of PGM_* flags
    unsigned long destAddress;          // Destination Flash program address to write the data in the FIFO to. This field is automatically updated in the EZBL_PGM_FIFO_OnData() function as data is successfully written.
    unsigned char fifoRAM[];            // This is a variable length field. Check FIFO_SIZE definition for this FIFO in ezbl_core.c to find it's actual value.
} EZBL_PGM_FIFO_vars;
#endif


#if !defined(EZBL_MULTISTORE_C)   // Access to certain struct members is needed in alternate source files, but declaring a struct extern first and then locally in the instancing .c file gives a redefinition compiler error. Therefore, we must not let the instancing .c file see this struct definition.
extern struct
{
    volatile unsigned int dataCount;    // Use EZBL_ATOMIC_ADD() and EZBL_ATOMIC_SUB() to change this value
    unsigned char   *headPtr;           // Head pointer for writing data into the fifoRAM (via the EZBL_BLOB_FIFO_Write*() functions)
    unsigned char   *tailPtr;           // Tail pointer for reading data from the fifoRAM (via the EZBL_BLOB_FIFO_Read*() functions; also EZBL_BLOB_FIFO_Peek*() functions if peeking is enabled)
    unsigned int    dataThreshold;      // Value to compare against dataCount for issuing OnData() callback
    unsigned int    opFlags;            // Bitfield of BLOB_* flags
    unsigned int    state;              // State machine for encoding/compressing/encrypting or decoding/decompressing/decrypting
    unsigned long   length;             // Total length of this full stream (self determined from blob contents or after finished creating the blob)
    unsigned long   lengthRemaining;    // Number of bytes still expected to be input
    unsigned long   subCounter;         // Subordinate counter for blob record decoding
    unsigned char   fifoRAM[];          // This is a variable length field. Check FIFO_SIZE definition for this FIFO in ezbl_core.c to find it's actual value.
} EZBL_BLOB_FIFO_vars;
#endif


void EZBL_HashApp(void *dest, unsigned int destLength);   // Computes a SHA-1 Hash. Implemented in ezbl_core_sym_dependent.c, depends on ezbl_sha1.c


#endif	/* EZBL_CORE_H */

