/* 
 * File:   ezbl_multistore.h
 * Author: C12128
 *
 * Created on April 1, 2014, 10:00 AM
 */

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

#ifndef EZBL_MULTISTORE_H
#define	EZBL_MULTISTORE_H

// FIFO functions (defined in ezbl_multistore.c via #include of "fifo.c.h")
void EZBL_BLOB_FIFO_Reset(void);                                                // Prototype for old programming code; may be removed in the future.
void EZBL_BLOB_FIFO_Write8(unsigned char writeData);                            // Prototype for old programming code; may be removed in the future.
void EZBL_BLOB_FIFO_Write16(unsigned short writeData);                          // Prototype for old programming code; may be removed in the future.
void EZBL_BLOB_FIFO_WriteUINT(unsigned int writeData);                          // Prototype for old programming code; may be removed in the future.
void EZBL_BLOB_FIFO_Write24(unsigned long writeData);                           // Prototype for old programming code; may be removed in the future.
void EZBL_BLOB_FIFO_Write32(unsigned long writeData);                           // Prototype for old programming code; may be removed in the future.
void EZBL_BLOB_FIFO_Write64(unsigned long long writeData);                      // Prototype for old programming code; may be removed in the future.
unsigned int EZBL_BLOB_FIFO_Write(void *data, unsigned int length);             // Prototype for old programming code; may be removed in the future.
void EZBL_PGM_FIFO_Reset(void);                                                 // Prototype for old programming code; may be removed in the future.
void EZBL_PGM_FIFO_Write8(unsigned char writeData);                             // Prototype for old programming code; may be removed in the future.
void EZBL_PGM_FIFO_Write16(unsigned short writeData);                           // Prototype for old programming code; may be removed in the future.
void EZBL_PGM_FIFO_WriteUINT(unsigned int writeData);                           // Prototype for old programming code; may be removed in the future.
void EZBL_PGM_FIFO_Write24(unsigned long writeData);                            // Prototype for old programming code; may be removed in the future.
void EZBL_PGM_FIFO_Write32(unsigned long writeData);                            // Prototype for old programming code; may be removed in the future.
void EZBL_PGM_FIFO_Write64(unsigned long long writeData);                       // Prototype for old programming code; may be removed in the future.
unsigned int EZBL_PGM_FIFO_Write(void *data, unsigned int length);              // Prototype for old programming code; may be removed in the future.
void EZBL_PGM_FIFO_Flush(void);                                                 // Prototype for old programming code; may be removed in the future.


// JournalRead() journalTopic constants
#define JOURNAL_UNUSED_STOREID					0u	// An unused storeID for creating a new store
#define JOURNAL_STORE_COUNT     				1u	// Count of how many stores exist
#define JOURNAL_ENUM_STOREIDS					2u	// Given an existant storeID, find the next storeID that exists. Results are returned in descending creation order (i.e. most recently written store is returned first, oldest store returned last). The ID numbers themsevles will normally be unordered due to store erasures. Start with 0xFFFF to get the most recently written storeID.
#define JOURNAL_FREE_SECTOR						4u	// Find the first unused sector, starting from EZBL_LAST_SAFE_SECTOR_ADDRESS and working
#define JOURNAL_FREE_SECTOR_WITH_RESERVATION    5u  // Find the first unused sector and mark it as reserved in EZBL_sectorBitmapRAM[]
#define JOURNAL_STORE_SECTOR_BITMAP				6u	// Return bitmap of all the sectors used by a given store
#define JOURNAL_STORE_FLAGS 					7u	// Options flags for the given store
#define JOURNAL_STORE_HASH                      8u	// Returns the store's stored hash in EZBL_SHA1_FIFO_vars.fifoRAM (first 20 bytes)


// EZBL_REC_HEADER.recordType constants
#define REC_TYPE_INVALIDATE      0x01u   // Invalidation record, invalidates all prior records in the journal with the same storeID and recordType
#define REC_TYPE_DEFINE_STORE    0x02u   // Defines a store according to the EZBL_REC_HEADER structure
#define REC_TYPE_END_OF_JOURNAL  0xFFu
#define REC_TYPE_UNINITIAILIZED  0xBBu

// Structures representing journal records in Flash. Every record starts with
// this 3 byte field, followed by a variable length structure specific to the
// decoded recordType and then a footer of 0xFF padding to fill any remaining
// bytes out to an integer multiple of the minimum write block size,
// SYM(EZBL_MIN_PROGRAM_BYTES).
typedef struct __attribute__((packed))
{
    unsigned int storeID;
    unsigned char recordType;
}
EZBL_REC_HEADER;

#define REC_OFFSET_HEADER_STOREID       0u
#define REC_OFFSET_HEADER_RECORD_TYPE   (REC_OFFSET_HEADER_STOREID + 2u)

// Invalidate all prior records matching the same storeID and having a
// recordType that matches this structure's priorRecordType field. Future
// records with the storeID and priorRecordType are not invalidated by this, but
// they may also be followed by more invalidation records.
typedef struct __attribute__((packed))
{
    unsigned char priorRecordType; // The recordType this invalidation record applies to.
    unsigned long checksum; // 32-bits of hash data from the beginning of the Journal to this address (but not including this).
}
EZBL_REC_INVALIDATE;

#define REC_OFFSET_INVALIDATE_PRIOR_RECORD_TYPE     sizeof(EZBL_REC_HEADER)
#define REC_OFFSET_INVALIDATE_PRIOR_RECORD_CHECKSUM (REC_OFFSET_INVALIDATE_PRIOR_RECORD_TYPE + 1u)

// Define (or redefine) a store's hash and sector bitmap
typedef struct __attribute__((packed))
{
    unsigned char storeFlags[4];    // Not defined yet, set to 0xFFFFFFFF
    unsigned char hash[20];         // 20 byte SHA-1 hash of the store
    unsigned char sectorBitmap[0];  // Length varies by device. Each bit represents one Flash sector. To get the needed size, in bytes, use EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP).
    unsigned long checksum;         // 32-bits of hash data from the beginning of the Journal to this address (but not including this).
}
EZBL_REC_DEFINE_STORE;

#define REC_OFFSET_DEFINE_STORE_STORE_FLAGS     sizeof(EZBL_REC_HEADER)
#define REC_OFFSET_DEFINE_STORE_HASH            (REC_OFFSET_DEFINE_STORE_STORE_FLAGS + 4u)
#define REC_OFFSET_DEFINE_STORE_SECTOR_BITMAP   (REC_OFFSET_DEFINE_STORE_HASH + 20u)
#define REC_OFFSET_DEFINE_STORE_CHECKSUM        (REC_OFFSET_DEFINE_STORE_SECTOR_BITMAP + EZBL_SYM(EZBL_BYTES_PER_SECTOR_MAP))


signed int EZBL_ROMAddressInJournal(unsigned long address);
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

signed int EZBL_StartEraseMin(unsigned long address);
signed int EZBL_Erase(unsigned long lowAddress, unsigned long highAddress);
void EZBL_PumpFlashIntoFIFO(unsigned long sourceAddress, unsigned int sourceByteOffset, unsigned int byteLength, EZBL_FIFO_TYPES dest);


#endif	/* EZBL_MULTISTORE_H */

