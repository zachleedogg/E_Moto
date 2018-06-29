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

import static com.microchip.apps.ezbl.Multifunction.CatStringList;
import java.io.*;
import java.util.*;


/**
 *
 * @author C12128
 */
public class DataRecord implements Serializable, Cloneable, Comparator<DataRecord>, Comparable<DataRecord>
{

    public boolean architecture16Bit = false;
    public long address = -1;
    public byte data[] = null;
    public String assignedMemory = "";
    public String comment = "";

    public DataRecord Clone()
    {
        DataRecord ret = new DataRecord();

        ret.address = this.address;
        ret.architecture16Bit = this.architecture16Bit;
        ret.assignedMemory = this.assignedMemory;
        ret.comment = this.comment;
        if(this.data == null)
        {
            ret.data = null;
        }
        else
        {
            ret.data = Arrays.copyOf(this.data, this.data.length);
        }

        return ret;
    }

    public DataRecord()
    {
    }

    public DataRecord(boolean architchture16Bit)
    {
        this.architecture16Bit = architchture16Bit;
    }

    public DataRecord(long recordAddress, byte recordData[], boolean architecture16Bit)
    {
        address = recordAddress;
        data = recordData;
        this.architecture16Bit = architecture16Bit;
    }

    /**
     * Creates a DataRecord from 1 or more lines of text dumped with
     * xc16-objdump -s. All lines must have a '\n' terminator, even if there is
     * only one line. When multiple lines are given, the data must lie at
     * contiguous addresses.
     *
     * @param elfDumpSectionDataLines
     *                                String of obj-dump output data for the section, not including
     *                                the section name. Ex:
     *                                0000 240204 000000 $.....
     */
    public DataRecord(String elfDumpSectionDataLines)
    {
        // Decode section addresses and if in range, save the data as data records
        int startIndex = 1;
        int eolIndex = 0;
        int asciiPrintIndex;
        int dataSeperatorIndex;
        int wordByteCount;
        int dataCount;
        int totalDataCount = 0;
        List<byte[]> dataBlocks = new ArrayList<byte[]>();

        this.address = -1;

        while(eolIndex >= 0)
        {
            dataSeperatorIndex = elfDumpSectionDataLines.indexOf(' ', startIndex + 1);  // +1 is for unneeded starting space on each line
            if(dataSeperatorIndex < 0)
            {
                break;
            }
            asciiPrintIndex = elfDumpSectionDataLines.indexOf("  ", dataSeperatorIndex + 1);  // +1 is for size of the data seperator space
            if(asciiPrintIndex < 0)
            {
                break;
            }
            wordByteCount = ((elfDumpSectionDataLines.indexOf(' ', dataSeperatorIndex + 1) - dataSeperatorIndex) / 2);
            eolIndex = elfDumpSectionDataLines.indexOf('\n', asciiPrintIndex + 2);
            if(this.address < 0)
            {
                this.architecture16Bit = wordByteCount == 3;

                try
                {
                    this.address = Integer.decode("0x" + elfDumpSectionDataLines.substring(startIndex, dataSeperatorIndex));
                }
                catch(NumberFormatException ex) // Happens if we've exited the Section data record and the line provided isn't another part of the last section
                {
                    return;
                }
            }
            startIndex = eolIndex + 2;  // +2 is for unneeded \n and space characters

            // Decode the data on this line and add it to data record
            String encodedData = elfDumpSectionDataLines.substring(dataSeperatorIndex + 1, asciiPrintIndex);
            encodedData = encodedData.replaceAll(" ", "");
            dataCount = encodedData.length() / 2;
            totalDataCount += dataCount;
            byte[] lineData = new byte[dataCount];
            for(int i = 0; i < dataCount; i++)
            {
                lineData[i] = (byte)((Integer.decode("0x" + encodedData.substring(i * 2, i * 2 + 2))) & 0xFF);
            }
            dataBlocks.add(lineData);
        }

        // Coallesce all data recovered into a single array
        this.data = new byte[totalDataCount];
        dataCount = 0;
        for(byte b[] : dataBlocks)
        {
            System.arraycopy(b, 0, this.data, dataCount, b.length);
            dataCount += b.length;
        }
    }

    public boolean equals(DataRecord dataRecord)
    {
        boolean matches = (dataRecord.architecture16Bit == this.architecture16Bit) && (dataRecord.address == this.address) && (dataRecord.data.length == this.data.length);
        if(!matches)
        {
            return false;
        }

        return Arrays.equals(dataRecord.data, this.data);
    }

    @Override
    public int compareTo(DataRecord y)
    {
        return this.address == y.address ? 0 : (this.address > y.address ? 1 : -1);
    }

    @Override
    public int compare(DataRecord x, DataRecord y)
    {
        return x.address == y.address ? 0 : (x.address > y.address ? 1 : -1);
    }

    /**
     * Splits a data record into two. The original is modified (lower address
     * portion to the left), while the split off portion (on the right) is
     * returned.
     *
     * @param splitAtAddress Address within the record to split
     *
     * @return Data on the right (at higher addresses) that has been split off.
     *         If the split address doesn't exist in the DataRecord, then
     *         nothing changes and null is returned.
     */
    public DataRecord SplitAtAddress(long splitAtAddress)
    {
        DataRecord newRecord = new DataRecord(this.architecture16Bit);
        newRecord.address = splitAtAddress;
        newRecord.assignedMemory = this.assignedMemory;
        newRecord.comment = this.comment;

        // Copy extra data into the new record
        byte newBaseData[] = new byte[this.GetDataIndexOfAddress(splitAtAddress)];
        if(newBaseData.length > this.data.length)  // But return null if the address was illegal and doesn't exist in the given record
        {
            return null;
        }
        newRecord.data = new byte[this.data.length - newBaseData.length];
        System.arraycopy(this.data, newBaseData.length, newRecord.data, 0, newRecord.data.length);

        // Shrink the original record data
        System.arraycopy(this.data, 0, newBaseData, 0, newBaseData.length);
        this.data = newBaseData;

        return newRecord;
    }

    public boolean isArchitecture16Bit()
    {
        return architecture16Bit;
    }

    public void setArchitecture16Bit(boolean architecture16Bit)
    {
        this.architecture16Bit = architecture16Bit;
    }

    public long getAddress()
    {
        return address;
    }

    public void setAddress(long address)
    {
        this.address = address;
    }

    public byte[] getData()
    {
        return data;
    }

    public void setData(byte[] data)
    {
        this.data = data;
    }

    public String getAssignedMemory()
    {
        return assignedMemory;
    }

    public void setAssignedMemory(String assignedMemory)
    {
        this.assignedMemory = assignedMemory;
    }

    public String getComment()
    {
        return comment;
    }

    public void setComment(String comment)
    {
        this.comment = comment;
    }

    /**
     * Processes a List of DataRecord classes and splits longer records that
     * exceed maxRecordAddresses into two or more DataRecord elements containing
     * no more than maxRecordAddresses addresses in each record. Each split
     * record is maximally sized, so when a split occurs, the first records are
     * exactly maxRecordAddresses long while the last one in the split can be 0
     * to maxRecordAddresses long.
     *
     * @param records            List of DataRecord classes to process.
     * @param maxRecordAddresses Number of addresses that every output record
     *                           should match or be smaller than.
     */
    public static void SplitRecordsByLength(List<DataRecord> records, int maxRecordAddresses)
    {
        for(int i = 0; i < records.size(); i++)
        {
            DataRecord r = records.get(i);
            if(r.GetEndAddress() - r.address > maxRecordAddresses)
            {
                records.add(i + 1, r.SplitAtAddress(r.address + maxRecordAddresses));
            }
        }
    }


    /**
     * Sorts a List of DataRecords by record address and combines any data 
     * regions which overlap each other such that no data record address range 
     * overlap can exist afterwards. Data is combined by doing a bitwise AND 
     * operation per overlapping byte.
     *
     * @param records List of DataRecord classes to sort and combine.
     *
     * @return Hashmap containing all data in the input records, copied to 
     * individual byte arrays, combined by bitwise ANDing and keyed in the map 
     * by their starting address. If a block of data contains less than the size 
     * of a full block, one or more 0xFF bytes are generated such that all data 
     * returned in the map is a fixed single address to exact block size, much 
     * as a flash memory behaves.
     */
    public static HashMap<Long, Byte[]> CombineData(List<DataRecord> records)
    {
        if(records == null)
            return null;
        
        HashMap<Long, Byte[]> map = new HashMap<Long, Byte[]>();
        for(int i = 0; i < records.size(); i++)
        {
            DataRecord dr = records.get(i);
            if((dr.data == null) || (dr.data.length == 0))
            {
                records.remove(i--);
                continue;
            }
            int combineBlockByteLen = 4;
            int combineBlockAddrLen = 0x4;
            if(dr.architecture16Bit)
            {
                combineBlockByteLen = 3;
                combineBlockAddrLen = 0x2;
            }
            for(long j = dr.address; j < dr.GetEndAddress(); j += combineBlockAddrLen)
            {
                Byte[] wordData = new Byte[combineBlockByteLen];
                int index = dr.GetDataIndexOfAddress(j);
                for(int k = 0; k < combineBlockByteLen; k++)    // Copy data, generating 0xFF bytes if needed
                {
                    if(index >= dr.data.length)
                    {
                        wordData[k] = (byte)0xFF;
                        continue;
                    }
                    wordData[k] = dr.data[index++];
                }
                if(map.containsKey(j))
                {
                    Byte[] existingData = map.get(j);
                    for(int k = 0; k < existingData.length; k++)
                        existingData[k] = (byte)(existingData[k] & wordData[k]);
                    map.put(j, existingData);
                    continue;
                }
                map.put(j, wordData);
            }
        }
        return map;
    }

    
    /**
     * Sorts a List of DataRecords by record address and coalesces adjacent
     * records that directly abut each other into the minimum number of total
     * records.
     *
     * @param records List of DataRecord classes to sort and coalesce.
     */
    public static void CoalesceRecords(List<DataRecord> records)
    {
        CoalesceRecords(records, true);
    }

    /**
     * Optionally sorts a List of DataRecords by record address and coalesces
     * adjacent records that directly abut each other into the minimum number of
     * total records.
     *
     * @param records List of DataRecord classes to optionally sort and then
     *                coalesce.
     *
     * @param presort true if a record address sort should be performed before
     *                looking for records which are adjacent to each other.
     *                This can often result in better coalescing results.
     *
     * Set to false if no record sorting should be performed
     * and only the coalescing step should occur.
     */
    public static void CoalesceRecords(List<DataRecord> records, boolean presort)
    {
        DataRecord record, nextRecord;

        if(records == null)
        {
            return;
        }
        if(records.size() <= 1)
        {
            return;
        }

        List<DataRecord> retRecords = new ArrayList<DataRecord>();


        if(presort)
        {
            Collections.sort(records);  // Sort the records by start address to increase likelihood of finding abutting records
        }

        // Iterate over all records
        for(int i = 0; i < records.size(); i++)
        {
            record = records.get(i);
            if((record.data == null) || (record.data.length == 0))
            {
                records.remove(i--);
                continue;
            }

            retRecords.add(record);

            // Can't coalesce anything else onto this record if it doesn't end on a perfect address boundary for 16-bit PICs
            if(record.architecture16Bit && (record.data.length % 3 != 0))
            {
                continue;
            }

            // Count how many bytes are in subsequent records until we find a non-contiguous location
            int contiguousBytes = 0;
            int lastMergeIndex = i;
            long checkEndAddress = record.GetEndAddress();
            for(int j = i + 1; j < records.size(); j++)
            {
                nextRecord = records.get(j);

                // See if this record can't be appended to the previous record
                if((checkEndAddress != nextRecord.address) || (record.architecture16Bit != nextRecord.architecture16Bit) || !record.assignedMemory.equals(nextRecord.assignedMemory))
                {
                    break;
                }
                lastMergeIndex = j;
                checkEndAddress = nextRecord.GetEndAddress();
                contiguousBytes += nextRecord.data.length;
            }

            if(lastMergeIndex != i)
            {
                List<String> mergeComments = new ArrayList<String>();

                // Merge the data arrays
                int destPos = 0;
                byte combinedData[] = new byte[record.data.length + contiguousBytes];
                for(int j = i; j <= lastMergeIndex; j++)
                {
                    DataRecord r = records.get(j);
                    System.arraycopy(r.data, 0, combinedData, destPos, r.data.length);
                    destPos += r.data.length;
                    if(r.comment != null)
                    {
                        if(!r.comment.isEmpty())
                        {
                            if((record.comment == null) || record.comment.isEmpty())
                            {
                                record.comment = r.comment;
                            }
                            else if(!record.comment.equals(r.comment))
                            {
                                mergeComments.add(", " + r.comment);
                            }
                        }
                    }
                }
                record.data = combinedData;
                if((record.comment != null) && !record.comment.isEmpty() && !mergeComments.isEmpty())
                {
                    record.comment += CatStringList(mergeComments);
                }
                i = lastMergeIndex;
            }
        }

        records.clear();
        records.addAll(retRecords);
    }


    /**
     * Sorts, removes 0 length records, aligns, pads, coalesces, and resolves
     * duplicate/padded data within overlapping records.
     *
     * For overlapping data, the resolved data is the bitwise AND of the overlap
     * bits, just as a Flash memory would cause if written to twice. Padding and
     * alignment uses 0xFF filler.
     *
     * @param records      List of DataRecord classes to sort and coalesce.
     * @param minAlignment Number of addresses to align+pad all records to
     *
     * @return true
     */
    public static boolean OptimizeRecords(List<DataRecord> records, int minAlignment)
    {
        if(records == null)
        {
            return true;
        }
        if(records.isEmpty())
        {
            return true;
        }

        // Sort the records by address
        Collections.sort(records);

        // Iterate over all records
        for(int i = 0; i < records.size(); i++)
        {
            DataRecord record = records.get(i);
            if(record.data.length == 0)
            {
                records.remove(i--);
                continue;
            }

            // Add pre and post padding to align it to start and end on perfect address boundaries
            record.PadAlign(minAlignment);

            // See if these manipulations allow us to collaece this record with the prior one
            if(i > 0)
            {
                DataRecord lastRecord = records.get(i - 1);
                int addrOverlap = (int)(lastRecord.GetEndAddress() - record.address);
                if(addrOverlap >= 0)
                { // Needs coalescing
                    int byteOverlap = record.architecture16Bit ? addrOverlap / 2 * 3 : addrOverlap;
                    int originalDataLen = lastRecord.data.length;
                    for(int j = 0; j < byteOverlap; j++)
                    {
                        record.data[j] &= lastRecord.data[lastRecord.data.length - byteOverlap + j];
                    }
                    lastRecord.data = Arrays.copyOf(lastRecord.data, lastRecord.data.length + record.data.length - byteOverlap);
                    System.arraycopy(record.data, 0, lastRecord.data, originalDataLen - byteOverlap, record.data.length);
                    records.remove(i--);
                }
            }
        }

        return true;
    }

    /**
     * Aligns all records to the given address boundary size by adding 0xFF
     * padding to the left of the record.
     *
     * The data on the right side is not adjusted, so it may not be aligned
     * perfectly. Ex: with 48-bit Flash word, and one 24-bit instruction in
     * the data record, it is possible that the record is already aligned to a
     * 48-bit boundary and therefore there is a hole for the remaining
     * 24-bits of the Flash word. Use PadAlignRecords() if complete padding and
     * alignment on both sides is desired.
     *
     * @param records
     *                List of data records to process
     * @param address
     *                Address boundary to left-pad the record to. This must be a
     *                binary power of 2. Generally, PIC24 and dsPIC products
     *                will need alignment to 0x2 or 0x4 depending on if they
     *                have a 1 instruction word Flash Word size (24-bits) or 2
     *                instruction word Flash Word size (48-bits), respectively.
     *
     */
    public static void AlignRecords(List<DataRecord> records, long alignBoundaryAddresses)
    {
        if(records == null)
        {
            return;
        }
        if(records.isEmpty())
        {
            return;
        }

        for(DataRecord record : records)
        {
            int leftPadNeeded = (int)(record.address % alignBoundaryAddresses);
            if(leftPadNeeded == 0)
            {
                continue;
            }
            leftPadNeeded = leftPadNeeded / 2 * 3;

            // Create padding and update start address
            byte newData[] = new byte[record.data.length + leftPadNeeded];
            Arrays.fill(newData, 0, leftPadNeeded, (byte)0xFF);
            System.arraycopy(record.data, 0, newData, leftPadNeeded, record.data.length);
            record.data = newData;
            record.address -= record.address % alignBoundaryAddresses;
        }
    }


    /**
     * Aligns all records to the given address boundary size by adding
     * 0xFF padding to the left and padding the right to also meet the given
     * addressing block size.
     *
     * An exception is made if the record start address lies in the 0xF8xxxx
     * address range as byte writable Config word bytes do not take padding.
     *
     * @param records
     *                              List of data records to process
     * @param alignBoundaryAddrSize
     *                              Address boundary to pad/align the record to. This must be a
     *                              binary power of 2. Generally, PIC24 and dsPIC products
     *                              will need alignment to 0x2 or 0x4 depending on if they
     *                              have a 1 instruction word Flash Word size (24-bits) or 2
     *                              instruction word Flash Word size (48-bits), respectively.
     *
     */
    public static void PadAlignRecords(List<DataRecord> records, long alignBoundaryAddrSize)
    {
        if(records == null)
        {
            return;
        }
        if(records.isEmpty())
        {
            return;
        }

        for(DataRecord record : records)
        {
            if(record.architecture16Bit)   // Adjust length on 16-bit platform
            {
                // But, supress padding on Config bytes with real fuses. It is
                // not appropriate to expand these, since they can be written
                // one byte at a time.
                if((record.address & 0xF80000) == 0xF80000)
                {
                    continue;
                }
            }
            record.PadAlign((int)alignBoundaryAddrSize);
        }
    }


    /**
     * Searches a List of DataRecords for the given elements in the specified
     * address range and trims them out of the list, returning trimmed data as a
     * new DataRecord.
     *
     * @param recordsToExtractFrom List of DataRecords to search for the
     *                             extraction addresses
     * @param extractStartAddress  Start address, inclusive, of data we wish to
     *                             remove/return
     * @param extractEndAddress    End address, exclusive, of the data we wish
     *                             to remove/return
     *
     * @return <p>
     * A new DataRecord containing the extracted address range. If
     * there are multiple records containing chunks of the extract
     * range, only the first fragment found is returned (fragment subject to
     * coalescing). In such cases, all chunks are still removed from
     * the recordsToExtractFrom list. See ExtractRanges() for a function that
     * returns all fragments.</p>
     *
     * <p>
     * If no part of the address range exists in the given
     * recordsToExtractFrom, or if recordsToExtractFrom is null, then null is
     * returned.</p>
     */
    public static DataRecord ExtractRange(List<DataRecord> recordsToExtractFrom, long extractStartAddress, long extractEndAddress)
    {
        List<DataRecord> retRecords = ExtractRanges(recordsToExtractFrom, extractStartAddress, extractEndAddress);

        if((retRecords == null) || retRecords.isEmpty())
        {
            return null;
        }
        if(retRecords.size() > 1)
        {
            Collections.sort(retRecords);
            // TODO: Should have an assert or something if we have records that only overlap with 0xFFFFFF padding data
        }
        return retRecords.get(0);
    }

    public static DataRecord ExtractRangeFromSections(List<Section> sectionsToExtractFrom, long extractStartAddress, long extractEndAddress)
    {
        if(sectionsToExtractFrom == null)
            return null;
        
        List<DataRecord> sectionData = new ArrayList<DataRecord>();

        for(Section sec : sectionsToExtractFrom)
        {
            if(sec.data != null)
            {
                sectionData.add(sec.data);
            }
        }

        return DataRecord.ExtractRange(sectionData, extractStartAddress, extractEndAddress);
    }


    /**
     * Searches a List of DataRecords for data within the specified address
     * range and trims them out of the list, returning trimmed data records as a
     * new list of DataRecords. If a data record contains data that straddles
     * the start or end address, it is cloned into a new DataRecord, the data
     * arrays for both DataRecords are trimmed to exactly hit the address range,
     * and the new cloned one is returned. If a data record straddles both start
     * and end addresses, the left part (lowest address range) is retained in
     * recordsToExtractFrom, the middle part cloned and returned, and the right
     * part cloned and inserted into recordsToExtractFrom.
     *
     * @param recordsToExtractFrom List of DataRecords to search for the
     *                             extraction addresses
     * @param extractStartAddress  Start address, inclusive, of data we wish to
     *                             remove/return
     * @param extractEndAddress    End address, exclusive, of the data we wish
     *                             to remove/return
     *
     * @return <p>
     * A list of DataRecords containing the extracted address range.</p>
     *
     * <p>
     * If no part of the address range exists in the given
     * recordsToExtractFrom, an empty list is returned.</p>
     *
     * <p>
     * If recordsToExtractFrom is null, null is returned.</p>
     */
    public static List<DataRecord> ExtractRanges(List<DataRecord> recordsToExtractFrom, long extractStartAddress, long extractEndAddress)
    {
        List<DataRecord> retRecords = new ArrayList<DataRecord>();

        if(recordsToExtractFrom == null)
        {
            return null;
        }


        for(int i = 0; i < recordsToExtractFrom.size(); i++)
        {
            DataRecord record = recordsToExtractFrom.get(i);
            long recEnd = record.GetEndAddress();

            if((record.address < extractEndAddress) && (recEnd > extractStartAddress))
            {
                // Overlap exists, determine how to handle it
                if((record.address >= extractStartAddress) && (recEnd <= extractEndAddress))
                {// Full record enclosed case, just move the record without changing it
                    recordsToExtractFrom.remove(record);
                    retRecords.add(record);
                    i--;
                    continue;
                }
                else if((record.address <= extractStartAddress) && (recEnd >= extractEndAddress))
                {// Full extract range enclosed case, extract middle of record with left and right portions remaining
                    int leftBytes = record.architecture16Bit ? (int)((extractStartAddress - record.address) * 3 / 2) : (int)(extractStartAddress - record.address);
                    int rightBytes = record.architecture16Bit ? (int)((recEnd - extractEndAddress) * 3 / 2) : (int)(recEnd - extractEndAddress);
                    int midBytes = record.data.length - leftBytes - rightBytes;
                    DataRecord right = record.Clone();
                    DataRecord mid = record.Clone();
                    mid.address = extractStartAddress;
                    mid.data = Arrays.copyOfRange(mid.data, leftBytes, leftBytes + midBytes);
                    if(rightBytes > 0)
                    {
                        right.address = extractEndAddress;
                        right.data = Arrays.copyOfRange(right.data, leftBytes + midBytes, leftBytes + midBytes + rightBytes);
                        recordsToExtractFrom.add(right);
                        i++;        // Increment i since we left the left side behind and also added one that can't be overlapping
                    }
                    if(leftBytes > 0)
                    {
                        record.data = Arrays.copyOf(record.data, leftBytes);    // Shrink record bytes to create left side to remain in place within recordsToExtractFrom
                    }
                    else
                    {
                        recordsToExtractFrom.remove(record);
                        i--;
                    }
                    retRecords.add(mid);
                    continue;
                }
                else if(record.address < extractStartAddress)
                {   // Keep left side, return right side
                    int leftBytes = record.architecture16Bit ? (int)((extractStartAddress - record.address) * 3 / 2) : (int)(extractStartAddress - record.address);
                    int rightBytes = record.data.length - leftBytes;
                    DataRecord right = record.Clone();
                    record.data = Arrays.copyOf(record.data, leftBytes);
                    right.address = extractStartAddress;
                    right.data = Arrays.copyOfRange(right.data, leftBytes, leftBytes + rightBytes);
                    retRecords.add(right);
                    continue;
                }
                else
                {// Return left side, keep right side
                    int rightBytes = record.architecture16Bit ? (int)((recEnd - extractEndAddress) * 3 / 2) : (int)(recEnd - extractEndAddress);
                    int leftBytes = record.data.length - rightBytes;
                    DataRecord left = record.Clone();
                    record.data = Arrays.copyOfRange(record.data, leftBytes, leftBytes + rightBytes);
                    left.address = extractEndAddress;
                    left.data = Arrays.copyOfRange(left.data, 0, leftBytes);
                    retRecords.add(left);
                    continue;
                }
            }
        }

        DataRecord.CoalesceRecords(retRecords);
        return retRecords;
    }


    /**
     * Adds 0xFF padding bytes to the left and right of the data in order to
     * meet the specified address alignment. On 16-bit devices, if the data
     * length doesn't start out as a perfect multiple of 3 bytes, pad bytes are
     * first added to make a perfect 3 byte/24-bit/0x2 address block size before
     * adding pre and post padding to meet the address alignment size.
     *
     * @param padAlignAddrSize Program space block size to align and pad to.
     *                         This value is specified in program space
     *                         addresses on 16-bit devices.
     *
     * @param bytePadVal       8-bit value to generate when any padding bytes
     *                         are needed. 0xFF is recommended.
     */
    public void PadAlign(int padAlignAddrSize)
    {
        this.PadAlign(padAlignAddrSize, 0xFF);
    }


    /**
     * Adds padding bytes to the left and right of the data in order to meed the
     * specified address alignment. On 16-bit devices, if the data length
     * doesn't start out as a perfect multiple of 3 bytes, pad bytes are first
     * added to make a perfect 3 byte/24-bit/0x2 address block size before
     * adding pre and post padding to meet the address alignment size.
     *
     * @param padAlignAddrSize Program space block size to align and pad to.
     *                         This value is specified in program space
     *                         addresses on 16-bit devices.
     *
     * @param bytePadVal       8-bit value to generate when any padding bytes
     *                         are needed. 0xFF is recommended.
     */
    public void PadAlign(int padAlignAddrSize, int bytePadVal)
    {
        // Expand 2 byte records on 16-bit devices to 3 bytes so we have a correct 0x2 address word
        if(this.architecture16Bit && (this.data.length % 3 != 0))
        {
            int pad = this.data.length % 3;
            int index = this.data.length;
            this.data = Arrays.copyOf(this.data, this.data.length + pad);    // Add right padding to match an instruciton word boundary
            Arrays.fill(this.data, index, index + pad, (byte)bytePadVal);
        }

        int leftPadNeeded = (int)(this.address % padAlignAddrSize);
        int rightPadNeeded = (int)this.GetEndAddress() % padAlignAddrSize;
        if(rightPadNeeded != 0)
        {
            rightPadNeeded = (int)padAlignAddrSize - rightPadNeeded;
        }
        int leftPadNeededBytes = leftPadNeeded;
        int rightPadNeededBytes = rightPadNeeded;
        if(this.architecture16Bit)
        {
            leftPadNeededBytes = leftPadNeeded / 2 * 3;
            rightPadNeededBytes = rightPadNeeded / 2 * 3;
        }

        // Create padding and update address/length
        byte newData[] = new byte[this.data.length + leftPadNeededBytes + rightPadNeededBytes];
        Arrays.fill(newData, 0, leftPadNeededBytes, (byte)bytePadVal);
        Arrays.fill(newData, this.data.length + leftPadNeededBytes, newData.length, (byte)bytePadVal);
        System.arraycopy(this.data, 0, newData, leftPadNeededBytes, this.data.length);
        this.data = newData;
        this.address -= leftPadNeeded;
    }

    /**
     * Returns a 24-bit value located at the specified address (32-bit value on
     * non-16bit devices). If the address does not lie within the given
     * DataRecord, 0xFFFFFF (0xFFFFFFFF) is returned instead. 
     * 
     * If a full block of  24-bit/32-bits of data does not exist at the 
     * specified address, 0xFF padding bytes are generated (on the big-end of 
     * the word).
     *
     * @param address
     *                Address to read from
     */
    public int GetIntDataAtAddress(long address)
    {
        int index = this.GetDataIndexOfAddress(address);

        if(architecture16Bit)
        {
            if((index < 0) || (index + 3 > this.data.length))
            {
                if(index+2 == this.data.length)
                    return (int)(this.data[index] & 0xFF) | (((int)(this.data[index + 1] & 0xFF)) << 8) | (0xFF << 16);
                if(index+1 == this.data.length)
                    return (int)(this.data[index] & 0xFF) | (0x00FFFF << 8);
                return 0xFFFFFF;
            }
            return (int)(this.data[index] & 0xFF) | (((int)(this.data[index + 1] & 0xFF)) << 8) | (((int)(this.data[index + 2] & 0xFF)) << 16);
        }

        if((index < 0) || (index + 4 > this.data.length))
        {
            if(index+3 == this.data.length)
                return (int)(this.data[index] & 0xFF) | (((int)(this.data[index + 1] & 0xFF)) << 8) | (((int)(this.data[index + 2] & 0xFF)) << 16) | (0xFF<<24);
            if(index+2 == this.data.length)
                return (int)(this.data[index] & 0xFF) | (((int)(this.data[index + 1] & 0xFF)) << 8) | (0xFFFF << 16);
            if(index+1 == this.data.length)
                return (int)(this.data[index] & 0xFF) | (0xFFFFFF << 8);
            return 0xFFFFFFFF;
        }
        return (int)(this.data[index] & 0xFF) | (((int)(this.data[index + 1] & 0xFF)) << 8) | (((int)(this.data[index + 2] & 0xFF)) << 16) | (((int)(this.data[index + 3] & 0xFF)) << 24);
    }

    /**
     * Returns a 16-bit value located at the specified address. If the address
     * does not lie within the given DataRecord, 0xFFFF is returned instead.
     *
     * @param address
     *                Address to read from
     */
    public int GetShortDataAtAddress(long address)
    {
        int index = this.GetDataIndexOfAddress(address);

        if((index < 0) || (index > this.data.length - 2))
        {
            return 0xFFFF;
        }
        return (int)(this.data[index] & 0xFF) | (((int)(this.data[index + 1] & 0xFF)) << 8);
    }

    /**
     * Returns the memory address just after the last byte of data in the
     * record. On 16-bit devices, this will always be a legal, even program
     * memory address, even if the data only occupies 1 or 2 bytes of the
     * instruction word.
     */
    public long GetEndAddress()
    {
        if(architecture16Bit)
        {
            return address + Blob.BytesToMCU16Addresses(data.length);
        }
        else
        {
            return address + data.length;
        }
    }

    public int GetDataIndexOfAddress(long address)
    {
        if(architecture16Bit)
        {
            return (int)((address - this.address) / 2 * 3);
        }
        else
        {
            return (int)(address - this.address);
        }
    }

    public long GetAddressOfDataIndex(int dataIndex)
    {
        if(architecture16Bit)
        {
            return address + ((dataIndex + 2) / 3 * 2);
        }
        else
        {
            return address + dataIndex;
        }
    }

    public AddressRange GetAddressRange()
    {
        return new AddressRange(this.address, this.GetEndAddress());
    }

    /**
     * Returns all used addresses as a List of AddressRanges by the given list
     * of DataRecords.
     *
     * @param dataRecords List of DataRecords to determine the addresses from.
     *                    The architecture16Bit field is observed when trying to
     *                    determine the correct size to address mapping.
     *
     * @return List of AddressRange's corresponding to the given DataRecords.
     */
    static public List<AddressRange> convertToAddressRanges(List<DataRecord> dataRecords)
    {
        List<AddressRange> ret = new ArrayList<AddressRange>();
        for(DataRecord dr : dataRecords)
        {
            ret.add(new AddressRange(dr.address, dr.GetEndAddress()));
        }
        return ret;


    }
}


/**
 * Defines alternate DataRecord sorting order. For 16-bit architectures,
 * anything in Test/Config Program space (address >=0x800000) is placed first,
 * followed by everything in ordinary Flash memory. In other words, 0x800000
 * comes before 0xFFFFFF, which comes before 0x000000, which comes before
 * 0x7FFFFF.
 */
class DRAddressComparatorTestSpaceFirst implements Comparator<DataRecord>
{

    @Override
    public int compare(DataRecord a, DataRecord b)
    {
        if(a.architecture16Bit || b.architecture16Bit)
        {
            long aAddress = (a.address & 0x800000) != 0 ? a.address - 0x1000000 : a.address;
            long bAddress = (b.address & 0x800000) != 0 ? b.address - 0x1000000 : b.address;
            return aAddress < bAddress ? -1 : (aAddress == bAddress ? 0 : 1);
        }
        else
        {
            return a.address < b.address ? -1 : a.address == b.address ? 0 : 1;
        }
    }
};
