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

import com.microchip.apps.ezbl.ELFReader.e_machine;
import static com.microchip.apps.ezbl.Multifunction.CatStringList;
import static com.microchip.apps.ezbl.Multifunction.FixSlashes;
import static com.microchip.apps.ezbl.Multifunction.TrimQuotes;
import java.io.*;
import java.util.*;
import java.util.zip.*;


/**
 * Converts .elf, .hex, .bl2, .blob file formats to .bl2 or .bin, optimizes and
 * sorts data records by address, performs explicit data manipulation, and/or
 * dumps the input file contents in a human readable report format.
 *
 * @author C12128
 */
public class Blobber
{
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args)
    {
        String stateSavePath = null;
        File hFile; // .hex file
        File eFile; // .elf file
        String outputFilename = null;
        String outputExtension = ".bl2";
        String outputMergeFilename;
        List<DataRecord> rawInputs = new ArrayList<DataRecord>();
        int extensionOffset;
        Bl2b bl2;
        byte buffer[];
        String fileData;
        boolean generate_merge;
        boolean dump;
        boolean binOut = false;
        ELFDumpParser metaParser = null;
        List<AddressRange> voidRanges = new ArrayList<AddressRange>();
        List<AddressRange> fillRanges = new ArrayList<AddressRange>();
        List<Byte> fillValues = new ArrayList<Byte>();
        List<AddressRange> padRanges = new ArrayList<AddressRange>();
        List<Byte> padValues = new ArrayList<Byte>();
        List<AddressRange> crcRanges = new ArrayList<AddressRange>();
        List<Long> crcDestinations = new ArrayList<Long>();
        List<AddressRange> movRanges = new ArrayList<AddressRange>();
        List<Long> movDestinations = new ArrayList<Long>();

        EZBLState state = new EZBLState();

        // Obtain the given command line options and all prior ezbl_tools invocation state information, if needed because of -generate_merge option
        for(String arg : args)
        {
            if(arg.toLowerCase().contains("-generate_merge"))
            {
                stateSavePath = CommandAndBuildState.GetStateFilePath(args);
                state = EZBLState.ReadFromFile(stateSavePath);                  // Load up the preexisting state file from disk, if one exists
                break;
            }
        }
        state = CommandAndBuildState.ReadArgs(state, args);                     // Parse any new command line options and save them

        // Decode special, non-persistent command line options that are specific to the --blobber
        generate_merge = state.undecodedOptions.contains("-generate_merge");
        dump = state.undecodedOptions.contains("-dump");

        // Dump decoded .bl2/.blob/.hex data if the -dump or -dump_hex command 
        // line option is passed in and we have a matching artifact file 
        // extension. This is done early so we don't do unnecessary work or 
        // record sorting which wouldn't make an accurate report of the file 
        // contents.
        if(dump && (state.artifactPath != null) && (state.artifactPath.matches(".*?[.](([hH][eE][xX])|([bB][lL]2)|([bB][lL][oO][bB]))$")))
        {
            String dumpText = "";
            byte fileBytes[] = Multifunction.ReadFileBinary(state.hexPath);

            if(fileBytes == null)
            {
                System.err.println("ezbl_tools: Unable to read \"" + state.hexPath + "\"");
                System.exit(-2);
            }

            if(state.undecodedOptions.contains("-dump_hex"))            // Dumps .hex file records decoded into a human readable report, with any parsing errors listed
            {
                dumpText = Bl2b.HexToReadable(state.artifactPath);
            }
            else if(state.artifactPath.matches(".*?[.][hH][eE][xX]$"))
            {
                dumpText = Bl2b.GetObjDumpView(new String(fileBytes));
            }
            else if(state.artifactPath.matches(".*?[.][bB][lL]2$"))
            {
                dumpText = Bl2b.GetObjDumpView(fileBytes);
            }
            else
            {
                dumpText = Blob.GetObjDumpView(fileBytes, state.MCU16Mode);
            }
            System.out.print(dumpText);
            return;
        }

        // If nothing is specified upon execution, write usage information to STDOUT
        if(!state.parseOkay)
        {
            System.out.print("\r\n"
                             + Multifunction.FormatHelpText(79, 0 * 3, "Usage:")
                             + Multifunction.FormatHelpText(79, 1 * 3, "java -jar ezbl_tools.jar --blobber [-options] -artifact=\"input.elf\"")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Inputs:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "-artifact='input.[elf/bl2/blob/hex]' specifies the input filename. .elf files are recommened as they contain the BOOTID_HASH and APPID_VER meta data that is needed for the .bl2 file output headers. However, a suitable xc16-objdump or xc32-objdump executable, as applicable, must be in the system path for .elf parsing. Other formats supported are .hex, .bl2, and (older) .blob files (do not require objdump in path).")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Outputs:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Input file decoded from .hex/.elf format and reencoded as a binary .bl2 file. The output filename is input.bl2.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "-dump")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Decodes and prints the decoded contents of the specified input artifact to stdout in human readable ASCII format.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "-dump_hex")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Alternate raw decoding of .hex file contents in human readable form. This option should only be used for .hex file inputs.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "-bin")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Writes a raw binary raw binary output file without any addressing information contained in it. Record addresses, with all padding/filler/adjustments completed, are sorted numerically before stripping addressing information.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "-raw=address:filename.ext[,addr2:filename2.ext,addr3:filename3.ext,...]")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Inserts the binary file contents of the specified file starting at the specified address. If data already exists within the given address range required, it is overwritten.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "-void=StartAddr,EndAddr[,StartAddr2,EndAddr2,...]")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Makes data, if any, dissappear from the specified starting address up to, but not including, the specified end address before generating the final output.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "-fill=StartAddr,EndAddr,byteVal[,StartAddr2,EndAddr2,byteVal2,...]")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Generates a specified byte value over all locations between the specified start address (inclusive) and end address (exclusive). If input data is already present in the address range, it is overwritten with fill bytes.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "-pad=StartAddr,EndAddr,byteVal[,StartAddr2,EndAddr2,byteVal2,...]")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Generates a specified byte value over all locations between the specified start address (inclusive) and end address (exclusive). If input data is already present in the address range, the existing data is retained with pad bytes only inserted into addressing gaps.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "-crc32=StartAddr,EndAddr,InsertAddr[,StartAddr2,EndAddr2,InsertAddr2,...]")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Generates a CRC32 (IEEE 802.3 Ethernet version) over data within the specified address range, then inserts this value at a specific address.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "-mov=SrcStartAddr,EndAddr,DestStartAddr[,SrcStartAddr2,EndAddr2,DestStartAddr2,...]")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Moves data from a given address range to a new block location. If existing data resides on the destination range, and there is source range data defined for the same target location, the existing destination data is overwritten.")
                             + "\r\n"
            // These are commented out since they are either not implemented, not tested, or known to need more work.
            //                             + Multifunction.FormatHelpText(79, 2 * 3, "-set=")
            //                             + Multifunction.FormatHelpText(79, 3 * 3, "-set=0xADDR:val0val1val2...,0xADDR2:val3val4val5...,...")
            //                             + "\r\n"
            //                             + Multifunction.FormatHelpText(79, 2 * 3, "-ior=")
            //                             + Multifunction.FormatHelpText(79, 3 * 3, "-ior=0xADDR:val0val1val2...,0xADDR2:val3val4val5...,...")
            //                             + "\r\n"
            //                             + Multifunction.FormatHelpText(79, 2 * 3, "-and=")
            //                             + Multifunction.FormatHelpText(79, 3 * 3, "-and=0xADDR:val0val1val2...,0xADDR2:val3val4val5...,...")
            //                             + "\r\n"
            //                             + Multifunction.FormatHelpText(79, 2 * 3, "-xor=")
            //                             + Multifunction.FormatHelpText(79, 3 * 3, "-xor=0xADDR:val0val1val2...,0xADDR2:val3val4val5...,...")
            //                             + "\r\n"
            );

            return;
        }

        List<String> tokens = new ArrayList<String>();
        tokens.addAll(Arrays.asList(args));
        for(String token : tokens)
        {
            String lowerToken = token.toLowerCase();

            if(lowerToken.startsWith("-raw="))  // Add binary external file contents as a datarecord within the output memory, starting at the specified address.
            {
                String rawParams[] = Multifunction.TrimQuotes(token.substring("-raw=".length())).split(",");
                for(String raw : rawParams)
                {
                    DataRecord dr = new DataRecord(true);
                    String addrFile[] = Multifunction.TrimQuotes(raw).split(":", 2);
                    String errorString = String.format("\nezbl_tools: Invalid format for 'raw' parameter. Use:"
                                                       + "\n  -raw=address:filename.ext[,addr2:filename2.ext,addr3:filename3.ext,...]:"
                                                       + "\n  \"%1$s\""
                                                       + "\n", raw
                    );
                    try
                    {
                        dr.address = Long.decode(TrimQuotes(addrFile[0]));
                    }
                    catch(Exception ex)
                    {
                        System.err.print(errorString);
                        continue;
                    }
                    String filePath = FixSlashes(TrimQuotes(addrFile[1]));
                    File f = new File(filePath);
                    if(!f.canRead())
                    {
                        System.err.printf("ezbl_tools: Can't read \"%1$s\"\n", addrFile[1]);
                        continue;
                    }
                    dr.data = Multifunction.ReadFileBinary(f.getPath());
                    rawInputs.add(dr);
                    if(outputFilename == null)
                    {
                        outputFilename = f.getPath();
                    }
                }
            }
            else if(lowerToken.startsWith("-bin"))  // Generate a binary output file containing no address information, record headers, or .bl2 file format data. Output is simply flat flash contents with record data concatenated together, even if there is an address gap between them.
            {
                binOut = true;
                outputExtension = ".bin";
            }
            else if(lowerToken.startsWith("-void="))    // Nuke all data within the address range from the output file. No records will have data defined within the address range(s).
            {
                String params[] = Multifunction.TrimQuotes(token.substring("-void=".length())).split(",");
                int i = 0;
                AddressRange ar = new AddressRange((long)-1, (long)-1);
                for(String param : params)
                {
                    i++;
                    String errorString = String.format("\nezbl_tools: Invalid format for 'void' parameter. Use:"
                                                       + "\n  -void=StartAddr,EndAddr[,StartAddr2,EndAddr2,...]:"
                                                       + "\n  \"%1$s\""
                                                       + "\n", param);
                    try
                    {
                        if((i & 0x1) == 0x1)    // Start Address
                        {
                            ar.startAddress = Long.decode(TrimQuotes(param));
                        }
                        else
                        {
                            ar.endAddress = Long.decode(TrimQuotes(param));
                            if(ar.startAddress != (long)-1)
                            {
                                voidRanges.add(ar);
                                ar = new AddressRange((long)-1, (long)-1);
                            }
                        }
                    }
                    catch(Exception ex)
                    {
                        System.err.print(errorString);
                        continue;
                    }
                }
            }
            else if(lowerToken.startsWith("-fill="))
            {
                String params[] = Multifunction.TrimQuotes(token.substring("-fill=".length())).split(",");
                int i = 0;
                AddressRange ar = new AddressRange((long)-1, (long)-1);
                byte fillVal;
                for(String param : params)
                {
                    i++;
                    String errorString = String.format("\nezbl_tools: Invalid format for 'fill' parameter. Use:"
                                                       + "\n  -fill=StartAddr,EndAddr,byteVal[,StartAddr2,EndAddr2,byteVal2,...]:"
                                                       + "\n  \"%1$s\""
                                                       + "\n", param);
                    try
                    {
                        switch(i % 0x3)
                        {
                            case 0x1:   // Start Address
                                ar.startAddress = Long.decode(TrimQuotes(param));
                                break;

                            case 0x2:   // End Address
                                ar.endAddress = Long.decode(TrimQuotes(param));
                                break;

                            default:    // Fill byte value
                                fillVal = Integer.decode(TrimQuotes(param)).byteValue();
                                if((ar.startAddress != (long)-1) && (ar.endAddress != (long)-1))
                                {
                                    fillRanges.add(ar);
                                    ar = new AddressRange((long)-1, (long)-1);
                                    fillValues.add(fillVal);
                                }
                                break;
                        }
                    }
                    catch(NumberFormatException ex)
                    {
                        System.err.print(errorString);
                    }
                }
            }
            else if(lowerToken.startsWith("-pad="))
            {
                String params[] = Multifunction.TrimQuotes(token.substring("-pad=".length())).split(",");
                int i = 0;
                AddressRange ar = new AddressRange((long)-1, (long)-1);
                byte padVal;
                for(String param : params)
                {
                    i++;
                    String errorString = String.format("\nezbl_tools: Invalid format for 'pad' parameter. Use:"
                                                       + "\n  -pad=StartAddr,EndAddr,byteVal[,StartAddr2,EndAddr2,byteVal2,...]:"
                                                       + "\n  \"%1$s\""
                                                       + "\n", param);
                    try
                    {
                        switch(i % 0x3)
                        {
                            case 0x1:   // Start Address
                                ar.startAddress = Long.decode(TrimQuotes(param));
                                break;

                            case 0x2:   // End Address
                                ar.endAddress = Long.decode(TrimQuotes(param));
                                break;

                            default:    // Fill byte value
                                padVal = Integer.decode(TrimQuotes(param)).byteValue();
                                if((ar.startAddress != (long)-1) && (ar.endAddress != (long)-1))
                                {
                                    padRanges.add(ar);
                                    ar = new AddressRange((long)-1, (long)-1);
                                    padValues.add(padVal);
                                }
                                break;
                        }
                    }
                    catch(NumberFormatException ex)
                    {
                        System.err.print(errorString);
                    }
                }
            }
            else if(lowerToken.startsWith("-crc32="))
            {
                String params[] = Multifunction.TrimQuotes(token.substring("-crc32=".length())).split(",");
                int i = 0;
                AddressRange ar = new AddressRange((long)-1, (long)-1);
                long convert;
                for(String param : params)
                {
                    i++;
                    String errorString = String.format("\nezbl_tools: Invalid format for 'crc32' parameter. Use:"
                                                       + "\n  -crc32=StartAddr,EndAddr,InsertAddr[,StartAddr2,EndAddr2,InsertAddr2,...]:"
                                                       + "\n  \"%1$s\""
                                                       + "\n", param);
                    try
                    {
                        convert = Long.decode(TrimQuotes(param));
                    }
                    catch(Exception ex)
                    {
                        System.err.print(errorString);
                        continue;
                    }
                    switch(i % 3)
                    {
                        case 0x1:   // Start address for data to include in CRC computation
                            ar.startAddress = convert;
                            break;
                        case 0x2:   // End address for data to include in CRC computation
                            ar.endAddress = convert;
                            break;
                        default:    // Computed CRC32 insertion address (destination write address)
                            if((ar.startAddress != (long)-1) && (ar.endAddress != (long)-1))
                            {
                                crcDestinations.add(convert);
                                crcRanges.add(ar);
                                ar = new AddressRange(-1, -1);
                            }
                            break;
                    }
                }
            }
            else if(lowerToken.startsWith("-mov="))
            {
                String params[] = Multifunction.TrimQuotes(token.substring("-mov=".length())).split(",");
                int i = 0;
                AddressRange ar = new AddressRange((long)-1, (long)-1);
                for(String param : params)
                {
                    i++;
                    String errorString = String.format("\nezbl_tools: Invalid format for 'mov' parameter. Use:"
                                                       + "\n  -mov=SrcStartAddr,EndAddr,DestStartAddr[,SrcStartAddr2,EndAddr2,DestStartAddr2,...]:"
                                                       + "\n  \"%1$s\""
                                                       + "\n", param);
                    try
                    {
                        switch(i % 0x3)
                        {
                            case 0x1:   // Start Src address
                                ar.startAddress = Long.decode(TrimQuotes(param));
                                break;

                            case 0x2:   // End Src address
                                ar.endAddress = Long.decode(TrimQuotes(param));
                                break;

                            default:    // Start Dest address
                                long destAddr = Long.decode(TrimQuotes(param));
                                if((ar.startAddress != (long)-1) && (ar.endAddress != (long)-1))
                                {
                                    movRanges.add(ar);
                                    ar = new AddressRange((long)-1, (long)-1);
                                    movDestinations.add(destAddr);
                                }
                                break;
                        }
                    }
                    catch(NumberFormatException ex)
                    {
                        System.err.print(errorString);
                        continue;
                    }
                }
            }
        }

        if((state.artifactPath == null) && rawInputs.isEmpty())
        {
            System.err.println("ezbl_tools: Missing input filename. Specify -artifact=input.hex/.elf/.blob/.bl2 or -raw parameter.");
            System.exit(-1);
        }

        // If artifact is an .elf, convert to .hex first
        if((state.artifactPath != null) && state.artifactPath.matches(".*?[.][eE][lL][fF]$"))
        {
            state.artifactPath = TrimQuotes(FixSlashes(state.artifactPath.trim()));
            int elfMachine = ELFReader.Machine(state.artifactPath);
            String toolPrefix = "elf";

            if(elfMachine == e_machine.EM_MIPS)
            {
                state.MCU16Mode = false;
                toolPrefix = "xc32";
                if(state.hexAlign == 0)
                {
                    state.hexAlign = state.hexAlign = 8;    // Use 0x8 for PIC32MM. Must match Flash minimum write block size if ECC is implemented.
                }
            }
            else if(elfMachine == e_machine.EM_PIC30)
            {
                state.MCU16Mode = true;
                toolPrefix = "xc16";
            }
            else if(elfMachine == e_machine.EM_XC8)
            {
                System.err.printf("ezbl_tools.jar: PIC16/PIC18 .elf files are not supported as blobber input\n"
                                  + "                artifacts. Supply a .hex file instead.\n");
                System.exit(-18);
            }
            List<String> cmdLine = new ArrayList<String>();
            cmdLine.add(((state.compilerFolder != null) ? state.compilerFolder + File.separator : "") + toolPrefix + "-objdump");
            //cmdLine.add("--full-contents");                       // Might want this in the future so we don't have to convert .elf to .hex using the (slower) compiler tools.
            cmdLine.add("--syms");
            cmdLine.add(state.artifactPath);
            metaParser = new ELFDumpParser();
            if(metaParser.startObjDump(cmdLine) < 0)
            {
                metaParser = null;
                if(!state.silent)
                {
                    System.err.println("ezbl_tools: " + CatStringList(cmdLine, " "));
                }
            }

            state.hexPath = state.artifactPath.replaceFirst("[.][eE][lL][fF]$", ".hex");
            eFile = new File(state.artifactPath);
            hFile = new File(state.hexPath);
            if(hFile.lastModified() < eFile.lastModified()) // Create .hex from .elf only if the .hex file doesn't exist (0 time) or is older than the .elf file
            {
                cmdLine.clear();
                cmdLine.add(((state.compilerFolder != null) ? state.compilerFolder + File.separator : "") + toolPrefix + "-bin2hex");
                cmdLine.add("-a");  // Sort sections by address
                if(state.MCU16Mode)
                {
                    cmdLine.add("-omf=elf");
                }
                cmdLine.add(state.artifactPath);
                ProcessBuilder proc = new ProcessBuilder(cmdLine);
                try
                {
                    proc.redirectErrorStream(true);
                    Process ps = proc.start();
                    ps.waitFor();
                }
                catch(IOException e)
                {
                    if(!state.silent)
                    {
                        System.err.println("ezbl_tools: " + e.getMessage());
                    }
                    System.exit(-1);
                    return;
                }
                catch(InterruptedException ex)
                { // No one to trigger this
                }
            }
        }

        if(state.hexAlign == 0)
        {
            state.hexAlign = state.flashWordSize;
        }

        if(state.hexPath == null)
        {
            bl2 = new Bl2b(state.MCU16Mode);
        }
        else
        {
            // Convert .hex (and .elf converted to .hex) file contents to to 
            // binary Bl2b class (if applicable)
            extensionOffset = state.hexPath.toLowerCase().lastIndexOf(".hex");
            if(extensionOffset >= 0)
            {
                fileData = Multifunction.ReadFile(state.hexPath, true);
                if(fileData == null)
                {
                    System.err.println("ezbl_tools: Unable to open \"" + state.hexPath + "\"");
                    System.exit(-3);
                }

                bl2 = new Bl2b(fileData);
                state.MCU16Mode = bl2.architecture16Bit;   // .hex file decoder could have changed the architecture if the addresses do not make sense for 16-bit devices

                // Strip off .hex file extension for the output, assuming there will be an output
                outputFilename = state.hexPath.substring(0, extensionOffset);
            }
            else    // .blob or .bl2 input
            {
                byte binaryFileData[] = Multifunction.ReadFileBinary(state.hexPath);
                if(binaryFileData == null)
                {
                    System.err.println("ezbl_tools: Unable to open \"" + state.hexPath + "\"");
                    System.exit(-4);
                    return;
                }

                try
                {
                    if(state.hexPath.toLowerCase().lastIndexOf(".blob") >= 0)
                    {
                        Blob b = new Blob(binaryFileData, state.MCU16Mode);
                        bl2 = new Bl2b(b, null, 0, 0, 0);
                    }
                    else    // Native .bl2 file
                    {
                        bl2 = new Bl2b(binaryFileData, null);
                    }
                }
                catch(Exception ex)
                {
                    System.err.println("ezbl_tools: " + ex.toString());
                    System.exit(-5);
                    return;
                }

                outputFilename = state.hexPath.replaceAll("([.]([hH][eE][xX])|([bB][lL][oO][bB])|([bB][lL]2))$", "");
            }
        }

        // Collect BOOTID_HASHx and APPID_VER_MAJOR/APPID_VER_MINOR/APPID_VER_BUILD 
        // meta data, if available from .elf files and add it to the Bl2b class
        if(metaParser != null)
        {
            metaParser.parseAllObjOutput(0);
            for(Symbol s : metaParser.getSymbols())
            {
                String indexStr = "";
                if(s.name.startsWith("_BOOTID_HASH"))   // XC16 name
                {
                    indexStr = (String)s.name.subSequence("_BOOTID_HASH".length(), s.name.length());
                }
                if(s.name.startsWith("BOOTID_HASH"))    // XC32 name
                {
                    indexStr = (String)s.name.subSequence("BOOTID_HASH".length(), s.name.length());
                }
                try
                {
                    int i = Integer.decode(indexStr);
                    if((i >= 0) && (i < bl2.bootIDHash.length)) // Successfuly parsed something?
                    {
                        bl2.bootIDHash[i] = (int)s.address;
                    }
                }
                catch(NumberFormatException ex)
                {
                }

                if(s.name.startsWith("_APPID_VER_BUILD") || s.name.startsWith("APPID_VER_BUILD"))
                {
                    bl2.appIDVerBuild = (int)s.address;
                }
                if(s.name.startsWith("_APPID_VER_MINOR") || s.name.startsWith("APPID_VER_MINOR"))
                {
                    bl2.appIDVerMinor = ((int)s.address) & 0x0000FFFF;
                }
                if(s.name.startsWith("_APPID_VER_MAJOR") || s.name.startsWith("APPID_VER_MAJOR"))
                {
                    bl2.appIDVerMajor = ((int)s.address) & 0x0000FFFF;
                }
            }
        }

        // Dump decoded human readable data if the -dump command line option was given with a .elf input artifact
        if(dump)
        {
            System.out.print(bl2.GetObjDumpView());
            return;
        }

        // Trim off the .production or .debug strings from the end of the
        // outputFilename for use when generating _merge.gld and _merge.s files.
        // This is done so you don't need a separate set of files in the MPLAB X
        // project when compiling the application and want to debug the
        // bootloader project.
        outputMergeFilename = outputFilename.replaceFirst("([.]production$)|([.]debug$)", "") + ".merge";

        // Optimize records
        bl2.CoalesceAdjacentDataRecords();
        bl2.AlignRecords(state.hexAlign);
        bl2.CoalesceAdjacentDataRecords();

        // Remove any existing data for each raw input given.
        // We must do this iteratively, since it is possible for each raw input
        // file to overlap with other raw input files. Order matters.
        if(!rawInputs.isEmpty())
        {
            bl2.RemoveIgnoreRangeByDR(rawInputs.get(0));
            for(int i = 0; i < rawInputs.size(); i++)
            {
                DataRecord dr = rawInputs.get(i);
                bl2.records.add(dr);
                bl2.RemoveIgnoreRangesByDR(rawInputs.subList(i + 1, rawInputs.size()));
            }

            bl2.CoalesceAdjacentDataRecords();
            bl2.AlignRecords(state.hexAlign);
            bl2.CoalesceAdjacentDataRecords();
        }

        if(generate_merge)          // Used for making merge.S and merge.gld files for an Application to link with
        {
            // Create a post-linking, absolute merge.S assembly file to represent the
            // bl2 contents
            String[] mergeScripts = GLDBuilder.CreateMergeScript(state, bl2, metaParser);
            if(mergeScripts.length != 2)    // Avoid progression if a fatal error was flagged rather than generating the merge .gld script.
            {
                System.exit(-6);
                return;
            }

            // Modify the requested output.gld file with our new linker information
            if(mergeScripts[0] != null)
            {
                if(Multifunction.UpdateFileIfDataDifferent(outputMergeFilename + ".gld", mergeScripts[0], true) < 0)
                {
                    System.err.println("ezbl_tools: failed to write to \"" + outputMergeFilename + ".gld\"");
                    System.exit(-7);
                    return;
                }
            }

            // Modify the requested output.s file with our new bootloader static information
            if(mergeScripts[1] != null)
            {
                if(Multifunction.UpdateFileIfDataDifferent(outputMergeFilename + ".S", mergeScripts[1], true) < 0)
                {
                    System.err.println("ezbl_tools: failed to write to \"" + outputMergeFilename + ".S\"");
                    System.exit(-8);
                    return;
                }
            }
        }
        else
        {
            DataRecord.OptimizeRecords(bl2.records, state.hexAlign);
        }

        if(!generate_merge && !state.romUseRanges.isEmpty())  // Remove reserved address sections for (obsolete) build_as_one mode
        {
            bl2.RemoveIgnoreRanges(state.romUseRanges);

            // Recoalesce and realign everything since we may have split or removed
            // records above. Unaligned records can be in the merge .gld file, but
            // are best removed from the .bl2 so bootloaders or other consumers
            // don't have to handle unaligned data.
            bl2.CoalesceAdjacentDataRecords();
            bl2.AlignRecords(state.hexAlign);
            bl2.CoalesceAdjacentDataRecords();
        }

        if(state.ignoreROMRanges != null)
        {
            bl2.RemoveIgnoreRanges(state.ignoreROMRanges);
            bl2.CoalesceAdjacentDataRecords();
            bl2.AlignRecords(state.hexAlign);
            bl2.CoalesceAdjacentDataRecords();
        }

        // Handle -fill= range overwrite/insertion of repeating data byte
        for(int i = 0; i < fillRanges.size(); i++)
        {
            AddressRange ar = fillRanges.get(i);
            DataRecord.ExtractRange(bl2.records, ar.startAddress, ar.endAddress);  // Discards everything overlapping
            DataRecord dr = new DataRecord();
            dr.address = ar.startAddress;
            dr.architecture16Bit = bl2.architecture16Bit;
            int fillSize = bl2.architecture16Bit ? (int)(ar.endAddress - ar.startAddress) * 3 / 2 : (int)(ar.endAddress - ar.startAddress);
            dr.data = new byte[fillSize];
            Arrays.fill(dr.data, fillValues.get(i));
            bl2.records.add(dr);
        }
        // Handle -pad= range insertion of repeating data byte
        for(int i = 0; i < padRanges.size(); i++)
        {
            AddressRange ar = padRanges.get(i);
            List<DataRecord> drs = DataRecord.ExtractRanges(bl2.records, ar.startAddress, ar.endAddress);
            if((drs == null) || (drs.isEmpty()))
            {   // No data existed in the given range, just create a new data range as like fill data
                DataRecord dr = new DataRecord();
                dr.address = ar.startAddress;
                dr.architecture16Bit = bl2.architecture16Bit;
                int padSize = bl2.architecture16Bit ? (int)(ar.endAddress - ar.startAddress) * 3 / 2 : (int)(ar.endAddress - ar.startAddress);
                dr.data = new byte[padSize];
                Arrays.fill(dr.data, padValues.get(i));
                bl2.records.add(dr);
                continue;
            }
            Collections.sort(drs);
            DataRecord.CoalesceRecords(drs);
            for(int drIndex = 0; drIndex < drs.size(); drIndex++)
            {   // Extend existing records with padding on left side, except for the last record, which gets padding on both the left and right sides, as applicable
                DataRecord dr = drs.get(drIndex);
                int leftPadSize = dr.architecture16Bit ? (int)((dr.address - ar.startAddress) * 3 / 2) : (int)(dr.address - ar.startAddress);
                int rightPadSize = 0;
                if(drIndex == drs.size() - 1)
                {
                    rightPadSize = dr.architecture16Bit ? (int)((ar.endAddress - dr.GetEndAddress()) * 3 / 2) : (int)(ar.endAddress - dr.GetEndAddress());
                }

                byte newData[] = new byte[dr.data.length + leftPadSize + rightPadSize];
                Arrays.fill(newData, 0, leftPadSize, padValues.get(i));
                Arrays.fill(newData, leftPadSize + dr.data.length, newData.length, padValues.get(i));
                System.arraycopy(dr.data, 0, newData, leftPadSize, dr.data.length);
                dr.data = newData;
                dr.address = ar.startAddress;
                bl2.records.add(dr);  // Put new padded array back into the bl2
                ar.startAddress = dr.GetEndAddress();
                continue;
            }
        }
        // Handle -void= command line option address range removal
        for(AddressRange ar : voidRanges)
        {
            DataRecord.ExtractRange(bl2.records, ar.startAddress, ar.endAddress);
        }
        // Handle -mov= data range movement
        for(int i = 0; i < movRanges.size(); i++)
        {
            AddressRange ar = movRanges.get(i);
            List<DataRecord> drs = DataRecord.ExtractRanges(bl2.records, ar.startAddress, ar.endAddress);
            for(DataRecord dr : drs)
            {
                dr.address -= ar.startAddress - movDestinations.get(i);
                bl2.records.add(dr);
            }
        }
        // Handle -crc32= CRC32 computation and insertion
        for(int i = 0; i < crcRanges.size(); i++)
        {
            AddressRange ar = crcRanges.get(i);
            DataRecord crcDR = new DataRecord();
            crcDR.address = crcDestinations.get(i);
            crcDR.architecture16Bit = bl2.architecture16Bit;
            List<DataRecord> drs = DataRecord.ExtractRanges(bl2.records, ar.startAddress, ar.endAddress);
            crcDR.data = new byte[bl2.architecture16Bit ? 6 : 4];
            CRC32 myCRC = new CRC32();
            long crcValue;
            if(crcDR.data.length > 4)
            {
                crcDR.data[4] = (byte)0xFF;
                crcDR.data[5] = (byte)0xFF;
            }
            if((drs == null) || (drs.isEmpty()))
            {   // No data exists in the given range, just create a new data range for the default CRC over 0 bytes
                crcValue = myCRC.getValue();
                crcDR.data[0] = (byte)(crcValue & 0xFF);
                crcDR.data[1] = (byte)((crcValue >> 8) & 0xFF);
                crcDR.data[2] = (byte)((crcValue >> 16) & 0xFF);
                crcDR.data[3] = (byte)((crcValue >> 24) & 0xFF);
                DataRecord.ExtractRange(bl2.records, crcDR.address, crcDR.address + 0x4);  // Throw away destination address range if it exists in the input
                bl2.records.add(crcDR);                                                    // Write computed CRC value
                continue;
            }
            Collections.sort(drs);
            DataRecord.CoalesceRecords(drs);
            for(DataRecord dr : drs)                    // Compute CRC for 1+ records
            {
                myCRC.update(dr.data);
                bl2.records.add(dr);                   // Put this record back from where it came from
            }
            crcValue = myCRC.getValue();
            crcDR.data[0] = (byte)(crcValue & 0xFF);
            crcDR.data[1] = (byte)((crcValue >> 8) & 0xFF);
            crcDR.data[2] = (byte)((crcValue >> 16) & 0xFF);
            crcDR.data[3] = (byte)((crcValue >> 24) & 0xFF);
            DataRecord.ExtractRange(bl2.records, crcDR.address, crcDR.address + 0x4);  // Throw away destination address range if it exists in the input
            bl2.records.add(crcDR);                                                    // Write computed CRC value
            if(!state.silent)
            {
                System.out.printf("    CRC32 from 0x%1$08X to 0x%2$08X = 0x%3$08X (stored at 0x%4$08X)\n", ar.startAddress, ar.endAddress, crcValue, crcDR.address);
            }
        }

        if(!fillRanges.isEmpty() || !padRanges.isEmpty() || !movRanges.isEmpty() || !crcRanges.isEmpty())
        {
            bl2.CoalesceAdjacentDataRecords();
        }

        if(binOut)        // Write .bin file with records sorted and no addressing information
        {
            Collections.sort(bl2.records);
            buffer = bl2.GetBinBytes();
            if(Multifunction.UpdateFileIfDataDifferent(outputFilename + outputExtension, buffer) < 0)
            {
                System.err.println("ezbl_tools: Failed to write processed data to \"" + outputFilename + outputExtension + "\"");
                System.exit(-10);
                return;
            }
            return;
        }

        // Do one last final reordering of records for FBOOT, Config Words or
        // other Test Segment data for serial upload to a bootloader
        bl2.ReorderForSequentialAccess();

        // Convert list of records into a blobbed byte array
        buffer = bl2.GetBytes();

        // If an encryption password is supplied and saveTemps is turned on,
        // write the unencrypted .bl2 output file too for debugging purposes
        if((state.encryptionPassword != null) && state.saveTemps)
        {
            if(Multifunction.UpdateFileIfDataDifferent(outputFilename + "_no_encrypt" + outputExtension, buffer) < 0)
            {
                System.err.println("ezbl_tools: Failed to write processed data to \"" + outputFilename + "_no_encrypt" + outputExtension + "\"");
                System.exit(-9);
                return;
            }
        }

        // Encrypt the bl2 bytes, if a password was supplied
        if(state.encryptionKey != null)
        {
            byte encryptedBuffer[] = Blob.EncryptBlobBytes(state, buffer);  // TODO: Update for encrypting .bl2 files. This is using the former .blob algorithm.
            buffer = encryptedBuffer;
        }

        // Write .bl2 output file
        if(Multifunction.UpdateFileIfDataDifferent(outputFilename + outputExtension, buffer) < 0)
        {
            System.err.println("ezbl_tools: Failed to write processed data to \"" + outputFilename + outputExtension + "\"");
            System.exit(-10);
            return;
        }

        // Report success and print internal bl2 hash to user
        if(!state.silent)
        {
            int i = buffer.length - 4 - 32;
            String hashString = String.format("%1$02x%2$02x%3$02x%4$02x%5$02x%6$02x%7$02x%8$02x%9$02x%10$02x%11$02x%12$02x%13$02x%14$02x%15$02x%16$02x%17$02x%18$02x%19$02x%20$02x%21$02x%22$02x%23$02x%24$02x%25$02x%26$02x%27$02x%28$02x%29$02x%30$02x%31$02x%32$02x",
                                              buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++],
                                              buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++], buffer[i++]);
            System.out.printf("    BL2 content SHA-256 hash + CRC32 (stored at offset 0x%1$08X) is: %2$s, 0x%6$02X%5$02X%4$02X%3$02X\n", buffer.length - 4 - 32, hashString, buffer[i++], buffer[i++], buffer[i++], buffer[i++]);
            System.out.printf("    Successfully wrote %1$d bytes to %2$s\n", buffer.length, outputFilename + outputExtension);
        }
    }
}
