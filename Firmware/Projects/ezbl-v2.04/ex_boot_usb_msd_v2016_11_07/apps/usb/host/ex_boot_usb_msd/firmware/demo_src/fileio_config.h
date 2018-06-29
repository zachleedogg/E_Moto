/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

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

#ifndef _FS_DEF_
#define _FS_DEF_

// Macro indicating how many drives can be mounted simultaneously.
#define FILEIO_CONFIG_MAX_DRIVES        1

// Defines a character to use as a delimiter for directories.  Forward slash ('/') or backslash ('\\') is recommended.
#define FILEIO_CONFIG_DELIMITER '/'

// Macro defining the maximum supported sector size for the FILEIO module.  This value should always be 512 , 1024, 2048, or 4096 bytes.
// Most media uses 512-byte sector sizes.
#define FILEIO_CONFIG_MEDIA_SECTOR_SIZE 		512

/* *******************************************************************************************************/
/************** Compiler options to enable/Disable Features based on user's application ******************/
/* *******************************************************************************************************/

/**********************************************************************
  Define FILEIO_CONFIG_FUNCTION_SEARCH to disable the functions used to
  search for files.                                                    
  **********************************************************************/
#define FILEIO_CONFIG_SEARCH_DISABLE

// Define FILEIO_CONFIG_FUNCTION_WRITE to disable the functions that write to a drive.  Disabling this feature will
// force the file system into read-only mode.
#define FILEIO_CONFIG_WRITE_DISABLE

// Define FILEIO_CONFIG_FUNCTION_FORMAT to disable the function used to format drives.
#define FILEIO_CONFIG_FORMAT_DISABLE

// Define FILEIO_CONFIG_FUNCTION_DIRECTORY to disable use of directories on your drive.  Disabling this feature will
// limit you to performing all file operations in the root directory.
#define FILEIO_CONFIG_DIRECTORY_DISABLE

// Define FILEIO_CONFIG_FUNCTION_DRIVE_PROPERTIES to disable the FILEIO_DrivePropertiesGet function.  This function
// will determine the properties of your device, including unused memory.
#define FILEIO_CONFIG_DRIVE_PROPERTIES_DISABLE

// Define FILEIO_CONFIG_MULTIPLE_BUFFER_MODE_DISABLE to disable multiple buffer mode.  This will force the library to
// use a single instance of the FAT and Data buffer.  Otherwise, it will use one FAT buffer and one data buffer per drive
// (defined by FILEIO_CONFIG_MAX_DRIVES).  If you are only using one drive in your application, this option has no effect.
#define FILEIO_CONFIG_MULTIPLE_BUFFER_MODE_DISABLE

#endif
