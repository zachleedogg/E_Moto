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

import static com.microchip.apps.ezbl.Multifunction.FixSlashes;
import static com.microchip.apps.ezbl.EDCReader.*;
import com.microchip.crownking.Pair;
import com.microchip.crownking.edc.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.util.regex.*;
import org.w3c.dom.*;


/**
 *
 * @author C12128
 */
public class CommandAndBuildState implements Serializable
{
    public static final byte defaultEncryptionSalt[] =
    {
        (byte)0x95, (byte)0x0F, (byte)0xEB, (byte)0xD9, (byte)0xD7, (byte)0xBA, (byte)0x8E, (byte)0xCD, (byte)0xA4, (byte)0x88, (byte)0x53, (byte)0x60, (byte)0x7C, (byte)0xE4, (byte)0xF9, (byte)0x4B
    };

    /**
     * Returns a EZBLState class which is either a reference to the
     * stateTemplate that has been provided, updated by the args that have also
     * been provided, or a new EZBLState class, filled in with all values
     * specified by args. The stateTemplate is modified only when the "-mcpu="
     * argument is NOT provided, or "-mcpu=" arg is provided, but matches that
     * which was already stored in the stateTemplate.
     *
     * @param stateTemplate A EZBLState class to modify or replace. Can be null
     *                      if no prior state information is needed/wanted.
     * @param args          Array of Strings containing individual command line
     *                      arguments.
     *
     * @return EZBLState class reference to the updated stateTemplate, or a new
     *         EZBLClass (as needed).
     */
    public static EZBLState ReadArgs(EZBLState stateTemplate, String[] args)
    {
        boolean FBOOTAddressOverride = false;   // Flag indicating if FBOOT was specified by command line in args (or if we should use what we find in the EDC database instead)
        String raw_mcpu = null;

        // Create a new, default EZBLState as the stateTemplate if none is explicitly passed in
        if(stateTemplate == null)
        {
            stateTemplate = new EZBLState();
        }
        if((args == null) || (args.length == 0))    // If no arguments are specified upon execution, the return the stateTemplate unchanged or generated fresh
        {
            return stateTemplate;
        }

        for(int inIndex = 0; inIndex < args.length; inIndex++)
        {
            if((args[inIndex] == null) || (args[inIndex].length() == 0))    // Skip zero length args (ones we've eaten someplace on purpose so they don't propagate).
            {
                continue;
            }

            // Get argument token, normalize to lowercase and if it starts with '--', make it only have one '-'.
            if(args[inIndex].startsWith("--"))
            {
                args[inIndex] = args[inIndex].substring(1);
            }
            String token = args[inIndex].toLowerCase();
            String newOption = args[inIndex];
            int lastEqualIndex = newOption.lastIndexOf('=');
            if(lastEqualIndex >= 0)
            {   // When the option contains an equals, we would have lost a set of quotation marks after the equals. Re-add them.
                newOption = newOption.substring(0, lastEqualIndex + 1) + "\"" + newOption.substring(lastEqualIndex + 1) + "\"";
            }

//            // Build a list of everything that was given to us so it can be passed down later
//            if (!stateTemplate.passThroughOptions.contains(newOption))
//            {
//                stateTemplate.passThroughOptions += " " + newOption + " ";
//            }
            if(token.startsWith("-build_standalone"))  // Default if -build_as_one is unspecified
            {
                stateTemplate.standaloneMode = true;
                stateTemplate.buildAsOneMode = false;
            }
            else if(token.startsWith("-build_as_one"))
            {
                stateTemplate.standaloneMode = false;
                stateTemplate.buildAsOneMode = true;
                stateTemplate.deleteResetSection = true;         // Note, this default true selection could be overridden by an explicit -delete.reset=false option given _after_ -build_as_one is specified on the command line. This will probably break the build, but if the user specifies it, maybe they have a reason to do so.
            }
            else if(token.startsWith("-state="))
            {
                stateTemplate.ezblStateSavePath = Multifunction.TrimQuotes(args[inIndex].substring("-state=".length()));
            }
            else if(token.startsWith("-16") || token.startsWith("-pic24") || token.startsWith("-dspic"))
            {
                stateTemplate.MCU16Mode = true;
            }
            else if(token.startsWith("-32") || token.startsWith("-pic32"))
            {
                stateTemplate.MCU16Mode = false;
            }
            else if(token.startsWith("-noivtremap"))
            {
                stateTemplate.remapISRThroughIGT = false;
            }
            else if(token.startsWith("-ivtremap"))
            {
                stateTemplate.remapISRThroughIGT = true;
            }
            else if(token.startsWith("-delete.reset="))    // Allows explicit true or false assigment of deleteResetSection
            {
                stateTemplate.deleteResetSection = Boolean.parseBoolean(Multifunction.TrimQuotes(args[inIndex].substring("-delete.reset=".length())));
            }
            else if(token.startsWith("-delete.reset"))     // Allows implicit true assignment to deleteResetSection
            {
                stateTemplate.deleteResetSection = true;
            }
            else if(token.startsWith("-timeout="))     // Timeout in milliseconds to use for communications command acknowledgements
            {
                stateTemplate.milliTimeout = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-timeout=".length())));
            }
            else if(token.startsWith("-save-temps") || token.startsWith("-save_temps"))   // Save temporary files generated by ezbl_tools.jar operations
            {
                stateTemplate.saveTemps = true;
                if(token.startsWith("-save-temps=") || token.startsWith("-save_temps="))    // Also save the given filename, if given
                {
                    stateTemplate.saveTempsFile = Multifunction.TrimQuotes(args[inIndex].substring("-save-temps=".length()));
                }
            }
            else if(token.startsWith("-base="))        // Base address to build the bootloader for
            {
                stateTemplate.baseAddress = Long.decode(Multifunction.TrimQuotes(args[inIndex].substring("-base=".length())));
            }
            else if(token.startsWith("-pass="))        // Pass count invoking this tool when doing multi-pass linking operations. Starts at 1.
            {
                stateTemplate.pass = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-pass=".length())));
            }
            else if(token.startsWith("-ignore="))      // Address range to remove or ignore from the input/output (context sensitive)
            {
                stateTemplate.ignoreROMRanges.add(new AddressRange(Multifunction.TrimQuotes(args[inIndex].substring("-ignore=".length()))));
            }
            else if(token.startsWith("-temp-folder=") || token.startsWith("-temp_folder=")) // Location to write temporary files to
            {
                stateTemplate.temporariesPath = Multifunction.TrimQuotes(args[inIndex].substring("-temp-folder=".length()));
            }
            else if(token.startsWith("-password="))    // Passphrase used for encryption/decryption
            {
                stateTemplate.encryptionPassword = Multifunction.TrimQuotes(args[inIndex].substring("-password=".length()));
            }
            else if(token.startsWith("-salt="))        // Salt used for generating the encryption/decryption key from the passphrase/password
            {
                stateTemplate.encryptionSaltString = Multifunction.TrimQuotes(args[inIndex].substring("-salt=".length()));
            }
            else if(token.startsWith("-conf="))        // MPLAB X ${CONF} make variable defining which configuration is targetted for the present build operation
            {
                stateTemplate.conf = Multifunction.TrimQuotes(args[inIndex].substring("-conf=".length()));
            }
            else if(token.startsWith("-partition="))           // Target codePartition when building an application supporting Dual Partition functions (requires hardware support). Option can be 1 or 2.
            {
                stateTemplate.targetPartition = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-partition=".length())));
                stateTemplate.dualPartitionMode = (stateTemplate.targetPartition != 0);
                if(stateTemplate.dualPartitionMode)
                {
                    stateTemplate.remapISRThroughIGT = false;
                }
            }
            else if(token.startsWith("-fboot="))               // Value to program into the FBOOT configuration word (only needed if absent from the .hex/.blob file and targetting a Dual Partition configuration)
            {
                stateTemplate.FBOOTvalue = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-fboot=".length())));
            }
            else if(token.startsWith("-fboot_address="))       // Address of the FBOOT configuration word (only needed if absent from the .hex/.blob file and targetting a Dual Partition configuration)
            {
                FBOOTAddressOverride = true;
                stateTemplate.FBOOTaddress = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-fboot_address=".length())));
            }
            else if(token.startsWith("-makefile="))            // Makefile-${CONF}.mk file used for the project build. This is automatically determined if -conf is specified, but specifying the -makefile option will take precedence.
            {
                stateTemplate.makefilePath = Multifunction.TrimQuotes(args[inIndex].substring("-makefile=".length()));
            }
            else if(token.startsWith("-elf_artifact="))
            {
                stateTemplate.elfPath = Multifunction.TrimQuotes(args[inIndex].substring("-elf_artifact=".length()));
            }
            else if(token.startsWith("-mcpu="))                // Main target CPU option (ex: "33ep512mu810")
            {
                raw_mcpu = args[inIndex];
                String newPartNumber = Multifunction.TrimQuotes(args[inIndex].substring("-mcpu=".length()));
                newPartNumber = newPartNumber.replaceFirst(Matcher.quoteReplacement("([dD][sS][pP][iI][cC])|([pP][iI][cC])"), "");
                if(stateTemplate.partNumber != null)
                {
                    if(!newPartNumber.equalsIgnoreCase(stateTemplate.partNumber))
                    {
                        // A previous stateTemplate file was loaded, but this part number does not match the prior one.
                        // This can break various operations due to presaved other parameters that are no longer applicable.
                        // Therefore, let's delete all prior stateTemplate information and start over again parsing all
                        // parameters we were given in the MakeEditor stage plus all the parameters we have now.
                        if(stateTemplate.verbose)
                        {
                            System.out.print("EZBL: Different -mcpu value provided; ignoring stale " + stateTemplate.ezblStateSavePath + " state file contents.\n");
                        }
                        EZBLState ret = ReadArgs(null, args);   // Generate new state, and parse present args
                        ret.argsFromMakeEditor = args;          // Save args strings indicating what this state file is configured for
                        return ret;
                    }
                }
                stateTemplate.partNumber = newPartNumber;
                if(stateTemplate.partNumber.startsWith("33") || stateTemplate.partNumber.startsWith("30") || stateTemplate.partNumber.startsWith("24"))
                {
                    stateTemplate.MCU16Mode = true;
                }
                else if(stateTemplate.partNumber.startsWith("32"))
                {
                    stateTemplate.MCU16Mode = false;
                }
                if(stateTemplate.partNumber.startsWith("33") || stateTemplate.partNumber.startsWith("30"))
                {
                    stateTemplate.fullPartNumber = "dsPIC" + stateTemplate.partNumber;
                }
                else
                {
                    stateTemplate.fullPartNumber = "PIC" + stateTemplate.partNumber;
                }
            }
            else if(token.startsWith("-artifact="))            // Path to a image file that will be manipulated (can be a .hex or .blob file)
            {
                stateTemplate.artifactPath = Multifunction.TrimQuotes(args[inIndex].substring("-artifact=".length()));
                stateTemplate.hexPath = stateTemplate.artifactPath;
            }
            else if(token.startsWith("-project_name="))        // Makefile ${PROJECTNAME} value
            {
                stateTemplate.projectName = Multifunction.TrimQuotes(args[inIndex].substring("-project_name=".length()));
            }
            else if(token.startsWith("-linkscript="))          // Path to the linker script file we shall be modifying for this build operation
            {
                stateTemplate.linkScriptPath = Multifunction.TrimQuotes(args[inIndex].substring("-linkscript=".length()));

                // Fix up slashes and backslashes in path/filename parameters so they fit the running OS
                stateTemplate.linkScriptPath = FixSlashes(stateTemplate.linkScriptPath);
                stateTemplate.linkScriptPath = stateTemplate.linkScriptPath.replace(",--script=", "");  // Trim junk from Makefile string
            }
            else if(token.startsWith("-compiler_folder="))     // Folder containing targetted MPLAB XC16/XC32 compiler installation binaries
            {
                stateTemplate.compilerFolder = Multifunction.FixSlashes(Multifunction.TrimQuotes(args[inIndex].substring("-compiler_folder=".length())));
            }
            else if(token.startsWith("-java="))                // Path to the Java JRE to use when self-executing subsequent commands that require Java. This can be used to ensure the same JRE is used between operations.
            {
                stateTemplate.javaPath = Multifunction.TrimQuotes(args[inIndex].substring("-java=".length()));
            }
            else if(token.startsWith("-path_to_ide_bin="))                // Path to the Java JRE to use when self-executing subsequent commands that require Java. This can be used to ensure the same JRE is used between operations.
            {
                stateTemplate.pathToIDEBin = Multifunction.TrimQuotes(args[inIndex].substring("-path_to_ide_bin=".length()));
            }
            else if(token.startsWith("-last_page_erasable"))   // Boolean specifying that the last Flash page should be allowed to be erased while bootloading. Many devices have Flash Configuration words located on this page.
            {
                stateTemplate.allowLastPageErase = true;
            }
            else if(token.startsWith("-first_page_erasable"))  // Boolean specifying if the first Flash page should be allowed to be erased while bootloading. This page has the device reset vector and IVT on 16-bit devices.
            {
                stateTemplate.allowFirstPageErase = true;
            }
            else if(token.startsWith("-verbose"))              // Verbose boolean
            {
                stateTemplate.verbose = true;
            }
            else if(token.startsWith("-silent"))               // Silent boolean
            {
                stateTemplate.silent = true;
            }
            else if(token.startsWith("-fkeep_conf_restore"))     // Force Config Words to appear in __EZBL_TOOLS_COMPUTED_ERASE_RESTORE_TABLE - feature not implemented (intended to suppress/allow Config word restoration data when we don't really need it since the Config words aren't erasable)
            {
                stateTemplate.forceConfigEraseRestore = true;
            }
            else if(token.startsWith("-com="))                 // Communications port path to open when uploading a .blob or otherwise communicating with a running bootloader. Prefix with \\.\ when specifying COM10 and above on Windows.
            {
                stateTemplate.comPort = Multifunction.TrimQuotes(args[inIndex].substring("-com=".length()));
                if(stateTemplate.comPort.toLowerCase().startsWith("i2c"))  // If the communications port is an I2C interface, hardware flow control is implemented intrinsically by SCL clock stretching.
                {
                    stateTemplate.hardwareFlowControl = true;
                }
            }
            else if(token.startsWith("-baud="))                // Communications baud rate when uploading a .blob or otherwise communicating with a running bootloader.
            {
                stateTemplate.baudRate = Multifunction.TrimQuotes(args[inIndex].substring("-baud=".length()));
            }
            else if(token.startsWith("-slave_address="))       // I2C Slave address to target when communicating with an I2C bootloader -- Legacy, deprecated option
            {
                stateTemplate.slaveAddress = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-slave_address=".length())));
            }
            else if(token.startsWith("-i2c_address="))       // I2C Slave address to target when communicating with an I2C bootloader
            {
                stateTemplate.slaveAddress = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-i2c_address=".length())));
            }
            else if(token.startsWith("-auto_baud_retries="))
            {
                stateTemplate.autoBaudRetryLimit = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-auto_baud_retries=".length())));
            }
            else if(token.startsWith("-command_prefix="))
            {
                stateTemplate.commandPrefix = Multifunction.TrimQuotes(args[inIndex].substring("-command_prefix=".length()));
            }
            else if(token.startsWith("-skip_handshake"))
            {
                stateTemplate.skipComHandshake = true;
            }
            else if(token.startsWith("-min_free_psv="))
            {
                stateTemplate.minFreePSVSpace = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-min_free_psv=".length())));
            }
            else if(token.startsWith("-warn_free_psv="))
            {
                stateTemplate.warnFreePSVSpace = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-warn_free_psv=".length())));
            }
            else if(token.startsWith("-align="))
            {
                stateTemplate.hexAlign = Integer.decode(Multifunction.TrimQuotes(args[inIndex].substring("-align=".length())));
            }
            else if(token.startsWith("-"))
            {
                if(!stateTemplate.undecodedOptions.contains(newOption))
                {
                    stateTemplate.undecodedOptions += " " + newOption + " ";
                }
            }
            else
            {
                System.err.println("    EZBL: Command token \"" + args[inIndex] + "\" is unrecognized. Ignoring.");
            }
        }

        // Choose default path for ezbl_build_state.bin file if all we have is a -conf= option
        if(stateTemplate.ezblStateSavePath == null)
        {
            stateTemplate.ezblStateSavePath = GetStateFilePath(args);
        }

        // Choose default makefile for the given configuration if -conf= specified, but no -makefile=
        if((stateTemplate.makefilePath == null) && (stateTemplate.conf != null))
        {
            stateTemplate.makefilePath = "nbproject" + File.separator + "Makefile-" + stateTemplate.conf + ".mk";
        }

        // Decode MCU16Mode, eCore, and the path to the compiler .gld linker
        // script file from -mcpu option and compiler path
        if((stateTemplate.partNumber != null) && (stateTemplate.fullPartNumber != null))
        {
            // Decode the target -mcpu= part number name to a device architecture
            if(stateTemplate.partNumber.startsWith("30") || stateTemplate.partNumber.startsWith("33") || stateTemplate.partNumber.startsWith("24"))
            {
                stateTemplate.MCU16Mode = true;
                stateTemplate.fCore = stateTemplate.partNumber.startsWith("30")
                                      || stateTemplate.partNumber.startsWith("33F") || stateTemplate.partNumber.startsWith("33f")
                                      || stateTemplate.partNumber.startsWith("24F") || stateTemplate.partNumber.startsWith("24f");
                stateTemplate.eCore = stateTemplate.partNumber.startsWith("33E") || stateTemplate.partNumber.startsWith("33e")
                                      || stateTemplate.partNumber.startsWith("24E") || stateTemplate.partNumber.startsWith("24e");
                stateTemplate.cCore = stateTemplate.partNumber.startsWith("33C") || stateTemplate.partNumber.startsWith("33c");
                if(stateTemplate.compilerLinkScriptPath == null)
                {
                    try // Try ignoring compiler provided linker script if the current directory or ezbl_tools.jar directory contains a properly named "pPartNumber.gld" file, which we would use instead
                    {
                        File f = new File("p" + stateTemplate.partNumber + ".gld");
                        if(f.exists())
                            stateTemplate.compilerLinkScriptPath = f.getCanonicalPath();
                        else
                        {
                            f = new File(EZBLState.ezblToolsExecPath() + "/p" + stateTemplate.partNumber + ".gld");
                            if(f.exists())
                            {
                                stateTemplate.compilerLinkScriptPath = f.getCanonicalPath();
                            }
                        }
                    }
                    catch(IOException ex)
                    {   // Handling file errors later
                    }

                    if((stateTemplate.compilerFolder != null) && (stateTemplate.compilerLinkScriptPath == null))    // If local file didn't exist, try default XC16 path to processor .gld path given the part number
                    {
                        String familyPrefix = "PIC24F";
                        try
                        {   // Get "dsPIC3xx" or "PIC24x" family prefix string
                            familyPrefix = stateTemplate.fullPartNumber.startsWith("ds") ? stateTemplate.fullPartNumber.substring(0, 8) : stateTemplate.fullPartNumber.substring(0, 6);
                        }
                        catch(IndexOutOfBoundsException ex)
                        {   // Do nothing - going to handle file loading problems later
                        }
                        stateTemplate.compilerLinkScriptPath = FixSlashes(stateTemplate.compilerFolder + "/../support/" + familyPrefix + "/gld/p" + stateTemplate.partNumber + ".gld");
                        if(!(new File(stateTemplate.compilerLinkScriptPath).exists()))
                        {
                            stateTemplate.compilerLinkScriptPath = null;
                        }
                    }
                    if((stateTemplate.compilerFolder != null) && (stateTemplate.compilerLinkScriptPath == null))    // Still not found, try searching all folders recursively in support for the .gld file
                    {
                        List<File> gldMatches = Multifunction.FindFiles(FixSlashes(stateTemplate.compilerFolder + "/../support/"), "p" + stateTemplate.partNumber + ".gld", true);
                        if((gldMatches != null) && (gldMatches.size() >= 1))
                        {
                            stateTemplate.compilerLinkScriptPath = gldMatches.get(0).getPath();
                        }
                    }
                    if((stateTemplate.compilerFolder != null) && (stateTemplate.compilerLinkScriptPath == null))    // Still not found, try searching all folders recursively in any compiler folder and with a wildcard prefix character instead of 'p' and case insensitive .gld extension
                    {
                        List<File> gldMatches = Multifunction.FindFilesRegEx(FixSlashes(stateTemplate.compilerFolder + "/../"), "." + stateTemplate.partNumber + "\\.[gG][lL][dD]", true);
                        if((gldMatches != null) && (gldMatches.size() >= 1))
                        {
                            stateTemplate.compilerLinkScriptPath = gldMatches.get(0).getPath();
                        }
                    }
                    if((stateTemplate.compilerFolder != null) && (stateTemplate.compilerLinkScriptPath == null))    // Still not found, report an error and continue. Only when building a bootloader is a compiler provided linker script really needed.
                    {
                        System.err.println("EZBL: could not find .gld linker script for '" + raw_mcpu + "'. Ensure '-compiler_folder=' option is set to point to a valid XC16/XC32 bin folder and a valid target processor for the compiler is selected, this device is supported by EZBL and ezbl_tools.jar make/command parameters are correct.");
                    }
                }
            }
            else if(stateTemplate.partNumber.startsWith("32MM") || stateTemplate.partNumber.startsWith("32mm"))
            {
                stateTemplate.MCU16Mode = false;
                stateTemplate.compilerLinkScriptPath = null;
                stateTemplate.fCore = false;
                stateTemplate.eCore = false;
                stateTemplate.cCore = false;
                stateTemplate.mmCore = true;
            }
        }

        // Set default temporaries path to the same folder this .jar sits in if
        // the command line options didn't set it
        if(stateTemplate.temporariesPath == null)
        {
            stateTemplate.temporariesPath = CommandAndBuildState.class.getProtectionDomain().getCodeSource().getLocation().getPath();
            try
            {
                stateTemplate.temporariesPath = URLDecoder.decode(stateTemplate.temporariesPath, "UTF-8");
            }
            catch(UnsupportedEncodingException ex)
            {
            }
            stateTemplate.temporariesPath = new File(stateTemplate.temporariesPath).getParent();

            // Check for null again in case if there is no parent directory
            if(stateTemplate.temporariesPath == null)
            {
                stateTemplate.temporariesPath = "";
            }
        }

        // Sanitize temporaries folder by adding directory seperator if it is
        // missing a trailing slash (we assume it always has one elsewhere)
        if(!stateTemplate.temporariesPath.isEmpty())
        {
            // Add trailing slash if there isn't one already
            if(!stateTemplate.temporariesPath.substring(stateTemplate.temporariesPath.length() - 1).equals(File.separator))
            {
                stateTemplate.temporariesPath += File.separator;
            }
        }

        // Read edc (Essential Device Characteristics) information for the given
        // device from the MPLAB X crownking.edc.jar file
        // (ex: C:\Program Files (x86)\Microchip\MPLABX\mplab_ipe\lib\crownking.edc.jar\content\edc\dsPIC33\DSPIC33EP512GM710.PIC)
        if((stateTemplate.partNumber != null) && !stateTemplate.edcInfoLoaded)
        {
            // Decide if this is a slave CPU core or not. For Slave cores, lets auto-disable IVT remapping.
            if(stateTemplate.fullPartNumber.matches("[^S]*?S[0-9]$"))   // Ex: dsPIC33CH128MP508S1 for Slave Core 1
            {
                // Looks like a Slave core to me; avoid making a regular Bootloader with ISR remapping
                stateTemplate.remapISRThroughIGT = false;
            }

            if(stateTemplate.pathToIDEBin != null)
            {
                stateTemplate.runtimeClassPaths = new ArrayList<String>();
                if(Main.LoadRuntimeJar("crownking.edc.jar"))
                    stateTemplate.runtimeClassPaths.add("crownking.edc.jar");
                else if(Main.LoadRuntimeJar(EZBLState.ezblToolsExecPath() + "/crownking.edc.jar"))
                    stateTemplate.runtimeClassPaths.add(EZBLState.ezblToolsExecPath() + "/crownking.edc.jar");
                else if(Main.LoadRuntimeJar(stateTemplate.pathToIDEBin + "../mplablibs/modules/ext/crownking.edc.jar"))
                    stateTemplate.runtimeClassPaths.add(stateTemplate.pathToIDEBin + "../mplablibs/modules/ext/crownking.edc.jar");
                else
                    System.err.println("ezbl_tools: could not locate crownking.edc.jar database containing device information.");

                if(Main.LoadRuntimeJar("crownking.common.jar"))
                    stateTemplate.runtimeClassPaths.add("crownking.common.jar");
                else if(Main.LoadRuntimeJar("crownking.jar"))
                    stateTemplate.runtimeClassPaths.add("crownking.jar");
                else if(Main.LoadRuntimeJar(EZBLState.ezblToolsExecPath() + "/crownking.common.jar"))
                    stateTemplate.runtimeClassPaths.add(EZBLState.ezblToolsExecPath() + "/crownking.common.jar");
                else if(Main.LoadRuntimeJar(EZBLState.ezblToolsExecPath() + "/crownking.jar"))
                    stateTemplate.runtimeClassPaths.add(EZBLState.ezblToolsExecPath() + "/crownking.jar");
                else if(Main.LoadRuntimeJar(stateTemplate.pathToIDEBin + "../mplablibs/modules/ext/crownking.common.jar"))              // Needed for MPLAB X IDE v4.01+
                    stateTemplate.runtimeClassPaths.add(stateTemplate.pathToIDEBin + "../mplablibs/modules/ext/crownking.common.jar");
                else if(Main.LoadRuntimeJar(stateTemplate.pathToIDEBin + "../mplablibs/modules/ext/crownking.jar"))                     // All MPLAB X IDE versions up to at least v4.01
                    stateTemplate.runtimeClassPaths.add(stateTemplate.pathToIDEBin + "../mplablibs/modules/ext/crownking.jar");
                else
                    System.err.println("ezbl_tools: could not locate crownking.common.jar or cronwking.jar files. This version of EZBL may not support this version of MPLAB X.");

                if(Main.LoadRuntimeJar("jna.jar"))
                    stateTemplate.runtimeClassPaths.add("jna.jar");
                else if(Main.LoadRuntimeJar(EZBLState.ezblToolsExecPath() + "/jna.jar"))                                                 // All MPLAB X IDE versions up to at least v4.01
                    stateTemplate.runtimeClassPaths.add(EZBLState.ezblToolsExecPath() + "/jna.jar");
                else if(Main.LoadRuntimeJar(stateTemplate.pathToIDEBin + "../mplablibs/modules/ext/jna.jar"))                           // All MPLAB X IDE versions up to at least v4.01
                    stateTemplate.runtimeClassPaths.add(stateTemplate.pathToIDEBin + "../mplablibs/modules/ext/jna.jar");
                else if(stateTemplate.verbose)
                    System.err.println("ezbl_tools: could not locate jna.jar.");
            }
            PrintStream originalStdout = System.out;
            PrintStream originalStderr = System.err;
            PrintStream nullOutput = null;
            if(!stateTemplate.verbose)
            {
                try
                {
                    nullOutput = new PrintStream(EZBLState.nullFile());
                    System.setOut(nullOutput);
                    System.setErr(nullOutput);
                }
                catch(FileNotFoundException ex)
                {
                    System.setErr(System.out);      // At least keep text from appearing in red within the IDE by mapping stderr to stdout. stdout text is ordinary black.
                    System.err.println("Could not open a file 'NUL' for discarding messages on your OS. You may see EDC messages that can be ignored.");
                }
            }
            PICFactory pf = PICFactory.getInstance();
            try
            {
                NamedNodeMap attribs;
                NodeList nodeList;
                Node node;
                Node configSectorNode;
                MemoryPartition codePartition;

                com.microchip.crownking.edc.PIC pic = pf.get(stateTemplate.fullPartNumber); // use p pf.release(p) p = null
                Document doc = pic.getDoc();

                nodeList = doc.getChildNodes();
                //LinkedHashMap<String, DecodedNodeList> tags = IndexNodesByTag(nodeList, true);        // For future use, avoiding use of crownking.common.jar/crownking.jar classes
                DevRegisters regs = IndexSFRDCRNodes(nodeList);

                // Enumerate elements in edc:ProgramSpace and edc:ProgramSubspace (if applicable), or edc:PhysicalSpace for PIC32,
                // so we know the addresses for Reset, IVT, _reserved (if present), AIVT (if present), code, and WORMs
                String tag = stateTemplate.MCU16Mode ? "edc:ProgramSpace" : "edc:PhysicalSpace";
                NodeList programSpace = doc.getElementsByTagName(tag).item(0).getChildNodes();
                List<Node> flatProgramSpaceNodes = new ArrayList<Node>();

                // Obtain a flat view of the program space without dual partition variations. This is for Single Partition mode only.
                for(int j = 0; j < programSpace.getLength(); j++)
                {
                    Node beginAddrNode, endAddrNode, partitionmodeNode;

                    node = programSpace.item(j);
                    if(!node.hasAttributes())
                    {
                        continue;
                    }
                    attribs = node.getAttributes();
                    beginAddrNode = attribs.getNamedItem("edc:beginaddr");
                    endAddrNode = attribs.getNamedItem("edc:endaddr");
                    if((beginAddrNode == null) || (endAddrNode == null))   // Skip over edc:ProgramSpace elements that don't have any begin or end address. These are things like edc:AltVectorArea, which are for relocatable alternate interrupt vector tables which don't have meaning to us anyway.
                    {
                        continue;
                    }
                    if(node.getNodeName().compareToIgnoreCase("edc:ProgramSubspace") != 0)
                    {
                        flatProgramSpaceNodes.add(node);
                    }
                    else    // Multi-partition device, enumerate the items in the edc:ProgramSubspace instead
                    {
                        partitionmodeNode = attribs.getNamedItem("edc:partitionmode");
                        if(partitionmodeNode != null)
                        {
                            if(partitionmodeNode.getNodeValue().equalsIgnoreCase("dual") != stateTemplate.dualPartitionMode)
                            {
                                continue;   // Wrong program subspace to parse for the selected partition mode, so skip over this node tree
                            }

                            // If we are a dual partition device, only look at the first partition view. We don't need the second partition addresses since they will always mirror the first but with a 0x400000 address offset, which is only important durring programming
                            if(stateTemplate.dualPartitionMode && (attribs.getNamedItem("edc:id") != null))
                            {
                                if(!attribs.getNamedItem("edc:id").getNodeValue().equalsIgnoreCase("first"))
                                {
                                    continue;
                                }
                            }
                        }
                    }

                    NodeList programSubspace = node.getChildNodes();
                    for(int k = 0; k < programSubspace.getLength(); k++)
                    {
                        node = programSubspace.item(k);
                        if(!node.hasAttributes())
                        {
                            continue;
                        }
                        attribs = node.getAttributes();
                        beginAddrNode = attribs.getNamedItem("edc:beginaddr");
                        endAddrNode = attribs.getNamedItem("edc:endaddr");
                        if((beginAddrNode == null) || (endAddrNode == null))   // Skip over edc:ProgramSpace elements that don't have any begin or end address. These are things like edc:AltVectorArea, which are for relocatable alternate interrupt vector tables which don't have meaning to us anyway.
                        {
                            continue;
                        }
                        flatProgramSpaceNodes.add(node);
                    }
                }

                for(int i = 0; i < flatProgramSpaceNodes.size(); i++)
                {
                    Node regionIDNode;
                    PairWithText item = new PairWithText();

                    node = flatProgramSpaceNodes.get(i);
                    attribs = node.getAttributes();
                    item.first = Long.decode(attribs.getNamedItem("edc:beginaddr").getNodeValue());
                    item.second = Long.decode(attribs.getNamedItem("edc:endaddr").getNodeValue());
                    regionIDNode = attribs.getNamedItem("edc:regionid");
                    item.text = (regionIDNode == null) ? "" : regionIDNode.getNodeValue();

                    // Test for Flash based Config Words (will have edc:regionid=cfgmem with region start address < 0x800000)
                    if((item.text.equalsIgnoreCase("cfgmem")) && (item.first < 0x800000))
                    {
                        stateTemplate.hasFlashConfigWords = true;
                        stateTemplate.configWordsRange = new AddressRange(item.first, item.second);
                    }

                    if(item.text.equalsIgnoreCase("cfgmem") || item.text.equalsIgnoreCase("bootcfg"))
                    {   // Found 'cfgmem' Config words. This may be either an
                        // edc:WORMHoleSector or edc:ConfigFuseSector. However,
                        // both will contain dcrs with edc:DCRDef nodes in them.
                        // For these, we want to parse out the individual
                        // register names since the compiler linker scripts
                        // treat them as special named memory regions.
                        List<PairWithText> pairs = EnumDCRDefs(node);
                        stateTemplate.deviceConfigWords.addAll(pairs);
                        for(int k = 0; k < pairs.size(); k++)
                        {
                            stateTemplate.devProgramSpaces.add(pairs.get(k));
                        }
                    }
                    else if(item.text.equalsIgnoreCase("testmem") || item.text.equalsIgnoreCase("devid") || item.text.equalsIgnoreCase("cfgwormmem"))
                    {// Skip stuff in testmem (ex: bootcfg), devid, cfgwormmem
                        continue;
                    }
                    else
                    {
                        stateTemplate.devProgramSpaces.add(item);
                    }
                }

                if(stateTemplate.verbose)
                {
                    for(PairWithText p : stateTemplate.devProgramSpaces)   // For Debugging Only
                    {
                        System.err.printf("devProramSpace: [0x%1$06X, 0x%2$06X) %3$s\n", p.first, p.second, p.text);
                    }
                }

                // Get a reference to the main program codePartition. On Dual
                // Partition devices, this will be the largest Single Partition
                // mode layout selected.
//                if(tags.containsKey("edc:ProgramSubspace"))
//                {
//                    tags.get("edc:ProgramSubspace").
//                }
                nodeList = doc.getElementsByTagName("edc:ProgramSubspace");
                if(nodeList.getLength() != 0)
                {
                    codePartition = new MemoryPartition(nodeList.item(0), pic.getMemTraits());
                    for(int i = 1; i < nodeList.getLength(); i++)
                    {
                        MemoryPartition testSubSpace = new MemoryPartition(nodeList.item(i), pic.getMemTraits());
                        // Preferably, obtain the proper partition reference using the edc:id field, which can be "only", "first", "second", or probably not defined
                        if(testSubSpace.getID() != null)
                        {
                            if((stateTemplate.dualPartitionMode && testSubSpace.getID().equalsIgnoreCase("first"))
                               || (!stateTemplate.dualPartitionMode && testSubSpace.getID().equalsIgnoreCase("only")))
                            {
                                codePartition = testSubSpace;
                                break;
                            }
                        }
                        // No ID returned or wrong partition found, so fall back on default handling of just the primary ("only") partition
                        if(testSubSpace.hasCode())
                        {
                            if(!codePartition.hasCode())
                            {
                                codePartition = testSubSpace;
                            }
                            else if(testSubSpace.getCodeRange().second - testSubSpace.getCodeRange().first > codePartition.getCodeRange().second - codePartition.getCodeRange().first)
                            {
                                codePartition = testSubSpace;
                            }
                        }
                    }
                }
                else
                {
                    codePartition = pic.getMainPartition();
                }

                // Get the Flash Word Size
                try
                {
                    node = doc.getElementsByTagName("edc:CodeMemTraits").item(0);
                    stateTemplate.flashWordSize = Integer.decode(GetAttr(node, "edc:wordsize", "0x4"));
                    if((stateTemplate.eCore || stateTemplate.cCore) && (stateTemplate.flashWordSize < 4))   // Probably unneeded, but just in case, set a min limit of 4 on dsPIC33E/dsPIC33C/PIC24E devices
                    {
                        stateTemplate.flashWordSize = 0x4;
                    }
                    else if(stateTemplate.fCore && !stateTemplate.fullPartNumber.contains("K") && !stateTemplate.fullPartNumber.startsWith("dsPIC30"))    // A number of devices like PIC24FJ128GC010 have CodeMemTraits<wordsize> = 0x4, even though they are PIC24FJ/dsPIC33FJ parts with a programmable size of 0x2 instead.
                    {
                        if(!regs.RegByName.containsKey("NVMADR") && !regs.RegByName.containsKey("NVMADDR") && !regs.RegByName.containsKey("NVMADRL") && !regs.RegByName.containsKey("NVMADDRL"))
                            stateTemplate.flashWordSize = 0x2;
                    }
                }
                catch(Exception e)
                {
                    stateTemplate.flashWordSize = 0x4;  // Assume 0x4 addresses per flash word (48-bit) if we can't find it in EDC
                }

                // Default value in case if edc doesn't have "erasepagesize"
                // defined. Choose 1024 instructions/0x800 address for
                // PIC24E/dsPIC33E devices, and 512 instructions/0x400 addresses
                // for all others (PIC24F/PIC24H/dsPIC33F devices). Most older
                // devices use 0x400 addresses but PIC24E/dsPIC33E usually uses
                // 0x800 for older and 0x400 for newer. Newer devices, may have
                // the correct value explicitly available via edc:erasepagesize,
                // which will override this default.
                stateTemplate.eraseBlockSize = stateTemplate.eCore ? 1024 : 512;

                // Lookup erase block size (edc:erasepagesize). This needs to be
                // done early so we can mask convert some addresses later to
                // page boundaries.
                nodeList = doc.getElementsByTagName("edc:Programming");
                attribs = nodeList.item(0).getAttributes();
                node = attribs.getNamedItem("edc:erasepagesize");
                if(node == null)   // No edc:erasepagesize found
                {
                    // Look up SFRDef's eds:_modsrc to find exceptions (Top
                    // Level DOS's). This enumerates all SFRs unless the
                    // exception is found, so is probably fairly expensive
                    // computationally.
                    nodeList = doc.getElementsByTagName("edc:SFRDef");
                    for(int i = 0; i < nodeList.getLength(); i++)
                    {
                        node = nodeList.item(i);
                        attribs = node.getAttributes();
                        node = attribs.getNamedItem("edc:_modsrc");
                        if(node == null)
                        {
                            continue;
                        }

                        String nodeValue = node.getNodeValue().toUpperCase();
                        if(nodeValue.contains("DOS-02073") //   SAAA, PIC24FJ1024GB610
                           || nodeValue.contains("DOS-02786")) // SABC, PIC24FJ256GA705
                        {
                            stateTemplate.eraseBlockSize = 1024;
                            break;
                        }
                    }
                }
                else    // Found edc:erasepagesize, so use it directly
                {
                    stateTemplate.eraseBlockSize = Integer.decode(node.getNodeValue());
                }
                stateTemplate.eraseBlockSizeBytes = stateTemplate.MCU16Mode ? stateTemplate.eraseBlockSize * 3 : stateTemplate.eraseBlockSize;
                stateTemplate.eraseBlockSizeAddresses = stateTemplate.MCU16Mode ? stateTemplate.eraseBlockSize * 2 : stateTemplate.eraseBlockSize;

                // Lookup DEVREV/DEVID/Device ID address, mask, and value
                stateTemplate.deviceIDAddress = stateTemplate.MCU16Mode ? 0xFF0000 : 0xBF803B20;    // 0xBF803B20 is for PIC32MM0064GPL036 family
                if(stateTemplate.mmCore && stateTemplate.partNumber.toLowerCase().contains("gpm"))
                    stateTemplate.deviceIDAddress = 0xBF803662; // PIC32MM0256GPM064 Family
                stateTemplate.deviceIDMask = stateTemplate.MCU16Mode ? 0x00FFFFFF : 0x0FFFFFFF;
                stateTemplate.deviceIDValue = 0x00000000;
                stateTemplate.deviceRevAddress = stateTemplate.MCU16Mode ? 0xFF0002 : stateTemplate.deviceIDAddress;
                stateTemplate.deviceRevMask = stateTemplate.MCU16Mode ? 0x00FFFFFF : 0xF0000000;
                nodeList = doc.getElementsByTagName("edc:DeviceIDSector");
                if(nodeList.getLength() == 0)
                {
                    if(!stateTemplate.silent)
                        System.err.println("EZBL: Cannot find edc:DeviceIDSector information in crownking.edc.jar; this EZBL version does not support the MPLAB X IDE version and target processor combination.");
                }
                else
                {
                    Node deviceIDSectorNode = nodeList.item(0);
                    attribs = deviceIDSectorNode.getAttributes();
                    if(attribs == null)
                    {
                        if(stateTemplate.verbose)
                            System.err.println("EZBL: Cannot obtain attributes from edc:DeviceIDSector.");
                    }
                    else
                    {
                        stateTemplate.deviceIDAddress = Long.decode(attribs.getNamedItem("edc:beginaddr").getNodeValue());
                        stateTemplate.deviceRevAddress = stateTemplate.deviceIDAddress + 0x2;
                        long devMaskEncoded = Long.decode(attribs.getNamedItem("edc:mask").getNodeValue());
                        long devEncoded = Long.decode(attribs.getNamedItem("edc:value").getNodeValue());
                        stateTemplate.deviceIDValue = 0;
                        int outBit = 0;
                        for(int inBit = 0; inBit < 32; inBit++)
                        {
                            if((devMaskEncoded & (0x1L << inBit)) != 0)
                            {
                                stateTemplate.deviceIDValue |= ((devEncoded & (0x1L << inBit)) >> inBit) << outBit;
                                outBit++;
                            }
                        }
                    }
                }

                // Get Executable Program Flash dimensions
                Pair<Long, Long> pair;
                stateTemplate.devNVGeometry = new ArrayList<AddressRange>();
                if(codePartition.hasCode())
                {
                    pair = codePartition.getCodeRange();
                    stateTemplate.mainExecutionRange = new AddressRange(pair.first, pair.second);

                    // Align start address to an erase page size (so we can get the reset and interrupt vectors)
                    pair.first &= ~(stateTemplate.eraseBlockSizeAddresses - 0x2);

                    // Align the end address to an erase page size if there are Flash Config words
                    if(stateTemplate.hasFlashConfigWords)
                    {
                        pair.second |= stateTemplate.eraseBlockSizeAddresses - 0x2;
                        pair.second += 0x2;
                    }

                    stateTemplate.devNVGeometry.add(new AddressRange(pair.first, pair.second));
                    stateTemplate.mainFlashRange = new AddressRange(pair.first, pair.second);
                }
                if(codePartition.hasAuxCode()) // Add Aux Flash to list
                {
                    pair = codePartition.getAuxCodeRange();

                    // Align start address and end address to page boundaries
                    pair.first &= ~(stateTemplate.eraseBlockSizeAddresses - 0x2);
                    pair.second |= stateTemplate.eraseBlockSizeAddresses - 0x2;
                    pair.second += 0x2;

                    stateTemplate.hasAuxFlash = true;
                    stateTemplate.devNVGeometry.add(new AddressRange(pair.first, pair.second));
                    stateTemplate.auxFlashRange = new AddressRange(pair.first, pair.second);
                }
                if(codePartition.hasDCR()) // Add Fuse based Config Words and WORMHoleSector Flash Config Word addresses
                {
                    pair = codePartition.getDCRRange();
                    if(!stateTemplate.hasFlashConfigWords)
                    {
                        stateTemplate.allowLastPageErase = true;    // Might as well allow us to erase the last page if Config Words are in Fuses separate from the Flash array
                        stateTemplate.devNVGeometry.add(new AddressRange(pair.first, pair.second));
                    }
                    stateTemplate.configWordsRange = new AddressRange(pair.first, pair.second);
                }

                // Get RAM dimensions (excludes SFRs, but includes X RAM, Y RAM, and Dual Port RAM (if applicable))
                pair = codePartition.getXMemoryRange();
                stateTemplate.devDataSpace = new AddressRange(pair.first, pair.second);

                // Find location of FBOOT, if this device has an FBOOT and FBOOT wasn't manually declared on the command line
                if(!FBOOTAddressOverride)
                {
                    stateTemplate.FBOOTaddress = -1;
                    if(regs.RegByName.containsKey("FBOOT"))
                        stateTemplate.FBOOTaddress = regs.RegByName.get("FBOOT").addr.intValue();
                }

                // Lookup Config word address block and certain important fuse locations, like Code Protect bits
                nodeList = doc.getElementsByTagName("edc:WORMHoleSector");
                int partitionNodeIndex = 0;
                if(nodeList.getLength() == 0)
                {
                    nodeList = doc.getElementsByTagName("edc:ConfigFuseSector");
                }
                if(nodeList.getLength() > 1)
                {
                    long maxAddress = 0;
                    Node n;
                    for(int i = 0; i < nodeList.getLength(); i++)
                    {
                        n = nodeList.item(i);
                        if(!n.hasAttributes())
                        {
                            continue;
                        }
                        NamedNodeMap a = n.getAttributes();
                        if(a.getNamedItem("edc:beginaddr") != null)
                        {
                            long j = Long.decode(a.getNamedItem("edc:beginaddr").getNodeValue());

                            // Ideally, choose the proper node based on it existing in the code partition
                            if((j >= stateTemplate.mainFlashRange.startAddress) && (j < stateTemplate.mainFlashRange.endAddress))
                            {
                                partitionNodeIndex = i;
                                break;
                            }

                            // Default huristic of finding the highest addressed edc:WORMHoleSector node that is not in test space
                            if((j & 0x3FFFFF) > maxAddress)
                            {
                                maxAddress = j;
                                partitionNodeIndex = i;
                            }
                        }
                    }
                }
                configSectorNode = nodeList.item(partitionNodeIndex);
                if(configSectorNode != null)
                {
                    // Find Reserved Bit and Code-Protect bit(s). On Dual partition devices, we will assume only processing the primary single partition setup.
                    List<ConfigFieldLocation> locations = LookupConfigField(configSectorNode, "SIGN");
                    if(locations.isEmpty())
                    {
                        locations = LookupConfigField(configSectorNode, "FSIGN");
                    }
                    if(locations.size() >= 1)
                    {
                        int prefered = 0;

                        // For devices which have multiple SIGN bits, try to figure out which one is appropriate
                        for(int i = 0; i < locations.size(); i++)
                        {
                            if(locations.get(i).bitmask == 0x8000)
                            {
                                prefered = i;
                                break;
                            }
                        }

                        stateTemplate.reservedBitMask = locations.get(prefered).bitmask;
                        stateTemplate.reservedBitAddress = locations.get(prefered).address;
                        stateTemplate.reserveBitConfigWordName = locations.get(prefered).configWordName;
                    }

                    locations = LookupConfigField(configSectorNode, "GCP");    // General Code-Protect
                    if(!locations.isEmpty())
                    {
                        stateTemplate.codeProtectMask = locations.get(0).bitmask;
                        stateTemplate.codeProtectAddress = locations.get(0).address;
                        stateTemplate.codeProtectConfigWordName = locations.get(0).configWordName;
                    }
                    else    // Didn't find GCP bit, must be a newer multi-segment security implementation
                    {
                        stateTemplate.codeProtectMask = 0;
                        locations = LookupConfigField(configSectorNode, "BSS");    // Boot Segment
                        if(!locations.isEmpty())
                        {
                            stateTemplate.codeProtectMask |= locations.get(0).bitmask;
                            stateTemplate.codeProtectAddress = locations.get(0).address;
                            stateTemplate.codeProtectConfigWordName = locations.get(0).configWordName;
                        }
                        locations = LookupConfigField(configSectorNode, "GSS");    // General Segment
                        if(!locations.isEmpty())
                        {
                            stateTemplate.codeProtectMask |= locations.get(0).bitmask;
                            stateTemplate.codeProtectAddress = locations.get(0).address;
                            stateTemplate.codeProtectConfigWordName = locations.get(0).configWordName;
                        }
                        locations = LookupConfigField(configSectorNode, "CSS");    // Configuration Segment
                        if(!locations.isEmpty())
                        {
                            stateTemplate.codeProtectMask |= locations.get(0).bitmask;
                            stateTemplate.codeProtectAddress = locations.get(0).address;
                            stateTemplate.codeProtectConfigWordName = locations.get(0).configWordName;
                        }
                    }

                    stateTemplate.deviceBACKBUGAddress = 0;
                    stateTemplate.deviceBACKBUGMask = 0;
                    stateTemplate.BACKBUGConfigWordName = null;
                    locations = LookupConfigField(configSectorNode, "BACKBUG");     // Background Debugger Disable bit
                    if(locations.isEmpty())
                    {
                        locations = LookupConfigField(configSectorNode, "BKBUG");   // Search for "BKBUG" alternate name (ex: dsPIC33EP256GM106)
                    }
                    if(locations.isEmpty())
                    {
                        locations = LookupConfigField(configSectorNode, "DEBUG");   // Search for "DEBUG" alternate name (ex: dsPIC33EP256GM106)
                    }
                    if(!locations.isEmpty())
                    {
                        stateTemplate.deviceBACKBUGAddress = locations.get(0).address;
                        stateTemplate.deviceBACKBUGMask = (int)locations.get(0).bitmask;
                        stateTemplate.BACKBUGConfigWordName = locations.get(0).configWordName;
                    }
                }

                // Get implemented Interrupt Vector list
                nodeList = doc.getElementsByTagName("edc:InterruptList");

                if(nodeList.getLength() != 0)
                {
                    NodeList interruptNodes = nodeList.item(0).getChildNodes();

                    stateTemplate.ivtVectorsImplemented = 0;
                    for(int i = 0; i < interruptNodes.getLength(); i++)
                    {
                        Node interrupt = interruptNodes.item(i);
                        if(!interrupt.hasAttributes())
                        {
                            continue;
                        }
                        stateTemplate.ivtVectorsImplemented++;
                    }
                }
                else
                {
                    System.setErr(originalStderr);
                    if(stateTemplate.MCU16Mode && !stateTemplate.legacyClassic)
                    {
                        System.err.println("EZBL: Could not find edc:InterruptList in device database. This version of ezbl_tools.jar is not compatible with this version of MPLAB X IDE.");
                    }
                }
                stateTemplate.edcInfoLoaded = true;

                pf.release(pic);
                pic = null;
            }
            catch(Exception ex)
            {
                System.setErr(originalStderr);
                System.err.println("Exception reading device database: " + ex.getMessage());
                ex.printStackTrace();
            }
            pf = null;

            System.setOut(originalStdout);
            System.setErr(originalStderr);
            if(nullOutput != null)
            {
                nullOutput.close();
                nullOutput = null;
            }
        }

        // Set or validate the warnFreePSVSpace/minFreePSVSpace values to be 1/2
        // the maximum possible size and 1/4 the maximum possible size,
        // respectively, assuming not explicitly set on the command line.
        long maxPossiblePSVSpace = (stateTemplate.mainFlashRange != null) ? stateTemplate.mainFlashRange.endAddress - stateTemplate.mainFlashRange.startAddress : 0x8000L;
        if(maxPossiblePSVSpace > 0x8000L)
        {
            maxPossiblePSVSpace = 0x8000;
        }
        int reasonablePSVMin = (int)(maxPossiblePSVSpace - 0x1800) / 4;
        if(reasonablePSVMin < stateTemplate.eraseBlockSizeAddresses)
            reasonablePSVMin = stateTemplate.eraseBlockSizeAddresses;
        if(stateTemplate.minFreePSVSpace == 0)
        {
            stateTemplate.minFreePSVSpace = reasonablePSVMin;
        }
        if(stateTemplate.warnFreePSVSpace == 0)
        {
            stateTemplate.minFreePSVSpace = reasonablePSVMin * 2;
        }
        if(stateTemplate.minFreePSVSpace > (int)(maxPossiblePSVSpace - 0x1800))    // Limit checking when device doesn't have much flash to begin with. There has to be at least some space for the Bootloader code.
            stateTemplate.minFreePSVSpace = (int)(maxPossiblePSVSpace - 0x1800);
        if(stateTemplate.warnFreePSVSpace > (int)(maxPossiblePSVSpace - 0x1000))
            stateTemplate.warnFreePSVSpace = (int)(maxPossiblePSVSpace - 0x1000);

        // Expand/pad encryption password and salt it, if provided
        if(stateTemplate.encryptionPassword != null)
        {
            int i;
            int chunkSize;
            byte passBytes[];

            stateTemplate.encryptionKey = new byte[16];

            // Expand the password string to a byte array, concatenating onto itself
            // until 16 bytes exist
            try
            {
                passBytes = stateTemplate.encryptionPassword.getBytes("UTF-8");
            }
            catch(UnsupportedEncodingException ex)
            {
                passBytes = stateTemplate.encryptionPassword.getBytes();
            }
            i = 0;
            while(i < stateTemplate.encryptionKey.length)
            {
                chunkSize = stateTemplate.encryptionKey.length - i;
                if(chunkSize > passBytes.length)
                {
                    chunkSize = passBytes.length;
                }
                System.arraycopy(passBytes, 0, stateTemplate.encryptionKey, i, chunkSize);
                i += chunkSize;
            }

            // Salt the password
            if(stateTemplate.encryptionSaltString == null)
            {
                stateTemplate.encryptionSalt = defaultEncryptionSalt;
            }
            for(i = 0; i < stateTemplate.encryptionKey.length; i++)
            {
                stateTemplate.encryptionKey[i] ^= stateTemplate.encryptionSalt[i];
            }
        }

        stateTemplate.parseOkay = true;
        return stateTemplate;
    }

    public static String GetStateFilePath(String[] args)
    {
        String ret = "ezbl_integration" + File.separator + "ezbl_build_state.bin";  // Default, if nothing else given to key on

        if(args != null)
        {
            for(int inIndex = 0; inIndex < args.length; inIndex++)
            {
                // Get argument token, normalize to lowercase and if it starts with '--', make it only have one '-'.
                if(args[inIndex].startsWith("--"))
                {
                    args[inIndex] = args[inIndex].substring(1);
                }
                String token = args[inIndex].toLowerCase();

                if(token.startsWith("-state="))
                {
                    return Multifunction.TrimQuotes(args[inIndex].substring("-state=".length()));
                }
                else if(token.startsWith("-artifact="))
                {
                    String artifact = Multifunction.TrimQuotes(args[inIndex].substring("-artifact=".length()));
                    File f = new File(artifact);
                    String parentFolder = f.getParent();
                    if(parentFolder != null)
                    {
                        ret = parentFolder + File.separator + "ezbl_build_state.bin";
                    }
                }
            }
        }

        return ret;
    }

    /**
     * Searches an XML Document object for a Node with the specified tagName
     * containing an specified attribute and with a particular value set for the
     * attribute.
     *
     * @param xmlDocument    XML document to search within
     * @param tagName        XML Tag to filter by
     * @param attributeName  Name of the XML tag's attribute to look for.
     * @param attributeValue Value of the specified attribute to match against.
     *
     * @return If an exact match is not found, null. If more than one match
     *         exists, only the first one found is returned. The full node
     *         reference is returned.
     */
    static Node FindNodeWithSpecificedAttribute(Document xmlDocument, String tagName, String attributeName, String attributeValue)
    {
        NodeList nodeList = xmlDocument.getElementsByTagName(tagName);
        if(nodeList.getLength() == 0)
        {
            return null;
        }
        for(int i = 0; i < nodeList.getLength(); i++)
        {
            NamedNodeMap a = nodeList.item(i).getAttributes();
            Node n = a.getNamedItem(attributeName);
            if(n == null)
            {
                continue;
            }
            if(n.getNodeValue().equals(attributeValue))
            {
                return nodeList.item(i);
            }
        }
        return null;
    }

//    // Returns a list of everything that lives in program memory (including test space items, config words in either Flash or Fuses, etc.) for the given partition view.
//    static List<Object> EnumProgramMemorySpace(crownking.edc.PIC pic, PartitionModeFlavor partitionView)
//    {
//        List<Object> ret = new ArrayList<Object>();
//
//        List<Object> programSpaces = pic.getProgramSpace().getProgramSubspaceOrCodeSectorOrAuxCodeSector();
//        for (Object o : programSpaces)
//        {
//            if (o instanceof ProgramSubspace)
//            {
//                ProgramSubspace pgm = (ProgramSubspace)o;
//                if (pgm.getPartitionmode() != partitionView)// Filter out DUAL or SIGNLE partition elements that don't match our view
//                {
//                    continue;
//                }
//                if ((partitionView == DUAL) && !pgm.getId().equalsIgnoreCase("first"))  // Filter out the "second" partition and only return the "first" partition.
//                {
//                    continue;
//                }
//                for (java.lang.reflect.Field i : pgm.getClass().getFields())
//                {
//                    try
//                    {
//                        Object newObject = i.get(pgm);
//                        if (newObject == null)
//                        {
//                            continue;
//                        }
//                        if ((newObject instanceof String) || (newObject instanceof PartitionModeFlavor))
//                        {
//                            continue;
//                        }
//
//                        ret.add(i.get(pgm));
//                    }
//                    catch (Exception ex)
//                    {
//                    }
//                }
//            }
//            else
//            {
//                ret.add(o);
//            }
//        }
//
//        return ret;
//    }
//    static class MemoryItem
//    {
//        int memorySpace;    // 0 = Program Memory; 1 = Data/RAM Memory; 2 = Debug
//        Long startAddress;
//        Long endAddress;
//        String name;
//        String region;
//        Object unsimplifiedOrigin;
//        String contents;
//    }
//
//
//    // NOT DONE IMPLEMENTING THIS; DO NOT USE
//    static List<MemoryItem> SimplifyProgramMemorySpace(List<Object> programMemoryItems)
//    {
//        List<MemoryItem> items = new ArrayList<MemoryItem>();
//        for (Object o : programMemoryItems)
//        {
//            MemoryItem item = new MemoryItem();
//            item.memorySpace = 0;
//            if (o instanceof WORMHoleSector)
//            {
//                WORMHoleSector w = (WORMHoleSector)o;
//                item.startAddress = Long.decode(w.getBeginaddr());
//                item.endAddress = Long.decode(w.getEndaddr());
//                item.region = "";
//                item.name = w.getRegionid();
//            }
//            items.add(item);
//        }
//        return items;
//    }
    // Use edc:WORMHoleSector or edc:ConfigFuseSector for configSectorNode parameter
    static List<PairWithText> EnumDCRDefs(Node cfgmem)
    {
        int size;
        List<PairWithText> dcrs = new ArrayList<PairWithText>();
        PairWithText p;

        NodeList registers = cfgmem.getChildNodes();  // edc:DCRDef

        // Process all edc:DCRDef nodes, which are the actual config registers in Flash , ex: exclude edc:WORMHole mapping info and edc:AdjustPoint's
        for(int i = 0; i < registers.getLength(); i++)
        {
            Node registerNode = registers.item(i);

            // Skip over edc:WORMHole's and edc:AdjustPoint's, we are only
            // interested in the actual DCRDef's since they have edc:_addr's all
            // pre-computed for them and seem to be the most accurate (as
            // compared to trying to compute them)
            if(!registerNode.getNodeName().equals("edc:DCRDef"))
            {
                continue;
            }

            // Check for attributes, nodes without them are useless to us
            NamedNodeMap attribs = registerNode.getAttributes();
            if(attribs == null)    // Register nodes must have attributes, so this must not be a true register node
            {
                continue;
            }

            // Convert edc:nzwidth in 'bits' to actual program memory addresses
            // occupied.
            Node sizeAttrib = attribs.getNamedItem("edc:nzwidth");
            size = 0x2;
            if(sizeAttrib != null) // Have nzwidth, so use it (units of bits)
            {
                size = Integer.decode(sizeAttrib.getNodeValue()) / 16 * 2;   // Some Config Words are defined as 24-bits, but this should be okay. It will still take 2 addresses
                if(size == 0)  // If the item is a Byte wide fuse, someone might have defined it as only having 8 bits or less. These fuses still take up a whole program word location (0x2 addresses) though, so it would not be appropriate to integer divide it down to 0x0 size.
                {
                    size = 0x2;
                }
            }
            else    // No nzwidth, check for offset instead (units of addresses)
            {
                sizeAttrib = attribs.getNamedItem("edc:offset");
                if(sizeAttrib != null)
                {
                    size = Integer.decode(sizeAttrib.getNodeValue());
                }
                else
                {
                    // We don't know how long this item is and they aren't making it easy for us to find out. Let's see if there are edc:endaddr or edc:_end attributes we can go on.
                    if((attribs.getNamedItem("edc:endaddr") != null) && (attribs.getNamedItem("edc:beginaddr") != null))
                    {
                        size = (int)(Long.decode(attribs.getNamedItem("edc:endaddr").getNodeValue()) - Long.decode(attribs.getNamedItem("edc:endaddr").getNodeValue()));
                    }
                    else if((attribs.getNamedItem("edc:_end") != null) && (attribs.getNamedItem("edc:_begin") != null))
                    {
                        size = (int)(Long.decode(attribs.getNamedItem("edc:_end").getNodeValue()) - Long.decode(attribs.getNamedItem("edc:_begin").getNodeValue()));
                    }
                    // else totally can't tell, let's assume it takes at least one instruction word though

                }
            }

            p = new PairWithText(attribs.getNamedItem("edc:cname").getNodeValue());
            p.first = Long.decode(attribs.getNamedItem("edc:_addr").getNodeValue());
            p.second = p.first + size;
            dcrs.add(p);
        }

        return dcrs;
    }


    static class ConfigFieldLocation
    {
        boolean found = false;
        String configWordName = null;
        long address = 0;
        long bitmask = 0;
        long bitPosition = 0;
    }

    // Use edc:WORMHoleSector or edc:ConfigFuseSector for configSectorNode parameter
    // If bitFieldName == "FSIGN", an "FSIGN" Config Word name or cname is found, or an edc:desc attribute contains "signature" (case inensitive) is found, but it contains no "FSIGN" or "SIGN" bit in it, but a "reserved" item is found (case insensitive and can be prefixed), or if a hidden item is found, we'll return that hidden or reserved item rather than coming back empty handed
    static List<ConfigFieldLocation> LookupConfigField(Node configSectorNode, String bitFieldName)
    {
        List<ConfigFieldLocation> ret = new ArrayList<ConfigFieldLocation>();
        long dcrAddress;
        String registerName;

        if(configSectorNode == null)
        {
            return null;
        }

        NodeList registers = configSectorNode.getChildNodes();  // edc:DCRDef

        // Process all edc:DCRDef nodes, which are the actual config registers in Flash , ex: exclude edc:WORMHole mapping info, but do include all edc:AdjustPoints and items with an nzwidth
        for(int i = 0; i < registers.getLength(); i++)
        {
            Node registerNode = registers.item(i);

            if(!registerNode.getNodeName().equals("edc:DCRDef"))
            {
                continue;
            }

            dcrAddress = 0;
            try
            {
                dcrAddress = Long.decode(registerNode.getAttributes().getNamedItem("edc:_addr").getNodeValue());
            }
            catch(Exception e)
            {
                System.err.printf("Could not find edc:_addr parameter for Config word %1$s. This may cause a run-time error. You may need different EZBL and/or MPLAB X IDE version combination.\n", registerNode.getAttributes().getNamedItem("edc:cname").getNodeValue());
            }

            registerName = registerNode.getAttributes().getNamedItem("edc:cname").getNodeValue();

            // Enumerate all edc:DCRModeList and then edc:DCRMode and then edc:DCRFieldDef + edc:AdjustPoint subnodes so we can search for the bitFiledName
            NodeList DCRModeLists = registerNode.getChildNodes();
            for(int j = 0; j < DCRModeLists.getLength(); j++)
            {
                if(!DCRModeLists.item(j).getNodeName().equals("edc:DCRModeList"))
                {
                    continue;
                }

                NodeList DCRModes = DCRModeLists.item(j).getChildNodes();
                for(int k = 0; k < DCRModes.getLength(); k++)
                {
                    if(!DCRModes.item(k).getNodeName().equals("edc:DCRMode"))
                    {
                        continue;
                    }

                    int bitOffset = 0;
                    int thisFieldWidth;

                    NodeList DCRFieldDefs = DCRModes.item(k).getChildNodes();
                    for(int l = 0; l < DCRFieldDefs.getLength(); l++)
                    {
                        Node DCRFieldDef = DCRFieldDefs.item(l);
                        if(!DCRFieldDef.hasAttributes())   // edc:DCRFieldDef and edc:AdjustPoint nodes always have attributes, so skip text that doesn't
                        {
                            continue;
                        }

                        // Track bit offset length
                        NamedNodeMap attribs = DCRFieldDef.getAttributes();
                        Node sizeAttrib;
                        sizeAttrib = attribs.getNamedItem("edc:nzwidth");       // For true edc:DCRFieldDef nodes
                        if(sizeAttrib == null)
                        {
                            sizeAttrib = attribs.getNamedItem("edc:offset");    // For edc:AdjustPoint nodes
                        }
                        thisFieldWidth = Integer.decode(sizeAttrib.getNodeValue());
                        bitOffset += thisFieldWidth;                            // Pre-increments since we may continue without a match here. Need to subtract off to get field start bit position

                        // Look for a matching bitFiledName. For non-matches, just record the bit offset so we can increase our counter
                        if(!DCRFieldDef.getNodeName().equals("edc:DCRFieldDef"))
                        {
                            continue;
                        }
                        if(!attribs.getNamedItem("edc:cname").getNodeValue().equals(bitFieldName))     // See if the Canonical Name, "cname", field matches the specified name
                        {
                            if(!attribs.getNamedItem("edc:name").getNodeValue().equals(bitFieldName))  // No match yet, see if the "name" field matches instead
                            {
                                // Still no match, see if at least the register name matches the bit field name in case if the field is hidden and saved in edc as a reserved field (ex: FSIGN's SIGN bit may not be called SIGN)
                                if(!registerName.equalsIgnoreCase(bitFieldName))
                                {
                                    continue;   // Nope - must be some other bit field, so keep searching
                                }
                                if(registerNode.getAttributes().getNamedItem("edc:ishidden") != null)
                                {
                                    if(!registerNode.getAttributes().getNamedItem("edc:ishidden").getNodeValue().equalsIgnoreCase("true"))
                                    {
                                        continue;   // Nope - must be some other bit field, so keep searching
                                    }
                                }
                            }
                        }

                        // Found one, add it for returning the Config word name, Config word address and bit mask
                        ConfigFieldLocation cfg = new ConfigFieldLocation();
                        cfg.configWordName = registerName;
                        cfg.address = dcrAddress;
                        cfg.bitmask = Long.decode(attribs.getNamedItem("edc:mask").getNodeValue()) << (bitOffset - thisFieldWidth);
                        cfg.bitPosition = bitOffset - thisFieldWidth;
                        cfg.found = true;
                        ret.add(cfg);

                        // Continue searching after this, on some devices, the same field will occur in multiple locations (ex: dual-partition config words)
                    }
                }
            }
        }

        return ret;
    }
}
