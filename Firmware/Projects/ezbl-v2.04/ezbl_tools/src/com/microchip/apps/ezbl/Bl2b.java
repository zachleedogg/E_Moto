/*
 *******************************************************************************
 * Copyright (c) 2017 Microchip Technology Inc. All rights reserved.
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

import static com.microchip.apps.ezbl.Blob.ParseHex;
import static com.microchip.apps.ezbl.Multifunction.CatStringList;
import static com.microchip.apps.ezbl.Multifunction.ReadFileBinary;
import java.io.File;
import java.security.*;
import java.util.*;
import java.util.zip.*;
import javax.crypto.*;
import javax.crypto.spec.*;


/**
 *
 * @author C12128
 */
public class Bl2b
// Do not serialize to get the .bl2 file normally. Use getBytes() (won't include encryption)
{
    boolean architecture16Bit;
    List<DataRecord> records = null;
    int[] bootIDHash = new int[4];
    int appIDVerBuild = 0;
    int appIDVerMinor = 0;
    int appIDVerMajor = 0;
    String encryptPassword = null;


    // Creates a default .bl2 class representation with no data in it
    public Bl2b()
    {
        records = new ArrayList<DataRecord>();
        this.architecture16Bit = false;
        this.encryptPassword = null;
    }

    public Bl2b(boolean architecture16Bit)
    {
        records = new ArrayList<DataRecord>();
        this.architecture16Bit = architecture16Bit;
    }

    public Bl2b(Blob v1Blob, int bootIDHash[], int appIDVerMajor, int appIDVerMinor, int appIDVerBuild)
    {
        this.records = v1Blob.records;
        this.architecture16Bit = v1Blob.architecture16Bit;
        if(bootIDHash != null && (bootIDHash.length >= 5))
        {
            int copyCount = this.bootIDHash.length > bootIDHash.length ? bootIDHash.length : this.bootIDHash.length;
            System.arraycopy(bootIDHash, 0, this.bootIDHash, 0, copyCount);
        }
        this.appIDVerBuild = appIDVerBuild;
        this.appIDVerMinor = appIDVerMinor;
        this.appIDVerMajor = appIDVerMajor;
    }

    /**
     * Initializes a Blob and internal data records from a preexisting .bl2 file
     * (passed in as a byte array).
     *
     * @param bl2Bytes           Array of bytes containing the whole .bl2,
     *                           including
     *                           synchronization string, all headers and hash + CRC32
     * @param encryptionPassword Password used to encrypt the bl2Bytes. If the
     *                           file is not encrypted, set to null.
     */
    public Bl2b(byte[] bl2Bytes, String encryptionPassword)
    {
        DataRecord record;
        int index;
        boolean invalidFile = false;

        if(bl2Bytes.length < 64 + 32 + 4)
        {
            invalidFile = true;
            throw new RuntimeException("Invalid bl2; length too small");
        }
        String firstBytes = new String(bl2Bytes, 0, 64);
        int BL2BIndex = firstBytes.indexOf("BL2B");
        invalidFile = BL2BIndex < 0;

        this.encryptPassword = encryptionPassword;
        this.architecture16Bit = false;
        this.records = new ArrayList<DataRecord>();
        index = 64; // Seek to data records
        if(BL2BIndex >= 0)  // Which if the sync field is a different length, needs to be based off the location of the "BL2B" string
        {
            index = BL2BIndex + 48;
        }
        while(index < bl2Bytes.length - 64 - 4 - 32)
        {
            record = new DataRecord(architecture16Bit);
            int recordByteLen;

            recordByteLen = ((int)bl2Bytes[index++]) & 0xFF;
            recordByteLen |= (((int)bl2Bytes[index++]) & 0xFFl) << 8;
            recordByteLen |= (((int)bl2Bytes[index++]) & 0xFFl) << 16;
            recordByteLen |= (((int)bl2Bytes[index++]) & 0xFFl) << 24;
            record.address = ((long)bl2Bytes[index++]) & 0xFFl;
            record.address |= (((long)bl2Bytes[index++]) & 0xFFl) << 8;
            record.address |= (((long)bl2Bytes[index++]) & 0xFFl) << 16;
            record.address |= (((long)bl2Bytes[index++]) & 0xFFl) << 24;
            if(invalidFile && ((recordByteLen > 1024 * 32) || (recordByteLen < 0))) // Truncate data length if we are trying to decode a nonsense/non-.bl2 file
            {
                recordByteLen = 32 * 1024;
            }
            record.data = Arrays.copyOfRange(bl2Bytes, index, index + recordByteLen);
            this.records.add(record);
            index += recordByteLen;
        }
        for(int i = 0; i < records.size(); i++)
        {
            DataRecord dr = records.get(i);
            if((dr.data.length % 3 == 0) && ((dr.address & 0x1) == 0) && (dr.data.length != 0) && (dr.address < 0x01000000))
            {
                dr.architecture16Bit = true;
            }
            if(i < records.size() - 1)
            {
                if((dr.address + (dr.data.length / 3 * 2) == records.get(i + 1).address) && (dr.architecture16Bit == true))
                {
                    architecture16Bit = true;
                    break;
                }
            }
        }
        if(!architecture16Bit)
        {
            boolean allRecords16Bit = true;
            for(DataRecord dr : records)
            {
                if(!dr.architecture16Bit)
                {
                    allRecords16Bit = false;
                    break;
                }
            }
            this.architecture16Bit = allRecords16Bit;
        }
        for(DataRecord dr : records)
        {
            dr.architecture16Bit = architecture16Bit;
        }
    }

    public static Bl2b GetBl2bFromFile(String artifactFilename)
    {
        byte[] fileData;
        fileData = Multifunction.ReadFileBinary(artifactFilename);
        if(fileData == null)
        {
            return null;
        }
        return new Bl2b(fileData, null);
    }

    public Bl2b(String hexFileContents)
    {
        this.architecture16Bit = false;
        this.records = ParseHex(hexFileContents, false);

        // Sort the list by address and coalesce adjacent data records
        DataRecord.CoalesceRecords(this.records);

        // Determine what the .hex file is targetting: 16-bit device or 8/32-bit device
        if(Blob.TestRecordsLookLike16Bit(this.records))
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


    // Decodes an Intel .hex file and returns the contents in a more human readable text format.
    static String HexToReadable(String hexFilePath)
    {
        Bl2b.HexDecoderState state = Bl2b.HexDecoderState.COLON;
        long decoderAddress = 0, hexHEX386Address = 0;
        int hexRecordType = 0;
        int hexRecordAddress = 0;
        int hexRecordLen = 0;
        int hexRecordDataIndex = 0;
        int hexRecordChecksum = 0, computedRecordChecksum = 0;
        int lineNumber = 0;
        int lineChecksumListIndex = 0;
        byte hexRecordData[] = new byte[256];
        byte b;
        byte[] hexFileContents;
        List<String> humanText = new ArrayList<String>();

        hexFileContents = ReadFileBinary(hexFilePath);
        if(hexFileContents == null)
        {
            return "    Error: could not read input file \"" + hexFilePath + "\\n";
        }

        humanText.add("Line Num  Line Bytes  Rec Address  Record Type     Checksum  Computed Addr  Record Data\n"
                      + "-----------------------------------------------------------------------------------------------------------------");

        // Decode .hex file text, one byte at a time
        for(int inIndex = 0; inIndex < hexFileContents.length; inIndex++)
        {
            // Get the byte into b
            b = hexFileContents[inIndex];

            // Eat whitespace
            if((b == ' ') || (b == '\r') || (b == '\n') || (b == '\t'))
            {
                if(b == '\n')
                {
                    lineNumber++;
                }
                if(state != Bl2b.HexDecoderState.COLON)
                {
                    humanText.add("\n    Error: whitespace detected within a record\n");
                }
                continue;
            }

            // Check for illegal non-hex, colon, or whitespace characters
            if(state == Bl2b.HexDecoderState.COLON)
            {
                if(b != ':')
                {
                    humanText.add("\n    Error: unknown character encountered\n");
                    continue;
                }
                humanText.add("\n");
                state = Bl2b.HexDecoderState.LENGTH0;
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
                humanText.add("\n    Error: unknown character encountered\n");
                continue;
            }

            // Decode the hextet using a .hex record decoder state machine
            switch(state)
            {
                case LENGTH0:
                    hexRecordLen = b & 0x0F;
                    state = Bl2b.HexDecoderState.LENGTH1;
                    break;
                case LENGTH1:
                    hexRecordLen <<= 4;
                    hexRecordLen |= b & 0x0F;
                    computedRecordChecksum = (computedRecordChecksum + hexRecordLen) & 0xFF;
                    humanText.add(String.format("%1$8d      %2$6d  ", lineNumber, hexRecordLen));
                    state = Bl2b.HexDecoderState.ADDRESS0;
                    break;

                case ADDRESS0:
                    hexRecordAddress = b & 0x0F;
                    state = Bl2b.HexDecoderState.ADDRESS1;
                    break;
                case ADDRESS1:
                    hexRecordAddress <<= 4;
                    hexRecordAddress |= b & 0x0F;
                    computedRecordChecksum = (computedRecordChecksum + hexRecordAddress) & 0xFF;
                    state = Bl2b.HexDecoderState.ADDRESS2;
                    break;
                case ADDRESS2:
                    hexRecordAddress <<= 4;
                    hexRecordAddress |= b & 0x0F;
                    state = Bl2b.HexDecoderState.ADDRESS3;
                    break;
                case ADDRESS3:
                    hexRecordAddress <<= 4;
                    hexRecordAddress |= b & 0x0F;
                    computedRecordChecksum = (computedRecordChecksum + hexRecordAddress) & 0xFF;
                    humanText.add(String.format("0x%1$04X       ", hexRecordAddress));
                    state = Bl2b.HexDecoderState.TYPE0;
                    break;

                case TYPE0:
                    hexRecordType = b & 0x0F;
                    state = Bl2b.HexDecoderState.TYPE1;
                    break;
                case TYPE1:
                    hexRecordType <<= 4;
                    hexRecordType |= b & 0x0F;
                    humanText.add(String.format("%1$X=%2$s  ", hexRecordType, (hexRecordType == 0x00) ? "Data        "
                                                                              : (hexRecordType == 0x01) ? "EOF         "
                                                                                : (hexRecordType == 0x02) ? "Ext Seg Addr"
                                                                                  : (hexRecordType == 0x03) ? "St Seg Addr "
                                                                                    : (hexRecordType == 0x04) ? "Ext Lin Addr"
                                                                                      : (hexRecordType == 0x05) ? "St Lin Addr "
                                                                                        : "Illegal     "));
                    lineChecksumListIndex = humanText.size();

                    hexRecordDataIndex = 0;
                    computedRecordChecksum = (computedRecordChecksum + hexRecordType) & 0xFF;
                    if(hexRecordType == 0x00)
                    {
                        humanText.add(String.format("0x%1$08X     ", ((decoderAddress & 0xFFFF0000L) | hexRecordAddress) >> 1));
                    }
                    else if(hexRecordType == 0x04)  // Extended Linear Address
                    {
                    }
                    else
                    {
                        humanText.add("               ");
                    }

                    state = Bl2b.HexDecoderState.DATA0;
                    if(hexRecordLen == 0)
                    {
                        state = Bl2b.HexDecoderState.CHECK0;
                    }
                    break;

                case DATA0:
                    hexRecordData[hexRecordDataIndex] = b;
                    state = Bl2b.HexDecoderState.DATA1;
                    break;
                case DATA1:
                    hexRecordData[hexRecordDataIndex] <<= 4;
                    hexRecordData[hexRecordDataIndex] |= b;
                    humanText.add(String.format("%1$02X", hexRecordData[hexRecordDataIndex]));
                    computedRecordChecksum = (computedRecordChecksum + (hexRecordData[hexRecordDataIndex] & 0xFF)) & 0xFF;
                    hexRecordDataIndex++;
                    if(hexRecordDataIndex % 4 == 0)
                    {
                        humanText.add(" ");
                    }
                    state = Bl2b.HexDecoderState.DATA0;
                    if(hexRecordDataIndex == hexRecordLen)
                    {
                        state = Bl2b.HexDecoderState.CHECK0;
                    }
                    break;

                case CHECK0:
                    hexRecordChecksum = b & 0x0F;
                    state = Bl2b.HexDecoderState.CHECK1;
                    break;
                case CHECK1:
                    hexRecordChecksum <<= 4;
                    hexRecordChecksum |= b & 0x0F;
                    computedRecordChecksum = (-computedRecordChecksum) & 0xFF;
                    humanText.add(lineChecksumListIndex, String.format("0x%1$02X %2$5s", hexRecordChecksum, hexRecordChecksum == computedRecordChecksum ? "" : "(BAD)"));
                    computedRecordChecksum = 0;
                    state = Bl2b.HexDecoderState.COLON;

                    // Process the record
                    switch(hexRecordType)
                    {
                        case 0: // Data Record
                            // Add to our address counter, but do not change the upper 16 bits, which are tied to linear extended addressing records
                            decoderAddress = (decoderAddress & 0xFFFF0000L) | ((decoderAddress + hexRecordLen) & 0xFFFFL);
                            break;

                        case 1: // End-of-File Record
                            if(inIndex + 12 < hexFileContents.length)
                            {
                                humanText.add("\n    Error: found EOF record with more data after it\n");
                            }
                            if(hexRecordLen != 0)
                            {
                                humanText.add("\n    Error: found EOF record with data in it\n");
                            }
                            break;

                        case 2: // HEX386 Extended Segment Address Record (additional address to add left shifted by 4 bits (16-bits))
                            hexHEX386Address = ((hexRecordData[0] & 0xFFL) << 12) | ((hexRecordData[1] & 0xFFL) << 4);
                            if(hexRecordLen != 2)
                            {
                                humanText.add("\n    Error: found extra data in an Extended Segment Address Record\n");
                            }
                            break;

                        case 4: // Extended Linear Address Record (high 16-bits of address)
                            decoderAddress = (decoderAddress & 0x0000FFFF) | ((hexRecordData[0] & 0xFFL) << 24) | ((hexRecordData[1] & 0xFFL) << 16);
                            humanText.add(lineChecksumListIndex + 1, String.format("0x%1$04X----     ", (decoderAddress & 0xFFFF0000L) >> 17));

                            if(hexRecordLen != 2)
                            {
                                humanText.add("\n    Error: found extra data in an Extended Linear Address Record\n");
                            }
                            break;

                        case 5: // Start Linear Address Record
                            humanText.add("\n    Error: Start Linear Address Records not supported\n");
                            break;

                        default:
                            humanText.add("\n    Error: unrecognized record type; skipping record\n");
                            break;
                    }
                    hexRecordDataIndex = 0;
                    break;
            }
        }

        if(state != Bl2b.HexDecoderState.COLON)
        {
            humanText.add("\n    Error: ran out of data in the middle of a record\n");
        }
        if(hexRecordType != 1)
        {
            humanText.add("\n    Error: missing EOF record\n");
        }

        return CatStringList(humanText);
    }


    /**
     * TODO: Reimplement this for the new .bl2 file format. This is old code
     * from the original .blob file format, which is not the planned
     * encryption method.
     *
     * @param state
     * @param bl2Bytes
     *
     * @return
     */
    public static byte[] EncryptBl2Bytes(EZBLState state, byte bl2Bytes[])
    {
        Cipher aes;
        SecretKeySpec key;
        byte counterBytes[] = new byte[16];
        byte keySteamBytes[];
        int chunkSize;
        long blobIndex;
        byte encryptedBytes[] = new byte[bl2Bytes.length];
        int i;

        // Get a copy of the non-encrypted blob bytes
        System.arraycopy(bl2Bytes, 0, encryptedBytes, 0, encryptedBytes.length);

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

    // Returns a 20 byte SHA-1 hash of the blob contents, including all record
    // data, record headers, and the blob headers. The only data excluded is the
    // Blob header hash field and trailing CRC32, which is treated as all zeros
    // for hash computation.
    public byte[] GetHash()
    {
        byte[] data = GetBytes();
        return Arrays.copyOfRange(data, data.length - 4 - 32, data.length - 4);
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
     * Generates an <address, data> map of all bl2 data record contents. Each
     * address is aligned to an instruction word boundary (0x2 for 16-bit
     * devices, 0x4 for 32-bit or other devices) unless the record doesn't start
     * on an aligned boundary.
     *
     * @return Map of addresses (keys) with data contents (values) in the memory
     *         image.
     */
    public Map<Integer, Integer> GetData()
    {
        Map<Integer, Integer> ret = new HashMap<Integer, Integer>(4096);

        for(DataRecord dr : this.records)
        {
            int address;
            int data;
            int i;

            address = (int)dr.address;
            if(!dr.isArchitecture16Bit())
            {
                for(i = 0; i < dr.data.length - 4; address += 4)
                {
                    data = (((int)dr.data[i++]) & 0xFF)
                           | ((((int)dr.data[i++]) & 0xFF) << 8)
                           | ((((int)dr.data[i++]) & 0xFF) << 16)
                           | ((((int)dr.data[i++]) & 0xFF) << 24);
                    ret.put(address, data);
                }
                if(dr.data.length - i == 3)
                {
                    data = (((int)dr.data[i++]) & 0xFF)
                           | ((((int)dr.data[i++]) & 0xFF) << 8)
                           | ((((int)dr.data[i++]) & 0xFF) << 16);
                    ret.put(address, data);
                }
                else if(dr.data.length - i == 2)
                {
                    data = (((int)dr.data[i++]) & 0xFF)
                           | ((((int)dr.data[i++]) & 0xFF) << 8);
                    ret.put(address, data);
                }
                else if(dr.data.length - i == 1)
                {
                    data = (((int)dr.data[i++]) & 0xFF);
                    ret.put(address, data);
                }
            }
            else
            {// 16-bit devices
                for(i = 0; i < dr.data.length - 3;)
                {
                    data = (((int)dr.data[i++]) & 0xFF)
                           | ((((int)dr.data[i++]) & 0xFF) << 8)
                           | ((((int)dr.data[i++]) & 0xFF) << 16);
                    ret.put(address, data);
                    address += 2;
                }
                if(dr.data.length - i == 2)
                {
                    data = (((int)dr.data[i++]) & 0xFF)
                           | ((((int)dr.data[i++]) & 0xFF) << 8);
                    ret.put(address, data);
                    continue;
                }
                if(dr.data.length - i == 1)
                {
                    data = (((int)dr.data[i++]) & 0xFF);
                    ret.put(address, data);
                }
            }
        }

        return ret;
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
        Bl2b tempBlob = new Bl2b(architecture16Bit);
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
     * Gets the total .bl2 file contents in serialized, ordered form, and with
     * all headers and hashing included + the 4 byte trailing CRC. Encryption is
     * performed if the key material has been set (TODO encryption).
     *
     * @param headerSYNCstring String of characters to write at the very
     *                         beginning of the file, typically data containing auto-baud
     *                         synchronization and/or other bootloader wake up
     *                         data.
     *                         If this parameter is null, the default
     *                         synchronization header string of
     *                         "UUUUUUUUMCUPHCME" is generated.
     *                         If this parameter is an empty string, no
     *                         synchronization header data is prepended.
     *
     * @return Array of bytes to save to a .bl2 file.
     */
    public byte[] GetBytes(String headerSYNCstring)
    {
        byte retData[];
        int index;
        DataRecord record;
        ListIterator<DataRecord> iterator;

        if(headerSYNCstring == null)    // Default "UUUUUUUUMCUPHCME" sync string for null input
        {
            headerSYNCstring = "UUUUUUUUMCUPHCME";
        }

        // Find the total length of the output array
        index = 0;
        iterator = records.listIterator();
        while(iterator.hasNext())
        {
            record = iterator.next();
            index += record.data.length + 8;   // 8 is the data record header length
        }

        // Allocate memory for a sync data string + 48 byte file header, the data records, a 32 bytes trailing SHA-256 hash, and a 4 byte final CRC32
        retData = new byte[headerSYNCstring.length() + 48 + index + 32 + 4];

        // Write synchronization/wake up string, typically 16-byte: "UUUUUUUUMCUPHCME"
        System.arraycopy(headerSYNCstring.getBytes(), 0, retData, 0, headerSYNCstring.length());
        index = headerSYNCstring.length();

        retData[index++] = 'B'; // "BL2B" file type string
        retData[index++] = 'L';
        retData[index++] = '2';
        retData[index++] = 'B';
        retData[index++] = (byte)((retData.length - 16 - 8) & 0xFF);; // Length to End Of File after this 64-byte header (size of all data records, their headers, the SHA-256 hash, and the CRC32)
        retData[index++] = (byte)(((retData.length - 16 - 8) >> 8) & 0xFF);
        retData[index++] = (byte)(((retData.length - 16 - 8) >> 16) & 0xFF);
        retData[index++] = (byte)(((retData.length - 16 - 8) >> 24) & 0xFF);

        // Write 16-byte BOOTID_HASH field (SHA-256 right-most 16 bytes)
        for(int i = 0; i < 4; i++)
        {
            retData[index++] = (byte)(this.bootIDHash[i] & 0xFF);
            retData[index++] = (byte)((this.bootIDHash[i] >> 8) & 0xFF);
            retData[index++] = (byte)((this.bootIDHash[i] >> 16) & 0xFF);
            retData[index++] = (byte)((this.bootIDHash[i] >> 24) & 0xFF);
        }

        // Write 8-byte APPID_VER field
        retData[index++] = (byte)(this.appIDVerBuild);
        retData[index++] = (byte)(this.appIDVerBuild >> 8);
        retData[index++] = (byte)(this.appIDVerBuild >> 16);
        retData[index++] = (byte)(this.appIDVerBuild >> 24);
        retData[index++] = (byte)(this.appIDVerMinor);
        retData[index++] = (byte)(this.appIDVerMinor >> 8);
        retData[index++] = (byte)(this.appIDVerMajor);
        retData[index++] = (byte)(this.appIDVerMajor >> 8);

        // Write 16-byte HMAC of this header (encryption password || 32-bytes of preceeding file data, starting and including the BL2B string). Only the right-most 16 bytes are stored (upper 16 bytes must be recomputed by the target and used as an encryption nonce)
        Arrays.fill(retData, index, index + 16, (byte)0x00);
        index += 16;


        // Copy all record data into the array
        iterator = records.listIterator();
        while(iterator.hasNext())
        {
            // Get a record
            record = iterator.next();

            // Create a record header containing 4 byte Record Length + 4 byte Record Start address
            retData[index++] = (byte)(((record.data.length) & 0x000000FFL));        // Length does not include either header fields: just the record data length
            retData[index++] = (byte)(((record.data.length) & 0x0000FF00L) >> 8);
            retData[index++] = (byte)(((record.data.length) & 0x00FF0000L) >> 16);
            retData[index++] = (byte)(((record.data.length) & 0xFF000000L) >> 24);
            retData[index++] = (byte)((record.address & 0x000000FFL));
            retData[index++] = (byte)((record.address & 0x0000FF00L) >> 8);
            retData[index++] = (byte)((record.address & 0x00FF0000L) >> 16);
            retData[index++] = (byte)((record.address & 0xFF000000L) >> 24);

            // Copy record data
            System.arraycopy(record.data, 0, retData, index, record.data.length);
            index += record.data.length;
        }

        // Compute SHA-256 hash (of all data up to this point except the syncrhonization fields)
        try
        {
            MessageDigest hashComputer = MessageDigest.getInstance("SHA-256");
            hashComputer.update(retData, 16, retData.length - 16 - 4 - 32);
            hashComputer.digest(retData, retData.length - 4 - 32, 32);
        }
        catch(NoSuchAlgorithmException ex)
        {
            System.err.println("ezbl_tools: Cannot find 'SHA-256' hash algorithm. Make sure your JRE includes SHA-256 support. Output hash will be set to all zeros.");
        }
        catch(DigestException ex)
        {
            System.err.printf("ezbl_tools: SHA-256 exception: %1$s\n", ex.getMessage());
        }
        index += 32;

        // Finally add the CRC32 for communications verification purposes
        // Compute expected value with last 4 byte CRC field EXCLUDED from the
        // computation (i.e. not included as zeros). When we place the CRC there
        // and someone computes the CRC over the whole file (less
        // synchronization header), they'll always get the magic 'm' CRC value
        // of 0x2144DF1C out if the data wasn't damaged (same result as if you
        // computed CRC32(0)).
        CRC32 myCRC = new CRC32();
        myCRC.update(retData, 16, retData.length - 16 - 4);
        long crcValue = myCRC.getValue();
        retData[retData.length - 4] = (byte)((crcValue) & 0xFF);
        retData[retData.length - 3] = (byte)((crcValue >> 8) & 0xFF);
        retData[retData.length - 2] = (byte)((crcValue >> 16) & 0xFF);
        retData[retData.length - 1] = (byte)((crcValue >> 24) & 0xFF);

        return retData;
    }


    /**
     * Gets the total .bl2 file contents in serialized, ordered form, and with
     * all headers and hashing included + the 4 byte trailing CRC. Encryption is
     * performed if the key material has been set (TODO encryption).
     *
     * This function implements the default synchronization header string of
     * "UUUUUUUUMCUPHCME".
     */
    public byte[] GetBytes()
    {
        return GetBytes(null);
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

    
    /**
     * Returns a human readable text representation of the Bl2b data contents.
     *
     * Useful for debugging, .bl2 format analysis, or learning about the .bl2 
     * structure for implementation in another tool.
     *
     * @return String representing the decoded data contents in human
     *         readable form, highlighting any errors detected in the data.
     */
    public String GetObjDumpView()
    {
        return Bl2b.GetObjDumpView(this.GetBytes());
    }

    
    /**
     * Returns a human readable text representation of a .hex file's
     * contents.
     *
     * Useful for debugging and easily checking what's in a .hex
     * file
     *
     * @param hexFileContents Text contents of a .hex file. 16-bit vs. 8/32-bit 
     *                        mode is detected automatically by examining record 
     *                        contents and addresses implemented.
     *
     * @return String representing the decoded .hex file contents, converted to 
     *         a .bl2 in memory, and then converted to human readable form.
     */
    public static String GetObjDumpView(String hexFileContents)
    {
        return Bl2b.GetObjDumpView(new Bl2b(hexFileContents).GetBytes()); // Converts .hex to .bl2 file inside Bl2b() constructor
    }

    
    /**
     * Returns a human readable text representation of a .bl2 file's
     * contents.
     *
     * Useful for debugging, .bl2 format analysis, or learning about the .bl2 
     * structure for implementation in another tool.
     *
     * @param fileBytes Raw .bl2 file contents.
     *
     * @return String representing the decoded .bl2 file contents in human
     *         readable form, highlighting any errors detected in the data.
     */
    public static String GetObjDumpView(byte[] fileBytes)
    {
        long size;
        int recordCount = 0;
        int bootID[] = new int[4];
        int appIDVerMajor;
        int appIDVerMinor;
        int appIDVerBuild;
        List<String> output = new ArrayList<String>();
        int index;
        boolean architecture16Bit;
        int recordCountInsertIndex;

        if(fileBytes == null)
        {
            return null;
        }

        if(fileBytes.length < 64 + 32 + 4)
        {
            output.add(String.format("  ERROR: BL2 file length too short to be a valid .bl2 file. Length = %1$d bytes, minimum is %2$d bytes.\n", fileBytes.length, 64 + 32 + 4));
        }
        index = 0;
        if(fileBytes.length >= 16)
        {
            index = 0;
            byte[] field = Arrays.copyOfRange(fileBytes, index, index + 16);
            String fieldStr = new String(field);
            String expect = "UUUUUUUUMCUPHCME";
            output.add(String.format("SYNC - Auto-baud and synchronization/bootloader wake-up bytes:\n  " + fieldStr + " (%1$02X %2$02X %3$02X %4$02X %5$02X %6$02X %7$02X %8$02X %9$02X %10$02X %11$02X %12$02X %13$02X %14$02X %15$02X %16$02X)\n", field[0], field[1], field[2], field[3], field[4], field[5], field[6], field[7], field[8], field[9], field[10], field[11], field[12], field[13], field[14], field[15]));
            if(!fieldStr.equals(expect))
            {
                output.add("  ERROR: expected: " + expect + "\n");
            }
        }
        if(fileBytes.length >= 16 + 4)
        {
            index = 16;
            byte[] field = Arrays.copyOfRange(fileBytes, index, index + 4);
            String fieldStr = new String(field);
            String expect = "BL2B";
            output.add(String.format("\nFILE_ID - File type:\n  0x%1$02X%2$02X%3$02X%4$02X (\"" + fieldStr + "\") %5$s\n", field[3], field[2], field[1], field[0], fieldStr.equals("BL2B") ? "(Blob version 2)" : ""));
            if(!fieldStr.equals(expect))
            {
                output.add("  ERROR: expected: " + expect + "\n");
            }
        }
        if(fileBytes.length >= 16 + 4 + 4)
        {
            index = 16 + 4;
            size = ((int)fileBytes[index++]) & 0xFF;
            size |= (((int)fileBytes[index++]) << 8) & 0xFF00;
            size |= (((int)fileBytes[index++]) << 16) & 0xFF0000;
            size |= (((int)fileBytes[index++]) << 24) & 0xFF000000;
            output.add(String.format("\nFILE_LEN - File size from BOOTID_HASH to EOF:\n  %1$d bytes (%2$d byte file total)\n", size, size + 16 + 8));
            if((size != fileBytes.length - 16 - 8) || (size + 16 + 8 != fileBytes.length))
            {
                output.add(String.format("  ERROR: expected: %1$d bytes (%2$d bytes total)\n", fileBytes.length - 16 - 8, fileBytes.length));
            }
        }
        if(fileBytes.length >= 16 + 8 + 16)
        {
            index = 16 + 8;
            for(int i = 0; i < bootID.length; i++)
            {
                bootID[i] = (((int)fileBytes[index++]) & 0xFF) | ((((int)fileBytes[index++]) & 0xFF) << 8) | ((((int)fileBytes[index++]) & 0xFF) << 16) | ((((int)fileBytes[index++]) & 0xFF) << 24);
            }
            output.add(String.format("\nBOOTID_HASH - Right-most 16 bytes of SHA-256 hash of BOOTID_* strings:\n  %1$08X%2$08X%3$08X%4$08X %5$s\n", bootID[3], bootID[2], bootID[1], bootID[0], (bootID[3] | bootID[2] | bootID[1] | bootID[0]) == 0 ? "(image not generated from .elf?)" : ""));
        }
        if(fileBytes.length >= 16 + 8 + 16 + 8)
        {
            index = 16 + 8 + 16;
            appIDVerBuild = (((int)fileBytes[index++]) & 0xFF) | ((((int)fileBytes[index++]) & 0xFF) << 8) | ((((int)fileBytes[index++]) & 0xFF) << 16) | ((((int)fileBytes[index++]) & 0xFF) << 24);
            appIDVerMinor = (((int)fileBytes[index++]) & 0xFF) | ((((int)fileBytes[index++]) & 0xFF) << 8);
            appIDVerMajor = (((int)fileBytes[index++]) & 0xFF) | ((((int)fileBytes[index++]) & 0xFF) << 8);
            output.add(String.format("\nAPPID_VER - Application version identifier:\n  %1$d.%2$02d.%3$04d\n", appIDVerMajor, appIDVerMinor, appIDVerBuild));
        }
        if(fileBytes.length >= 16 + 8 + 16 + 8 + 16)
        {
            index = 16 + 8 + 16 + 8;
            byte[] field = Arrays.copyOfRange(fileBytes, index, index + 16);
            byte sumOR = 0x00;
            output.add(String.format("\nHMAC_SHA_256 - (future use):\n  "));
            for(int i = 0; i < 16; i += 4)
            {
                output.add(String.format("%1$02X%2$02X%3$02X%4$02X", field[i], field[i + 1], field[i + 2], field[i + 3]));
                sumOR |= field[i] | field[i + 1] | field[i + 2] | field[i + 3];
            }
            if(sumOR == 0x00)
            {
                output.add(sumOR == 0x00 ? " (file not encrypted)\n" : "\n");
            }
        }
        recordCountInsertIndex = output.size();

        // Do a quick enumeration of record headers to try and determine if this can't be a 16-bit .blob/.bl2
        architecture16Bit = true;
        int searchByteIndex = 64;
        while(searchByteIndex < fileBytes.length - 32 - 4)
        {
            long recordLength;
            long recordAddress;

            if(searchByteIndex + 8 > fileBytes.length - 32 - 4)
            {
                break;
            }
            recordLength = ((long)fileBytes[searchByteIndex++]) & 0xFF;
            recordLength |= (((long)fileBytes[searchByteIndex++]) & 0xFF) << 8;
            recordLength |= (((long)fileBytes[searchByteIndex++]) & 0xFF) << 16;
            recordLength |= (((long)fileBytes[searchByteIndex++]) & 0xFF) << 24;
            recordAddress = ((long)fileBytes[searchByteIndex++]) & 0xFF;
            recordAddress |= (((long)fileBytes[searchByteIndex++]) & 0xFF) << 8;
            recordAddress |= (((long)fileBytes[searchByteIndex++]) & 0xFF) << 16;
            recordAddress |= (((long)fileBytes[searchByteIndex++]) & 0xFF) << 24;

            if(((recordAddress & 0xFF000001) != 0) || (recordLength % 3 != 0))
            {
                architecture16Bit = false;  // 16-bit devices only have 24-bit address space
                break;
            }
            searchByteIndex += recordLength;
        }

        output.add("Starting record decoding in " + (architecture16Bit ? "16-bit" : "8-bit/32-bit") + " architecture mode\n\n");

        index = 64;
        while(index < fileBytes.length - 32 - 4)
        {
            long recordLength;
            long recordAddress;
            int i;

            if(index + 8 > fileBytes.length - 32 - 4)
            {
                output.add(String.format("  ERROR: found %1$d bytes at the end of the file. This is too small to be a record header.\n", fileBytes.length - 32 - 4 - index));
                output.add("    Bytes are: ");
                for(i = 0; i < fileBytes.length - 32 - 4 - index; i++)
                {
                    output.add(String.format("%1$02X", fileBytes[index + i]));
                }
                output.add("\n");
                break;
            }

            recordLength = ((long)fileBytes[index++]) & 0xFF;
            recordLength |= (((long)fileBytes[index++]) & 0xFF) << 8;
            recordLength |= (((long)fileBytes[index++]) & 0xFF) << 16;
            recordLength |= (((long)fileBytes[index++]) & 0xFF) << 24;

            recordAddress = ((long)fileBytes[index++]) & 0xFF;
            recordAddress |= (((long)fileBytes[index++]) & 0xFF) << 8;
            recordAddress |= (((long)fileBytes[index++]) & 0xFF) << 16;
            recordAddress |= (((long)fileBytes[index++]) & 0xFF) << 24;

            recordCount++;

            output.add(String.format("DATA_REC_HDR.REC_LEN  - bytes in record     : 0x%1$08X (%1$d data bytes)\n", recordLength));
            output.add(String.format("DATA_REC_HDR.REC_ADDR - record start address: 0x%1$08X (to 0x%2$08X)\n", recordAddress, recordAddress + (architecture16Bit ? ((recordLength + 2) / 3 * 2) : recordLength)));
            if((int)recordLength + index > fileBytes.length - 32 - 4)
            {
                output.add("  ERROR: record length indicator extends into hash/CRC area\n");
                recordLength = fileBytes.length - 32 - 4 - index;
            }
            else if(recordLength < 0)
            {
                output.add("  ERROR: record length is invalid (computed record end address will be wrong)\n");
                recordLength = fileBytes.length - 32 - 4 - index;
            }

            if(architecture16Bit)
            {
                int lineByteIndex = 0;

                while(recordLength > 0)
                {
                    if(lineByteIndex == 0)
                    {
                        long printAddr = recordAddress;
                        if(((recordAddress & 0x7) != 0) && (recordLength >= 12))
                        {
                            printAddr = recordAddress & 0xFFFFFFF8;
                        }
                        output.add(String.format("%1$06X  ", printAddr));

                        if(((recordAddress & 0x7) != 0) && (recordLength >= 12))
                        {
                            for(printAddr = recordAddress & 0xFFFFFFF9; printAddr != recordAddress; printAddr += 0x2)
                            {
                                output.add("       ");
                                lineByteIndex += 3;
                            }
                            if((recordAddress & 0x1) != 0)
                            {
                                recordAddress += 0x1;
                                if(recordLength >= 2)
                                {
                                    output.add(String.format("%2$02X%1$02X   ", fileBytes[index++], fileBytes[index++]));
                                    lineByteIndex += 3;
                                    recordLength -= 2;
                                }
                                else
                                {
                                    output.add(String.format("  %1$02X   ", fileBytes[index++]));
                                    lineByteIndex += 3;
                                    recordLength -= 1;
                                }
                            }
                        }
                    }
                    if(recordLength >= 3)
                    {
                        output.add(String.format("%3$02X%2$02X%1$02X ", fileBytes[index++], fileBytes[index++], fileBytes[index++]));
                        lineByteIndex += 3;
                        recordAddress += 0x2;
                        recordLength -= 3;
                    }
                    else if(recordLength == 2)
                    {
                        output.add(String.format("  %2$02X%1$02X ", fileBytes[index++], fileBytes[index++]));
                        lineByteIndex += 3;
                        recordAddress += 0x2;
                        recordLength -= 2;
                    }
                    else if(recordLength == 1)
                    {
                        output.add(String.format("    %1$02X ", fileBytes[index++]));
                        lineByteIndex += 3;
                        recordAddress += 0x2;
                        recordLength -= 1;
                    }
                    if((lineByteIndex >= 12) || (recordLength <= 0))
                    {
                        output.add("\n");
                        lineByteIndex = 0;
                    }
                }
            }
            else    // 8-bit or 32-bit mode
            {
                for(i = 0; i + 16 <= (int)recordLength; i += 16)
                {
                    output.add(String.format("%1$08X  ", recordAddress));
                    output.add(String.format("%4$02X%3$02X%2$02X%1$02X %8$02X%7$02X%6$02X%5$02X %12$02X%11$02X%10$02X%9$02X %16$02X%15$02X%14$02X%13$02X\n", fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++]));
                    recordAddress += 0x10;
                }
                if(i < recordLength)
                {
                    output.add(String.format("%1$08X  ", recordAddress));
                    for(; i + 4 <= (int)recordLength; i += 4)
                    {
                        output.add(String.format("%4$02X%3$02X%2$02X%1$02X ", fileBytes[index++], fileBytes[index++], fileBytes[index++], fileBytes[index++]));
                    }
                    if((int)recordLength - i == 3)
                    {
                        output.add(String.format("  %3$02X%2$02X%1$02X", fileBytes[index++], fileBytes[index++], fileBytes[index++]));
                    }
                    else if((int)recordLength - i == 2)
                    {
                        output.add(String.format("    %2$02X%1$02X", fileBytes[index++], fileBytes[index++]));
                    }
                    else if((int)recordLength - i == 1)
                    {
                        output.add(String.format("      %1$02X", fileBytes[index++]));
                    }

                    output.add("\n");
                }
            }
            output.add("\n");
        }

        output.add(recordCountInsertIndex, String.format("\n\nScanning found DATA_RECORDs: %1$d\n", recordCount));

        if(fileBytes.length >= 64 + 32)
        {
            index = fileBytes.length - 4 - 32;
            byte[] field = Arrays.copyOfRange(fileBytes, index, index + 32);
            byte[] computedHash = null;
            boolean match;

            // Compute SHA-256 hash (of all data up to this point except the syncrhonization fields)
            try
            {
                MessageDigest hashComputer = MessageDigest.getInstance("SHA-256");
                hashComputer.update(fileBytes, 16, fileBytes.length - 16 - 32 - 4);
                computedHash = hashComputer.digest();
            }
            catch(NoSuchAlgorithmException ex)
            {
                output.add("ezbl_tools: Cannot find 'SHA-256' hash algorithm. Make sure your JRE includes SHA-256 support. Cannot test hash.");
            }

            output.add(String.format("\n\nFILE_HASH - SHA-256 hash from FILE_ID field to last DATA_RECORD byte:\n  "));
            for(int i = 0; i < 32; i += 4)
            {
                output.add(String.format("%1$02X%2$02X%3$02X%4$02X", field[i], field[i + 1], field[i + 2], field[i + 3]));
            }
            match = Arrays.equals(field, computedHash);
            output.add(String.format(" %1$s (computed over file offset 16 to %2$d)\n", match ? "(correct)" : "(INVALID)", fileBytes.length - 16 - 32 - 4));
            if(!match)
            {
                output.add("\n  ERROR: expected: ");
                for(int i = 0; i < 32; i += 4)
                {
                    output.add(String.format("%1$02X%2$02X%3$02X%4$02X", computedHash[i], computedHash[i + 1], computedHash[i + 2], computedHash[i + 3]));
                }
            }
        }
        if(fileBytes.length >= 64 + 32 + 4)
        {
            boolean match;
            index = fileBytes.length - 4;
            byte[] field = Arrays.copyOfRange(fileBytes, index, index + 4);
            long fieldCRC = (((long)field[0]) & 0xFF) | ((((long)field[1]) & 0xFF) << 8) | ((((long)field[2]) & 0xFF) << 16) | ((((long)field[3]) & 0xFF) << 24);
            CRC32 myCRC = new CRC32();
            myCRC.update(fileBytes, 16, fileBytes.length - 16 - 4);
            long crcValue = myCRC.getValue();
            match = crcValue == fieldCRC;
            output.add(String.format("\nCRC32 - CRC32 from FILE_ID to last FILE_HASH byte:\n  0x%1$02X%2$02X%3$02X%4$02X %5$s (computed over file offset 16 to %6$d)\n", field[3], field[2], field[1], field[0], match ? "(correct)" : "INVALID", fileBytes.length - 16 - 4));
            if(!match)
            {
                output.add(String.format("  ERROR: expected: 0x%1$08X\n", crcValue));
            }
        }
        output.add("\nEnd of file/decoding");

        
        String ret = CatStringList(output).replace(" \n", "\n");    // Remove a trailing whitespace character on flash word printouts
        if(File.separatorChar == '\\')  // On Windows, replace LF with CR+LF so primitive editors like Notepad can correctly view the report
        {
            ret = ret.replace("\n", "\r\n");
        }
        return ret;
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

    // Aligns records to 0x4 addresses on any 16-bit program space data, 
    // otherwise fixed 0x10 or 16 addresses.
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

