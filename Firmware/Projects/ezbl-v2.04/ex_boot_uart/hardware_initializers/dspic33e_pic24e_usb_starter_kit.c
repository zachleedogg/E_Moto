/*
 * File:   dspic33e_pic24e_usb_starter_kit.c
 *
 * Initializes the device configuration fuses, clock frequency, LED I/O, 
 * Button I/O, and clock frequency for the dsPIC33E USB Starter Kit and PIC24E 
 * USB Starter Kit.
 *
 * dsPIC33EP512MU810 product page:
 *   http://www.microchip.com/dspic33ep512mu810
 */

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

#if defined(__dsPIC33EP512MU810__) || defined(__PIC24EP512GU810__)


#define FCY         70000000ul      // Changing this automatically changes the PLL settings to run at this target frequency

#include <xc.h>
#include "../ezbl_integration/ezbl.h"


// Device Configuration Words
// Config words can be defined in a Bootloader project, Application project,
// or mixed between each other so long as any given flash/config write-word-
// size location has exectly one definition. On devices with flash Config
// words and a flash double word minimum programming size (0x4 program
// addresses), this means two adjacent Config words may not be mixed between
// projects and instead both Config words must be defined in the same project.
//
// These defaults place all (or almost all) in the Bootloader project as this
// is the safest from a bootloader-bricking standpoint.
#if defined(EZBL_BOOT_PROJECT)  // Compiling for a Bootloader Project
    EZBL_SET_CONF(_FGS, GWRP_OFF & GSS_OFF & GSSK_OFF)                                       // Code Protect OFF, Write Protect OFF
    EZBL_SET_CONF(_FOSCSEL, FNOSC_FRC & IESO_OFF)                                            // Start with FRC then run-time switch to FRC+PLL after configuring PLL
    EZBL_SET_CONF(_FICD, ICS_PGD1 & JTAGEN_OFF)
    EZBL_SET_CONF(_FWDT, WDTPOST_PS1024 & WDTPRE_PR32 & PLLKEN_ON & WINDIS_OFF & FWDTEN_OFF) // ~1024ms timeout when watchdog is turned on in software (not forced on in hardware), wait for PLL locks on clock switch, use ordinary (non-windowed) Watchdog Timer mode
    EZBL_SET_CONF(_FOSC, POSCMD_XT & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSECME)               // XT Primary Oscillator, enable run-time clock switching and fail-safe clock monitor
    EZBL_SET_CONF(_FPOR, FPWRT_PWR16 & BOREN_ON & ALTI2C1_ON & ALTI2C2_OFF)                  // 16ms POR Power-up Timer, BOR Brown Out Reset enabled, Alternate I2C1 (USB devices have D+/D- where I2C1 pins would ordinarily be) and ordinary I2C2 pinout
#else   // Compiling for an Application Project (EZBL_BOOT_PROJECT is not defined)
#endif  // Goes to: #if defined(EZBL_BOOT_PROJECT)



//const unsigned int EZBL_i2cSlaveAddr = 0x60;          // Define I2C Slave Address that this Bootloader will listen/respond to, applicable only if I2C_Reset() is called
EZBL_FIFO *EZBL_COMBootIF __attribute__((persistent));  // Pointer to RX FIFO to check activity on for bootloading
const long EZBL_COMBaud = -230400;                      // Communications baud rate: <= 0 means auto-baud with default value (for TX) set by the 2's complemented value; otherwise baud in bits/sec (ex: 460800)



/**
 * Initializes system level hardware, such as the system clock source (PLL),
 * I/O pins for LED status indication, a timer for the NOW_*() timekeeping and
 * scheduling APIs and one or more communications peripherals for EZBL
 * bootloading.
 *
 * Although not required or used for bootloading, this function also initializes
 * I/O pins for push buttons, LCD interfacing and some extra items commonly on
 * Microchip development boards. When porting to other hardware, the extra
 * initialization code can be deleted.
 *
 * @return unsigned long system execution clock, in instructions/second (FCY).
 *
 *         One 16-bit timer peripheral will be enabled, along with it's ISR at a
 *         a period of 65536 system clocks. The timer/CCP used for this is
 *         selected within this code using the NOW_Reset() macro.
 *
 *         At least one communications is also selected and initialized, along
 *         with 1 or 2 ISRs for it (UART RX + TX ISRs or Slave I2C (no Master
 *         I2C).
 */
// @return: FCY clock speed we just configured the processor for
unsigned long InitializeBoard(void)
{
    // Switch to FRC clock (no PLL), in case if the PLL is currently in use.
    // We are not allowed to change the PLL prescalar, postscalar or feedback
    // divider while it is running.
    __builtin_write_OSCCONH(0x00);
    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);
    while(OSCCONbits.OSWEN);            // Wait for clock switch to complete

//    // Configure PLL for Fosc = 140MHz/Fcy = 70MIPS using 7.37 MHz internal FRC oscillator
//    CLKDIV = 0xB000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 2:1
//    PLLFBD = (FCY * 2u * 2u * 2u + 7370000u / 2u) / 7370000u - 2u; // 74 @ 70 MIPS (7.37 MHz input clock from FRC)
//    __builtin_write_OSCCONH(0x01);      // Initiate Clock Switch to use the FRC Oscillator + PLL (NOSC = 0b001)
//    __builtin_write_OSCCONL(0x01);

    // Configure PLL for Fosc = 140MHz/Fcy = 70MIPS using 8.000 MHz Primary Oscillator
    CLKDIV = 0xB000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 2:1
    PLLFBD = (FCY * 2u * 2u * 2u + 8000000u / 2u) / 8000000u - 2u; // 68 @ 70 MIPS (8.000 MHz input clock)
    __builtin_write_OSCCONH(0x03);      // Initiate Clock Switch to use the Primary Oscillator with PLL (NOSC = 0b011)
    __builtin_write_OSCCONL(0x01);

    // Wait for clock switch to complete and PLL to be locked (if enabled)
    while(OSCCONbits.OSWEN);            // Wait for clock switch to complete
    if((OSCCONbits.COSC & 0x5) == 0x1)  // 0x0 = Fast RC Oscillator (FRC); 0x1 = Fast RC Oscillator (FRC) with Divide-by-N and PLL; 0x2 = Primary Oscillator (XT, HS, EC); 0x3 = Primary Oscillator (XT, HS, EC) with PLL; 0x4 = Secondary Oscillator (SOSC); 0x5 = Low-Power RC Oscillator (LPRC); 0x6 = Fast RC Oscillator (FRC) with Divide-by-16; 0x7 = Fast RC Oscillator (FRC) with Divide-by-N
    {
        while(!_LOCK);
    }


    // Configure the auxiliary PLL for 48 MHz output needed for USB. Since the 
    // primary oscillator provides the source clock to the auxiliary
    // PLL, the auxiliary oscillator is disabled. Note that the AUX PLL is 
    // enabled. The input 8MHz clock is divided by 2, multiplied by 24 and then 
    // divided by 2.
    ACLKCON3 = 0x24C1;
    ACLKDIV3 = 0x7;
    _ENAPLL  = 1;


    // Initialize/select 16-bit hardware timer for NOW time keeping/scheduling
    // APIs. This call selects the hardware timer resource (can be TMR1-TMR6 or
    // CCP1-CCP8, if available) and the _Tx/_CCTxInterrupt gets automatically
    // implemented by code in ezbl_lib.a.
    NOW_Reset(TMR1, FCY);


    // Set push buttons as GPIO inputs
    EZBL_DefineButtonMap(RA7, RD7, RD6);
    _ANSD6  = 0;
    _ANSD7  = 0;
    _ANSA7  = 0;
    _TRISD7 = 1;
    _TRISD6 = 1;
    _TRISA7 = 1;

    // Set LED pins as GPIO outputs
    EZBL_DefineLEDMap(RD2, RD1, RD0);
    LATD   &= ~(1 | 1<<1 | 1<<2);
    TRISD  &= ~(1 | 1<<1 | 1<<2);

    // Turn on the USB host regulator switch (U7 - FPF2100) on the board
    _TRISB5 = 0;
    _LATB5  = 1;

    // Wait for the auxiliary PLL to lock 
    while(!_APLLCK);
    
    // Report 70 MIPS on dsPIC33E
    return FCY;
}


#endif //#if defined(__dsPIC33EP512MU810__) || defined(__PIC24EP512GU810__)
