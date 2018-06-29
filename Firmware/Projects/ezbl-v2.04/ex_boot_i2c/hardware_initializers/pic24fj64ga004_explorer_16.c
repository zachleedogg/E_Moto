/*
 * File:   pic24fj64ga004_explorer_16.c
 *
 * Initializes the device configuration fuses, clock frequency, UART2 pins,
 * LED I/O, Button I/O, LCD and 25LC256 SPI2 pins for the PIC24FJ64GA004 PIM on
 * the Explorer 16 or Explorer 16/32 development board. For I2C projects, I2C 2
 * is configured for use with SCL2/SDA2.
 *
 * PIC24FJ64GA004 product page:
 *   http://www.microchip.com/pic24fj64ga004
 *
 * PIC24FJ64GA004 PIM Info Sheet and schematic:
 *   http://ww1.microchip.com/downloads/en/DeviceDoc/PIM_24Fj64ga004_51663a.pdf
 *
 * Explorer 16/32 schematic:
 *   http://ww1.microchip.com/downloads/en/DeviceDoc/Explorer_16_32_Schematics_R6_3.pdf
 *
 * For the Explorer 16 (original version) schematic:
 *   http://ww1.microchip.com/downloads/en/DeviceDoc/DM240001%20BOM%20and%20Schematics.pdf
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

#if defined(__PIC24FJ64GA004__) || defined(__PIC24FJ48GA004__) || defined(__PIC24FJ32GA004__) || defined(__PIC24FJ16GA004__)


#define FCY         16000000ul

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
    EZBL_SET_CONF(_CONFIG2, POSCMOD_XT & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRC & SOSCSEL_SOSC & WUTSEL_FST & IESO_OFF)
    EZBL_SET_CONF(_CONFIG1, WDTPS_PS1024 & FWPSA_PR32 & WINDIS_ON & FWDTEN_OFF & ICS_PGx1 & COE_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)                      // Code Protect = OFF, Write Protect = OFF
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
    // We should not be changing the PLL prescalar, postscalar or feedback
    // divider (if present) while the PLL is clocking anything.
    __builtin_write_OSCCONH(0x00);
    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);
    while(OSCCONbits.OSWEN);            // Wait for clock switch to complete

    // Configure PLL for Fosc = 32MHz/Fcy = 16MIPS using 8 MHz internal FRC oscillator
    CLKDIV = 0x0000;    // Set 1:1 8MHz FRC postscalar so 4x PLL gives 16 MIPS (32MHz)
    __builtin_write_OSCCONH(0x01);      // Initiate Clock Switch to use the FRC Oscillator + PLL (NOSC = 0b001)
    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);

    // Wait for clock switch to complete and PLL to be locked (if enabled)
    while(OSCCONbits.OSWEN);            // Wait for clock switch to complete
    if((OSCCONbits.COSC & 0x5) == 0x1)  // 0x0 = Fast RC Oscillator (FRC); 0x1 = Fast RC Oscillator (FRC) with Divide-by-N and PLL; 0x2 = Primary Oscillator (XT, HS, EC); 0x3 = Primary Oscillator (XT, HS, EC) with PLL; 0x4 = Secondary Oscillator (SOSC); 0x5 = Low-Power RC Oscillator (LPRC); 0x6 = Fast RC Oscillator (FRC) with Divide-by-16; 0x7 = Fast RC Oscillator (FRC) with Divide-by-N
    {
        while(!_LOCK);
    }


    // Initialize/select 16-bit hardware timer for NOW time keeping/scheduling
    // APIs. This call selects the hardware timer resource (can be TMR1-TMR6 or
    // CCP1-CCP8, if available) and the _Tx/_CCTxInterrupt gets automatically
    // implemented by code in ezbl_lib.a.
    NOW_Reset(TMR1, FCY);


    // Set push buttons as GPIO inputs
    //
    // Function     Explorer 16 PIM Header      PIC24FJ64GA004 Device Pins
    // Button       PIM#, PICtail#, name        PIC#, name
    // S4 (LSb)       80, 106, RD13               13, TCK/PMPA7/RA7               // Muxed with LED D4  (LED bit 1) -> Configured for button use
    // S5             92,  74, RA7                 2, RP22/CN18/PMPA1/RC6         // Muxed with LED D10 (LED bit 7) -> Configured for button use
    // S6             84, 100, RD7                35, TDI/PMPA9/RA9               // Muxed with LED D7  (LED bit 4) -> Configured for button use
    // S3 (MSb)       83,  99, RD6                12, TMS/PMPA10/RA10             // Muxed with LED D3  (LED bit 0) -> Configured for button use
    EZBL_DefineButtonMap(RA10, RA9, RC6, RA7);
    _TRISA7 = 1;
    _TRISC6 = 1;
    _TRISA9 = 1;
    _TRISA10 = 1;


    // Set LED pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      PIC24FJ64GA004 Device Pins
    // LED          PIM#, PICtail#, name        PIC#, name
    // D3 (LSb)       17, 69, RA0/TMS             12, TMS/PMPA10/RA10             // Muxed with Button S3 (Button bit 3) ->  Configured for Button Use
    // D4             38, 70, RA1/TCK             13, TCK/PMPA7/RA7               // Muxed with Button S4 (Button bit 0) ->  Configured for Button Use
    // D5             58, 38, RA2/SCL2            44, RP8/SCL1/CN22/PMPD4/RB8     // Muxed with LCD Data 4
    // D6             59, 40, RA3/SDA2             1, RP9/SDA1/CN21/PMPD3/RB9     // Muxed with LCD Data 3
    // D7             60, 71, RA4/TDI             35, TDI/PMPA9/RA9               // Muxed with Button S6 (Button bit 2) ->  Configured for Button Use
    // D8             61, 72, RA5/TDO             32, TDO/PMPA8/RA8               // Muxed with SPI2 25LC256 Chip Select ->  Both outputs so can coexist, but do not use if you care about EEPROM contents
    // D9             91, 73, RA6                 10, RP12/CN14/PMPD0/RB12        // Muxed with LCD Data 0
    // D10 (MSb)      92, 74, RA7                  2, RP22/CN18/PMPA1/RC6         // Muxed with Button S5 (Button bit 1) ->  Configured for Button Use
    EZBL_DefineLEDMap(RB12, RA8, RB9, RB8);
    //_LATA10   = 0;  // BUTTON3
    //_LATA7    = 0;  // BUTTON0
    _LATB8    = 0;
    _LATB9    = 0;
    //_LATA9    = 0;  // BUTTON2
    _LATA8    = 1;  // !CS on 25LC256 - Set to 1 because of SPI2 25LC256 !CS muxing
    _LATB12   = 0;
    //_LATC6    = 0;  // BUTTON1
    //_TRISA10  = 0;  // BUTTON3
    //_TRISA7   = 0;  // BUTTON0    
    _TRISB8   = 0;
    _TRISB9   = 0;
    //_TRISA9   = 0;  // BUTTON2
    _TRISA8   = 0;  // !CS on 25LC256
    _TRISB12  = 0;
    //_TRISC6   = 0;  // BUTTON1


    // Set 16x2 character LCD pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      PIC24FJ64GA004 Device Pins
    // 16x2 LCD     PIM#, PICtail#, name        PIC#, name
    // Data 0         93, 109, RE0/PMPD0          10, RP12/CN14/PMPD0/RB12       // Muxed with LED D9 (LED bit 6), but compatible as both are outputs
    // Data 1         94, 110, RE1/PMPD1           9, RP11/CN15/PMPD1/RB11
    // Data 2         98, 111, RE2/PMPD2           8, RP10/CN16/PMPD2/RB10
    // Data 3         99, 112, RE3/PMPD3           1, RP9/SDA1/CN21/PMPD3/RB9    // Muxed with LED D6 (LED bit 3), but compatible as both are outputs
    // Data 4        100, 113, RE4/PMPD4          44, RP8/SCL1/CN22/PMPD4/RB8    // Muxed with LED D5 (LED bit 2), but compatible as both are outputs
    // Data 5          3, 114, RE5/PMPD5          43, RP7/INT0/CN23/PMPD5/RB7
    // Data 6          4, 115, RE6/PMPD6          42, RP6/ASCL1/CN24/PMPD6/RB6
    // Data 7          5, 116, RE7/PMPD7          41, RP5/ASDA1/CN27/PMPD7/RB5
    // E (Enable)     81,  97, RD4/PMPWR          14, AN10/CVREF/RTCC/RP14/CN12/PMPWR/RB14
    // R/!W           82,  98, RD5/PMPRD          11, RP13/CN13/PMPRD/RB13
    // RS (Reg Sel)   44,  84, RB15/PMPA0          3, RP23/CN17/PMPA0/RC4
    LATB   &= ~(1<<12 | 1<<11 | 1<<10 | 1<<9 | 1<<8 | 1<<7 | 1<<6 | 1<<5 | 1<<14 | 1<<13);
    TRISB  &= ~(1<<12 | 1<<11 | 1<<10 | 1<<9 | 1<<8 | 1<<7 | 1<<6 | 1<<5 | 1<<14 | 1<<13);
    _LATC4  = 0;
    _TRISC4 = 0;
    _PCFG10 = 1;    // 1 = digital; 0 = analog



    // Configure pins for 25LC256 (32Kbyte SPI EEPROM)
    // - Pin names are with respect to the PIC, which is the SPI Master.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   SPI hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      PIC24FJ64GA004 Device Pins
    // SPI2         PIM#, PICtail#, name        PIC#, name
    // !CS   (out)    79, 105, RD12               32, TDO/PMPA8/RA8               // Muxed with LED D8 (LED bit 5), compatible as both are outputs
    // SCK2  (out)    10,  35, RG6/PMPA5/SCK2      4, RP24/CN20/PMPA5/RC8
    // SDI2  (in)     11,  37, RG7/PMPA4/SDI2     37, RP20/CN25/PMPA4/RC4
    // SDO2  (out)    12,  39, RG8/PMPA3/SDO2     38, RP21/CN26/PMPA3/RC5
    _LATA8   = 1;   // 1 is inactive
    _TRISA8  = 0;   // !CS on RA8
    _CN25PUE = 1;   // Turn on pull up on SDI2 so it doesn't float when SPI module tri-states it
    _SDI2R   = 20;  // MISO2 (SDI2) on RP20
    _RP24R   = 11;  //_RPOUT_SCK2  // SCK2 on RP24
    _RP21R   = 10;  //_RPOUT_SDO2  // MOSI2 (SDO2) on RP21


#if defined(XPRJ_uart)  // Defined by MPLAB X on command line when compiling "uart" Build Configuration
    // Configure UART2 pins as UART.
    // - Pin names are with respect to the PIC.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   UART2 hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      PIC24FJ64GA004 Device Pins
    // UART2        PIM#, PICtail#, name        PIC#, name
    // U2RX  (in)     49, 34, RF4/PMPA9/U2RX      36, RP19/CN28/PMPBE/RC3
    // U2TX  (out)    50, 36, RF5/PMPA8/U2TX       5, RP25/CN19/PMPA6/RC9
    _CN28PUE = 1;               // Turn on weak pull up on U2RX so no spurious data arrives if nobody connected
    _U2RXR   = 19;              // U2RX on RP19
    _RP25R   = _RPOUT_U2TX;     // U2TX on RP25
    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 1);
#endif


#if defined(XPRJ_i2c)   // Defined by MPLAB X on command line when compiling "i2c" Build Configuration
    // Configure I2C2 pins for MCP2221A
    // Function         Explorer 16 PIM Header          PIC24FJ64GA004 Device Pins
    // I2C 2            PIM#, PICtail#, name            PIC#, name
    // SDA2 (in/out)      56, 8, RG3/SDA1                 23, AN4/C1IN-/RP2/SDA2/CN6/RB2
    // SCL2 (in/out)      57, 6, RG2/SCL1                 24, AN5/C1IN+/RP3/SCL2/CN7/RB3
    _CN6PUE  = 1;       // Enable Weak pull up on SDA1 line so pins float to idle state if not connected or externally pulled up
    _CN7PUE  = 1;       // Enable Weak pull up on SCL1 line so pins float to idle state if not connected or externally pulled up
    _PCFG4   = 1;       // 1 = digital; 0 = analog
    _PCFG5   = 1;       // 1 = digital; 0 = analog
    _RP2R    = 0;       // Null output function - disable any PPS assignment on this pin since SDA2 function comes after RPx (I2C is lower priority)
    _RP3R    = 0;       // Null output function - disable any PPS assignment on this pin since SCL2 function comes after RPx (I2C is lower priority)
    EZBL_COMBootIF = I2C_Reset(2, FCY, 0, EZBL_i2cSlaveAddr, 0);
#endif


    // Configure Analog Inputs for U4 TC1047A Temperature Sensor and R6 10K Potentiometer
    //
    // Function     Explorer 16 PIM Header      PIC24FJ64GA004 Device Pins
    // Analog Input PIM#, PICtail#, name        PIC#, name
    // TC1047A Temp   21, 14, RB4/AN4             25, AN6/RP16/CN8/RC0
    // 10K Pot        20, 77, RB5/AN5             26, AN7/RP17/CN9/RC1
    _PCFG6  = 0;    // 1 = digital; 0 = analog
    _PCFG7  = 0;    // 1 = digital; 0 = analog
    _TRISC0 = 1;
    _TRISC1 = 1;


    // Report 16 MIPS on PIC24F
    return FCY;
}


#endif //#if defined(__PIC24FJ64GA004__) || defined(__PIC24FJ48GA004__) || defined(__PIC24FJ32GA004__) || defined(__PIC24FJ16GA004__)
