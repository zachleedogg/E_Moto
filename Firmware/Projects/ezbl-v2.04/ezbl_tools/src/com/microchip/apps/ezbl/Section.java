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
import java.io.*;
import java.security.*;
import java.util.*;


/**
 *
 * @author C12128
 */
public class Section implements Serializable, Cloneable, Comparable<Section>
{
    public int id = 0;
    public String name = "";
    public String combinedName = "";
    public String sourceFile = "";
    public long size = -1;                  // In linker units. This is bytes for RAM or PC units for anything that is stored in program space (including PSV constants).
    public long virtualMemoryAddress = -1;
    public long loadMemoryAddress = -1;
    public long fileOffset = 0;
    public int alignment = 2;
    public SectionFlags flags = new SectionFlags();
    public List<Symbol> symbols = null;
    public Map<Integer, Symbol> symbolsByAddr = null;
    public DataRecord data = null;
    public boolean isROM = false;
    public boolean isRAM = false;
    public boolean isDebug = false;

    public Section Clone()
    {
        Section ret = new Section();

        ret.alignment = this.alignment;
        ret.combinedName = this.combinedName;
        if(this.data != null)
        {
            ret.data = this.data.Clone();
        }
        ret.fileOffset = this.fileOffset;
        ret.flags = this.flags.Clone();
        ret.id = this.id;
        ret.isDebug = this.isDebug;
        ret.isRAM = this.isRAM;
        ret.isROM = this.isROM;
        ret.loadMemoryAddress = this.loadMemoryAddress;
        ret.name = this.name;
        ret.size = this.size;
        ret.sourceFile = this.sourceFile;
        if(this.symbols != null)
        {
            ret.symbols = new ArrayList<Symbol>(this.symbols.size());
            ret.symbolsByAddr = new HashMap<Integer, Symbol>(this.symbolsByAddr.size());
            for(int i = 0; i < this.symbols.size(); i++)
            {
                Symbol s = this.symbols.get(i).Clone();
                ret.symbolsByAddr.put((Integer)(int)s.address, s);
                ret.symbols.add(s);
            }
        }

        ret.virtualMemoryAddress = this.virtualMemoryAddress;

        return ret;
    }


    public Section()
    {
    }

    public Section(String obj_dump_line)
    {
        //Sections:
        //Idx Name          Size      VMA       LMA       File off  Algn
        //  0 EZBL       00003600  00000200  00000200  0000035c  2**1
        //                  CONTENTS, ALLOC, LOAD, CODE

        // TODO: See if parsing is meaningfully faster when split() isn't used -- split requires regular expression evaluation

        try
        {
            String[] dump = obj_dump_line.split("[\\s]+?");
            List<String> allFlags = new ArrayList<String>();
            int i = 0;
            for(String s : dump)
            {
                if(s.length() == 0)
                {
                    continue;
                }

                switch(i)
                {
                    case 0: // Idx
                        id = Integer.parseInt(s);
                        break;
                    case 1: // Section name
                        name = s;
                        break;
                    case 2: // Size
                        size = Long.parseLong(s, 16);
                        break;
                    case 3: // VMA
                        virtualMemoryAddress = Long.parseLong(s, 16);
                        break;
                    case 4: // LMA
                        loadMemoryAddress = Long.parseLong(s, 16);
                        break;
                    case 5: // File offset
                        fileOffset = Long.parseLong(s, 16);
                        break;
                    case 6: // Alignment
                        alignment = 2 ^ Integer.parseInt(s.substring(s.lastIndexOf('*') + 1));
                        break;
                    case 7: // 7+ Flags
                        i--;
                        s = s.toUpperCase();
                        allFlags.add(s + " ");
                        if(s.endsWith(","))
                        {
                            s = s.substring(0, s.length() - 1);
                        }
                        if(s.equals("CONTENTS"))
                        {
                            flags.CONTENTS = true;
                        }
                        else if(s.equals("ALLOC"))
                        {
                            flags.ALLOC = true;
                        }
                        else if(s.equals("LOAD"))
                        {
                            flags.LOAD = true;
                        }
                        else if(s.equals("CODE"))
                        {
                            flags.CODE = true;
                        }
                        else if(s.equals("ABSOLUTE"))
                        {
                            flags.ABSOLUTE = true;
                        }
                        else if(s.equals("DEBUGGING"))
                        {
                            flags.DEBUGGING = true;
                        }
                        else if(s.equals("DATA"))
                        {
                            flags.DATA = true;
                        }
                        else if(s.equals("NEVER_LOAD"))
                        {
                            flags.NEVER_LOAD = true;
                        }
                        else if(s.equals("PERSIST"))
                        {
                            flags.PERSIST = true;
                        }
                        else if(s.equals("PSV"))
                        {
                            flags.PSV = true;
                        }
                        else if(s.equals("PAGE"))
                        {
                            flags.PAGE = true;
                        }
                        else if(s.equals("READONLY"))
                        {
                            flags.READONLY = true;
                        }
                        else if(s.equals("EDS"))
                        {
                            flags.EDS = true;
                        }
                        else if(s.equals("RELOC"))
                        {
                            flags.RELOC = true;
                        }
                        else if(s.equals("NEAR"))
                        {
                            flags.NEAR = true;
                        }
                        else if(s.equals("REVERSE"))
                        {
                            flags.REVERSE = true;
                        }
                        else if(s.equals("SECURE"))
                        {
                            flags.SECURE = true;
                        }
                        else if(s.equals("XMEMORY"))
                        {
                            flags.XMEMORY = true;
                        }
                        else if(s.equals("YMEMORY"))
                        {
                            flags.YMEMORY = true;
                        }
                        else if(s.equals("MEMORY")) // External memory
                        {
                            flags.MEMORY = true;
                        }
                        else if(s.equals("PACKEDFLASH"))    // __pack_upper_byte
                        {
                            flags.PACKEDFLASH = true;
                        }
                        else if(s.equals("PRESERVED"))
                        {
                            flags.PRESERVED = true;
                        }
                        else if(s.equals("UPDATE"))
                        {
                            flags.UPDATE = true;
                        }
                        else
                        {
                            flags.unknown += "," + s;
                        }
                        break;
                }
                i++;
            }
            if(!flags.unknown.isEmpty())   // Prune off initial comma if there are any unknown flags.
            {
                flags.unknown = flags.unknown.substring(1);
            }
            flags.wholeString = CatStringList(allFlags);
            if(flags.wholeString.endsWith(" "))
            {
                flags.wholeString = flags.wholeString.substring(0, flags.wholeString.length() - 1);
            }

            isDebug = this.flags.DEBUGGING;
            isROM = !this.flags.DEBUGGING && (this.flags.CODE || this.flags.PSV || this.flags.PACKEDFLASH || this.flags.LOAD || (this.flags.CONTENTS && !this.flags.DATA && !this.flags.NEAR && !this.flags.EDS && !this.flags.MEMORY && !this.flags.PERSIST && !this.flags.XMEMORY && !this.flags.YMEMORY && !this.flags.REVERSE));
            isRAM = !isROM && this.flags.ALLOC;
            if((isDebug || isROM || isRAM) == false)
            {
                // TODO: This might not be appropriate. Be sure and check against new compiler features.
                isDebug = true;
            }
        }
        catch(Exception ex)
        {
            name = null;    // Tag that an error occured durring parsing
        }
    }

    /**
     * Compares the 'loadMemoryAddress' element as a signed numerical value.
     *
     * @param y Section to compare against
     *
     * @return 0 if both .loadMemoryAddress elements are equal, -1 if
     *         y.loadMemoryAddress is less than this' loadMemoryAddress, or +1
     *         otherwise.
     */
    @Override
    public int compareTo(Section y) // Needed for calling Collections.sort()
    {
        return this.loadMemoryAddress < y.loadMemoryAddress ? -1 : this.loadMemoryAddress == y.loadMemoryAddress ? 0 : 1;
    }


    /**
     * Returns all used addresses by the given list of sections. The section's
     * loadMemoryAddress and size parameters are used to determine the addresses
     * (not section data).
     *
     * @param sections List of sections to determine the addresses from.
     *
     * @return List of AddressRange's corresponding to the given sections.
     */
    static public List<AddressRange> convertToAddressRanges(List<Section> sections)
    {
        List<AddressRange> ret = new ArrayList<AddressRange>();
        for(Section sec : sections)
        {
            ret.add(new AddressRange(sec.loadMemoryAddress, sec.loadMemoryAddress + sec.size));
        }
        return ret;
    }

    /**
     * Returns all DataRecords contained in the given list of sections.
     *
     * If a section contains a null data field, a new DataRecord is created for
     * the return list based on the section's loadMemoryAddress and size fields
     * and the data is set to all '1's (0xFF bytes). Additionally, to try and
     * maintain consistent downstream handling, any created DataRecord's
     * architecture16Bit field is set to the logical OR of all non-null
     * DataRecords' architecture16Bit value. This adjusts the DataRecord's
     * size for sections that have isROM==true AND architecture16Bit==true.
     *
     * @param sections List of sections to determine the data from.
     *
     * @return List of DataRecord's corresponding to the given sections.
     */
    static public List<DataRecord> convertToDataRecords(List<Section> sections)
    {
        List<DataRecord> ret = new ArrayList<DataRecord>();
        boolean architectureIs16Bit = false;

        // First pass, decide if this is a 16-bit device or not by searching for
        // any true sec.data.architecture16Bit field. This will be needed if we
        // need to create any DataRecords.
        for(Section sec : sections)
        {
            if(sec.data != null)
            {
                if(sec.data.architecture16Bit)
                {
                    architectureIs16Bit = true;
                    break;
                }
            }
        }

        // Second pass, actually copy DataRecord references or create DataRecords
        for(Section sec : sections)
        {
            DataRecord dr;

            dr = sec.data;
            if(dr == null)  // When null, need to create a DataRecord with all '1's set
            {
                dr = new DataRecord();
                dr.address = sec.loadMemoryAddress;
                dr.architecture16Bit = architectureIs16Bit;
                dr.data = new byte[(int)((sec.isROM && dr.architecture16Bit) ? sec.size / 2 * 3 : sec.size)];
                Arrays.fill(dr.data, (byte)0xFF);
            }

            ret.add(dr);
        }

        return ret;
    }


    /**
     * @return the loadMemoryAddress
     */
    public long getLoadMemoryAddress()
    {
        return loadMemoryAddress;
    }

    /**
     * @param loadMemoryAddress the loadMemoryAddress to set
     */
    public void setLoadMemoryAddress(long loadMemoryAddress)
    {
        this.loadMemoryAddress = loadMemoryAddress;
    }

    /**
     * @return the data
     */
    public DataRecord getData()
    {
        return data;
    }

    /**
     * @param data the data to set
     */
    public void setData(DataRecord data)
    {
        this.data = data;
    }

//    /**
//     * Splits the section into two Section classes based on a Load Memory
//     * Address.
//     *
//     * Symbols within the section are retained unchanged in both the original
//     * and split off portion. The DataRecord associated with the section is,
//     * however, adjusted.
//     *
//     * @return null if the address does not exist in the section. Otherwise, the
//     *         right side section is returned (class is trimmed, returned class
//     *         is the one at the higher memory addresses).
//     *
//     */
//    Section SplitAtAddress(long splitAddress)
//    {
//        if((this.loadMemoryAddress > splitAddress) || (this.loadMemoryAddress + this.size < splitAddress))
//        {
//            return null;
//        }
//
//        Section newSection = this.Clone();
//
//        // Update return parameters
//        newSection.loadMemoryAddress = splitAddress;
//        newSection.virtualMemoryAddress += splitAddress - this.loadMemoryAddress;
//        newSection.size = splitAddress - this.loadMemoryAddress;
//        newSection.fileOffset = 0;
//        if(this.data != null)
//        {
//            newSection.data = newSection.data.SplitAtAddress(splitAddress);
//        }
//
//        // Trim off tail from the left side
//        this.size -= splitAddress - this.loadMemoryAddress;
//        if(this.data != null)
//        {
//            this.data.SplitAtAddress(splitAddress);
//        }
//
//        return newSection;
//    }

    /**
     * Splits the section into separate Section classes based on the section's
     * loadMemoryAddress and size.
     *
     * Symbols within the section are retained unchanged in both the original
     * and split off portion. The DataRecords associated with the section are,
     * however, adjusted.
     *
     * @return The section corresponding to the right-side of the split. The
     *         left side is updated in place. If the splitAddress does not fall
     *         in the section, null is returned.
     */
    public Section Split(long splitAddress)
    {
        // Handle right
        if(this.loadMemoryAddress + this.size > splitAddress)
        {
            Section newRight = this.Clone();

            if(this.data != null)
            {
                newRight.data = this.data.SplitAtAddress(splitAddress);
            }
            newRight.size = (this.loadMemoryAddress + this.size) - splitAddress;
            this.size -= newRight.size;
            newRight.loadMemoryAddress += this.size;
            newRight.virtualMemoryAddress += this.size;
            return newRight;
        }

        return null;
    }

    void LoadSymbols(List<Symbol> symbols)
    {
        this.symbols = new ArrayList<Symbol>();
        this.symbolsByAddr = new TreeMap<Integer, Symbol>();
        for(Symbol sym : symbols)
        {
            if(sym.section.equals(this.name))
            {
                if(sym.name.matches("[_A-Za-z0-9]*$"))
                {
                    this.symbolsByAddr.put((Integer)(int)sym.address, sym);
                }
                this.symbols.add(sym);
            }
            if(sym.flags.debugging)
            {
                if(this.sourceFile == null)
                {
                    if(sym.address == this.virtualMemoryAddress)
                    {
                        this.sourceFile = sym.probableFile;
                    }
                }
            }
        }
    }

    /**
     * Given a elf object section data dump created with the command
     * "xc16-objdump -s buildArtifact.elf > elfSectionDump.txt", extracts the
     * data values stored at the same address range occupied by this section.
     *
     * @param elfSectionDump
     *                       Text file contents of the dump created by the
     *                       xc16-objdump compiler tool.
     *
     * @return true if the text file contained data applicable to this section
     *         and was loaded.
     *         false if no data records were created.
     */
    boolean LoadSectionContents(String elfSectionDump)
    {
        int possibleSectionStartIndex;
        int endOffset;
        String sectionContents;
        int wordByteCount;
        List<DataRecord> lines = new ArrayList<DataRecord>();

        //Contents of section .data.LCD_str1:
        // 1390 4275636b 20302e30 30562030 2e303041  Buck 0.00V 0.00A
        //Contents of section .data.Vin_str1:
        // 13a0 565f496e 70757420 3d202020 20202056  V_Input =      V
        //Contents of section .reset:
        // 0000 240204 000000                $.....
        //Contents of section .text:
        // 0224 6f3f21 2efe22 0e0188 000000  o?!.þ"......
        // 022c 000120 200288 0c0007 004122  ..  ......A"
        //Contents of section .config_GSSK:
        // f80004 cfffff                       Ïÿÿ
        //Contents of section .const:
        // 8000 00000000 00000000 00000000 00000000  ................
        // 8010 00000000 00000000 00000000 00000000  ................

        // Find the named section block start index
        //Pattern p = Pattern.compile("(?m)^Contents of section ([^\n]*)\n");
        //Matcher m = p.matcher(elfSectionDump);
        //while(m.find())    // Loop over all sections that have this same name (they don't have to be uniquely named)
        String searchString = "Contents of section " + this.name + ":\n";
        possibleSectionStartIndex = 0;
        while(true)
        {
            possibleSectionStartIndex = elfSectionDump.indexOf(searchString, possibleSectionStartIndex);
            if(possibleSectionStartIndex < 0)
            {
                break;
            }
            possibleSectionStartIndex += searchString.length();

            // Find the section block end index
            endOffset = elfSectionDump.indexOf("\nC", possibleSectionStartIndex);
            if(endOffset < 0)
            {
                endOffset = elfSectionDump.length();
            }

            // Look at only this section data
            sectionContents = elfSectionDump.substring(possibleSectionStartIndex, endOffset);

            // Decode section addresses and if in range, save the data as data records
            int startIndex = 1;
            int eolIndex = 0;
            int asciiPrintIndex;
            int dataSeperatorIndex;
            while(eolIndex >= 0)
            {
                dataSeperatorIndex = sectionContents.indexOf(' ', startIndex + 1);  // +1 is for unneeded starting space on each line
                if(dataSeperatorIndex < 0)
                {
                    break;
                }
                asciiPrintIndex = sectionContents.indexOf("  ", dataSeperatorIndex + 1);  // +1 is for size of the data seperator space
                if(asciiPrintIndex < 0)
                {
                    break;
                }
                wordByteCount = ((sectionContents.indexOf(' ', dataSeperatorIndex + 1) - dataSeperatorIndex) / 2);
                eolIndex = sectionContents.indexOf('\n', asciiPrintIndex + 2);
                long address = Integer.decode("0x" + sectionContents.substring(startIndex, dataSeperatorIndex));
                startIndex = eolIndex + 2;  // +2 is for unneeded \n and space characters

                // Skip parsing this line/section if the content addresses are outside the section (must be an identically named section)
                if((this.loadMemoryAddress > address) || ((this.loadMemoryAddress + this.size) < address))
                {
                    break;
                }

                // Decode the data on this line and add it to the section as a data record
                String encodedData = sectionContents.substring(dataSeperatorIndex + 1, asciiPrintIndex);
                encodedData = encodedData.replaceAll(" ", "");
                int dataCount = encodedData.length() / 2;
                byte data[] = new byte[dataCount];
                for(int i = 0; i < dataCount; i++)
                {
                    data[i] = (byte)((Integer.decode("0x" + encodedData.substring(i * 2, i * 2 + 2))) & 0xFF);
                }

                DataRecord dr = new DataRecord(address, data, wordByteCount == 3);
                lines.add(dr);
            }
        } // Coalesce all data since we created a record for each line of dump file
        DataRecord.CoalesceRecords(lines);
        if(lines.isEmpty())
        {
            return false;
        }
        this.data = lines.get(0);
        return lines.size() == 1;
    }


    /**
     * Returns a 32 byte SHA-256 hash of this Section class. This hash
     * represents
     * everything that is unique to the Section, including section addresses,
     * name, contents, flags, and alignment. Unimportant or derived
     * information,
     * such as the id, fileOffset, sourceFile, and combinedName fields are
     * ignored/not included in the hash.
     */
    byte[] GetHash()
    {
        Section cleanSection = this.Clone();
        cleanSection.id = 0;
        cleanSection.fileOffset = 0;
        cleanSection.sourceFile = null;
        cleanSection.combinedName = null;

        ByteArrayOutputStream byteStream = new ByteArrayOutputStream();
        ObjectOutput objectOut = null;
        try
        {
            objectOut = new ObjectOutputStream(byteStream);
            objectOut.writeObject(this);
            byte[] thisClassBytes = byteStream.toByteArray();
            objectOut.close();

            // Compute SHA-256 hash
            try
            {
                MessageDigest hashComputer = MessageDigest.getInstance("SHA-256");
                return hashComputer.digest(thisClassBytes);
            }
            catch(NoSuchAlgorithmException ex)
            {
                System.err.println("    EZBL: ERROR! Can't find 'SHA-256' hash algorithm. Make sure your JRE includes SHA-256 support.");
                return null;
            }
        }
        catch(IOException ex)
        {
            return null;
        }
    }

    public int getId()
    {
        return id;
    }

    public void setId(int id)
    {
        this.id = id;
    }

    public String getName()
    {
        return name;
    }

    public void setName(String name)
    {
        this.name = name;
    }

    public String getCombinedName()
    {
        return combinedName;
    }

    public void setCombinedName(String combinedName)
    {
        this.combinedName = combinedName;
    }

    public String getSourceFile()
    {
        return sourceFile;
    }

    public void setSourceFile(String sourceFile)
    {
        this.sourceFile = sourceFile;
    }

    public long getSize()
    {
        return size;
    }

    public void setSize(long size)
    {
        this.size = size;
    }

    public long getVirtualMemoryAddress()
    {
        return virtualMemoryAddress;
    }

    public void setVirtualMemoryAddress(long virtualMemoryAddress)
    {
        this.virtualMemoryAddress = virtualMemoryAddress;
    }

    public long getloadMemoryAddress()
    {
        return loadMemoryAddress;
    }

    public void setloadMemoryAddress(long loadMemoryAddress)
    {
        this.loadMemoryAddress = loadMemoryAddress;
    }

    public long getFileOffset()
    {
        return fileOffset;
    }

    public void setFileOffset(long fileOffset)
    {
        this.fileOffset = fileOffset;
    }

    public int getAlignment()
    {
        return alignment;
    }

    public void setAlignment(int alignment)
    {
        this.alignment = alignment;
    }

    public SectionFlags getFlags()
    {
        return flags;
    }

    public void setFlags(SectionFlags flags)
    {
        this.flags = flags;
    }

    public List<Symbol> getSymbols()
    {
        return symbols;
    }

    public void setSymbols(List<Symbol> symbols)
    {
        this.symbols = symbols;
    }

    public boolean isIsROM()
    {
        return isROM;
    }

    public void setIsROM(boolean isROM)
    {
        this.isROM = isROM;
    }

    public boolean isIsRAM()
    {
        return isRAM;
    }

    public void setIsRAM(boolean isRAM)
    {
        this.isRAM = isRAM;
    }

    public boolean isIsDebug()
    {
        return isDebug;
    }

    public void setIsDebug(boolean isDebug)
    {
        this.isDebug = isDebug;
    }
}

