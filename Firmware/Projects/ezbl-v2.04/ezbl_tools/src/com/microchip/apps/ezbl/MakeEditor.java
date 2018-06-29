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

import java.io.*;
import static java.lang.String.valueOf;
import java.security.*;
import java.util.*;
import java.util.regex.*;


/**
 *
 * @author C12128
 */
public class MakeEditor
{

    /**
     * @param args the command arguments
     */
    public static void main(String[] args)
    {
        String makeContents, newMakeContents;
        String originalCommand, originalLinkOutputFile;
        String expandedPassThroughOptions;
        boolean found;
        boolean partitionOptionFound;
        List<String> newCommands;
        String str;                         // Temporary string concatenations
        String stateSavePath;

        boolean keepGoing = false;
        for(int i = 0; i < args.length; i++)
        {
            String arg = args[i];
            String token = arg.trim().toLowerCase();
            if(token.startsWith("-updateid="))
            {
                IncIDAndBuildNumber(arg.trim().substring("-updateid=".length()));
                args[i] = ""; // Nuke this argument now that we've used it. There is no sense in propagating it to Bootloader build steps.
                continue;
            }
            else if(token.startsWith("-conf="))
            {
                keepGoing = true;
            }
            else if(token.startsWith("-makefile="))
            {
                keepGoing = true;
            }
        }
        if(!keepGoing)
        {
            return;
        }

        // Obtain the given command line options and all prior ezbl_tools invocation state information
        stateSavePath = CommandAndBuildState.GetStateFilePath(args);
        EZBLState state = EZBLState.ReadFromFile(stateSavePath);    // Load up the preexisting state file from disk, if one exists
        if((state.argsFromMakeEditor == null) || !Arrays.equals(state.argsFromMakeEditor, args))    // If read-failed, we got a new class
        {
            state = CommandAndBuildState.ReadArgs(state, args);                 // Parse command line options and load extended EDC data and save them
        }

        // If nothing is specified upon execution, write usage information to STDOUT
        if(!state.parseOkay)
        {
            System.out.print("\r\n"
                             + Multifunction.FormatHelpText(79, 0 * 3, "Usage:")
                             + Multifunction.FormatHelpText(79, 1 * 3, "java -jar ezbl_tools.jar --make_editor -makefile=\"nbproject/Makefile-${CONF}.mk\" [options]")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Inputs:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "-conf=${CONF}")
                             + Multifunction.FormatHelpText(79, 3 * 3, "or")
                             + Multifunction.FormatHelpText(79, 2 * 3, "-makefile=\"nbproject/Makefile-${CONF}.mk\"")
                             + Multifunction.FormatHelpText(79, 3 * 3, "MPLAB X current project configuration Makefile (not the user Makefile) to modify for EZBL linking/merging.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Outputs:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "Project Makefile-${CONF}.mk is edited to invoke the ezbl_tools.jar at appropriate times during the build process so that EZBL can be linked with the application and contain all needed project/device specific data.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 1 * 3, "Options:")
                             + Multifunction.FormatHelpText(79, 2 * 3, "-save-temps")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Option to keep any intermediate files generating by ezbl_tools.jar during the make process.")
                             + "\r\n"
                             + Multifunction.FormatHelpText(79, 2 * 3, "-temp-folder=\"temporaries_path\"")
                             + Multifunction.FormatHelpText(79, 3 * 3, "Path to write intermediate output files. This option is ignored if -save-temps is unspecified. If this option is unspecified, the same folder that this .jar file is in will be used.")
                             + "\r\n"
            );

            return;
        }

        if(state.MCU16Mode == false)
        {   // For PIC32MM devices, we don't edit the Makefile in this EZBL version.
            return;
        }

        if(state.makefilePath == null)
        {
            System.err.println("ezbl_tools: Missing required -conf=${CONF} or -makefile=\"nbproject/Makefile-${CONF}.mk\" parameter.");
            System.exit(-1);
        }

        // Temporary path name for Pass 1 linking output
        state.elfPath = state.temporariesPath + "ezbl_partial_link.elf";

        // Add as much information as we might need later.
        expandedPassThroughOptions = state.undecodedOptions;
        if(!expandedPassThroughOptions.contains("-conf="))
        {
            expandedPassThroughOptions += " -conf=${CONF}";
        }
        if(!expandedPassThroughOptions.contains("-mcpu="))
        {
            expandedPassThroughOptions += " -mcpu=${MP_PROCESSOR_OPTION}";
        }
        if(!expandedPassThroughOptions.contains("-artifact="))
        {
            expandedPassThroughOptions += " -artifact=\"${FINAL_IMAGE}\"";
        }
        if(!expandedPassThroughOptions.contains("-linkscript="))
        {
            expandedPassThroughOptions += " -linkscript=${MP_LINKER_FILE_OPTION}";
        }
        if(!expandedPassThroughOptions.contains("-compiler_folder="))
        {
            expandedPassThroughOptions += " -compiler_folder=${MP_CC_DIR}";
        }
        if(!expandedPassThroughOptions.contains("-java"))
        {
            expandedPassThroughOptions += " -java=${MP_JAVA_PATH}";
        }
        if(!expandedPassThroughOptions.contains("-project_name="))
        {
            expandedPassThroughOptions += " -project_name=\"${PROJECTNAME}\"";
        }
        if(!expandedPassThroughOptions.contains("-state="))
        {
            expandedPassThroughOptions += " -state=\"dist" + File.separator + "${CND_CONF}" + File.separator + "${IMAGE_TYPE}" + File.separator + "ezbl_build_state.bin\"";
        }

        // Read in the selected configuration makefile (nbproject/Makefile-${CONF}.mk)
        makeContents = Multifunction.ReadFile(state.makefilePath, true);
        if(makeContents == null)
        {
            System.err.println("ezbl_tools: Unable to read \"" + state.makefilePath + "\"");
            System.exit(-2);
        }

        // Remove all historically inserted sections, if any
        makeContents = makeContents.replaceAll("(?sm)^###\\<EZBL MODIFIED 1\\>###\n.*?###\\</EZBL MODIFIED 1\\>###\n", ""); // DO NOT CHANGE ORDER OR TRY AND COMBINE: these are nested, so order maters
        makeContents = makeContents.replaceAll("(?sm)^###\\<EZBL MODIFIED 0\\>###\n.*?###\\</EZBL MODIFIED 0\\>###\n", ""); // DO NOT CHANGE ORDER OR TRY AND COMBINE: these are nested, so order maters

        // Process all invocations of the compiler's linker (should be two: one for debug mode, one for production mode)
        TextBlock makeData = new TextBlock(makeContents);
        found = false;
        makeData.ResetFind();
        while(makeData.Find("(?m)^[\t][^\\$]*[\\$]([{]|[(])MP_CC([}]|[)]).*?-Wl[^\n]*", "\n"))
        {
            found = true;
            originalCommand = makeData.GetInner(false);
            originalCommand = originalCommand.substring(0, originalCommand.length() - 1); // Trim off trailing new-line: we need to append to this line
            originalCommand = originalCommand.replaceAll("[.][$]([{]|[(])OUTPUT_SUFFIX([}]|[)])", Matcher.quoteReplacement(".${DEBUGGABLE_SUFFIX}"));   // Should output .elf filename when linking, but MPLAB X IDE circa v3.50 produces something like "dist/${CND_CONF}/${IMAGE_TYPE}/ex_boot_uart.${IMAGE_TYPE}.${OUTPUT_SUFFIX}" in debug builds, which intends to makes a .hex file (!) output from the linker. This is not right and needs switching back to the .elf suffix.

            // Look for "--partition,1" or "--partition,2" linker options indicating we should get split view data from EDC database
            partitionOptionFound = originalCommand.contains("--partition");
            if(state.dualPartitionMode != partitionOptionFound)
            {
                // Partition mode changed, so flag that the cached (if any) EDC info is invalid and needs reparsing.
                state.edcInfoLoaded = false;
                state.dualPartitionMode = partitionOptionFound;
            }

            TextBlock commandParser = new TextBlock(originalCommand);
            if(commandParser.Find("-o[\\s]+[\\\"]", "[\\\"][\\s]"))
            {
            }
            else
            {
                commandParser.ResetFind();
                commandParser.Find("-o[\\s]+", "[\\s]");
            }
            originalLinkOutputFile = commandParser.GetInner(true);

            if(originalCommand.toLowerCase().contains("-save-temps"))
            {
                state.saveTemps = true;
            }

            str = "###<EZBL MODIFIED 0>###\n"
                  + "\t@echo EZBL: Updating automatic sections in linker script\n"
                  + "\t" + state.javaLaunchString + " --gldbuilder " + expandedPassThroughOptions + "\n"
                  + "\t@echo EZBL: Starting linking pass 1 of 2\n"
                  + originalCommand;
            str = str.replaceAll("[\\s]+-o[\\s]+[\\S]+[\\s]", Matcher.quoteReplacement(" -o \"" + state.elfPath + "\"")) + (System.getProperty("os.name").startsWith("Windows") ? " 1> NUL\n" : "1> /dev/null\n");
            str = str.replaceAll("--check-sections", Matcher.quoteReplacement("--no-check-sections"));  // Link with --no-check-sections set on Pass 1 so we can force linking and determine sizes for Pass 2 linking (where MPLAB specified section checking will apply).
            if(!str.contains("--no-check-sections"))
            {
                str = str.replaceFirst("-Wl", Matcher.quoteReplacement("-Wl,--no-check-sections"));
            }
            newCommands = new ArrayList<String>();
            newCommands.add(str);
            newCommands.add("\t" + state.javaLaunchString + " --dump_parser -pass=1 -elf_artifact=\"" + state.elfPath + "\" " + expandedPassThroughOptions + "\n");
            newCommands.add("\t@echo EZBL: Starting linking pass 2 of 2\n");
            newCommands.add("###</EZBL MODIFIED 0>###\n");
            newCommands.add(originalCommand + "\n");  // Add the original linking line for pass 2
            newCommands.add("###<EZBL MODIFIED 0>###\n");
            newCommands.add("#\tRun the ezbl_tools.jar dump processor again so we can extract the final EZBL reserved locations\n");
            newCommands.add("\t" + state.javaLaunchString + " --dump_parser -pass=2 -elf_artifact=\"" + originalLinkOutputFile + "\" " + expandedPassThroughOptions + "\n");
            newCommands.add("\t" + state.javaLaunchString + " --blobber -generate_merge " + expandedPassThroughOptions.replace("-artifact=\"${FINAL_IMAGE}\"", "-artifact=\"" + originalLinkOutputFile + "\"") + "\n");
            if(!state.saveTemps)    // Delete ezbl_partial_link.elf used in pass 1 linking if save-temps not specified in MPLAB project and the file exists (wasn't already deleted)
            {
                newCommands.add("\t@test \"" + state.elfPath + "\" && rm \"" + state.elfPath + "\"\n");
            }
            newCommands.add("###</EZBL MODIFIED 0>###\n");
            str = Multifunction.CatStringList(newCommands);

            makeData.ReplaceOuter(str);
        }
        if(!found)
        {
            System.err.println("ezbl_tools: Could not find linking command line location(s) in " + state.makefilePath + ". This EZBL tool version will not work properly with this MPLAB X IDE version.");
        }

        // Add --blobber commands for unified .hex files
        {
            // Set up command lines needed for converting .hex file output to a .blob file (and also merge.s/merge.gld files when in standaloneMode)
            newCommands = new ArrayList<String>();
            newCommands.add("###<EZBL MODIFIED 1>###\n");
            newCommands.add("\t@echo Converting ${CND_ARTIFACT_NAME_${CONF}} to an EZBL firmware image\n");
            newCommands.add("\t" + state.javaLaunchString + " --blobber -generate_merge " + expandedPassThroughOptions.replaceFirst("[$][{(]FINAL_IMAGE[})]", Matcher.quoteReplacement("$(subst .hex,.elf,${FINAL_IMAGE})")) + "\n");
            if(!state.saveTemps)    // Delete ezbl_partial_link.elf used in pass 1 linking if save-temps not specified in MPLAB project and the file exists (wasn't already deleted)
            {
                newCommands.add("\t@test \"" + state.elfPath + "\" && rm \"" + state.elfPath + "\"\n");
            }
            newCommands.add("###</EZBL MODIFIED 1>###\n");
            str = Multifunction.CatStringList(newCommands);

            // Find invocations of hexmate.exe which MPLAB X IDE uses for combining Loadables with the project build output in order to create a unified.hex file
            makeData.ResetFind();
            while(makeData.Find("(?m)^[\t][^\n]*?hexmate[\\s\".]*", "-[oO][\"]?", "[\\s\"]", "\n"))         // Look for invocations of hexmate (used when building a project with loadables in it)
            {
                makeData.ReplaceOuter(makeData.GetOuter() + str);
            }
        }

        // Write new Makefile, but only if we made any changes
        newMakeContents = makeData.GetFullBlock();
        int writeResult = Multifunction.UpdateFileIfDataDifferent(state.makefilePath, newMakeContents, false);
        if(writeResult < 0)    // Check for error
        {
            System.err.println("ezbl_tools: failed to write to \"" + state.makefilePath + "\"");
        }

        // Save any parsing work we did for later use
        state.pass = 0;
        state.SaveToFile();
    }

    static void IncIDAndBuildNumber(String ezblProductIDFilePath)
    {
        String BOOTID_HASH;
        List<String> findStrings = new ArrayList<String>();
        String fieldContents;
        boolean errorReturn = false;
        String fileContents = Multifunction.ReadFile(ezblProductIDFilePath, true);

        if(fileContents == null)
        {
            System.err.printf("ezbl_tools: could not read '%1$s' for updating BOOTID_HASH/APPID_VER_BUILD data\n", ezblProductIDFilePath);
            System.exit(-1);
        }

        // Update BOOTID_HASH fields from the BOOTID_VENDOR/BOOTID_MODEL/BOOTID_NAME/BOOTID_NAME strings (with quotation marks)
        byte computedHash[];
        MessageDigest hashComputer;
        try
        {
            hashComputer = MessageDigest.getInstance("SHA-256");

            findStrings.add("VENDOR");
            findStrings.add("MODEL");
            findStrings.add("NAME");
            findStrings.add("OTHER");

            for(String fstr : findStrings)
            {
                Matcher m = Pattern.compile("^ *?BOOTID_" + fstr + "[ ]*?=[\\s]*?", Pattern.MULTILINE | Pattern.UNIX_LINES).matcher(fileContents);

                if(m.find())
                {
                    int fieldStartIndex = m.end() + 1;
                    int fieldEndIndex = fileContents.indexOf('\n', fieldStartIndex + 1);
                    fieldContents = fileContents.substring(fieldStartIndex, fieldEndIndex);
                    int firstQuote = fieldContents.indexOf('\"');
                    if(firstQuote >= 0)
                    {
                        int secondQuote = fieldContents.indexOf('\"', firstQuote + 1);
                        if(secondQuote >= 0)
                        {
                            fieldContents = fieldContents.substring(firstQuote, secondQuote + 1); // Include quotations marks, but nothing else
                        }
                    }

                    hashComputer.update(fieldContents.getBytes());
                }
            }

            computedHash = hashComputer.digest();
            BOOTID_HASH = String.format("%1$02X%2$02X%3$02X%4$02X%5$02X%6$02X%7$02X%8$02X%9$02X%10$02X%11$02X%12$02X%13$02X%14$02X%15$02X%16$02X%17$02X%18$02X%19$02X%20$02X",
                                        computedHash[0], computedHash[1], computedHash[2], computedHash[3], computedHash[4],
                                        computedHash[5], computedHash[6], computedHash[7], computedHash[8], computedHash[9],
                                        computedHash[10], computedHash[11], computedHash[12], computedHash[13], computedHash[14],
                                        computedHash[15], computedHash[16], computedHash[17], computedHash[18], computedHash[19]);
            for(int i = 20; i < computedHash.length; i += 4)    // Bytes 20 to 32 for SHA-256
            {
                BOOTID_HASH += String.format("%1$02X%2$02X%3$02X%4$02X", computedHash[i], computedHash[i + 1], computedHash[i + 2], computedHash[i + 3]);
            }

            fileContents = fileContents.replaceFirst("(?mu)^[ ]*?BOOTID_HASH[ ]*?=.*?$", "BOOTID_HASH  = " + BOOTID_HASH);
            int j = BOOTID_HASH.length();
            for(int i = 0; i < computedHash.length; i += 4, j -= 8)
            {
                String index = String.valueOf(i / 4);
                fileContents = fileContents.replaceFirst("(?mu)^[ ]*?BOOTID_HASH" + index + "[ ]*?=.*?$", "BOOTID_HASH" + index + " = 0x" + BOOTID_HASH.substring(j - 8, j));
            }
        }
        catch(NoSuchAlgorithmException ex)
        {
            System.err.println("Missing SHA-256 java hashing library. BOOTID not computed/updated.");
            errorReturn = true; // Flag to exit with non-zero exit code, but don't exit yet so we can update APPID_VER_BUILD still.
        }

        // Handle APPID_VER_BUILD += APPID_VER_BUILD_INC, if present
        int increment = 0;
        long lastBuild = 0;
        int buildIndex = -1, buildEndIndex = -1;

        // Find/decode APPID_VER_BUILD_INC value
        Matcher m = Pattern.compile("^ *?APPID_VER_BUILD_INC[ ]*?=[ ]*?", Pattern.MULTILINE | Pattern.UNIX_LINES).matcher(fileContents);
        if(m.find())
        {
            int fieldStartIndex = m.end() + 1;
            int fieldEndIndex = fileContents.indexOf('\n', fieldStartIndex + 1);
            fieldContents = fileContents.substring(fieldStartIndex, fieldEndIndex);
            increment = Integer.decode(fieldContents);

            // Find APPID_VER_BUILD variable
            m = Pattern.compile("^ *?APPID_VER_BUILD[ ]*?=[ ]*?", Pattern.MULTILINE | Pattern.UNIX_LINES).matcher(fileContents);
            if(m.find())
            {
                buildIndex = m.end() + 1;
                buildEndIndex = fileContents.indexOf('\n', buildIndex + 1);
                fieldContents = fileContents.substring(buildIndex, buildEndIndex);
                lastBuild = Long.decode(fieldContents);

                // Add and update file contents as string
                fileContents = fileContents.substring(0, buildIndex) + valueOf(lastBuild + increment) + fileContents.substring(buildEndIndex);
            }
        }

        // Returns -1 if file was updated. 0 if nothing changed.
        System.exit(Multifunction.UpdateFileIfDataDifferent(ezblProductIDFilePath, fileContents, false));

        if(errorReturn)
        {
            System.exit(-1);
        }
    }
}
