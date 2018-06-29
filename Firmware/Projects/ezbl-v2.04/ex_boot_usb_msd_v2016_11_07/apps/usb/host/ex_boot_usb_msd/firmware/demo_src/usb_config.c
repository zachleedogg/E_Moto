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

// Created by the Microchip USBConfig Utility, Version 2.0.0.0, 11/18/2008, 8:08:56

#include "usb.h"
#include "usb_host_msd.h"
#include "usb_host_msd_scsi.h"

// *****************************************************************************
// Media Interface Function Pointer Table for the Mass Storage client driver
// *****************************************************************************

CLIENT_DRIVER_TABLE usbMediaInterfaceTable =
{                                           
    USBHostMSDSCSIInitialize,
    USBHostMSDSCSIEventHandler,
    0
};

// *****************************************************************************
// Client Driver Function Pointer Table for the USB Embedded Host foundation
// *****************************************************************************

CLIENT_DRIVER_TABLE usbClientDrvTable[] =
{                                        
    {
        USBHostMSDInitialize,
        USBHostMSDEventHandler,
        0
    }
};

// *****************************************************************************
// USB Embedded Host Targeted Peripheral List (TPL)
// *****************************************************************************

USB_TPL usbTPL[] =
{
    { INIT_CL_SC_P( 8ul, 6ul, 0x50ul ), 0, 0, {TPL_CLASS_DRV} } // Most types of MSD flash drives
    ,
    { INIT_CL_SC_P( 8ul, 5ul, 0x50ul ), 0, 0, {TPL_CLASS_DRV} } // Some MSD flash drives use this instead
};



