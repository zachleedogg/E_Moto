/*******************************************************************************
  Easy Bootloader for PIC24 and dsPIC33 Library API Source File

  File Name:
    EZBL_gUSBDrive.c

  Summary:
    Instances a default weak FILEIO_DRIVE_CONFIG structure called "gUSBDrive"
    when no user defined structure overrides it (only implemented when some code
    actually references gUSBDrive).

    This structure maps the FILEIO MLA library to a USB Mass Storage Device
    (MSD host mode) "thumb drive" type physical media.


 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Copyright (C) 2017 Microchip Technology Inc.

  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
  derivatives created by any person or entity by or on your behalf, exclusively
  with Microchip's products.  Microchip and its licensors retain all ownership
  and intellectual property rights in the accompanying software and in all
  derivatives here to.

  This software and any accompanying information is for suggestion only.  It
  does not modify Microchip's standard warranty for its products.  You agree
  that you are solely responsible for testing the software and determining its
  suitability.  Microchip has no obligation to modify, test, certify, or
  support the software.

  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
  TERMS.
*******************************************************************************/
// DOM-IGNORE-END

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Definitions from the MLA v2016_11_07 FILEIO library header files.

// Enumeration to define media error types
typedef enum
{
    MEDIA_NO_ERROR,                     // No errors
    MEDIA_DEVICE_NOT_PRESENT,           // The requested device is not present
    MEDIA_CANNOT_INITIALIZE             // Cannot initialize media
} FILEIO_MEDIA_ERRORS;

// Media information flags.  The driver's MediaInitialize function will return a pointer to one of these structures.
typedef struct
{
    FILEIO_MEDIA_ERRORS errorCode;              // The status of the initialization FILEIO_MEDIA_ERRORS
    // Flags
    union
    {
        uint8_t    value;
        struct
        {
            uint8_t    sectorSize  : 1;         // The sector size parameter is valid.
            uint8_t    maxLUN      : 1;         // The max LUN parameter is valid.
        }   bits;
    } validityFlags;

    uint16_t    sectorSize;                     // The sector size of the target device.
    uint8_t    maxLUN;                          // The maximum Logical Unit Number of the device.
} FILEIO_MEDIA_INFORMATION;

typedef void (*FILEIO_DRIVER_IOInitialize)(void * mediaConfig);
typedef bool (*FILEIO_DRIVER_MediaDetect)(void * mediaConfig);
typedef FILEIO_MEDIA_INFORMATION * (*FILEIO_DRIVER_MediaInitialize)(void * mediaConfig);
typedef bool (*FILEIO_DRIVER_MediaDeinitialize)(void * mediaConfig);
typedef bool (*FILEIO_DRIVER_SectorRead)(void * mediaConfig, uint32_t sector_addr, uint8_t* buffer);
typedef uint8_t (*FILEIO_DRIVER_SectorWrite)(void * mediaConfig, uint32_t sector_addr, uint8_t* buffer, bool allowWriteToZero);
typedef bool (*FILEIO_DRIVER_WriteProtectStateGet)(void * mediaConfig);

// Function pointer table that describes a drive being configured by the user
typedef struct
{
    FILEIO_DRIVER_IOInitialize funcIOInit;                          // I/O Initialization function
    FILEIO_DRIVER_MediaDetect funcMediaDetect;                      // Media Detection function
    FILEIO_DRIVER_MediaInitialize funcMediaInit;                    // Media Initialization function
    FILEIO_DRIVER_MediaDeinitialize funcMediaDeinit;                // Media Deinitialization function.
    FILEIO_DRIVER_SectorRead funcSectorRead;                        // Function to read a sector of the media.
    FILEIO_DRIVER_SectorWrite funcSectorWrite;                      // Function to write a sector of the media.
    FILEIO_DRIVER_WriteProtectStateGet funcWriteProtectGet;         // Function to determine if the media is write-protected.
} FILEIO_DRIVE_CONFIG;


// Default prototypes for USB Host MSD (Mass Storage Device) physical layer
uint8_t USBHostMSDSCSIMediaDetect(uint8_t * address);
FILEIO_MEDIA_INFORMATION * USBHostMSDSCSIMediaInitialize(uint8_t * address);
bool USBHostMSDSCSIMediaDeinitialize(void *mediaConfig);
uint8_t USBHostMSDSCSISectorRead(uint8_t * address, uint32_t sectorAddress, uint8_t *dataBuffer);
uint8_t USBHostMSDSCSISectorWrite(uint8_t * address, uint32_t sectorAddress, uint8_t *dataBuffer, uint8_t allowWriteToZero);
uint8_t USBHostMSDSCSIWriteProtectState( uint8_t * address);


// Declare a FILEIO_DRIVE_CONFIG structure to describe which functions the File I/O library will use to communicate with the media
const __attribute__((weak)) FILEIO_DRIVE_CONFIG gUSBDrive =
{
    (FILEIO_DRIVER_IOInitialize)NULL,                                       // Function to initialize the I/O pins used by the driver.
    (FILEIO_DRIVER_MediaDetect)USBHostMSDSCSIMediaDetect,                   // Function to detect that the media is inserted.
    (FILEIO_DRIVER_MediaInitialize)USBHostMSDSCSIMediaInitialize,           // Function to initialize the media.
    (FILEIO_DRIVER_MediaDeinitialize)USBHostMSDSCSIMediaDeinitialize,       // Function to de-initialize the media.
    (FILEIO_DRIVER_SectorRead)USBHostMSDSCSISectorRead,                     // Function to read a sector from the media.
    (FILEIO_DRIVER_SectorWrite)USBHostMSDSCSISectorWrite,                   // Function to write a sector to the media.
    (FILEIO_DRIVER_WriteProtectStateGet)USBHostMSDSCSIWriteProtectState,    // Function to determine if the media is write-protected.
};