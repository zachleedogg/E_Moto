/*
 *******************************************************************************
 * Copyright (c) 2016 Microchip Technology Inc. All rights reserved.
 *
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital signal
 * controller that is integrated into your product or third party product
 * (pursuant to the sublicense terms in the accompanying license agreement).
 *
 * You should refer to the license agreement accompanying this Software for
 * additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
 * MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED
 * UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF
 * WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR
 * EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT,
 * PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD
 * PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR
 * COSTS.
 *******************************************************************************
 */
package com.microchip.apps.ezbl;

import static com.microchip.apps.ezbl.Multifunction.CatStringList;
import java.security.*;
import java.util.*;
import java.util.zip.*;
import javax.crypto.*;
import javax.crypto.spec.*;


/**
 *
 * @author C12128
 */
public class Blob
// Do not serialize to get the blob normally. Use getBytes().
{
    boolean architecture16Bit;
    List<DataRecord> records = null;
    long typeCode = 0x424F4C42; // 0x42324C42 = "BL2B" or 0x424F4C42 = "BLOB"

    private enum HexDecoderState
    {
        COLON,
        LENGTH0,
        LENGTH1,
        ADDRESS0,
        ADDRESS1,
        ADDRESS2,
        ADDRESS3,
        TYPE0,
        TYPE1,
        DATA0,
        DATA1,
        CHECK0,
        CHECK1
    };


    // Creates a blob with no data in it
    // @param architecture16Bit Boolean specifying if this blob is for a 16-bit
    //                          device (PIC24 or dsPIC) or other (PIC32, etc.).
    //                          true means PIC24 or dsPIC. Specify false for any
    //                          other architecture.
    public Blob(boolean architecture16Bit)
    {
        records = new ArrayList<DataRecord>();
        this.architecture16Bit = architecture16Bit;
    }

    // Initializes a Blob and internal data records from a preexisting blob file
    // (passed in as a byte array).
    // @param blobData Array of bytes containing the whole blob, including all
    //                 headers and hash (i.e. complete .blob file contents).
    // @param architecture16Bit Boolean specifying if this blob is for a 16-bit
    //                          device (PIC24 or dsPIC) or other (PIC32, etc.).
    //                          true means PIC24 or dsPIC. Specify false for any
    //                          other architecture.
    public Blob(byte blobData[], boolean architecture16Bit)
    {
        long length;
        byte hash[] = new byte[20];
        long crcInFile = 0;
        DataRecord record;

        if(blobData.length < 32)
        {
            throw new RuntimeException("Invalid blob; length too small");
        }

        // Get a copy of the Hash and trailing CRC and zero them both within the blob
        System.arraycopy(blobData, 8, hash, 0, 20);
        for(int i = 0; i < 4; i++)
        {
            crcInFile |= (((long)blobData[blobData.length - 4 + i]) & 0xFF) << (i * 8);
            blobData[blobData.length - 4 + i] = 0x00;       // Zero the CRC so it matches the proper hash computation sequence
        }
        Arrays.fill(blobData, 8, 28, (byte)0x00);           // Zero the hash so we can compare it when done

        // Compute and compare with stored SHA-1 hash with computed hash
        byte computedHash[];
        MessageDigest hashComputer;
        try
        {
            hashComputer = MessageDigest.getInstance("SHA-1");
        }
        catch(NoSuchAlgorithmException ex)
        {
            throw new RuntimeException("Cannot parse blob; missing SHA-1 java hashing library");
        }
        hashComputer.update(blobData);
        computedHash = hashComputer.digest();
        for(int i = 0; i < 20; i++)
        {
            if(computedHash[i] != hash[i])
            {
                throw new RuntimeException("Invalid blob; internal hash mismatches with computed hash");
            }
        }

        length = ((long)blobData[0]) & 0xFFl;
        length |= (((long)blobData[1]) & 0xFFl) << (8 * 1);
        length |= (((long)blobData[2]) & 0xFFl) << (8 * 2);
        length |= (((long)blobData[3]) & 0xFFl) << (8 * 3);

        if(length != blobData.length)
        {
            throw new RuntimeException("Invalid blob; data size does not match blob size");
        }

        this.records = new ArrayList<DataRecord>();

        this.typeCode = ((long)blobData[4]) & 0xFFl;
        this.typeCode |= (((long)blobData[5]) & 0xFFl) << (8 * 1);
        this.typeCode |= (((long)blobData[6]) & 0xFFl) << (8 * 2);
        this.typeCode |= (((long)blobData[7]) & 0xFFl) << (8 * 3);

        // Put the hash back in the array since it has been verified
        System.arraycopy(computedHash, 0, blobData, 8, computedHash.length);

        // Now compute the expected CRC32 at the end
        CRC32 myCRC = new CRC32();
        myCRC.update(blobData, 0, blobData.length - 4);
        long crcValue = myCRC.getValue();
        if(crcValue != crcInFile)
        {
            throw new RuntimeException("Invalid blob; internal CRC mismatches computed CRC");
        }

        int curIndex = 28;
        while(curIndex < blobData.length - 4)
        {
            record = new DataRecord(architecture16Bit);
            int recordByteLen;

            recordByteLen = ((int)blobData[curIndex++]) & 0xFF;
            recordByteLen |= (((int)blobData[curIndex++]) & 0xFFl) << (8 * 1);
            recordByteLen |= (((int)blobData[curIndex++]) & 0xFFl) << (8 * 2);
            recordByteLen |= (((int)blobData[curIndex++]) & 0xFFl) << (8 * 3);
            record.address = ((long)blobData[curIndex++]) & 0xFFl;
            record.address |= (((long)blobData[curIndex++]) & 0xFFl) << (8 * 1);
            record.address |= (((long)blobData[curIndex++]) & 0xFFl) << (8 * 2);
            record.address |= (((long)blobData[curIndex++]) & 0xFFl) << (8 * 3);
            record.data = Arrays.copyOfRange(blobData, curIndex, curIndex + recordByteLen - 8);
            this.records.add(record);
            curIndex += recordByteLen - 8;
        }

        this.architecture16Bit = architecture16Bit;
    }

    public Blob(String hexFileContents, boolean architecture16Bit)
    {
        this.architecture16Bit = false;
        this.records = ParseHex(hexFileContents, false);

        // Sort the list by address and coalesce adjacent data records
        DataRecord.CoalesceRecords(this.records);

        // Determine what the .hex file is targetting: 16-bit device or 8/32-bit device
        if(architecture16Bit || TestRecordsLookLike16Bit(this.records))
        {
            this.architecture16Bit = true;
            for(DataRecord dr : this.records)
            {
                dr.architecture16Bit = true;
            }

            // Remove phantom bytes if this is for a PIC24/dsPIC
            RemoveMCU16PhantomBytesAndAddresses();
        }
    }

    /**
     * Enumerates the list of records and attempts to auto-determine if the
     * records are for a 16-bit PIC24/dsPIC or a 32-bit/8-bit device
     *
     * @param records List of records to test address/data for 16-bit device
     *                requirements
     *
     * @return true if the records are consistent with XC16 output, false
     *         otherwise
     */
    static public boolean TestRecordsLookLike16Bit(List<DataRecord> records)
    {
        int highAddresses = 0;
        int dataAtPhantomBytePos = 0;
        for(DataRecord dr : records)
        {
            if((dr.address & 0xFE000000) != 0)  // Leave one extra address bit available since .hex files use addresses left shifted by 1
            {
                highAddresses++;
            }
            for(int i = 3 - (int)(dr.address & 0x3); i < dr.data.length; i += 4)
            {
                if((dr.data[i] != 0))
                {
                    dataAtPhantomBytePos++;
                }
            }
        }
        return ((highAddresses == 0) && (dataAtPhantomBytePos == 0));
    }

    // Creates a list of DataRecords, uncoalesced and unoptimized from a .hex file passed as a binary array of data
    static List<DataRecord> ParseHex(byte hexFileContents[], boolean architecture16Bit)
    {
        HexDecoderState state = HexDecoderState.COLON;
        long decoderAddress = 0, hexHEX386Address = 0;
        int hexRecordType = 0;
        int hexRecordAddress = 0;
        int hexRecordLen = 0;
        int hexRecordDataIndex = 0;
        int hexRecordChecksum = 0, computedRecordChecksum = 0;
        byte hexRecordData[] = new byte[256];
        byte b;
        DataRecord record = null;
        List<DataRecord> records = new ArrayList<DataRecord>();

        // Decode .hex file text, one byte at a time
        for(int inIndex = 0; inIndex < hexFileContents.length; inIndex++)
        {
            // Get the byte into b
            b = hexFileContents[inIndex];

            // Eat whitespace
            if((b == ' ') || (b == '\r') || (b == '\n') || (b == '\t'))
            {
                if(state != HexDecoderState.COLON)
                {
                    System.err.println("    EZBL: Error reading .hex file: whitespace detected within a record; skipping");
                }
                continue;
            }

            // Check for illegal non-hex, colon, or whitespace characters
            if(state == HexDecoderState.COLON)
            {
                if(b != ':')
                {
                    System.err.println("    EZBL: Error reading .hex file: unknown character encountered; skipping");
                    continue;
                }
                state = HexDecoderState.LENGTH0;
                continue;
            }

            // Convert ASCII hex character to a binary number (nibble) from 0 to 15
            if((b >= '0') && (b <= '9'))
            {
                b = (byte)(b - '0');
            }
            else if((b >= 'a') && (b <= 'f'))
            {
                b = (byte)(b - 'a' + 10);
            }
            else if((b >= 'A') && (b <= 'F'))
            {
                b = (byte)(b - 'A' + 10);
            }
            else
            {
                System.err.println("    EZBL: Error reading .hex file: unknown character encountered; skipping");
                continue;
            }

            // Decode the hextet using a .hex record decoder state machine
            switch(state)
            {
                case LENGTH0:
                    hexRecordLen = b & 0x0F;
                    state = HexDecoderState.LENGTH1;
                    break;
                case LENGTH1:
                    hexRecordLen <<= 4;
                    hexRecordLen |= b & 0x0F;
                    computedRecordChecksum = (computedRecordChecksum + hexRecordLen) & 0xFF;
                    state = HexDecoderState.ADDRESS0;
                    break;

                case ADDRESS0:
                    hexRecordAddress = b & 0x0F;
                    state = HexDecoderState.ADDRESS1;
                    break;
                case ADDRESS1:
                    hexRecordAddress <<= 4;
                    hexRecordAddress |= b & 0x0F;
                    computedRecordChecksum = (computedRecordChecksum + hexRecordAddress) & 0xFF;
                    state = HexDecoderState.ADDRESS2;
                    break;
                case ADDRESS2:
                    hexRecordAddress <<= 4;
                    hexRecordAddress |= b & 0x0F;
                    state = HexDecoderState.ADDRESS3;
                    break;
                case ADDRESS3:
                    hexRecordAddress <<= 4;
                    hexRecordAddress |= b & 0x0F;
                    computedRecordChecksum = (computedRecordChecksum + hexRecordAddress) & 0xFF;
                    state = HexDecoderState.TYPE0;
                    break;

                case TYPE0:
                    hexRecordType = b & 0x0F;
                    state = HexDecoderState.TYPE1;
                    break;
                case TYPE1:
                    hexRecordType <<= 4;
                    hexRecordType |= b & 0x0F;
                    hexRecordDataIndex = 0;
                    computedRecordChecksum = (computedRecordChecksum + hexRecordType) & 0xFF;
                    byte recordData[] = new byte[hexRecordLen];
                    record = new DataRecord(((decoderAddress & 0xFFFF0000L) | hexRecordAddress), recordData, architecture16Bit);
                    state = HexDecoderState.DATA0;
                    if(hexRecordLen == 0)
                    {
                        state = HexDecoderState.CHECK0;
                    }
                    break;

                case DATA0:
                    hexRecordData[hexRecordDataIndex] = b;
                    state = HexDecoderState.DATA1;
                    break;
                case DATA1:
                    hexRecordData[hexRecordDataIndex] <<= 4;
                    hexRecordData[hexRecordDataIndex] |= b;
                    record.data[hexRecordDataIndex] = hexRecordData[hexRecordDataIndex];
                    computedRecordChecksum = (computedRecordChecksum + (hexRecordData[hexRecordDataIndex] & 0xFF)) & 0xFF;
                    hexRecordDataIndex++;
                    state = HexDecoderState.DATA0;
                    if(hexRecordDataIndex == hexRecordLen)
                    {
                        state = HexDecoderState.CHECK0;
                    }
                    break;

                case CHECK0:
                    hexRecordChecksum = b & 0x0F;
                    state = HexDecoderState.CHECK1;
                    break;
                case CHECK1:
                    hexRecordChecksum <<= 4;
                    hexRecordChecksum |= b & 0x0F;
                    computedRecordChecksum = (-computedRecordChecksum) & 0xFF;
                    if(hexRecordChecksum != computedRecordChecksum)
                    {
                        System.err.println("    EZBL: Error reading .hex file: record checksum failure; using bad data anyway");
                    }
                    computedRecordChecksum = 0;
                    state = HexDecoderState.COLON;

                    // Process the record
                    switch(hexRecordType)
                    {
                        case 0: // Data Record
                            // Save data record contents in list
                            records.add(record);

                            // Add to our address counter, but do not change the upper 16 bits, which are tied to linear extended addressing records
                            decoderAddress = (decoderAddress & 0xFFFF0000L) | ((decoderAddress + hexRecordLen) & 0xFFFFL);
                            break;

                        case 1: // End-of-File Record
                            if(inIndex + 12 < hexFileContents.length)
                            {
                                System.err.println("    EZBL: Error reading .hex file: found EOF record with more data after it; going to try and process extra data");
                            }
                            if(hexRecordLen != 0)
                            {
                                System.err.println("    EZBL: Error reading .hex file: found EOF record with data in it; throwing extra data away");
                            }
                            break;

                        case 2: // HEX386 Extended Segment Address Record (additional address to add left shifted by 4 bits (16-bits))
                            hexHEX386Address = ((hexRecordData[0] & 0xFFL) << 12) | ((hexRecordData[1] & 0xFFL) << 4);
                            if(hexRecordLen != 2)
                            {
                                System.err.println("    EZBL: Error reading .hex file: found extra data in an Extended Segment Address Record; throwing extra data away");
                            }
                            break;

                        case 4: // Extended Linear Address Record (high 16-bits of address)
                            decoderAddress = (decoderAddress & 0x0000FFFF) | ((hexRecordData[0] & 0xFFL) << 24) | ((hexRecordData[1] & 0xFFL) << 16);
                            if(hexRecordLen != 2)
                            {
                                System.err.println("    EZBL: Error reading .hex file: found extra data in an Extended Linear Address Record; throwing extra data away");
                            }
                            break;

                        case 5: // Start Linear Address Record
                            System.err.println("    EZBL: Error reading .hex file: Start Linear Address Records not supported; skipping record");
                            break;

                        default:
                            System.err.println("    EZBL: Error reading .hex file: unrecognized record type; skipping record");
                            break;
                    }
                    hexRecordDataIndex = 0;
                    break;
            }
        }

        if(state != HexDecoderState.COLON)
        {
            System.err.println("    EZBL: Error reading .hex file: ran out of data in the middle of a record; discarding record fragment");
        }
        if(hexRecordType != 1)
        {
            System.err.println("    EZBL: Error reading .hex file: missing EOF record; continuing anyway");
        }

        return records;
    }

    // Parses an Intel .hex file and returns a list of Data Records that were in
    // the hex file. Takes input as a String containing all of the data (not a
    // file path!)
    static List<DataRecord> ParseHex(String hexFileContents, boolean architecture16Bit)
    {
        // Convert string to byte array using default system character encoding
        return ParseHex(hexFileContents.getBytes(), architecture16Bit);
    }

    public static byte[] EncryptBlobBytes(EZBLState state, byte blobBytes[])
    {
        Cipher aes;
        SecretKeySpec key;
        byte counterBytes[] = new byte[16];
        byte keySteamBytes[];
        int chunkSize;
        long blobIndex;
        byte encryptedBytes[] = new byte[blobBytes.length];
        int i;

        // Get a copy of the non-encrypted blob bytes
        System.arraycopy(blobBytes, 0, encryptedBytes, 0, encryptedBytes.length);

        // Create the correct starting Counter + Nonce
        System.arraycopy(encryptedBytes, 8, counterBytes, 4, 12);

        // Start up the Java AES engine
        try
        {
            aes = Cipher.getInstance("AES/ECB/NoPadding");
            key = new SecretKeySpec(state.encryptionKey, "AES");
            aes.init(Cipher.ENCRYPT_MODE, key);
        }
        catch(NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException ex)
        {
            return null;
        }

        blobIndex = 28; // Start encryption just after the 28-byte blob header (4 bytes Blob Length + 4 byte Address/Type + 20 bytes SHA-1 Hash)
        int blockIndex = (int)(blobIndex / 16);
        int blockOffset = (int)(blobIndex % 16);
        while(blockIndex * 16 < encryptedBytes.length)
        {
            // Compute 16 bytes of key stream corresponding to the block's
            // counter location
            counterBytes[0] = (byte)blockIndex;
            counterBytes[1] = (byte)(blockIndex >> 8);
            counterBytes[2] = (byte)(blockIndex >> 16);
            counterBytes[3] = (byte)(blockIndex >> 24);
            try
            {
                keySteamBytes = aes.doFinal(counterBytes);
            }
            catch(IllegalBlockSizeException | BadPaddingException ex)
            {
                return null;
            }

            // Encrypt all 16-byte blocks of blob data with the appropriate
            // counter value
            chunkSize = keySteamBytes.length;
            if((blockIndex * 16) + blockOffset + chunkSize > encryptedBytes.length)
            {
                chunkSize = encryptedBytes.length - (blockIndex * 16) - blockOffset;
            }

            for(i = blockOffset; i < chunkSize; i++)
            {
                encryptedBytes[blockIndex * 16 + i] ^= keySteamBytes[i];
            }
            blockIndex++;
            blockOffset = 0;
        }

        // Compute new CRC32 in last 4 bytes for communications verification.
        // Compute expected value with last 4 byte CRC field EXCLUDED from the
        // computation (i.e. not included as zeros). When we place the CRC there
        // and someone computes the CRC over the whole blob, they'll always get
        // the magic 'm' CRC value of 0x2144DF1C out if the data wasn't damaged
        // (same result as if you computed CRC32(0)).
        CRC32 myCRC = new CRC32();
        myCRC.update(encryptedBytes, 0, encryptedBytes.length - 4);
        long crcValue = myCRC.getValue();
        encryptedBytes[encryptedBytes.length - 4] = (byte)((crcValue) & 0xFF);
        encryptedBytes[encryptedBytes.length - 3] = (byte)((crcValue >> 8) & 0xFF);
        encryptedBytes[encryptedBytes.length - 2] = (byte)((crcValue >> 16) & 0xFF);
        encryptedBytes[encryptedBytes.length - 1] = (byte)((crcValue >> 24) & 0xFF);

        return encryptedBytes;
    }

    /**
     * Returns a human readable text representation of a .blob/.bl2 file's
     * contents.
     *
     * Useful for debugging, blob analysis, easily checking what's in a .hex
     * file (by converting the .hex to a .blob, then a using this dump
     * function), or learning about the .blob structure for implementation.
     */
    public static String GetObjDumpView(byte blobBytes[], boolean architecture16Bit)
    {
        String header = "";
        String expectedHash = "";
        long size;
        long typeField;
        String typeFieldString;
        int byteIndex = 0;
        int recordCount = 0;
        byte hash[] = new byte[20];
        long metaLen = 0, metaType = 0;
        String metaTypeString = null;
        int bootID[] = new int[8];
        int appIDVerMajor = 0;
        int appIDVerMinor = 0;
        int appIDVerBuild = 0;
        long expectedCRC, readCRC;
        List<String> output = new ArrayList<String>();

        size = (((long)blobBytes[byteIndex++]) & 0xFF);
        size |= (((long)blobBytes[byteIndex++]) & 0xFF) << 8;
        size |= (((long)blobBytes[byteIndex++]) & 0xFF) << 16;
        size |= (((long)blobBytes[byteIndex++]) & 0xFF) << 24;

        typeField = (((long)blobBytes[byteIndex++]) & 0xFF);
        typeField |= (((long)blobBytes[byteIndex++]) & 0xFF) << 8;
        typeField |= (((long)blobBytes[byteIndex++]) & 0xFF) << 16;
        typeField |= (((long)blobBytes[byteIndex++]) & 0xFF) << 24;
        typeFieldString = String.format("%1$c%2$c%3$c%4$c", blobBytes[byteIndex - 4], blobBytes[byteIndex - 3], blobBytes[byteIndex - 2], blobBytes[byteIndex - 1]);

        String hashString = "";
        for(int i = 0; i < 20; i += 4)
        {
            hashString += String.format("%1$02X%2$02X%3$02X%4$02X", blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++]);
        }

        if(typeFieldString.equals("BL2B"))
        {
            metaLen = (((int)blobBytes[byteIndex++]) & 0xFF) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 8) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 16) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 24);
            metaType = (((int)blobBytes[byteIndex++]) & 0xFF) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 8) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 16) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 24);
            metaTypeString = String.format("%1$c%2$c%3$c%4$c", blobBytes[byteIndex - 4], blobBytes[byteIndex - 3], blobBytes[byteIndex - 2], blobBytes[byteIndex - 1]);
            for(int i = 0; i < bootID.length; i++)
            {
                bootID[i] = (((int)blobBytes[byteIndex++]) & 0xFF) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 8) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 16) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 24);
            }
            appIDVerBuild = (((int)blobBytes[byteIndex++]) & 0xFF) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 8) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 16) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 24);
            appIDVerMinor = (((int)blobBytes[byteIndex++]) & 0xFF) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 8);
            appIDVerMajor = (((int)blobBytes[byteIndex++]) & 0xFF) | ((((int)blobBytes[byteIndex++]) & 0xFF) << 8);
        }

        readCRC = (((long)blobBytes[blobBytes.length - 4]) & 0xFF);
        readCRC |= (((long)blobBytes[blobBytes.length - 3]) & 0xFF) << 8;
        readCRC |= (((long)blobBytes[blobBytes.length - 2]) & 0xFF) << 16;
        readCRC |= (((long)blobBytes[blobBytes.length - 1]) & 0xFF) << 24;


        // Compute SHA-1 hash
        try
        {
            MessageDigest hashComputer = MessageDigest.getInstance("SHA-1");
            hashComputer.update(blobBytes, 0, 8);
            hashComputer.update(hash);              // Need 20 zeros where the Hash normally goes
            hashComputer.update(blobBytes, 28, blobBytes.length - 28 - 4);
            hashComputer.update(hash, 0, 4);        // Need 4 more zeros where the CRC32 footer goes
            hash = hashComputer.digest();
            for(int i = 0; i < 20; i += 4)
            {
                expectedHash += String.format("%1$02X%2$02X%3$02X%4$02X", hash[i], hash[i + 1], hash[i + 2], hash[i + 3]);
            }
        }
        catch(NoSuchAlgorithmException ex)
        {
            System.err.println("    EZBL: ERROR! Can't find 'SHA-1' hash algorithm. Make sure your JRE includes SHA-1 support. Output hash will be set to all zeros.");
        }

        // Compute CRC32 of all bytes except the 4 byte CRC footer field, which should match this value
        CRC32 myCRC = new CRC32();
        myCRC.update(blobBytes, 0, blobBytes.length - 4);
        expectedCRC = myCRC.getValue();

        header += String.format("Blob file length:      0x%1$08X (%1$d)\n", blobBytes.length)
                  + String.format("Computed hash:         %1$s\n", expectedHash)
                  + String.format("Computed CRC:          0x%1$08X\n", expectedCRC)
                  + "\n"
                  + String.format("Blob header.length:    0x%1$08X (%1$d) (%2$s)\n", size, size == blobBytes.length ? "Correct" : "INVALID")
                  + String.format("Blob header.flags:     0x%1$08X (\"%2$s\")\n", typeField, typeFieldString)
                  + String.format("Blob header.hash:      %1$s (%2$s)\n", hashString, hashString.equals(expectedHash) ? "Correct" : "INVALID");
        if(metaTypeString != null)
        {
            header += "\n"
                      + String.format("Blob meta.length:      0x%1$08X (%1$d) (%2$s)\n", metaLen, metaLen == 8 + bootID.length * 4 + 8 ? "Correct" : "INVALID")
                      + String.format("Blob meta.type:        0x%1$08X (\"%2$s\") (%3$s)\n", metaType, metaTypeString, metaTypeString.equals("META") ? "Correct" : "INVALID")
                      + String.format("Blob meta.BOOTID:      %1$08X%2$08X%3$08X%4$08X%5$08X%6$08X%7$08X%8$08X\n", bootID[7], bootID[6], bootID[5], bootID[4], bootID[3], bootID[2], bootID[1], bootID[0])
                      + String.format("Blob meta.APPID_VER:   %1$d.%2$02d.%3$04d\n", appIDVerMajor, appIDVerMinor, appIDVerBuild);
        }
        header += "\n"
                  + String.format("Blob footer CRC32:     0x%1$08X (%2$s)\n", readCRC, readCRC == expectedCRC ? "Correct" : "INVALID")
                  + "\n";

        // Do a quick enumeration of record headers to try and determine if this can't be a 16-bit .blob/.bl2
        if(architecture16Bit)
        {
            int searchByteIndex = byteIndex;
            while(searchByteIndex < blobBytes.length - 4)
            {
                long recordLength;
                long recordAddress;

                if(searchByteIndex + 8 > blobBytes.length - 4)
                {
                    break;
                }
                recordLength = ((long)blobBytes[searchByteIndex++]) & 0xFF;
                recordLength |= (((long)blobBytes[searchByteIndex++]) & 0xFF) << 8;
                recordLength |= (((long)blobBytes[searchByteIndex++]) & 0xFF) << 16;
                recordLength |= (((long)blobBytes[searchByteIndex++]) & 0xFF) << 24;
                recordAddress = ((long)blobBytes[searchByteIndex++]) & 0xFF;
                recordAddress |= (((long)blobBytes[searchByteIndex++]) & 0xFF) << 8;
                recordAddress |= (((long)blobBytes[searchByteIndex++]) & 0xFF) << 16;
                recordAddress |= (((long)blobBytes[searchByteIndex++]) & 0xFF) << 24;

                if((recordAddress & 0xFF000000) != 0)
                {
                    architecture16Bit = false;  // 16-bit devices only have 24-bit address space
                    break;
                }
                searchByteIndex += recordLength - 8;
            }
        }

        header += "Starting record decoding in " + (architecture16Bit ? "16-bit" : "8-bit/32-bit") + " architecture mode\n"
                  + "\n";

        while(byteIndex < blobBytes.length - 4)
        {
            long recordLength;
            long recordAddress;
            int i;

            if(byteIndex + 8 > blobBytes.length - 4)
            {
                output.add(String.format("    ERROR: found %1$d bytes at the end of the file. This is too small to be a record header.\n", blobBytes.length - 4 - byteIndex));
                output.add("    Bytes are: ");
                for(i = 0; i < blobBytes.length - 4 - byteIndex; i++)
                {
                    output.add(String.format("%1$02X", blobBytes[byteIndex + i]));
                }
                output.add("\n");
                break;
            }

            recordLength = ((long)blobBytes[byteIndex++]) & 0xFF;
            recordLength |= (((long)blobBytes[byteIndex++]) & 0xFF) << 8;
            recordLength |= (((long)blobBytes[byteIndex++]) & 0xFF) << 16;
            recordLength |= (((long)blobBytes[byteIndex++]) & 0xFF) << 24;

            recordAddress = ((long)blobBytes[byteIndex++]) & 0xFF;
            recordAddress |= (((long)blobBytes[byteIndex++]) & 0xFF) << 8;
            recordAddress |= (((long)blobBytes[byteIndex++]) & 0xFF) << 16;
            recordAddress |= (((long)blobBytes[byteIndex++]) & 0xFF) << 24;

            recordCount++;

            output.add(String.format("Record header.length:  0x%1$08X (8 header + %2$d data bytes)\n", recordLength, recordLength - 8));
            output.add(String.format("Record header.address: 0x%1$08X (to 0x%2$08X)\n", recordAddress, recordAddress + (architecture16Bit ? ((recordLength - 8 + 2) / 3 * 2) : recordLength - 8)));
            recordLength -= 8;  // Subtract off header length for subsequent convenience
            if((int)recordLength + byteIndex > blobBytes.length - 4)
            {
                output.add("    ERROR: record length indicator extends into CRC area\n");
                recordLength = blobBytes.length - 4 - byteIndex;
            }
            else if(recordLength < 0)
            {
                output.add("    ERROR: record length is invalid (computed record end address will be wrong)\n");
                recordLength = blobBytes.length - 4 - byteIndex;
            }

            if(architecture16Bit)
            {
                for(i = 0; i + 12 <= (int)recordLength; i += 12)
                {
                    output.add(String.format("%1$06X  %4$02X%3$02X%2$02X %7$02X%6$02X%5$02X %10$02X%9$02X%8$02X %13$02X%12$02X%11$02X\n", recordAddress, blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++]));
                    recordAddress += 0x8;
                }
                if(i < recordLength)
                {
                    output.add(String.format("%1$06X  ", recordAddress));
                    for(; i + 3 <= (int)recordLength; i += 3)
                    {
                        output.add(String.format("%3$02X%2$02X%1$02X ", blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++]));
                    }
                    if((int)recordLength - i == 2)
                    {
                        output.add(String.format("  %2$02X%1$02X", blobBytes[byteIndex++], blobBytes[byteIndex++]));
                    }
                    else if((int)recordLength - i == 1)
                    {
                        output.add(String.format("    %1$02X", blobBytes[byteIndex++]));
                    }

                    output.add("\n");
                }
            }
            else    // 8-bit or 32-bit mode
            {
                for(i = 0; i + 16 <= (int)recordLength; i += 16)
                {
                    output.add(String.format("%1$08X  ", recordAddress));
                    output.add(String.format("%4$02X%3$02X%2$02X%1$02X %8$02X%7$02X%6$02X%5$02X %12$02X%11$02X%10$02X%9$02X %16$02X%15$02X%14$02X%13$02X\n", blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++]));
                    recordAddress += 0x10;
                }
                if(i < recordLength)
                {
                    output.add(String.format("%1$08X  ", recordAddress));
                    for(; i + 4 <= (int)recordLength; i += 4)
                    {
                        output.add(String.format("%4$02X%3$02X%2$02X%1$02X ", blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++]));
                    }
                    if((int)recordLength - i == 3)
                    {
                        output.add(String.format("  %3$02X%2$02X%1$02X", blobBytes[byteIndex++], blobBytes[byteIndex++], blobBytes[byteIndex++]));
                    }
                    else if((int)recordLength - i == 2)
                    {
                        output.add(String.format("    %2$02X%1$02X", blobBytes[byteIndex++], blobBytes[byteIndex++]));
                    }
                    else if((int)recordLength - i == 1)
                    {
                        output.add(String.format("      %1$02X", blobBytes[byteIndex++]));
                    }

                    output.add("\n");
                }
            }
            output.add("\n");
        }

        header += String.format("Found Records: %1$d\n", recordCount)
                  + "\n";
        output.add(0, header);
        output.add(String.format("CRC32: 0x%1$08X\n", readCRC));

        return CatStringList(output);
    }


    // Returns a 20 byte SHA-1 hash of the blob contents, including all record
    // data, record headers, and the blob headers. The only data excluded is the
    // Blob header hash field and trailing CRC32, which is treated as all zeros
    // for hash computation.
    public byte[] GetHash()
    {
        return Arrays.copyOfRange(GetBytes(), 8, 28);
    }

    /*
     * Adds more data at a specified address to the blob. If the blob already
     * contains data in all or part of the required address range, the
     * respective addresses are overwritten with the new data.
     *
     * All blob records are sorted and coalesced upon return.
     */
    public void AddData(long address, byte data[])
    {
        DataRecord newData = new DataRecord(address, data, architecture16Bit);
        AddressRange range = new AddressRange(newData.address, newData.GetEndAddress());

        RemoveIgnoreRange(range);                   // Remove any records or partial rec data that overlaps with this new range
        this.records.add(newData);                  // Add the new rec
        DataRecord.CoalesceRecords(this.records);   // Sort and remove wasted rec boundaries
    }

    /**
     * Fills all gaps within the specified address range with filler character
     * specified. Preexisting data within the range is preserved. If there is no
     * preexisting data in the specified range, then the entire range is filled.
     *
     * All data records are sorted and coalesced upon return.
     */
    public void FillData(AddressRange range, byte fillChar)
    {
        // Create a temporary blob containing only filler
        DataRecord newData = new DataRecord(architecture16Bit);
        newData.address = range.startAddress;
        newData.data = new byte[newData.GetDataIndexOfAddress(range.endAddress)];
        Arrays.fill(newData.data, fillChar);
        Blob tempBlob = new Blob(architecture16Bit);
        tempBlob.records.add(newData);


        // Now search through all records and compose a list of conflicting
        // address ranges
        List<AddressRange> conflicts = new ArrayList<AddressRange>();
        for(int i = 0; i < records.size(); i++)
        {
            DataRecord rec = records.get(i);
            if(rec.GetEndAddress() <= range.startAddress)    // No interference
            {
                continue;
            }
            if(rec.address >= range.endAddress)    // No interference
            {
                continue;
            }

            // Something conflicts, add it as something to subtract
            conflicts.add(rec.GetAddressRange());
        }

        // Remove all the conflicting (preexisting data) ranges from the tempBlob
        tempBlob.RemoveIgnoreRanges(conflicts);

        // Copy all the remaining tempBlob filler rec(s) (if any) to the main blob
        this.records.addAll(tempBlob.records);

        // Sort and remove wasted rec boundaries
        DataRecord.CoalesceRecords(this.records);
    }


    /**
     * Gets the total blob contents in serialized, ordered form, and with all
     * headers and hashing included + the 4 byte trailing CRC.
     */
    public byte[] GetBytes()
    {
        byte retData[];
        int index;
        DataRecord record;
        ListIterator<DataRecord> iterator;

        // Find the total length of the output array
        index = 0;
        iterator = records.listIterator();
        while(iterator.hasNext())
        {
            record = iterator.next();
            index += record.data.length + 8;   // 8 is the blob's record header length
        }

        // Allocate memory for a 28 byte Blob header, the data records, and the 4 byte trailing CRC32
        retData = new byte[28 + index + 4];

        // Write blob length (4 bytes) + type (4 bytes) + SHA-1 hash (20 bytes) header
        retData[0] = (byte)((retData.length) & 0xFF);               // 4 for this length, 4 for the "BLOB" type field, and 20 for a SHA1 hash of this blob, including everything but assuming all 20 hash bytes are 0x00
        retData[1] = (byte)(((retData.length) >> 8) & 0xFF);
        retData[2] = (byte)(((retData.length) >> 16) & 0xFF);
        retData[3] = (byte)(((retData.length) >> 24) & 0xFF);
        retData[4] = (byte)(this.typeCode & 0xFF);              //'B'
        retData[5] = (byte)(this.typeCode >> 8 & 0xFF);         //'L'
        retData[6] = (byte)(this.typeCode >> 16 & 0xFF);        //'2' for .bl2 format (or 'O' for .blob format)
        retData[7] = (byte)(this.typeCode >> 24 & 0xFF);        //'B'

        index = 28; // Skip over the 20 byte SHA-1 file hash for now

        // Copy all record data into the array
        iterator = records.listIterator();
        while(iterator.hasNext())
        {
            // Get a record
            record = iterator.next();

            // Create a blob record header containing 4 byte Record Length + 4 byte Record Start address
            retData[index++] = (byte)(((record.data.length + 8) & 0x000000FFL));
            retData[index++] = (byte)(((record.data.length + 8) & 0x0000FF00L) >> 8);
            retData[index++] = (byte)(((record.data.length + 8) & 0x00FF0000L) >> 16);
            retData[index++] = (byte)(((record.data.length + 8) & 0xFF000000L) >> 24);
            retData[index++] = (byte)((record.address & 0x000000FFL));
            retData[index++] = (byte)((record.address & 0x0000FF00L) >> 8);
            retData[index++] = (byte)((record.address & 0x00FF0000L) >> 16);
            retData[index++] = (byte)((record.address & 0xFF000000L) >> 24);

            // Copy record data
            System.arraycopy(record.data, 0, retData, index, record.data.length);
            index += record.data.length;
        }

        // Compute SHA-1 hash (with the CRC32 field included, but still set to 0x00000000)
        try
        {
            MessageDigest hashComputer = MessageDigest.getInstance("SHA-1");
            hashComputer.update(retData);
            hashComputer.digest(retData, 8, 20);
        }
        catch(NoSuchAlgorithmException ex)
        {
            System.err.println("ezbl_tools: Cannot find 'SHA-1' hash algorithm. Make sure your JRE includes SHA-1 support. Output hash will be set to all zeros.");
        }
        catch(DigestException ex)
        {
            System.err.printf("ezbl_tools: SHA-1 exception: %1$s\n", ex.getMessage());
        }

        // Finally add the CRC32 for communications verification purposes
        // Compute expected value with last 4 byte CRC field EXCLUDED from the
        // computation (i.e. not included as zeros). When we place the CRC there
        // and someone computes the CRC over the whole blob, they'll always get
        // the magic 'm' CRC value of 0x2144DF1C out if the data wasn't damaged
        // (same result as if you computed CRC32(0)).
        CRC32 myCRC = new CRC32();
        myCRC.update(retData, 0, retData.length - 4);
        long crcValue = myCRC.getValue();
        retData[retData.length - 4] = (byte)((crcValue) & 0xFF);
        retData[retData.length - 3] = (byte)((crcValue >> 8) & 0xFF);
        retData[retData.length - 2] = (byte)((crcValue >> 16) & 0xFF);
        retData[retData.length - 1] = (byte)((crcValue >> 24) & 0xFF);

        return retData;
    }

    /**
     * Gets the the binary contents of all data records, sorted by internal
     * record list order and with addressing information stripped out. No
     * hashing, CRC or other data is included.
     */
    public byte[] GetBinBytes()
    {
        byte retData[];
        int index;

        // Find the total length of the output array
        int byteLen = 0;
        for(DataRecord r : records)
        {
            if(r.data == null)
            {
                continue;
            }
            byteLen += r.data.length;
        }

        // Allocate RAM for all data bytes
        retData = new byte[byteLen];
        index = 0;

        // Build the return data array as a copy of all data record contents
        for(DataRecord r : records)
        {
            if(r.data == null)
            {
                continue;
            }
            System.arraycopy(r.data, 0, retData, index, r.data.length);
            index += r.data.length;
        }

        return retData;
    }


    /*
     * Returns the number of address elements needed to represent the given
     * length in bytes. This performs a divide by 3 and multiply by 2 operation
     * with padding if needed to align to the 0x2 address minimum instruction
     * word size.
     */
    static long BytesToMCU16Addresses(long lengthInBytes)
    {
        // 3 bytes / 2 addresses
        long ret = (lengthInBytes * 2 + 2) / 3;
        if((ret & 0x1) != 0)   // Addresses must also be even
        {
            ret++;
        }
        return ret;
    }


    private void RemoveMCU16PhantomBytesAndAddresses()
    {
        int destIndex;
        DataRecord record;
        byte newData[];
        ListIterator<DataRecord> iterator;

        iterator = records.listIterator();

        while(iterator.hasNext())
        {
            record = iterator.next();

            // Remove all phantom bytes from the data
            newData = new byte[(record.data.length * 3 + 3) / 4];
            destIndex = 0;
            for(int i = 0; i < record.data.length; i++)
            {
                if(((record.address + i) & 0x3) == 0x3)
                {
                    continue;
                }
                newData[destIndex++] = record.data[i];
            }

            // Assign new values to original list element
            record.address >>= 1;
            record.data = newData;
            iterator.set(record);
        }
    }

    /*
     * Deletes all data from the specified address range, if present. The
     * individual data records that contain something within the ignore range
     * are deleted or split as needed to retain all data outside the
     * keepRange.
     *
     * Upon return, all data records are sorted and coalesced.
     */
    public void RemoveIgnoreRange(AddressRange ignoreRange)
    {
        List<AddressRange> ignoreRanges = new ArrayList<AddressRange>();
        ignoreRanges.add(ignoreRange);
        RemoveIgnoreRanges(ignoreRanges);
    }

    public void RemoveIgnoreRangeByDR(DataRecord ignoreRecordRange)
    {
        List<AddressRange> ignoreRanges = new ArrayList<AddressRange>();
        ignoreRanges.add(new AddressRange(ignoreRecordRange.address, ignoreRecordRange.GetEndAddress()));
        RemoveIgnoreRanges(ignoreRanges);
    }

    public void RemoveIgnoreRangesByDR(List<DataRecord> ignoreRecordRanges)
    {
        List<AddressRange> ignoreRanges = new ArrayList<AddressRange>();
        for(DataRecord dr : ignoreRecordRanges)
        {
            ignoreRanges.add(new AddressRange(dr.address, dr.GetEndAddress()));
        }
        RemoveIgnoreRanges(ignoreRanges);
    }


    /*
     * Deletes all data from the specified address ranges, if present. The
     * individual data records that contain something within the ignore ranges
     * are deleted or split as needed to retain all data outside the
     * ignoreRanges.
     *
     * Upon return, all data records are sorted and coalesced.
     */
    public void RemoveIgnoreRanges(List<AddressRange> ignoreRanges)
    {
        DataRecord rec;
        DataRecord newRecordRight;
        byte newData[];

        if(ignoreRanges == null)
        {
            return;
        }
        if(ignoreRanges.isEmpty())
        {
            return;
        }

        // For speed, sort the records and ranges by address
        Collections.sort(records);
        Collections.sort(ignoreRanges);

        for(int i = 0; i < records.size(); i++)
        {
            rec = records.get(i);

            for(AddressRange ignoreRange : ignoreRanges)
            {
                // See if the whole record is within the ignore range and we can delete it all
                if((rec.address >= ignoreRange.startAddress) && (rec.GetEndAddress() <= ignoreRange.endAddress))
                {
                    records.remove(i);
                    i--;
                    break;
                }

                // See if no part of this record exists in the selected ignore range
                if((rec.GetEndAddress() < ignoreRange.startAddress) || (rec.address >= ignoreRange.endAddress))
                {
                    continue;
                }

                // Some overlap exists, decode if we need to trim the left, right, or split into two records (ignore range eats data in the middle of the record)
                if((rec.address <= ignoreRange.startAddress) && (rec.GetEndAddress() >= ignoreRange.endAddress))
                {   // good data ... ignore range ... good data
                    newData = new byte[rec.data.length - rec.GetDataIndexOfAddress(ignoreRange.endAddress)];
                    System.arraycopy(rec.data, rec.GetDataIndexOfAddress(ignoreRange.endAddress), newData, 0, newData.length);
                    newRecordRight = new DataRecord(ignoreRange.endAddress, newData, architecture16Bit);
                    records.add(i + 1, newRecordRight);

                    newData = new byte[rec.GetDataIndexOfAddress(ignoreRange.startAddress)];
                    System.arraycopy(rec.data, 0, newData, 0, newData.length);
                    rec.data = newData;
                    continue;
                }
                else if(rec.address >= ignoreRange.startAddress)
                {   // ignore range ... good data
                    newData = new byte[rec.data.length - rec.GetDataIndexOfAddress(ignoreRange.endAddress)];
                    System.arraycopy(rec.data, rec.GetDataIndexOfAddress(ignoreRange.endAddress), newData, 0, newData.length);
                    rec.data = newData;
                    rec.address = ignoreRange.endAddress;
                    continue;
                }
                else
                {   // good data ... ignore range
                    newData = new byte[rec.GetDataIndexOfAddress(ignoreRange.startAddress)];
                    System.arraycopy(rec.data, 0, newData, 0, newData.length);
                    rec.data = newData;
                    continue;
                }
            }
        }

        // Removing ranges could result in new coalescing opportunites. Coalesce
        // them now.
        DataRecord.CoalesceRecords(this.records);
    }


//    /*
//     * Deletes all data which lies outside the specified address ranges. Any
//     * individual data records that stradle a keep range is split as needed to
//     * retain all data inside the rangesToKeep while throwing away that which
//     * isn
//     * 't.
//     *
//     * Upon return, all data records are sorted and coalesced.
//     */
//    public void RemoveDataOutsideRanges(List<AddressRange> rangesToKeep)
//    {
//        DataRecord rec;
//        DataRecord newRecordRight;
//        byte newData[];
//        List<DataRecord> recKept;
//
//        if(rangesToKeep == null)
//        {
//            this.records = null;
//            return;
//        }
//        if(rangesToKeep.isEmpty())
//        {
//            this.records.clear();
//            return;
//        }
//
//        // For speed, sort the records and ranges by address
//        Collections.sort(records);
//        Collections.sort(rangesToKeep);
//
//        for(int i = 0; i < records.size(); i++)
//        {
//            rec = records.get(i);
//
//            // Start out assuming the whole data record will get killed
//
//
//            // Iterate through the keep ranges to determine what can be kept
//            for(AddressRange keepRange : rangesToKeep)
//            {
//                // See if the whole record is within the keep range and we can abort searching
//                if((rec.address >= keepRange.startAddress) && (rec.GetEndAddress() <= keepRange.endAddress))
//                {
//                    records.remove(i);
//                    i--;
//                    break;
//                }
//
//                // See if no part of this record exists in the selected keep range
//                if((rec.GetEndAddress() < keepRange.startAddress) || (rec.address >= keepRange.endAddress))
//                {
//                    continue;
//                }
//
//                // Some overlap exists, decode if we need to trim the left, right, or split into two records (keep range eats data in the middle of the record)
//                if((rec.address <= keepRange.startAddress) && (rec.GetEndAddress() >= keepRange.endAddress))
//                {   // bad data ... keep range ... bad data
//                    newData = new byte[rec.data.length - rec.GetDataIndexOfAddress(keepRange.endAddress)];
//                    System.arraycopy(rec.data, rec.GetDataIndexOfAddress(keepRange.endAddress), newData, 0, newData.length);
//                    newRecordRight = new DataRecord(keepRange.endAddress, newData, architecture16Bit);
//                    records.add(i + 1, newRecordRight);
//
//                    newData = new byte[rec.GetDataIndexOfAddress(keepRange.startAddress)];
//                    System.arraycopy(rec.data, 0, newData, 0, newData.length);
//                    rec.data = newData;
//                    continue;
//                }
//                else if(rec.address >= keepRange.startAddress)
//                {   // keep range ... bad data
//                    newData = new byte[rec.data.length - rec.GetDataIndexOfAddress(keepRange.endAddress)];
//                    System.arraycopy(rec.data, rec.GetDataIndexOfAddress(keepRange.endAddress), newData, 0, newData.length);
//                    rec.data = newData;
//                    rec.address = keepRange.endAddress;
//                    continue;
//                }
//                else
//                {   // bad data ... keep range
//                    newData = new byte[rec.GetDataIndexOfAddress(keepRange.startAddress)];
//                    System.arraycopy(rec.data, 0, newData, 0, newData.length);
//                    rec.data = newData;
//                    continue;
//                }
//            }
//        }
//
//        // Removing ranges could result in new coalescing opportunites. Coalesce
//        // them now.
//        DataRecord.CoalesceRecords(this.records);
//    }
    /*
     * Coalesceces adjacent data records wherever possible. I.e. if one data
     * record ends at the same address that another record starts on, the two
     * data records are combined into a single longer record. All data is
     * preserved.
     *
     * Upon return, all data records are sorted and maximally coalesced.
     */
    public void CoalesceAdjacentDataRecords()
    {
        DataRecord.CoalesceRecords(this.records);
    }

    /*
     * Sorts the data records into a more practical order for serial device
     * programming where random read access into the .blob is unavailable. For
     * example, Configuration Word values are placed before the .blob record
     * for address 0x000000 so that they can be programmed before programming
     * the main Flash array. Also, applicable to 16-bit architectures, any Test
     * Segment data is sorted to the beginning of the array so things like FBOOT
     * or a programming executive could be programmed first.
     */
    public void ReorderForSequentialAccess()
    {
        DataRecord dr;
        List<DataRecord> orderFirstElements = new ArrayList<DataRecord>();

        // Do nothing for non-16-bit architectures. FBOOT in is really the only critical thing to be reordered right now.
        if(!architecture16Bit)
        {
            return;
        }

        // Enumerate all data records, looking for fuses and test segment data
        for(int i = 0; i < this.records.size(); i++)
        {
            dr = this.records.get(i);

            // Check if this is a record starting in the Test Segment address range
            if(dr.architecture16Bit && ((dr.address & 0x800000) == 0x800000))
            {
                // Check if this is a Configuration Fuse item
                if((dr.address & 0xFFFF00) == 0xF80000)
                {
                    // Config bytes presently do not require reordering
                }
                else    // FBOOT or other test space data
                {
                    orderFirstElements.add(dr);
                    this.records.remove(i);
                    i--;    // Lost a record, don't increment i next round
                }
            }
        }

        // All done finding fuses, place the security ones back on, this time at the very end
        this.records.addAll(0, orderFirstElements);
    }

    public void AlignRecords()
    {
        // Do nothing for 8/32-bit architectures. Someone may be using this .blob for external SPI Flash programming or something, so we don't want to add 0xFF bytes without knowing what we are doing with it.
        if(!architecture16Bit)
        {
            return;
        }

        DataRecord.AlignRecords(this.records, this.architecture16Bit ? 4 : 16);
    }

    public void AlignRecords(int alignSize)
    {
        DataRecord.AlignRecords(this.records, (long)alignSize);
    }

}

