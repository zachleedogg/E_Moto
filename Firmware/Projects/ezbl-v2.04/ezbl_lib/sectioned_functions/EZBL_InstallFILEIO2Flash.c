
/*******************************************************************************
  Easy Bootloader for PIC24 and dsPIC33 Library API Source File

  Summary:
    Installs a .bl2 Application image file to the local flash memory from an
    external FILEIO memory (ex: USB host Mass Storage Device class thumb drive
    or SD Card).

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

#include <string.h>
#include <stdint.h>
#include "../ezbl.h"


extern volatile unsigned int __attribute__((near)) IPC0;
extern volatile unsigned int __attribute__((near)) IEC0;
extern const int __attribute__((weak)) EZBL_CryptKey[];

int __attribute__((weak)) EZBL_PreInstall(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer);


typedef enum
{
    FILEIO_SEEK_SET = 0,                // Change the position in the file to an offset relative to the beginning of the file.
    FILEIO_SEEK_CUR,                    // Change the position in the file to an offset relative to the current location in the file.
    FILEIO_SEEK_END                     // Change the position in the file to an offset relative to the end of the file.
} FILEIO_SEEK_BASE;

size_t FILEIO_Read (void * buffer, size_t size, size_t count, void *filePtr);   // See FILEIO library (fileio.h). This is compatible, but not the real prototype.
int FILEIO_Seek (void * handle, int32_t offset, FILEIO_SEEK_BASE base);         // See FILEIO library (fileio.h). This is compatible, but not the real prototype.



//#define EZBL_DEBUG     // Uncomment to allow debugging messages to be printed to stdout
//#define VERBOSE        // Uncomment to have verbose printing of all flash write commands and data

#if defined(EZBL_DEBUG)
void EZBL_DumpFlashDiff(const char *refMsgPreFix, const char *cmpMsgPreFix, unsigned long addr, const void *dataRef, const void *dataCmp, unsigned int byteLen);
#else
#define EZBL_printf(...)        do{}while(0)
#define EZBL_DumpFlashDiff(...) do{}while(0)
#endif



/**
 * Reads a .bl2 file from the specified FILEIO communications handle, checks 
 * that the offering has a matching BOOTID_HASH, optionally checks that the 
 * APPID_VER field is acceptable, reads the whole file contents to verify a 
 * CRC32 and ensure proper structure, seeks back to the beginning of the .bl2 
 * file, erases all App-space flash pages, reprocesses the .bl2 file programming
 * and verifying the flash writes, reverifies the overall file CRC32 and FILEIO
 * communications integrity, and programs a flag indicating the new Application
 * is valid and available for execution.
 *
 * This function is blocking and will return only upon firmware rejection, 
 * bootload completion, or if an error occurs.
 * 
 * If this function is invoked while an existing Application is executing and 
 * the observed .bl2 image matches the installation requirements, then all
 * non-bootloader ISRs are disabled, the existing Application is erased, and 
 * this function will not return. Instead, at bootload termination (success or
 * failure), a device reset is done to reenter the bootloader with a clean stack
 * and device state to allow new Application launch (or bootload retry if
 * failure occurred).
 *
 * Because reception, decryption erase and programming of a whole .bl2 file
 * could take an extended period, this function internally executes the
 * CLRWDT opcode before NVM erase/programming operations. As the operations
 * generally do not have deterministic timing, you must NOT enable the watchdog
 * timer in a Windowed mode.
 *
 * @param *bl2FILEIOHandle FILEIO_OBJECT pointer opened externally via the
 *        FILEIO_Open() API. The file position pointer should point to the
 *        Start Of File (byte 0) of the .bl2 file that you wish to install.
 *
 * @param commit Set to 1 to enable flash erase/write and verification
 *        operations to take place for a normal bootload procedure.
 *
 *        0 and all other commit values will internally suppress all flash
 *        erase/write operations and blank expected read-back verification
 *        mismatches to allow complete file processing in a "simulation" mode.
 *        Use this option to pre-check the complete image availability,
 *        integrity, and data record target address sensibility to gain a "warm
 *        fuzzy feeling" before repeating the EZBL_InstallFILEIO2Flash() call
 *        with non-volatile consequences.
 *
 * @param BOOTID_HASH0, BOOTID_HASH1, BOOTID_HASH2, BOOTID_HASH3 project
 *        symbols. These are generated as a SHA-256 hash (truncated to 16-bytes)
 *        of the concatenated BOOTID_VENDOR, BOOTID_MODEL, BOOTID_NAME and
 *        BOOTID_OTHER strings defined in your ezbl Makefile
 *        (i.e.: 'ezbl_product_id.mk' with proper include of
 *        'ezbl_product_id.mk' in 'Makefile').
 *
 * @param APPID_VER_MAJOR, APPID_VER_MINOR, APPID_VER_BUILD project symbols
 *        defined in the last installed Application's ezbl Makefile (i.e.:
 *        'ezbl_product_id.mk' with proper include of 'ezbl_product_id.mk' in
 *        'Makefile' within the Application project). These get stored in the
 *        EZBL_appBootloadState flash address as part of the Application, so
 *        will not exist if the Application has been erased.
 *
 * @param EZBL_NO_APP_DOWNGRADE optional project symbol. Set to 1 using
 *        EZBL_SetSYM() if you wish to trigger pre-erase/programming abort when
 *        the offered .bl2 image file contains
 *        APPID_VER_MAJOR.APPID_VER_MINOR.APPID_VER_BUILD version meta data that
 *        indicates the firmware is less than the presently executing
 *        application version (as defined by EZBL_appIDVer.appIDVer64).
 *
 * @return Signed integer result code. Generally zero or greater is good, while
 *         negative values indicate a failure. Anything erased/programmed prior
 *         to a failure is left as it was upon failure detection (will
 *         automatically be cleaned up on next bootload attempt).
 *
 *         Specific return code meanings are:
 *         <ul>
 *          <li>EZBL_ERROR_SUCCESS (1): Operation completed successfully. The
 *              new application is ready for execution.</li>
 *          <li>EZBL_ERROR_ALREADY_INSTALLED (3): Offered firmware image already
 *              matches the existing target firmware.</li>
 *          <li>EZBL_ERROR_COM_READ_TIMEOUT (-20): Bootloader communications
 *              timeout attempting to read firmware image.</li>
 *          <li>EZBL_ERROR_IMAGE_MALFORMED (-21): Bootloader rejected firmware
 *              as malformed or of unsupported type. Possible communications
 *              error.</li>
 *          <li>EZBL_ERROR_BOOTID_HASH_MISMATCH (-22): Bootloader rejected
 *              firmware as incompatible (with this Bootloader project).</li>
 *          </li>EZBL_ERROR_APPID_VER_MISMATCH (-23): Bootloader rejected
 *              firmware as out of the required programming order.
 *
 *              This error is only generated when the EZBL_NO_APP_DOWNGRADE
 *              symbol is non-zero and the received
 *              APPID_VER_MAJOR.APPID_VER_MINOR.APPID_VER_BUILD information in
 *              the file is less than the presently executing Application ID
 *              version. If no Application exists, this error will not be
 *              generated.</li>
 *          <li>EZBL_ERROR_HARD_VERIFY_ERROR (-24): Bootloader read-back
 *              verification failure.</li>
 *          <li>EZBL_ERROR_SOFT_VERIFY_ERROR (-25): Bootloader read-back
 *              verification mismatch in reserved address range.</li>
 *          <li>EZBL_ERROR_IMAGE_CRC (-26): Bootloader computed CRC mismatch
 *              with CRC contained in firmware image. Probable communications
 *              error.</li>
 *          <li>No return: Unhandled exception or unknown. (Image file may have
 *              records targeting unimplemented memory, causing an Address Error
 *              Trap upon verification, or electrical operating parameters are
 *              incorrect.) Ensure the image file was compiled specifically
 *              for this device and bootloader. For unhandled exceptions, the
 *              device will automatically reset back into the bootloader.</li>
 *         </ul>
 */
int EZBL_InstallFILEIO2Flash(void *bl2FILEIOHandle, int commit)
{
    union
    {
        unsigned long long  u64[8];
        unsigned long       u32[16];
        unsigned short      u16[32];
        unsigned char        u8[64];
    } buf;
    unsigned long crc32;
    unsigned long bytesToEOF;
    unsigned long nextAddr;
    int bootCode;
    unsigned int i;
    unsigned int j;
    unsigned int chunkSize;
    int ret;                            // Variable for temporary signed return codes
    EZBL_MISMATCH_STATUS verifyStatus;  // Not used, but useful to have available if debugging programming failures (doesn't really cost much since it is on the stack)
    EZBL_INSTALLED_FLAGS installFlags;
    unsigned long extraNoProgRange[2];
    struct
    {
        union
        {
            unsigned long  len;
            unsigned short len16[2];
            unsigned char  len8[4];
        };
        union
        {
            unsigned long  addr;
            unsigned short addr16[2];
            unsigned char  addr8[4];
        };
    } recHdr;

    EZBL_printf("\n  Starting %s(0x%04X, %i);", __func__, (unsigned int)bl2FILEIOHandle, commit);


    extraNoProgRange[0] = EZBL_SYM32(EZBL_appBootloadState);    // Get this value early since used in multiple states and takes a fair amount of code to retrieve

    FILEIO_Seek(bl2FILEIOHandle, 0, FILEIO_SEEK_SET);          // Start of file
    if(FILEIO_Read(&buf, 1, 48, bl2FILEIOHandle) != 48)
    {
        return EZBL_ERROR_COM_READ_TIMEOUT;         // Error (-20): Bootloader communications timeout attempting to read firmware image
    }

    // Locate "BL2B" FILE_ID header (must skip over synchronization bytes, which
    // may not be a fixed 16 bytes in future .bl2 files, but should always be a
    // multiple of 4 bytes and shouldn't be more than 48 bytes)
    for(i = 0; i < 12u; i++)
    {
        if(buf.u32[i] == 0x42324C42ul)  // 0x42324C42 = "BL2B"  ('B'=0x42, 'L'=0x4C, '2'=0x32, 'B'=0x42)
        {
            EZBL_printf("\n  Found BL2B header");
            break;
        }
    }
    if(i >= 12u)
        return EZBL_ERROR_IMAGE_MALFORMED;          // (-21): Bootloader rejected firmware as malformed or of unsupported type. Possible communications error.
    
    // Realign BL2B file data in buffer to index 0
    j = 48u - i*4u;
    EZBL_RAMCopy(&buf, &buf.u32[i], j);
    
    // Get all of the header bytes (48 total, not counting the sync field which we've thrown away)
    if(FILEIO_Read(&buf.u8[j], 1, 48u-j, bl2FILEIOHandle) != 48u-j)
    {
        return EZBL_ERROR_COM_READ_TIMEOUT;                     // Error (-20): Bootloader communications timeout attempting to read firmware image
    }
    crc32 = EZBL_CRC32(0x00000000, &buf, 48);
    bytesToEOF = buf.u32[1];                                    // Need to subtract 16+8+16 bytes for size of BOOTID_HASH, APPID_VER, and HMAC-SHA-256 data fields in the header (done later after potentially using this value)
    bootCode = EZBL_ERROR_SUCCESS;                              // Default return value
    EZBL_printf("\n  BOOID_HASH offered: %016llX%016llX", buf.u64[2], buf.u64[1]);
    EZBL_printf("\n                ours: %016llX%016llX", EZBL_bootIDHash.bootIDHash64[1], EZBL_bootIDHash.bootIDHash64[0]);
    if(EZBL_RAMCompare(&buf.u32[2], &EZBL_bootIDHash, 16) != 0) // Check BOOTID_HASH field matches our Bootloader's bootIDHash
    {
        EZBL_printf("\n  \x1B[31mBOOID_HASH mismatch\x1B[0m");
        return EZBL_ERROR_BOOTID_HASH_MISMATCH;
    }

    // Check that APPID_VER is >= APPID_VER that we are presently running
    EZBL_ReadROMObj(&installFlags, extraNoProgRange[0]);            // Read the EZBL_INSTALLED_FLAGS struct for the current Application
    EZBL_printf("\n  APPID_VER: offered = %u.%02u.%04lu"
                "\n             ours    = %u.%02u.%04lu%s", buf.u16[15], buf.u16[14], buf.u32[6], installFlags.appVer.appIDVerMajor, installFlags.appVer.appIDVerMinor, installFlags.appVer.appIDVerBuild, installFlags.appInstalled == EZBL_APP_INSTALLED ? "" : " (no App present)");
    if(installFlags.appInstalled == EZBL_APP_INSTALLED)             // Check that the structure is valid
    {
        if(EZBL_WeakSYM(EZBL_NO_APP_DOWNGRADE))
        {
            if(buf.u64[3] < installFlags.appVer.appIDVer64)         // Abort if offered firmware is older than existing app firmware
            {
                EZBL_printf("\n  APPID_VER older than already installed (EZBL_NO_APP_DOWNGRADE flag enabled)");
                return EZBL_ERROR_APPID_VER_MISMATCH;
            }
        }
        if(buf.u64[3] == installFlags.appVer.appIDVer64)
        {
            EZBL_printf("\n  APPID_VER matches, so no need to erase and reprogram");
            return EZBL_ERROR_ALREADY_INSTALLED;                    // Abort if offered firmware matches existing installed firmware
        }
    }
    installFlags.appVer.appIDVer64 = buf.u64[3];                    // Save appIDVer for later writing to flash

    // Compute HMAC, if crypto is enabled
    // Compare 16-bytes of HMAC-SHA-256 data, if crypto is enabled
    // Use 16 extra bytes of HMAC-SHA-256 data as NONCE for AES-128 IV
    // If valid, begin AES-128 CTR decryption (actually encryption + XOR)
    if(&EZBL_CryptKey == 0u)
    {   // No crypto enabled, just verify HMAC field is all zero's
        for(j = 0x0000u, i = 16; i < 24u; i++)
        {
            j |= buf.u16[i];
        }
        if(j)
        {   // Crypto appears to be enabled, check HMAC. NOTE: this
            // should be done using a constant-time compare
            // algorithm to avoid attack by sequentially sending
            // HMACs that only increment the first byte until a
            // compare duration is observed increasing.
            return EZBL_ERROR_IMAGE_MALFORMED;
        }
    }

    // If enabled, execute a callback to the Bootloader project requesting if we
    // should proceed with erase/reprogramming of this App offering.
    if(EZBL_PreInstall)
    {
        ret = EZBL_PreInstall(0, 0, bytesToEOF + 64u, &installFlags.appVer);
        if(ret <= 0)
        {
            EZBL_printf("\n  EZBL_PreInstall() returned %i, so aborting update", ret);
            return EZBL_ERROR_IMAGE_REJECTED;
        }
    }

    EZBL_printf("\n  Accepting offered firmware update: size = %lu", bytesToEOF);

    bytesToEOF -= 16u + 8u + 16u;  // Subtract bytes for size of BOOTID_HASH, APPID_VER, and HMAC-SHA-256 data fields in the header

    // Might have been called from a running application, so ensure all
    // interrupts stay in the bootloader. The EZBL_PreInstall() callback should
    // have already disabled all interrupts except bootloader ones, but no harm
    // in ensuring execution doesn't leak out of the bootloader (could be useful
    // when testing with commit == 0).
    EZBL_ForwardBootloaderISR = 0;

    // Everything checks out so far, let's start the one-way trip
    EZBL_NVMKey += (unsigned int)((((0xFC21u + 0x4232u) ^ 0x4C42u) ^ buf.u16[0]) - buf.u16[1]);      // Required unlock value for EZBL_EraseAll()/EZBL_EraseAppSpace() is 0xFC21. Should evaluate as 0x7211 at compile time and with run-time XOR of "BL2B" LSword MINUS MSword, we should re-arrive at 0xFC21. This run-time evaluation creates long term dependency to reduce bricking risk due to VDD contact bounce, ESD, gamma rays, or other phenomina that could result in a jumping of the PC into executing this immediate code without actually getting anything from a FILEIO source.
    EZBL_NVMKey *= commit;                                                      // Suppress actual flash writes for simulation mode by corrupting the NVM unlock value
    EZBL_printf("\n  Erasing");
    EZBL_EraseAll(1);   // Clears the watchdog timer for each page erase
    EZBL_printf("... done");

    // Iterate over all DATA_RECORDs
    while(bytesToEOF > 32u+4u)  // 32-byte SHA-256 FILE_HASH field + 4-byte CRC32 field at EOF
    {
        // Read a DATA_REC_HDR (8-bytes)
        if(FILEIO_Read(&recHdr, 1, sizeof(recHdr), bl2FILEIOHandle) != sizeof(recHdr))
        {
            bootCode = EZBL_ERROR_COM_READ_TIMEOUT;             // Error (-20): Bootloader communications timeout attempting to read firmware image
            goto Terminate;
        }
        EZBL_NVMKey += recHdr.addr16[0] ^ recHdr.len16[0];      // Set up dynamic keying criteria requirements
        crc32 = EZBL_CRC32(crc32, &recHdr, sizeof(recHdr));
        bytesToEOF -= sizeof(recHdr);

        if((recHdr.addr8[3] | recHdr.len8[3] | (recHdr.addr8[0] & 0x1u)) || (bytesToEOF < recHdr.len + 36u)) // Sanity check that both the address and length fit in 24-bits, and that the address is an even number
        {
            bootCode = EZBL_ERROR_IMAGE_MALFORMED;
            goto Terminate;
        }

        extraNoProgRange[1] = extraNoProgRange[0] + EZBL_Div3Mul2(sizeof(EZBL_INSTALLED_FLAGS), 0);
        chunkSize = (sizeof(buf)/6u)*6u;            // Get maximum programming chunk size (this is a compile-time evaluated constant)
        while(recHdr.len)
        {
            if((unsigned long)chunkSize > recHdr.len)   // Saturate to number of bytes left in this data record
                chunkSize = (unsigned int)recHdr.len;

            if(FILEIO_Read(&buf, 1, chunkSize, bl2FILEIOHandle) != chunkSize)
            {
                bootCode = EZBL_ERROR_COM_READ_TIMEOUT; // Error (-20): Bootloader communications timeout attempting to read firmware image
                goto Terminate;
            }
            crc32 = EZBL_CRC32(crc32, &buf, chunkSize);
            bytesToEOF -= chunkSize;

            EZBL_NVMKey += 0x03DF - (recHdr.addr16[0] ^ recHdr.len16[0]);       // Compute unlock key as dynamic run-time evaluated statement
#if defined(VERBOSE)
            EZBL_DumpFlashDiff("  Write: ", 0, recHdr.addr, &buf, 0, chunkSize);
#endif
            EZBL_NVMKey *= commit;                                              // Suppress actual flash writes for simulation mode by corrupting the NVM unlock value
            nextAddr = EZBL_WriteROMEx(recHdr.addr, &buf, chunkSize, EZBL_FLAG_CLRWDT, 1, extraNoProgRange);
            ret = EZBL_VerifyROM(recHdr.addr, &buf, chunkSize, &verifyStatus) * commit;
#if defined(EZBL_DEBUG)
            unsigned char buf2[sizeof(buf)];
            EZBL_ReadPackedFlash(buf2, recHdr.addr, 0, chunkSize);
            if(EZBL_RAMCompare(&buf, buf2, chunkSize) != 0)
            {
                const char *causes[9] =
                    {
                        "unknown; hardware write protect?",
                        "programming restricted",
                        "verify restricted; expected mismatch",
                        "program and verify restricted; expected mismatch",
                        "erase restricted",
                        "program and erase restricted; bootloader mismatch",
                        "verify and erase restricted; expected mismatch",
                        "program, verify and erase restricted",
                        "expected EZBL_appBootloadState mismatch"
                    };
                i = EZBL_IsProgramRestricted(recHdr.addr) | (EZBL_IsVerifyRestricted(recHdr.addr)<<1) | (EZBL_IsEraseRestricted(recHdr.addr)<<2);
                if((recHdr.addr <= extraNoProgRange[0]) && (recHdr.addr + EZBL_Div3Mul2(chunkSize, 0) >= extraNoProgRange[0] + sizeof(installFlags)/3u*2u))
                    i = 8;
                EZBL_printf("\n  MISMATCH (%s)", causes[i]);
                EZBL_DumpFlashDiff("  Write: ", "  Read:  ", recHdr.addr, &buf, buf2, chunkSize);
            }
#endif
            recHdr.addr = nextAddr;
            recHdr.len -= chunkSize;
            if(ret < 0)
            {
                bootCode = EZBL_ERROR_HARD_VERIFY_ERROR;            // App-space data programmed does not match read-back value, even after adjusting for special addresses
                goto Terminate;
            }
            if(ret > 0)
            {
                bootCode = EZBL_ERROR_SOFT_VERIFY_ERROR;            // Bootloader reserved region data changed in image, but all App space data still checks out
            }
            EZBL_NVMKey += recHdr.addr16[0] ^ recHdr.len16[0];      // Set up dynamic keying criteria requirements
        }
    }
    
    // All programming done. Get FILE_HASH (32-bytes) and CRC32 fields (4 bytes)
    if(FILEIO_Read(&buf, 1, 32+4, bl2FILEIOHandle) != 36)
    {
        bootCode = EZBL_ERROR_COM_READ_TIMEOUT;     // Error (-20): Bootloader communications timeout attempting to read firmware image
        goto Terminate;
    }
    crc32 = EZBL_CRC32(crc32, &buf, 32);            // Update CRC to include FILE_HASH (everything now included up to the CRC32 field itself)
    if(buf.u32[8] == crc32)
    {   // All programming successful and .BL2 integrity verified, so program final EZBL_INSTALLED_FLAGS structure to mark the Application as valid/runnable
        installFlags.appInstalled = EZBL_APP_INSTALLED;             // Set 0x12CD flag to indicate valid/runnable
        EZBL_RAMCopy(&(installFlags.id), &buf.u32[7], 8);           // Save last 8 bytes of file: 4 bytes of Hash and CRC32
        EZBL_NVMKey += 0x03DF -  (recHdr.addr16[0] ^ recHdr.len16[0]);
        EZBL_NVMKey *= commit;
        EZBL_WriteROM(extraNoProgRange[0], &installFlags, sizeof(installFlags));
        EZBL_printf("\n  CRC32 good: 0x%08lX", installFlags.id.CRC32);
    }
    else
    {   // CRC failure
        EZBL_printf("\n  \x1B[31mCHECKSUM MISMATCH\x1B[0m"
                    "\n  File:     0x%08lX"
                    "\n  Computed: \x1B[31m0x%08lX\x1B[0m", buf.u32[8], crc32);
        bootCode = EZBL_ERROR_IMAGE_CRC;
    }

Terminate:
    EZBL_NVMKey = 0x0000;
    EZBL_printf("\n  Terminating: code = 0x%04X", bootCode);

    if(EZBL_appIsRunning)   // Can't return if we started this sequence while an existing Application was running (and then we erased it)
    {
#if defined(EZBL_DEBUG)
        EZBL_FIFOFlush(EZBL_STDOUT, NOW_sec);
#endif
        EZBL_ResetCPU();
    }

    return bootCode;
}
