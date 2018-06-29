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

#ifndef _usb_config_h_
#define _usb_config_h_

#include <xc.h>

#define _USB_CONFIG_VERSION_MAJOR 2
#define _USB_CONFIG_VERSION_MINOR 0
#define _USB_CONFIG_VERSION_DOT   0
#define _USB_CONFIG_VERSION_BUILD 0

// Supported USB Configurations

#define USB_SUPPORT_HOST

// Hardware Configuration

//USB_PING_PONG__FULL_PING_PONG
#define USB_PING_PONG_MODE  USB_PING_PONG__FULL_PING_PONG 

// Host Configuration

#define NUM_TPL_ENTRIES 2
#define USB_NUM_CONTROL_NAKS 200
#define USB_SUPPORT_INTERRUPT_TRANSFERS
#define USB_NUM_INTERRUPT_NAKS 3
#define USB_SUPPORT_BULK_TRANSFERS
#define USB_NUM_BULK_NAKS 20000
//#define USB_SUPPORT_ISOCHRONOUS_TRANSFERS
#define USB_INITIAL_VBUS_CURRENT (100/2)
#define USB_INSERT_TIME (250+1)
#define USB_HOST_APP_EVENT_HANDLER USB_ApplicationEventHandler

// Host Mass Storage Client Driver Configuration

//#define USB_ENABLE_TRANSFER_EVENT

#define USB_MAX_MASS_STORAGE_DEVICES 1

// Helpful Macros

#define USBTasks()                  \
    {                               \
        USBHostTasks();             \
        USBHostMSDTasks();          \
    }

#define USBInitialize(x)            \
    {                               \
        USBHostInit(x);             \
    }


#endif

