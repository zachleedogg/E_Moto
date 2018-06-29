/*******************************************************************************
Copyright 2017 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

/** INCLUDES *******************************************************/
#include "system.h"
#include "usb.h"
#include "usb_host_msd.h"
#include "usb_host_msd_scsi.h"
#include "../ezbl_integration/ezbl.h"


void GetTimestamp (FILEIO_TIMESTAMP * timestamp);


// Bootloader items
void EZBL_MSDBootloaderTask(int resetInternalState);
int (*EZBL_AppPreInstall)(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer);    // Application's EZBL_PreInstall() callback function pointer


#define EZBL_APP_IMAGE_FILE                 "FIRMWARE.BL2"  // Filename the bootloader checks on reset for in the root of the MSD drive
#define EZBL_APP_LAUNCH_TIMEOUT             (NOW_sec * 1u)  // Length of time we should wait at reset trying to enumerate a mass storage device to check for new firmware. 1 second is too short for hard drives, but should work for most thumb drives.
extern const FILEIO_DRIVE_CONFIG gUSBDrive;                 // Declared in ezbl_lib.a/EZBL_gUSBDrive.c as weak default implementation forwarding to USB APIs
uint8_t deviceAddress = 0;                                  // USB media parameters destination
#if(__PROGRAM_LENGTH < 0x00B000u)                           // <= 64KB devices
EZBL_SetAppReservedHole(0x006000, 0x008000);                // Reserve some flash addresses for the Application on the first PSV window. This helps avoid having the .const PSV page run out of space when compiling future Application projects due to Bootloader executable code occupying excess space in the same 32KB (0x008000 program address) PSV window.
EZBL_SetAppReservedHole(0x009000, __PROGRAM_BASE + __PROGRAM_LENGTH - 0x2); // Reserve more addresses on the second PSV window. We don't know what .const will link to.
#else
EZBL_SetAppReservedHole(0x004000, 0x008000);                // Reserve some flash addresses for the Application. This helps avoid having the .const PSV page run out of space when compiling future Application projects due to Bootloader executable code occupying excess space in the same 32KB (0x008000 program address) PSV window.
#endif
EZBL_DefineWeakLEDMap(RA7, RA6, RA5, RA4, RA3, RA2, RA1, RA0);  // Default I/O pin Map to LED bit positions for later use when calling LEDToggle() in ezbl_lib.a. MSbits on left, LSbit on right. This may be overridden in system.c.

//#define DEBUG_BOOTLOADER                    (-230400)       // Uncomment to enable console debug messages. The value defines the UART baud rate with negative values meaning auto-baud for RX, 2's complemented value for initial TX baud rate
#if !defined(DEBUG_BOOTLOADER)
#define EZBL_printf(...)    // Remove printf statements
#endif


/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/
int main(void)
{
    SYS_Initialize();

    FILEIO_Initialize();

    FILEIO_RegisterTimestampGet(GetTimestamp);

    //Initialize the stack
    USBHostInit(0);

    while(1)
    {
        ClrWdt();
        USBHostTasks();
        USBHostMSDTasks();
        EZBL_MSDBootloaderTask(0);
    }//end while
}//end main


/****************************************************************************
  Function:
    bool USB_ApplicationEventHandler( uint8_t address, USB_EVENT event,
                void *data, uint32_t size )

  Summary:
    This is the application event handler.  It is called when the stack has
    an event that needs to be handled by the application layer rather than
    by the client driver.

  Description:
    This is the application event handler.  It is called when the stack has
    an event that needs to be handled by the application layer rather than
    by the client driver.  If the application is able to handle the event, it
    returns true.  Otherwise, it returns false.

  Precondition:
    None

  Parameters:
    uint8_t address    - Address of device where event occurred
    USB_EVENT event    - Identifies the event that occurred
    void *data         - Pointer to event-specific data
    uint32_t size      - Size of the event-specific data

  Return Values:
    true    - The event was handled
    false   - The event was not handled

  Remarks:
    The application may also implement an event handling routine if it
    requires knowledge of events.  To do so, it must implement a routine that
    matches this function signature and define the USB_HOST_APP_EVENT_HANDLER
    macro as the name of that function.
  ***************************************************************************/

bool USB_ApplicationEventHandler( uint8_t address, USB_EVENT event, void *data, uint32_t size )
{
    switch( (int) event )
    {
        case EVENT_VBUS_REQUEST_POWER:
            // The data pointer points to a byte that represents the amount of power
            // requested in mA, divided by two.  If the device wants too much power,
            // we reject it.
            return true;

        case EVENT_VBUS_RELEASE_POWER:
            //This means that the device was removed
            return true;
            break;

        /* Here are various other events that a user might want to handle
         * or be aware of.  In this demo we are not handling them so we
         * will just return true to allow the stack to move on from the error.
         */
        case EVENT_HUB_ATTACH:
        case EVENT_UNSUPPORTED_DEVICE:
        case EVENT_CANNOT_ENUMERATE:
        case EVENT_CLIENT_INIT_ERROR:
        case EVENT_OUT_OF_MEMORY:
        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
            return true;
            break;

        case EVENT_MSD_ATTACH:
            deviceAddress = address;
            break;

        default:
            break;
    }

    return false;
}

// Placeholder function to get the timestamp for FILEIO operations
void GetTimestamp(FILEIO_TIMESTAMP * timestamp)
{
    // Populate the timestamp field with some inaccurate timestamp information
    timestamp->date.bitfield.day         = EZBL_CompileDay();
    timestamp->date.bitfield.month       = EZBL_CompileMonth();
    timestamp->date.bitfield.year        = EZBL_CompileYear() - 1980u;
    timestamp->time.bitfield.hours       = EZBL_CompileHour() + 1u;
    timestamp->time.bitfield.secondsDiv2 = EZBL_CompileSecond()/2u;
    timestamp->time.bitfield.minutes     = EZBL_CompileMinute() + 1u;
    timestamp->timeMs                    = 0;
}


/**
 * Checks if a USB Mass Storage Device is attached, and if so, checks to see if 
 * a "FIRMWARE.BL2" application firmware file is present in the root directory.
 * If not found or a 1 second mounting/searching timeout elapses, the existing
 * application in flash is launched (if one exists). If none exists, this
 * function returns and you should call it again in the future to test again for
 * USB MSD insertion with an appropriate "FIRMWARE.BL2" file to install.
 * 
 * If a "FIRMWARE.BL2" file is found on the attached MSD, it is checked to see
 * if it matches the Application firmware already installed in flash. If it
 * matches, the Application is immediately launched without bootloading 
 * anything.
 * 
 * If the "FIRMWARE.BL2" file contains different firmware, then the existing
 * application is erased, "FIRMWARE.BL2" is decoded and programmed to flash,
 * and if all operations succeeded, the newly installed application is 
 * flagged as valid and immediately launched. If the "FIRMWARE.BL2"
 * decoding/programming fails (ex: contains a corrupt or invalid EZBL image, 
 * user unplugs the USB drive during programming, power is lost, drive read 
 * error occurs, etc.), then this function returns with the partially programmed 
 * application left in flash, but not marked as valid. The partial image will
 * not be executed on subsequent reset or calls to EZBL_MSDBootloaderTasks(). A
 * valid and readable "FIRMWARE.BL2" image is needed to repeat the process
 * successfully.
 * 
 * @param resetInternalState
 *        Flag indicating that this function should reset its internal countdown
 *        timer and other internal variables/peripheral state. Set to non-zero
 *        to force this internal state reset. 0 maintains internal state, except
 *        for the first call after reset, where the function will always reset
 *        its internal state, regardless of the resetInternalState parameter
 *        value.
 * 
 * @return This function only returns during the initial 1-second MSD mounting 
 *        window or when no valid application firmware exists to execute 
 *        instead. Otherwise, this function will launch the existing or new 
 *        application and not return.
 */
void EZBL_MSDBootloaderTask(int resetInternalState)
{
    static unsigned char initialized = 0;
    static unsigned char lastMediaDetect;
    static unsigned long timeToLaunchApp;
    static unsigned long ledTimer;
    static unsigned int originalRCON;
    FILEIO_OBJECT imgFile;
    unsigned char mediaDetect;
    int result;

    // On first entry or anytime the forcing resetInternalState flag is set, initialize various bootloader flags
    if(!initialized || resetInternalState)
    {
        EZBL_ForwardBootloaderISR = 0x00000000;     // Keep all interrupts in bootloader for now
        EZBL_appIsRunning = 0;                      // EZBL_InstallFILEIO2Flash() is allowed to return after erasing App space
        EZBL_NVMKey = 0x0000;                       // Generally already zero, but make sure
        EZBL_AppPreInstall = 0;                     // Existing Application callback function pointer for last chance bootload accept/reject/App clean up
        lastMediaDetect = 0;
        NOW_Reset(TMR1, EZBL_InitializeBoard());    // Initialize NOW_32() time keeping (using Timer 1) and other I/O pins normally implemented in EZBL projects
        LEDSet(0x00);                               // Start with all LEDs off
        EZBL_MapWriteEx(0, EZBL_LEDMap, (unsigned int)&TRISA - (unsigned int)&LATA);   // Clear all TRISx bits for the LED IO map - see EZBL_DefineLEDMap() call at top of file
        ledTimer = NOW_32();
        timeToLaunchApp = ledTimer + EZBL_APP_LAUNCH_TIMEOUT;    // Give up trying to enumerate the USB MSD device and search for a "FIRMWARE.BLO" file after this time
        initialized = 1;


#if defined(DEBUG_BOOTLOADER)
        #if defined(__dsPIC33EP512MU810__) || defined(__dsPIC33EP256MU810__) || defined(__PIC24EP512GU810__) || defined(__PIC24EP256GU810__)    // dsPIC33EP512GM810/PIC24EP512GU810 PIM
        _U2RXR     = 100;           // U2RX on RP100/RF4
        _RP101R    = _RPOUT_U2TX;   // U2TX on RP101/RF5
        #elif defined(_RP17R)   // PIC24FJ1024GB610 PIM, PIC24FJ256GB410 PIM, PIC24FJ256GB110 PIM
        _U2RXR = 10;                // U2RX = RP10
        _RP17R = _RPOUT_U2TX;       // U2TX = RP17
        #endif
        UART_Reset(2, NOW_Fcy, DEBUG_BOOTLOADER, 1);// UART2, CPU MIPS, 230400 baud, use for stdio
        EZBL_printf("\x0F"                          // "\x0F"      = ANSI SI "Shift In" control code so terminal emulators go back to the correct character set if they got changed while we were being programmed or reset
                    "\x1B[0m"                       // "\x1B[0m"   = ANSI Reset All Attributes (no color)
                    //"\x1B[2J"                     // "\x1B[2J"   = ANSI clear screen (scroll back buffer retained)
                    //"\x1B[1;1f"                   // "\x1B[1;1f" = ANSI set cursor at row 1, column 1,
                    "\n\n\n%s(%i) initialized", __func__, resetInternalState);  // EZBL_MSDBootloaderTask() initialized
#endif

        // If a trap reset occurred (or compare mismatch, illegal opcode, or 
        // watchdog timeout), do not attempt to start a new Bootload attempt. 
        // This avoids a continuous Bootload -> trap -> reset -> erase flash ->
        // Bootload attempt loop if the trap occurred due to an illegal image 
        // file being presented (ex: targets unimplemented flash, triggering
        // _AddressError() during verification. To retry, remove and reinsert
        // the mass storage media or power cycle the device.
        originalRCON = RCON;
        if(originalRCON & (_RCON_CM_MASK | _RCON_IOPUWR_MASK | _RCON_TRAPR_MASK | _RCON_WDTO_MASK))
        {
            EZBL_printf("\nAbnormal reset detected, skipping bootloading until POR/BOR/MCLR; RCON = 0x%04X", RCON);
            initialized = 10;   // Bootload suppression flag
            RCON = originalRCON & ~(_RCON_CM_MASK | _RCON_IOPUWR_MASK | _RCON_TRAPR_MASK | _RCON_WDTO_MASK);
        }
    }

    // Enter a branch flow loop (not an actual infinite loop)
    while(1)
    {
        // Blink an LED at 8Hz to indicate we are in Bootloader mode
        if((long)(NOW_32() - ledTimer) >= 0)
        {
            LEDToggle(0x01);
            ledTimer += NOW_sec/16;
        }

        // Check if a valid application is installed
        if(EZBL_IsAppPresent())
        {
            // Yes, check if it is time to branch to the application
            if((long)(NOW_32() - timeToLaunchApp) >= 0)
            {
                // Yes, close down USB, peripheral interrupts, begin forwarding interrupts, then branch to the application
                USBHostShutdown();
                EZBL_RAMSet((void*)&IEC0, 0x00, (unsigned int)&IPC0 - (unsigned int)&IEC0); // Disable all interrupts
                EZBL_ForwardBootloaderISR = 0xFFFFFFFF;                                     // Forward all interrupt vectors to the Application
                //NOW_EndAllTasks();                                                          // Disable all NOW_TASK callbacks to avoid executing them if the Application calls NOW_32()/NOW_64(). Commented out since we don't call EZBL_CreateRepeatingTask() or use NOW_TASKs in this USB Host MSD Bootloader example project.

                EZBL_ForwardIntToBoot(T1);      // Undo forwarding the NOW timer interrupt so the Bootloader's NOW_*() timing APIs can be used in the Application.
                NOW_IntEnable();
#if defined(DEBUG_BOOTLOADER)
                EZBL_ForwardIntToBoot(U2RX);    // Since we initialized it and put the code in flash, also take the UART RX/TX interrupts back from the Application so the UART FIFOing APIs can be reused.
                EZBL_ForwardIntToBoot(U2TX);
                EZBL_FIFOIntEnable(EZBL_STDIN);
                EZBL_FIFOIntEnable(EZBL_STDOUT);
                EZBL_printf("\nLaunching app\n\n");
                EZBL_FIFOFlush(EZBL_STDOUT, NOW_sec);
#endif

                LEDSet(0x00);                   // Turn off all LEDs
                RCON = originalRCON;            // Restore bits in RCON for App to use since we might have changed them
                EZBL_StartAppIfPresent();       // Branch to application's reset vector
                EZBL_ResetCPU();                // Application shouldn't return, but if it does, issue a RESET instruction so bootloader starts up cleanly again
            }
        }

        // When media removed, we should retry bootloading the next time it is plugged in
        mediaDetect = FILEIO_MediaDetect(&gUSBDrive, &deviceAddress);
        if((lastMediaDetect ^ mediaDetect) & lastMediaDetect)
        {
            initialized = 1;
        }
#if defined(DEBUG_BOOTLOADER)
        if(lastMediaDetect ^ mediaDetect)
        {
            EZBL_printf("\nMedia %s", mediaDetect ? "inserted" : "removed\n");
        }
#endif
        lastMediaDetect = mediaDetect;
        if(!mediaDetect)
            break;
        
        // Check if we already tried to (unsuccessfully) bootload and it resulted in failure
        if(initialized != 1u)
            break;  // Yes, so do nothing until media is reset

        if(FILEIO_DriveMount('A', &gUSBDrive, &deviceAddress) != FILEIO_ERROR_NONE)
            break;

        if(FILEIO_Open(&imgFile, EZBL_APP_IMAGE_FILE, FILEIO_OPEN_READ) == FILEIO_RESULT_FAILURE)
        {
            // No firmware file present, so just clean up and set timeToLaunchApp
            EZBL_printf("\n  " EZBL_APP_IMAGE_FILE " not found");
            FILEIO_DriveUnmount('A');
            timeToLaunchApp = NOW_32();
            continue;
        }
        EZBL_printf("\n  Found " EZBL_APP_IMAGE_FILE);


        // If an App already exists in flash, run through full FIRMWARE.BL2 file
        // contents in simulation, non-commit mode first, then if it checks out,
        // erase the existing App and do the programming steps. This checks that
        // the file is for us, has a valid CRC, addresses memory locations that
        // won't trigger an address error trap, and is otherwise structurally
        // valid before we erase anything.
        result = EZBL_ERROR_SUCCESS;
        if(EZBL_IsAppPresent())
            result = EZBL_InstallFILEIO2Flash(&imgFile, 0); // Simulation read-only
        if(result == EZBL_ERROR_SUCCESS)
            result = EZBL_InstallFILEIO2Flash(&imgFile, 1); // Real erase/program/verify
        LEDSet(result);         // Display 8 LSbits of return code on LEDs
        EZBL_printf("\n  EZBL_InstallFILEIO2Flash() returned 0x%04X (%s)",
                    result,
                    (result == EZBL_ERROR_SUCCESS) ? "success" :
                    (result == EZBL_ERROR_SOFT_VERIFY_ERROR) ? "soft error - bootloader mismatch" :
                    (result == EZBL_ERROR_ALREADY_INSTALLED) ? "already installed" :
                    (result == EZBL_ERROR_BOOTID_HASH_MISMATCH) ? "image not for this bootloader" :
                    (result == EZBL_ERROR_COM_READ_TIMEOUT) ? "file read error" :
                    "other error code");

        // (Hopefully) a matching app or a freshly bootloaded application is
        // present now, so indicate it is timeToLaunchApp
        timeToLaunchApp = NOW_32();
        FILEIO_Close(&imgFile);
        FILEIO_DriveUnmount('A');

        if(result < 0)
        {   // Something failed. Increment the initialized state so we can
            // suppress repeated retries until the user unplugs the media and
            // plugs it back in.
            initialized++;
            EZBL_printf("\n  Failed: %s", EZBL_IsAppPresent() ? "4s indicator delay then starting existing app." : "reinsert media to try again");
            NOW_Wait(4u*NOW_sec);
            LEDSet(0x00);           // Clear LEDs and return to checking for media
        }
    }
}


/**
 * Callback function executed by EZBL_InstallFILEIO2Flash() just before the existing
 * Application (if any) is erased, permitting last minute rejection of a
 * firmware update or an opportunity to cleanly shut down any running operations
 * (especially if they might interfere with successful Bootloading).
 * 
 * See callback documentation for EZBL_PreInstall() in ezbl.h for more
 * information.
 *
 * @param *rxFromRemote null for USB Host MSD Bootloaders
 *
 * @param *txToRemote null for USB Host MSD Bootloaders
 *
 * @param fileSize Total file size of the .bl2 file being offered, including
 *                 the SYNC bytes and .bl2 header.
 *
 * @param *appIDVer Pointer to the EZBL_APPID_VER data contained in the offered
 *                  .bl2 file header.
 *
 *                  By the time this callback is called, the BOOTID_HASH has
 *                  already been confirmed as a match for this Bootloader.
 *                  Additionally, checking of EZBL_NO_APP_DOWNGRADE conformance
 *                  has passed, when enabled and an existing Application is
 *                  already installed.
 *
 * @return Return 0 to reject the offered firmware image or 1 to accept the
 *         image and proceed with erase/programming steps.
 *
 *         This USB Host MSD implementation calls the EZBL_AppPreInstall
 *         function pointer, when non-null, to let an existing Application
 *         decide if it is appropriate to shutdown and let bootloading proceed.
 *         When bootloading due to device reset with storage media inserted, any
 *         existing Application would not have set EZBL_AppPreInstall to a
 *         non-null value.
 *
 *         By default, this USB Host MSD bootloader only executes at device
 *         reset (before any existing App has started executing), so this
 *         callback is not required. EZBL_AppPreInstall will always contain
 *         null.
 */
int EZBL_PreInstall(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer)
{
    int ret = 1;    // Default to accepting the offered .bl2 App update

    // Check if EZBL_AppPreInstall function pointer has been set to a non-null
    // value by an existing Application, and if so, call the pointer to yield
    // the accept/reject decision to the running Application project.
    if(EZBL_AppPreInstall)
    {
        ret = (*EZBL_AppPreInstall)(rxFromRemote, txToRemote, fileSize, appIDVer);      // Calls the function pointer
        if(ret > 0)
        {   // Installation accepted by existing App, so disable all interrupts except the ones needed for bootloading
            EZBL_RAMSet((void*)&IEC0, 0x00, (unsigned int)&IPC0 - (unsigned int)&IEC0); // Clear every bit in every IECx Interrupt Enable register
            EZBL_ForwardBootloaderISR = 0;                                              // Stop forwarding any interrupt vectors to the App

            // Turn our Bootloader interrupts back on so we can read the .bl2 file
            NOW_IntEnable();
            _USB1IE = 1;
        }
    }

    return ret; // Accepts update offering if no App installed or running right now
}
