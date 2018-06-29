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

import static com.microchip.apps.ezbl.DataRecord.CoalesceRecords;
import static com.microchip.apps.ezbl.Multifunction.CatStringList;
import java.io.*;
import java.util.*;
import java.util.regex.*;


/**
 *
 * @author C12128
 */
public class DumpParser
{

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args)
    {
        String fileData;
        String debuggableArtifactPath;
        List<DataRecord> romRestoreElements = new ArrayList<DataRecord>();
        List<AddressRange> romNoLoadRanges = new ArrayList<AddressRange>();
        List<AddressRange> appSpaceRegions;
        //List<Section> romSections = new ArrayList<Section>();
        //List<Section> ramSections = new ArrayList<Section>();
        //List<Section> otherSections = new ArrayList<Section>();
        AddressRange journalRange = new AddressRange(-1, -1);

        String firstPassResultsSectionRegexStart = "AUTOMATIC FIRST PASS RESULTS SECTION[^\n]*?\n";
        String firstPassResultsSectionRegexEnd = "(?<=\n)[ \t]*?/[*][^E\n]*END OF FIRST PASS RESULTS SECTION";
        String forwardBootloaderFlagsVariableName = "EZBL_ForwardBootloaderISR";

        List<String> gldOutput = new ArrayList<String>();
        String outputValuesString;
        String elfDumpText;
        int genROMDataAddrSize = 0;
        int genISRDispatchStubs = 0;


        // Obtain the given command line options and all prior ezbl_tools invocation state information
        String stateSavePath = CommandAndBuildState.GetStateFilePath(args);
        EZBLState state = EZBLState.ReadFromFile(stateSavePath);                // Load up the preexisting state file from disk, if one exists
        state = CommandAndBuildState.ReadArgs(state, args);                     // Parse any new command line options and save them

        // If nothing is specified upon execution, write usage information to STDOUT
        if(!state.parseOkay)
        {
            System.out.print(
                    "\r\n"
                    + Multifunction.FormatHelpText(79, 0 * 3, "Usage:")
                    + Multifunction.FormatHelpText(79, 1 * 3, "java -jar ezbl_tools.jar --dump_parser \"symbols_and_section_dump.txt\" \"linker_script_to_edit.gld\"")
                    + "\r\n"
                    + Multifunction.FormatHelpText(79, 1 * 3, "Inputs:")
                    + Multifunction.FormatHelpText(79, 2 * 3, "xc16-objdump output containing headers and symbols. Ex: \"symbols_and_section_dump.txt\", where this text file is generated using:")
                    + Multifunction.FormatHelpText(79, 3 * 3, "xc16-objdump -x -t -r MyApp.elf > symbols_and_section_dump.txt")
                    + "\r\n"
                    + Multifunction.FormatHelpText(79, 2 * 3, "Filename to write extracted EZBL parameters into.")
                    + "\r\n"
                    + Multifunction.FormatHelpText(79, 1 * 3, "Outputs:")
                    + Multifunction.FormatHelpText(79, 2 * 3, ""
                                                              + "Parses a text file elf dump created from xc16-objdump (part of the XC compiler "
                                                              + "toolchain) and extracts needed information for linking with EZBL. "
                                                              + "linker_script_to_edit.gld is meant to be the .gld linker script you are using in your project. When the project is (re)linked with this updated file, the bootloader will be properly placed in memory.")
                    + "\r\n"
            );

            return;
        }

        if((state.artifactPath == null) || (state.linkScriptPath == null))
        {
            if(!state.silent)
            {
                System.err.println("ezbl_tools: Missing required input parameter.");
            }
            System.exit(-1);
            return;
        }

        state.elfDump = new ELFDumpParser();

        debuggableArtifactPath = state.elfPath != null ? state.elfPath : state.artifactPath;
        if(!debuggableArtifactPath.toLowerCase().endsWith(".elf"))
        {
            debuggableArtifactPath = debuggableArtifactPath.replaceFirst("\\.[hH][eE][xX]$", ".elf");
        }

        // Start getting the Section header table and Symbol Tables
        List<String> cmdLine = new ArrayList<String>();
        cmdLine.add(state.compilerFolder + File.separator + "xc16-objdump");
        cmdLine.add("--section-headers");
        //cmdLine.add("--debugging");   // DO NOT ENABLE: Does not seem compatible with XC16 on Mac - data not used anyway
        cmdLine.add("--syms");
        //cmdLine.add("--reloc");       // DO NOT ENABLE: Does not seem compatible with XC16 on Mac - data not used anyway
        //cmdLine.add("--dynamic-reloc"); // data not used
        cmdLine.add(debuggableArtifactPath);
        if(state.verbose)
        {
            System.out.println("ezbl_tools: " + CatStringList(cmdLine, " "));
        }
        if(state.elfDump.startObjDump(cmdLine) < 0)
        {
            System.exit(-2);
        }
        // Start getting the Section contents list
        cmdLine.clear();
        cmdLine.add(state.compilerFolder + File.separator + "xc16-objdump");
        cmdLine.add("--full-contents");
        cmdLine.add(debuggableArtifactPath);
        if(state.verbose)
        {
            System.out.println("ezbl_tools: " + CatStringList(cmdLine, " "));
        }
        if(state.elfDump.startObjDump(cmdLine) < 0)
        {
            System.exit(-3);
        }


        // Clear last lists of used ranges and allocate new lists
        state.romUseRanges = new ArrayList<AddressRange>();
        state.ramUseRanges = new ArrayList<AddressRange>();
        state.otherSections = new ArrayList<Section>();
        state.ramSections = new ArrayList<Section>();
        state.romSections = new ArrayList<Section>();
        state.noProgramRanges = new ArrayList<AddressRange>();
        state.noEraseRanges = new ArrayList<AddressRange>();
        state.noVerifyRanges = new ArrayList<AddressRange>();

        // Things could have moved, so erase any previous end address
        // information derived from a prior DumpParser pass
        state.endAddressOfBootloader = 0;

        // Wait for Section header table and Symbol Tables to be all read and processed
        elfDumpText = state.elfDump.parseAllObjOutput(0);
        // Wait for Section contents to be decoded
        elfDumpText += state.elfDump.parseAllObjOutput(0);

        if(state.saveTemps)
        {
            Multifunction.WriteFile(state.temporariesPath + "ezbl_objdump.txt", elfDumpText.getBytes(), false);
        }
        else
        {
            Multifunction.DeleteFile(state.temporariesPath + "ezbl_objdump.txt");
        }

        // Find all the important data ("ram") and nonvolatile/Flash program
        // ("rom") sections for reporting in the RAM and ROM use tables. Read-
        // only bootloader sections are expanded to cover a full erase block so
        // we know what should be marked as reserved in the ROM use table.
        for(int secIndex = 0; secIndex < state.elfDump.sections.size(); secIndex++)
        {
            Section sec = state.elfDump.sections.get(secIndex);
            //Section sec = secOriginal.Clone();  // Clone section so we can modify addresses and add padding

            // Ignore debugging sections and noload attributed sections
            // ("NEVER_LOAD") in Flash or RAM - they generate no code/data
            // and we shouldn't therefore treat them as reserved in the
            // bootloader project. An exception is made for .heap section RAM
            // memory. We don't currently have a mechanism for sharing
            // bootloader .heap with application .heap, so to avoid having the
            // Application clobber running Bootloader .heap space, we must
            // reserve it as an ordinary Bootloader RAM section.
            //if(sec.isDebug || (sec.flags.NEVER_LOAD && !sec.name.startsWith(".heap")))
            if(sec.isDebug)
            {
                state.otherSections.add(sec);
                continue;
            }

            // Harvest all nonvolatile Flash sections (.text, .prog, .const, .psv, .ivt, Flash config words, etc.)
            if(sec.isROM)
            {
                // Throw away .text.EZBL_AppReservedHole.* ROM sections. They 
                // are only needed for linking of the Bootloader and shouldn't 
                // participate in other EZBL processing, should not be included 
                // in the __EZBL_TOOLS_COMPUTED_NO_PROGRAM_RANGES or 
                // __EZBL_TOOLS_COMPUTED_NO_ERASE_RANGES, which indicate 
                // Bootloader reserved regions, and shouldn't get padded with 
                // '1's to flash erase page boundaries.
                if(sec.name.startsWith(".text.EZBL_AppReserved"))
                {
                    state.elfDump.sections.remove(sec);
                    secIndex--;
                    continue;
                }
                
                // Otherwise add this ROM section contents to a list so we can regenerate the data as .s assembly output
                state.romSections.add(sec);
                
                // Don't process sections that are erasable (i.e. belong to the bootloaded application) in buildAsOneMode
                if(state.buildAsOneMode)
                {
                    // Extract address range and check special named EZBL_JOURNAL section
                    if(sec.name.startsWith("EZBL_JOURNAL"))
                    {
                        journalRange.startAddress = sec.loadMemoryAddress;
                        journalRange.endAddress = sec.loadMemoryAddress + sec.size;
                        if(journalRange.startAddress % state.eraseBlockSizeAddresses != 0)    // Start address should be already sector aligned, but if for some reason it isn't, we don't want to start erasing and writing to something else, so increment to the next sector boundary instead.
                        {
                            journalRange.startAddress += state.eraseBlockSizeAddresses - (journalRange.endAddress % state.eraseBlockSizeAddresses);
                        }
                        journalRange.endAddress &= ~(state.eraseBlockSizeAddresses - 1);   // This end address should be already sector aligned, but if for some reason it isn't, we don't want to start erasing and writing to something else, so truncate it instead.

                        // Sanity check that the journal is at least 2 sectors big
                        if(journalRange.startAddress + 2 * state.eraseBlockSizeAddresses > journalRange.endAddress)
                        {
                            System.err.println("ezbl_tools: Section \"" + sec.name + "\" is <2 Flash Erase Page sizes.");
                        }
                    }

                    if(!sec.name.startsWith("EZBL"))
                    {
                        continue;
                    }
                }

                // Compute start and end addresses for this section that fills
                // a complete sector if they don't already, aren't in test
                // memory (ex: real fuses outside Flash), or are a special
                // region we want to handle seperately
                AddressRange ar = new AddressRange(sec.loadMemoryAddress, sec.loadMemoryAddress + sec.size);    // Addresses, not bytes

                // Only modify sections that are in Flash (i.e. not fuse based config words)
                // Do not modify Flash config word sections if we are allowed to erase the last page
                if(((ar.startAddress & 0x800000) == 0) // !Test memory
                   && ((!state.allowFirstPageErase) || (state.allowFirstPageErase && (ar.startAddress >= state.eraseBlockSizeAddresses) && (state.baseAddress >= state.eraseBlockSizeAddresses))) // !Reset vector/IVT
                   && ((!state.allowLastPageErase) || (state.configWordsRange == null) || (state.allowLastPageErase && (ar.endAddress <= state.configWordsRange.startAddress)))) // !Flash Config words
                {
                    ar.startAddress &= ~(state.eraseBlockSizeAddresses - 1);    // Left pad to page erase boundary
                    if(ar.endAddress % state.eraseBlockSizeAddresses != 0)      // Right pad to page erase boundary
                    {
                        ar.endAddress += state.eraseBlockSizeAddresses - (ar.endAddress % state.eraseBlockSizeAddresses);
                    }
                }
//                if((ar.startAddress != sec.loadMemoryAddress) || (ar.endAddress != (sec.loadMemoryAddress + sec.size)))
//                {
//                    if(sec.data != null)
//                    {   // Add 0xFF padding bytes
//                        int newDataLen = sec.data.architecture16Bit ? (int)((ar.endAddress - ar.startAddress) * 3 / 2) : (int)(ar.endAddress - ar.startAddress);
//                        int newDataStartIndex = sec.data.architecture16Bit ? (int)((sec.data.address - ar.startAddress) * 3 / 2) : (int)(sec.data.address - ar.startAddress);
//                        byte[] newData = new byte[newDataLen];
//                        Arrays.fill(newData, (byte)0xFF);
//                        System.arraycopy(sec.data.data, 0, newData, newDataStartIndex, sec.data.data.length);
//                        sec.data.address = ar.startAddress;
//                        sec.data.data = newData;
//                    }
//                    sec.virtualMemoryAddress -= ar.startAddress - sec.loadMemoryAddress;
//                    sec.loadMemoryAddress = ar.startAddress;
//                    sec.size = ar.endAddress - ar.startAddress;
//                }

                // Add the effective ROM use requirements range to a collection
                // of addresses that need to be non-erasable (i.e. belong to the
                // bootloader). This will be used to generate the
                // __EZBL_ROM_USE_TABLE/__EZBL_TOOLS_COMPUTED_NO_ERASE_RANGES.
                FixupAlignAndPadAR(ar, state.flashWordSize, new AddressRange(0x800000, 0xFFFFFF));
                if(sec.flags.NEVER_LOAD)
                {
                    romNoLoadRanges.add(ar);
                }
                else
                {
                    state.romUseRanges.add(ar);                 // TODO: Need to subtract off the dummy .const reserved for app space
                    state.noProgramRanges.add(ar.clone());      // TODO: Need to subtract off the dummy .const reserved for app space
                    if(!(state.allowLastPageErase && (ar.startAddress >= state.configWordsRange.startAddress) && (ar.startAddress < state.configWordsRange.endAddress)))    // May need to add protection/address range splitting for non-compiler sections, like a single large section that spans the whole page + others before it and contains config words
                    {
                        state.noEraseRanges.add(ar.clone());    // TODO: Need to subtract off the dummy .const reserved for app space
                    }
                }


                // Check if this ROM region is in an execution space (i.e. not
                // config words). If so, we keep track of the highest address
                // used for possible placement of the EZBL_AppErasable sector
                if((state.endAddressOfBootloader < ar.endAddress) && (ar.startAddress >= state.mainExecutionRange.startAddress) && (ar.endAddress < state.mainExecutionRange.endAddress))
                {
                    state.endAddressOfBootloader = ar.endAddress;
                }
                continue;
            }

            // Harvest all RAM sections required for the bootloader(.bss, .nbss, .data, .ndata, .pbss, .xbss, .ybss, .npbss?, etc.)
            if(sec.isRAM)
            {
                state.ramSections.add(sec);

                // Capture the debug/production build state for setting BKBUG
                if(sec.name.equals(".icd") || sec.name.startsWith("reserve_data_0x"))
                {
                    state.linkedAsDebugImage = true;
                }
                else if(sec.name.equals("EZBL_ICD_RAM_RESERVE"))
                {
                    state.linkedAsDebugImage = false;
                }

                // Ignore all non-bootloader RAM ranges in old builtAsOneMode
                if((state.buildAsOneMode && !sec.name.startsWith("EZBL")) || sec.name.startsWith("EZBL_ICD_RAM_RESERVE"))
                {
                    continue;
                }

                state.ramUseRanges.add(new AddressRange(sec.loadMemoryAddress, sec.loadMemoryAddress + sec.size));
                continue;
            }

            // If we get down here, it means this section isn't for debugging, known allocation in RAM, or known allocation in ROM. This ideally shouldn't happen. If it does, we should debug this...
            if(state.verbose)
            {
                System.err.println("ezbl_tools: Don't know what memory region the \"" + sec.name + "\" section belongs in, so bootloader handling may be incorrect. This EZBL version may not be compatible with this XC16 compiler version.");
            }
            state.otherSections.add(sec);
        }

        // Sort the sections by loadMemoryAddress
        Collections.sort(state.romSections);
        Collections.sort(state.ramSections);

        // Compute the base addresses to place app erasable data, including the app's goto __reset and Interrupt Goto Table
        state.sizeOfIGT = (state.ivtVectorsImplemented + 1) * 4;
        if(!state.remapISRThroughIGT)
        {
            state.sizeOfIGT = 0;
        }
        state.sizeOfAppErasable = EZBLState.sizeOfAppBootloadState + EZBLState.sizeOfAppGotoReset + state.sizeOfIGT;
        long newAddressOfAppErasable = state.appVectorsAfterBootloader ? state.endAddressOfBootloader : state.baseAddress - state.sizeOfAppErasable;  // For app data just after or just before the bootloader
        if(state.pass > 1)
        {
            if(state.baseAddressOfAppErasable != newAddressOfAppErasable)
            {
                String errorMsg = Multifunction.FormatHelpText("ezbl_tools: bootloader changed geometry between pass 1 and pass 2 linking. This condition requires an additional linking step for ezbl_tools.jar to compute final addresses. Please rebuild your project (this error will automatically go away once the bootloader geometry stays fixed).", 120, 0, 6, true);
                System.err.println(errorMsg);
                System.exit(-300);
            }
        }
        state.baseAddressOfAppErasable = newAddressOfAppErasable;
        state.baseAddressOfAppBootloadState = state.baseAddressOfAppErasable;
        state.baseAddressOfGotoReset = state.baseAddressOfAppBootloadState + EZBLState.sizeOfAppBootloadState;
        state.baseAddressOfIGT = state.baseAddressOfGotoReset + EZBLState.sizeOfAppGotoReset;

        // Add the appBootloadState as a noVerifyRange since we will skip it
        // during bootloading and only write it when everything is verified. We
        // can't abort with a verify error when the image file presents this
        // record.
        AddressRange appBootloadStateRange = new AddressRange(state.baseAddressOfAppBootloadState, state.baseAddressOfAppBootloadState + EZBLState.sizeOfAppBootloadState);
        state.noVerifyRanges.add(appBootloadStateRange);

        // Sort and coalesce the used address ranges
        AddressRange.SortAndReduce(state.ramUseRanges);
        AddressRange.SortAndReduce(state.romUseRanges);


        // Find/compute final IVT entry values
        String ivtDispacher = "";
        String ivtMasks = "";
        if(state.remapISRThroughIGT)
        {
            state.ivtVectorEntries = new int[state.ivtVectors.length];
            state.ivtVectorPointsToBootloader = new boolean[state.ivtVectors.length];
            int igtIndex = 0;

            for(int i = 0; i < state.ivtVectors.length; i++)
            {
                Symbol sym = Symbol.GetSymbolByName(state.elfDump.symbols, state.ivtVectors[i]);

                if((sym == null) || (genISRDispatchStubs > 32))
                {
                    if((sym != null) && (genISRDispatchStubs > 32))
                    {
                        System.err.print(Multifunction.FormatHelpText(110, 0, String.format("ezbl_tools: warning: There are more than 32 interrupts defined in your bootloader. EZBL only supports up to 32 run-time choosable ISRs in your Bootloader. Building will proceed by not hooking the Bootloader's %1$s() function up in the IVT and will instead always forward this Interrupt to the Application IGT.", state.ivtVectors[i].substring(1))));
                    }
                    // ISR not implemented in this build, point to IGT
                    // Compute proper IGT Address
                    int igtAddress = (int)state.baseAddressOfIGT + 4 * igtIndex;
                    if(!state.ivtVectorImplemented[i])
                    {
                        // When unimplemented, send to extra __DefaultInterrupt IGT entry
                        igtAddress = (int)state.baseAddressOfIGT + 4 * state.ivtVectorsImplemented;
                    }
                    state.ivtVectorEntries[i] = igtAddress;
                    state.ivtVectorPointsToBootloader[i] = false;
                }
                else
                {
                    // Function defined in this build, map to a muxing stub so we
                    // can control if the bootloader or application receives the
                    // interrupt
                    state.ivtVectorEntries[i] = (int)sym.address;
                    state.ivtVectorPointsToBootloader[i] = true;

                    // Create code to forward implemented interrupts to the
                    // application when bootloader interrupt forwarding is turned off
                    String shortIntName = state.ivtVectors[i].replace("Interrupt", "").replaceFirst("__", "");
                    ivtMasks += String.format("\n%1$-32s = 0x%2$08X;"
                                              + "\n%3$-32s = %4$d;",
                                              "_EZBL_FORWARD_MASK_" + shortIntName, (1 << genISRDispatchStubs),
                                              "_EZBL_FORWARD_POS_" + shortIntName, genISRDispatchStubs);

                    ivtDispacher += String.format("%1$-104s", "\n    EZBL_Dispatch" + state.ivtVectors[i] + " = ABSOLUTE(.);") + String.format("%1$-49s", " /* EZBL_Dispatch" + state.ivtVectors[i] + ":") + " */ "
                                    + String.format("%1$-104s", String.format("\n    BTSC(ABSOLUTE(_" + forwardBootloaderFlagsVariableName + ") + 0x%1$X, %2$d);", genISRDispatchStubs / 16 * 2, genISRDispatchStubs % 16)) + String.format("%1$-49s", String.format(" /*    btsc    _" + forwardBootloaderFlagsVariableName + ", #%1$d", genISRDispatchStubs)) + " */ "
                                    + String.format("%1$-104s", String.format("\n    GOTO(ABSOLUTE(_EZBL_IGT_BASE) + 0x%1$04X);", 4 * igtIndex, state.baseAddressOfIGT + 4 * igtIndex)) + String.format("%1$-49s", String.format(" /*    goto    0x%1$06X", state.baseAddressOfIGT + 4 * igtIndex)) + " */ "
                                    + String.format("%1$-104s", String.format("\n    GOTO(DEFINED(" + state.ivtVectors[i] + ") ? ABSOLUTE(" + state.ivtVectors[i] + ") : ABSOLUTE(_EZBL_IGT_BASE) + 0x%1$04X);", 4 * igtIndex)) + String.format("%1$-49s", String.format(" /*    goto    %1$s", state.ivtVectors[i])) + " */ ";
                    state.ivtVectorImplemented[i] = true;
                    genISRDispatchStubs++;
                }

                // Advance counter so we know what the next IGT entry address
                // should be whenever we process an implemented vector
                if(state.ivtVectorImplemented[i])
                {
                    igtIndex++;
                }
            }

            ivtDispacher = ivtDispacher.replaceAll("\n", Matcher.quoteReplacement("\\\n"));     // Add line continuation character to all preprocessor new-lines
            genROMDataAddrSize += 0xA * genISRDispatchStubs;
            if(state.mainFlashRange.endAddress <= 0x10000)  // For < 96KB devices, the BRA opcode can branch anywhere, so we don't need longer GOTO instructions
            {
                ivtDispacher = ivtDispacher.replaceAll("GOTO\\(", Matcher.quoteReplacement("BRA( "));
                ivtDispacher = ivtDispacher.replaceAll("goto", Matcher.quoteReplacement("bra "));
                genROMDataAddrSize -= 0x4 * genISRDispatchStubs;
            }
        }


        // Build romRestoreElements data restore records for erasable
        // bootloader elements.
        // Find items that aren't on an whole erase page by themself (ex: Config
        // Word Values, Reset vector, used interrupt vector list if allowing
        // Page 0 or Last Config Words page to be erased)
        {
            // Going to need Section contents now, so read and parse the result
            // of the xc16-objdump process started earlier.
            //elfDumpText += state.elfDump.parseAllObjOutput(0);

            // Enumerate everything in ROM, including test space i.e. address >= 0x800000
            for(Section sec : state.elfDump.sections)
            {
                if(!sec.isROM)
                {
                    continue;
                }

                // Check if section overlaps the first erase page and we allow erasing the first page
                if(state.allowFirstPageErase)
                {
                    if(sec.loadMemoryAddress < 0x000000 + state.eraseBlockSizeAddresses)
                    {
                        // Save everything up to the end of the first page.
                        if(sec.data.address >= state.eraseBlockSizeAddresses)
                        {
                            continue;
                        }

                        // Clone this data record, except the comment and assignedMemory String references since we are going to possibly make changes to the address and data
                        DataRecord restoreRecord = sec.data.Clone();
                        restoreRecord.comment = "";
                        restoreRecord.assignedMemory = "";

                        if(restoreRecord.GetEndAddress() > 0x000000 + state.eraseBlockSizeAddresses)  // Filter out anything outside of the first erase page
                        {
                            restoreRecord = restoreRecord.SplitAtAddress(state.eraseBlockSizeAddresses);
                        }
                        romRestoreElements.add(restoreRecord);
                    }
                }

                if(state.hasFlashConfigWords && state.allowLastPageErase && (state.configWordsRange != null))
                {
                    // Check if this is a Flash Config value or other Bootloader data on last page, if so, save it to romRestoreElements
                    if((sec.loadMemoryAddress + sec.size > state.configWordsRange.startAddress) && (sec.loadMemoryAddress < state.configWordsRange.endAddress))
                    {
                        if(sec.data == null)
                        {
                            System.err.print("ezbl_tools: failed to get section data from object dump. Rebuilding might help.\n");
                            System.exit(-4);
                        }
                        DataRecord restoreRecord = sec.data.Clone();
                        if(sec.data.address < state.configWordsRange.startAddress)
                        {
                            restoreRecord = restoreRecord.SplitAtAddress(state.configWordsRange.startAddress);
                        }
                        romRestoreElements.add(restoreRecord);
                        continue;
                    }
                }

                // Add "real" config fuse values, if they exist, but not if the device also has Flash Config Words. FBOOT is excluded from the address check here since all devices with FBOOT have Flash Config Words.
                if(!state.hasFlashConfigWords && (sec.loadMemoryAddress >= 0x800000))
                {
                    if(sec.data == null)
                    {
                        System.err.print("ezbl_tools: failed to get section data from object dump. Rebuilding might help.\n");
                        System.exit(-4);
                    }
                    romRestoreElements.add(sec.data.Clone());   // These do not need address padding as they are byte based
                    continue;
                }
            }
        }

        // If built for debug mode, mask the BKBUG bit
        DataRecord FICDRecord = DataRecord.ExtractRange(romRestoreElements, state.deviceBACKBUGAddress, state.deviceBACKBUGAddress + state.flashWordSize);
        if(FICDRecord == null && state.linkedAsDebugImage && (state.deviceBACKBUGAddress != 0))
        {   // FICDRecord == null case
            FICDRecord = new DataRecord();
            FICDRecord.address = state.deviceBACKBUGAddress;
            FICDRecord.architecture16Bit = state.MCU16Mode;
            FICDRecord.assignedMemory = state.BACKBUGConfigWordName;
            FICDRecord.comment = "Generated section for debugging after bootloader erase of Config words flash page";
            FICDRecord.data = new byte[3];
            Arrays.fill(FICDRecord.data, (byte)0xFF);   // Going to clear /BKBUG bit below
        }
        if(FICDRecord != null)
        {
            state.noVerifyRanges.add(FICDRecord.GetAddressRange());  // Do not try to verify the Config word address containing BKBUG since it may or may not have been manipulated by MPLAB ICSP programming
            if(state.linkedAsDebugImage && (state.deviceBACKBUGAddress != 0))
            {
                FICDRecord.data[0] &= (byte)((~state.deviceBACKBUGMask) & 0xFF);
                FICDRecord.data[1] &= (byte)(((~state.deviceBACKBUGMask) & 0xFF00) >> 8);
                FICDRecord.data[2] &= (byte)(((~state.deviceBACKBUGMask) & 0xFF0000) >> 16);
            }
            romRestoreElements.add(FICDRecord); // Put record back in the romRestoreElements list. DataRecord.ExtractRange() removes it from the list.
        }

        // Optimize records and pad/align to minimum flash programming size
        DataRecord.CoalesceRecords(romRestoreElements, true);   // Coalesce all restore items
        DataRecord.PadAlignRecords(romRestoreElements, state.flashWordSize);   // Left pad any records if they don't start on a Flash word size boundary
        DataRecord.CoalesceRecords(romRestoreElements, false);   // Coalesce again in case if the padding now makes two records abut each other


        // Now move/create/force-mask the Reserved bit containing Config Word to the very end of the rom restore table
        if(state.reservedBitAddress != 0)
        {
            long reservedWordStart = state.reservedBitAddress - (state.reservedBitAddress % state.flashWordSize);
            long reservedWordEnd = reservedWordStart + state.flashWordSize;
            DataRecord reservedBitRecord = DataRecord.ExtractRange(romRestoreElements, reservedWordStart, reservedWordEnd);
            if(reservedBitRecord != null)
            {   // Do not try to verify the reserved bit address since it may or may not have a zero bit in it at run time
                state.noVerifyRanges.add(new AddressRange(state.reservedBitAddress, state.reservedBitAddress+0x2L));
                //state.noVerifyRanges.add(reservedBitRecord.GetAddressRange());  // Includes word padding, which is less granular than possible for verification purposes
            }
            if(state.hasFlashConfigWords && state.allowLastPageErase)
            {
                if(reservedBitRecord == null)   // Config word does not exist, so create one
                {
                    reservedBitRecord = new DataRecord();
                    reservedBitRecord.address = reservedWordStart;
                    reservedBitRecord.architecture16Bit = state.MCU16Mode;
                    reservedBitRecord.assignedMemory = state.reserveBitConfigWordName;
                    reservedBitRecord.comment = "Generated section for clearing Reserved Bit";
                    reservedBitRecord.data = new byte[state.flashWordSize / 2 * 3];
                    Arrays.fill(reservedBitRecord.data, (byte)0xFF);
                }
                // Force mask the reserved bit to '0'
                int reservedDataIndex = reservedBitRecord.GetDataIndexOfAddress(state.reservedBitAddress);
                reservedBitRecord.data[reservedDataIndex+0] &= (byte)((~state.reservedBitMask) & 0xFF);
                reservedBitRecord.data[reservedDataIndex+1] &= (byte)(((~state.reservedBitMask) & 0xFF00) >> 8);
                reservedBitRecord.data[reservedDataIndex+2] &= (byte)(((~state.reservedBitMask) & 0xFF0000) >> 16);
                romRestoreElements.add(reservedBitRecord);    // DO NOT sort at this point since this record needs to appear last
            }
            CoalesceRecords(romRestoreElements, false); // Do not sort, just coalesce the reservedBitRecord, if applicable
        }

        // Optimize table generation data
        AddressRange.SortAndReduce(state.noProgramRanges);
        AddressRange.SortAndReduce(state.noEraseRanges);
        AddressRange.SortAndReduce(state.noVerifyRanges);

        appSpaceRegions = AddressRange.SubtractRanges(state.devNVGeometry, state.romUseRanges);
        appSpaceRegions = AddressRange.SubtractRanges(appSpaceRegions, romNoLoadRanges);            // Subtract off any bootloader noload/NEVER_LOAD attributed sections, typically used for data EEPROM emulation, which we don't want to erase as app space and don't want to restrict erase by the application.

        // Sum all the data we are generating and adding to the linker script within Bootloader space
        for(DataRecord dr : romRestoreElements)
        {
            genROMDataAddrSize += 0x4 + (dr.GetEndAddress() - dr.address);  // 0x4 is for start address and byte length record data
        }
        genROMDataAddrSize += 0x4 * (appSpaceRegions.size()
                                     + state.noProgramRanges.size()
                                     + state.noEraseRanges.size()
                                     + state.noVerifyRanges.size()
                                     + state.devNVGeometry.size());
        genROMDataAddrSize += 0x2; // 0x2 offset for header in __EZBL_NV_GEOMETRY_TABLE

        // Generate Pass 1+ linking edits to project .gld file for ordinary bootloaders
        {
            gldOutput.add("/* EZBL addresses discovered in pass 1 linking */\n"
                          + String.format("\n/* ezbl_tools generated rom data size = 0x%1$06X */\n", genROMDataAddrSize));

            gldOutput.add(String.format("\n%1$-32s =   0x%2$06X;", "_EZBL_appBootloadState", state.baseAddressOfAppBootloadState)
                          + String.format("\n%1$-32s =   0x%2$06X;", "_EZBL_APP_RESET_BASE", state.baseAddressOfGotoReset)
                          + String.format("\n%1$-32s =   0x%2$06X;", "_EZBL_IGT_BASE", state.baseAddressOfIGT)
                          + "\n");
            if(state.remapISRThroughIGT)
            {
                gldOutput.add("\n/* Bit mask flags for " + forwardBootloaderFlagsVariableName + " */"
                              + ivtMasks + "\n"
                              + "\n");
            }


            // Add JOURNAL addresses, if present
            if(journalRange.startAddress != -1L)
            {
                gldOutput.add(String.format(
                        "_EZBL_JOURNAL_BASE = 0x%1$06X;\n"
                        + "_EZBL_JOURNAL_END  = 0x%2$06X;\n"
                        + "\n", journalRange.startAddress, journalRange.endAddress));
            }


            // Write formatted bootloader ram use table FYI (EZBL v1.11+)
            gldOutput.add(String.format("\n%1$-36s /* This data FYI - not stored in a flash table */", "#define __EZBL_TOOLS_COMPUTED_BOOT_RAM_USE"));
            for(AddressRange ar : state.ramUseRanges)
            {
                gldOutput.add(String.format("\\\n%1$-36sLONG(0x%2$06X); LONG(0x%3$06X);             ", "", ar.startAddress, ar.endAddress));
            }
            gldOutput.add("\n\n");

//            // Write formatted __EZBL_NV_GEOMETRY_TABLE
//            gldOutput.add("#undef  __EZBL_NV_GEOMETRY_TABLE");
//            gldOutput.add(String.format("\n%1$-36sLONG(%2$d); /* Count of address pairs below */ ", "#define __EZBL_NV_GEOMETRY_TABLE", state.devNVGeometry.size()));
//            for(AddressRange ar : state.devNVGeometry)
//            {
//                gldOutput.add(String.format("\\\n%1$-36sLONG(0x%2$06X); LONG(0x%3$06X);             ", "", ar.startAddress, ar.endAddress));
//            }
//            gldOutput.add("\n\n");
//
//            // Write formatted __EZBL_RAM_USE_TABLE
//            gldOutput.add("#undef  __EZBL_RAM_USE_TABLE");
//            gldOutput.add(String.format("\n%1$-36sLONG(%2$d); /* Count of address pairs below */ ", "#define __EZBL_RAM_USE_TABLE", state.ramUseRanges.size()));
//            for(AddressRange ar : state.ramUseRanges)
//            {
//                gldOutput.add(String.format("\\\n%1$-36sLONG(0x%2$06X); LONG(0x%3$06X);             ", "", ar.startAddress, ar.endAddress));
//            }
//            gldOutput.add("\n\n");
//
//            // Write formatted __EZBL_ROM_USE_TABLE
//            gldOutput.add("#undef  __EZBL_ROM_USE_TABLE");
//            gldOutput.add(String.format("\n%1$-36sLONG(%2$d); /* Count of address pairs below */ ", "#define __EZBL_ROM_USE_TABLE", state.romUseRanges.size()));
//            for(AddressRange ar : state.romUseRanges)
//            {
//                gldOutput.add(String.format("\\\n%1$-36sLONG(0x%2$06X); LONG(0x%3$06X);             ", "", ar.startAddress, ar.endAddress));
//            }
//            gldOutput.add("\n\n");
//
//
//            // Write formatted __EZBL_APP_SPACE_TABLE
//            List<AddressRange> appSpaceRegions = AddressRange.SubtractRanges(state.devNVGeometry, state.romUseRanges);
//            gldOutput.add("#undef  __EZBL_APP_SPACE_TABLE");
//            gldOutput.add(String.format("\n%1$-36sLONG(%2$d); /* Count of address pairs below */ ", "#define __EZBL_APP_SPACE_TABLE", appSpaceRegions.size()));
//            for(AddressRange ar : appSpaceRegions)
//            {
//                gldOutput.add(String.format("\\\n%1$-36sLONG(0x%2$06X); LONG(0x%3$06X);             ", "", ar.startAddress, ar.endAddress));
//            }
//            gldOutput.add("\n\n");
//            // Write formatted __EZBL_ERASE_RESTORE_TABLE
//            gldOutput.add("#undef  __EZBL_ERASE_RESTORE_TABLE");
//            gldOutput.add(String.format("\n%1$-80s", "#define __EZBL_ERASE_RESTORE_TABLE")
//                          + String.format("\\\n%1$-80s", String.format("    LONG(%1$d); /* Count of restorable address ranges (with data) below */", romRestoreElements.size())));
//            for(DataRecord element : romRestoreElements)
//            {
//                String restoreLine = String.format("    LONG(0x%1$06X); LONG(0x%2$06X); /* [Start Address; End Address) */", element.address, element.GetEndAddress());
//                for(long addr = element.address; addr < element.GetEndAddress(); addr += (element.architecture16Bit ? 2 : 4))
//                {
//                    if((addr - element.address) % 10 == 0)
//                    {
//                        gldOutput.add(String.format("\\\n%1$-80s", restoreLine));
//                        restoreLine = "    ";
//                    }
//                    restoreLine += String.format("LONG(0x%1$06X); ", element.GetIntDataAtAddress(addr));
//                }
//                gldOutput.add(String.format("\\\n%1$-80s", restoreLine));
//            }
//            gldOutput.add("\n\n");

            // Write formatted .text.EZBLAppSpaceGeometry (EZBL v1.11+)
            gldOutput.add("#undef  __EZBL_TOOLS_COMPUTED_APP_SPACE_GEOMETRY");
            gldOutput.add(String.format("\n%1$-36s ", "#define __EZBL_TOOLS_COMPUTED_APP_SPACE_GEOMETRY"));
            for(AddressRange ar : appSpaceRegions)
            {
                gldOutput.add(String.format("\\\n%1$-36sLONG(0x%2$06X); LONG(0x%3$06X);             ", "", ar.startAddress, ar.endAddress));
            }
            gldOutput.add("\n\n");

            // Write formatted .text.EZBLNoProgramRanges (EZBL v1.11+)
            gldOutput.add("#undef  __EZBL_TOOLS_COMPUTED_NO_PROGRAM_RANGES");
            gldOutput.add(String.format("\n%1$-80s", "#define __EZBL_TOOLS_COMPUTED_NO_PROGRAM_RANGES"));
            for(AddressRange ar : state.noProgramRanges)
            {
                gldOutput.add(String.format("\\\n%1$-36sLONG(0x%2$06X); LONG(0x%3$06X);             ", "", ar.startAddress, ar.endAddress));
            }
            gldOutput.add("\n\n");

            // Write formatted .text.EZBLNoEraseRanges  (EZBL v1.11+)
            gldOutput.add("#undef  __EZBL_TOOLS_COMPUTED_NO_ERASE_RANGES");
            gldOutput.add(String.format("\n%1$-80s", "#define __EZBL_TOOLS_COMPUTED_NO_ERASE_RANGES"));
            for(AddressRange ar : state.noEraseRanges)
            {
                gldOutput.add(String.format("\\\n%1$-36sLONG(0x%2$06X); LONG(0x%3$06X);             ", "", ar.startAddress, ar.endAddress));
            }
            gldOutput.add("\n\n");

            // Write formatted .text.EZBLNoVerifyRanges  (EZBL v1.11+)
            gldOutput.add("#undef  __EZBL_TOOLS_COMPUTED_NO_VERIFY_RANGES");
            gldOutput.add(String.format("\n%1$-80s", "#define __EZBL_TOOLS_COMPUTED_NO_VERIFY_RANGES"));
            for(AddressRange ar : state.noVerifyRanges)
            {
                gldOutput.add(String.format("\\\n%1$-36sLONG(0x%2$06X); LONG(0x%3$06X);             ", "", ar.startAddress, ar.endAddress));
            }
            gldOutput.add("\n\n");


            // Write formatted .text.EZBLEraseRestoreTable (EZBL v1.11+)
            gldOutput.add("#undef  __EZBL_TOOLS_COMPUTED_ERASE_RESTORE_TABLE");
            gldOutput.add(String.format("\n%1$-84s", "#define __EZBL_TOOLS_COMPUTED_ERASE_RESTORE_TABLE"));
            for(DataRecord element : romRestoreElements)
            {
                String restoreLine = String.format("    LONG(0x%1$06X); LONG(%2$d); /* Start address, byte length [0x%1$06X, 0x%3$06X) */ ", element.address, element.data.length, element.GetEndAddress());
                for(long addr = element.address; addr < element.GetEndAddress(); addr += (element.architecture16Bit ? 2 : 4))
                {
                    if((addr - element.address) % 10 == 0)
                    {
                        gldOutput.add(String.format("\\\n%1$-84s", restoreLine));
                        restoreLine = "    ";
                    }
                    restoreLine += String.format("LONG(0x%1$06X); ", element.GetIntDataAtAddress(addr));
                }
                gldOutput.add(String.format("\\\n%1$-84s", restoreLine));
            }
            gldOutput.add("\n\n");

            // Write EZBL_ForwardBootloaderISR mask symbols and formatted __EZBL_IVT_DISPATCH_CODE
            gldOutput.add("#undef  __EZBL_IVT_DISPATCH_CODE"
                          + String.format("%1$-157s", "\n#define __EZBL_IVT_DISPATCH_CODE ")
                          + ivtDispacher + "\n"
                          + "\n");

        }

        // Read the output .gld file we are going to modify
        fileData = Multifunction.ReadFile(state.linkScriptPath, true);
        if(fileData == null)
        {
            System.err.println("ezbl_tools: could not read \"" + state.linkScriptPath + "\"");
            return;
        }

        // Find the proper FIRST PASS RESULTS SECTION and replace what was in
        // it with our new linking results
        TextBlock outGLD = new TextBlock(fileData);

        if(!outGLD.Find(firstPassResultsSectionRegexStart, firstPassResultsSectionRegexEnd))
        {
            System.err.println("ezbl_tools: could not find section to write first pass linking results in \"" + state.linkScriptPath + "\"");
            return;
        }

        outputValuesString = CatStringList(gldOutput);
        outGLD.ReplaceInner(outputValuesString);

        // Write computed data to output file
        if(Multifunction.UpdateFileIfDataDifferent(state.linkScriptPath, outGLD.GetFullBlock(), true) < 0)
        {
            System.err.println("ezbl_tools: failed to write to \"" + state.linkScriptPath + "\"");
            return;
        }

        // Save any parsing work we've done for later use (ex: symbols in blobber .merge.S/.gld generation)
        state.SaveToFile();
    }

    /**
     * Looks for .const sections and then __EZBL_APP_CONST_RESERVE and
     * __EZBL_APP_CONST_RESERVE_END symbols created by the
     * EZBL_RESERVE_CONST_FOR_APP() macro. The located data range(s) are removed
     * from the section, which could result in fragmented, multiple .const
     * sections being created in its place.
     *
     * @param romSections List of sections to search for ".const" named
     *                    sections, and when matches are found, the List is
     *                    updated to exclude the reserved regions. This may
     *                    result in more total romSections appearing in the List
     *                    as more than one .const section can be created as
     *                    output (in the event the .const section reservation is
     *                    fragmented).
     *
     * @return AddressRanges found and reserved using the
     *         EZBL_RESERVE_CONST_FOR_APP() macro. The list is empty if there is
     *         nothing reserved.
     */
    static public List<AddressRange> FixupConstReservation(List<Section> romSections)   // Not called, probably not tested
    {
        List<AddressRange> reservedRanges = new ArrayList<AddressRange>();
        boolean error = false;

        for(int i = 0; i < romSections.size(); i++)
        {
            Section sec = romSections.get(i);

            // Look for __EZBL_APP_CONST_RESERVE and
            // __EZBL_APP_CONST_RESERVE_END symbols in the .const section.
            // If found, we should treat them as page bounded noload for
            // purposes of generating the .s file. There may be multiple
            // such symbols, so we must add all ranges to make a total
            // byte count.
            if(sec.name.startsWith(".const"))
            {
                List<Symbol> psvReservedList = new ArrayList<Symbol>();

                for(int j = 0; j < sec.symbols.size(); j++)
                {
                    Symbol sym = sec.symbols.get(j);
                    if(sym.name.equals("__EZBL_APP_CONST_RESERVE") || sym.name.equals("__EZBL_APP_CONST_RESERVE_END"))
                    {
                        psvReservedList.add(sym);
                        sec.symbols.remove(j--);
                    }
                }

                Collections.sort(psvReservedList);
                error |= (psvReservedList.size() % 2 != 0);
                for(int j = 0; j < psvReservedList.size() - 1; j += 2)
                {
                    if(!psvReservedList.get(j).name.equals("__EZBL_APP_CONST_RESERVE"))
                    {
                        error = true;
                        continue;
                    }
                    if(!psvReservedList.get(j + 1).name.equals("__EZBL_APP_CONST_RESERVE_END"))
                    {
                        error = true;
                        continue;
                    }

                    long startAddr = psvReservedList.get(j).address;
                    long endAddr = psvReservedList.get(j + 1).address;
                    reservedRanges.add(new AddressRange(startAddr, endAddr));
                    sec = sec.Split(startAddr);     // Keep the left portion in place, set sec = new copy of the right hand portion
                    sec = sec.Split(endAddr);       // Throw away the left hand portion, get a new copy of the right hand portion
                    romSections.add(i + 1, sec);    // Add the right portion as new data in the romSections array
                }
            }
        }

        AddressRange.SortAndReduce(reservedRanges);

        return reservedRanges;
    }

    /**
     * Iterates through all of the sections and computes padding addresses for
     * any section to fill a given padding/alignment sized boundary.
     *
     * Optionally accepts address range select masks to ignore objects
     * overlapping the mask. Also accepts padding keep-out masks to suppress
     * padding from being generated over certain addresses.
     *
     * @param objsToFix           List of sections to search for unused holes
     *                            between,
     *                            and have padding applied to.
     * @param padAlignSize        Address increment to which the alignment and
     *                            end padding should be applied out to.
     * @param paddingKeepoutMasks Optional list of AddressRanges which no
     *                            padding bytes should be generated within.
     * @param objectIgnoreMask    Optional list of AddressRanges which suppress
     *                            objects from having padding added to them when
     *                            overlapping. If an object straddles an
     *                            objectIgnoreMask AddressRange, only the side
     *                            extending into the objectIgnoreMask will have
     *                            padding suppressed for it.
     *
     *
     * @return The address ranges of the input objects and any padding that
     *         should be added are returned in reduced/sorted ordering.
     */
    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPad(List<Object> objsToFix, long padAlignSize, List<AddressRange> objectIgnoreMask, List<AddressRange> paddingKeepoutMasks)
    {
        long objRight = 0;
        long objLeft = 0;
        boolean generateLeftPad;
        boolean generateRightPad;
        List<AddressRange> paddingFragments = new ArrayList<AddressRange>();

        if(objsToFix == null)
        {
            return null;
        }
        if(objsToFix.isEmpty())
        {
            return paddingFragments;
        }

        // Sort the list of objects by start address
        if(objsToFix.get(0).getClass().equals(AddressRange.class))
        {
            Collections.sort((List<AddressRange>)(Object)objsToFix);
        }
        else if(objsToFix.get(0).getClass().equals(Section.class))
        {
            Collections.sort((List<Section>)(Object)objsToFix);
        }
        else if(objsToFix.get(0).getClass().equals(DataRecord.class))
        {
            Collections.sort((List<DataRecord>)(Object)objsToFix);
        }
        else
        {
            return null;
        }


        // First, sort all romSections and noPaddingAddress so we can quickly
        // tell if there is an unused hole before/after a section
        if(objectIgnoreMask != null)
        {
            AddressRange.SortAndReduce(objectIgnoreMask);
        }
        if(paddingKeepoutMasks != null)
        {
            AddressRange.SortAndReduce(paddingKeepoutMasks);
        }

        // Iterate through each romSection
        for(Object o : objsToFix)
        {
            objLeft = o.getClass().equals(Section.class) ? ((Section)o).loadMemoryAddress : objLeft;
            objLeft = o.getClass().equals(AddressRange.class) ? ((AddressRange)o).startAddress : objLeft;
            objLeft = o.getClass().equals(DataRecord.class) ? ((DataRecord)o).address : objLeft;
            objRight = o.getClass().equals(Section.class) ? ((Section)o).loadMemoryAddress + ((Section)o).size : objRight;
            objRight = o.getClass().equals(AddressRange.class) ? ((AddressRange)o).endAddress : objRight;
            objRight = o.getClass().equals(DataRecord.class) ? ((DataRecord)o).GetEndAddress() : objRight;

            // See if the object stradles or lies within an ignore object region
            generateLeftPad = (objLeft % padAlignSize != 0);
            generateRightPad = (objRight % padAlignSize != 0);
            if(objectIgnoreMask != null)
            {
                for(AddressRange ar : objectIgnoreMask)
                {
                    generateLeftPad &= !((objLeft >= ar.startAddress) && (objLeft < ar.endAddress));
                    generateRightPad &= !((objRight >= ar.startAddress) && (objRight < ar.endAddress));
                }
            }


            // Generate padding fragments for this object
            if(generateLeftPad)
            {
                paddingFragments.add(new AddressRange((objLeft / padAlignSize) * padAlignSize, objLeft));
            }
            if(generateRightPad)
            {
                paddingFragments.add(new AddressRange(objRight, (objRight / padAlignSize + 1) * padAlignSize));
            }
        }

        // OR all the padding Fragments together to get rid of duplicate data
        AddressRange.SortAndReduce(paddingFragments);

        // Remove all the padding keep out masks
        if(paddingKeepoutMasks != null)
        {
            paddingFragments = AddressRange.SubtractRanges(paddingFragments, paddingKeepoutMasks);
        }

        return paddingFragments;
    }

    /**
     * Iterates through all of the sections and computes padding addresses for
     * any section to fill a given padding/alignment sized boundary.
     *
     * Optionally accepts address range select masks to ignore objects
     * overlapping the mask. Also accepts padding keep-out masks to suppress
     * padding from being generated over certain addresses.
     *
     * @param objsToFix           List of sections to search for unused holes
     *                            between,
     *                            and have padding applied to.
     * @param padAlignSize        Address increment to which the alignment and
     *                            end padding should be applied out to.
     * @param paddingKeepoutMasks Optional list of AddressRanges which no
     *                            padding bytes should be generated within.
     *
     * @return The address ranges of padding that should be added are returned.
     *         If no padding is needed, the list has zero elements.
     */
    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPad(List<Object> objsToFix, long padAlignSize, List<AddressRange> paddingKeepoutMasks)
    {
        long lastAddress = 0;
        long leftPadSize, rightPadSize;
        long rightAddress = 0;
        long leftAddress = 0;
        List<AddressRange> paddingRanges = new ArrayList<AddressRange>();

        if(objsToFix == null)
        {
            return null;
        }
        if(objsToFix.isEmpty())
        {
            return paddingRanges;
        }

        // Sort the list of objects by start address
        if(objsToFix.get(0).getClass().equals(AddressRange.class))
        {
            Collections.sort((List<AddressRange>)(Object)objsToFix);
        }
        else if(objsToFix.get(0).getClass().equals(Section.class))
        {
            Collections.sort((List<Section>)(Object)objsToFix);
        }
        else if(objsToFix.get(0).getClass().equals(DataRecord.class))
        {
            Collections.sort((List<DataRecord>)(Object)objsToFix);
        }
        else
        {
            return null;
        }


        // First, sort all romSections and noPaddingAddress so we can quickly
        // tell if there is an unused hole before/after a section
        if(paddingKeepoutMasks != null)
        {
            AddressRange.SortAndReduce(paddingKeepoutMasks);
        }

        // Iterate through each romSection
        for(int i = 0; i < objsToFix.size(); i++)
        {
            Object o = objsToFix.get(i);
            leftAddress = o.getClass().equals(Section.class) ? ((Section)o).loadMemoryAddress : leftAddress;
            leftAddress = o.getClass().equals(AddressRange.class) ? ((AddressRange)o).startAddress : leftAddress;
            leftAddress = o.getClass().equals(DataRecord.class) ? ((DataRecord)o).address : leftAddress;
            rightAddress = o.getClass().equals(Section.class) ? ((Section)o).loadMemoryAddress + ((Section)o).size : rightAddress;
            rightAddress = o.getClass().equals(AddressRange.class) ? ((AddressRange)o).endAddress : rightAddress;
            rightAddress = o.getClass().equals(DataRecord.class) ? ((DataRecord)o).GetEndAddress() : rightAddress;

            // Compute size of left-side hole
            leftPadSize = 0;
            if(leftAddress != lastAddress)
            {
                // We may have an unused hole before this item starts
                leftPadSize = leftAddress % padAlignSize;
            }

            // Compute size of right-side hole, assuming the next section doesn't abutt this one.
            rightPadSize = padAlignSize - (rightAddress % padAlignSize);
            if(rightPadSize == padAlignSize)
            {
                rightPadSize = 0;
            }
            for(int j = i + 1; j < objsToFix.size(); j++)
            {
                Object nextO = objsToFix.get(j);
                long nextOLeftAddress = 0;
                nextOLeftAddress = nextO instanceof Section ? ((Section)nextO).loadMemoryAddress : nextOLeftAddress;
                nextOLeftAddress = nextO instanceof AddressRange ? ((AddressRange)nextO).startAddress : nextOLeftAddress;
                nextOLeftAddress = nextO instanceof DataRecord ? ((DataRecord)nextO).address : nextOLeftAddress;

                if(rightAddress == nextOLeftAddress)
                {
                    rightPadSize = 0;
                }
                else if(nextOLeftAddress < rightAddress)    // Ignore sections which overlap us and test the next one instead. Overlapped sections can occur durring pass 1 linking where linker is told to allow overlap.
                {
                    continue;
                }
                else if(rightAddress + rightPadSize > nextOLeftAddress)
                {
                    rightPadSize = nextOLeftAddress - rightAddress;
                }
                break;
            }

            if(leftPadSize + rightPadSize != 0)
            {

                if(paddingKeepoutMasks != null)
                {
                    // Update left and right padding sizes if they overlap with a paddingKeepoutMask
                    for(AddressRange paddingKeepoutMask : paddingKeepoutMasks)
                    {
                        long leftAddr = paddingKeepoutMask.startAddress;
                        long rightAddr = paddingKeepoutMask.endAddress;
                        if(leftAddr > rightAddress + rightPadSize)
                        {
                            break;
                        }
                        if(rightAddr < leftAddress - leftPadSize)
                        {
                            continue;
                        }
                        if((leftAddr < leftAddress) && (rightAddr > leftAddress - leftPadSize))
                        {
                            // We have to alter the left pad
                            if(leftAddr > leftAddress - leftPadSize)
                            {
                                // TODO: The allowed hole lies within our padding, we need to split the padding and create an orphin pad on the left to accurately accomodate this
                            }
                            leftPadSize = leftAddress - rightAddr;
                            if(leftPadSize < 0)
                            {
                                leftPadSize = 0;
                            }
                        }
                        if((leftAddr < rightAddress + rightPadSize) && (rightAddr > rightAddress))
                        {
                            // We have to alter the right pad
                            if(rightAddr < rightAddress + rightPadSize)
                            {
                                // TODO: The allowed hole lies within our padding, we need to split the padding and create an orphin pad on the right to accurately accomodate this
                            }
                            rightPadSize = leftAddr - rightAddress;
                            if(rightPadSize < 0)
                            {
                                rightPadSize = 0;
                            }
                        }
                    }
                }

                DataRecord dr = null;

                if(o instanceof Section)
                {
                    dr = ((Section)o).data;
                    ((Section)o).loadMemoryAddress -= leftPadSize;
                    ((Section)o).virtualMemoryAddress -= leftPadSize;
                    ((Section)o).size += leftPadSize + rightPadSize;
                }
                else if(o instanceof DataRecord)
                {
                    dr = ((DataRecord)o);
                }
                else if(o instanceof AddressRange)
                {
                    ((AddressRange)o).startAddress -= leftPadSize;
                    ((AddressRange)o).endAddress += rightPadSize;
                }

                if(dr != null)
                {
                    dr.address -= leftPadSize;
                    if(dr.data != null)
                    {
                        if(dr.architecture16Bit)
                        {
                            // Convert padding size addresses to bytes
                            leftPadSize = leftPadSize / 2 * 3;
                            rightPadSize = rightPadSize / 2 * 3;
                            rightPadSize += dr.data.length % 3;
                        }
                        byte[] newData = new byte[dr.data.length + (int)(leftPadSize + rightPadSize)];
                        Arrays.fill(newData, (byte)0xFF);
                        System.arraycopy(dr.data, 0, newData, (int)leftPadSize, dr.data.length);
                        dr.data = newData;
                    }
                }

                if(leftPadSize != 0)
                {
                    paddingRanges.add(new AddressRange(leftAddress, leftAddress + leftPadSize));
                }
                if(rightPadSize != 0)
                {
                    paddingRanges.add(new AddressRange(rightAddress, rightAddress + rightPadSize));
                }
            }

            // Update last Address so we can find left-side holes
            lastAddress = rightAddress + rightPadSize;
        }

        AddressRange.SortAndReduce(paddingRanges);

        return paddingRanges;
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadSections(List<Section> objsToFix, long padAlignSize, List<AddressRange> noPaddingAddresses)
    {
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadSections(Section objToFix, long padAlignSize, List<AddressRange> noPaddingAddresses)
    {
        List<Section> objsToFix = new ArrayList<Section>();
        objsToFix.add(objToFix);
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadSections(List<Section> objsToFix, long padAlignSize, AddressRange noPaddingAddress)
    {
        List<AddressRange> noPaddingAddresses = null;
        if(noPaddingAddress != null)
        {
            noPaddingAddresses = new ArrayList<AddressRange>();
            noPaddingAddresses.add(noPaddingAddress);
        }
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadSections(Section objToFix, long padAlignSize, AddressRange noPaddingAddress)
    {
        List<Section> objsToFix = new ArrayList<Section>();
        objsToFix.add(objToFix);
        List<AddressRange> noPaddingAddresses = null;
        if(noPaddingAddress != null)
        {
            noPaddingAddresses = new ArrayList<AddressRange>();
            noPaddingAddresses.add(noPaddingAddress);
        }
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadSections(Section objToFix, long padAlignSize)
    {
        List<Section> objsToFix = new ArrayList<Section>();
        objsToFix.add(objToFix);
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, null);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadAR(List<AddressRange> objsToFix, long padAlignSize, List<AddressRange> noPaddingAddresses)
    {
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadAR(AddressRange objToFix, long padAlignSize, List<AddressRange> noPaddingAddresses)
    {
        List<AddressRange> objsToFix = new ArrayList<AddressRange>();
        objsToFix.add(objToFix);
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadAR(List<AddressRange> objsToFix, long padAlignSize, AddressRange noPaddingAddress)
    {
        List<AddressRange> noPaddingAddresses = null;
        if(noPaddingAddress != null)
        {
            noPaddingAddresses = new ArrayList<AddressRange>();
            noPaddingAddresses.add(noPaddingAddress);
        }
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadAR(AddressRange objToFix, long padAlignSize, AddressRange noPaddingAddress)
    {
        List<AddressRange> objsToFix = new ArrayList<AddressRange>();
        objsToFix.add(objToFix);
        List<AddressRange> noPaddingAddresses = null;
        if(noPaddingAddress != null)
        {
            noPaddingAddresses = new ArrayList<AddressRange>();
            noPaddingAddresses.add(noPaddingAddress);
        }
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadAR(AddressRange objToFix, long padAlignSize)
    {
        List<AddressRange> objsToFix = new ArrayList<AddressRange>();
        objsToFix.add(objToFix);
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, null);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadDR(List<DataRecord> objsToFix, long padAlignSize, List<AddressRange> noPaddingAddresses)
    {
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadDR(DataRecord objToFix, long padAlignSize, List<AddressRange> noPaddingAddresses)
    {
        List<DataRecord> objsToFix = new ArrayList<DataRecord>();
        objsToFix.add(objToFix);
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadDR(List<DataRecord> objsToFix, long padAlignSize, AddressRange noPaddingAddress)
    {
        List<AddressRange> noPaddingAddresses = null;
        if(noPaddingAddress != null)
        {
            noPaddingAddresses = new ArrayList<AddressRange>();
            noPaddingAddresses.add(noPaddingAddress);
        }
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadDR(DataRecord objToFix, long padAlignSize, AddressRange noPaddingAddress)
    {
        List<DataRecord> objsToFix = new ArrayList<DataRecord>();
        objsToFix.add(objToFix);
        List<AddressRange> noPaddingAddresses = null;
        if(noPaddingAddress != null)
        {
            noPaddingAddresses = new ArrayList<AddressRange>();
            noPaddingAddresses.add(noPaddingAddress);
        }
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, noPaddingAddresses);
    }

    @SuppressWarnings("unchecked")
    static public List<AddressRange> FixupAlignAndPadDR(DataRecord objToFix, long padAlignSize)
    {
        List<DataRecord> objsToFix = new ArrayList<DataRecord>();
        objsToFix.add(objToFix);
        return FixupAlignAndPad((List<Object>)(Object)objsToFix, padAlignSize, null);
    }

//    /**
//     * Moves any Sections that overlap each other to addresses that won't
//     * overlap each other. This is useful for correcting first pass linking
//     * results in which things like the .text.EZBL_Tables section declared and
//     * explicitly located in the linker script can appear at address 0x000000
//     * while the .reset section is also located at the same address.
//     *
//     * @param objsToFix        List of Sections to look for overlap in. In the
//     *                         event of
//     *                         overlap, one of the sections will be moved to an address
//     *                         range that doesn't overlap with any other sections.
//     * @param keepOutAddresses List of AddressRanges which we should not use
//     *                         when we have to move a section.
//     *
//     * @return List of historical overlap regions that were found and corrected.
//     */
//    static public List<AddressRange> FixupMoveOverlapSections(List<Section> objsToFix, List<AddressRange> keepOutAddresses)
//    {
//        long objLeft;
//        long objRight;
//        long size;
//        long compareLeft;
//        long compareRight;
//        long compareSize;
//        long holeSize;
//        List<AddressRange> overlaps = new ArrayList<AddressRange>();
//
//        if(objsToFix == null)
//        {
//            return null;
//        }
//        if(objsToFix.isEmpty())
//        {
//            return overlaps;
//        }
//
//        // Sort the objects by loadMemoryAddress
//        Collections.sort(objsToFix);
//
//        objLeft = 0;
//        objRight = 0;
//        for(int i = 0; i < objsToFix.size(); i++)
//        {
//            size = objsToFix.get(i).size;
//            if(size == 0)
//            {
//                continue;
//            }
//            objLeft = objsToFix.get(i).loadMemoryAddress;
//            objRight = objLeft + size;
//
//            // Compare current section with all other Sections
//            for(int j = 0; j < objsToFix.size(); j++)
//            {
//                compareSize = objsToFix.get(j).size;
//                if(compareSize == 0)
//                {
//                    continue;
//                }
//                compareLeft = objsToFix.get(j).loadMemoryAddress;
//                compareRight = compareLeft + compareSize;
//                if(compareLeft < objRight) // Non zero length section and our sorted array has a right boundary on the right of this sections left boundary
//                {
//                    overlaps.add(new AddressRange(compareLeft, objRight));
//                }
//            }
//        }
//
//        return overlaps;
//    }

    /**
     * Create a device ROM address map (__EZBL_NV_GEOMETRY_TABLE) section in
     * the
     * given section list.
     *
     * @param romSections List of sections to add to
     *
     * @return number of bytes created. New section is added to romSections.
     */
    static public int FixupCreateNVGeometry(List<Section> romSections)
    {
        Section nvGeometry = new Section();

        nvGeometry.alignment = 0x2;
        nvGeometry.flags.ALLOC = true;
        nvGeometry.flags.CODE = true;
        nvGeometry.flags.CONTENTS = true;
        nvGeometry.flags.READONLY = true;
        nvGeometry.isROM = true;
        //nvGeometry.data.address =
        //nvGeometry.data.architecture16Bit =
        nvGeometry.data.assignedMemory = "program";
        //nvGeometry.data.data =
        //nvGeometry.loadMemoryAddress
        //nvGeometry.virtualMemoryAddress
        //nvGeometry.size

        romSections.add(nvGeometry);
        return (int)nvGeometry.size;
    }
}

