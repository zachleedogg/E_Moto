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
import java.util.*;
import java.util.regex.Matcher;


/**
 *
 * @author C12128
 */
public class GLDBuilder
{
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args)
    {
        String gldContents;
        TextBlock deviceGLD;
        String dynamicSectionsHeader = "", dynamicSections = "", dynamicSectionsFooter = "";

        String equDefineSectionTagsRegexStart = "AUTOMATIC EQU/DEFINE FILL SECTION[^\n]*?\n";
        String equDefineSectionTagsRegexEnd = "(?<=\n)[ \t]*?/[*][^E\n]*END OF EQU/DEFINE FILL SECTION";
        String igtFillSectionTagsRegexStart = "AUTOMATIC IGT FILL SECTION[^\n]*?\n";
        String igtFillSectionTagsRegexEnd = "(?<=\n)[ \t]*?/[*][^E\n]*END OF AUTOMATIC IGT FILL SECTION";
        String deviceSectionFillSectionTagsRegexStart = "AUTOMATIC DEVICE SECTION FILL SECTION[^\n]*?\n";
        String deviceSectionFillSectionTagsRegexEnd = "(?<=\n)[ \t]*?/[*][^E\n]*END OF DEVICE SECTION FILL SECTION";

        // Obtain the given command line options and all prior ezbl_tools invocation state information
        String stateSavePath = CommandAndBuildState.GetStateFilePath(args);
        EZBLState state = EZBLState.ReadFromFile(stateSavePath);        // Load up the preexisting state file from disk, if one exists
        state = CommandAndBuildState.ReadArgs(state, args);             // Parse any new command line options and save them

        if(state.undecodedOptions.contains("-make_non_ezbl_gld"))
        {
            System.exit(CreateNonEZBLLinkerScripts(args));
        }

        // If nothing is specified upon execution or a pasing error occured, write usage information to STDOUT
        if(!state.parseOkay)
        {
            System.out.print("\r\n"
                             + Multifunction.FormatHelpText(79, 0 * 3, "Usage:")
                             + Multifunction.FormatHelpText(79, 1 * 3, "java -jar ezbl_tools.jar --gldbuilder -mcpu=PIC [-options] \"path to XC16 bins\" \"linker_script.gld\"")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Inputs:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Target PIC part number: ex: -mcpu=33EP64MC506, -mcpu=24FJ128GA010,-mcpu=33FJ256GP710A")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "Path to the XC16 bin installation folder for device .gld searching. Ex: \"C:\\Program Files (x86)\\Microchip\\xc16\\v1.21\\bin\"")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "Preexisting ezbl_build_as_one.gld or ezbl_build_standalone.gld linker script to insert parsed device specific linker script information into.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Options:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "-ignore=0x002000,0x3000")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Address range to drop from the device Flash MEMORY regions in the linker script. Typically, this is set to a bootloader start address for the first paramter and a bootloader end address for the second end parameter. The second parameter is exclusive, or 1 address greater than the actual ignore range. This option may be repeated if multiple address ranges should be ignored.")
                             // build_as_one is deprecated; build_standalone is the default, so there is no need for either option displayed now
                             //+ "\r\n"
                             //+ Multifunction.FormatHelpText(79, 2 * 3, "-build_standalone")
                             //+ Multifunction.FormatHelpText(79, 3 * 3, "Configures the output linker script for creating a standalone bootloader. This is the default if -build_as_one or other build plan is not specified.")
                             //+ "\r\n"
                             //+ Multifunction.FormatHelpText(79, 2 * 3, "-build_as_one")
                             //+ Multifunction.FormatHelpText(79, 3 * 3, "Configures the output linker script for creating an application and linking with the base bootloader library directly.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Outputs:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Appropriately tagged sections in the preexisting ezbl .gld linker script are updated to reflect the information in the current device .gld file.")
                             + "\r\n"
            );

            return;
        }

        // Validate input parameters
        if(state.partNumber == null || state.compilerLinkScriptPath == null || state.linkScriptPath == null)
        {
            System.err.println("ezbl_tools: Missing required input parameter: need a device part number, compiler linker script to start from, and output linker script file specified.");
            System.exit(-1);
        }

        // Read in the project'sec .gld file that we will be reading and likely editing
        gldContents = Multifunction.ReadFile(state.linkScriptPath, true);
        if(gldContents == null)
        {
            System.err.println("ezbl_tools: Unable to open \"" + state.linkScriptPath + "\". Ensure project contains valid EZBL .gld linker script.");
            System.exit(-2);
        }

        // Convert .gld intput byte stream into a parsable TextBlock
        TextBlock outGLD = new TextBlock(gldContents);

        // Read the compiler's device .gld contents into the deviceGLD TextBlock class
        gldContents = Multifunction.ReadFile(state.compilerLinkScriptPath, true);
        if(gldContents == null)
        {
            System.err.println("ezbl_tools: Unable to read \"" + state.compilerLinkScriptPath + "\"");
            System.exit(-3);
        }
        deviceGLD = new TextBlock(gldContents);
        state.compilerGLDContents = gldContents; // Also save a copy in the app state so we don't have to read it again in a later invocation

        // If a bootloader base address is defined, we need to modify the
        // bootloader .gld MEMORY regions to remove execution memories that
        // exist before the specified base address. This ensures the bootloader
        // doesn't build with anything allocated before the base address.
        // TODO: Make this work for Dual Partition devices which have
        // preprocessor commands and duplicit memory regions. Presently, a dual
        // partition device will only work when the bootloader base address is
        // 0x000000.
        if(state.baseAddress > 0)
        {
            // Process the MEMORY definitions region from the original device linker script
            deviceGLD.ResetFind();
            if(!deviceGLD.Find("\\s*MEMORY[^{]*", "\\{", "\\}", "\n"))
            {
                System.err.println("ezbl_tools: Couldn't find MEMORY block in compiler's device linker script");
                System.exit(-4);
                return;
            }

            // Parse all the defined regions into the GLDMemories class
            state.gldMemories = new GLDMemories(deviceGLD.GetOuter());

            // Determine which Program memory regions come before the
            // -base=0x##### command line option base address and should be
            // removed when linking the project
            List<AddressRange> reservedMemories = new ArrayList<AddressRange>();
            reservedMemories.add(new AddressRange(state.gldMemories.execution.get(0).origin, state.baseAddress));

            // Actually subtract the ignore ranges and save back to a .gld file
            // text. We will also save the areas we deleted so we can use them
            // later if needed for something.
            state.deletedMemories = SubtractIgnoreRangesFromMemories(state.gldMemories, reservedMemories);
            deviceGLD.ReplaceOuter(state.gldMemories.toString());
        }

        // Process all SECTIONS regions
        while(deviceGLD.Find("SECTIONS[^{]*", "\\{", "\\}", "\n"))
        {
            if(dynamicSectionsHeader.isEmpty())
            {
                dynamicSectionsHeader = deviceGLD.GetOuterLeft(false);
                dynamicSectionsFooter = deviceGLD.GetOuterRight(false);
            }
            TextBlock sectionBlock = new TextBlock(deviceGLD.GetInner(true));

            // Process any .ivt sections we find. These need to be remapped into
            // application space. Simultaneously we want to generate an
            // "Interrupt Goto Table", or .igt section. This code could be
            // modified to support an Interrupt Branch Table instead on devices
            // with small enough memory for branching to any address.
            if(state.remapISRThroughIGT)
            {
                if(sectionBlock.Find("\\s[.]ivt[^:]*:[^{]*", "\\{", "\\}", "\n"))
                {
                    String staticIVT = "";
                    String gldParse = sectionBlock.GetInner(true);

                    gldParse = gldParse.replaceAll("__DefaultInterrupt", "");
                    gldParse = gldParse.replaceAll("LONG[\\s]*?[(]", "");
                    gldParse = gldParse.replaceAll("ABSOLUTE[\\s]*?[(]", "");
                    gldParse = gldParse.replaceAll(":[\\s\\S]*?;", "");
                    gldParse = gldParse.replaceAll("DEFINED[\\s]*?[(][\\s\\S]*?[?]", "");
                    gldParse = gldParse.replaceAll("[)]", "");
                    gldParse = gldParse.replaceAll("[\\s]+", "\n");
                    gldParse = gldParse.trim();

                    // Save a list of possible ivtVectors for the DumpParser to determine which are in use
                    state.ivtVectors = gldParse.split("\n");

                    // Convert the IVT entries to point to an IGT Entry and create
                    // the IGT
                    {
                        state.erasableIGT = "";

                        // First, count out how many true interrupts are implemented
                        // on this device and create the IGT
                        state.ivtVectorImplemented = new boolean[state.ivtVectors.length];
                        for(int i = 0; i < state.ivtVectors.length; i++)
                        {
                            // Skip interrupt ivtVectors named "__InterruptX", where X is a number.
                            // These are reserved filler entries in the IVT that have no physical
                            // hardware that can invoke them, so we don't need to waste IGT gotos
                            // on them.
                            if(state.ivtVectors[i].startsWith("__Interrupt"))
                            {
                                state.ivtVectorImplemented[i] = false;
                                continue;
                            }

                            String formattedVectorName = String.format("%1$-18s", state.ivtVectors[i]);
                            state.erasableIGT += "\n      LONG(0x040000 | (((DEFINED(" + formattedVectorName + ") ? ABSOLUTE(" + formattedVectorName + ") : ABSOLUTE(__DefaultInterrupt)) & 0x00FFFF)    ));";
                            state.erasableIGT += "\n      LONG(0x000000 | (((DEFINED(" + formattedVectorName + ") ? ABSOLUTE(" + formattedVectorName + ") : ABSOLUTE(__DefaultInterrupt)) & 0x7F0000)>>16));";
                            state.ivtVectorImplemented[i] = true;
                        }

                        // Create the last IGT goto entry for unimplemented hardware ivtVectors that just points to the __DefaultInterrupt (in case if ever a new vector is added because it was missing originally in the compiler .gld)
                        state.erasableIGT += "\n      LONG(0x040000 | ((ABSOLUTE(__DefaultInterrupt) & 0x00FFFF)    ));";
                        state.erasableIGT += "\n      LONG(0x000000 | ((ABSOLUTE(__DefaultInterrupt) & 0x7F0000)>>16));";

                        // Rebuild the .ivt section with all ivtVectors pointing into
                        // the dynamic IGT. Now that we know how many ivtVectors there
                        // are total, we know the address of where we should put our
                        // default interrupt and can build the static IVT.
                        int outputIndex = 0;
                        for(int i = 0; i < state.ivtVectors.length; i++)
                        {
                            String formattedVectorName = String.format("%1$-26s", state.ivtVectors[i]);

                            if(state.ivtVectors[i].startsWith("__Interrupt"))
                            {
                                String s = String.format("\n      LONG(ABSOLUTE(_EZBL_IGT_BASE) + 0x%1$04X);", state.ivtVectorsImplemented * 4);
                                String s2 = String.format("/* %1$-26s, IRQ %2$3d, Vector %3$3d */", "__DefaultInterrupt", i, i + 8);
                                staticIVT += String.format("%1$-150s %2$s", s, s2);
                            }
                            else
                            {
                                // In standalone mode, only remap ivtVectors that the
                                // bootloader doesn't implement
                                if(state.standaloneMode)
                                {
                                    staticIVT += "\n      LONG(DEFINED(EZBL_Dispatch" + formattedVectorName + ") ? ABSOLUTE(EZBL_Dispatch" + formattedVectorName + ") : ABSOLUTE(_EZBL_IGT_BASE) + 0x" + String.format("%1$04X", outputIndex++ * 4) + "); /* " + formattedVectorName + String.format(", IRQ %1$3d, Vector %2$3d", i, i + 8) + " */";
                                }
                                else    // In build_as_one mode (or others besides standalone), remap all ivtVectors.
                                {
                                    staticIVT += "\n      LONG(ABSOLUTE(_EZBL_IGT_BASE) + 0x" + String.format("%1$04X", outputIndex++ * 4) + "); /* " + formattedVectorName + String.format(", IRQ %1$3d, Vector %2$3d", i, i + 8) + " */";
                                }
                            }
                        }

                        // Format .ivt and retain other text near it
                        staticIVT = sectionBlock.GetOuterLeft(true).replaceAll("\n(?! )", "\n  ") + "{" + staticIVT + "\n  " + sectionBlock.GetOuterRight(false);
                    }

                    // Update .ivt to point to bootloader space or delete it
                    sectionBlock.ReplaceOuter(staticIVT);
                }
            }
            else
            {
                state.erasableIGT = "";
            }

            if(state.deleteResetSection)
            {
                // Find and delete .reset section. We will be using our own.
                if(sectionBlock.Find("\\s[.]reset[^:]*:[^{]*", "\\{", "\\}", "\n"))
                {
                    sectionBlock.ReplaceOuter("\n");
                }
            }

            dynamicSections += sectionBlock.GetFullBlock();
            deviceGLD.DeleteOuter();
        }

        // If an encryption password was supplied, save it in the project so it
        // can be used at run time to decode .blobs encrypted with the same
        // password
        if(state.encryptionKey != null)
        {
            String saltString = "";
            for(int i = 0; i < 16; i++)
            {
                saltString += String.format("%1$02X", state.encryptionSalt[i]);
            }

            dynamicSections += String.format("\n  /* Encryption password: %1$s */ ", state.encryptionPassword)
                               + "\n  /* Encryption salt: " + saltString + " */"
                               + "\n  EZBL_CryptKey :"
                               + "\n  {"
                               + "\n      _EZBL_CryptKey = ABSOLUTE(.);"
                               + "\n      ";
            for(int i = 0; i < 15; i += 3)
            {
                dynamicSections += String.format("LONG(0x%3$02X%2$02X%1$02X); ", state.encryptionKey[i], state.encryptionKey[i + 1], state.encryptionKey[i + 2]);
            }
            dynamicSections += String.format("LONG(0x%3$02X%2$02X%1$02X); ", state.encryptionKey[15], 0x00, 0x00);
            dynamicSections += "\n  } >program"
                               + "\n";
        }

        // Append header and footer closing braces and extras
        dynamicSections = dynamicSectionsHeader + dynamicSections + dynamicSectionsFooter;

        // AUTOMATIC EQU/DEFINE FILL SECTION
        if(outGLD.Find(equDefineSectionTagsRegexStart, equDefineSectionTagsRegexEnd))
        {
            List<String> deviceParameters = new ArrayList<String>();
            deviceParameters.add(String.format("_EZBL_MAIN_FLASH_BASE           = 0x%1$06X;\n", state.mainFlashRange.startAddress | (state.dualPartitionMode ? 0x400000 : 0)));
            deviceParameters.add(String.format("_EZBL_MAIN_FLASH_END_ADDRESS    = 0x%1$06X;\n", state.mainFlashRange.endAddress | (state.dualPartitionMode ? 0x400000 : 0)));
            deviceParameters.add(String.format("_EZBL_AUX_FLASH_BASE            = 0x%1$06X;\n", state.auxFlashRange == null ? 0 : state.auxFlashRange.startAddress));
            deviceParameters.add(String.format("_EZBL_AUX_FLASH_END_ADDRESS     = 0x%1$06X;\n", state.auxFlashRange == null ? 0 : state.auxFlashRange.endAddress));
            deviceParameters.add(String.format("_EZBL_CONFIG_BASE               = 0x%1$06X;\n", state.configWordsRange == null ? 0 : state.configWordsRange.startAddress | (state.dualPartitionMode ? 0x400000 : 0)));
            deviceParameters.add(String.format("_EZBL_CONFIG_END_ADDRESS        = 0x%1$06X;\n", state.configWordsRange == null ? 0 : state.configWordsRange.endAddress | (state.dualPartitionMode ? 0x400000 : 0)));
            deviceParameters.add(String.format("_EZBL_DEVID_ADDRESS             = 0x%1$06X;\n", state.deviceIDAddress));
            deviceParameters.add(String.format("_EZBL_DEVID_MASK                = 0x%1$06X;\n", state.deviceIDMask));
            deviceParameters.add(String.format("_EZBL_DEVID_VALUE               = 0x%1$06X;\n", state.deviceIDValue));
            deviceParameters.add(String.format("_EZBL_REVID_ADDRESS             = 0x%1$06X;\n", state.deviceRevAddress));
            deviceParameters.add(String.format("_EZBL_REVID_MASK                = 0x%1$06X;\n", state.deviceRevMask));
            deviceParameters.add(String.format("_EZBL_RESERVED_BIT_ADDRESS      = 0x%1$06X;   %2$s\n", state.reservedBitAddress | (state.dualPartitionMode ? 0x400000 : 0), "/* " + state.reserveBitConfigWordName + " */"));
            deviceParameters.add(String.format("_EZBL_RESERVED_BIT_MASK         = 0x%1$06X;\n", state.reservedBitMask));
            deviceParameters.add(String.format("_EZBL_CODE_PROTECT_ADDRESS      = 0x%1$06X;   %2$s\n", state.codeProtectAddress | (state.dualPartitionMode ? 0x400000 : 0), "/* " + state.codeProtectConfigWordName + " */"));
            deviceParameters.add(String.format("_EZBL_CODE_PROTECT_MASK         = 0x%1$06X;\n", state.codeProtectMask));
            deviceParameters.add(String.format("_EZBL_BACKBUG_ADDRESS           = 0x%1$06X;   %2$s\n", state.deviceBACKBUGAddress | (state.dualPartitionMode ? 0x400000 : 0), "/* " + state.BACKBUGConfigWordName + " */"));
            deviceParameters.add(String.format("_EZBL_BACKBUG_MASK              = 0x%1$06X;\n", state.deviceBACKBUGMask));

            for(PairWithText p : state.deviceConfigWords)
            {
                deviceParameters.add(String.format("_EZBL_%1$-25s = 0x%2$06X;\n", p.text, p.first));
            }

            // Commented out things that aren't used anymore
            outGLD.ReplaceInner(
                    deviceGLD.GetFullBlock()
                    + "\n"
                    + "\n"
                    + Multifunction.CatStringList(deviceParameters)
                    + "\n"
                    //+ (state.fCore ? "#define __EZBL_F_CORE                     /* PIC24F/dsPIC30F/dsPIC33F/PIC24H CPU core family */\n" : "")
                    //+ (state.eCore ? "#define __EZBL_EP_CORE                    /* dsPIC33D/dsPIC33E/PIC24E CPU core family */\n" : "")
                    //+ (state.cCore ? "#define __EZBL_C_CORE                     /* dsPIC33C CPU core family, including devices with Slave cores */\n" : "")
                    //+ (state.hasFlashConfigWords ? "#define __EZBL_CONFIG_IN_FLASH              /* Configuration Words live in Flash; this makes erasing typically the last Flash page special */\n" : "")
                    + String.format("#define __EZBL_PROGRAM_BASE       0x%1$06X\n", state.mainFlashRange.startAddress)
                    + String.format("#define __EZBL_PROGRAM_LENGTH     0x%1$06X\n", state.mainFlashRange.endAddress - state.mainFlashRange.startAddress)
                    + String.format("#define __EZBL_PROGRAM_ERASE_SIZE 0x%1$06X\n", state.eraseBlockSizeAddresses)
                    + String.format("#define __EZBL_BASE_ADDRESS       0x%1$06X\n", state.baseAddress)
                    + String.format("#define __EZBL_IGT_ADDRESSES      0x%1$06X", state.remapISRThroughIGT ? 0x4 * (state.ivtVectorsImplemented + 1) : 0)
                    + " /* Number of Flash addresses needed to store all Interrupt Goto Table entries, including one for the __DefaultInterrupt */\n"
                    + "\n");
        }
        else
        {
            // If we don't find anything, reset the search locations so that subsequent sections can still be found
            outGLD.ResetFind();
            System.err.println("ezbl_tools: Unable to find AUTOMATIC EQU/DEFINE FILL SECTION in " + state.linkScriptPath + "; skipping insertion of this section. Ensure correct linker script in use and marker lines are present.");
        }

        // Don't ResetFind() - IGT section must always be after EQU/DEFINE section, so we need not search all the prior text
        // AUTOMATIC IGT FILL SECTION
        if(!state.standaloneMode)
        {
            if(outGLD.Find(igtFillSectionTagsRegexStart, igtFillSectionTagsRegexEnd))
            {
                outGLD.ReplaceInner(state.erasableIGT + "\n");
            }
            else
            {
                // If we don't find anything, reset the search locations so that subsequent sections can still be found
                outGLD.ResetFind();
                System.err.println("ezbl_tools: Unable to find AUTOMATIC IGT FILL SECTION in " + state.linkScriptPath + "; skipping insertion of this section. Ensure correct linker script in use and marker lines are present.");
            }
        }

        // Don't ResetFind() - section area must always be at the end of the file, so we need not search all the prior text
        // AUTOMATIC DEVICE SECTION FILL SECTION
        if(outGLD.Find(deviceSectionFillSectionTagsRegexStart, deviceSectionFillSectionTagsRegexEnd))
        {
            outGLD.ReplaceInner(dynamicSections);
        }
        else
        {
            // If we don't find anything, reset the search locations so that subsequent sections can still be found
            outGLD.ResetFind();
            System.err.println("ezbl_tools: Unable to find AUTOMATIC DEVICE SECTION FILL SECTION in " + state.linkScriptPath + "; skipping insertion of this section. Ensure correct linker script in use and marker lines are present.");
        }

        // Modify the requested output.gld file with our new linker information
        if(Multifunction.UpdateFileIfDataDifferent(state.linkScriptPath, outGLD.GetFullBlock(), true) < 0)
        {
            System.err.println("ezbl_tools: failed to write to \"" + state.linkScriptPath + "\"");
        }

        state.SaveToFile();
    }

    /**
     * Generates an absolute assembly file containing the specified
     * application/bootloader bl2 encoded as literal, post-linked text for
     * encapsulation in another project. A suitable .gld linker script is also
     * generated.
     *
     * @param state State of the build process. Must be created in one of the
     *              other modules (ex: MakeEditor + GLDBuilder + DumpParser +
     *              Blobber chain)
     * @param bl2   Bl2b class to extract program data from
     *
     * @return New linker script text in index 0, plus asm (.sec) file text in
     *         index 1. Array length is always 2 for success, or null return for
     *         failure.
     */
    public static String[] CreateMergeScript(EZBLState state, Bl2b bl2, ELFDumpParser elfData)
    {
        // Items from the bl2
        List<Symbol> romSymbols = new ArrayList<Symbol>();
        List<Symbol> ramSymbols = new ArrayList<Symbol>();
        List<Symbol> otherSymbols = new ArrayList<Symbol>();

        String gldContents;

        List<PairWithText> gldPairs = new ArrayList<PairWithText>();
        String gldSectionAllocation;

        // Items for the output .gld and .sec files
        List<String> symbolTable = new ArrayList<String>();
        List<String> exportTable = new ArrayList<String>();
        List<String> asmMerge = new ArrayList<String>();
        long EZBL_ForwardBootloaderISRAddr = 0;
        AddressRange appBootloadStateRange;
        List<Section> romNoLoadSections = new ArrayList<Section>();

        String[] retGLDASM = new String[2];

        if(elfData == null || (state.romUseRanges == null))
        {
            // Validate input parameters
            if(state.partNumber == null || ((state.compilerGLDContents == null) && (state.compilerLinkScriptPath == null) && state.MCU16Mode))
            {
                if(!state.silent)
                {
                    System.err.println("ezbl_tools: Missing required input parameter. Device part number and compiler linker script path required.");
                }
                System.exit(-10);
            }
        }

        {
            // Expand bl2 ROM ranges to include all unused addresses that are
            // within bootloader space (so that they can't be reused by anything
            // during merge linking.
            for(AddressRange ar : state.romUseRanges)
            {
                bl2.FillData(ar, (byte)0xFF);
            }

            // Remove appBootloadState range since the Bootloader will be
            // writing to it
            appBootloadStateRange = new AddressRange();
            appBootloadStateRange.startAddress = state.baseAddressOfAppBootloadState;
            appBootloadStateRange.endAddress = state.baseAddressOfAppBootloadState + EZBLState.sizeOfAppBootloadState;
            bl2.RemoveIgnoreRange(appBootloadStateRange);

            // Uncoalesce all records so that they fit in a named MEMORY region
            for(int i = 0; i < bl2.records.size(); i++)
            {
                DataRecord d = bl2.records.get(i);

                // Map each bl2 range to a edc:ProgramSpace definition, splitting the bl2 section if needed, so that linking is possible
                for(PairWithText p : state.devProgramSpaces)
                {
                    if((d.address >= p.first) && (d.address < p.second))
                    {   // Found a matching ProgramSpace region that this section belongs in
                        if(d.GetEndAddress() > p.second)
                        {
                            // Needs a split
                            DataRecord splitRecord = d.SplitAtAddress(p.second);
                            bl2.records.add(i + 1, splitRecord);
                        }
                        d.assignedMemory = p.text;
                        break;
                    }
                    else if((d.GetEndAddress() > p.first) && (d.address <= p.first))
                    {// Found opposite case where a region appears to be inside or overlap the data
                        // Needs a split
                        DataRecord splitRecord = d.SplitAtAddress(p.first);
                        bl2.records.add(i + 1, splitRecord);
                    }
                }

                // Throw away regions that don't have GLD memory defined for
                // them, assuming they have only 0xFFFFFF data in them (we
                // likely generated these in the first place as sector keepout).
                if(d.assignedMemory.isEmpty())
                {
                    byte sum = (byte)0xFF;
                    for(byte dByte : d.data)
                    {
                        sum &= dByte;
                    }
                    if(sum == (byte)0xFF)
                    {
                        bl2.records.remove(d);
                        i--;    // Removed an element, don't increment loop counter this time
                        continue;
                    }
                    else
                    {
                        // For device program spaces that might not be named (like FBOOT in EDC data),
                        // choose the default "program" destination, which may be slightly wrong, but
                        // at least it will not trigger a GLD syntax error and is still likely to
                        // ultimately reside in the correct program memory space anyway.
                        if(!state.silent)
                        {
                            System.out.printf("ezbl_tools: data at [0x%1$06X, 0x%2$06X) being mapped to .gld MEMORY region \"program\"\n", d.address, d.GetEndAddress());
                        }
                        d.assignedMemory = "program";
                    }
                }
            }

            // Find all global symbols in each ROM or RAM section (no debugging sections)
            if(state.elfDump != null)
            {
                // Collect absolute _EZBL_* symbols for application use. Ex: _EZBL_FORWARD_ISR_* definitions.
                for(Symbol s : state.elfDump.symbols)
                {
                    if(s.name.compareTo("_EZBL_SYMBOL_EXPORT_LEVEL") == 0)
                    {
                        state.symbolExportLevel = (int)s.address;
                    }
                    else if(s.name.compareTo("_EZBL_LEGACY_BOOT_STYLE") == 0)
                    {
                        state.legacyClassic = (s.address != 0);
                    }

                    if(s.flags.local || !s.name.startsWith("_EZBL") || s.section.equalsIgnoreCase("*UND*"))
                    {
                        continue;
                    }
                    otherSymbols.add(s);
                }

                // Collect RAM and ROM symbols by section
                for(Section sect : state.elfDump.sections)
                {
                    // Only spend time processing RAM and ROM sections (no debugging)
                    if(!sect.isRAM && !sect.isROM)
                    {
                        continue;
                    }

                    // Generate a list of rom noload/NEVER_LOAD ranges typically used for data EEPROM emulation data
                    if(sect.isROM && sect.flags.NEVER_LOAD && sect.flags.ALLOC)
                    {
                        // Find matching named MEMORY region
                        for(PairWithText progSpace : state.devProgramSpaces)
                        {
                            if((sect.loadMemoryAddress >= progSpace.first) && (sect.loadMemoryAddress < progSpace.second))
                            {   // Found a matching ProgramSpace region that this section belongs in
                                sect.combinedName = progSpace.text;
                                break;
                            }
                        }
                        romNoLoadSections.add(sect);
                    }

                    sect.LoadSymbols(state.elfDump.symbols);
                    for(int i = 0; i < sect.symbols.size(); i++)
                    {
                        Symbol sym = sect.symbols.get(i);

                        if(sym.flags.local) // Only collect non-local symbols
                        {
                            continue;
                        }
                        if(!sym.name.matches("[_a-zA-Z][_a-zA-Z0-9]*$"))    // Only collect symbols if they are legal function/variable names (i.e. get rid of things like "__FSEC.sec", which has a dot in it and can't be generated normally)
                            continue;

                        if(sect.isRAM && sym.name.equals("_EZBL_ForwardBootloaderISR"))
                        {
                            EZBL_ForwardBootloaderISRAddr = sym.address;
                        }

                        // Filter out a few compiler/linker generated symbols 
                        // that don't have value to the Application and could 
                        // lead to ambiguity of usage in the App.
                        if(sym.name.startsWith("__") && !(sym.name.startsWith("___")))
                        {
                            if(sym.name.equals("__Size_block")
                               || sym.name.equals("__Aldata")
                               || sym.name.equals("__Aldata")
                               || sym.name.equals("__reset")
                               || sym.name.equals("__resetPRI")
                               || sym.name.equals("__psv_init")
                               || sym.name.equals("__crt_start_mode"))
                            {
                                continue;
                            }
                        }

                        // Place symbol into correct bin
                        if(sect.isRAM)
                        {
                            ramSymbols.add(sym);
                        }
                        else if(sect.isROM)
                        {
                            romSymbols.add(sym);
                        }
                    }
                }

                // Filter out any duplicit symbols already captured as a RAM or ROM symbol
                for(int i = 0; i < otherSymbols.size(); i++)
                {
                    Symbol s = otherSymbols.get(i);
                    for(Symbol sym : ramSymbols)
                    {
                        if(s.equals(sym))
                        {
                            otherSymbols.remove(s);
                            s = null;
                            i--;
                            break;
                        }
                    }
                    if(s == null)
                    {
                        continue;
                    }
                    for(Symbol sym : romSymbols)
                    {
                        if(s.equals(sym))
                        {
                            otherSymbols.remove(s);
                            i--;
                            break;
                        }
                    }
                }

                // Sort by Symbol name for easy display
                Collections.sort(otherSymbols, new com.microchip.apps.ezbl.SymbolNameComparator());
                Collections.sort(ramSymbols, new com.microchip.apps.ezbl.SymbolNameComparator());
                Collections.sort(romSymbols, new com.microchip.apps.ezbl.SymbolNameComparator());
            }

            // Build bootloader code/constants as an absolute assembly file
            asmMerge.add("/**"
                         + "\n * EZBL Bootloader Code and RAM Allocation"
                         + "\n *"
                         + "\n * Automatically generated file - not intended for manual editing. If changes "
                         + "\n * are made here, they will normally be overwritten when you rebuild your "
                         + "\n * Bootloader. If necessary, maintain a backup copy and manually merge your "
                         + "\n * customizations back in."
                         + "\n * "
                         + "\n * Built for:"
                         + "\n *     " + state.fullPartNumber
                         + "\n * From:"
                         + "\n *     " + state.artifactPath
                         + "\n * Using build configuration:"
                         + "\n *     " + state.conf
                         + "\n */"
                         + "\n"
                         + "\n ; Validate matching target processors between Bootloader and Application projects."
                         + "\n ; If you get this error and wish to force the two dissimilar targets together anyway,"
                         + "\n ; you can comment out these .error statements. However, something will likely be"
                         + "\n ; broken at run time, so do so only if you are sure of what you are doing."
                         + (state.MCU16Mode
                            ? "\n    .ifndef __" + state.partNumber
                              + "\n    .error \"Bootloader's " + state.projectName + ".merge.S/.gld files were generated for a different target processor.\""
                              + "\n    .error \"Recompile this Application project or the " + state.projectName + " Bootloader project for the same hardware.\""
                              + "\n    .endif"
                            : "\n#if !defined(__" + state.partNumber + "__)"
                              + "\n    .error \"Bootloader's " + state.projectName + ".merge.S file was generated for a different target processor.\""
                              + "\n    .error \"Recompile this Application project or the " + state.projectName + " Bootloader project for the same hardware.\""
                              + "\n#endif")
                         + "\n"
                         + "\n"
            );

            if(elfData != null && !state.MCU16Mode) // Generate PIC32 EZBL_BOOTLOADER_SIZE and EZBL_appBootloadState struct
            {
                Symbol bootSize = Symbol.GetSymbolByName(elfData.symbols, "EZBL_BOOTLOADER_SIZE");
                if(bootSize != null)
                {
                    asmMerge.add(String.format("\n;----Bootloader reserved flash space propagated from the bootloader project----"
                                               + "\n    .global EZBL_BOOTLOADER_SIZE"
                                               + "\nEZBL_BOOTLOADER_SIZE = 0x%1$08X"
                                               + "\n"
                                               + "\n", bootSize.address));
                }

                Symbol appBootloadState = Symbol.GetSymbolByName(elfData.symbols, "EZBL_appBootloadState");

                if(appBootloadState != null)
                {
                    asmMerge.add(String.format("\n\n;----App erasable items that the Bootloader knows about----"
                                               + "\n    .pushsection    .EZBL_appBootloadState, address(0x%1$08X), code, keep"
                                               + "\n    ; EZBL_appBootloadState - Base address of EZBL_INSTALLED_FLAGS structure (24 bytes/3 instruction words)"
                                               + "\n    .global EZBL_appBootloadState"
                                               + "\n    .global EZBL_appIDVer"
                                               + "\n    .type   EZBL_appBootloadState, @object"
                                               + "\n    .type   EZBL_appIDVer, @object"
                                               + "\nEZBL_appBootloadState:"
                                               + "\nEZBL_appIDVer:"
                                               + "\n    .long   _APPID_VER_BUILD    ; EZBL_appIDVer.appIDVerBuild"
                                               + "\n    .short  _APPID_VER_MINOR    ; EZBL_appIDVer.appIDVerMinor"
                                               + "\n    .short  _APPID_VER_MAJOR    ; EZBL_appIDVer.appIDVerMajor"
                                               + "\n    .size   EZBL_appIDVer, . - EZBL_appIDVer"
                                               + "\n    .long   0xFFFFFFFF          ; hash32 - not defined at compile time, set when run-time programmed"
                                               + "\n    .long   0xFFFFFFFF          ; CRC32  - not defined at compile time, set when run-time programmed"
                                               + "\n    .long   0xFFFF12CD          ; appInstalled == EZBL_APP_INSTALLED (0x12CD)"
                                               + "\n    .long   0xFFFFFFFF          ; padding to fit 3x flash double words"
                                               + "\n    .size   EZBL_appBootloadState, . - EZBL_appBootloadState"
                                               + "\n    .popsection"
                                               + "\n", appBootloadState.address
                    ));
                }
            }

            asmMerge.add(String.format("\n;----Target Bootloader ID and this Application Version meta data for .bl2 file header----"
                                       + "\n    .pushsection    .info.EZBL_metaParameters, info, keep"
                                       + "\n    .weak   _BOOTID_HASH0"
                                       + "\n    .weak   _BOOTID_HASH1"
                                       + "\n    .weak   _BOOTID_HASH2"
                                       + "\n    .weak   _BOOTID_HASH3"
                                       + "\n    .weak   _APPID_VER_BUILD"
                                       + "\n    .weak   _APPID_VER_MINOR"
                                       + "\n    .weak   _APPID_VER_MAJOR"
                                       + "\n_BOOTID_HASH0 = 0x%1$08X"
                                       + "\n_BOOTID_HASH1 = 0x%2$08X"
                                       + "\n_BOOTID_HASH2 = 0x%3$08X"
                                       + "\n_BOOTID_HASH3 = 0x%4$08X"
                                       + "\nEZBL_metaAppIDVerBuild:"
                                       + "\n    .long   _APPID_VER_BUILD"
                                       + "\nEZBL_metaAppIDVerMinor:"
                                       + "\n    .short  _APPID_VER_MINOR"
                                       + "\nEZBL_metaAppIDVerMajor:"
                                       + "\n    .short  _APPID_VER_MAJOR"
                                       + "\n    .popsection"
                                       + "\n", bl2.bootIDHash[0], bl2.bootIDHash[1], bl2.bootIDHash[2], bl2.bootIDHash[3]));

            if(state.MCU16Mode)
            {
                // Reserve all the needed RAM ranges
                gldPairs.add(new PairWithText(EZBL_ForwardBootloaderISRAddr, EZBL_ForwardBootloaderISRAddr + 0x4,
                                              String.format("\n#if defined(EZBL_HIDE_BOOT_SYMBOLS)"
                                                            + "\n  EZBL_RAM_AT_0x%1$04X 0x%1$04X :"
                                                            + "\n  {"
                                                            + "\n    *(EZBL_RAM_AT_0x%1$04X); /* [0x%1$04X, 0x%3$04X), contains %2$d bytes */"
                                                            + "\n  } >data"
                                                            + "\n#endif"
                                                            + "\n", EZBL_ForwardBootloaderISRAddr, 0x4, EZBL_ForwardBootloaderISRAddr + 0x4)));

                asmMerge.add("\n;----Bootloader reserved static RAM----"
                             + "\n#if defined(EZBL_HIDE_BOOT_SYMBOLS)"
                             + String.format("\n    ; Bootloader RAM block intended for data region"
                                             + "\n    ; 0x%1$04X to 0x%2$04X, length 0x0004 (4 bytes)"
                                             + "\n    .pushsection    EZBL_RAM_AT_0x%1$04X, address(0x%1$04X), persist, keep", EZBL_ForwardBootloaderISRAddr, EZBL_ForwardBootloaderISRAddr + 4)
                             + "\n    .global _EZBL_ForwardBootloaderISR"
                             + "\n    .type   _EZBL_ForwardBootloaderISR, @object"
                             + "\n_EZBL_ForwardBootloaderISR: ; This variable cannot be hidden since ISR dispatch code uses it"
                             + "\n    .space  0x4"
                             + "\n    .size   _EZBL_ForwardBootloaderISR, . - _EZBL_ForwardBootloaderISR"
                             + "\n    .popsection"
                             + "\n#else");
                for(AddressRange range : state.ramUseRanges)
                {
                    asmMerge.add(String.format("\n    ; Bootloader RAM block intended for data region"
                                               + "\n    ; 0x%1$04X to 0x%2$04X, length 0x%3$04X (%3$d bytes)"
                                               + "\n    .pushsection    EZBL_RAM_AT_0x%1$04X, address(0x%1$04X), persist, keep",
                                               range.startAddress, range.endAddress, range.endAddress - range.startAddress));

                    gldPairs.add(new PairWithText(range.startAddress, range.endAddress,
                                                  String.format("\n#if !defined(EZBL_HIDE_BOOT_SYMBOLS)"
                                                                + "\n  EZBL_RAM_AT_0x%1$04X 0x%1$04X :"
                                                                + "\n  {"
                                                                + "\n    *(EZBL_RAM_AT_0x%1$04X); /* [0x%1$04X, 0x%3$04X), contains %2$d bytes */"
                                                                + "\n  } >data"
                                                                + "\n#endif"
                                                                + "\n", range.startAddress, range.endAddress - range.startAddress, range.endAddress)));

                    // Print the exported symbols that are in this address range
                    for(Symbol s : ramSymbols)
                    {
                        if((s.address >= range.startAddress) && (s.address < range.endAddress))
                        {
                            symbolTable.add(String.format("\n    .equ        %1$s, 0x%2$06X", s.name, s.address));
                            if(s.name.equals("_EZBL_ForwardBootloaderISR"))    // Do not weaken this always present/required symbol
                            {
                                exportTable.add(String.format("\n    .global     %1$s", s.name));
                            }
                            else
                            {
                                exportTable.add(String.format("\n    .weak       %1$s", s.name));
                            }
                            if(s.flags.function)
                            {
                                exportTable.add(String.format("\n    .type       %1$s, @function", s.name));
                            }
                            else if(s.flags.object)
                            {
                                exportTable.add(String.format("\n    .type       %1$s, @object", s.name));
                            }
                        }
                    }

                    asmMerge.add(String.format("\n    .space      0x%1$04X ; (%1$d bytes)\n", range.endAddress - range.startAddress));
                    asmMerge.add("    .popsection");
                }
                asmMerge.add("\n#endif");

                // Finally, locate the appBootloadState, hash and user flags, app reset vector, and leave space for IGT (created by linker script)
                asmMerge.add("\n\n;----App erasable items that the Bootloader knows about----");
                asmMerge.add(String.format(""
                                           + "\n    .pushsection    EZBL_AppErasable, address(0x%1$06X), code, keep"
                                           + "\n    ; EZBL_appBootloadState - Base address of EZBL_INSTALLED_FLAGS structure (18 bytes/6 instruction words)"
                                           + "\n    .pword      0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x12CDFF"
                                           + "\n"
                                           + "\n    ; _EZBL_APP_RESET_BASE"
                                           + "\n    .extern     __reset"
                                           + "\n    goto        __reset"
                                           + "\n"
                                           + "\n    ; Interrupt Goto Table follows for 0x%2$X addresses (0x%3$06X to 0x%4$06X)"
                                           + "\n    ; The .igt section is defined in the linker script since this is an application"
                                           + "\n    ; erasable/updatable structure.",
                                           state.baseAddressOfAppErasable, state.sizeOfIGT, state.baseAddressOfIGT, state.baseAddressOfIGT + state.sizeOfIGT
                ));
                asmMerge.add("\n    .popsection");

                gldPairs.add(new PairWithText(state.baseAddressOfAppErasable, state.baseAddressOfIGT,
                                              String.format("\n  EZBL_AppErasable 0x%1$06X :"
                                                            + "\n  {"
                                                            + "\n    *(EZBL_AppErasable); /* [0x%1$06X, 0x%3$06X), contains %2$d bytes */"
                                                            + "\n  } >program"
                                                            + "\n", state.baseAddressOfAppErasable, state.sizeOfAppErasable / 2 * 3, state.baseAddressOfAppBootloadState + state.sizeOfAppErasable)));

                // Add .igt section to our list of GLD section mappings
                if((state.erasableIGT != null) && (!state.erasableIGT.isEmpty()))
                {
                    gldPairs.add(new PairWithText(state.baseAddressOfIGT, state.baseAddressOfIGT + state.sizeOfIGT,
                                                  String.format(
                                                          "\n  /* Application's erasable Interrupt Goto Table */"
                                                          + "\n  .igt 0x%1$06X :"
                                                          + "\n  { /* [0x%1$06X, 0x%3$06X), contains %4$d bytes */"
                                                          + "%2$s"
                                                          + "\n  } >program"
                                                          + "\n", state.baseAddressOfIGT, state.erasableIGT, state.baseAddressOfIGT + state.sizeOfIGT, state.sizeOfIGT / 2 * 3)));
                }

                // Find the bootloader's .const section, assuming one exists
                Section constSect = null;
                long constPageStartAddress = 0;
                long constPageEndAddress = constPageStartAddress + 0x8000L;
                if(constPageEndAddress > state.mainExecutionRange.endAddress)
                    constPageEndAddress = state.mainExecutionRange.endAddress;
                ArrayList<AddressRange> usedSpaces = new ArrayList<AddressRange>();

                if(state.elfDump != null)   // Will be null for PIC32MM right now
                {
                    for(Section c : state.elfDump.sections)
                    {
                        if((c.name.compareTo(".const") == 0) && c.isROM)
                        {
                            constPageStartAddress = c.loadMemoryAddress & ~0x7FFFL;
                            constPageEndAddress = constPageStartAddress + 0x8000L;
                            if(constPageEndAddress > state.mainExecutionRange.endAddress)
                                constPageEndAddress = state.mainExecutionRange.endAddress;
                            constSect = c;
                            break;
                        }
                    }
                }

                // If bootloader had a .const section, we need to explicitly
                // generate a .const section consumer in the linker script for the
                // application that will match the same PSV page that the bootloader
                // used so const pointers can be passed between each other during
                // function calls. Otherwise, bad run-time bugs would happen due 
                // to PSVPAG/DSRPAG mismatch.
                if(constSect != null)
                {
                    // First search the bootloader .bl2 data so we know what
                    // addresses are already occupied on the PSV page. We ultimately
                    // need to find the biggest free block on the same PSV page 
                    // to put the application .const section.
                    for(int i = 0; i <= bl2.records.size(); i++)
                    {
                        AddressRange ar = new AddressRange();
                        ar.startAddress = state.baseAddressOfAppErasable;
                        ar.endAddress = state.baseAddressOfIGT + state.sizeOfIGT;
                        if(i < bl2.records.size())
                        {
                            DataRecord d = bl2.records.get(i);
                            ar.startAddress = d.address;
                            ar.endAddress = d.GetEndAddress();
                        }
                        if((ar.endAddress > constPageStartAddress) && (ar.startAddress < constPageEndAddress))
                        {
                            if(ar.startAddress < constPageStartAddress)
                            {
                                ar.startAddress = constPageStartAddress;
                            }
                            if(ar.endAddress > constPageEndAddress)
                            {
                                ar.endAddress = constPageEndAddress;
                            }
                            usedSpaces.add(ar);
                        }
                    }

                    // Now locate biggest free space on the 32KB/32K address PSV page
                    AddressRange.SortAndReduce(usedSpaces);
                    long constPageBiggestHoleSize = 0;
                    long constPageBiggestHoleStartAddress = constPageStartAddress;
                    long lastHoleStartAddress = constPageStartAddress;
                    for(int i = 0; i < usedSpaces.size() + 1; i++)
                    {
                        AddressRange used;
                        long thisHoleSize;

                        if(i >= usedSpaces.size())
                        {
                            used = new AddressRange(constPageEndAddress, constPageEndAddress);
                        }
                        else
                        {
                            used = usedSpaces.get(i);
                        }

                        thisHoleSize = used.startAddress - lastHoleStartAddress;
                        if(thisHoleSize > constPageBiggestHoleSize)
                        {
                            constPageBiggestHoleSize = thisHoleSize;
                            constPageBiggestHoleStartAddress = lastHoleStartAddress;
                        }
                        lastHoleStartAddress = used.endAddress;
                    }

                    PairWithText pt;
                    pt = new PairWithText(constPageBiggestHoleStartAddress, constPageBiggestHoleStartAddress + constPageBiggestHoleSize,
                                          String.format("\n#if !defined(EZBL_HIDE_BOOT_SYMBOLS)"
                                                        + "\n  /*"
                                                        + "\n  ** .const compatibility with Bootloader .const section"
                                                        + "\n  ** "
                                                        + "\n  ** Explicitly locate the .const section so the largest possible PSV "
                                                        + "\n  ** window is available for the Application project without using a "
                                                        + "\n  ** different PSVPAG/DSRPAG value than the Bootloader project requires "
                                                        + "\n  ** for API compatibility. "
                                                        + "\n  */"
                                                        + "\n  .const 0x%1$06X : AT(0x%2$06X)"
                                                        + "\n  {"
                                                        + "\n    LONG(0xFFFFFF);/* Dummy word to avoid empty .const section and prevent linker de-initializing CORCON<PSV>/DSRPAG when App starts */"
                                                        + "\n    *(.const);     /* 0x%3$06X (%3$d) available addresses/bytes for auto PSV constants */"
                                                        + "\n  } >program"
                                                        + "\n#endif"
                                                        + "\n", 0x8000 | (constPageBiggestHoleStartAddress & 0x7FFF), constPageBiggestHoleStartAddress, constPageBiggestHoleSize
                                          ));
                    if(state.legacyClassic)
                    {
                        pt.text = "\n#if 0    /* Block removed due to use of _EZBL_LEGACY_BOOT_STYLE */" + pt.text.replace("\n  ", "\n    ") + "#endif\n";
                    }
                    gldPairs.add(pt);
                    if(!state.legacyClassic && !state.buildAsOneMode && ((constPageBiggestHoleSize < state.minFreePSVSpace) || (constPageBiggestHoleSize < state.warnFreePSVSpace)))
                    {
                        if(!state.silent)
                        {
                            long spaceNeeded = Math.max(Math.max(state.warnFreePSVSpace, state.minFreePSVSpace), state.eraseBlockSizeAddresses);
                            long alignedAppReservedEnd = constSect.loadMemoryAddress + (0x8000 - (constSect.loadMemoryAddress % 0x8000));
                            long alignedAppReservedStart = constSect.loadMemoryAddress + constSect.size;
                            if((alignedAppReservedStart % state.eraseBlockSizeAddresses) != 0)
                                alignedAppReservedStart += state.eraseBlockSizeAddresses - (alignedAppReservedStart % state.eraseBlockSizeAddresses);
                            for(PairWithText p : state.devProgramSpaces)
                            {
                                if((constSect.loadMemoryAddress >= p.first) && (constSect.loadMemoryAddress + constSect.size <= p.second) && (alignedAppReservedEnd > p.second))
                                {
                                    alignedAppReservedEnd = p.second - 0x2;
                                    break;
                                }
                            }
                            long spaceMakable = alignedAppReservedEnd - alignedAppReservedStart;
                            while(spaceMakable > spaceNeeded + 0x1000)
                            {
                                alignedAppReservedStart += state.eraseBlockSizeAddresses;
                                spaceMakable = alignedAppReservedEnd - alignedAppReservedStart;
                            }

                            if(spaceMakable < spaceNeeded)
                            {   // Couldn't fullfil minimum requirements. See if we can if we start from the beginning of the PSV page, before the .const section
                                long alignedAppReservedEnd2 = constSect.loadMemoryAddress & ~state.eraseBlockSizeAddresses;
                                long alignedAppReservedStart2 = constSect.loadMemoryAddress & ~0x8000L;
                                if(alignedAppReservedStart2 < state.eraseBlockSizeAddresses)
                                    alignedAppReservedStart2 = state.eraseBlockSizeAddresses;
                                long spaceMakable2 = alignedAppReservedEnd2 - alignedAppReservedStart2;
                                if(spaceMakable2 > spaceMakable)
                                {
                                    while(spaceMakable2 > spaceNeeded + 0x1000)
                                    {
                                        alignedAppReservedEnd2 -= state.eraseBlockSizeAddresses;
                                        spaceMakable2 = alignedAppReservedEnd2 - alignedAppReservedStart2;
                                        if(spaceMakable2 > spaceNeeded + 0x1000)
                                        {
                                            alignedAppReservedStart2 += state.eraseBlockSizeAddresses;
                                            spaceMakable2 = alignedAppReservedEnd2 - alignedAppReservedStart2;
                                        }
                                    }
                                    alignedAppReservedStart = alignedAppReservedStart2;
                                    alignedAppReservedEnd = alignedAppReservedEnd2;
                                    spaceMakable = spaceMakable2;
                                }
                            }

                            if(spaceMakable < spaceNeeded)
                            {   // Can't fit either before or after the .const section, try to come up with something that straddles two PSV regions and occupies the .const region to force the linker to move .const onto the next PSV page
                                alignedAppReservedEnd = constSect.loadMemoryAddress + (0x8000 - (constSect.loadMemoryAddress % 0x8000)) + 0x8000;
                                alignedAppReservedStart = (constSect.loadMemoryAddress + constSect.size - 0x2);
                                alignedAppReservedStart -= alignedAppReservedStart % state.eraseBlockSizeAddresses;
                                while(alignedAppReservedEnd - alignedAppReservedStart > spaceNeeded + 0x1000)
                                {
                                    alignedAppReservedEnd -= state.eraseBlockSizeAddresses;
                                }
                                for(PairWithText p : state.devProgramSpaces)
                                {
                                    if((constSect.loadMemoryAddress >= p.first) && (constSect.loadMemoryAddress + constSect.size <= p.second) && (alignedAppReservedEnd > p.second))
                                    {
                                        alignedAppReservedEnd = p.second - 0x2;
                                        break;
                                    }
                                }
                                spaceMakable = alignedAppReservedEnd - alignedAppReservedStart;
                            }

                            if(spaceMakable < spaceNeeded)
                            {   // Still can't find a solution. A solution may not exist, so just generate the min required. Device has too little flash or too many constants in the Bootloader to accomodate the check boundary.
                                alignedAppReservedStart = state.eraseBlockSizeAddresses;
                                alignedAppReservedEnd = alignedAppReservedStart + ((spaceNeeded % state.eraseBlockSizeAddresses == 0) ? spaceNeeded : spaceNeeded + (state.eraseBlockSizeAddresses - (spaceNeeded % state.eraseBlockSizeAddresses)));
                                for(PairWithText p : state.devProgramSpaces)
                                {
                                    if((constSect.loadMemoryAddress >= p.first) && (constSect.loadMemoryAddress + constSect.size <= p.second) && (alignedAppReservedEnd > p.second))
                                    {
                                        alignedAppReservedEnd = p.second - 0x2;
                                        break;
                                    }
                                }
                            }

                            System.err.print("EZBL check " + ((constPageBiggestHoleSize < state.minFreePSVSpace) ? "error:\n" : "warning:\n")
                                             + Multifunction.FormatHelpText(
                                            String.format(".const found at 0x%1$06X; %2$d bytes free on PSV/EDS page\n"
                                                          + "\n"
                                                          + "If calling Bootloader APIs from an Application project, all constants must reside on the same PSV/EDS page. To avoid running out of PSV addresses in future Application projects, "
                                                          + "recompile this Bootloader project with space explicitly reserved for the Application using the EZBL_SetAppReservedHole() macro, ex:"
                                                          + "\n    EZBL_SetAppReservedHole(0x%3$06X, 0x%4$06X);"
                                                          + "\nTo change the checking thresholds for this diagnostic, specify the -min_free_psv=x and/or -warn_free_psv=y command line options.",
                                                          constSect.loadMemoryAddress, constPageBiggestHoleSize, alignedAppReservedStart, alignedAppReservedEnd), 110, 4, 0, false));
                            System.err.flush(); // Flush text now since we are going to cause a failure and don't want subsequent Make errors to print between our lines.
                        }
                        if(constPageBiggestHoleSize < state.minFreePSVSpace)
                        {
                            return null;
                        }
                    }
                }

                if(!romNoLoadSections.isEmpty())
                {
                    asmMerge.add("\n\n\n;----Bootloader defined noload holes in program memory----");
                    for(Section sec : romNoLoadSections)
                    {
                        long byteLen = sec.size;
                        if(state.MCU16Mode)
                        {
                            byteLen = byteLen / 2 * 3;
                        }
                        asmMerge.add(String.format("\n"
                                                   + "\n    ; Bootloader noload attributed hole intended for program region '%5$s'"
                                                   + "\n    ; 0x%1$06X to 0x%2$06X, length 0x%3$06X (%4$d bytes; needs %6$d pages)"
                                                   + "\n    .pushsection    %7$s, address(0x%1$06X), code, keep, noload"
                                                   + "\n    .skip 0x%3$06X"
                                                   + "\n    .popsection",
                                                   sec.loadMemoryAddress, sec.loadMemoryAddress + sec.size, sec.size, byteLen, "program", byteLen / state.eraseBlockSizeBytes, sec.name));
                        gldPairs.add(new PairWithText(sec.loadMemoryAddress, sec.loadMemoryAddress + sec.size,
                                                      String.format("\n  %1$s 0x%2$06X (NOLOAD) :"
                                                                    + "\n  {"
                                                                    + "\n    *(%1$s); /* [0x%2$06X, 0x%3$06X), occupies %4$d bytes */"
                                                                    + "\n  } >%5$s"
                                                                    + "\n", sec.name, sec.loadMemoryAddress, sec.loadMemoryAddress + sec.size, byteLen, "program")));
                        // Print the exported symbols that are in this address range
                        for(Symbol s : sec.symbols)
                        {
                            if(s.flags.local)
                            {
                                continue;
                            }
                            symbolTable.add(String.format("\n    .equ        %1$s, 0x%2$06X", s.name, s.address));
                            exportTable.add(String.format("\n    .weak       %1$s", s.name));
                            if(s.flags.function)
                            {
                                exportTable.add(String.format("\n    .type       %1$s, @function", s.name));
                            }
                            else if(s.flags.object)
                            {
                                exportTable.add(String.format("\n    .type       %1$s, @object", s.name));
                            }
                        }
                    }
                }

                asmMerge.add("\n\n\n;----Bootloader reserved program memory----");
                for(DataRecord d : bl2.records)
                {
                    int data;
                    int i;
                    int outCounter = 0;

                    asmMerge.add(String.format("\n"
                                               + "\n    ; Bootloader code block intended for program region '%5$s'"
                                               + "\n    ; 0x%1$06X to 0x%2$06X, length 0x%3$06X (%4$d bytes; needs %6$d pages)"
                                               + "\n    .pushsection    EZBL_ROM_AT_0x%1$06X, address(0x%1$06X), code, keep",
                                               d.address, d.GetEndAddress(), d.GetEndAddress() - d.address, d.data.length, d.assignedMemory, d.data.length / state.eraseBlockSizeBytes));

                    gldPairs.add(new PairWithText(d.address, d.GetEndAddress(),
                                                  String.format("\n  EZBL_ROM_AT_0x%1$06X 0x%1$06X :"
                                                                + "\n  {"
                                                                + "\n    *(EZBL_ROM_AT_0x%1$06X); /* [0x%1$06X, 0x%3$06X), contains %2$d bytes */"
                                                                + "\n  } >%4$s"
                                                                + "\n", d.address, d.data.length, d.GetEndAddress(), d.assignedMemory)));

                    // Print the exported symbols that are in this address range
                    for(Symbol s : romSymbols)
                    {
                        if((s.address >= d.address) && (s.address < d.GetEndAddress()))
                        {
                            symbolTable.add(String.format("\n    .equ        %1$s, 0x%2$06X", s.name, s.address));
                            exportTable.add(String.format("\n    .weak       %1$s", s.name));
                            if(s.flags.function)
                            {
                                exportTable.add(String.format("\n    .type       %1$s, @function", s.name));
                            }
                            else if(s.flags.object)
                            {
                                exportTable.add(String.format("\n    .type       %1$s, @object", s.name));
                            }
                        }
                    }

                    // Print the code and constants in this range
                    long dAddr = d.address;
                    for(i = 0; i < d.data.length - (d.data.length % 3); i += 3)
                    {
                        data = (d.data[i + 0] & 0xFF);
                        data |= (d.data[i + 1] & 0xFF) << 8;
                        data |= (d.data[i + 2] & 0xFF) << 16;
                        if(outCounter++ % 8 == 0)
                        {
                            if(dAddr % state.eraseBlockSizeAddresses == 0)
                            {
                                asmMerge.add(String.format("\n    ; 0x%06X", dAddr));
                            }
                            asmMerge.add(String.format("\n    .pword      0x%1$06X", data));
                        }
                        else
                        {
                            asmMerge.add(String.format(", 0x%1$06X", data));
                        }
                        dAddr += 0x2;
                    }
                    while(i != d.data.length)
                    {
                        if(outCounter++ % 8 == 0)
                        {
                            asmMerge.add(String.format("\n    .pbyte      0x%1$02X", d.data[i++] & 0xFF));
                        }
                        else
                        {
                            asmMerge.add(String.format(", 0x%1$02X", d.data[i++] & 0xFF));
                        }
                    }
                    asmMerge.add("\n    .popsection");
                }

                // Print other absolute and EZBL symbols
                for(Symbol s : otherSymbols)
                {
                    symbolTable.add(String.format("\n    .equ        %1$s, 0x%2$06X", s.name, s.address));
                    exportTable.add(String.format("\n    .weak       %1$s", s.name));
                    if(s.flags.function)
                    {
                        exportTable.add(String.format("\n    .type       %1$s, @function", s.name));
                    }
                    else if(s.flags.object)
                    {
                        exportTable.add(String.format("\n    .type       %1$s, @object", s.name));
                    }
                }

                asmMerge.add("\n\n\n#if !defined(EZBL_HIDE_BOOT_SYMBOLS)");
                if(!exportTable.isEmpty())  // Print the export tables
                {
                    asmMerge.add("\n\n;----Bootloader symbol export table----"
                                 + CatStringList(exportTable)
                                 + "\n");
                }
                if(!symbolTable.isEmpty())  // Print the symbol table
                {
                    asmMerge.add("\n\n\n;----Bootloader symbol addresses----"
                                 + CatStringList(symbolTable)
                                 + "\n");
                }
                asmMerge.add("\n#endif");
                asmMerge.add("\n");

                // Read the given device .gld contents into the deviceGLD TextBlock class
                gldContents = state.compilerGLDContents;
                if(gldContents == null)
                {
                    gldContents = Multifunction.ReadFile(state.compilerLinkScriptPath, true);
                    if(gldContents == null)
                    {
                        if(!state.silent)
                        {
                            System.err.println("ezbl_tools: Unable to read \"" + state.compilerLinkScriptPath + "\"");
                        }
                        return null;
                    }
                }
                // Convert .gld intput byte stream into a parsable TextBlock
                TextBlock outGLD = new TextBlock(gldContents);
                String dynamicSectionsHeader = "", dynamicSections = "", dynamicSectionsFooter = "";

                // Search through all SECTIONS regions for things that need to be
                // deleted and to find the correct place to add things
                while(outGLD.Find("SECTIONS[^{]*", "\\{", "\\}", "\n"))
                {
                    if(dynamicSectionsHeader.isEmpty())
                    {
                        dynamicSectionsHeader = outGLD.GetOuterLeft(false);
                        dynamicSectionsFooter = outGLD.GetOuterRight(false);
                    }
                    TextBlock sectionBlock = new TextBlock(outGLD.GetInner(true));

                    // Find and delete .reset section. We will be using our own. This is
                    // also a good place to insert all our bootloader sections because
                    // it is the first thing the compiler'sec default linker scripts place
                    // (it is address 0x000000 afterall). Included here are
                    // EZBL_AppErasable, EZBL_ROM_AT_0xXXXXXX, .igt, and possibly other
                    // blocks.
                    if(sectionBlock.Find("\\s[.]reset[^:]*:[^{]*", "\\{", "\\}", "\n"))
                    {
                        // Sort all GLD section mappings by address for nice logical
                        // appearance in the .gld output file and output as a single
                        // string.
                        Collections.sort(gldPairs);
                        gldSectionAllocation = "";
                        for(PairWithText p : gldPairs)
                        {
                            // Explicitly map all Bootloader sections before any
                            // Application sections are placed, except for Config
                            // Words/Bytes. The Config data will go after the
                            // Application'sec Config data is placed. We explicitly map
                            // these so the linker'sec best fit allocator can see these
                            // and not try and place data on top of a Bootloader region.
                            if((state.configWordsRange == null) || (!((p.first >= state.configWordsRange.startAddress) && (p.first < state.configWordsRange.endAddress))))
                            {
                                gldSectionAllocation += p.text;
                            }
                        }
                        sectionBlock.ReplaceOuter(gldSectionAllocation + "\n");
                    }

                    // Find and vaporize .ivt section since we added our own .igt section at the beginning where the .reset section used to be
                    if(sectionBlock.Find("\\s[.]ivt[^:]*:[^{]*", "\\{", "\\}", "\n"))
                    {
                        sectionBlock.ReplaceOuter("\n");
                    }

                    // If this device has a .aivt section, delete it. We aren't
                    // supporting it right now since we have our own alternate vector
                    // remapping method that provides individual ISR mapping granularity
                    // and and ability to run-time toggle ownership back and fourth
                    // between the App and Bootloader.
                    if(sectionBlock.Find("\\s[.]aivt[^:]*:[^{]*", "\\{", "\\}", "\n"))
                    {
                        sectionBlock.ReplaceOuter("\n");
                    }

                    dynamicSections += sectionBlock.GetFullBlock();
                    outGLD.DeleteOuter();
                }

                // Remove junk "/*\n** Interrupt Vector Table\n*/" comment from compiler .gld file if it is sitting around. We have our own IVT in the Bootloader and an IGT for this Application.
                dynamicSections = dynamicSections.replace("/*\n** Interrupt Vector Table\n*/\n", "");

                // Append header braces and extras
                dynamicSections = dynamicSectionsHeader + dynamicSections;

                // Finally, allocate Config words/bytes after the linker script'sec default KEEP (*("__XXX.sec*")); config word assignment so we can force an error whenever the Bootloader and Application both have a Config word defined. Without proper placement, the compiler section allocation can just increment to the next instruction word, even though it is outside the MEMORY region'sec size.
                gldSectionAllocation = "";
                if(state.configWordsRange != null)
                {
                    for(PairWithText p : gldPairs)
                    {
                        if(((p.first >= state.configWordsRange.startAddress) && (p.first < state.configWordsRange.endAddress)))
                        {
                            gldSectionAllocation += p.text;
                        }
                    }
                }

                if(!gldSectionAllocation.isEmpty())    // Only print the header comment when we actually have some Config words in the Bootloader that could overlap with Application Config words.
                {
                    gldSectionAllocation = "  /*\n"
                                           + "  ** Bootloader Config words/bytes\n"
                                           + "  ** \n"
                                           + "  ** Explicitly map after the Application's Config values\n"
                                           + "  ** (if any) have been placed. This ensures an overlap\n"
                                           + "  ** linking failure error if you try to define the same\n"
                                           + "  ** Config word in both the Bootloader and Application\n"
                                           + "  ** projects (an ambiguous no-no).\n"
                                           + "  ** Be sure and remove any overlapping Config words from\n"
                                           + "  ** either your Bootloader or Application if you get an\n"
                                           + "  ** error here.\n"
                                           + "  */"
                                           + gldSectionAllocation;
                }

                // Append footer closing braces and extras
                dynamicSections += gldSectionAllocation + dynamicSectionsFooter;

                retGLDASM[0] = outGLD.GetFullBlock() + "\n\n" + dynamicSections;
            }

            // Done making the merge.S file contents
            retGLDASM[1] = CatStringList(asmMerge);
            if(!state.MCU16Mode)    // ASM code comment character is '#' instead of ';' for XC32
            {
                retGLDASM[1] = retGLDASM[1].replace(';', '#');
            }

            return retGLDASM;
        }
    }

// Returns all deleted or truncated memory regions
    static List<MemoryRegion> SubtractIgnoreRangesFromMemories(GLDMemories gldMemories, List<AddressRange> ignoreRanges)
    {
        List<MemoryRegion> deletedMemories;
        int nextMemIndex = 0;

        deletedMemories = new ArrayList<MemoryRegion>();

        while((nextMemIndex >= 0) && (nextMemIndex < gldMemories.all.size()))
        {
            MemoryRegion mem = gldMemories.all.get(nextMemIndex);
            nextMemIndex++;

            // Ignore RAM Memories
            if(mem.attribA < 0)
            {
                continue;
            }

            // Delete or shrink any memory region that overlaps in any way with an ignoreRange
            for(AddressRange ignoreRange : ignoreRanges)
            {
                // See if no part of this record exists in the selected ignore range
                if((mem.origin >= ignoreRange.endAddress) || (mem.origin + mem.length <= ignoreRange.startAddress))
                {
                    continue;
                }

                // Overlap exists, need region manipulation
                // See if the entire MEMORY region is encapsulated in
                // the ignoreRange and therefore should simply be
                // deleted.
                // Also, delete the entire "ivt" MEMORY region when
                // found and any overlap exists. This is done because
                // not all ivtVectors are populated and therefore won't all
                // be in the standalone .gld. However, it is unsafe for the 
                // linker to place anything in the unused addresses due to 
                // security hardware on manyPIC24/dsPIC devices which would 
                // causes a device reset if the PC ever pointed to an address in 
                // the ivt.
                if(((mem.origin >= ignoreRange.startAddress) && (mem.origin + mem.length <= ignoreRange.endAddress))
                   || ((mem.name.equals("ivt") || mem.name.equals("aivt"))))
                {
                    deletedMemories.add(mem);
                    gldMemories.all.remove(mem);
                    nextMemIndex--;
                    break;
                }

                // See if the ignoreRange lies in the middle of the memory region, in which case we need to split it into two regions
                if((mem.origin < ignoreRange.startAddress) && (mem.origin + mem.length > ignoreRange.endAddress))
                {
                    MemoryRegion mem2 = mem.Clone();
                    mem2.name += ".";
                    mem2.comment = "region split by bootloader";
                    mem2.origin = ignoreRange.endAddress;
                    mem2.length = mem.origin + mem.length - ignoreRange.endAddress;
                    gldMemories.all.add(nextMemIndex, mem2);
                    mem.length = ignoreRange.startAddress - mem.origin;
                    continue;
                }

                // See if we need to trim the right side of the memory region
                if((mem.origin + mem.length >= ignoreRange.startAddress) && (mem.origin < ignoreRange.startAddress))
                {
                    mem.length = ignoreRange.startAddress - mem.origin;
                    continue;
                }

                // Last case, means we need to trim the left side of the memory region
                mem.length -= ignoreRange.endAddress - mem.origin;
                mem.origin = ignoreRange.endAddress;
            }
        }

        return deletedMemories;
    }

    public static int CreateNonEZBLLinkerScripts(String[] args)
    {
        EZBLState state = CommandAndBuildState.ReadArgs(null, args);            // Parse command line options into instance specific state
        List<String> deviceParameters = new ArrayList<String>();
        List<String> commonSections = new ArrayList<String>();
        List<String> bootSections = new ArrayList<String>();
        List<String> appSections = new ArrayList<String>();
        List<String> appIGT = new ArrayList<String>();
        List<String> ivt = new ArrayList<String>();
        List<String> bootISRDispatchers = new ArrayList<String>();
        List<Integer> unimplementedIRQs = new ArrayList<Integer>();
        int implementedVectorCount = 0;
        int dispatchBranchSize = 0x4;       // 0x4 for GOTO, 0x2 for BRA on smaller devices <= 96KB or when it is known that the target is nearby
        int appIGTSpacing = 0x4;            // 0x4 used in IGT for GOTOs, or 0x2 for BRA on smaller devices <= 96KB. Must use GOTO on bigger devices since the reprogrammable App defines where the branch targets will lie
        String gldContents;
        TextBlock deviceGLD;
        String gldOriginalSectionsHeader = "", gldOriginalSections = "", gldOriginalSectionsFooter = "";

        // If nothing is specified upon execution or a pasing error occured, write usage information to STDOUT
        if(!state.parseOkay)
        {
            System.out.print("\r\n"
                             + Multifunction.FormatHelpText(79, 0 * 3, "Usage:")
                             + Multifunction.FormatHelpText(79, 1 * 3, "java -jar ezbl_tools.jar --gldbuilder -make_non_ezbl_glds -mcpu=PIC [-options] -compiler_folder=\"path to XC16 bins\" -path_to_ide_bin=\"path to MPLAB X bins\"")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Inputs:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Target PIC part number: ex: -mcpu=33EP64MC506, -mcpu=24FJ128GA010,-mcpu=33FJ256GP710A")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "Path to the XC16 bin installation folder for device .gld searching. Ex: \"C:\\Program Files (x86)\\Microchip\\xc16\\v1.32\\bin\"")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Outputs:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Generated .gld linker script file for two App project with interrupt multiplexing printed to stdout.")
                             + "\r\n"
            );

            return -1;
        }

        // Validate input parameters
        if(state.partNumber == null || state.compilerLinkScriptPath == null)
        {
            System.err.println("ezbl_tools: Missing required input parameter: need a device part number, compiler linker script to start from, and output linker script file specified.");
            return -2;
        }

        // Read the compiler's device .gld contents into the deviceGLD TextBlock class
        gldContents = Multifunction.ReadFile(state.compilerLinkScriptPath, true);
        if(gldContents == null)
        {
            System.err.println("ezbl_tools: Unable to read \"" + state.compilerLinkScriptPath + "\"");
            return -3;
        }

        deviceGLD = new TextBlock(gldContents);
        // Process all SECTIONS regions
        while(deviceGLD.Find("SECTIONS[^{]*", "\\{", "\\}", "\n"))
        {
            if(gldOriginalSectionsHeader.isEmpty())
            {
                gldOriginalSectionsHeader = deviceGLD.GetOuterLeft(false);
                gldOriginalSectionsFooter = deviceGLD.GetOuterRight(false);
            }
            TextBlock sectionBlock = new TextBlock(deviceGLD.GetInner(true));

            // Process any .ivt sections we find. These need to be remapped into
            // application space. Simultaneously we want to generate an
            // "Interrupt Goto Table", or .igt section. This code could be
            // modified to support an Interrupt Branch Table instead on devices
            // with small enough memory for branching to any address.
            if(sectionBlock.Find("((?ms)\\s/[*].*[*]/\\s\\.ivt)|((?-ms)\\s\\.ivt)[^:]*:[^{]*[^:]*:[^{]*", "\\{", "\\}", "\n"))
            //if(sectionBlock.Find("\\s[.]ivt[^:]*:[^{]*", "\\{", "\\}", "\n"))
            {
                String gldParse = sectionBlock.GetInner(true);

                gldParse = gldParse.replaceAll("__DefaultInterrupt", "");
                gldParse = gldParse.replaceAll("LONG[\\s]*?[(]", "");
                gldParse = gldParse.replaceAll("ABSOLUTE[\\s]*?[(]", "");
                gldParse = gldParse.replaceAll(":[\\s\\S]*?;", "");
                gldParse = gldParse.replaceAll("DEFINED[\\s]*?[(][\\s\\S]*?[?]", "");
                gldParse = gldParse.replaceAll("[)]", "");
                gldParse = gldParse.replaceAll("[\\s]+", "\n");
                gldParse = gldParse.trim();

                // Save a list of possible ivtVectors for the DumpParser to determine which are in use
                state.ivtVectors = gldParse.split("\n");

                // First, count out how many true interrupts are implemented
                // on this device 
                implementedVectorCount = 0;
                for(int i = 0; i < state.ivtVectors.length; i++)
                {
                    if(state.ivtVectors[i].startsWith("__Interrupt"))
                    {   // No physical hardware interrupt present: coalesce all IVT targets to single _DefaultInterrupt() App IGT entry
                        continue;
                    }
                    implementedVectorCount++;
                }

                // Create the IVT and IGT tables
                int outputIndex = 0;
                String igtCode;
                String formattedVectorName;
                for(int i = 0; i < state.ivtVectors.length; i++)
                {
                    if(state.ivtVectors[i].startsWith("__Interrupt"))
                    {   // No physical hardware interrupt present: coalesce all IVT targets to single _DefaultInterrupt() App IGT entry
                        ivt.add(String.format("%1$-54s/* Vector at 0x%2$06X, IRQ %3$3d, Vector %4$3d, target _APP_RESET_VECTOR + 0x%5$03X, %6$s coalesces to _DefaultInterrupt() */", "LONG(ABSOLUTE(_Dispatch__DefaultInterrupt));", 0x4 + 0x2 * i, i, i + 8, 0x4 + implementedVectorCount * appIGTSpacing, state.ivtVectors[i]));
                        unimplementedIRQs.add(i);
                        continue;
                    }

                    formattedVectorName = String.format("%1$-18s", state.ivtVectors[i]);
                    igtCode = "GOTO(DEFINED(" + formattedVectorName + ") ? " + formattedVectorName + " : __DefaultInterrupt);";
                    appIGT.add(String.format("%1$-84s/* IRQ %2$3d, Vector %3$3d, _APP_RESET_VECTOR + 0x%4$03X */", igtCode, i, i + 8, 0x4 + outputIndex * appIGTSpacing));
                    ivt.add(String.format("%1$-54s/* Vector at 0x%2$06X, IRQ %3$3d, Vector %4$3d, target _APP_RESET_VECTOR + 0x%5$03X */", "LONG(ABSOLUTE(_Dispatch" + state.ivtVectors[i] + "));", 0x4 + 0x2 * i, i, i + 8, 0x4 + outputIndex * appIGTSpacing));
                    bootISRDispatchers.add(String.format("_Dispatch%1$18s /* IRQ %2$3d, Vector %3$3d */", state.ivtVectors[i] + " = ABSOLUTE(.);", i, i + 8));
                    bootISRDispatchers.add("    BTSC(_IVTMuxTarget, 0);");
                    bootISRDispatchers.add("    BRA(_APP1_RESET_VECTOR + 0x" + String.format("%1$04X);", 0x04 + (outputIndex * appIGTSpacing)));
                    bootISRDispatchers.add("    BRA(_APP2_RESET_VECTOR + 0x" + String.format("%1$04X);", 0x04 + (outputIndex++ * appIGTSpacing)));
                }

                // Create the last IGT goto entry for unimplemented hardware ivtVectors that just points to the __DefaultInterrupt (in case if ever a new vector is added because it was missing originally in the compiler .gld)
                appIGT.add(String.format("%1$-84s/* _APP_RESET_VECTOR + 0x%2$03X, Unimplemented/coalesced IRQs */", "GOTO(__DefaultInterrupt);", 0x4 + implementedVectorCount * appIGTSpacing));
                bootISRDispatchers.add(String.format("_Dispatch%1$18s /* Unimplemented/coalesced IRQs */", "__DefaultInterrupt = ABSOLUTE(.);"));
                bootISRDispatchers.add("    BTSC(_IVTMuxTarget, 0);");
                bootISRDispatchers.add("    BRA(_APP1_RESET_VECTOR + 0x" + String.format("%1$04X);", 0x04 + (implementedVectorCount * appIGTSpacing)));
                bootISRDispatchers.add("    BRA(_APP2_RESET_VECTOR + 0x" + String.format("%1$04X);", 0x04 + (implementedVectorCount * appIGTSpacing)));
            }

            // Delete .ivt section mapping since we generated our own
            sectionBlock.ReplaceOuter("");

            // Delete .reset section since we will be generating our own
            //if(sectionBlock.Find("(?ms)(/[*]).*?([*]/)\\s*?\\.reset[^:]*:[^{]*", "\\{", "\\}", "\n\\s*?\\#endif\n"))  // Nukes more than it should, such as Program Memory comment before the section
            if(sectionBlock.Find("\\s[.]reset[^:]*:[^{]*", "\\{", "\\}", "\n"))
            {
                sectionBlock.ReplaceOuter("");// /* removed, see above .reset mappings */");
            }

            gldOriginalSections += sectionBlock.GetFullBlock();
            deviceGLD.DeleteOuter();
        }

        // Append header and footer closing braces and extras
        gldOriginalSections = gldOriginalSectionsHeader + gldOriginalSections + gldOriginalSectionsFooter;

        // Add all stuff in the original .gld file, minus the stuff we took out
        deviceParameters.add(deviceGLD.GetFullBlock());

        // Add our symbol constants obtained from the Essential Device Characteristics database
        deviceParameters.add("\n\n");
        deviceParameters.add(String.format("\n_EZBL_ADDRESSES_PER_SECTOR      = 0x%1$06X;", state.eraseBlockSizeAddresses));
        deviceParameters.add(String.format("\n_EZBL_MAIN_FLASH_BASE           = 0x%1$06X;", state.mainFlashRange.startAddress | (state.dualPartitionMode ? 0x400000 : 0)));
        deviceParameters.add(String.format("\n_EZBL_MAIN_FLASH_END_ADDRESS    = 0x%1$06X;", state.mainFlashRange.endAddress | (state.dualPartitionMode ? 0x400000 : 0)));
        deviceParameters.add(String.format("\n_EZBL_AUX_FLASH_BASE            = 0x%1$06X;", state.auxFlashRange == null ? 0 : state.auxFlashRange.startAddress));
        deviceParameters.add(String.format("\n_EZBL_AUX_FLASH_END_ADDRESS     = 0x%1$06X;", state.auxFlashRange == null ? 0 : state.auxFlashRange.endAddress));
        deviceParameters.add(String.format("\n_EZBL_CONFIG_BASE               = 0x%1$06X;", state.configWordsRange == null ? 0 : state.configWordsRange.startAddress | (state.dualPartitionMode ? 0x400000 : 0)));
        deviceParameters.add(String.format("\n_EZBL_CONFIG_END_ADDRESS        = 0x%1$06X;", state.configWordsRange == null ? 0 : state.configWordsRange.endAddress | (state.dualPartitionMode ? 0x400000 : 0)));
        deviceParameters.add(String.format("\n_EZBL_DEVID_ADDRESS             = 0x%1$06X;", state.deviceIDAddress));
        deviceParameters.add(String.format("\n_EZBL_DEVID_MASK                = 0x%1$06X;", state.deviceIDMask));
        deviceParameters.add(String.format("\n_EZBL_DEVID_VALUE               = 0x%1$06X;", state.deviceIDValue));
        deviceParameters.add(String.format("\n_EZBL_REVID_ADDRESS             = 0x%1$06X;", state.deviceRevAddress));
        deviceParameters.add(String.format("\n_EZBL_REVID_MASK                = 0x%1$06X;", state.deviceRevMask));
        deviceParameters.add(String.format("\n_EZBL_RESERVED_BIT_ADDRESS      = 0x%1$06X;   %2$s", state.reservedBitAddress | (state.dualPartitionMode ? 0x400000 : 0), "/* " + state.reserveBitConfigWordName + " */"));
        deviceParameters.add(String.format("\n_EZBL_RESERVED_BIT_MASK         = 0x%1$06X;", state.reservedBitMask));
        deviceParameters.add(String.format("\n_EZBL_CODE_PROTECT_ADDRESS      = 0x%1$06X;   %2$s", state.codeProtectAddress | (state.dualPartitionMode ? 0x400000 : 0), "/* " + state.codeProtectConfigWordName + " */"));
        deviceParameters.add(String.format("\n_EZBL_CODE_PROTECT_MASK         = 0x%1$06X;", state.codeProtectMask));
        deviceParameters.add(String.format("\n_EZBL_BACKBUG_ADDRESS           = 0x%1$06X;   %2$s", state.deviceBACKBUGAddress | (state.dualPartitionMode ? 0x400000 : 0), "/* " + state.BACKBUGConfigWordName + " */"));
        deviceParameters.add(String.format("\n_EZBL_BACKBUG_MASK              = 0x%1$06X;", state.deviceBACKBUGMask));
        deviceParameters.add("\n"
                             + "\n/* Define macros for generating Reset Vector, IVT, IGT and run-time multiplexing dispatch code */"
                             + "\n#define BTSC(address, bit)   LONG(0xAF0000 | (ABSOLUTE(address) & 0x1FFE) | ((ABSOLUTE(bit) & 0x7)<<13) | ((ABSOLUTE(bit) & 0x8)>>3))"
                             + "\n#define BRA(short_address)   LONG(0x370000 | (((ABSOLUTE(short_address) - ABSOLUTE(. + 2))>>1) & 0x00FFFF))"
                             + "\n#define GOTO(address)        LONG(0x040000 | (ABSOLUTE(address) & 0x00FFFF)); LONG(0x000000 | ((ABSOLUTE(address) & 0x7F0000)>>16))"
                             + "\n"
                             + "\n");

        commonSections.add(
                "\nSECTIONS"
                + "\n{"
                + "\n  /* INTRecipient global RAM variable allocated in all projects to select ISR to execute when HW Interrupt occurs */"
                + "\n  .data.IVTMuxTarget (__DATA_BASE + 0x50) (NOLOAD) :"
                + "\n  {"
                + "\n    _IVTMuxTarget = .;"
                + "\n    . += 2;"
                + "\n  } >data"
                + "\n");

        bootSections.add("\n"
                         + "\n/* Flash contents for Bootloader or other project that owns the hardware vectors */"
                         + "\n#if !defined(APP_RESET_VECTOR)    "
                         + "\n  #if !defined(__CORESIDENT) || defined(__DEFINE_RESET)"
                         + "\n  .reset :"
                         + "\n  {"
                         + "\n    GOTO(__reset);"
                         + "\n  } >reset"
                         + "\n  #endif"
                         + "\n"
                         + "\n"
                         + "\n  .ivt __IVT_BASE :"
                         + "\n  {"
                         + "\n    " + CatStringList(ivt, "\n    ")
                         + "\n  } >ivt"
                         + "\n"
                         + "\n"
                         + "\n  /**"
                         + "\n   * Program code to run time select which ISR executes on hardware interrupt."
                         + "\n   *   IVTMuxTarget<0> == '0' means App 1 ISRs execute."
                         + "\n   *   IVTMuxTarget<0> == '1' means App 2 ISRs execute."
                         + "\n   * Access the IVTMuxTarget variable using this declaration:"
                         + "\n   *   extern volatile unsigned int __attribute__((near, keep, noload)) IVTMuxTarget;"
                         + "\n   */"
                         + "\n  .text.ISRDispatchers :"
                         + "\n  {"
                         + "\n    " + CatStringList(bootISRDispatchers, "\n    ")
                         + "\n  } >program"
                         + "\n");

        appSections.add("\n#else    /* APP_RESET_VECTOR macro defined in project and set to the base program address for the Application project */"
                        + "\n"
                        + "\n  _APP_RESET_VECTOR = ABSOLUTE(APP_RESET_VECTOR);"
                        + "\n"
                        + "\n  .reset (ABSOLUTE(_APP_RESET_VECTOR) + 0x0) :"
                        + "\n  {"
                        + "\n    GOTO(__reset);"
                        + "\n  } >program"
                        + "\n"
                        + "\n"
                        + "\n  .igt (ABSOLUTE(_APP_RESET_VECTOR) + 0x4) :"
                        + "\n  {"
                        + "\n    " + CatStringList(appIGT, "\n    ")
                        + "\n  } >program"
                        + "\n#endif"
                        + "\n}"
                        + "\n"
                        + "\n"
                        + "\n");

        System.out.print(CatStringList(deviceParameters) + CatStringList(commonSections) + CatStringList(bootSections) + CatStringList(appSections) + gldOriginalSections);
        return 0;
    }
}
