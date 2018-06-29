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

import java.io.*;
import java.net.*;
import java.security.*;
import java.util.*;


/**
 *
 * @author C12128
 */
public class EZBLState implements Serializable
{
    public String ezblStateSavePath = null;

    public int pass = 0;                           // -pass option: A pass counter for parsing .elf data - not related to -password

    public boolean parseOkay = false;
    public boolean MCU16Mode = true;               // PIC24 or dsPIC targetted (16-bit device)
    public boolean fCore = false;                  // PIC24F/dsPIC30F/dsPIC33F/PIC24H core device targetted
    public boolean eCore = false;                  // PIC24E/dsPIC33E core device targetted
    public boolean cCore = false;                  // dsPIC33C core device targetted
    public boolean mmCore = false;                 // PIC32MM core device targetted
    public boolean verbose = false;                // Flag for printing more debug messages
    public boolean silent = false;                 // Flag for printing no status messages
    public boolean linkedAsDebugImage = false;     // Flag to setting BKBUG to match what the ICD requires when the project is compiled for Debug mode
    public boolean forceConfigEraseRestore = false;// Flag to force the Config words in the Erase Restore Table, even though the bootloader will never erase them (due to existing as a __EZBL_ROM_USE_TABLE element)
    public int flashWordSize = 0x4;                // Number of program addresses that a minimally sized Flash word occupies. Used for padding to minimum Flash write size requirements.
    public int hexAlign = 0;                       // Minimum address alignment for Flash padding/alignment operations (Blobber). 0 is reserved to mean option not set. If you don't want alignment, use hexAlign = 1.
    public boolean buildAsOneMode = false;
    public boolean standaloneMode = true;          // Default is an ordinary standalone bootloader
    public boolean legacyClassic = false;          // false: Make an EZBL linked BIOS Bootloader; true: Make a classic Bootloader that exports no symbols and completely avoids Application interference.
    public boolean remapISRThroughIGT = true;      // true: Generate an Interrupt Goto Table to remap all Interrupt Vectors onto an erasable Flash page; false: Assume interrupts as defined by the compiler are fine (ex: Dual Partition ommits any need to forward interrupts).
    public int symbolExportLevel = 100;            // 0 = export no symbols at all, 1 = export EZBL_ForwardBootloaderISR only, 100 = normal forwarding level of all public symbols without too many preceeding underscores or known problems
    public boolean saveTemps = false;              // Configuration option of saving ezbl_tools temporary files during the build process, including elf dumps
    public boolean allowLastPageErase = true;      // Configuration Option
    public boolean allowFirstPageErase = false;    // Configuration Option
    public boolean appVectorsAfterBootloader = true;// Determins if App Erasable items should be placed immediately after a bootloader sector (true), or immediately before (false)
    public boolean deleteResetSection = false;     // Determins if the GLDBuilder should delete the .reset section that pre-exists in XC16 .gld linker scripts or not. Replacement is needed for buildAsOneMode mode, but also may be desired if explicitly specified on the command line.
    public boolean dualPartitionMode = false;      // false = Single Partition view; true = Partition 1 (Active Partition/requires Dual Partition supporting device); 2 = Partition 2 (Inactive Partition/requires Dual Partition supporting device)
    public String[] argsFromMakeEditor = null;     // Array of original command line arguments passed in when the MakeEditor class (--make_editor) was first invoked. Used to rebuild a new state file at the first GLDBuilder stage if the target processor has changed.
    public String conf = null;                     // ${CONF}, name of MPLAB X build configuration, ex:"default"
    public String makefilePath = null;             // Path to MPLAB X build configuration makefile, ex: "nbproject/Makefile-${CONF}.mk"
    public String artifactPath = null;             // Location of MPLAB X .hex or .elf build output
    public String projectName = null;              // Makefile ${PROJECTNAME}, ex: ex_boot_uart
    public String linkScriptPath = null;           // MPLAB X project linker script file path
    public String compilerFolder = null;           // Makefile ${MP_CC_DIR}, ex: C:\Program Files (x86)\Microchip\xc16\v1.25\bin
    public String compilerLinkScriptPath = null;   // XC16, etc. compiler linker script for the selected project processor
    public String saveTempsFile = null;            // -save_temps= file path for communications logging or other single file outputs
    public String temporariesPath = null;          // -temp-folder= command line option, with a trailing '/' added. If no such option is specified, the folder that this ezbl_tools.jar file is located is used.
    public String partNumber = null;               // Makefile ${MP_PROCESSOR_OPTION}, ex: "33EP512GM710", "24FJ128GA010"
    public String fullPartNumber = null;           // Derived from partNumber, ex: "dsPIC33EP512GM710", "PIC24FJ128GA010"
    public String javaPath = null;                 // Makefile ${MP_JAVA_PATH}, ex: "C:\Program Files (x86)\Microchip\MPLABX\sys\java\jre1.7.0_25-windows-x64\java-windows/bin/"
    public String pathToIDEBin = null;             // Makefile ${PATH_TO_IDE_BIN}, ex: "C:/Program Files (x86)/Microchip/MPLABX/DefaultMPLAB/mplab_ide/platform/../mplab_ide/modules/../../bin/"
    public String blobPath = null;                 // Derived from hexPath, ex: "dist/default/production/ex_boot_uart_icsp_prog.production.blob"
    public String hexPath = null;                  // Makefile ${CND_ARTIFACT_PATH_${CONF}}, ex: "dist/default/production/ex_boot_uart_icsp_prog.production.hex" or "dist/default/debug/ex_boot_uart_icsp_prog.debug.elf"
    public String elfPath = null;                  // Path to .elf output artifact/temporary Pass 1 linking
    public String undecodedOptions = "";
    public String passThroughOptions = "";
    public String comPort = null;                  // Serial COM port name, ex: COM8, I2C, I2C0, I2C1, etc.
    public String baudRate = null;                 // Serial COM port or I2C baud rate, ex: 115200, 460800, 100000, 400000, etc.
    public int autoBaudRetryLimit = 4;             // Maximum number of non-timeout communications auto-baud echo failures to attempt before giving up if echo data is coming back corrupt. Every two attempts represents a halfing of baud rate used (70.71% multipler per attempt).
    public boolean hardwareFlowControl = false;    // Boolean indicating if communications implement hardware flow control or if the software protocol has to implement it.
    public String commandPrefix = "";              // Communicator command transmit prefix to apply for transmission (to avoid interfering with other nodes that are using a shared communications bus)
    public boolean skipComHandshake = false;       // Communicator should try to communicate without doing auto-baud or echo testing
    public int slaveAddress = 0x0;                 // Slave target address on the I2C bus (only applicable to I2C coms)
    public String remoteHost = null;               // IP address or hostname of the remote node for Communicator
    public int remotePort = 0;                     // TCP or UDP remote port number for Communicator
    public int milliTimeout = 1100;                // Default communications timeout (time since last RX activity, not absolute time for operations). Should be set > ping time.
    public String javaLaunchString = null;         // Command execution string for makefiles including full ClassPath for crownking.edc.jar, crownking.jar/crownking.common.jar and jna.jar from MPLAB X.
    public List<String> runtimeClassPaths = null;  // List of .jar file paths that can be loaded at run time based on prior gnu make variables from MPLAB
    public String encryptionPassword = null;       // AES128 raw .blob encryption password as entered by the user
    public String encryptionSaltString = null;     // Not implemented
    public byte encryptionSalt[] = null;       // Custom password salt (does not really need to be kept secret as it only prevents using pre-computed existing hash tables. Any unique value could work.)
    public byte encryptionKey[] = null;        // Padded (self-concatenated) and salted Password info (must be kept private)
    public long baseAddress = 0x000000;        // Base address of the bootloader, not including GOTO @ 0x000000, IVT entries, or config words. 0x000000 means no special handling and just let the linker choose where to place things.
    public long endAddressOfBootloader = 0;    // First address after a bootloader sector
    public long baseAddressOfAppErasable;      // Base address of EZBL_appBootloadState, EZBL_APP_RESET_BASE, and Interrupt Goto Table
    public long baseAddressOfAppBootloadState; // Base address of EZBL_appBootloadState flag word
    public long baseAddressOfGotoReset;        // Base address of app's goto __reset instruction
    public long baseAddressOfIGT;              // Base address of the Interrupt Goto Table (application erasable)
    public static final int sizeOfAppBootloadState = 0xC;   // Number of addresses used by the EZBL_appBootloadState structure
    public static final int sizeOfAppGotoReset = 0x4;       // Number of addresses used by the EZBL_APP_RESET_BASE
    public int sizeOfIGT;                      // Number of addresses used by the Interrupt Goto Table
    public int sizeOfAppErasable;              // Number of addresses used by the whole App Erasable section

    // Block of data decoded from EDC .PIC file
    public boolean hasFlashConfigWords = false;    // Specifies if Config Words are in Flash or in real Fuses
    public boolean hasAuxFlash = false;            // Specifies if AuxFlash exists on this device
    public boolean edcInfoLoaded = false;                      // Flag indicating the following variables have been found and successfully read for the given device
    public List<AddressRange> devNVGeometry = null; // [Address range) array to describe the physical part. Info read from EDC.
    public List<PairWithText> devProgramSpaces = new ArrayList<PairWithText>(); // [Address range) array to describe the physical part. Info read from EDC.
    public List<PairWithText> deviceConfigWords = new ArrayList<PairWithText>();    // A list of all Config words, their address, and their size for the selected device (extracted from EDC)
    public AddressRange devDataSpace = null;         // [Address range) array to describe the physical part. Info read from EDC.
    public AddressRange mainExecutionRange = null;             // [Address range) of ordinary execution Flash, not including Reset Vector/IVT/Flash Config Words addresses. I.e. __CODE_BASE to __CODE_BASE + __CODE_LENGTH on devices with Aux Flash or secondary partitions.
    public AddressRange mainFlashRange = null;                 // [Address range) of main Flash array (ex: starting at address 0x000000 with Reset Vector)
    public AddressRange auxFlashRange = null;                  // [Address range) of Aux Flash, if present (stays null when not present)
    public AddressRange configWordsRange = null;               // [Address range) where all of the Flash Configuration words lie between on the device
    public int eraseBlockSize = 512;                           // Count of instructions for the minimum erase block size
    public int eraseBlockSizeBytes = eraseBlockSize * 3;       // Count of bytes for the minimum erase block size
    public int eraseBlockSizeAddresses = eraseBlockSize * 2;   // Count of addresses for the minimum erase block size
    public long deviceIDAddress = 0;                   // Location of DEVID value in program memory
    public long deviceIDMask = 0;                      // Mask for important bits in the DEVID register (not part of revision)
    public long deviceIDValue = 0;                     // Value that should corresponds to the selected PIC, but formatted as stored in DDT (not masked yet)
    public long deviceRevAddress = 0;                  // Location of DEVREV value in program memory; This seems to be stored as one range in edc, so it is called an end address.
    public long deviceRevMask = 0;                     // Mask for important bits in the DEVREV register (or DEVID register on PIC32MM)
    public long deviceBACKBUGAddress = 0;              // Address of the Config Word containing BACKBUG ("BKBUG") background debugger config bit
    public long deviceBACKBUGMask = 0;                 // Bit position mask in the Config Word containing BACKBUG bit
    public String BACKBUGConfigWordName = null;
    public long codeProtectAddress = 0;                // Ex: address of FGS Flash Config Word
    public long codeProtectMask = 0;                   // Ex: 0x000002, meaning clearing bit 1 will turn on Code Protect. Some devices with multiple levels of Code Guard security will have more than one Code Protect Bit
    public String codeProtectConfigWordName = null;
    public long reservedBitAddress = 0;                // Flash address of the Flash Reserved Bit (should only be applicable if Config Words are in Flash)
    public long reservedBitMask = 0;                   // Bit position mask info in the reservedBitAddress program location for the Reserved Bit; this bit must be programmed as '0' if it doesn't already have that value in it.
    public String reserveBitConfigWordName = null;     // Name of the Flash Config Word that contains the reserved bit

    public ELFDumpParser elfDump = null;               // Parsed symbol and section list
    public GLDMemories gldMemories = null;             // Structure of all of the memories found in the Project linker script
    public String[] ivtVectors = null;                 // List of symbol names used by all of the device interrupt ivtVectors, including reserved/unimplemented ones. These are primary ivtVectors only (not alternates).
    public int ivtVectorsImplemented = 0;              // Count of how many true interrupt ivtVectors the selected processor supports (i.e. all reserved vector entries in the table are thrown away when counting)
    public boolean[] ivtVectorImplemented = null;      // Array indicating if the device actually has hardware to trigger this interrupt vector or not
    public boolean[] ivtVectorPointsToBootloader = null;// Array indicating if the device IVT entry points to the bootloader or the IGT
    public int[] ivtVectorEntries = null;              // Post bootloader linking IVT contents
    public String ivtSectionRemapped = null;           // .ivt section, but changed to point to IGT
    public String erasableIGT = null;                  // Remapped interrupt vector table (Interrupt Goto Table)
    public String compilerGLDContents = null;          // Processor .gld file contents read from compiler folder. This field should not be modified except when loading the compiler's gld for the first time.
    public List<MemoryRegion> deletedMemories = null;  // List of MEMORY regions that we pruned out of the linker script because they were occupied by bootloader code or marked as an -ignore= range.
    public List<AddressRange> romUseRanges = new ArrayList<AddressRange>();     // Sector padded (except possibly 1st sector and config words sector) list of addresses used by this project
    public List<AddressRange> ramUseRanges = new ArrayList<AddressRange>();     // Static/global RAM address range used by this project
    //public List<DataRecord> romRestoreElements = null; // For restoring bootloader items that can be erased but are still needed (ex: Goto@0 instruction (if page 0 erasable), any interrupt ivtVectors used by the bootloader (if page 0 erasable and ivt not remapped), and any configuration fuses used by the bootloader (if last page erasable and where the fuses are stored).
    public List<AddressRange> ignoreROMRanges = new ArrayList<AddressRange>(); // -ignore= ranges specified on the command line and internal ones created by EZBL (ex: EZBL_RESERVE_CONST_FOR_APP() macro in bootloader)
    public List<AddressRange> noEraseRanges = null;
    public List<AddressRange> noProgramRanges = null;
    public List<AddressRange> noVerifyRanges = null;

    //public List<Section> romExport = null;             // Copy of elfDump.sections which are in ROM and have been Flash Page padded, trimmed of ignore ranges, and otherwise manipulated for use as a bootloader
    //public List<Section> ramExport = null;             // Copy of elfDump.sections which are in RAM and possibly been manipulated to represent the bootloader
    //public List<Section> otherExport = null;           // Copy of elfDump.sections which are neither ROM or RAM and possibly been manipulated to represent the bootloader
    //public List<Symbol> symbolExport = null;
    public int minFreePSVSpace = 0;                     // Minimum number of PSV address space bytes (and program addresses) that a standalone mode bootloader must keep free for future application use without triggering a build failure
    public int warnFreePSVSpace = 0;                    // Minimum number of PSV address space bytes (and program addresses) that a standalone mode bootloader must keep free for future application use without triggering a warning message

    public List<Section> ramSections = null;            // RAM use section list (includes decoded symbols and section data contents)
    public List<Section> romSections = null;            // ROM use section list (includes decoded symbols and section data contents)
    public List<Section> otherSections = null;          // Debug and other section list (includes decoded symbols and section data contents)

    // Live-Update items
    public int FBOOTvalue = 0xFFFFFF;                  // Value to program into the FBOOT config
    public int FBOOTaddress = -1;                      // Address of FBOOT (could be 0x801000 or 0x801800, depending on device). If the device does not have an FBOOT Config word, then this field is set to -1.
    public int targetPartition = 0;                    // Specify 0 for non-dual boot capable device or to enable Single boot mode, specify 1 for Dual Boot mode with the code targetting the Active Partition or Partition 1, 2 for Daul Boot mode with the code targetting the Inactive Partition or Partition 2

    public static final String nullFile()
    {
        String os = System.getProperty("os.name");
        if((os == null) || os.startsWith("Windows"))
            return "NUL";
        else
            return "/dev/null";
    }

    public static final String ezblToolsExecPath()  // Not including any trailing slashes
    {
        String execPath = null;
        try
        {
            CodeSource codeSource = Main.class.getProtectionDomain().getCodeSource();
            File jarFile = new File(codeSource.getLocation().toURI().getPath());
            execPath = jarFile.getAbsolutePath();
        }
        catch(URISyntaxException ex)
        {
            execPath = Main.class.getProtectionDomain().getCodeSource().getLocation().getPath();
        }
        try
        {
            File f = new File(execPath).getParentFile();
            if(f != null)
                execPath = f.getCanonicalPath();
        }
        catch(IOException ex)
        {
            execPath = "";
        }
        if(execPath == null)
            return "";

        return execPath;
    }

    public static final String ourJarPath()  // Inclusive of ezbl_tools.jar executable name or classpath string
    {
        try
        {
            CodeSource codeSource = Main.class.getProtectionDomain().getCodeSource();
            File jarFile = new File(codeSource.getLocation().toURI().getPath());
            return jarFile.getAbsolutePath();
        }
        catch(URISyntaxException ex)
        {
            return Main.class.getProtectionDomain().getCodeSource().getLocation().getPath();
        }
    }

    public EZBLState()
    {
        javaLaunchString = "${MP_JAVA_PATH}java -cp \"" + ourJarPath() + File.pathSeparator + "${PATH_TO_IDE_BIN}../mplablibs/modules/ext/*.jar\" com.microchip.apps.ezbl.Main";
    }

    void SaveToFile()
    {
        // Remove non-persistent options so we don't try to use them later
        this.undecodedOptions = this.undecodedOptions.replaceAll("-dump[\\s]*?", "");                   // Used by Blobber only
        this.undecodedOptions = this.undecodedOptions.replaceAll("-generate_merge[\\s]*?", "");         // Used by Blobber only

        // Remove tons of section and symbol data that takes forever to load. These will be regenerated when needed (with the symbol table being reparsed from one large symbol dump string).
        if(this.elfDump != null)
        {
            this.elfDump.symbols = null;
        }
        if(this.pass != 2)
        {
            this.elfDump = null;
            this.romUseRanges = null;
            this.ramUseRanges = null;
            this.otherSections = null;
            this.ramSections = null;
            this.romSections = null;
        }
        this.gldMemories = null;
        //this.otherExport = null;
        //this.romExport = null;
        //this.ramExport = null;
        this.deletedMemories = null;

        File f = new File(this.ezblStateSavePath);
        File parentFolder = f.getParentFile();
        if(parentFolder != null)
        {
            if(!parentFolder.exists())
            {
                parentFolder.mkdirs();
            }
        }

        // Write everything to a binary file for later use in another stage
        ObjectOutputStream objectOut;
        FileOutputStream fileOut;
        try
        {
            fileOut = new FileOutputStream(this.ezblStateSavePath, false);
            objectOut = new ObjectOutputStream(fileOut);
            objectOut.writeObject(this);
            objectOut.close();
            fileOut.close();
        }
        catch(Exception ex)
        {
            if(!this.silent)
            {
                System.err.print("ezbl_tools: Unable to save state information\n    " + ex.getMessage());
            }
        }
    }

    public static EZBLState ReadFromFile(String filename)
    {
        // Ordinary binary file saving/loading
        if(!(new File(filename).exists()))
        {
            return new EZBLState();
        }

        // Read everything from file
        ObjectInputStream objectIn;
        FileInputStream in;
        try
        {
            in = new FileInputStream(filename);
            objectIn = new ObjectInputStream(in);
            EZBLState ret = (EZBLState)objectIn.readObject();
            objectIn.close();
            in.close();

            if((ret.elfDump != null) && (ret.elfDump.symbols == null) && (ret.elfDump.symbolTableAsString != null))
            {
                ret.elfDump.symbols = new ArrayList<Symbol>();
                int lastIndex = -1;
                int curIndex;
                while((curIndex = ret.elfDump.symbolTableAsString.indexOf('\n', lastIndex + 1)) >= 0)
                {
                    ret.elfDump.symbols.add(new Symbol(ret.elfDump.symbolTableAsString.substring(lastIndex + 1, curIndex)));
                    lastIndex = curIndex;
                }
            }

            return ret;
        }
        catch(InvalidClassException ex)
        {
            System.out.println("ezbl_tools: " + filename + " contains incompatible state data; using new state data.");
            return new EZBLState();
        }
        catch(Exception ex)
        {
            ex.printStackTrace();
        }
        return null;
    }

    /**
     * Creates a sorted and reduced List of simple AddressRanges that correspond
     * to all addresses contained in the given list of sections, . Abutting
     * sections result in a one larger AddressRange covering both.
     *
     * None of the existing lists are modified or sorted by this operation. The
     * ranges returned are new copies.
     *
     * @param sections      List of Sections to include in the returned ranges.
     *                      The sections must have the .loadMemoryAddress and
     *                      .size parameters set appropriately. The .size field
     *                      is in addresses. If not needed, specify null or an
     *                      empty List.
     *
     * @param dataRecords   List of DataRecords to include in the returned
     *                      ranges. The DataRecords must have the .address and
     *                      .GetEndAddress() returning appropriate values (i.e.
     *                      .architecture16bit must be set right) . If not
     *                      needed specify null or an empty List.
     *
     * @param addressRanges List of AddressRanges to also include in the
     *                      returned ranges. If not needed specify null or an
     *                      empty List.
     *
     * @return Numerically sorted list of Addresses occupied by all elements in
     *         the lists.
     */
    public static List<AddressRange> computeSimpleUnion(List<Section> sections, List<DataRecord> dataRecords, List<AddressRange> addressRanges)
    {
        List<AddressRange> unionRanges = new ArrayList<AddressRange>();

        if(sections != null)
        {
            for(Section sec : sections)
            {
                unionRanges.add(new AddressRange(sec.loadMemoryAddress, sec.loadMemoryAddress + sec.size));
            }
        }
        if(dataRecords != null)
        {
            for(DataRecord rec : dataRecords)
            {
                unionRanges.add(new AddressRange(rec.address, rec.GetEndAddress()));
            }
        }
        if(addressRanges != null)
        {
            for(AddressRange ar : addressRanges)
            {
                unionRanges.add(new AddressRange(ar.startAddress, ar.endAddress));
            }
        }

        AddressRange.SortAndReduce(unionRanges);

        return unionRanges;
    }
}
