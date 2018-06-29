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

#include <xc.h>
#include "usb.h"

/** CONFIGURATION Bits **********************************************/
// FSEC
#pragma config BWRP = OFF               // Boot Segment Write Protect (Boot segment may be written)
#pragma config BSS = DISABLED           // Boot segment Protect (No Protection (other than BWRP))
#pragma config BSEN = OFF               // Boot Segment Control bit (No Boot Segment)
#pragma config GWRP = OFF               // General Segment Write Protect (Writes to program memory are allowed)
#pragma config GSS = DISABLED           // General Segment Code Protect (Code protection is disabled)
#pragma config CWRP = OFF               // Configuration Segment Program Write Protection bit (Configuration Segment may be written)
#pragma config CSS = DISABLED           // Configuration Segment Code Protection Level bits (No Protection (other than CWRP))
#pragma config AIVTDIS = OFF            // Alternate Interrupt Vector Table Disable bit (Disable AIVT)

// FOSCSEL and FOSC
#pragma config FNOSC = PRIPLL           // Oscillator Select (Primary Oscillator with PLL module (XTPLL, HSPLL, ECPLL))
#pragma config PLLMODE = PLL96DIV2      // Frequency Multiplier Select Bits (96 MHz PLL. Oscillator input is divided by 2 (8 MHz input).)
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config PLLSS = PLL_PRI          // PLL Secondary Selection Configuration bit (PLL is fed by the Primary oscillator)
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)
#pragma config SOSCSEL = ON             // SOSC Power Selection Configuration bits (SOSC is used in crystal (SOSCI/SOSCO) mode)
#pragma config IOL1WAY = OFF            // IOLOCK One-Way Set Enable (The IOLOCK bit can be set and cleared using the unlock sequence)
#pragma config FCKSM = CSECME           // Clock Switching and Monitor Selection (Clock switching enabled, Fail-Safe Clock Monitor enabled)

// FWDT
#pragma config WDTPS = PS2048           // Watchdog Timer Postscaler (1:2048)
#pragma config FWPSA = PR32             // WDT Prescaler (Prescaler ratio of 1:32)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard Watchdog Timer enabled (Windowed-mode is disabled))
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (Watch Dog Timer Window Width is 25 percent)
#pragma config WDTCMX = WDTCLK          // WDT Clock Source Select bits (WDT clock source is determined by the WDTCLK Configuration bits)
#pragma config WDTCLK = LPRC            // WDT Clock Source Select bits (WDT uses LPRC)

// FPOR
#pragma config BOREN = ON               // Brown-out Reset Enable bits (Brown-out Reset Enabled)
#pragma config LPCFG = OFF              // Low power regulator control (Disabled)

// FICD
#pragma config ICS = PGX2               // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC1/PGED1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)
#pragma config BTSWP = ON               // BOOTSWP Instruction Enable bit (BOOTSWP instruction is enabled, if applicable)

// FDEVOPT1
#pragma config ALTCMPI = DISABLE        // Alternate Comparator Input Enable bit (C1INC, C2INC, and C3INC are on their standard pin locations)
#pragma config TMPRPIN = OFF            // Tamper Pin Enable bit (TMPRN pin function is disabled)
#pragma config ALTVREF = ALTVREFDIS     // Alternate VREF location Enable (VREF is on a default pin (VREF+ on RA10 and VREF- on RA9))
 
/*********************************************************************
* Function: void SYS_Initialize(void)
*
* Overview: Initializes the system.
*
* PreCondition: None
*
* Input:  None
*
* Output: None
*
********************************************************************/
void SYS_Initialize()
{
}


/* Interrupt handler for USB host. */
void __attribute__((interrupt,auto_psv)) _USB1Interrupt()
{
    USB_HostInterruptHandler();
}