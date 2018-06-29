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
import static com.microchip.apps.ezbl.Multifunction.FixSlashes;
import java.io.*;
import java.util.*;


/**
 *
 * @author C12128
 */
public class ObjDumpFilter
{
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args)
    {
        String elfReaderPath = "xc16-objdump";
        String artifacts[];
        ELFDumpParser parsers[];
        boolean decodeRestartDataInit = false;
        boolean decodeDataInit = false;
        boolean generateFastDataInitCode = false;
        boolean generateFastDataInitCodeSummary = false;


        // Parse the given command line options
        EZBLState state = new EZBLState();
        state = CommandAndBuildState.ReadArgs(state, args);                     // Parse any new command line options and save them

        // If nothing is specified upon execution, write usage information to STDOUT
        if(!state.parseOkay)
        {
            System.out.print(
                    "\r\n"
                    + Multifunction.FormatHelpText(79, 0 * 3, "Usage:")
                    + Multifunction.FormatHelpText(79, 1 * 3, "java -jar ezbl_tools.jar --obj-dump_filter [options] -artifact=\"app_v2.production.elf\"[,\"app_v1.production.elf\"]")
                    + "\r\n"
                    + Multifunction.FormatHelpText(79, 1 * 3, "Options and Inputs:")
                    + Multifunction.FormatHelpText(79, 2 * 3, "-decode.dinit")
                    + Multifunction.FormatHelpText(79, 3 * 3, "Optional parameter that turns on decoding of the .dinit RAM initializer table (normal data initialization for non-'preserved' variables)")
                    + "\r\n"
                    + Multifunction.FormatHelpText(79, 2 * 3, "-decode.rdinit")
                    + Multifunction.FormatHelpText(79, 3 * 3, "Optional parameter that turns on decoding of the .rdinit RAM initializer table (restart data initialization for 'update' variables)")
                    + "\r\n"
                    + Multifunction.FormatHelpText(79, 2 * 3, "-generate.fast_init")
                    + Multifunction.FormatHelpText(79, 3 * 3, "Optional parameter that turns on generation of fast, 'inline' initialization code to replace .dinit/.rdinit and _data_init() compiler methods.")
                    + "\r\n"
                    + Multifunction.FormatHelpText(79, 2 * 3, "-summarize.fast_init")
                    + Multifunction.FormatHelpText(79, 3 * 3, "Optional parameter displaying summary of fast 'inline' initialization code.")
                    + "\r\n"
                    + Multifunction.FormatHelpText(79, 2 * 3, "-elf_reader=\"compiler\\path\\to\\bin\\xc16-objdump.exe\"")
                    + Multifunction.FormatHelpText(79, 3 * 3, "Optional path to the GCC object dumping executable, i.e. xc16-objdump. If not specified, \"xc16-objdump\" will attempt to be used with OS specific path environment rules passed to the ezbl_tools.jar JVM.")
                    + "\r\n"
                    + Multifunction.FormatHelpText(79, 2 * 3, "-artifact=\"project_v2.production.elf\",[\"project_v1.production.elf\"]")
                    + Multifunction.FormatHelpText(79, 3 * 3, "One or more input .elf/.o file to read and process the contents of")
                    + "\r\n"
                    + Multifunction.FormatHelpText(79, 1 * 3, "Outputs:")
                    + Multifunction.FormatHelpText(79, 2 * 3, "Summary of suspicious data object file contents moving between artifact versions + any optional output data")
                    + "\r\n"
            );

            return;
        }


        // Decode -elf_reader="path" option. This isn't handled in the generic CommandAndBuiltState parsing
        if(state.undecodedOptions.contains("-elf_reader="))
        {
            for(int i = 0; i < args.length; i++)
            {
                if(args[i].startsWith("-elf_reader="))
                {
                    elfReaderPath = Multifunction.TrimQuotes(args[i].substring("-elf_reader=".length()));
                    elfReaderPath = FixSlashes(elfReaderPath);
                    break;
                }
            }
        }
        decodeDataInit = state.undecodedOptions.contains("-decode.dinit");
        decodeRestartDataInit = state.undecodedOptions.contains("-decode.rdinit");
        generateFastDataInitCode = state.undecodedOptions.contains("-generate.fast_init");
        generateFastDataInitCodeSummary = state.undecodedOptions.contains("-summarize.fast_init");

        if(state.artifactPath == null)
        {
            if(!state.silent)
            {
                System.err.println("Error: Missing required input artifact. The -artifact parameter is required.");
            }
            System.exit(-1);
        }
        artifacts = state.artifactPath.split("[,]");
        int filesPresent = 0;
        for(int i = 0; i < artifacts.length; i++)
        {
            if(new File(artifacts[i]).exists())
            {
                filesPresent++;
            }
        }
        if((filesPresent < 2) && !decodeDataInit && !decodeRestartDataInit && !generateFastDataInitCode && !generateFastDataInitCodeSummary)
        {
            if(!state.silent)
            {
                System.err.println("EZBL: At least two .elf files must be provided for RAM address checking; skipping...");
            }
            System.exit(-2);
        }

        parsers = new ELFDumpParser[artifacts.length];


        // Launch the compiler's .elf object dumping tool for each of the .elfs to compare
        for(int i = 0; i < parsers.length; i++)
        {
            parsers[i] = new ELFDumpParser();

            if(parsers[i].startObjDump(elfReaderPath, "--section-header", "--full-contents", artifacts[i]) < 0)
            {
                System.exit(-3);
            }
        }


        // Wait for completion then compare each result
        for(int i = 0; i < parsers.length; i++)
        {
            parsers[i].parseAllObjOutput(0);
            Collections.sort(parsers[i].symbols);
            Collections.sort(parsers[i].sections);

//            String htmlReport = parsers[i].generateHTMLReport();
//            Multifunction.UpdateFileIfDataDifferent(String.format("report%1$d.htm", i), htmlReport, true);
//            System.out.print(htmlReport);
//            int longestName = 0;
//            int biggestSize = 0;
//            for(Section sec : parsers[p].sections)
//            {
//                if(sec.isRAM)
//                {
//                    if(sec.name.length() > longestName)
//                    {
//                        longestName = sec.name.length();
//                    }
//                    if(sec.size > biggestSize)
//                    {
//                        biggestSize = (int)sec.size;
//                    }
//                }
//            }
//            String namePad = "-" + Integer.toString(longestName);
//            String sizePad = Integer.toString(Integer.toString(biggestSize).length());
//            for(Section sec : parsers[p].sections)
//            {
//                if(sec.isRAM)
//                {
//                    System.out.printf("\n%1$04X:%2$04X (%3$" + sizePad + "d) %4$" + namePad + "s %5$s", sec.loadMemoryAddress, sec.loadMemoryAddress + sec.size, sec.size, sec.name, sec.flags.wholeString);
//                }
//            }
//            for(Symbol s : parsers[p].symbols)
//            {
//                System.out.printf("\n%1$08X %2$-30s %3$s", s.address, s.section, s.name);
//            }
        }

        // Loop through the parsed contents, testing for mismatched data sections
        List<String> secAddrErrors = new ArrayList<String>();
        for(int p = 0; p < parsers.length; p++)
        {
            ELFDumpParser parser = parsers[p];

            boolean exactMatch = false;
            boolean nameMatch = false;
            boolean weakNameMatch = false;
            boolean addressMatch = false;
            boolean sizeMatch = false;
            boolean updateMatch = false;
            boolean preservedMatch = false;
            List<Section> partialMatches;


            // Only analyze the .dinit/.rdinit sections and generate an optimal
            // inline comparison for the first elf object.
            if((p == 0) && (decodeDataInit || decodeRestartDataInit || generateFastDataInitCode || generateFastDataInitCodeSummary))
            {
                for(Section sec : parser.sections)
                {
                    if(!sec.name.equals(".rdinit") && !sec.name.equals(".dinit"))
                    {
                        continue;
                    }

                    Blob ramInitBlob = new Blob(false);
                    List<String> initCode = new ArrayList<String>();
                    List<String> dinitDecode = new ArrayList<String>();
                    int nonzeroBytesEncoded = 0;
                    int zeroBytesEncoded = 0;
                    int functionCallsEncoded = 0;
                    int tableSize = sec.data.data.length;

                    for(int j = 0; j < sec.data.data.length - 9; j += 9)
                    {
                        int dest = (((int)sec.data.data[j]) & 0xFF) | ((((int)sec.data.data[j + 1]) & 0xFF) << 8) | ((((int)sec.data.data[j + 2]) & 0xFF) << 16);
                        int recordLen = (((int)sec.data.data[j + 3]) & 0xFF) | ((((int)sec.data.data[j + 4]) & 0xFF) << 8) | ((((int)sec.data.data[j + 5]) & 0xFF) << 16);
                        int format = (((int)sec.data.data[j + 6]) & 0x7F);
                        int page = ((((int)sec.data.data[j + 6]) & 0x80) >> 7) | ((((int)sec.data.data[j + 7]) & 0xFF) << 1);

                        String decodedFormat = (format == 0) ? "zero fill"
                                               : (format == 1) ? "copy pgm2"
                                                 : (format == 2) ? "copy pgm3"
                                                   : (format == 3) ? "call func"
                                                     : (format == 0x1F) ? "ldslvPRAM"
                                                       : "unknown  ";
                        byte[] recordData = new byte[recordLen];
                        long realDest = (long)dest; // Hardware dest address, in either data or program space, without EDS page obfuscation
                        if(format == 3) // Call function, a program space target, so ordinarily page should be 0x200+, but crt probably uses normal long call
                        {
                            realDest |= page << 16;
                        }
                        else if(((dest & 0x8000) != 0) && (page != 0))  // A data space target. Page should be 1+, right? Maybe not for non-EDS parts?
                        {
                            realDest ^= 0x8000 ^ (page << 15);
                        }
                        else    // Non-extended data space target or ldslvPRAM
                        {
                            realDest |= ((page - 1) << 16);    // Is this appropriate? Might be best to just leave unchanged.
                        }
                        String initSectionName = "";
                        for(Section secLookName : parser.sections)
                        {
                            if(secLookName.loadMemoryAddress == realDest)
                            {
                                initSectionName = secLookName.name;
                                break;
                            }
                        }

                        if(format == 0)         // Zero fill
                        {
                            zeroBytesEncoded += recordLen;
                            ramInitBlob.FillData(new AddressRange(realDest, recordLen), (byte)0x00);
                        }
                        if(format == 1)         // Copy 2 bytes per instruction word
                        {
                            nonzeroBytesEncoded += recordLen;
                            for(int k = 0; k < recordLen; k += 2)
                            {
                                recordData[k] = sec.data.data[j + 9];
                                j++;
                                if(sec.data.data.length >= j + 9)
                                {
                                    recordData[k + 1] = sec.data.data[j + 9];
                                }
                                j++;
                                if(sec.data.data.length >= j + 9)
                                {
                                    j++;
                                }
                            }
                            ramInitBlob.AddData(realDest, recordData);
                        }
                        else if(format == 2)    // Copy 3 bytes per instruction word
                        {
                            nonzeroBytesEncoded += recordLen;
                            for(int k = 0; k < recordLen; k++)
                            {
                                recordData[k] = sec.data.data[j + 9];
                                j++;
                            }
                            j += ((recordLen % 3) == 0) ? 0 : 3 - (recordLen % 3);
                            ramInitBlob.AddData(realDest, recordData);
                        }
                        else if(format == 3)    // Function call
                        {
                            functionCallsEncoded += 1;
                            initCode.add(String.format("call 0x%1$06X ; %2$s", dest, initSectionName));
                        }

                        dinitDecode.add(String.format("%1$s: dest = 0x%2$04X, len = %3$3d, format = %4$X (%5$s), page = 0x%6$03X: %7$s", sec.name, dest, recordLen, format, decodedFormat, page, initSectionName));
                        if((recordLen != 0) && (format != 0))
                        {
                            String displayBytes = Communicator.FormatBinaryHelp(recordData, sec.name.length() + 2, 16, 2, sec.name.length() + 2);
                            dinitDecode.add(String.format("{" + displayBytes + "\n}"));
                        }
                    }

                    if((sec.name.equals(".dinit") && decodeDataInit) || (sec.name.equals(".rdinit") && decodeRestartDataInit))
                    {
                        dinitDecode.add(0, String.format("Decoded contents of %1$s data initialiation table:"
                                +        "\n----------------------------------------------------", sec.name));
                        System.out.print(CatStringList(dinitDecode, "\n") + "\n\n");
                    }


                    System.out.printf("\nTotal %1$s section size: %2$3d bytes of flash (0x%3$06X addresses)", sec.name, tableSize, (tableSize + 2) / 3 * 2);
                    System.out.printf("\n    RAM bytes written: %1$d", nonzeroBytesEncoded + zeroBytesEncoded + 6 * functionCallsEncoded);
                    System.out.printf("\n    Function calls : %1$d", functionCallsEncoded);
                    System.out.printf("\n    Zero fill bytes: %1$d", zeroBytesEncoded);
                    System.out.printf("\n    Non-zero bytes : %1$d", nonzeroBytesEncoded);
                    System.out.printf("\n    Ratio RAM written/table size: %1$1.3f (bytes/byte)\n", ((double)(nonzeroBytesEncoded + zeroBytesEncoded + 6 * functionCallsEncoded)) / tableSize);


                    if(generateFastDataInitCode || generateFastDataInitCodeSummary)
                    {
                        ramInitBlob.architecture16Bit = false;
                        for(DataRecord dr : ramInitBlob.records)
                        {
                            dr.architecture16Bit = false;
                        }

                        ramInitBlob.CoalesceAdjacentDataRecords();
                        long curAddr = -1;
                        int executionTime = 0;
                        int regs[] = new int[15];
                        Arrays.fill(regs, 0xFFFFFFFF);


                        for(DataRecord dr : ramInitBlob.records)
                        {
                            int index = 0;
                            int doLen = dr.data.length;
                            int bytesAdded;
                            int tempIndex;
                            int aluGenCount;

                            if(curAddr != dr.address)
                            {
                                curAddr = dr.address;
                                if(curAddr >= 0x8000)
                                {
                                    initCode.add(String.format("movpag  #0x%1$03X, DSWPAG", curAddr >> 15));
                                    initCode.add(String.format("mov     #0x%1$04X, w0", (curAddr | 0x8000) & 0xFFFF));
                                    regs[0] = ((int)curAddr) & 0xFFFF;
                                    executionTime += 2;
                                }
                                else
                                {
                                    initCode.add(String.format("mov     #0x%1$04X, w0", curAddr));
                                    regs[0] = ((int)curAddr) & 0xFFFF;
                                    executionTime++;
                                }
                            }

                            if(((curAddr & 0x1) != 0) && (doLen != 0))
                            {
                                // Dest is odd, only load one byte
                                if(dr.data[index] == 0x00)
                                {
                                    initCode.add("clr.b   [w0++]");
                                    regs[0] = (regs[0] + 1) & 0xFFFF;
                                }
                                else
                                {
                                    initCode.add(String.format("mov     #0x00%1$02X, w2", dr.data[index]));
                                    regs[2] = ((int)dr.data[index]) & 0x00FF;
                                    initCode.add("mov.b   w2, [w0++]");
                                    regs[0] = (regs[0] + 1) & 0xFFFF;
                                    executionTime++;
                                }
                                executionTime++;
                                index++;
                                doLen--;
                                curAddr++;
                            }
                            while(doLen != 0)
                            {
                                // Count how many contiguous zeros are present here
                                aluGenCount = 0;
                                for(tempIndex = index; tempIndex < dr.data.length; tempIndex++)
                                {
                                    if(dr.data[tempIndex] != 0x00)
                                    {
                                        break;
                                    }
                                    aluGenCount++;
                                }
                                if((aluGenCount & 0x1) != 0)  // Force even
                                {
                                    aluGenCount--;
                                }
                                if(aluGenCount >= 6)  // If there are enough to make it worthwhile encode these as a repeat clr.w loop
                                {
                                    initCode.add(String.format("repeat  #%1$d", aluGenCount / 2 - 1));
                                    initCode.add("clr     [w0++]");
                                    regs[0] = (regs[0] + 2 * aluGenCount / 2) & 0xFFFF;
                                    executionTime += 1 + aluGenCount / 2;
                                    index += aluGenCount;
                                    doLen -= aluGenCount;
                                    curAddr += aluGenCount;
                                    continue;
                                }
                                if(aluGenCount == 4)
                                {
                                    initCode.add("clr     [w0++]");
                                    initCode.add("clr     [w0++]");
                                    regs[0] = (regs[0] + 4) & 0xFFFF;
                                    executionTime += 2;
                                    index += 4;
                                    doLen -= 4;
                                    curAddr += 4;
                                    continue;
                                }
                                if(aluGenCount == 2)
                                {
                                    initCode.add("clr     [w0++]");
                                    regs[0] = (regs[0] + 2) & 0xFFFF;
                                    executionTime++;
                                    index += aluGenCount;
                                    doLen -= aluGenCount;
                                    curAddr += aluGenCount;
                                    continue;
                                }


                                // Count how many contiguous 0xFFFF's are present here
                                aluGenCount = 0;
                                for(tempIndex = index; tempIndex < dr.data.length; tempIndex++)
                                {
                                    if(dr.data[tempIndex] != 0xFF)
                                    {
                                        break;
                                    }
                                    aluGenCount++;
                                }
                                if((aluGenCount & 0x1) != 0)  // Force even
                                {
                                    aluGenCount--;
                                }
                                if(aluGenCount >= 6)  // If there are enough to make it worthwhile encode these as a repeat clr.w loop
                                {
                                    initCode.add(String.format("repeat  #%1$d", aluGenCount / 2 - 1));
                                    initCode.add("setm    [w0++]");
                                    regs[0] = (regs[0] + 2 * aluGenCount / 2) & 0xFFFF;
                                    executionTime += 1 + aluGenCount / 2;
                                    index += aluGenCount;
                                    doLen -= aluGenCount;
                                    curAddr += aluGenCount;
                                    continue;
                                }
                                if(aluGenCount == 4)
                                {
                                    initCode.add("setm    [w0++]");
                                    initCode.add("setm    [w0++]");
                                    regs[0] = (regs[0] + aluGenCount) & 0xFFFF;
                                    executionTime += 2;
                                    index += aluGenCount;
                                    doLen -= aluGenCount;
                                    curAddr += aluGenCount;
                                    continue;
                                }
                                if(aluGenCount == 2)
                                {
                                    initCode.add("setm    [w0++]");
                                    regs[0] = (regs[0] + aluGenCount) & 0xFFFF;
                                    executionTime++;
                                    index += aluGenCount;
                                    doLen -= aluGenCount;
                                    curAddr += aluGenCount;
                                    continue;
                                }

                                bytesAdded = 0;
                                for(int j = 2; j <= 14; j++)
                                {
                                    if(doLen < 2)
                                    {
                                        break;
                                    }
                                    int wordData = ((((int)dr.data[index + 1]) << 8) & 0xFF00) | (((int)dr.data[index + 0]) & 0x00FF);
                                    if((wordData == 0) && (doLen >= 4) && ((j % 2) == 0) && (j <= 12))
                                    {
                                        int nextWordData = ((((int)dr.data[index + 3]) << 8) & 0xFF00) | (((int)dr.data[index + 2]) & 0x00FF);
                                        if(nextWordData == 0)
                                        {
                                            initCode.add(String.format("mul.uu  w0, #0, w%1$d", j));
                                            bytesAdded += 4;
                                            executionTime++;
                                            index += 4;
                                            doLen -= 4;
                                            curAddr += 4;
                                            j++;
                                            continue;
                                        }
                                    }
                                    initCode.add(String.format("mov     #0x%1$04X, w%2$d", wordData, j));
                                    executionTime++;
                                    bytesAdded += 2;
                                    index += 2;
                                    doLen -= 2;
                                    curAddr += 2;
                                    continue;
                                }
                                if(bytesAdded >= 6)
                                {
                                    initCode.add("mov     #w2, w1");
                                    initCode.add(String.format("repeat  #%1$d", bytesAdded / 2 - 1));
                                    initCode.add("mov     [w1++], [w0++]");
                                    executionTime += 2 + (bytesAdded / 2);
                                    bytesAdded -= (bytesAdded / 2) * 2;
                                }
                                for(int j = 2; bytesAdded >= 2; bytesAdded -= 2)
                                {
                                    initCode.add(String.format("mov     w%1$d, [w0++]", j++));
                                    executionTime++;
                                }
                                if(doLen == 1)
                                {
                                    if(dr.data[index] == 0x00)
                                    {
                                        initCode.add("clr.b   [w0++]");
                                        executionTime++;
                                    }
                                    if(dr.data[index] == 0xFF)
                                    {
                                        initCode.add("setm.b  [w0++]");
                                        executionTime++;
                                    }
                                    else
                                    {
                                        initCode.add(String.format("mov     #0x00%1$02X, w2", dr.data[index++]));
                                        initCode.add("mov.b   w2, [w0++]");
                                        executionTime += 2;
                                    }
                                    doLen--;
                                    curAddr++;
                                }
                            }
                        }

                        int initCodeBytes = 3 * initCode.size() + 3 * functionCallsEncoded;
                        executionTime += 10 * functionCallsEncoded;

                        if(generateFastDataInitCode)
                        {
                            System.out.print("\n\nFast initializer equivalent code:"
                                             + "\n    " + CatStringList(initCode, "\n    ") + "\n\n");
                        }

                        System.out.printf("\nTotal generated code size: %1$3d bytes of flash (0x%2$06X addresses)", initCodeBytes, initCodeBytes / 3 * 2);
                        System.out.printf("\n    RAM bytes initialized: %1$d bytes (0x%1$04X)", nonzeroBytesEncoded + zeroBytesEncoded + 6 * functionCallsEncoded);
                        System.out.printf("\n    Ratio RAM written/code size: %1$1.3f (bytes/byte)", ((double)(nonzeroBytesEncoded + zeroBytesEncoded + 6 * functionCallsEncoded)) / initCodeBytes);
                        System.out.printf("\n    Relative ROM improvement over %1$s table: %2$d ROM table + 270 shared ROM code bytes for data_init() (%3$1.1f%%)", sec.name, tableSize - initCodeBytes, (double)initCodeBytes / (tableSize + 270) * 100);
                        System.out.printf("\n    Approximate execution time, excluding function code: %1$d instruction cycles (%2$1.3fus @ 70 MIPS)\n", executionTime, executionTime / 70.0);
                        System.out.printf("\n\n\n\n");
                    }
                }
            }


            for(Section sec : parser.sections)
            {
                if(!sec.isRAM)
                {
                    continue;
                }
                if(sec.flags.UPDATE && !sec.flags.PRESERVED)
                {
                    continue;
                }

                for(int j = p + 1; j < parsers.length; j++)
                {
                    ELFDumpParser parser2 = parsers[j];

                    exactMatch = false;
                    nameMatch = false;
                    addressMatch = false;
                    sizeMatch = false;
                    updateMatch = false;
                    partialMatches = new ArrayList<Section>();
                    for(Section sec2 : parser2.sections)
                    {
                        if(!sec.isRAM)
                        {
                            continue;
                        }

                        if(sec.name.equals(sec2.name) && (sec.loadMemoryAddress == sec2.loadMemoryAddress) && (sec.size == sec2.size))
                        {   // An exact match found
                            exactMatch = true;
                            preservedMatch |= sec2.flags.PRESERVED;
                            updateMatch |= sec2.flags.UPDATE;
                            partialMatches.add(sec2);
                        }
                        else if(sec.name.equals(sec2.name) && (sec.loadMemoryAddress == sec2.loadMemoryAddress))
                        {   // A close match that may have been resized or possibly a very bad match for a generic ex: ".data" section was made.
                            nameMatch = true;
                            addressMatch = true;
                            preservedMatch |= sec2.flags.PRESERVED;
                            updateMatch |= sec2.flags.UPDATE;
                            partialMatches.add(sec2);
                        }
                        else if((sec.size == sec2.size) && ((sec.loadMemoryAddress == sec2.loadMemoryAddress) || sec.name.equals(sec2.name)))
                        {   // Potentially a real match that has been renamed, but more likely than not is a problem
                            sizeMatch = true;
                            preservedMatch |= sec2.flags.PRESERVED;
                            updateMatch |= sec2.flags.UPDATE;
                            partialMatches.add(sec2);
                        }
                        else if(sec.name.equals(sec2.name))
                        {   // Only a name match... this doesn't sound good at all
                            nameMatch = true;
                            preservedMatch |= sec2.flags.PRESERVED;
                            updateMatch |= sec2.flags.UPDATE;
                            partialMatches.add(sec2);
                        }
                        else if(sec.name.matches(".*?\\.[0-9]*?$"))
                        {
                            String left = sec.name.substring(0, sec.name.lastIndexOf('.'));
                            String right = sec2.name;
                            if(right.matches(".*?\\.[0-9]*?$"))
                            {
                                right = right.substring(0, right.lastIndexOf('.'));
                            }
                            if(left.equals(right))
                            {
                                weakNameMatch = true;
                                partialMatches.add(sec2);
                            }
                        }
                    }
                    if((partialMatches.size() == 1) && exactMatch && !nameMatch && !addressMatch && !sizeMatch)    // The best unambiguous case
                    {
                        continue;
                    }
                    if(exactMatch)  // Still good, at least for this section
                    {
                        continue;
                    }
                    if((partialMatches.size() == 1) && nameMatch && updateMatch)    // Probably also fine
                    {
                        continue;
                    }

                    // Output possible problems
                    String namePad = "-";
                    int longestName = sec.name.length();
                    for(Section partial : partialMatches)
                    {
                        if(partial.name.length() > longestName)
                        {
                            longestName = partial.name.length();
                        }
                    }
                    namePad += Integer.toString(longestName);

                    secAddrErrors.add(String.format("Found: 0x%1$04X (%2$3d) %3$" + namePad + "s, %4$s\n", sec.loadMemoryAddress, sec.size, sec.name, sec.flags.wholeString));
                    if(partialMatches.isEmpty())
                    {
                        secAddrErrors.add(String.format("       No likely related sections found\n"));
                    }

                    for(Section partial : partialMatches)
                    {
                        secAddrErrors.add(String.format(" Prev: 0x%1$04X (%2$3d) %3$" + namePad + "s, %4$s\n", partial.loadMemoryAddress, partial.size, partial.name, partial.flags.wholeString));
                    }
                }
            }

            // Only output results for the first .elf file compared against all prior .elf files, not prior elf files against other prior elf files
            if(!secAddrErrors.isEmpty() && (p == 0))
            {
                System.err.println();
                for(String s : secAddrErrors)
                {
                    System.err.print(s);
                }
                System.err.println();
            }
        }
    }


}

