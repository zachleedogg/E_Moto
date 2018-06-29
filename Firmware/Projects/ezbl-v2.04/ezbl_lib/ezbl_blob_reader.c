/*******************************************************************************
  Easy Bootloader for PIC24 and dsPIC33 Library Source File

  Summary:
    Code for the essential core components of the EZBL library.

  Description:
    Contains code for the main EZBL API, and essential Flash read/write/erase
    functions.

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

#include "ezbl.h"


// Weak references to the EZBL_CryptBlob() function and EZBL_cryptState 
// structure so we don't force symbol references and waste code/RAM space 
// without having crypto enabled
static signed int __attribute__((weakref("EZBL_CryptBlob"))) EZBL_CryptBlobW(EZBL_CRYPT_STATE *cryptState, unsigned char *blobBytesIn, unsigned char *blobBytesOut, unsigned int bytesToCrypt);
static EZBL_CRYPT_STATE __attribute__((weakref("EZBL_cryptState"))) EZBL_cryptStateW;


unsigned int EZBL_SendFlowUpdate(EZBL_BLOB_RX_COM *blobRX, unsigned int dontBotherThreshold);


/**
 * Opens a .bl2 file for reading and reads the first 8 bytes of the file after
 * the synchronization string to get the full file length. Internal CRC32
 * computation, decryption (if applicable),and flow control state data is reset
 * to start the read or file transfer.
 * 
 * @param *blobRX Pointer to a caller allocated EZBL_FILE_RX_STATE structure
 *                in a RAM location that will persist until EZBL_BlobReadEnd() 
 *                is called. 
 * 
 *                The structure members are initialized by this function, 
 *                overwriting any previous contents.
 *
 * @param *EZBL_COM_RX EZBL_FIFO pointer for the FIFO to read the file contents
 *                     from. This global pointer cannot be null.
 *
 * @param *EZBL_COM_TX EZBL_FIFO pointer to write flow control transmit data.
 *                     This pointer should be null if the communications medium
 *                     does not require EZBL software flow control (ex: hardware
 *                     flow control exists or memory is onboard and requires no
 *                     flow control).
 *
 * @param activityTimeout Number of now counts that this and future 
 *                        EZBL_BlobRead() calls can maximally block for before 
 *                        timing out and returning an error.
 * 
 * @return On success, 8, representing the number of bytes already read by this
 *         call to get the file's total length. Calls to EZBL_BlobRead() can be
 *         subsequently made.
 * 
 *         On read/communications timeout or other error, a negative number is
 *         returned, matching the meaning defined for a 4-byte EZBL_BlobRead()
 *         call.
 */
int EZBL_BlobReadStart(EZBL_BLOB_RX_COM *blobRX, unsigned long activityTimeout)
{
    if(!EZBL_COM_RX)    // Abort if EZBL_COM_RX pointer is null. This is the file source.
    {
        return EZBL_ERROR_INVALID_PARAMETER;    // EZBL_ERROR_INVALID_PARAMETER (-101): Invalid input parameter(s)
    }

    // Request the first 8 bytes of the .bl2 file, which will be "BL2B" followed by the remaining file size
    if(EZBL_COM_TX)
    {
        blobRX->flags.reqInAdv     = 1; // Request bytes from the remote node in advance of needing them
        EZBL_FIFOWrite16(EZBL_COM_TX, 4);
    }
    
    blobRX->lastActivity           = NOW_32();
    blobRX->timeoutInterval        = activityTimeout;
    blobRX->cryptState             = &EZBL_cryptStateW;
    if(&EZBL_cryptStateW)
    {
        blobRX->cryptState->offset = 0;
    }
    blobRX->crc32                  = 0x00000000;
    blobRX->pendingRX              = 4;
    blobRX->bytesToEOF             = 4;

    // Read the .blob file length to kick off a forward data request
    return EZBL_BlobRead(&(blobRX->bytesToEOF), blobRX, 4);
}

/**
 * Reads the specified number of bytes from the image file previously opened for
 * reading with EZBL_BlobReadStart(). This is a blocking function, but will 
 * timeout and return without all specified bytes if a non-infinite timeout was 
 * set and the timeoutInterval has elapsed since the last recorded RX or TX 
 * activity.
 * 
 * For communications mediums that require it, this function also simultaneously 
 * transmits 16-bit unsigned short byte count requests for software flow 
 * control.
 * 
 * If encryption was enabled at compile time, the raw data is decrypted before
 * return.
 * 
 * @param *dest RAM pointer to write the decrypted, image file data to. This
 *        parameter cannot be null unless bytesToRead is 0.
 *
 * @param *blobRX Pointer to a EZBL_FILE_RX_STATE structure for the file that
 *        was previously opened for reading. The structure members are updated 
 *        to decrement bytesToEOF, record a new lastActivity time stamp, update 
 *        the running crc32 over the full file, decryption state is updated if 
 *        applicable, and pendingRX is also updated if a new flow control data
 *        request was generated.
 * 
 * @param bytesToRead Number of file bytes that should be read from the 
 *        communications/storage medium. If the count extends past the End Of 
 *        File, only the bytes needed to achieve EOF are returned. If set to 
 *        zero, no data will be read, but any flow control processing will still 
 *        take place.
 * 
 * @return On success, the number of bytes written to *dest. The number may be 
 *         less than bytesToRead, down to 0, if EOF is reached. 
 * 
 *         On read timeout, the number of bytes written to *dest prior to the 
 *         timeout is one's complemented prior to return so as to always be a 
 *         negative quantity. I.e. ~bytesRead. This implies that if 0 bytes were 
 *         read, 0xFFFF or -1 will be returned.
 */
int EZBL_BlobRead(void *dest, EZBL_BLOB_RX_COM *blobRX, unsigned int bytesToRead)
{
    unsigned int i;
    unsigned int initialBytesToRead;
    
    if(blobRX->bytesToEOF < bytesToRead)
        bytesToRead = (unsigned int)(blobRX->bytesToEOF);
    
    // Request some data to fill up our RX buffer if we haven't already done so
    if(EZBL_COM_TX)
    {
        if(!blobRX->flags.reqInAdv)
        {
            i = EZBL_COM_RX->fifoSize - EZBL_COM_RX->dataCount - blobRX->pendingRX;
            if(i > blobRX->bytesToEOF - blobRX->pendingRX)
                i = (unsigned int)blobRX->bytesToEOF - blobRX->pendingRX;
            if(i > bytesToRead)
                i = bytesToRead;
            if(i != 0u)
            {
                EZBL_FIFOWrite16(EZBL_COM_TX, i);
                blobRX->pendingRX      += i;
                blobRX->lastActivity    = NOW_32();
            }
        }
    }
    
    // Read RX bytes with timeout handling
    initialBytesToRead = bytesToRead;
    while(bytesToRead)
    {
        i = EZBL_COM_RX->dataCount;
        if(i > bytesToRead)
            i = bytesToRead;
        if(i)
        {
            EZBL_FIFORead(dest, EZBL_COM_RX, i);
            blobRX->crc32          = EZBL_CRC32(blobRX->crc32, dest, i);
            blobRX->pendingRX     -= i;
            bytesToRead           -= i;
            dest                  += i;
            blobRX->lastActivity   = NOW_32();
            
            // Decrypt this data chunk, if applicable
            if(EZBL_CryptBlobW && blobRX->cryptState)
            {
                EZBL_CryptBlobW(blobRX->cryptState, dest, dest, i);     // NOTE: Must have "EZBL_KeepSYM(EZBL_CryptBlob)" in your project for this to be called
            }
        }
        if(bytesToRead > blobRX->pendingRX + EZBL_COM_RX->dataCount)
            EZBL_SendFlowUpdate(blobRX, EZBL_COM_RX->fifoSize>>1);
        
        if(NOW_32() - blobRX->lastActivity > blobRX->timeoutInterval)   // NOTE: 0xFFFFFFFF for the timeoutInterval means 'infinite', not a standard NOW_32() count
        {
            return ~(initialBytesToRead-bytesToRead);
        }
    }

    blobRX->bytesToEOF    -= initialBytesToRead;

    // Request new data to keep our RX pipeline going
    if(blobRX->flags.reqInAdv)
    {
        EZBL_SendFlowUpdate(blobRX, initialBytesToRead);
    }
    
    return (int)initialBytesToRead;
}


/**
 * Closes the .blob/.bl2 file for reading.
 * 
 * Termination entails sending a final 0x0000 termination indicator + the 16-bit
 * terminationStatus code back to the host (for non-local .blob/.bl2 file 
 * sources). File handle/pointers are closed. Any decryption key state, if 
 * applicable, is cleared from RAM.
 * 
 * @param *blobRX Pointer to a EZBL_FILE_RX_STATE structure for the file that
 *        was previously opened for reading.
 *
 * @param terminationStatus 16-bit identifier to send back to the remote node 
 *        and use as a return value.
 * 
 *        For local .blob/.bl2 file sources, this parameter is used only as a 
 *        return value.
 * 
 * @param flushRXFIFO Flag indicating that the EZBL_COM_RX contents should be
 *        purged on any waiting or arriving characters that were previously 
 *        requested by a flow control mechanism. On ordinary file completion, 
 *        there generally is no need to purge anything as any subsequent FIFO 
 *        contents would likely be valid post-upload commands. However, on 
 *        abnormal termination or error cases, some residual bytes may be 
 *        floating on the wire or waiting in the buffer which belong to the 
 *        .blob/.bl2 file, so this flag attempts to purge these bytes for the 
 *        FIFO to be reused for command processing quicker than an idle timeout.
 * 
 *        Set to 1 to attempt purging of any remaining bytes belonging to the 
 *        .blob/.bl2 file. 0 leaves the RX FIFO contents unchanged.
 *        
 * @return terminationStatus, unchanged.
 */
int EZBL_BlobReadEnd(EZBL_BLOB_RX_COM *blobRX, int terminationStatus, int flushRXFIFO)
{
    if(EZBL_COM_TX)
    {
        EZBL_FIFOWrite32(EZBL_COM_TX, 0x0000ul | (((unsigned long)terminationStatus)<<16));   // Signal termination to remote node
    }
    if(flushRXFIFO)
    {
        EZBL_FIFORead(0, EZBL_COM_RX, blobRX->pendingRX);
        blobRX->pendingRX = 0;
    }

    // We are done using the crypto key material, so there is no sense in 
    // leaving it in RAM.
    if(blobRX->cryptState)
    {
        EZBL_RAMSet(blobRX->cryptState, 0x00, sizeof(EZBL_CRYPT_STATE));
    }

    return terminationStatus;
}


/**
 * Writes a flow control window update message to the specified blob reader's TX 
 * EZBL_FIFO indicating how many bytes of free space exist in the RX FIFO for 
 * the remote node to throttle transmissions to us.
 * 
 * If the blob reader was opened without needing flow control, then this 
 * function does nothing.
 * 
 * A flow control message is an unsigned short integer (16-bit little endian) 
 * indicating how many bytes of free space exist in the RX FIFO less the how 
 * many unseen bytes have already been advertised as free space.
 * 
 * @param *blobRX Pointer to an EZBL_FILE_RX_STATE structure containing bytes to
 *                EOF and prior flow control message status. This parameter 
 *                cannot be null.
 * 
 * @param dontBotherThreshold Limit that suppresses the flow control update if
 *                the computed free RX space (less prior unfilled flow window 
 *                advertisements) is less than this value. This is used to 
 *                avoid silly-window syndromes where our RX buffer is already 
 *                essentially full and each byte read results in two bytes of 
 *                flow control data being transmitted, and one new byte 
 *                transmitted from the remote node to replace it, repeating ad 
 *                nauseam. Such transactions can be a huge waste of bandwidth on 
 *                packet oriented interfaces and simultaneously slow processing 
 *                by framing/deframing each small message.
 * 
 * @return Number of window bytes advertised to the remote node as a result of 
 *         this call. Zero means that no flow control update message was 
 *         transmitted.
 * 
 *         If the blob was opened indicating no flow control is needed, 1 is 
 *         returned without any internal processing.
 */
unsigned int EZBL_SendFlowUpdate(EZBL_BLOB_RX_COM *blobRX, unsigned int dontBotherThreshold)
{
    unsigned int i;
    
    // Do nothing if this RX session doesn't require flow control
    if(!EZBL_COM_TX)
    {
        return 1;
    }
    
    // Compute rxFIFO free space, less already advertised but not yet received data size
    i = EZBL_COM_RX->fifoSize - (blobRX->pendingRX - EZBL_COM_RX->dataCount);

    // Saturate at the bytes left to End Of File
    if(i > blobRX->bytesToEOF - blobRX->pendingRX)
        i = (unsigned int)blobRX->bytesToEOF - blobRX->pendingRX;

    // Only generate flow update if the TX FIFO actually has space to support a 
    // non-blocking write
    if(EZBL_COM_TX->fifoSize - EZBL_COM_TX->dataCount < 2u)
        return 0;
        
    // Reduce the dontBotherThreshold if we are at or near EOF
    if(blobRX->bytesToEOF < dontBotherThreshold)
        dontBotherThreshold = blobRX->bytesToEOF;

    // Don't send flow update unless there is enough free space to be worthwhile
    if((i == 0u) || (i < dontBotherThreshold))
        return 0;

    EZBL_FIFOWrite16(EZBL_COM_TX, i);
    blobRX->pendingRX      += i;
    blobRX->lastActivity    = NOW_32();
    return i;
}
