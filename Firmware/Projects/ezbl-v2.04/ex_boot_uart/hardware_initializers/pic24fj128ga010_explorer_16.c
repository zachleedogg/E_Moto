/*
 * File:   pic24fj128ga010_explorer_16.c
 *
 * Initializes the device configuration fuses, clock frequency, UART2 pins,
 * LED I/O, Button I/O, and 25LC256 SPI2 pins for the PIC24FJ128GA010 PIM on
 * the Explorer 16 or Explorer 16/32 development board. For I2C projects, I2C 1
 * is configured for use with SCL1/SDA1.
 *
 * PIC24FJ128GA010 product page:
 *   http://www.microchip.com/pic24fj128ga010
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

#if defined(__PIC24FJ128GA010__) || defined(__PIC24FJ96GA010__) || defined(__PIC24FJ64GA010__)

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
    EZBL_SET_CONF(_CONFIG2, POSCMOD_XT & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRC & IESO_OFF)
    EZBL_SET_CONF(_CONFIG1, WDTPS_PS1024 & FWPSA_PR32 & WINDIS_ON & FWDTEN_OFF & ICS_PGx2 & COE_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
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
    // Function     Explorer 16 PIM Header      PIC24FJ128GA010 Device Pins
    // Button       PIM#, PICtail#, name        PIC#, name
    // S4 (LSb)       80, 106, RD13               80, CN19/RD13
    // S5             92,  74, RA7                92, RA7                 <- Pin function is muxed with LED D10; S5 button can't be used because LED clamps weak 10k pull up voltage too low
    // S6             84, 100, RD7                84, CN16/RD7
    // S3 (MSb)       83,  99, RD6                83, CN15/RD6
    EZBL_DefineButtonMap(RD6, RD7, RD13);
    _TRISD13 = 1;
    //_TRISA7  = 1;
    _TRISD7  = 1;
    _TRISD6  = 1;


    // Set LED pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      PIC24FJ128GA010 Device Pins
    // LED          PIM#, PICtail#, name        PIC#, name
    // D3 (LSb)       17, 69, RA0/TMS             17, TMS/RA0
    // D4             38, 70, RA1/TCK             38, TCK/RA1
    // D5             58, 38, RA2/SCL2            58, SCL2/RA2
    // D6             59, 40, RA3/SDA2            59, SDA2/RA3
    // D7             60, 71, RA4/TDI             60, TDI/RA4
    // D8             61, 72, RA5/TDO             61, TDO/RA5
    // D9             91, 73, RA6                 91, RA6
    // D10 (MSb)      92, 74, RA7                 92, RA7         <- Pin function is muxed with button S5; we will use it as an LED output only
    EZBL_DefineLEDMap(RA7, RA6, RA5, RA4, RA3, RA2, RA1, RA0);
    *((volatile unsigned char *)&LATA)  = 0x00; // Write bits LATA<7:0> simultaneously, don't change LATA<15:8>
    *((volatile unsigned char *)&TRISA) = 0x00;


    // Set 16x2 character LCD pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      PIC24FJ128GA010 Device Pins
    // 16x2 LCD     PIM#, PICtail#, name        PIC#, name
    // Data 0         93, 109, RE0/PMPD0          93, PMD0/RE0
    // Data 1         94, 110, RE1/PMPD1          94, PMD1/RE1
    // Data 2         98, 111, RE2/PMPD2          98, PMD2/RE2
    // Data 3         99, 112, RE3/PMPD3          99, PMD3/RE3
    // Data 4        100, 113, RE4/PMPD4         100, PMD4/RE4
    // Data 5          3, 114, RE5/PMPD5           3, PMD5/RE5
    // Data 6          4, 115, RE6/PMPD6           4, PMD6/RE6
    // Data 7          5, 116, RE7/PMPD7           5, PMD7/RE7
    // E (Enable)     81,  97, RD4/PMPWR          81, PMWR/OC5/CN13/RD4
    // R/!W           82,  98, RD5/PMPRD          82, PMRD/CN14/RD5
    // RS (Reg Sel)   44,  84, RB15/PMPA0         44, PMA0/AN15/OCFB/CN12/RB15
    *((volatile unsigned char *)&LATE)  = 0x00; // Write bits LATE<7:0> simultaneously, don't change LATE<15:8>
    *((volatile unsigned char *)&TRISE) = 0x00;
    _LATD4    = 0;
    _LATD5    = 0;
    _TRISD4   = 0;
    _TRISD5   = 0;
    _LATB15   = 0;
    _TRISB15  = 0;
    _PCFG15   = 1;    // 1 = digital; 0 = analog


    // Configure pins for 25LC256 (32Kbyte SPI EEPROM). 
    // - Pin names are with respect to the PIC, which is the SPI Master.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   SPI hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      PIC24FJ128GA010 Device Pins
    // SPI2         PIM#, PICtail#, name        PIC#, name
    // !CS   (out)    79, 105, RD12               79, IC5/RD12
    // SCK2  (out)    10,  35, RG6/PMPA5/SCK2     10, PMA5/SCK2/CN8/RG6
    // SDI2  (in)     11,  37, RG7/PMPA4/SDI2     11, PMA4/SDI2/CN9/RG7
    // SDO2  (out)    12,  39, RG8/PMPA3/SDO2     12, PMA3/SDO2/CN10/RG8
    _LATD12  = 1;      // 1 is inactive
    _TRISD12 = 0;      // !CS on RD12
    _CN9PUE  = 1;      // Turn on pull up on SDI2 so it doesn't float when SPI module tri-states it


#if defined(XPRJ_uart)  // Defined by MPLAB X on command line when compiling "uart" Build Configuration
    // Configure UART2 pins as UART.
    // - Pin names are with respect to the PIC.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   UART2 hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      PIC24FJ128GA010 Device Pins
    // UART2        PIM#, PICtail#, name        PIC#, name
    // U2RX  (in)     49, 34, RF4/PMPA9/U2RX      49, PMA9/U2RX/CN17/RF4
    // U2TX  (out)    50, 36, RF5/PMPA8/U2TX      50, PMA8/U2TX/CN18/RF5
    _CN17PUE = 1;               // Turn on weak pull up on U2RX so no spurious data arrives if nobody connected
    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 1);
#endif


#if defined(XPRJ_i2c)   // Defined by MPLAB X on command line when compiling "i2c" Build Configuration
    // Configure I2C1 pins for MCP2221A
    // Function     Explorer 16 PIM Header      PIC24FJ128GA010 Device Pins
    // I2C 1        PIM#, PICtail#, name        PIC#, name
    // SDA1 (in/out)  56, 8, RG3/SDA1             56, SDA1/RG3
    // SCL1 (in/out)  57, 6, RG2/SCL1             57, SCL1/RG2
    EZBL_COMBootIF = I2C_Reset(1, FCY, 0, EZBL_i2cSlaveAddr, 0);
#endif


    // Configure Analog Inputs for U4 TC1047A Temperature Sensor and R6 10K Potentiometer
    //
    // Function     Explorer 16 PIM Header      PIC24FJ128GA010 Device Pins
    // Analog Input PIM#, PICtail#, name        PIC#, name
    // TC1047A Temp   21, 14, RB4/AN4             21, C1IN-/AN4/CN6/RB4
    // 10K Pot        20, 77, RB5/AN5             20, C1IN+/AN5/CN7/RB5
    _PCFG4  = 0;    // 1 = digital; 0 = analog
    _PCFG5  = 0;    // 1 = digital; 0 = analog
    _TRISB4 = 1;
    _TRISB5 = 1;


    // Report 16 MIPS on PIC24F
    return FCY;
}


#endif //#if defined(__PIC24FJ128GA010__) || defined(__PIC24FJ96GA010__) || defined(__PIC24FJ64GA010__)
