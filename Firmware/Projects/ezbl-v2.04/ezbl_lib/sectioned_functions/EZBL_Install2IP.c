/*******************************************************************************
  Easy Bootloader Library API Source File

  Summary:
    Installs a .bl2 image file to the Inactive Partition of a Dual Partition
    device from an EZBL_FIFO based remote communications interface (ex: UART or
    I2C).

  Description:
    Erases the Inactive Partition, then sets up for flow control (if applicable),
    reads a .bl2 file from the communications medium, programs the file
    contents exclusively to Inactive Partition addresses (0x400000+), and 
    verifies correct Flash read-back contents/valid .bl2 file structure/
    valid CRC32.

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

#if defined(EZBL_LIB)
#include "../ezbl.h"
#else
#include "ezbl_integration/ezbl.h"
#endif


extern const int __attribute__((weak)) EZBL_CryptKey[];

int __attribute__((weak)) EZBL_PreInstall(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer);


//#define EZBL_DEBUG     // Uncomment to allow debugging messages to be printed to stdout
//#define VERBOSE        // Uncomment to have verbose printing of all flash write commands and data

#if !defined(EZBL_DEBUG)
#define EZBL_printf(...)        do{}while(0)
#define EZBL_DumpFlashDiff(...) do{}while(0)
#endif


/**
 * Reads a .bl2 file from the communications medium, checks that the offering
 * has a matching BOOTID_HASH, optionally checks that the APPID_VER field is
 * acceptable, erases the Inactive Partition, programs and read-back verifies
 * the .bl2 data records on the Inactive Partition, verifies overall file and 
 * communications integrity, and optionally programs FBTSEQ on the Inactive 
 * Partition indicating the new Application is valid and available for 
 * execution.
 *
 * This function is non-blocking and will return, even if an active bootload
 * operation is underway. To ensure bootload operations are completely handled,
 * this function should be called periodically anytime bootloader functionality
 * is desired.
 *
 * @param *s Pointer to caller allocated EZBL_INSTALL_CTX structure that must
 *           remain within scope anytime a bootload session occurs. Before
 *           calling EZBL_Install2IP() for the first time, set the following
 *           fields:
 *              s.state = SM_SYNC_INIT;
 *              s.timeout = (application defined NOW timeout value);
 *           For the timeout, specify the timeout in some combination of
 *           NOW_sec and/or NOW_ms, ex: s.timeout = NOW_sec*1u; for a one second
 *           communications idle timeout. Higher latency communications mediums,
 *           such as wireless links, will likely require more than 1 second.
 *
 *           On subsequent calls to EZBL_Install2IP(), pass the *s parameter
 *           back to the function with unchanged structure elements.
 *
 * @param EZBL_ADDRESSES_PER_SECTOR project symbol. This symbol must be defined 
 *        to match the hardware's native Flash Page Erase size, expressed in 
 *        flash addresses. Every 0x2 flash addresses is one 24-bit instruction 
 *        word, or 3 bytes of .bl2 record data. 0x4 flash addresses represents 
 *        one 48-bit flash double word, or 6 bytes of .bl2 record data.
 *          
 *        Most hardware requires 0x400 or 0x800 for this definition (i.e. 1536 
 *        bytes per page or 3072 bytes per page), with the needed value chosen 
 *        from the device data sheet. If this symbol is not set, the default 
 *        value of 0x400 addresses is used, which may or may not be correct for 
 *        your hardware target.
 *
 *        To assign this parameter, within a project source file, write:
 *              EZBL_SetSYM(EZBL_ADDRESSES_PER_SECTOR, 0x400);
 *        or,
 *              EZBL_SetSYM(EZBL_ADDRESSES_PER_SECTOR, 0x800);
 *        This should be placed at global/file level scope with the ezbl.h
 *        header already #include'd.
 *
 * @param BOOTID_HASH0, BOOTID_HASH1, BOOTID_HASH2, BOOTID_HASH3 project
 *        symbols. These are generated as a SHA-256 hash (truncated to 16-bytes)
 *        of the concatenated BOOTID_VENDOR, BOOTID_MODEL, BOOTID_NAME and
 *        BOOTID_OTHER strings defined in your EZBL makefile
 *        (i.e.: 'ezbl_dual_partition.mk', which is included in 'Makefile').
 *
 * @param APPID_VER_MAJOR, APPID_VER_MINOR, APPID_VER_BUILD project symbols
 *        defined in the EZBL makefile (i.e.: 'ezbl_dual_partition.mk', which is
 *        included in 'Makefile').
 * 
 * @param EZBL_AUTO_PROG_FBTSEQ optional project symbol to enable automatic 
 *        reprogramming of the Inactive Partition's FBTSEQ to be one less than 
 *        the presently Active Partition's FBTSEQ, enabling the new firmware to 
 *        go live on next reset or BOOTSWP.
 *
 *        To use this feature, within a project source file, write:
 *              EZBL_KeepSYM(EZBL_AUTO_PROG_FBTSEQ);
 *        This should be placed at global/file level scope.
 *
 *        FBTSEQ is programmed only if the entire programming sequence completes
 *        successfully, is verified to not be corrupted in transit, and reads
 *        back from flash as expected. In other cases, FBTSEQ is left either
 *        unprogrammed or programmed with the value contained in the original
 *        .hex file (.bl2 file).
 *
 *        When using this feature, do not create .bl2 files with the FBTSEQ
 *        Config word defined in it. If defined, it will be programmed as normal
 *        data and then require the EZBL_AUTO_PROG_FBTSEQ function to erase and
 *        reprogram the entire last page on the inactive partition - needlessly
 *        wasting flash endurance.
 * 
 * @param EZBL_NO_APP_DOWNGRADE optional project symbol. Set to 1 using
 *        EZBL_SetSYM() if you wish to trigger pre-erase/programming abort when
 *        the offered .bl2 image file contains
 *        APPID_VER_MAJOR.APPID_VER_MINOR.APPID_VER_BUILD version meta data that
 *        indicates the firmware is less than the presently executing
 *        application version (as defined by EZBL_appIDVer.appIDVer64).
 *
 * @param EZBL_FLOW_THRESHOLD optional project symbol. Provides loose control
 *        over flow control advertisements, as a byte count. Anytime the
 *        EZBL_COM_RX fifo contains >= EZBL_FLOW_THRESHOLD bytes worth of
 *        freespace available, a flow control message is generated, so smaller
 *        values for EZBL_FLOW_THRESHOLD cause more flow control advertisements
 *        to be generated while larger values can reduce overhead (at the
 *        possible expense of communications "burstiness").
 *
 *        If unspecified, the default value for EZBL_FLOW_THRESHOLD is 18 bytes.
 *
 *        To set this value, use the EZBL_SetSYM() macro in a project level
 *        source file.
 *
 * @param *EZBL_COM_TX Global pointer to a communications transmit FIFO for flow
 *        control and status messages. As an example, to use the default UART
 *        implementation, set:
 *          EZBL_COM_TX = &UART_TxFifo;
 *        To use an I2C slave medium, set:
 *          EZBL_COM_TX = &I2C_TxFifo;
 *
 *        By default, this parameter can be ignored as the associated
 *        communications RX ISR(s) contains a BOOTLOADER_WAKE_KEY definition,
 *        which causes the ISR to automatically set both EZBL_COM_TX and
 *        EZBL_COM_RX at the correct time based on an incoming data
 *        synchronization match of at least 8 bytes.
 *
 * @param *EZBL_COM_RX Global pointer to a communications receive FIFO to read
 *        the .bl2 firmware image from. As an example, to use the default UART
 *        implementation, set:
 *          EZBL_COM_RX = &UART_RxFifo;
 *        To use an I2C slave medium, set:
 *          EZBL_COM_RX = &I2C_RxFifo;
 *
 *        By default, this parameter can be ignored as the associated
 *        communications RX ISR(s) contains a BOOTLOADER_WAKE_KEY definition,
 *        which causes the ISR to automatically set both EZBL_COM_TX and
 *        EZBL_COM_RX at the correct time based on an incoming data
 *        synchronization match of at least 8 bytes. After this initial 8 byte
 *        key match, this function internally requires a BOOT_ID match, ensuring
 *        at least 16 more bytes are valid EZBL communications data.
 *
 *        Upon termination of the bootload operation (due to idle timeout,
 *        bootload failure, offered version mismatch, successful reprogramming,
 *        or any other cause), EZBL_COM_RX is set to 0 (null) to place this
 *        EZBL_Install2IP() function into a dormant/passive state (requires
 *        new ISR BOOTLOADER_WAKE_KEY match to wake up).
 *
 * @return <ul>
 *              <li>0 - normal operation. When in the passive state, this is the
 *                      immediate function response.</li>
 *              <li>1 - bootload-end event detected. A bootload-end event does
 *                      not necessarily indicate new firmware is available or
 *                      was attempted to be uploaded as COM RX idle timeouts are
 *                      also reported.</li>
 *         <ul>
 *
 *         When the return value is 1, read the s->bootCode parameter to
 *         determine the bootload-end cause.
 *
 *         For s->bootCode:
 *         Generally zero or greater is good, while negative values indicate a
 *         failure. Anything erased/programmed prior to a failure is left as it
 *         was upon failure detection (will automatically be cleaned up on next
 *         bootload attempt).
 *
 *         Specific return code meanings are:
 *         <ul>
 *          <li>EZBL_ERROR_SUCCESS (1): Operation completed successfully. The
 *              Inactive Partition is ready for execution.</li>
 *          <li>EZBL_ERROR_SUCCESS_VER_GAP (2): Operation completed successfully, 
 *              but the programmed image did not have an 
 *              APPID_VER_MAJOR.APPID_VER_MINOR field that was +1 (minor code) 
 *              from the existing application. Execution of the new code should 
 *              proceed by resetting rather than a Live Update bootswap.</li>
 *          <li>EZBL_ERROR_COM_READ_TIMEOUT (-20): Bootloader signaled 
 *              communications timeout attempting to read firmware image.</li>
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
 *              version.</li>
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
 *              for this device and bootloader.</li>
 *         </ul>
 */
int EZBL_Install2IP(EZBL_INSTALL_CTX *s)
{
    union
    {
        unsigned long long  u64[6];
        unsigned long       u32[12];
        unsigned short      u16[24];
        unsigned char        u8[48];
    } buf;                              // Can't be less than 48-bytes, preferably matches EZBL_FLOW_THRESHOLD: won't be better when bigger than necessary
    unsigned long now;
    unsigned long tmp;
    unsigned int i;
    unsigned int dataCount;
    int ret;                            // Variable for temporary signed return codes
    EZBL_MISMATCH_STATUS verifyStatus;  // Not used, but useful to have available if debugging programming failures (doesn't really cost much since it is on the stack)
    int moreWork;                       // Flag indicating we should restart the state machine for another execution pass
    int workDone;                       // Flag indicating something useful happened such that we should reset the idle timeout


    // Return immediately if the Bootloader's RX communications channel has been
    // disabled (i.e. no UUUUUUUUMCUPHCME unlock sequence detected in ISR yet)
    if(!EZBL_COM_RX)
        return 0;

    workDone = 0;
    do
    {
        moreWork = 0;
        dataCount = EZBL_COM_RX->dataCount;

        switch(s->state)
        {
            case SM_SYNC_INIT:  // Look for "BL2B" file start. Throws away any preceeding data, such as the "UUUUUUUUMCUPHCME" auto-baud/synchronization/bootloader wake up string decoded in the RX ISR.
                while(EZBL_COM_RX->dataCount >= 4u)
                {
                    workDone = 1;
                    EZBL_FIFOPeek(&tmp, EZBL_COM_RX, 4);
                    if(tmp == 0x42324C42ul)         // See if "BL2B" FILE_ID header sitting in FIFO
                    {
                        EZBL_printf("\n  Found BL2B header");
                        EZBL_FIFORead(0, EZBL_COM_TX, EZBL_COM_TX->dataCount);  // Throw all data in TX FIFO, if any, in the null bit bucket. This is needed for I2C which could have old failure codes laying around if the PC stopped polling us last time before we gave up.
                        s->state = SM_GET_HEADER;
                        moreWork = 1;
                        break;
                    }
                    EZBL_FIFORead8(EZBL_COM_RX);    // No match, eat a character to change peek location before testing again
                }
                break;

            case SM_GET_HEADER: // "BL2B" (4-byte), File Length (4-byte), BOOTID_HASH (16-byte), APPID_HASH (8-byte), HMAC-SHA-256 (16-byte)
                if(dataCount < 48u)
                    break;
                workDone = 1;
                EZBL_FIFORead(&buf, EZBL_COM_RX, 48);
                s->crc32 = EZBL_CRC32(0x00000000, &buf, 48);                // Start CRC32 computation to validate the whole communications stream/whole file integrity
                s->bytesToEOF = buf.u32[1] - 16u - 8u - 16u;                // Get File Length; Subtract 16+8+16 bytes for size of BOOTID_HASH, APPID_VER, and HMAC-SHA-256 data fields in the header (done later after potentially using this value)
                s->bootCode = EZBL_ERROR_SUCCESS;                           // Default value
                EZBL_printf("\n  BOOTID_HASH offered: %016llX%016llX", buf.u64[2], buf.u64[1]);
                EZBL_printf("\n                 ours: %016llX%016llX", EZBL_bootIDHash.bootIDHash64[1], EZBL_bootIDHash.bootIDHash64[0]);
                if(EZBL_RAMCompare(&buf.u64[1], &EZBL_bootIDHash, 16) != 0) // Check BOOTID_HASH field matches our Bootloader's bootIDHash
                {
                    EZBL_printf("\n  \x1B[31mBOOID_HASH mismatch\x1B[0m");
                    s->bootCode = EZBL_ERROR_BOOTID_HASH_MISMATCH;
                    s->state = SM_IGNORING;
                    break;
                }

                // Check that APPID_VER is >= APPID_VER that we are presently running
                EZBL_RAMCopy(&s->appVer, &buf.u64[3], 8);   // Get APPID_VER offering
                EZBL_printf("\n  APPID_VER: offered = %u.%02u.%04lu"
                            "\n             ours    = %u.%02u.%04lu", s->appVer.appIDVerMajor, s->appVer.appIDVerMinor, s->appVer.appIDVerBuild, EZBL_appIDVer.appIDVerMajor, EZBL_appIDVer.appIDVerMinor, EZBL_appIDVer.appIDVerBuild);
                if(EZBL_WeakSYM(EZBL_NO_APP_DOWNGRADE))
                {
                    if(s->appVer.appIDVer64 < EZBL_appIDVer.appIDVer64)
                    {
                        EZBL_printf("\n  APPID_VER older than already installed (EZBL_NO_APP_DOWNGRADE flag enabled)");
                        s->bootCode = EZBL_ERROR_APPID_VER_MISMATCH;
                        s->state = SM_TERMINATE;
                        moreWork = 1;
                        break;
                    }
                }

                // Also check that the APPID_VER is +1 our existing APPID_VER or else save EZBL_ERROR_SUCCESS_VER_GAP possible return value
                if((s->appVer.appIDVerMajorMinor < EZBL_appIDVer.appIDVerMajorMinor) || (s->appVer.appIDVerMajorMinor > EZBL_appIDVer.appIDVerMajorMinor + 1u))
                {
                    s->bootCode = EZBL_ERROR_SUCCESS_VER_GAP;
                }

                // Compute HMAC, if crypto is enabled
                // Compare 16-bytes of HMAC-SHA-256 data, if crypto is enabled
                // Use 16 extra bytes of HMAC-SHA-256 data as NONCE for AES-128 IV
                // If valid, begin AES-128 CTR decryption (actually encryption + XOR)
                if(&EZBL_CryptKey == 0u)
                {   // No crypto enabled, just verify HMAC field is all zero's
                    for(buf.u16[0] = 0x0000u, i = 16; i < 24u; i++)
                    {
                        buf.u16[0] |= buf.u16[i];
                    }
                    if(buf.u16[0])
                    {   // Crypto appears to be enabled, check HMAC. NOTE: this
                        // should be done using a constant-time compare
                        // algorithm to avoid attack by sequentially sending
                        // HMACs that only increment the first byte until a
                        // compare duration is observed increasing.
                        s->bootCode = EZBL_ERROR_IMAGE_MALFORMED;
                        s->state = SM_TERMINATE;
                        moreWork = 1;
                        EZBL_printf("\n  HMAC invalid; terminating");
                        break;
                    }
                }

                if(EZBL_PreInstall)
                {
                    ret = EZBL_PreInstall(EZBL_COM_RX, EZBL_COM_TX, s->bytesToEOF + 64u, &s->appVer);
                    if(ret <= 0)
                    {
                        EZBL_printf("\n  EZBL_PreInstall() returned %i, so aborting update", ret);
                        s->bootCode = EZBL_ERROR_IMAGE_REJECTED;                // (-27)  // 0xFFE5 Bootloader or running application rejected the offered image;
                        s->state = SM_TERMINATE;
                        moreWork = 1;
                        break;
                    }
                }

                EZBL_printf("\n  Accepting offered firmware update: size = %lu", s->bytesToEOF + 64);   // 64 bytes for header (SYNC included)
                // No break;

            //case SM_ERASE:    // Don't need this state for EZBL_Install2IP(). It is in the state machine, however, for use in other EZBL_Install*() functions.
                EZBL_FIFOWrite16(EZBL_COM_TX, 0xFF13);          // 0x13 is an XOFF character to allow rudimentary file upload via an EZBL protocol unaware application at slower baud rates
                EZBL_printf("\n  Erasing Inactive Partition");
                EZBL_EraseInactivePartition();                  // Not done erasing yet, but since we aren't blocking for the Inactive Partition erase, keep going
                s->recHdr.len = 0;                              // First read needs to get a DATA_RECORD header
                EZBL_FIFOWrite16(EZBL_COM_TX, 0xFF11);          // 0x11 is an XON character to allow rudimentary file upload via a terminal application at slower baud rates
                dataCount = EZBL_COM_RX->dataCount;             // We consumed 48 bytes from SM_GET_HEADER, so since we are falling through, update our cached dataCount
                s->bytesRequested = 0;                          // Init flow control number
                //s->state = SM_SET_UP_GET;
                // No break

            case SM_SET_UP_GET:
                workDone = 1;
                s->state = SM_GET_DATA;
                if(s->bytesToEOF <= 32u + 4u)                   // 32-byte SHA-256 file hash + 4-byte CRC32 just before EOF
                {
                    s->getDataBytes = 32u + 4u;
                    s->getDataNextState = SM_TEST_CHECKSUMS;
                    EZBL_printf("\n  Getting checksums");
                }
                else if(s->recHdr.len == 0u)
                {
                    s->getDataBytes = 8;                        // 8 bytes/record header
                    s->getDataNextState = SM_SET_UP_GET;
#if defined(EZBL_DEBUG) && defined(VERBOSE)
                    EZBL_printf("\n  Getting header");
#endif
                }
                else                                            // Ordinary data record bytes
                {
                    EZBL_NVMKey += 0x03DF;                      // Unlock here with add so code must follow correct non-linear path to SM_PROG_REC_DATA in order to execute the NVM write
                    s->getDataBytes = 12;                       // Somewhat arbitrary read chunk size to suppress looping on overly tiny chunks of data
                    if(dataCount > 12u)
                        s->getDataBytes = dataCount;
                    if(s->getDataBytes > sizeof(buf))
                        s->getDataBytes = sizeof(buf);
                    if((unsigned long)s->getDataBytes >= s->recHdr.len)
                    {   // Only fetch up to the end of this record
                        s->getDataBytes = (unsigned int)s->recHdr.len;
                    }
                    else
                    {   // When fetching less than the full record, make sure we always process data in 6 byte multiples so we can stay aligned to word/double word flash write size requirements
                        s->getDataBytes -= s->getDataBytes % 6;
                    }
#if defined(EZBL_DEBUG) && defined(VERBOSE)
                    EZBL_printf("\n  Getting %u bytes of prog data", s->getDataBytes);
#endif
                    s->getDataNextState = SM_PROG_REC_DATA;
                }
                // No break;

            case SM_GET_DATA:
                i = EZBL_COM_RX->fifoSize - s->bytesRequested;
                if((unsigned char)i == 0x13u)           // Adjust by one byte if a 0x13 XOFF character would appear in the stream
                    i--;
                if(((unsigned char*)&i)[1] == 0x13u)    // Also adjust by 0x100 bytes if a 0x13 XOFF character would appear in the second byte
                    i ^= 0x0100u;
                ret = EZBL_SYM(EZBL_FLOW_THRESHOLD);            // ret is just a temporary variable
                if((unsigned int)ret > EZBL_COM_RX->fifoSize)   // Limit to real RX buffer space to ensure reliable results if multiple communications mediums (ex: UART + I2C) could be used at run-time and different FIFO sizes exist on them.
                    ret = EZBL_COM_RX->fifoSize;
                if((i >= (unsigned int)ret) || (s->getDataBytes > s->bytesRequested))
                {
                    if(s->getDataBytes > s->bytesRequested)
                    {   // If we need to send a flow data request but don't have space in the TX FIFO to write this don't risk blindly writing and losing synchronization due to a dropped byte
                        if(EZBL_COM_TX->fifoSize - EZBL_COM_TX->dataCount < 2)
                        {
                            break;
                        }
                    }
                    workDone = 1;
#if defined(EZBL_DEBUG) && defined(VERBOSE)
                    EZBL_printf("\n  Requesting: %u bytes", i);
#endif
                    EZBL_FIFOWrite16(EZBL_COM_TX, i);       // Transmit a 16-bit flow control data request (number of bytes of free space we have/can accept if sent to us)
                    s->bytesRequested += i;
                }
                if(dataCount < s->getDataBytes)
                    break;
                workDone = 1;
                s->bytesToEOF -= s->getDataBytes;
                s->bytesRequested -= s->getDataBytes;
                EZBL_FIFORead(&buf, EZBL_COM_RX, s->getDataBytes);
                s->crc32 = EZBL_CRC32(s->crc32, &buf, s->getDataBytes);
                if(s->getDataNextState == SM_SET_UP_GET)            // Copy buf data to recHdr if we came from the SM_SET_UP_GET state and are going back to it
                {
                    EZBL_RAMCopy(&s->recHdr, &buf, s->getDataBytes);
                    if(s->recHdr.addr8[3] | s->recHdr.len8[3])      // Sanity check that both the address and length fit in 24-bits
                    {
                        s->bootCode = EZBL_ERROR_IMAGE_MALFORMED;
                        s->state = SM_TERMINATE;
                        moreWork = 1;
                        break;
                    }
                }
                s->state = s->getDataNextState;
                moreWork = 1;
                break;

            case SM_PROG_REC_DATA:
                workDone = 1;
                moreWork = 1;
#if defined(EZBL_DEBUG) && defined(VERBOSE)
                EZBL_DumpFlashDiff("  Write: ", (void*)0, s->recHdr.addr, &buf, (void*)0, s->getDataBytes);
#endif
                tmp = EZBL_WriteROMEx(s->recHdr.addr, &buf, s->getDataBytes, EZBL_FLAG_CLRWDT | EZBL_FLAG_IPARTITION, 0, 0);
                ret = EZBL_VerifyROM(s->recHdr.addr | 0x400000u, &buf, s->getDataBytes, &verifyStatus);
#if defined(EZBL_DEBUG)
                unsigned char buf2[sizeof(buf)];
                EZBL_ReadPackedFlash(buf2, s->recHdr.addr | 0x400000u, 0, s->getDataBytes);
                if(EZBL_RAMCompare(&buf, buf2, s->getDataBytes) != 0)
                {
                    const char *causes[] =
                        {
                            "unknown; hardware write protect?",
                            "programming restricted",
                            "verify restricted; expected mismatch",
                            "program and verify restricted; expected mismatch",
                            "erase restricted",
                            "program and erase restricted; bootloader mismatch",
                            "verify and erase restricted; expected mismatch",
                            "program, verify and erase restricted",
                        };
                    i = EZBL_IsProgramRestricted(s->recHdr.addr) | (EZBL_IsVerifyRestricted(s->recHdr.addr)<<1) | (EZBL_IsEraseRestricted(s->recHdr.addr)<<2);
                    EZBL_printf("\n  MISMATCH (%s)", causes[i]);
                    EZBL_DumpFlashDiff("  Write: ", "  Read:  ", s->recHdr.addr, &buf, buf2, s->getDataBytes);
                }
#endif
                s->recHdr.addr = tmp;
                s->recHdr.len -= s->getDataBytes;
                s->state = SM_SET_UP_GET;
                if(ret < 0)
                {
                    s->bootCode = EZBL_ERROR_HARD_VERIFY_ERROR;                 // App-space data programmed does not match read-back value, even after adjusting for special addresses
                    s->state = SM_TERMINATE;
                }
                if(ret > 0)
                {
                    s->bootCode = EZBL_ERROR_SOFT_VERIFY_ERROR;                 // Bootloader reserved region data changed in image, but all App space data still checks out
                }
                break;

            case SM_TEST_CHECKSUMS:
                workDone = 1;
                EZBL_FIFOWrite16(EZBL_COM_TX, 0x0000);                          // Signal bootload termination
                if(s->crc32 == 0x2144DF1Cu)                                     // CRC32 computation includes the CRC32 at the very end, so magic constant 0x2144DF1C indicates validity of all data
                {   // All programming successful and .BL2 integrity verified, so program FBTSEQ as needed to make it the reset active partition
                    EZBL_printf("\n  CRC32 good: 0x%08lX\n\n", buf.u32[8]);
                    if(EZBL_WeakSYM(EZBL_AUTO_PROG_FBTSEQ))
                    {
                        if(EZBL_WriteFBTSEQW)
                        {
                            EZBL_WriteFBTSEQW(0, 0, -1);    // Write Inactive Partition's FBTSEQ value to be one less than this Active Partition's FBTSEQ value
                        }
                    }
                }
                else
                {   // CRC failure
                    s->bootCode = EZBL_ERROR_IMAGE_CRC;
                    EZBL_printf("\n  \x1B[31mCHECKSUM MISMATCH\x1B[0m"
                                "\n  File:     0x%08lX"
                                "\n  Expect:   0x2144DF1C"
                                "\n  Computed: 0x%08lX"
                                "\n"
                                "\n", buf.u32[8], s->crc32);
                }
                EZBL_FIFOWrite16(EZBL_COM_TX, s->bootCode);                     // Reply back with EZBL_ERROR_SUCCESS, EZBL_ERROR_SUCCESS_VER_GAP, EZBL_ERROR_SOFT_VERIFY_ERROR or EZBL_ERROR_IMAGE_CRC
                s->state = SM_SYNC_INIT;
                EZBL_FIFOFlush(EZBL_COM_TX, NOW_ms*2u);                         // Block up to 2ms for the status return code (and debugging messages, if enabled) to be transmitted since we have completed a whole bootload cycle and the caller may try to reset or partition swap immediately.
                EZBL_COM_RX = 0;                                                // Stop processing data until reenabled by RX interrupt
                break;

            case SM_TERMINATE:
                if(EZBL_COM_TX->fifoSize - EZBL_COM_TX->dataCount > 4u)
                {
                    EZBL_printf("\n  Terminating: code = 0x%04X", s->bootCode);
                    workDone = 1;
                    moreWork = 1;
                    s->state = SM_IGNORING;
                    EZBL_FIFOWrite16(EZBL_COM_TX, 0x0000);                      // Send termination indicator value
                    EZBL_FIFOWrite16(EZBL_COM_TX, s->bootCode);                 // Reply back with EZBL_ERROR_SUCCESS, EZBL_ERROR_SUCCESS_VER_GAP, EZBL_ERROR_SOFT_VERIFY_ERROR or EZBL_ERROR_IMAGE_CRC
                }
                break;

            case SM_IGNORING:
                if(dataCount)
                {
                    workDone = 1;
                    if((unsigned long)dataCount >= s->bytesToEOF)
                    {
                        dataCount = (unsigned int)s->bytesToEOF;
                        EZBL_FIFORead(0, EZBL_COM_RX, dataCount);               // Send RX data to the bit bucket until EOF is reached
                        s->state = SM_SYNC_INIT;                                // All done, so can start listening again for a new bootload sequence
                        EZBL_COM_RX = 0;                                        // Stop processing data until reenabled by RX interrupt
                    }
                    EZBL_FIFORead(0, EZBL_COM_RX, dataCount);                   // Send RX data to the bit bucket until EOF is reached. If we made EZBL_COM_RX null above, this function does nothing.
                    s->bytesToEOF -= dataCount;
                }
                break;

            default:    // Should not happen
                EZBL_ResetCPU();
                break;
        }

        // Track/handle timeout asynchronous reset
        now = NOW_32();
        if(workDone)
        {
            workDone = 0;
            s->lastActivity = now;
        }
        if(now - s->lastActivity > s->timeout)                                  // At any timeout, go to the sync initialization state and abort
        {
            s->bootCode = EZBL_ERROR_COM_READ_TIMEOUT;
            if((s->state > SM_GET_HEADER) && (s->state < SM_TEST_CHECKSUMS))
            {
                EZBL_FIFOWrite16(EZBL_COM_TX, 0x0000);                          // Indicate termination and error code back to remote node
                EZBL_FIFOWrite16(EZBL_COM_TX, s->bootCode);
                EZBL_FIFORead(0, EZBL_COM_RX, 0xFFFF);                          // Throw away any data sitting in the RX FIFO. If EZBL_COM_RX is null, this does nothing.
                EZBL_printf("\n--------TIMEOUT--------\n\n");
            }
            s->lastActivity = now;
            s->state = SM_SYNC_INIT;
            EZBL_COM_RX = 0;
        }
    } while(EZBL_COM_RX && moreWork); // Keep processing more tasks if we changed state to a non-linear flow down case



    return (EZBL_COM_RX == 0);  // Return 1 if are signaling a bootload completion event (caller must check s->bootCode to see if we actually bootloaded something since failures will also return 1)
}
