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
// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = OFF                // General Segment Code-Protect bit (General Segment Code protect is disabled)
#pragma config GSSK = OFF               // General Segment Key bits (General Segment Write Protection and Code Protection is Disabled)

// FOSCSEL
#pragma config FNOSC = FRC              // Initial Oscillator Source Selection Bits (Internal 7.37MHz Fast RC Oscillator)
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECME           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are enabled)

// FWDT
#pragma config WDTPOST = PS1024         // Watchdog Timer Postscaler Bits (1:1024, or ~1024ms @ 1:32 prescale)
#pragma config WDTPRE = PR32            // Watchdog Timer Prescaler bit (1:32)
#pragma config PLLKEN = ON              // PLL Lock Wait Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR16            // Power-on Reset Timer Value Select bits (16ms)
#pragma config BOREN = OFF              // Brown-out Reset (BOR) Detection Enable bit (BOR is disabled)
#pragma config ALTI2C1 = ON             // Alternate I2C pins for I2C1 (Alternate SDA1/SCK1 pins are selected as the I/O pins for I2C1)
#pragma config ALTI2C2 = OFF            // Alternate I2C pins for I2C2 (SDA2/SCK2 pins are selected as the I/O pins for I2C2)

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config RSTPRI = PF              // Reset Target Vector Select bit (Device will obtain reset instruction from Primary flash)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FAS
#pragma config AWRP = OFF               // Auxiliary Segment Write-protect bit (Auxiliary program memory is not write-protected)
#pragma config APL = OFF                // Auxiliary Segment Code-protect bit (Aux Flash Code protect is disabled)
#pragma config APLK = OFF               // Auxiliary Segment Key bits (Aux Flash Write Protection and Code Protection is Disabled)

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
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;
    ANSELE = 0x0000;
    ANSELG = 0x0000;

    // Configure the device PLL to obtain 60 MIPS operation. The crystal
    // frequency is 8MHz. Divide 8MHz by 2, multiply by 60 and divide by
    // 2. This results in Fosc of 120MHz. The CPU clock frequency is
    // Fcy = Fosc/2 = 60MHz. Wait for the Primary PLL to lock and then
    // configure the auxilliary PLL to provide 48MHz needed for USB
    // Operation.

    PLLFBD = 58;                        /* M  = 60  */
    CLKDIVbits.PLLPOST = 0;             /* N1 = 2   */
    CLKDIVbits.PLLPRE = 0;              /* N2 = 2   */
    OSCTUN = 0;

    /*	Initiate Clock Switch to Primary
     *	Oscillator with PLL (NOSC= 0x3)*/
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(0x01);

    while (OSCCONbits.COSC != 0x3);

    // Configuring the auxiliary PLL, since the primary
    // oscillator provides the source clock to the auxiliary
    // PLL, the auxiliary oscillator is disabled. Note that
    // the AUX PLL is enabled. The input 8MHz clock is divided
    // by 2, multiplied by 24 and then divided by 2. Wait till
    // the AUX PLL locks.

    ACLKCON3 = 0x24C1;
    ACLKDIV3 = 0x7;

    //Turn on the USB host regulator switch (U7 - FPF2100) on the board
    TRISBbits.TRISB5 = 0;
    LATBbits.LATB5 = 1;
    
    ACLKCON3bits.ENAPLL = 1;
    while(ACLKCON3bits.APLLCK != 1);
}


/* Interrupt handler for USB host. */
void __attribute__((interrupt,auto_psv)) _USB1Interrupt()
{
    USB_HostInterruptHandler();
}
