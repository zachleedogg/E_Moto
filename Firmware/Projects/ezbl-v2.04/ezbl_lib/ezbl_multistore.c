/*******************************************************************************
  Easy Bootloader for PIC24 and dsPIC33 Source File For Handling Multiple Image Stores

  File Name:
    ezbl_multistore.c

  Summary:
    Code for the optional multi-image store component of the EZBL library.

  Description:
    Implements code for the optional multi-image store component of the EZBL
    library. This includes journaling and other tasks that are only needed when
    full application self-recovery is needed. Basic "classic" bootloaders that
    contain their own communications routines and execute on power up without
    simultaneously running a full application generally shouldn't need these
    functions.

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
#define EZBL_MULTISTORE_C

#include "ezbl_core.h"
#include "ezbl_sha1.h"
#include "ezbl_multistore.h"
#include "fifo.h"           // For FIFO_FEATURES flag definitions

// Header for public API definitions that we are implementing in this file.
#include "ezbl.h"


// Define a few bit masks for the NVMCON register. All devices should
// (hopefully?) have these minimum set of bits and use the same positions.
#define NVMCON_WR           0x8000
#define NVMCON_WREN         0x4000
#define NVMCON_WRERR        0x2000

#define ADDR_PER_INSTR      0x2u
#define BYTES_PER_INSTR     3u


// Main functions intended to be called from outside the bootloader (device vector or application call)
void __attribute__((noreturn, naked)) EZBL_OnReset(void);

// Internal functions specific to this multistore method bootloader core. They 
// normally shouldn't be called from anywhere outside the bootloader and should 
// not be called at all in new bootloader code.
void __attribute__((noreturn)) EZBL_OnResetContinued(void);
signed short EZBL_OnCommand(unsigned short operation, unsigned short storeID, void *inOutData, unsigned long inOutDataLen);
//signed int EraseProgDone(void);
signed int EZBL_ProgMin(unsigned long address, const void *data);
//signed short EZBL_MoveSector(unsigned long destAddress, unsigned long sourceAddress);
unsigned int EZBL_COM_FIFO_OnData(void *data, unsigned int dataLength);
unsigned int EZBL_PGM_FIFO_OnData(void *data, unsigned int dataLength);
unsigned int EZBL_BLOB_FIFO_OnData(void *data, unsigned int dataLength);

// Internal functions specific to this bootloader core. They could be unsafe to
// expose publically. Most of these are for Fash read/erase/write functionality.
void EZBL_JournalConfigureAddress(void);
signed int EZBL_JournalValidate(unsigned long journalBaseAddress);
unsigned int EZBL_JournalCompactable(unsigned long journalBaseAddress);
void EZBL_JournalCompact(void);
signed long EZBL_JournalRead(unsigned int journalTopic, unsigned int storeID, void *extendedData);
signed int EZBL_JournalWrite(unsigned char recordType, unsigned int storeID, void *recordData);
signed long EZBL_JournalSearch(unsigned char recordType, unsigned int storeID);
signed int EZBL_JournalEnumStores(unsigned int lastStoreID, unsigned long attributeMask, unsigned long attributes);
unsigned int EZBL_ValidateStore(unsigned int storeID);
signed int EZBL_EraseStore(unsigned int storeID);
signed int EZBL_PumpStoreIntoFIFO(unsigned int storeID, EZBL_FIFO_TYPES dest, unsigned int usePreexistingBitmap);
signed int EZBL_InstallStoreToStore0(unsigned int storeID, unsigned int ignoreStore0Attributes);
signed long EZBL_EnumStoreAddresses(unsigned int storeID, signed long lastAddress, unsigned int usePreexistingBitmap);


// Some quasi-variable length RAM arrays allocated in the linker script.
extern unsigned char EZBL_sectorBitmapRAM[]; // Size, in bytes specified by EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP);
extern unsigned char EZBL_sectorBitmapRAM2[]; // Size, in bytes specified by EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP);

// The current journal base address
static unsigned long currentJournalAddress;
static unsigned long currentJournalAddressComplement; // A complemented copy of currentJournalAddress for integrity checking and knowing when JournalConfigureAddress() needs to be called to set currentJournalAddress.

// These two items actually belong to the application you are linking the
// bootloader to. They also will change as you link in different applications
// since the EZBL_nextLayerSPInit section is mapped to a fixed application
// address in the ezbl_*.gld linker script.
extern int _SP_init;    // Not a real variable. This is a symbol generated by the linker.
//extern int _SPLIM_init; // Not a real variable. This is a symbol generated by the linker.
unsigned int __attribute__((space(prog), section("EZBL_nextLayerSPInit"), unused, weak)) EZBL_appSPInit = (unsigned int)&_SP_init;
//unsigned int __attribute__((space(prog), section(".ezbl_app_splim_init"), keep, unused)) EZBL_appSPLIMInit = (unsigned int)&_SPLIM_init;
//
//// Define another couple variables, this time residing in our read-only
//// bootloader Flash area that we should use if the application stack pointer
//// doesn't exist or is too risky to use.
//unsigned int __attribute__((space(prog), keep, unused)) EZBL_failSafeSPInit = (unsigned int)&_SP_init;
//unsigned int __attribute__((space(prog), keep, unused)) EZBL_failSafeSPLIMInit = (unsigned int)&_SPLIM_init;


// State bitmask flags definitions for EZBL_COM_FIFO_vars.opFlags variable
#define EZBL_COM_WRITE_START 0x0001
#define EZBL_COM_WRITING     0x0002
#define EZBL_COM_FAIL        0x0004

// Instantiate FIFO for buffering bootloader journal and specialty write data to Flash
#define FIFO_NAME                           EZBL_COM_FIFO_
#define FIFO_SIZE                           6         // Must be at least 4 or EZBL_SYM(EZBL_MIN_PROGRAM_BYTES), whichever is larger; an integer multiple of EZBL_SYM(EZBL_MIN_PROGRAM_BYTES) is best for performance due to better alignment
#define FIFO_ON_DATA_THRESHOLD_RESET_VALUE  4
#define FIFO_FEATURES                       FIFO_FEATURE_WRITABLE | FIFO_FEATURE_ON_DATA_LINEAR
#define FIFO_EXTRA_VARS                     unsigned int opFlags;       \
                                            unsigned long length;       \
                                            unsigned long destAddress;

#define FIFO_memcpy(dst,src,len)            EZBL_RAMCopy(dst,src,len)    // Remap FIFO's internal memcpy() use to our own EZBL_RAMCopy() function
#include "fifo.c.h"



// Instantiate FIFO for buffering all Program operations to Flash up to the minimum program size of the device
#define FIFO_NAME                           EZBL_PGM_FIFO_
#define FIFO_SIZE                           6         // Must be at least EZBL_SYM(EZBL_MIN_PROGRAM_BYTES); an integer multiple of EZBL_SYM(EZBL_MIN_PROGRAM_BYTES) is best for performance due to better alignment
#define FIFO_ON_DATA_THRESHOLD_RESET_VALUE  EZBL_SYM(EZBL_MIN_PROGRAM_BYTES)
#define FIFO_FEATURES                       FIFO_FEATURE_WRITABLE | FIFO_FEATURE_ON_DATA_LINEAR
#define FIFO_EXTRA_VARS                     unsigned int opFlags;       \
                                            unsigned long destAddress;
#define FIFO_memcpy(dst,src,len)            EZBL_RAMCopy(dst,src,len)    // Remap FIFO's internal memcpy() use to our own EZBL_RAMCopy() function
#include "fifo.c.h"



// Instantiate blob encoding/decoding FIFO
#define FIFO_NAME                           EZBL_BLOB_FIFO_
#define FIFO_SIZE                           20
#define FIFO_ON_DATA_THRESHOLD_RESET_VALUE  20
#define FIFO_FEATURES                       FIFO_FEATURE_WRITABLE | FIFO_FEATURE_ON_DATA_LINEAR
#define FIFO_EXTRA_VARS                     unsigned int  opFlags;          /* Bitfield of BLOB_* flags */                                                                  \
                                            unsigned int  state;            /* State machine for encoding/compressing/encrypting or decoding/decompressing/decrypting */    \
                                            unsigned long length;           /* Total length of this full stream (self determined from blob contents or after finished creating the blob) */ \
                                            unsigned long lengthRemaining;  /* Number of bytes still expected to be input */                                                \
                                            unsigned long subCounter;       /* Subordinate counter for blob record decoding */                                              

#define FIFO_memcpy(dst,src,len)            EZBL_RAMCopy(dst,src,len)       // Remap FIFO's internal memcpy() use to our own EZBL_RAMCopy() function
#include "fifo.c.h"


// Main startup code. Note that we aren't linking against any C start up code,
// so no variable initalization is done. The stack pointer (WREG15), SPLIM, and
// PSVPAG/EDS registers are not initialized. No RAM locations are zeroed. This
// code executes immediately after reset, directly from the GOTO instruction at
// program memory address 0x000000.
void __attribute__((noreturn, naked)) EZBL_OnReset(void)
{
    // Initialize WREG15 stack pointer to someplace legal and safe. We
    // aren't using __SP_init and __SPLIM_init provided by the linker because
    // their values are encoded into mov instructions (in the bootloader's
    // non-updatable Flash region), but they will need to move around as
    // applications get loaded by the bootloader. Using one-time values provided
    // when the bootloader was first linked would work, but is undesirable since
    // we would then be subject to reset clobbering any application variables
    // attributed __persistent__.

    // Read the EZBL_appSPInit value that *should* be defined (due to this
    // file creating it) and contain the application's __SP_init stack pointer
    // initialization value. It is possible that it doesn't exist or is invalid,
    // however, due to it existing in erasable application Flash so we first
    // should validate what we read at it's address.
    __asm__ volatile(
            "   .extern __DATA_BASE                                             \n"
            "   clr    _EZBL_NVMKey                                             \n"
            "   mov    #tblpage(_EZBL_appSPInit), w0                            \n"
            "   mov    w0, _TBLPAG                                              \n"
            "   mov    #tbloffset(_EZBL_appSPInit), w0                          \n"
            "   tblrdl  [w0], w0                                                \n" // w0 = app's __SP_init value
            "   mov    #__DATA_BASE, w1                                         \n" // w1 = lowest legal RAM value
            "   mov    #_EZBL_DEFAULT_SPLIM_INIT-0x200, w2                      \n" // w2 = 512 (0x200) bytes less than the stack limit
            "   cp     w2, w0                                                   \n" // if(w2 < w0) { use failsafe SP value }
            "   bra    LTU, 1f                                                  \n"
            "   cp     w1, w0                                                   \n" // if(w1 <= w0) { go ahead an use app's __SP_init }
            "   bra    LEU, 2f                                                  \n"
            "1: mov    #_EZBL_DEFAULT_STACK_INIT, w0                            \n" // w0 - safe stack init value if _EZBL_appSPInit is unprogrammed/illegal
            "2: mov    #_EZBL_DEFAULT_SPLIM_INIT, w1                            \n" // Set SPLIM near the max so we can't accidentally TRAP ourself to death
            "   mov    w1, SPLIM                                                \n" // Don't do an indirect read operation from w15 for at least one cycle (therefore this is out of natural order)
            "   mov    w0, w15                                                  \n" // Set stack pointer
            "   mov    w0, w14                                                  \n" // Set frame pointer
            );

    // Initialize flags to trap all interrupts implemented by the bootloader
    EZBL_ForwardBootloaderISR = 0;
    
    // Call the EZBL_OnResetContinued() function to continue reset
    // initialization. This is purposely done as a function call so that the
    // compiler will create a LNK instruction and let us use variables on the
    // stack, call functions, etc. as normal. By calling instead of branching,
    // we do waste 4 bytes of stack RAM for a return address, but this is
    // irrelevant since the stack gets reiniialized when the bootloader is done
    // and the Application code is executed. Switching this to a branch may
    // break debuggability as there could then be two LNK instructions executed
    // without any intervening call/rcall instructions. The mismatch would
    // trigger a StackError hardware trap when the debug executive does stack
    // frame analysis.
    EZBL_OnResetContinued();

    // Dummy compiler built in call to suppress warning that this noreturn
    // attributed function does return. In fact, it never returns.
    __builtin_unreachable();
}

void __attribute__((weak)) EZBL_MultistoreResetWrapper(void);

void __attribute__((noreturn)) EZBL_OnResetContinued(void)
{
    EZBL_CallFunctionIfPresent(EZBL_MultistoreResetWrapper);

    // Bootloader core done looking for things to do -> call the next layer
    // up's reset function.
    __asm__ volatile ("call __reset");

    
    // Dummy compiler built in call to suppress warning that this noreturn
    // attributed function does return. In fact, it never returns due to the
    // above asm GOTO.
    __builtin_unreachable();
}


void EZBL_MultistoreReset(void)
{
    unsigned long sectorAddress;
    unsigned int bitMaskOffset, bitMaskIndex;
    unsigned int storeID;
    unsigned int storeValid;
    unsigned int goldenStoreID;
    unsigned int archiveStoreID;
    unsigned int otherStoreID;
    unsigned int selectedStoreID;

    EZBL_JournalConfigureAddress();

    goldenStoreID = 0xFFFF;
    archiveStoreID = 0xFFFF;
    otherStoreID = 0xFFFF;
    selectedStoreID = 0xFFFF;
    storeID = 0;


    // Check store0 presence and validity
    storeValid = EZBL_ValidateStore(storeID);

    // store0 invalid
    while(!storeValid)
    {
        // Enumerate stores looking for ones with the archive flag set and are not erasable (a "golden store")
        storeID = (unsigned int)EZBL_JournalEnumStores(storeID, EZBL_ATTRIBUTE_ARCHIVE | EZBL_ATTRIBUTE_ERASABLE, EZBL_ATTRIBUTE_ARCHIVE);
        if(storeID & 0x8000)
        {
            break;
        }

        storeValid = EZBL_ValidateStore(storeID);

        // If we found a valid store with the archive flag set, but store0 isn't valid, try to install this archived store into store0
        if(storeValid)
        {
            goldenStoreID = storeID;
            EZBL_InstallStoreToStore0(storeID, 1); // Resets PIC on success, so we would automatically start using store0 then. Otherwise returns.
        }
        storeValid = 0;
    }

    // store0 invalid, no installable golden archive, non-eraseable
    while(!storeValid)
    {
        // Enumerate all archive flagged stores
        storeID = (unsigned int)EZBL_JournalEnumStores(storeID, EZBL_ATTRIBUTE_ARCHIVE, EZBL_ATTRIBUTE_ARCHIVE);
        if(storeID & 0x8000)
        {
            break;
        }

        storeValid = EZBL_ValidateStore(storeID);

        // If we found a valid store without the archive flag set, and store0 isn't valid, try to install this store into store0
        if(storeValid)
        {
            archiveStoreID = storeID;
            EZBL_InstallStoreToStore0(storeID, 1); // Resets PIC on success, so we would automatically start using store0 then. Otherwise returns.
        }

        storeValid = 0;
    }

    // store0 invalid, no installable archive at all
    while(!storeValid)
    {
        // Enumerate all non-archive flaged stores
        storeID = (unsigned int)EZBL_JournalEnumStores(storeID, EZBL_ATTRIBUTE_ARCHIVE, 0);
        if(storeID & 0x8000)
        {
            break;
        }

        storeValid = EZBL_ValidateStore(storeID);

        // If we found a valid store without the archive flag set, and store0 isn't valid, try to install this store into store0
        if(storeValid)
        {
            otherStoreID = storeID;
            EZBL_InstallStoreToStore0(storeID, 1); // Resets PIC on success, so we would automatically start using store0 then. Otherwise returns.
        }

        storeValid = 0;
    }

    // store0 invalid, no installable stores at all
    if(!storeValid)
    {
        // Start enumerating from store 0x0001 on by initializing to zero. This is done so as a last resort, we can try store0 as is, even though it isn't fully valid.
        storeID = 0x0000;

        // Since there weren't any installable stores, we shall now try to erase
        // everything but a single store and then try to install that store
        // again.
        if((goldenStoreID & 0x8000) == 0x0000u)
        {
            selectedStoreID = goldenStoreID;
        }
        else if((archiveStoreID & 0x8000) == 0x0000u)
        {
            selectedStoreID = archiveStoreID;
        }
        else if((otherStoreID & 0x8000) == 0x0000u)
        {
            selectedStoreID = otherStoreID;
        }

        if((selectedStoreID & 0x8000) == 0x0000u)
        {
            while(1)
            {
                // Enumerate all stores and erase them
                storeID = (unsigned int)EZBL_JournalEnumStores(storeID, 0, 0);
                if(storeID & 0x8000)
                {
                    break;
                }

                // Don't erase the one we are trying to deperately install
                if(storeID == selectedStoreID)
                {
                    continue;
                }

                EZBL_EraseStore(storeID);
            }
            EZBL_InstallStoreToStore0(selectedStoreID, 1);
            // We're having a bad day if we get here, above line is supposed to succeed and reboot the device.
        }
        else
        {
            // Absolutely no valid stores exist. This happens when we are
            // starting for the first time booting up after being programmed via
            // ICSP since the journal starts out as blank. To fix this, we shall
            // write a store0 journal entry to correspond to all the sectors
            // that have data currently on them (besides the bootloader's
            // sectors).
            // Compute a sector bitmap of which ones are in use (besides the bootloader). This will be recorded as store0.
            EZBL_RAMSet(EZBL_sectorBitmapRAM, 0x00, EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP));
            for(sectorAddress = 0; sectorAddress < EZBL_SYM32(EZBL_MAIN_FLASH_END_ADDRESS); sectorAddress += EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR))
            {
                // Skip over bootloader and journal sectors
                if(EZBL_ROMAddressInBootloader(sectorAddress))
                {
                    continue;
                }

                if(!EZBL_SectorIsClear(sectorAddress))
                {
                    // Mark this sector as used in the sectorBitmap we are building
                    bitMaskOffset = sectorAddress >> EZBL_SYM(EZBL_ADDRESS_TO_SECTOR_MAP_SHIFT);
                    bitMaskIndex = bitMaskOffset >> 3; // 8 bits-per byte
                    bitMaskOffset = bitMaskOffset & 0x7;
                    EZBL_sectorBitmapRAM[bitMaskIndex] |= 1 << bitMaskOffset;
                    storeValid = 1;
                }
            }

            // Commit the store0 definition to the journal (store0 hash computed inside EZBL_JournalWrite() function)
            EZBL_JournalWrite(REC_TYPE_DEFINE_STORE, 0x0000, NULL);
        }
    }
}


// Determines if a given program memory ("ROM") address is within a bootloader 
// journal region. The journal region(s) are determined at link time and stored 
// in Flash via the linker script.
signed int EZBL_ROMAddressInJournal(unsigned long address)
{
    return (address >= EZBL_SYM32(EZBL_JOURNAL_BASE)) && (address < EZBL_SYM32(EZBL_JOURNAL_END));
}


// Checks a given storeID for validity, including presence in Journal and
// correct hash of all data. Returns 1 on success (specified storeID is valid
// and has correct hash). Returns 0 on if the given store is absent or hash an
// incorrect hash.
// Requires the SHA1 FIFO (free on return); does not preserve the FIFO's state.
unsigned int EZBL_ValidateStore(unsigned int storeID)
{
    // Feed all the store data into the Hash engine
    EZBL_SHA1_FIFO_Reset();
    EZBL_SHA1_FIFO_vars.length = 0;
    EZBL_SHA1_FIFO_vars.opFlags = EZBL_SHA1_HASH;
    if(storeID > 0)
    {
        EZBL_SHA1_FIFO_vars.opFlags |= EZBL_SHA1_ZERO_BLOBS_HASH;
    }
    EZBL_PumpStoreIntoFIFO(storeID, SHA1, 0);
    EZBL_SHA1_FIFO_Flush();

    // Get the stored hash out of the journal
    if(EZBL_JournalRead(JOURNAL_STORE_HASH, storeID, EZBL_SHA1_FIFO_vars.fifoRAM) <= 0)
        return 0; // Invalid storeID

    // Check journal's hash against actual computed store's hash
    if(EZBL_RAMCompare(EZBL_SHA1_FIFO_vars.fifoRAM, EZBL_SHA1_FIFO_vars.hash, sizeof(EZBL_SHA1_FIFO_vars.hash)) == 0u)
        return 1; // Store present and verified with correct hash

    return 0; // Store present, but invalid hash
}

// Erases the given store, regardless of attributes
signed int EZBL_EraseStore(unsigned int storeID)
{
    unsigned long address;
    unsigned int i;

    // Erase all the destination sectors
    address = 0xFFFFFFFF;
    while(1)
    {
        // Get the first (or subsequent sector address for the given storeID)
        address = EZBL_EnumStoreAddresses(storeID, address, 0);
        if(address & 0x80000000)
        {
            break;
        }

        // Disable and do not reenable interrupts if store0 is being erased
        if(storeID == 0u)
        {
            EZBL_DisableInterrupts();
        }

        // Erase the sector, if it isn't already. Error out if we can't erase it.
        EZBL_Erase(address, address);
    }

    // Update the Journal to reflect the store being gone
    i = REC_TYPE_DEFINE_STORE;
    if(EZBL_JournalWrite(REC_TYPE_INVALIDATE, storeID, &i) < 0)
        return -2; // Failed to write to journal

    return 1;
}

// Reads the selected bytes from a given store (except store0) and writes them
// to the given RAM pointer. Returns actual number of bytes read. Bytes read can
// be less than bytes requested due to EOF being reached or an invalid storeID
// being specified (always returns 0).
unsigned int EZBL_ReadStoreBytes(void *dest, unsigned int storeID, unsigned long readByteOffset, unsigned int readLength)
{
    unsigned int chunkSize;
    unsigned long flashAddress;
    unsigned long blobLengthRemaining;
    unsigned int bytesActuallyRead = 0;

    // Do nothing if nothing requested
    if(readLength == 0u)
        return 0;

    // Get the blob Length, starting Flash address, and cache the sector use bitmap
    flashAddress = EZBL_EnumStoreAddresses(storeID, (unsigned long)(-1L), 0);
    if(flashAddress & 0x80000000)   // EOF already or storeID invalid
        return 0;
    EZBL_ReadPackedFlash(&blobLengthRemaining, flashAddress, 0, sizeof(blobLengthRemaining));

    // Seek to the readByteOffset Flash sector
    while(readByteOffset >= EZBL_SYM(EZBL_BYTES_PER_SECTOR))
    {
        readByteOffset -= EZBL_SYM(EZBL_BYTES_PER_SECTOR);
        blobLengthRemaining -= EZBL_SYM(EZBL_BYTES_PER_SECTOR);
        if(blobLengthRemaining & 0x80000000)    // If length underflows, offset is illegally high
            return 0;
        flashAddress = EZBL_EnumStoreAddresses(storeID, flashAddress, 1);
        if((signed long)flashAddress < 0)
            return 0;
    }

    // Loop through each Flash sector containing .blob data
    while(blobLengthRemaining)
    {
        // Start out assuming we are going to transfer everything to the end of the sector at a time
        chunkSize = EZBL_SYM(EZBL_BYTES_PER_SECTOR) - (unsigned int)readByteOffset;
        if(chunkSize > readLength)
            chunkSize = readLength;
        if(chunkSize > blobLengthRemaining)
            chunkSize = blobLengthRemaining;

        // Read a chunk
        EZBL_ReadPackedFlash(dest, flashAddress, readByteOffset, chunkSize);

        // Keep track of the running length required so we know when to truncate a chunk
        blobLengthRemaining -= (unsigned int)readByteOffset + chunkSize;
        dest += chunkSize;
        readByteOffset = 0;
        readLength -= chunkSize;
        bytesActuallyRead += chunkSize;

        if(readLength == 0u)
            break;

        // Find the next Flash sector address
        flashAddress = EZBL_EnumStoreAddresses(storeID, flashAddress, 1);
        if(flashAddress & 0x8000000)    
        { // Reached EOF, or storeID not in journal
            break;
        }
    }

    return bytesActuallyRead;
}

// Copies a store (both store0 and inactive stores) into a FIFO
signed int EZBL_PumpStoreIntoFIFO(unsigned int storeID, EZBL_FIFO_TYPES dest, unsigned int usePreexistingBitmap)
{
    unsigned int chunkSize;
    unsigned long address;
    unsigned long length;

    length = 0;
    address = (unsigned long)(-1L);
    while(1)
    {
        address = EZBL_EnumStoreAddresses(storeID, address, usePreexistingBitmap);
        usePreexistingBitmap = 1;
        if((signed long)address == -1L)
        { // Reached EOF
            break;
        }
        else if((signed long)address <= -2L)
        { // Error finding the store in the Journal, return error code from EZBL_EnumStoreAddresses unchanged
            return(signed int)(address >> 16);
        }

        // The special "ERASE" FIFO doesn't actually exist. It is just for
        // erasing a whole Store with a single function call that enumerates
        // the sector bitmap and erases the corresponding sectors.
        if(dest == ERASE)
        {
            EZBL_Erase(address, address);
            continue;
        }

        // Start out assuming we are going to transfer a whole sector at a time
        chunkSize = EZBL_SYM(EZBL_BYTES_PER_SECTOR);

        // Inactive .blob stores have a length we can use to more precisely know
        // when to terminate pumping data into the FIFO (versus sending a bunch
        // of 0xFFFFFF trailer bytes into the FIFO).
        if(storeID != 0u)
        {
            // Push all sectors containing blob data into the specified FIFO
            // Because this is coming from a .blob stored in an inactive store, we
            // can obtain the total length as the first 4 bytes of the blob and
            // not push extra data into the FIFO when done but still not at the end
            // of the sector.
            // If this is the first time going through this loop, read the .blob length so we know when to stop
            if(length == 0u)
            {
                EZBL_ReadPackedFlash(&length, address, 0, sizeof(length));
            }

            // If the last sector of a blob is being read, only pump the actual blob data into the FIFO, not the whole sector
            if(length < chunkSize)
            {
                chunkSize = (unsigned int)length;
            }

            // Keep track of the running length required so we know when to truncate a chunk above
            length -= chunkSize;
        }

        // Copy a chunk into the FIFO (generally a full sector, but could be
        // less if comming from a .blob and this is the last chunk
        EZBL_PumpFlashIntoFIFO(address, 0, chunkSize, dest);
    }

    return 1;
}


// Writes a record of specified recordType and storeID into the journal
// For recordType == REC_TYPE_DEFINE_STORE:
//      recordData: pointer to an optional EZBL_ATTRIBUTES structure to be
//                  written into the journal record. If NULL, default attributes
//                  will be set for the given store location.
//                  EZBL_storeBitmap[] must bet set to match the given store
//                  layout. EZBL_SHA1_FIFO_vars.hash must contain a
//                  pre-computed hash of the given store contents.
// For recordType == REC_TYPE_INVALIDATE:
//      recordData: pointer to a REC_INVALIDATE structure to define the record to invalidate
// Requires exclusive use of EZBL_PGM_FIFO and EZBL_SHA1_FIFO, but both are free upon return.
signed int EZBL_JournalWrite(unsigned char recordType, unsigned int storeID, void *recordData)
{
    unsigned long address;
    unsigned long data;
    unsigned long storeHash[5];
    unsigned int i;
    unsigned int addressesNeeded;
    unsigned long attributes;


    // Recompute the journal base address if our RAM has been erased or
    // corrupted by the application (or more likely, the application's CRT
    // zeroing all RAM)
    if(currentJournalAddress != ~currentJournalAddressComplement)
    {
        EZBL_JournalConfigureAddress();
    }

    // Compute a (pessimistic) estimate of the worst case record length we may
    // write so we can try compacting the journal if needed, or error out
    // beforehand, so we don't risk overflow.
    addressesNeeded = (sizeof(EZBL_REC_DEFINE_STORE) + 1u)*2u / 3u + EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR_MAP) + EZBL_SYM(EZBL_MIN_PROGRAM_ADDRESSES) - 0x2u;

    // Find the address of the end of the journal where we can write this record
    for(i = 0; i < 2u; i++)
    {
        EZBL_PGM_FIFO_vars.destAddress = (unsigned long)EZBL_JournalSearch(REC_TYPE_END_OF_JOURNAL, 0xFFFF);

        // Check if the return address is negative, meaning the journal is corrupt/full/uninitialized/etc.
        if(BYTE3(EZBL_PGM_FIFO_vars.destAddress) & 0x80)
        {
            // Try compacting the journal and trying again.
            EZBL_JournalCompact();
            continue;
        }

        // Check if there is enough space to write a worst case record
        if((unsigned int)(EZBL_PGM_FIFO_vars.destAddress - currentJournalAddress + addressesNeeded) > (EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR)))
        {
            EZBL_JournalCompact();
            continue;
        }

        // Break out of loop before executing i++ step so we can tell if the
        // journal address passed both checks
        break;
    }

    // Error out if we've done two passes and still haven't found/made needed
    // journal free space
    if(i >= 2u)
    {
        return -1;
    }

    // Compute store0's current hash if we are defining store0 with this journal
    // entry
    if((recordType == REC_TYPE_DEFINE_STORE) && (storeID == 0x0000u))
    {
        // Feed all the store0 data into the Hash engine
        EZBL_SHA1_FIFO_Reset();
        EZBL_SHA1_FIFO_vars.length = 0;
        EZBL_SHA1_FIFO_vars.opFlags = EZBL_SHA1_HASH;
        EZBL_PumpStoreIntoFIFO(0x0000, SHA1, 1);
        EZBL_SHA1_FIFO_Flush();
    }

    // Save last hash (needed for REC_TYPE_DEFINE_STORE types which comes with the hash still in the SHA1 FIFO.)
    EZBL_RAMCopy(storeHash, EZBL_SHA1_FIFO_vars.hash, sizeof(EZBL_SHA1_FIFO_vars.hash));

    // Initialize programming FIFO
    EZBL_PGM_FIFO_Reset();

    // Hash all data up to the end of the journal (before this record we are about to write)
    EZBL_SHA1_FIFO_Reset();
    EZBL_SHA1_FIFO_vars.opFlags = EZBL_SHA1_HASH;
    EZBL_SHA1_FIFO_vars.length = 0;
    for(address = currentJournalAddress; address < EZBL_PGM_FIFO_vars.destAddress; address += ADDR_PER_INSTR)
    {
        EZBL_SHA1_FIFO_Write24(EZBL_ReadFlash(address));
    }


    EZBL_PGM_FIFO_vars.opFlags = EZBL_PGM_TARGET_JOURNAL;
    data = (unsigned long)storeID;
    BYTE2(data) = recordType;

    switch(recordType)
    {
        case REC_TYPE_DEFINE_STORE:
            EZBL_PGM_FIFO_Write24(data); // REC_HEADER.recordType[1]:REC_HEADER.storeID[2]
            EZBL_SHA1_FIFO_Write24(data); // Add same data to hash
            if(recordData != NULL)
            {
                // Set attributes according to data that was passed in
                EZBL_PGM_FIFO_Write(recordData, 4); // EZBL_REC_DEFINE_STORE.storeFlags[4]
                EZBL_SHA1_FIFO_Write(recordData, 4); // Add same data to hash
            }
            else
            {
                // Set default attributes for new store
                attributes = EZBL_ATTRIBUTE_ERASABLE;
                if(storeID == 0x0000u)
                {
                    attributes |= EZBL_ATTRIBUTE_INSTALLED | EZBL_ATTRIBUTE_EXECUTABLE;
                }
                EZBL_PGM_FIFO_Write32(attributes); // EZBL_REC_DEFINE_STORE.storeFlags[4]
                EZBL_SHA1_FIFO_Write32(attributes); // Add same data to hash
            }
            EZBL_PGM_FIFO_Write(storeHash, sizeof(storeHash)); // EZBL_REC_DEFINE_STORE.hash[20]
            EZBL_SHA1_FIFO_Write(storeHash, sizeof(storeHash)); // Add same data to hash
            EZBL_PGM_FIFO_Write(EZBL_sectorBitmapRAM, EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP)); // EZBL_REC_DEFINE_STORE.sectorBitmap[] is next
            EZBL_SHA1_FIFO_Write(EZBL_sectorBitmapRAM, EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP)); // Add same data to hash
            break;

        case REC_TYPE_INVALIDATE:
            BYTE3(data) = ((EZBL_REC_INVALIDATE*)recordData)->priorRecordType;
            EZBL_PGM_FIFO_Write32(data); // REC_INVALIDATE.priorRecordType[1], REC_HEADER.recordType[1], REC_HEADER.storeID[2]
            EZBL_SHA1_FIFO_Write32(data); // Add same data to hash
            break;

        default:
            return -2;
    }

    EZBL_SHA1_FIFO_Flush(); // Compute journal hash
    EZBL_PGM_FIFO_Write32(EZBL_SHA1_FIFO_vars.hash[0]); // Write 32-bits of hash data (upper 128 bits truncated off)
    EZBL_PGM_FIFO_Flush();
    return 1;
}


// EZBL_JournalRead: Reads specified data from the journal
// journalTopic: Any of the JOURNAL_* constants
// storeID: unsigned int storeID parameter, if needed. If journalTopic doesn't relate to a store, then this is a don't care input.
// returns: depends on journalTopic
// *extendedData is only used for some topics (JOURNAL_STORE_HASH). It is otherwise ignored.
signed long EZBL_JournalRead(unsigned int journalTopic, unsigned int storeID, void *extendedData)
{
    unsigned long address;
    unsigned long storeAddress;
    unsigned int bitMask;
    unsigned int bitMaskIndex;
    unsigned int sectorUsed;


    // Recompute the journal base address if our RAM has been erased or
    // corrupted by the application (or more likely, the application's CRT
    // zeroing all RAM)
    if(currentJournalAddress != ~currentJournalAddressComplement)
    {
        EZBL_JournalConfigureAddress();
    }

    switch(journalTopic)
    {
        case JOURNAL_UNUSED_STOREID:
            for(storeID = 1; storeID < 0x7FFF; storeID++)
            {
                if(EZBL_JournalSearch(REC_TYPE_DEFINE_STORE, storeID) < 0)
                {
                    return storeID;
                }
            }
            return -2; // All exhausted from 0x0001 to 0x7FFF? That's hard to believe, but if you're here...

        case JOURNAL_STORE_COUNT:
            break;

        case JOURNAL_ENUM_STOREIDS:
            break;

        case JOURNAL_FREE_SECTOR:                   // Returns the free sector address, but doesn't mark it as used in EZBL_sectorBitmapRAM[]
        case JOURNAL_FREE_SECTOR_WITH_RESERVATION:  // Marks the returned free sector as used in the EZBL_sectorBitmapRAM[] bitmap array
            // Find the first unused sector starting from the last sector or
            // the sector immediately before sector with configuration fuses on
            // it.

            address = EZBL_SYM32(EZBL_LAST_SAFE_SECTOR_ADDRESS) + EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR);
            while(1)
            {
                // Decrement to next lower sector
                address -= EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR);

                // Skip over bootloader sectors
                if(EZBL_ROMAddressInBootloader(address) || EZBL_ROMAddressInJournal(address))
                {
                    continue;
                }

                // Check if this sector is free and if so, mark it as used and return it
                if(EZBL_SectorIsClear(address))
                {
                    // Compute proper bit position and index in sector use bitmaps
                    bitMask = address >> EZBL_SYM(EZBL_ADDRESS_TO_SECTOR_MAP_SHIFT);
                    bitMaskIndex = bitMask >> 3; // 8 bits-per byte
                    bitMask = 1 << (bitMask & 0x7u);

                    // See if any store has reserved this sector
                    sectorUsed = 0;
                    storeID = 0xFFFF;
                    while(1)
                    {
                        storeID = EZBL_JournalEnumStores(storeID, 0, 0);
                        if(storeID & 0x8000)
                        {
                            break;
                        }

                        // Find the journal define store record address of this storeID
                        storeAddress = (unsigned long)EZBL_JournalSearch(REC_TYPE_DEFINE_STORE, storeID); // This should always be successful since we just got the ID from EZBL_JournalEnumStores(), which did this same operation

                        // Read the needed sector bitmap byte. storeAddress is a convenient temporary variable to store the return result.
                        EZBL_ReadPackedFlash(&storeAddress, storeAddress, REC_OFFSET_DEFINE_STORE_SECTOR_BITMAP + bitMaskIndex, 1);

                        if((unsigned char)storeAddress & (unsigned char)bitMask)
                        {
                            // Sector already reserved, try next sector
                            sectorUsed = 1;
                            break;
                        }
                    }
                    if(sectorUsed)
                    {
                        continue;
                    }


                    if(journalTopic == JOURNAL_FREE_SECTOR_WITH_RESERVATION)
                    {
                        // Make sure we ourselves haven't already claimed this sector
                        if(EZBL_sectorBitmapRAM[bitMaskIndex] & (unsigned char)bitMask)
                        {
                            continue;
                        }

                        // Mark this sector as used in the sectorBitmap we are building
                        EZBL_sectorBitmapRAM[bitMaskIndex] |= bitMask;
                    }

                    return address;
                }

                // Return error if there are no free sectors left
                if(address == 0u)
                    return -1;
            }

        case JOURNAL_STORE_SECTOR_BITMAP:
            address = (unsigned long)EZBL_JournalSearch(REC_TYPE_DEFINE_STORE, storeID);
            // See if return address isn't a negative return code. Can't compare with < 0 because address is an unsigned long
            if(address & 0x80000000)
                return -1;

            EZBL_ReadPackedFlash(EZBL_sectorBitmapRAM, address, REC_OFFSET_DEFINE_STORE_SECTOR_BITMAP, EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP)); // Offset is byte offset of sectorBitmap[] in EZBL_REC_DEFINE_STORE structure
            return 1;

        case JOURNAL_STORE_FLAGS:
            address = (unsigned long)EZBL_JournalSearch(REC_TYPE_DEFINE_STORE, storeID);
            // See if return address isn't a negative return code. Can't compare with < 0 because address is an unsigned long
            if(address & 0x80000000)
                return -1;

            EZBL_ReadPackedFlash(&address, address, REC_OFFSET_DEFINE_STORE_STORE_FLAGS, 4);
            return address;

        case JOURNAL_STORE_HASH: // Returns the store's stored hash in EZBL_SHA1_FIFO_vars.fifoRAM (first 20 bytes)
            address = (unsigned long)EZBL_JournalSearch(REC_TYPE_DEFINE_STORE, storeID);
            // See if return address isn't a negative return code. Can't compare with < 0 because address is an unsigned long
            if(address & 0x80000000)
                return -1;

            EZBL_ReadPackedFlash(extendedData, address, REC_OFFSET_DEFINE_STORE_HASH, 20); // Offset is byte offset of hash in EZBL_REC_DEFINE_STORE structure
            return 1;
    }

    return -1; // Illegal read command
}


// Enumerates the sector addresses used by the given storeID in sequential order.
// Set lastAddress negative to find the starting sector. Returns the next
// sector address if found, -1 on EOF, or other negative value(s) on error.
// lastAddress need-not be aligned to the exact start of the sector as
// returned by this function. It can be any address within the same prior sector.
// Overwrites EZBL_sectorBitmap[] contents on start if usePreexistingBitmap
// is set. Otherwise, the preexisting contents of EZBL_sectorBitmap[] are
// used. Caller must not overwrite EZBL_sectorBitmap[] until done calling
// EZBL_EnumStoreAddresses().
signed long EZBL_EnumStoreAddresses(unsigned int storeID, signed long lastAddress, unsigned int usePreexistingBitmap)
{
    unsigned int bitMaskIndex;
    unsigned int bitMaskOffset;
    unsigned int sector;
    signed int preIncrement;
    unsigned int endSector;


    // Recompute the journal base address if our RAM has been erased or
    // corrupted by the application (or more likely, the application's CRT
    // zeroing all RAM)
    if(currentJournalAddress != ~currentJournalAddressComplement)
    {
        EZBL_JournalConfigureAddress();
    }

    // Initialize loop direction and start/end sector numbers
    if(storeID == 0u) // store0 goes forward, starting at sector 0
    {
        sector = (unsigned int) - 1;
        preIncrement = 1;
        endSector = EZBL_SYM(EZBL_FLASH_SECTOR_COUNT) - 1;
    }
    else
    { // Inactive stores go backwards, starting at the highest sector number
        sector = EZBL_SYM(EZBL_FLASH_SECTOR_COUNT);
        preIncrement = -1;
        endSector = 0;
    }

    // See if we are starting an enum and need to fetch the sector map
    if(lastAddress < 0)
    {
        if(!usePreexistingBitmap)
        {
            if(EZBL_JournalRead(JOURNAL_STORE_SECTOR_BITMAP, storeID, NULL) < 0)
                return -2;
        }
    }
    else
    {
        // Compute last sector index
        sector = ((unsigned long)lastAddress) >> EZBL_SYM(EZBL_ADDRESS_TO_SECTOR_MAP_SHIFT);

        // Return -1 if this is the absolute EOF sector limit (0 or total sectors - 1)
        if(sector == endSector)
            return -1;
    }

    // Loop over all sectors, looking for the first sector that is taken from the start condition
    do
    {
        sector += preIncrement;

        // See if this sector has data on it belonging to this store
        bitMaskOffset = sector & 0x7;
        bitMaskIndex = sector >> 3;
        if(EZBL_sectorBitmapRAM[bitMaskIndex] & (1 << bitMaskOffset))
        {
            // Compute Flash address from sector number
            return(signed long)(((unsigned long)sector) << EZBL_SYM(EZBL_ADDRESS_TO_SECTOR_MAP_SHIFT));
        }
    }
    while(sector != endSector);

    // No more bits to check, EOF reached
    return -1;
}


// Searches the journal for the address of the specified recordType, and if
// applicable, the given storeID. If the recordType doesn't apply to any given
// store (such as REC_TYPE_END_OF_JOURNAL for finding the address where to write
// the next journal entry), set storeID to 0xFFFF.
// Returns: >= 0 address of most up to date record found with matching recordType and storeID
//            -1 if the journal is uninitialized/corrupt/full
//            -2 if the no such recordType exists (for the given storeID, if applicable)
signed long EZBL_JournalSearch(unsigned char recordType, unsigned int storeID)
{
    unsigned long address;
    signed long ret;
    unsigned long data;


    // Recompute the journal base address if our RAM has been erased or
    // corrupted by the application (or more likely, the application's CRT
    // zeroing all RAM)
    if(currentJournalAddress != ~currentJournalAddressComplement)
    {
        EZBL_JournalConfigureAddress();
    }

    ret = -2;
    address = currentJournalAddress;
    while(1)
    {
        // Read a record header (REC_HEADER)
        data = EZBL_ReadFlash(address);

        // Check if the recordType field matches what we are looking for
        if(BYTE2(data) == recordType)
        {
            // See if storeID matches this one that we are looking for
            if(((unsigned int)data) == storeID)
            {
                // Match: save address, but continue scanning in case if a newer record has subsequently been written to the journal, invalidating this one
                ret = (signed long)address;
            }
        }

        // Process matching invalidation records
        if(BYTE2(data) == REC_TYPE_INVALIDATE)
        {
            // See if storeID matches this one that we are looking for
            if(((unsigned int)data) == storeID)
            {
                ret = -2;
            }
        }

        // Compute the address of the next record
        if(BYTE2(data) == REC_TYPE_DEFINE_STORE)
        {
            address += EZBL_SYM(EZBL_ADDRESSES_PER_REC_DEFINE_STORE);
        }
        else if(BYTE2(data) == REC_TYPE_INVALIDATE)
        {
            address += EZBL_SYM(EZBL_ADDRESSES_PER_REC_INVALIDATE);
        }
        else if(BYTE2(data) == REC_TYPE_END_OF_JOURNAL)
        {
            return ret;
        }
        else // Unrecognized or uninitialized journal entry, don't know how to many bytes to skip, so must bail
        {
            return -1; // Journal not in valid state
        }

        // Don't read past the journal end
        if(address + ADDR_PER_INSTR >= currentJournalAddress + EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR))
        {
            return -1;
        }
    }
}

// journalBaseAddress must be the base sector address of the journal to validate
// Requires use of the SHA1 FIFO, but it is available upon return
// Returns: <0 on invalid. Value returned is the negation of the approximate number of bytes processed before the invalid item was detected.
//           0 on valid, but empty
//           1 on valid and has data
signed int EZBL_JournalValidate(unsigned long journalBaseAddress)
{
    unsigned long address;
    unsigned long data;
    unsigned int bytesProcessed;
    signed int ret = 0;
    unsigned long sha1ContextSave[(sizeof(EZBL_SHA1_FIFO_vars) + (sizeof(unsigned long) - 1)) / sizeof(unsigned long)];

    // Goting to need SHA1_FIFO, but someone else might be using it. Save it's state.
    EZBL_RAMCopy(sha1ContextSave, &EZBL_SHA1_FIFO_vars, sizeof(EZBL_SHA1_FIFO_vars));

    address = journalBaseAddress;
    bytesProcessed = 0;

    // Enumerate each record in the given journal
    while(bytesProcessed < EZBL_SYM(EZBL_BYTES_PER_SECTOR))
    {
        // Read a record header (REC_HEADER)
        data = EZBL_ReadFlash(address);

        // Decode what do to based on the record type
        if(BYTE2(data) == REC_TYPE_DEFINE_STORE)
        {
            ret = 1;

            // Validate the hash
            EZBL_SHA1_FIFO_Reset();
            EZBL_SHA1_FIFO_vars.length = 0;
            EZBL_SHA1_FIFO_vars.opFlags = EZBL_SHA1_HASH;
            EZBL_PumpFlashIntoFIFO(journalBaseAddress, 0, bytesProcessed + REC_OFFSET_DEFINE_STORE_CHECKSUM, SHA1);
            EZBL_SHA1_FIFO_Flush();
            EZBL_ReadPackedFlash(&data, journalBaseAddress, bytesProcessed + REC_OFFSET_DEFINE_STORE_CHECKSUM, 4);
            if(data != EZBL_SHA1_FIFO_vars.hash[0])
            {
                // Restore SHA1_FIFO state and return error
                EZBL_RAMCopy(&EZBL_SHA1_FIFO_vars, sha1ContextSave, sizeof(EZBL_SHA1_FIFO_vars));
                return -bytesProcessed;
            }

            // Advance to the next record address
            address += EZBL_SYM(EZBL_ADDRESSES_PER_REC_DEFINE_STORE);
            bytesProcessed += EZBL_SYM(EZBL_BYTES_PER_REC_DEFINE_STORE);
        }
        else if(BYTE2(data) == REC_TYPE_INVALIDATE)
        {
            ret = 1;

            // Validate the hash
            EZBL_SHA1_FIFO_Reset();
            EZBL_SHA1_FIFO_vars.length = 0;
            EZBL_SHA1_FIFO_vars.opFlags = EZBL_SHA1_HASH;
            EZBL_PumpFlashIntoFIFO(journalBaseAddress, 0, bytesProcessed + REC_OFFSET_INVALIDATE_PRIOR_RECORD_CHECKSUM, SHA1);
            EZBL_SHA1_FIFO_Flush();
            EZBL_ReadPackedFlash(&data, journalBaseAddress, bytesProcessed + REC_OFFSET_INVALIDATE_PRIOR_RECORD_CHECKSUM, 4);
            if(data != EZBL_SHA1_FIFO_vars.hash[0])
            {
                // Restore SHA1_FIFO state and return error
                EZBL_RAMCopy(&EZBL_SHA1_FIFO_vars, sha1ContextSave, sizeof(EZBL_SHA1_FIFO_vars));
                return -bytesProcessed;
            }
            address += EZBL_SYM(EZBL_ADDRESSES_PER_REC_INVALIDATE);
            bytesProcessed += EZBL_SYM(EZBL_BYTES_PER_REC_INVALIDATE);
        }
        else if(BYTE2(data) == REC_TYPE_END_OF_JOURNAL)
        {
            if((unsigned int)data != 0xFFFFu)
            {
                // Restore SHA1_FIFO state and return error
                EZBL_RAMCopy(&EZBL_SHA1_FIFO_vars, sha1ContextSave, sizeof(EZBL_SHA1_FIFO_vars));
                return -bytesProcessed;
            }

            address += ADDR_PER_INSTR;
            bytesProcessed += BYTES_PER_INSTR;
        }
        else // Unrecognized, or uninitialized journal entry, don't know how to many bytes to skip, so treat this journal as invalid
        {
            // Restore SHA1_FIFO state and return error
            EZBL_RAMCopy(&EZBL_SHA1_FIFO_vars, sha1ContextSave, sizeof(EZBL_SHA1_FIFO_vars));
            return -bytesProcessed;
        }
    }

    // Restore SHA1_FIFO state and return success
    EZBL_RAMCopy(&EZBL_SHA1_FIFO_vars, sha1ContextSave, sizeof(EZBL_SHA1_FIFO_vars));
    return ret;
}

// Returns 0 if the journal is already as compact as possible or 1 if obsolete
// records can be removed. This function assumes the given journal is 100%
// valid. Use JournalValid() on the given journal address if unsure.
unsigned int EZBL_JournalCompactable(unsigned long journalAddress)
{
    unsigned long data;
    unsigned int bytesProcessed;
    unsigned long latestAddress;
    unsigned long currentJournalAddressSave;
    unsigned long journalAddressStart;

    journalAddressStart = journalAddress;
    bytesProcessed = 0;

    // Enumerate each record in the given journal
    while(bytesProcessed < EZBL_SYM(EZBL_BYTES_PER_SECTOR))
    {
        // Read a record header (REC_HEADER)
        data = EZBL_ReadFlash(journalAddress);

        // Decode what do to based on the record type
        if(BYTE2(data) == REC_TYPE_DEFINE_STORE)
        {
            // Search the journal for an updated record for this storeID.
            // Because EZBL_JournalSearch() uses the currentJournalAddress global
            // variable as the target, we must save it, change it to the current
            // journal base address we are inspecting, and then restore it since
            // the inspection sector may not be the currently active journal
            // sector.
            currentJournalAddressSave = currentJournalAddress;
            currentJournalAddress = journalAddressStart;
            latestAddress = EZBL_JournalSearch(REC_TYPE_DEFINE_STORE, (unsigned int)data);
            currentJournalAddress = currentJournalAddressSave;
            if(latestAddress != journalAddress)
            {
                return 1;
            }

            // Advance to the next record address
            journalAddress += EZBL_SYM(EZBL_ADDRESSES_PER_REC_DEFINE_STORE);
            bytesProcessed += EZBL_SYM(EZBL_BYTES_PER_REC_DEFINE_STORE);
        }
        else if(BYTE2(data) == REC_TYPE_INVALIDATE)
        {
            return 1;
        }
        else if(BYTE2(data) == REC_TYPE_END_OF_JOURNAL)
        {
            return 0;
        }
    }

    return 0;
}

// Compacts the current Journal by copying only still valid and most up to date
// records to the destination Flash address. The current (non-compacted) journal
// sector is then erased and currentJournalAddress is updated to match
// destAddress. destAddress must be the address of a fully erased
// sector. Function does nothing if the source journal is already optimally
// compact.
void EZBL_JournalCompact(void)
{
    unsigned int bytesProcessed;
    unsigned int bytesInDest;
    unsigned long data;
    unsigned long sourceAddress;
    unsigned long destAddress;
    unsigned long destAddressStart;
    unsigned long latestAddress;

    // Do nothing if the journal is already as compact as possible.
    if(!EZBL_JournalCompactable(currentJournalAddress))
    {
        return;
    }

    // Choose the right destination sector address, and make a copy for manipulation
    destAddressStart = (currentJournalAddress == EZBL_SYM32(EZBL_JOURNAL_BASE)) ? EZBL_SYM32(EZBL_JOURNAL_SECTOR2) : EZBL_SYM32(EZBL_JOURNAL_BASE);
    destAddress = destAddressStart;


    // Save the destination journal address. We need to set
    // currentJournalAddress to it when done.


    // Enumerate each record in the active source journal
    sourceAddress = currentJournalAddress;
    bytesInDest = 0;
    bytesProcessed = 0;
    while(bytesProcessed < EZBL_SYM(EZBL_BYTES_PER_SECTOR))
    {
        // Read a record header (REC_HEADER)
        data = EZBL_ReadFlash(sourceAddress);

        // Decode what do to based on the record type
        if(BYTE2(data) == REC_TYPE_DEFINE_STORE)
        {
            // Search for a later update of this record and storeID combo, or
            // an invalidation record causing us to skip copying to the compact
            // journal.
            latestAddress = EZBL_JournalSearch(REC_TYPE_DEFINE_STORE, (unsigned int)data);

            // If record is valid and it is the latest version, copy it over
            // to the PGM_FIFO, which writes it into the alternate journal
            if((latestAddress & 0x8000000) == 0u)
            {
                if(sourceAddress == latestAddress)
                {
                    // Compute Sector 2 hash checksum up to the end so far, plus the data we are about to write.
                    EZBL_SHA1_FIFO_Reset();
                    EZBL_SHA1_FIFO_vars.length = 0;
                    EZBL_SHA1_FIFO_vars.opFlags = EZBL_SHA1_HASH;
                    EZBL_PumpFlashIntoFIFO(destAddressStart, 0, bytesInDest, SHA1);
                    EZBL_PumpFlashIntoFIFO(latestAddress, 0, REC_OFFSET_DEFINE_STORE_CHECKSUM, SHA1);
                    EZBL_SHA1_FIFO_Flush();

                    // Push the source journal entry into the destination journal
                    EZBL_PGM_FIFO_Reset();
                    EZBL_PGM_FIFO_vars.destAddress = destAddress;
                    EZBL_PGM_FIFO_vars.opFlags = EZBL_PGM_TARGET_JOURNAL;
                    EZBL_PumpFlashIntoFIFO(latestAddress, 0, REC_OFFSET_DEFINE_STORE_CHECKSUM, PGM);
                    EZBL_PGM_FIFO_Write32(EZBL_SHA1_FIFO_vars.hash[0]); // Write the 32-bit checksum computed above (tuncated hash value)
                    EZBL_PGM_FIFO_Flush();
                    destAddress += EZBL_SYM(EZBL_ADDRESSES_PER_REC_DEFINE_STORE);
                    bytesInDest += EZBL_SYM(EZBL_BYTES_PER_REC_DEFINE_STORE);
                }
            }

            // Advance to the next record address
            sourceAddress += EZBL_SYM(EZBL_ADDRESSES_PER_REC_DEFINE_STORE);
            bytesProcessed += EZBL_SYM(EZBL_BYTES_PER_REC_DEFINE_STORE);
        }
        else if(BYTE2(data) == REC_TYPE_INVALIDATE)
        {
            // Advance to the next record address
            sourceAddress += EZBL_SYM(EZBL_ADDRESSES_PER_REC_INVALIDATE);
            bytesProcessed += EZBL_SYM(EZBL_BYTES_PER_REC_INVALIDATE);
        }
        else if(BYTE2(data) == REC_TYPE_END_OF_JOURNAL)
        {
            // All done compacting the journal.
            // Erase the non-compact journal and set currentJournalAddress to
            // this new compact one.
            EZBL_StartEraseMin(currentJournalAddress);
            currentJournalAddress = destAddressStart;
            return;
        }
    }
}

// Validates the journal sectors and searches for the proper journal start
// address. This value is saved in the currentJournalAddress global variable.
// This function must be called before calling any other Journal functions on
// reset and on first call of EZBL_Command() from the application (since the
// application's CRT will likely zero out currentJournalAddress). If the
// possibility of currentJournalAddresss getting corrupted by the application
// exists, JournalConfigureAddress() should be called on every EZBL_Command()
// entry that needs the journal so as always regenerate correct address (at a
// performance penalty of wasting time revalidating the journal which will
// normally be good).
void EZBL_JournalConfigureAddress(void)
{
    signed int journalState[2]; // < 0 invalid, 0 clear, 1 valid with data

    journalState[0] = EZBL_JournalValidate(EZBL_SYM32(EZBL_JOURNAL_BASE));
    journalState[1] = EZBL_JournalValidate(EZBL_SYM32(EZBL_JOURNAL_SECTOR2));

    if(journalState[0] < 0)
    {
        EZBL_StartEraseMin(EZBL_SYM32(EZBL_JOURNAL_BASE));
        journalState[0] = 0;
    }
    if(journalState[1] < 0)
    {
        EZBL_StartEraseMin(EZBL_SYM32(EZBL_JOURNAL_SECTOR2));
        journalState[1] = 0;
    }

    // See if both are valid and both have data (not likely, but possible if we finished compacting the journal and lost power before erasing the uncompact source
    if(journalState[0] & journalState[1])
    {
        if(EZBL_JournalCompactable(EZBL_SYM32(EZBL_JOURNAL_BASE)))
        {
            EZBL_StartEraseMin(EZBL_SYM32(EZBL_JOURNAL_BASE));
            journalState[0] = 0;
        }
        else if(EZBL_JournalCompactable(EZBL_SYM32(EZBL_JOURNAL_SECTOR2)))
        {
            EZBL_StartEraseMin(EZBL_SYM32(EZBL_JOURNAL_SECTOR2));
            journalState[1] = 0;
        }
        else
        { // This should never happen (both journals are valid and both compact), but it is probably a good idea to handle it anyway.
            EZBL_StartEraseMin(EZBL_SYM32(EZBL_JOURNAL_SECTOR2));
            currentJournalAddress = EZBL_SYM32(EZBL_JOURNAL_BASE);
            currentJournalAddressComplement = ~currentJournalAddress;
            return;
        }
    }


    if(journalState[0])
    {
        currentJournalAddress = EZBL_SYM32(EZBL_JOURNAL_BASE);
        currentJournalAddressComplement = ~currentJournalAddress;
        return;
    }

    currentJournalAddress = EZBL_SYM32(EZBL_JOURNAL_SECTOR2);
    currentJournalAddressComplement = ~currentJournalAddress;
}

// Enumerates REC_TYPE_DEFINE_STORE type records in the journal (that haven't
// been invalidated) and match the associated attributes (masked by
// attributeMask), and returns the corresponding storeID.
// lastStoreID: Pass the last positive storeID returned by EZBL_JournalEnumStores()
// to retrieve the next higher valid store number. When no more stores exist,
// -1 is returned. When starting (or restarting) an enumeration, set
// lastStoreID to 0xFFFF (or any other value with the MSbit set);
// attributeMask: bitmask to bitwise AND with the stored attributes before comparison to the attributes parameter. If any attributes are desired for this enumeration, pass in 0 for attributeMask.
// attributes: Bitwise combination of EZBL_ATTRIBUTE_* flags. Only the bits set to 1 in attributeMask are used. Other bits within attributes are ignored.
// Returns: >= 0 storeID number that was found
//            -1 No more stores recorded in the journal
//            -2 if the journal is uninitialized/corrupt/full
signed int EZBL_JournalEnumStores(unsigned int lastStoreID, unsigned long attributeMask, unsigned long attributes)
{
    unsigned long address;
    unsigned long latestAddress;
    unsigned long data;
    unsigned long storeAttributes;
    unsigned int biggestStoreIDOption;


    // Recompute the journal base address if our RAM has been erased or
    // corrupted by the application (or more likely, the application's CRT
    // zeroing all RAM)
    if(currentJournalAddress != ~currentJournalAddressComplement)
    {
        EZBL_JournalConfigureAddress();
    }

    address = currentJournalAddress;
    biggestStoreIDOption = 0xFFFF;
    while(1)
    {
        // Read a record header (REC_HEADER)
        data = EZBL_ReadFlash(address);

        // Check if the recordType field matches what we are looking for
        if(BYTE2(data) == REC_TYPE_DEFINE_STORE)
        {
            // See if storeID is higher than the last one enumerated
            if(((signed int)(unsigned int)data) > (signed int)lastStoreID)
            {
                // But, make sure we don't return an option prematurely if there are other possibilities with lower sequential store IDs later in the journal
                if((unsigned int)data < biggestStoreIDOption)
                {
                    // Yes, call EZBL_JournalSearch() to see if there has been subsequent invalidation records or redefinition records applied to it
                    latestAddress = EZBL_JournalSearch(REC_TYPE_DEFINE_STORE, (unsigned int)data);
                    if(!(BYTE3(latestAddress) & 0x80))
                    { // Found latest store record with this storeID; store not been invalidated
                        // Check attributes for match
                        storeAttributes = EZBL_JournalRead(JOURNAL_STORE_FLAGS, (unsigned int)data, NULL);
                        storeAttributes &= attributeMask;
                        attributes &= attributeMask;
                        if(attributes == storeAttributes)
                        { // Match - return this storeID
                            biggestStoreIDOption = (unsigned int)data;
                            if(biggestStoreIDOption == lastStoreID + 1u)
                                return (signed int)biggestStoreIDOption;
                        }
                    }
                }
            }
        }

        // Compute the address of the next record
        if(BYTE2(data) == REC_TYPE_DEFINE_STORE)
        {
            address += EZBL_SYM(EZBL_ADDRESSES_PER_REC_DEFINE_STORE);
        }
        else if(BYTE2(data) == REC_TYPE_INVALIDATE)
        {
            address += EZBL_SYM(EZBL_ADDRESSES_PER_REC_INVALIDATE);
        }
        else if(BYTE2(data) == REC_TYPE_END_OF_JOURNAL)
        {
            return (signed int)biggestStoreIDOption; // End of journal; no more stores to enumerate
        }
        else // Unrecognized or uninitialized journal entry, don't know how to many bytes to skip, so must bail
        {
            return -2; // Journal not in valid state
        }

        // Don't read past the journal end
        if(address + ADDR_PER_INSTR >= currentJournalAddress + EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR))
        {
            return (signed int)biggestStoreIDOption; // End of journal; no more stores to enumerate and no more records to parse because the journal is full
        }
    }
}

// Read an inactive blob store and attempts to install it into store0. Device
// resets on success. On failure, and error code < 0 is returned:
//      -1 store0 is marked as not erasable and ignoreStore0Attrbiutes is zero.
//      -2 storeID invalid
//      -3 blob decoding error (blob contents invalid)
//      -4 blob hash error (blob contents invalid)
//      -5 unsupported address overlap between the blob in inactive store and the needed output store0 address ranges for the blob
//      -6 store0 Flash erase error
//      -7 store0 Flash write error
signed int EZBL_InstallStoreToStore0(unsigned int storeID, unsigned int ignoreStore0Attributes)
{
    unsigned int i;
    unsigned long address;
    unsigned int bitMaskOffset, bitMaskIndex;
    unsigned long storedHash[5];

    // Read store0 attributes
    if(!ignoreStore0Attributes)
    {
        address = EZBL_JournalRead(JOURNAL_STORE_FLAGS, 0x0000, NULL);
        if((BYTE3(address) & 0x80u) == 0x00u)
        {
            // Check if erasable attribute is set
            if(!(address & EZBL_ATTRIBUTE_ERASABLE))
                return -1; // store0 is not attributed as erasable (use EZBL_OP_SET_ATTRIBUTES to clear the erasable attribute first)
        }
    }

    // Read the expected hash from the journal
    if(EZBL_JournalRead(JOURNAL_STORE_HASH, storeID, storedHash) < 0)
        return -2; // Invalid storeID

    // Push all the data through the BLOB FIFO to obtain the sector bitmap needed after installation
    // This also forwards data to the SHA1 FIFO for hashing to verify the blob
    EZBL_BLOB_FIFO_Reset();
    EZBL_BLOB_FIFO_vars.state = 0; // Reset blob processor state machine
    EZBL_BLOB_FIFO_vars.opFlags = EZBL_BLOB_INCOMING | EZBL_BLOB_MAP_SECTORS | EZBL_BLOB_HASH; // Do not actually write anything, just process the blob so we know which sectors are needed for the full blob installation
    EZBL_RAMSet(EZBL_sectorBitmapRAM2, 0x00, EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP));
    if(EZBL_PumpStoreIntoFIFO(storeID, BLOB, 0) < 0)
        return -3; // Invalid BLOB contents

    // Discontinue installation if decoding or verification of the blob failed
    if((EZBL_BLOB_FIFO_vars.opFlags & (EZBL_BLOB_FAIL | EZBL_BLOB_DONE)) != EZBL_BLOB_DONE)
    {
        return -3; // Invalid BLOB contents
    }

    // Discontinue installation if the blob hash doesn't match what we have stored
    if(EZBL_RAMCompare(storedHash, EZBL_SHA1_FIFO_vars.hash, sizeof(storedHash) != 0u))
    {
        return -4; // BLOB hash invalid (blob corrupted)
    }


    // Check for impossible conditions, correct any overlap between the source and destination
    for(i = 0; i < EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP); i++)
    {
        if(EZBL_sectorBitmapRAM[i] & EZBL_sectorBitmapRAM2[i])
        {
            // Overlap exists, which isn't handled yet. For now, if this happens, you must erase all the inactive stores and then reprogram a new image which doesn't use any of the high sectors (ex: above >50% of the Flash memory)
            return -5; // Unhandled address overlap
        }
    }

    // Manipulating store0 is dangerous, we most not let an ISR or trap trigger once we start erasing the current store0 since the ISR will be in store0
    EZBL_DisableInterrupts();

    // Erase all the destination sectors, except sector 0 and the last sector of Flash (if it has config fuse words on it)
    for(address = EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR); address <= EZBL_SYM32(EZBL_LAST_SAFE_SECTOR_ADDRESS); address += EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR))
    {
        bitMaskIndex = address >> EZBL_SYM(EZBL_ADDRESS_TO_SECTOR_MAP_SHIFT);
        bitMaskOffset = bitMaskIndex & 0x7;
        bitMaskIndex >>= 3;
        if(EZBL_sectorBitmapRAM2[bitMaskIndex] & (1 << bitMaskOffset)) // See if sector is needed
        {
            // Erase the sector, if it isn't already. Error out if we can't erase it.
            if(EZBL_Erase(address, address) < 0)
                return -6;
        }
    }

    // Seems to be all ready for installation, install store1 to store0
    EZBL_BLOB_FIFO_Reset();
    EZBL_BLOB_FIFO_vars.state = 0;
    EZBL_BLOB_FIFO_vars.opFlags = EZBL_BLOB_INCOMING | EZBL_BLOB_WRITE; // Program this time
    if(EZBL_PumpStoreIntoFIFO(storeID, BLOB, 0) < 0)
    {
        return -7;
    }

    // Update store0 in the journal; we do not check the return value because there is nothing we can do if the journal write fails
    EZBL_RAMCopy(EZBL_sectorBitmapRAM, EZBL_sectorBitmapRAM2, EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP));
    EZBL_JournalWrite(REC_TYPE_DEFINE_STORE, 0x0000, NULL);

    // All done, reset and start running new application
    Reset();

    // Supress control reaches end of non-void function warning
    __builtin_unreachable();
}


// Stub for calling EZBL_OnCommand() when the linkage between the two is not defined in the linker script.
signed short __attribute__((weak)) EZBL_Command(unsigned short operation, unsigned short storeID, void *inOutData, unsigned long inOutDataLen)
{
    return EZBL_OnCommand(operation, storeID, inOutData, inOutDataLen);
}

// EZBL_OnCommand() and EZBL_Command() is for the old multistore bootloading 
// method. Do not call this function in new bootloader code.
//
// This is effectively the EZBL_Command() function. In reality,
// EZBL_Command() is just a vectored address jump table element that when
// called, simply branches to this function.
signed short EZBL_OnCommand(unsigned short operation, unsigned short storeID, void *inOutData, unsigned long inOutDataLen)
{
    unsigned long address;
    signed int ret;

    switch(operation)
    {
        case EZBL_OP_ADD_STORE:
        {
            // Clear the sector bitmap accumulator variable
            EZBL_RAMSet(EZBL_sectorBitmapRAM, 0x00, EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP));

            // Set up programming FIFO
            EZBL_PGM_FIFO_Reset();
            EZBL_PGM_FIFO_vars.opFlags = EZBL_PGM_TARGET_INACTIVE_STORE;
            EZBL_PGM_FIFO_vars.destAddress = EZBL_JournalRead(JOURNAL_FREE_SECTOR_WITH_RESERVATION, 0, NULL);
            if(((signed long)EZBL_PGM_FIFO_vars.destAddress) < 0)
            {
                return -1;
            }

            // Set up communications FIFO
            EZBL_COM_FIFO_Reset();
            EZBL_COM_FIFO_vars.opFlags = EZBL_COM_WRITE_START | EZBL_COM_WRITING;


            // Return the storeID that we found as free
            return EZBL_JournalRead(JOURNAL_UNUSED_STOREID, 0, NULL);
        }

        case EZBL_OP_DELETE_STORE:
            // Read store attributes
            address = EZBL_JournalRead(JOURNAL_STORE_FLAGS, storeID, NULL);
            if(BYTE3(address) & 0x80u)
                return -1; // Invalid storeID

            // Check if erasable attribute is set
            if(!(address & EZBL_ATTRIBUTE_ERASABLE))
                return -2; // Store is not attributed as erasable (use EZBL_OP_SET_ATTRIBUTES to clear the erasable attribute first if the store needs to be erased)

            if(storeID == 0u)
            {
                if(EZBL_JournalEnumStores(0, 0, 0) & 0x8000)
                    return -3; // Don't allow erasing store0 if there aren't any other stores defined
            }

            ret = EZBL_EraseStore(storeID);

            if(storeID == 0u)
            {
                Reset();
            }

            return ret;

        case EZBL_OP_INSTALL_STORE:
            return EZBL_InstallStoreToStore0(storeID, 0);

        case EZBL_OP_WRITE:
            if((EZBL_COM_FIFO_vars.opFlags & EZBL_COM_WRITING) == 0x0000u) // Don't allow programming if we weren't expecting anything
            {
                return -1;
            }
            // Pass data to FIFOs
            EZBL_COM_FIFO_Write(inOutData, inOutDataLen);

            // If less than 4 bytes have arrived, we still don't know how many we shall need
            if(EZBL_COM_FIFO_vars.opFlags & EZBL_COM_WRITE_START)
            {
                return 4 - EZBL_COM_FIFO_ReadableLength();
            }

            // Check if the blob is finished being written and can be committed to the journal
            if((EZBL_COM_FIFO_vars.opFlags & EZBL_COM_WRITING) == 0x0000u)
            {
                // Save this completed store bitmap and information in the journal
                if(EZBL_JournalWrite(REC_TYPE_DEFINE_STORE, storeID, NULL) <= 0)
                    return -2;
            }

            // If the number of bytes still needed can fit in 15 bits, then use it directly
            inOutDataLen = EZBL_COM_FIFO_vars.length - EZBL_COM_FIFO_ReadableLength();
            if(inOutDataLen < 0x7FFF)
                return(int)inOutDataLen;

            // Otherwise, return the maximal count of how many more bytes we can report as needed
            return 0x7FFF;

        case EZBL_OP_GET_HASH:
            EZBL_SHA1_FIFO_Reset();
            EZBL_SHA1_FIFO_vars.length = 0;
            EZBL_SHA1_FIFO_vars.opFlags = EZBL_SHA1_HASH;
            if(storeID > 0u)
            {
                EZBL_SHA1_FIFO_vars.opFlags |= EZBL_SHA1_ZERO_BLOBS_HASH;
            }

            EZBL_PumpStoreIntoFIFO(storeID, SHA1, 0);
            EZBL_SHA1_FIFO_Flush();

            // Copy no more than 20 bytes needed for the Hash into the caller's buffer
            if(inOutDataLen > sizeof(EZBL_SHA1_FIFO_vars.hash))
            {
                inOutDataLen = sizeof(EZBL_SHA1_FIFO_vars.hash);
            }
            EZBL_RAMCopy(inOutData, EZBL_SHA1_FIFO_vars.hash, inOutDataLen);
            return inOutDataLen;

        case EZBL_OP_ENUM_STORES:
            return EZBL_JournalEnumStores(storeID, 0, 0);

        case EZBL_OP_GET_ATTRIBUTES:
            address = EZBL_JournalSearch(REC_TYPE_DEFINE_STORE, storeID);
            if(BYTE3(address) & 0x80)
            {
                return -1; // Invalid storeID
            }
            if(inOutDataLen > 4u) // Truncate attributes length to 4 if more buffer space was provided
            {
                inOutDataLen = 4;
            }
            EZBL_ReadPackedFlash(inOutData, address, REC_OFFSET_DEFINE_STORE_STORE_FLAGS, inOutDataLen);
            return inOutDataLen;

        case EZBL_OP_SET_ATTRIBUTES:
            // Check if the specified store exists, and simultaneously read it's saved hash
            if(EZBL_JournalRead(JOURNAL_STORE_HASH, storeID, EZBL_SHA1_FIFO_vars.hash) < 0)
            {
                return -10; // Invalid storeID
            }
            if(inOutDataLen != sizeof(unsigned long))
            {
                return -11; // Invalid inOutDataLen length (must be 4)
            }

            // Update the store attributes
            EZBL_JournalRead(JOURNAL_STORE_SECTOR_BITMAP, storeID, NULL); // Load up the sectorBitmap[0] first so we don't write bogus data into the journal. Return value not checked since it will always succeed after finding the hash
            return EZBL_JournalWrite(REC_TYPE_DEFINE_STORE, storeID, inOutData);

        default:
            break;
    }

    return -10; // EZBL_OP operation not supported at this time
}

// EZBL_SectorIsClear() is for the old multistore bootloading method. Do not 
// call this function in new bootloader code. Use the EZBL_IsPageErased() 
// function instead.
//
// Function blocks if the Flash is currently being erased or programmed.
// address is any program memory address. The whole sector it lies on is checked.
signed int EZBL_SectorIsClear(unsigned long address)
{
    unsigned int i;
    unsigned long readData;
    unsigned long reservedBitAddress, reservedBitMask;
    union
    {
        unsigned int int_val[2];
        unsigned long long_val;
    } ourResetAddress;

    // Start at the beginning of the sector and read each instruction word
    address &= EZBL_SYM32(EZBL_SECTOR_ADDRESS_MASK);

    // Load some constants into RAM for quick repeated accesses
    reservedBitAddress = EZBL_SYM32(EZBL_RESERVED_BIT_ADDRESS);
    reservedBitMask = EZBL_SYM32(EZBL_RESERVED_BIT_MASK);
    ourResetAddress.long_val = EZBL_WeakSYM32(__reset);
    if(ourResetAddress.long_val == 0u)
    {
        ourResetAddress.long_val = EZBL_WeakSYM32(EZBL_Reset);
    }
    if(ourResetAddress.long_val == 0u)
    {
        ourResetAddress.long_val = EZBL_WeakSYM32(EZBL_Reset);
    }

    i = EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR)/2u;
    while(i--)
    {
        // Read one instruction word
        readData = EZBL_ReadFlash(address);

        // Check if this is a special reserved bit address, which is always zero
        if(address == reservedBitAddress)
        {
            if((readData | reservedBitMask) != 0x00FFFFFFu)
            {
                return 0;
            }
        }
        else if(address == 0x000000u) // Check if this is address 0x000000 where a 2 word GOTO _EZBL_Reset instruction must sit
        {
            if(readData != (0x040000 | ourResetAddress.int_val[0]))
            {
                return 0;
            }
        }
        else if(address == 0x000002u) // Check if this is address 0x000002 where a 2 word GOTO _EZBL_Reset instruction must sit
        {
            if(readData != ourResetAddress.int_val[1])
            {
                return 0;
            }
        }
        else if(readData != 0x00FFFFFFu) // Check if the read data is otherwise all 1's, meaning unprogrammed
        {
            return 0;
        }

        // Increment address to next instruction
        address += 2u;
    }

    // If we make it down here, that means the whole sector was unprogrammed (except for possible required items above)
    return 1;
}

// EZBL_SectorIsClear() is for the old multistore bootloading method. Do not 
// call this function in new bootloader code. Use the EZBL_ErasePage(), 
// EZBL_EraseAll() and/or EZBL_EraseAllPiecewise() functions instead.
// 
// lowAddress: unsigned long program memory address lower limit, inclusive. Note that erase granularity is 1 full sector, so all data before lowAddress that sits on the same sector is also erased. Put another way, the least significant lowAddress bits indicating the word within the sector are ignored and assumed to be all zeros.
// highAddress: unsigned long program memory address upper limit, inclusive. Just like lowAddress, erase granularity is 1 full sector, so all data after highAddress that sits on the same sector is also erased. Put another way, the least significant highAddress bits indicating the word within the sector are ignored and assumed to be all ones.
// return: Positive value on success. Negative error code on failure. 0 undefined.
// Notes: Erasing occurs starting at the sector immediately preceeding highAddress and working backwards towards lowAddress. Erasure wraps around to the original highAddress's sector once lowAddress is reached (or exceeded if they start out pointing to the same sector), and finally highAddress is erased. This is purposely done in backwards order to retain the Code Protect configuration fuse all the way up until the device is blank (less the bootloader itself and possibly sector 0). lowAddress cannot point to a higher sector address than highAddress does (pointing to a higher address on the same sector is okay, though). This function cannot be used to erase the bootloader or bootloader journal.
signed int EZBL_Erase(unsigned long lowAddress, unsigned long highAddress)
{
    unsigned char tryCount;
    unsigned long stopAddress;
    unsigned int intSave;

    // Convert addresses to sector aligned addresses
    lowAddress &= EZBL_SYM32(EZBL_SECTOR_ADDRESS_MASK);
    highAddress &= EZBL_SYM32(EZBL_SECTOR_ADDRESS_MASK);

    // Bound the highAddress given to the last legal address of Flash.
    // Attempting to access beyond the last legal address causes an
    // Address Error Trap.
    if(highAddress >= EZBL_SYM32(EZBL_MAIN_FLASH_END_ADDRESS))
    {
        highAddress = EZBL_SYM32(EZBL_LAST_SECTOR_ADDRESS);
    }


    // Disallow potentially illegal condition where lowAddress is bigger than highAddress
    if(lowAddress > highAddress)
    {
        return -1;
    }

    // Disallow potentially hazardous fuse erase if not specifically enabled
    if(EZBL_WeakSYM(EZBL_CONFIG_ALLOW_ERASE_FUSES) == 0u)
    {
        if((lowAddress <= EZBL_SYM32(EZBL_CONFIG_SECTOR_ADDRESS)) && (highAddress >= EZBL_SYM32(EZBL_CONFIG_SECTOR_ADDRESS)))
        {
            return -2;
        }
    }

    // Choose the ending address
    stopAddress = highAddress;

    // Loop over every sector in the specified address range
    do
    {
        // We are about to pre-decrement the pointer. If it is going to
        // underflow, prevent this by wrapping around to stopAddress + 1 sector,
        // so the decrement will stop at StopAddress.
        if(highAddress == 0x00000000u)
        {
            highAddress = stopAddress + (unsigned long)EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR);
        }

        // Pre-decrement the highAddress pointer, which we will be using to
        // chose which sector to erase. See function notes for reason why we
        // are using this abnormal ordering.
        highAddress -= (unsigned long)EZBL_SYM(EZBL_ADDRESSES_PER_SECTOR);

        // If we have decremented beyond the startAddress, wrap back around to
        // the final stopAddress sector
        if(highAddress < lowAddress)
        {
            highAddress = stopAddress;
        }

        // Do not allow erasing of ourself or the journal
        if(EZBL_ROMAddressInBootloader(highAddress))
            continue;

        // Do not allow erasing of the first sector, which would be dangerous 
        // since that is where the IVT and Reset vector are.
        if(highAddress == 0x00000000u)
            continue;


        // Repetitively try to erase a sector. This is done in a while loop so
        // that we don't necessarily have to disable interrupts. If the WR bit
        // fails to set due to an interrupt occurring in the middle of the 
        // unlock sequence, we'll just repeat the process. This also provides 
        // automatic retry on erase failure (should never happen, but might 
        // still happen if you are out of Erase/Write endurance cycles or 
        // operating the device outside of legal conditions, such as too low 
        // voltage or excessive temperature).
        tryCount = 0;
        while(!EZBL_SectorIsClear(highAddress)) // Check to see if the sector is already erased or successfully erased before trying to erase it again.
        {
            // Make sure we don't repetitively erase the same sector over and
            // over again, wasting Flash endurance due to a code bug, such as
            // attempting to erase the last sector of a PIC24F where hardware
            // always programs a '0' bit in the Flash configuration values section.
            if(tryCount >= 2u)
            {
                return -1; // Retry count failure on sector erase
            }

            // Start a (possibly) non-blocking erase attempt
            intSave = EZBL_DisableInterrupts();
            EZBL_StartEraseMin(highAddress);

            // If we are erasing Sector 0, immediately replace goto instruction at address 0x000000
            if(highAddress == 0x000000u)
            {
                EZBL_PGM_FIFO_Reset();
                EZBL_PGM_FIFO_vars.opFlags = EZBL_PGM_TARGET_RESET_VECTOR;
                EZBL_PGM_FIFO_vars.destAddress = 0x00000000;
                EZBL_PGM_FIFO_Write24(0x040000 | EZBL_SYM(EZBL_Reset));
                EZBL_PGM_FIFO_Write24(0x000000 | (unsigned short)(EZBL_SYM32(EZBL_Reset) >> 16));
                EZBL_PGM_FIFO_Flush();
            }
            else if(highAddress == (EZBL_SYM32(EZBL_RESERVED_BIT_ADDRESS) & EZBL_SYM32(EZBL_SECTOR_ADDRESS_MASK)))
            {// If we are erasing the last sector, immediately reprogram the reserved bit
                unsigned long sign_data = ~EZBL_SYM32(EZBL_RESERVED_BIT_MASK);

                EZBL_PGM_FIFO_Reset();
                EZBL_PGM_FIFO_vars.opFlags = EZBL_PGM_TARGET_FUSES;
                EZBL_PGM_FIFO_vars.destAddress = EZBL_SYM32(EZBL_RESERVED_BIT_ADDRESS);
                EZBL_PGM_FIFO_Write24(sign_data);
                EZBL_PGM_FIFO_Flush();
            }
            EZBL_RestoreInterrupts(intSave);
            tryCount++;
        }
    }
    while(highAddress != stopAddress);

    return 1; // Success and no impossible regions encountered.
}



// EZBL_StartEraseMin() is for the old multistore bootloading method. Do not 
// call this function in new bootloader code. Use the EZBL_ErasePage(), 
// EZBL_EraseAll() and/or EZBL_EraseAllPiecewise() functions instead.
// 
// Starts erasure of the minimum block size containing the specified address.
// The minimum block size is typically one Flash Sector or "Page". If the
// address is within bootloader reserved region, the function safely does
// nothing. If the address is unimplemented, the function safely does nothing.
// If the target sector is already erased, the function will still start an
// erase, so make sure not to call this function unless needed. Erasing an
// already erased sector will burn through an erase/write endurance cycle,
// even if nothing was ever written. Use SectorIsClear() to tell if the sector
// needs erasing ahead of time.
//
// For dual panel devices or cases where erasing does not necessarily cause the
// CPU to halt execution, use EraseProgDone() to determine when the erase is
// complete.
//
// If EZBL_StartEraseMin() is called while a prior erase or program operation is
// still underway, the function blocks until the prior operation is complete.
// Then, the erase operation is started and the function returns.
//
// Use EZBL_SectorIsClear() after EZBL_EraseProgDone() returns non-zero to 
// determine if the erase cleared all contents or failed for some reason. This 
// isn't necessary though if you are just going to reprogram the sector anyway. 
// The programming routines will not allow programming of a sector that has data
// still on it (other than an exact match of the data you are asking to write).
signed int EZBL_StartEraseMin(unsigned long address)
{
    unsigned int tblpagSave;

    // Disallow unimplemented addresses to be accessed
    // Attempting to access beyond the last legal address causes an
    // Address Error Trap.
    if(address >= EZBL_SYM32(EZBL_MAIN_FLASH_END_ADDRESS))
    {
        return -1;
    }

    // Do not allow erasing of our own bootloader self, but do allow journal
    // space to be erased if this function is called directly (can't do it from
    // Erase())
    if(EZBL_ROMAddressInBootloader(address) && !EZBL_ROMAddressInJournal(address))
    {
        return -2;
    }

    tblpagSave = TBLPAG;        // Save TBLPAG
    while(NVMCON & NVMCON_WR);  // Block until any prior Erase or Program operation is complete
    TBLPAG = BYTE2(address);    // Initialize PM Page Boundary SFR
    if(&NVMADR == NULL)
    {
        // Set base address of erase block with dummy latch write
        __builtin_tblwtl((unsigned int)address, 0xFFFF);
    }
    else
    {
        // This device has NVMADRU:NVMADR registers, set address by
        // explicitly loading them
        NVMADR = (unsigned int)address;
        NVMADRU = TBLPAG;
    }

    // Disable interrupts
    // Erasing sector 0 must be done with interrupts off. Otherwise, an interrupt would trigger an instant bricked device (no IVT or reset vector).
    // Initialize NVMCON for page erase and set the write enable. The actual value has to come from the linker script because there are multiple op code varitions and encodings between device families (and possibly even within device families).
    EZBL_NVMKey += 0x03DF;
    EZBL_WriteNVMCON(EZBL_SYM(EZBL_NVMOP_PAGE_ERASE), 0x03E5, 1);  // Sets WR and block until done
    
    TBLPAG = tblpagSave; // Restore TBLPAG

    return 1; // Successfully started the erase and no impossible regions encountered.
}

// EZBL_ProgMin() is for the old multistore bootloading method. Do not call this 
// function in new bootloader code. Use the EZBL_WriteROM(), WriteROMOnlyOnce() 
// and/or WriteROMChecked() functions instead. 
// 
// Programs EZBL_SYM(EZBL_MIN_PROGRAM_BYTES) worth of data to a properly aligned program address
signed int EZBL_ProgMin(unsigned long address, const void *data)
{
    unsigned int tblpagSave;
    unsigned int dataL, dataH;
    unsigned long readData, writeData, cumulativeData;
    unsigned int pgmAddress;
    unsigned int i;
    unsigned char *dataPtr;


    // Don't try to program an address that doesn't exist. Attempting to simply read from one would cause an address exception.
    if(address >= EZBL_SYM32(EZBL_MAIN_FLASH_END_ADDRESS))
    {
        return -1;
    }

    // Addresses must be aligned to the minimum program boundary
    if(((unsigned int)address) & (EZBL_SYM(EZBL_MIN_PROGRAM_ADDRESSES) - 1u))
    {
        return -2;
    }

    // Prohibit programming any addresses within the bootloader static region,
    // but do allow if it is in the journal since we have to write there too.
    if(EZBL_ROMAddressInBootloader(address) && !EZBL_ROMAddressInJournal(address))
    {
        return -3;
    }


    dataPtr = (unsigned char*)data; // Set up read access pointer
    BYTE1(dataH) = 0x00;            // Upper byte of 24-bit instruction word is always 0x00, so set it once here
    cumulativeData = 0x00FFFFFF;    // Set up latching low value to check if we actually need to write anything
    tblpagSave = TBLPAG;            // Save TBLPAG

    // Check programming algorithm required
    if(&NVMADR == NULL)
    {
        // Write data to latches at the destination address
        TBLPAG = BYTE2(address);
        pgmAddress = (unsigned int)address;
    }
    else
    {
        // Write data to latches at fixed latch address
        NVMADRU = BYTE2(address);
        NVMADR = (unsigned int)address;
        TBLPAG = 0xFA;
        pgmAddress = 0x0000;
    }


    // Write the requisite number of instruction words to the holding latch(es)
    for(i = 0; i < EZBL_SYM(EZBL_MIN_PROGRAM_INSTRUCTIONS); i++)
    {
        // Read in data to write
        BYTE0(writeData) = *dataPtr++;
        BYTE1(writeData) = *dataPtr++;
        BYTE2(writeData) = *dataPtr++;
        BYTE3(writeData) = 0x00;

        // Read what is already in this Flash location. Generally there should be nothing! (reads back as 0x00FFFFFF)
        readData = EZBL_ReadFlash(address);

        //// Error out if the destination contains 0 bits that we can't possibly write as 1
        //if((~readData & writeData) != 0x00000000u)
        // Error out if the destination isn't erased or exactly match what we were going to write
        if((readData != 0x00FFFFFFu) && (readData != writeData))
        {
            TBLPAG = tblpagSave; // Restore TBLPAG
            return -4;
        }

        // Only try to program bits to 0 that are currently 1. Re-clearing an
        // already cleared bit could degrade the Flash cells.
        writeData ^= ~readData;
        cumulativeData &= writeData;
        dataL = (unsigned int)writeData;
        BYTE0(dataH) = BYTE2(writeData);
        __builtin_tblwtl(pgmAddress, dataL);
        __builtin_tblwth(pgmAddress, dataH);
        pgmAddress += 2;
        address += 2u;
    }

    // Issue the Flash write command, but only if there is actually something that needs writing. No sense in wasting time/Flash wear if the data is already present in the Flash.
    if(cumulativeData != 0x00FFFFFFu)
    {
        // Write data
        EZBL_NVMKey += 0x03DF;
        EZBL_WriteNVMCON(EZBL_SYM(EZBL_NVMOP_MIN_PROGRAM), 0x3E5, 1);  // Sets WR and block until done
        NVMCON = 0; // Clear WREN (write enable), ERASE, and NVMOP bits for code safety
    }

    TBLPAG = tblpagSave; // Restore TBLPAG


    // Verify read data matches what we programmed. If it doesn't, either the NVM write didn't occur (ex: IPL 7 ISR took over and canceled the NVM unlock sequence), or the Flash is in bad shape (ex: exceeded max endurance).
    BYTE3(writeData) = 0x00;
    address -= EZBL_SYM(EZBL_MIN_PROGRAM_ADDRESSES);
    dataPtr = (unsigned char*)data;
    for(i = 0; i < EZBL_SYM(EZBL_MIN_PROGRAM_INSTRUCTIONS); i++)
    {
        readData = EZBL_ReadFlash(address);
        BYTE0(writeData) = *dataPtr++;
        BYTE1(writeData) = *dataPtr++;
        BYTE2(writeData) = *dataPtr++;
        if(readData != writeData)
        {
            return -5;
        }
        address += 2u;
    }

    // Successfully written and verified
    return 1;
}

// EZBL_PumpFlashIntoFIFO() is for the old multistore bootloading method. Do not 
// call this function in new bootloader code.
//
// Copies byteLength bytes of Flash memory contents at the specified 
// sourceAddres + byte offset into the dest FIFO.
void EZBL_PumpFlashIntoFIFO(unsigned long sourceAddress, unsigned int sourceByteOffset, unsigned int byteLength, EZBL_FIFO_TYPES dest)
{
    unsigned char buffer[8 * 3]; // Must be a multiple of 3 bytes
    unsigned int chunkSize;

    chunkSize = sizeof(buffer);
    while(byteLength)
    {
        // Check for final fractional chunk
        if(chunkSize > byteLength)
        {
            chunkSize = byteLength;
        }

        // Read data into RAM
        EZBL_ReadPackedFlash(buffer, sourceAddress, sourceByteOffset, chunkSize);

        // Write data to destination FIFO
        switch(dest)
        {
            case COM:
                EZBL_COM_FIFO_Write(buffer, chunkSize);
                break;
            case PGM:
                EZBL_PGM_FIFO_Write(buffer, chunkSize);
                break;
            case BLOB:
                EZBL_BLOB_FIFO_Write(buffer, chunkSize);
                break;
            case SHA1:
                EZBL_SHA1_FIFO_Write(buffer, chunkSize);
                break;
            default: // Illegal parameter specified
                __builtin_software_breakpoint();
                break;
        }

        // Increment address and decrement length remaining
        sourceAddress += sizeof(buffer) / 3 * 2; // chunkSize stays equal to sizeof(buffer) until the very last chunk where the sourceAddress is no longer used, so we can always use the more efficient compile time computation of sizeof(buffer)/3*2 instead of a run time computation of chunkSize/3*2.
        byteLength -= chunkSize;
    }
}


// Called automatically whenever EZBL_SYM(EZBL_MIN_PROGRAM_BYTES) are in the 
// EZBL_PGM_FIFO. Do not call this function directly.
unsigned int EZBL_PGM_FIFO_OnData(void *data, unsigned int dataLength)
{
    unsigned long addressL, addressH;
    unsigned int bitMaskIndex;
    unsigned int bitMaskOffset;

    // Do not allow programming if we are programming an inactive store and ran out of freespace last time such that we couldn't set the next address
    if((EZBL_PGM_FIFO_vars.opFlags & (EZBL_PGM_TARGET_INACTIVE_STORE | EZBL_PGM_NO_FREE_SPACE)) == (EZBL_PGM_TARGET_INACTIVE_STORE | EZBL_PGM_NO_FREE_SPACE))
        return dataLength;

    // Programming Configuration Fuses or other data outside the main Flash array isn't supported yet, so skip writing or including it in the sector bitmap. Fuses can be at addresses like 0xFFF80006, so would break our sector bitmap computations
    addressH = EZBL_SYM32(EZBL_MAIN_FLASH_END_ADDRESS);
    if(EZBL_PGM_FIFO_vars.destAddress < addressH)
    {
        // Set the appropriate sector bitmap flag if requested
        if(EZBL_PGM_FIFO_vars.opFlags & EZBL_PGM_MAP_SECTORS)
        {
            bitMaskIndex = EZBL_PGM_FIFO_vars.destAddress >> EZBL_SYM(EZBL_ADDRESS_TO_SECTOR_MAP_SHIFT);
            bitMaskOffset = bitMaskIndex & 0x7;
            bitMaskIndex >>= 3;
            EZBL_sectorBitmapRAM2[bitMaskIndex] |= (1 << bitMaskOffset);
        }

        // Try to write the data if a target is specified
        if((EZBL_PGM_FIFO_vars.opFlags & EZBL_PGM_TARGET_ANY) == EZBL_PGM_TARGET_ANY)
        {// Target is anywhere, just pass data though
            if(EZBL_ProgMin(EZBL_PGM_FIFO_vars.destAddress, data) < 0)
            {
                EZBL_PGM_FIFO_vars.opFlags |= EZBL_PGM_ERROR_WRITING;
            }
        }
        else if(EZBL_PGM_FIFO_vars.opFlags & EZBL_PGM_TARGET_JOURNAL)
        {// Target is journal
            // Validate destination is only within the journal addresses
            addressL = EZBL_SYM32(EZBL_JOURNAL_BASE);
            addressH = EZBL_SYM32(EZBL_JOURNAL_END);
            if((EZBL_PGM_FIFO_vars.destAddress >= addressL) && (EZBL_PGM_FIFO_vars.destAddress + EZBL_SYM(EZBL_MIN_PROGRAM_ADDRESSES) <= addressH))
            {
                if(EZBL_ProgMin(EZBL_PGM_FIFO_vars.destAddress, data) < 0)
                {
                    EZBL_PGM_FIFO_vars.opFlags |= EZBL_PGM_ERROR_WRITING;
                }
            }
        }
        else if(EZBL_PGM_FIFO_vars.opFlags & (EZBL_PGM_TARGET_INACTIVE_STORE | EZBL_PGM_TARGET_STORE0))
        {
            // Validate destination is only within the app space (no bootloader or journal)
            if(!EZBL_ROMAddressInBootloader(EZBL_PGM_FIFO_vars.destAddress) &&
                    !EZBL_ROMAddressInBootloader(EZBL_PGM_FIFO_vars.destAddress + EZBL_SYM(EZBL_MIN_PROGRAM_ADDRESSES)))            {
                if(EZBL_ProgMin(EZBL_PGM_FIFO_vars.destAddress, data) < 0)
                {
                    EZBL_PGM_FIFO_vars.opFlags |= EZBL_PGM_ERROR_WRITING;
                }
            }
        }
        else if(EZBL_PGM_FIFO_vars.opFlags & EZBL_PGM_TARGET_RESET_VECTOR)
        {// Target address 0x000000-0x000004 only for a GOTO instruction write
            // Validate destination is only within the app space (no bootloader or journal)
            if(EZBL_PGM_FIFO_vars.destAddress <= 0x000004u)
            {
                if(EZBL_ProgMin(EZBL_PGM_FIFO_vars.destAddress, data) < 0)
                {
                    EZBL_PGM_FIFO_vars.opFlags |= EZBL_PGM_ERROR_WRITING;
                }
            }
        }
        else if(EZBL_PGM_FIFO_vars.opFlags & EZBL_PGM_TARGET_FUSES)
        {// Target fuses (not implemented in this multi-image bootload style)
            EZBL_PGM_FIFO_vars.opFlags |= EZBL_PGM_ERROR_WRITING;
        }
    }

    // Increment destination pointer (even if we didn't write anything)
    EZBL_PGM_FIFO_vars.destAddress += EZBL_SYM(EZBL_MIN_PROGRAM_ADDRESSES);

    // Process inactive store specific address increment rules
    if(EZBL_PGM_FIFO_vars.opFlags & EZBL_PGM_TARGET_INACTIVE_STORE)
    {
        // Check for address increment into the next sector, in such a case, we need to find a new one to write to next
        if((EZBL_PGM_FIFO_vars.destAddress & ~EZBL_SYM32(EZBL_SECTOR_ADDRESS_MASK)) == 0ul)
        {
            // Try to find a new sector to write to
            EZBL_PGM_FIFO_vars.destAddress = EZBL_JournalRead(JOURNAL_FREE_SECTOR_WITH_RESERVATION, 0, NULL);
            if(((signed long)EZBL_PGM_FIFO_vars.destAddress) < 0)
            {
                EZBL_PGM_FIFO_vars.opFlags |= EZBL_PGM_NO_FREE_SPACE; // Signal no more space
                EZBL_COM_FIFO_SetOnDataThreshold(0); // Abort receiving any more data
                EZBL_COM_FIFO_vars.opFlags = EZBL_COM_FAIL;
            }
        }
    }

    return dataLength;
}

// EZBL_PGM_FIFO_Flush() is for the old multistore bootloading method. Do not 
// use this function in new bootloader code.
// 
// Simple function for writing 0xFF padding until the data in the FIFO reaches 
// the minimum and writes itself via the OnData callback.
void EZBL_PGM_FIFO_Flush(void)
{
    while(EZBL_PGM_FIFO_ReadableLength() != 0u)
    {
        EZBL_PGM_FIFO_Write8(0xFF);
    }
}


// Called automatically whenever a specific threshold defined by a 
// EZBL_COM_FIFO_SetOnDataThreshold() call worth of bytes are written to the COM 
// FIFO. Do not call this function directly.
unsigned int EZBL_COM_FIFO_OnData(void *data, unsigned int dataLength)
{
    // Check if a new write operation. In such a case, capture the needed blob length.
    if(EZBL_COM_FIFO_vars.opFlags & EZBL_COM_WRITE_START)
    {
        // This condition always starts with a 32-bit blob length (i.e. dataLength == 4)
        BYTE0(EZBL_COM_FIFO_vars.length) = ((unsigned char*)data)[0];
        BYTE1(EZBL_COM_FIFO_vars.length) = ((unsigned char*)data)[1];
        BYTE2(EZBL_COM_FIFO_vars.length) = ((unsigned char*)data)[2];
        BYTE3(EZBL_COM_FIFO_vars.length) = ((unsigned char*)data)[3];

        // Set up Hashing FIFO
        EZBL_SHA1_FIFO_Reset();
        EZBL_SHA1_FIFO_vars.length = 0;
        EZBL_SHA1_FIFO_vars.opFlags = EZBL_SHA1_HASH | EZBL_SHA1_ZERO_BLOBS_HASH;

        // Increase data threshold for greater performance now that we don't need an exact 4 byte window
        EZBL_COM_FIFO_SetOnDataThreshold(sizeof(EZBL_COM_FIFO_vars.fifoRAM));

        // Clear flag now that we know the transaction (blob) length
        EZBL_COM_FIFO_vars.opFlags ^= EZBL_COM_WRITE_START;
    }

    // Don't underflow
    if(EZBL_COM_FIFO_vars.length < dataLength)
    {
        dataLength = (unsigned int)EZBL_COM_FIFO_vars.length;
        EZBL_COM_FIFO_SetOnDataThreshold(0); // Stop further callbacks
    }

    // Pass data to the PGM FIFO for writing and SHA1 FIFO for hashing
    EZBL_PGM_FIFO_Write(data, dataLength);
    EZBL_SHA1_FIFO_Write(data, dataLength);

    // Compute remaining bytes now to completion
    EZBL_COM_FIFO_vars.length -= dataLength;


    // Flush anything left in the PGM fifo if we are done
    if(EZBL_COM_FIFO_vars.length == 0u)
    {
        EZBL_PGM_FIFO_Flush();
        EZBL_COM_FIFO_vars.opFlags ^= EZBL_COM_WRITING;
        EZBL_SHA1_FIFO_Flush();
    }
    else if(EZBL_COM_FIFO_vars.length < sizeof(EZBL_COM_FIFO_vars.fifoRAM))
    {
        // If we are down to the last chunk of data, set the callback threshold so we receive that last residual
        EZBL_COM_FIFO_SetOnDataThreshold((unsigned int)EZBL_COM_FIFO_vars.length);
    }

    // Signal consumption of data from the FIFO
    return dataLength;
}

// Called automatically whenever threshold defined by a 
// EZBL_BLOB_FIFO_SetOnDataThreshold() call is reached. Do not call this 
// function directly.
unsigned int EZBL_BLOB_FIFO_OnData(void *data, unsigned int dataLength)
{
    unsigned char *dataChars = (unsigned char*)data;

    // Abort all work if there was a processing or writing error
    if(EZBL_BLOB_FIFO_vars.opFlags & EZBL_BLOB_FAIL)
        return dataLength;

    // If the blob is finished being processed just ignore all other data
    if(EZBL_BLOB_FIFO_vars.opFlags & EZBL_BLOB_DONE)
        return dataLength;

    // Determine the "direction" of the data processing to do
    if(EZBL_BLOB_FIFO_vars.opFlags & EZBL_BLOB_INCOMING) // Blob comming in, optionally needs decryption, decompression, decoding, and writing to a store (store0 or inactive store1+)
    {
        // Get Blob Length
        switch(EZBL_BLOB_FIFO_vars.state)
        {
            case 0: // Get Blob Length and Blob Type Field (8 bytes)
                dataLength = 8; // Signal that we are only using 8 bytes

                // Get bytes 0-3, which is the total blob length
                BYTE0(EZBL_BLOB_FIFO_vars.length) = *dataChars++;
                BYTE1(EZBL_BLOB_FIFO_vars.length) = *dataChars++;
                BYTE2(EZBL_BLOB_FIFO_vars.length) = *dataChars++;
                BYTE3(EZBL_BLOB_FIFO_vars.length) = *dataChars++;
                EZBL_BLOB_FIFO_vars.lengthRemaining = EZBL_BLOB_FIFO_vars.length;

                // If hash requested, initialize the hashing FIFO
                if(EZBL_BLOB_FIFO_vars.opFlags & EZBL_BLOB_HASH)
                {
                    EZBL_SHA1_FIFO_Reset();
                    EZBL_SHA1_FIFO_vars.length = 0;
                    EZBL_SHA1_FIFO_vars.opFlags = EZBL_SHA1_HASH | EZBL_SHA1_ZERO_BLOBS_HASH;
                }

                // Skip reserved bytes 4-7 "BLOB" Blob Type Field and go to SHA-1 hash state (20 bytes)
                EZBL_BLOB_FIFO_vars.state = 1;

                // Configure the EZBL_PGM_FIFO for programming (even if we aren't actually going to use it, we still need it's destination address tracking
                EZBL_PGM_FIFO_Reset();
                EZBL_PGM_FIFO_vars.opFlags = EZBL_PGM_MAP_SECTORS;
                if(EZBL_BLOB_FIFO_vars.opFlags & EZBL_BLOB_WRITE)
                {
                    EZBL_PGM_FIFO_vars.opFlags |= EZBL_PGM_TARGET_STORE0;
                }

                break;

            case 1: // Get SHA-1 hash (20 bytes)
                dataLength = 20; // Signal that we are using all 20 bytes
                // Do nothing with the hash at this time
                EZBL_BLOB_FIFO_vars.state = 2;
                break;

            case 2: // Get Record Length and Record Address (8 bytes)
                dataLength = 8; // Signal that we are only using 8 bytes
                BYTE0(EZBL_BLOB_FIFO_vars.subCounter) = *dataChars++;
                BYTE1(EZBL_BLOB_FIFO_vars.subCounter) = *dataChars++;
                BYTE2(EZBL_BLOB_FIFO_vars.subCounter) = *dataChars++;
                BYTE3(EZBL_BLOB_FIFO_vars.subCounter) = *dataChars++;
                BYTE0(EZBL_PGM_FIFO_vars.destAddress) = *dataChars++;
                BYTE1(EZBL_PGM_FIFO_vars.destAddress) = *dataChars++;
                BYTE2(EZBL_PGM_FIFO_vars.destAddress) = *dataChars++;
                BYTE3(EZBL_PGM_FIFO_vars.destAddress) = *dataChars++;
                EZBL_BLOB_FIFO_vars.subCounter -= 8; // Subtract off the size of this record header to get the record byte count

                // Only advance to next state if this record contains data
                if(EZBL_BLOB_FIFO_vars.subCounter != 0u)
                {
                    EZBL_BLOB_FIFO_vars.state = 3;
                }
                break;

            case 3: // Get Record Data
                // Only use data up to the end of the data record
                if(EZBL_BLOB_FIFO_vars.subCounter < dataLength)
                {
                    dataLength = EZBL_BLOB_FIFO_vars.subCounter;
                }

                // Program this chunk of data (may just increment address if null write target flag is set)
                EZBL_PGM_FIFO_Write(data, dataLength);

                EZBL_BLOB_FIFO_vars.subCounter -= dataLength;

                // Check if we are out of record data and need to go process a new record header
                if(EZBL_BLOB_FIFO_vars.subCounter == 0u)
                {
                    // Flush any data in the program FIFO if it isn't aligned to a perfect min programming block size
                    EZBL_PGM_FIFO_Flush();
                    EZBL_BLOB_FIFO_vars.state = 2; // Go Back to Get Record Length
                }
                break;

            default:
                break;
        }

        // Hash the data if requested
        if(EZBL_BLOB_FIFO_vars.opFlags & EZBL_BLOB_HASH)
        {
            EZBL_SHA1_FIFO_Write(data, dataLength);
        }


        // Decrement total blob length and check for completion
        EZBL_BLOB_FIFO_vars.lengthRemaining -= dataLength;
        if(EZBL_BLOB_FIFO_vars.lengthRemaining == 0u)
        {
            EZBL_BLOB_FIFO_vars.state = 0xFFFF; // Done/Idle
            EZBL_BLOB_FIFO_vars.opFlags |= EZBL_BLOB_DONE;

            if(EZBL_BLOB_FIFO_vars.opFlags & EZBL_BLOB_HASH)
            {
                EZBL_SHA1_FIFO_Flush();
            }
        }
        else if(EZBL_BLOB_FIFO_vars.lengthRemaining < EZBL_BLOB_FIFO_vars.dataThreshold)
        {
            // Make sure we get to process the final residual that's smaller that this FIFO size
            EZBL_BLOB_FIFO_SetOnDataThreshold((unsigned int)EZBL_BLOB_FIFO_vars.lengthRemaining);
        }
    }
    else // store0 type data coming in, optionally needs encoding, compression, encryption, and writing to an inactive store1+
    {

    }

    return dataLength;
}