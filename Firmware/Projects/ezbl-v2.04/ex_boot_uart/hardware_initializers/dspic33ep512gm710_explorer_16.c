/*
 * File:   dspic33ep512gm710_explorer_16.c
 *
 * Initializes the device configuration fuses, clock frequency, UART2 pins,
 * LED I/O, Button I/O, and 25LC256 SPI2 pins for the dsPIC33EP512GM710 PIM on
 * the Explorer 16 or Explorer 16/32 development board. For I2C projects, I2C 1
 * is configured for use with SCL1/SDA1.
 *
 * dsPIC33EP512GM710 product page:
 *   http://www.microchip.com/dspic33ep512gm710
 *
 * dsPIC33EP512GM710 PIM Info Sheet and schematic:
 *   http://ww1.microchip.com/downloads/en/DeviceDoc/PIM%20Info%20Sheet.pdf
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

#if defined(__dsPIC33EP512GM710__) || defined(__dsPIC33EP256GM710__) || defined(__dsPIC33EP128GM710__) || defined(__dsPIC33EP512GM310__) || defined(__dsPIC33EP256GM310__) || defined(__dsPIC33EP128GM310__)


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
    EZBL_SET_CONF(_FICD, ICS_PGD1 & JTAGEN_OFF)                                               // Debug using PGEC1 and PGED1, turn off JTAG to recover I/O pins
    EZBL_SET_CONF(_FPOR, BOREN_ON & ALTI2C1_OFF & ALTI2C2_OFF & WDTWIN_WIN75)                 // Enable Brown-out Reset (BOR), use primary I2C1 and I2C2 pin mappings, set watchdog window to 75% (not important in non-window mode, but included for completness)
    EZBL_SET_CONF(_FWDT, WDTPOST_PS1024 & WDTPRE_PR32 & PLLKEN_ON & WINDIS_OFF & FWDTEN_OFF)  // ~1024ms timeout when watchdog is turned on in software (not forced on in hardware), wait for PLL lock on clock switch, use ordinary (non-windowed) Watchdog Timer mode
    EZBL_SET_CONF(_FOSC, POSCMD_XT & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSECME)                // XT crystal oscillator, allow multiple PPS remappings, enable clock switching and safe clock monitor
    EZBL_SET_CONF(_FOSCSEL, FNOSC_FRC & PWMLOCK_OFF & IESO_OFF)                               // Start with FRC, then switch to FRC+PLL at run time
    EZBL_SET_CONF(_FGS, GWRP_OFF & GCP_OFF)                                                   // Code Protect = OFF, Write Protect = OFF (Don't enable Write Protect either if you plan to use a bootloader!)
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

    // Configure PLL for Fosc = 140MHz/Fcy = 70MIPS using 7.37 MHz internal FRC oscillator
    CLKDIV = 0xA000; // ROI = 1, DOZE = 4:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 2:1 - NOTE: Errata on this device family for DOZE 8:1 and slower
    PLLFBD = (FCY * 2u * 2u * 2u + 7370000u / 2u) / 7370000u - 2u; // 74 @ 70 MIPS (7.37 MHz input clock from FRC)
    __builtin_write_OSCCONH(0x01);      // Initiate Clock Switch to use the FRC Oscillator + PLL (NOSC = 0b001)
    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);

//    // Configure PLL for Fosc = 140MHz/Fcy = 70MIPS using 8.000 MHz Primary Oscillator (make sure to set Configuration Fuse to enable the OSC1/OSC2 oscillator)
//    CLKDIV = 0xA000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 2:1 - NOTE: Errata on this device family for DOZE 8:1 and slower
//    PLLFBD = (FCY * 2u * 2u * 2u + 8000000u / 2u) / 8000000u - 2u; // 68 @ 70 MIPS (8.000 MHz input clock)
//    __builtin_write_OSCCONH(0x03);      // Initiate Clock Switch to use the Primary Oscillator with PLL (NOSC = 0b011)
//    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);

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
    // Function     Explorer 16 PIM Header      dsPIC33EP512GM710 Device Pins
    // Button       PIM#, PICtail#, name        PIC#, name
    // S4 (LSb)       80, 106, RD13               80, RPI77/RD13
    // S5             92,  74, RA7                92, RF7             <- Pin function is muxed with LED D10; S5 button can't be used because LED clamps weak 10k pull up voltage too low
    // S6             84, 100, RD7                84, RP57/RC9
    // S3 (MSb)       83,  99, RD6                83, RP70/RD6
    EZBL_DefineButtonMap(RD6, RC9, RD13);
    _TRISD13 = 1;
    //_TRISF7 = 1;    // LED D10 (bit 7)
    _TRISC9  = 1;
    _TRISD6  = 1;


    // Set LED pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512GM710 Device Pins
    // LED          PIM#, PICtail#, name        PIC#, name
    // D3 (LSb)       17, 69, RA0/TMS             17, AN22/RG10
    // D4             38, 70, RA1/TCK             38, AN35/RG11
    // D5             58, 38, RA2/SCL2            58, AN42/RG2
    // D6             59, 40, RA3/SDA2            59, AN43/RG3
    // D7             60, 71, RA4/TDI             60, AN44/RF4
    // D8             61, 72, RA5/TDO             61, AN45/RF5
    // D9             91, 73, RA6                 91, RF6
    // D10 (MSb)      92, 74, RA7                 92, RF7             <- Pin function is muxed with button S5; we will use it as an LED output only
    EZBL_DefineLEDMap(RF7, RF6, RF5, RF4, RG3, RG2, RG11, RG10);
    LEDSet(0x00);   // Clear all LATx bits in the map
    TRISG  &= ~(1<<10 | 1<<11 | 1<<2 | 1<<3);
    ANSELG &= ~(1<<10 | 1<<11 | 1<<2 | 1<<3);
    TRISF  &= ~(1<<4 | 1<<5 | 1<<6 | 1<<7);
    ANSELF &= ~(1<<4 | 1<<5 | 1<<6 | 1<<7);


    // Set 16x2 character LCD pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512GM710 Device Pins
    // 16x2 LCD     PIM#, PICtail#, name        PIC#, name
    // Data 0         93, 109, RE0/PMPD0          93, RP42/PWM3H/PMD0/RB10
    // Data 1         94, 110, RE1/PMPD1          94, RP43/PWM3L/PMD1/RB11
    // Data 2         98, 111, RE2/PMPD2          98, RPI44/PWM2H/PMD2/RB12
    // Data 3         99, 112, RE3/PMPD3          99, RPI45/PWM2L/CTPLS/PMD3/RB13
    // Data 4        100, 113, RE4/PMPD4         100, TDO/PWM4H/PMD4/RA10
    // Data 5          3, 114, RE5/PMPD5           3, TDI/PWM4L/PMD5/RA7
    // Data 6          4, 115, RE6/PMPD6           4, RPI46/PWM1H/T3CK/T7CK/PMD6/RB14
    // Data 7          5, 116, RE7/PMPD7           5, RPI47/PWM1L/T5CK/T6CK/PMD7/RB15
    // E (Enable)     81,  97, RD4/PMPWR          81, RP56/PMWR/RC8
    // R/!W           82,  98, RD5/PMPRD          82, RP69/PMRD/RD5
    // RS (Reg Sel)   44,  84, RB15/PMPA0         44, AN15/RPI95/FLT8/PMA0/RE15
    LATB    &= ~(1<<10 | 1<<11 | 1<<12 | 1<<13 | 1<<14 | 1<<15);
    TRISB   &= ~(1<<10 | 1<<11 | 1<<12 | 1<<13 | 1<<14 | 1<<15);
    _LATA10  = 0;
    _TRISA10 = 0;
    _LATA7   = 0;
    _TRISA7  = 0;
    IOCON3bits.PENH = 0;    // Disable PWM3H control so RB10 GPIO can be used
    IOCON3bits.PENL = 0;    // Disable PWM3L control so RB11 GPIO can be used
    IOCON2bits.PENH = 0;    // Disable PWM2H control so RB12 GPIO can be used
    IOCON2bits.PENL = 0;    // Disable PWM2L control so RB13 GPIO can be used
    IOCON4bits.PENH = 0;    // Disable PWM4H control so RA10 GPIO can be used
    IOCON4bits.PENL = 0;    // Disable PWM4L control so RA7  GPIO can be used
    IOCON1bits.PENH = 0;    // Disable PWM1H control so RB14 GPIO can be used
    IOCON1bits.PENL = 0;    // Disable PWM1L control so RB15 GPIO can be used
    _LATC8   = 0;
    _TRISC8  = 0;
    _LATD5   = 0;
    _TRISD5  = 0;
    _LATE15  = 0;
    _TRISE15 = 0;
    _ANSE15  = 0;


    // Configure Pins for SPI1 on Explorer 16 (nothing attached on the Explorer
    // 16 directly, but useful for various PICtails).
    // - Pin names are with respect to the PIC, which is the SPI Master.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   SPI hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512GM710 Device Pins
    // SPI1         PIM#, PICtail#, name        PIC#, name
    // !CS   (out)    23,   1, RB2/SS1/AN2        23, OA2IN+/AN1/C2IN1+/RPI17/RA1
    // SCK1  (out)    55,   3, RF6/SCK1           55, AN29/SCK1/RPI51/RC3
    // SDI1  (in)     54,   5, RF7/SDI1           54, AN28/SDI1/RPI25/RA9
    // SDO1  (out)    53,   7, RF8/SDO1           51, OA5IN+/AN24/C5IN3-/C5IN1+/SDO1/RP20/T1CK/RA4
    _LATA1   = 1;   // 1 is inactive
    _TRISA1  = 0;   // !CS on RA1
    _ANSA1   = 0;
    _CNPDA9  = 1;   // Turn on SDI1 pull down so it doesn't float when slave chip is inactive
    _TRISC3  = 1;
    _TRISA9  = 1;
    _TRISA4  = 1;
    _ANSC3   = 0;
    _ANSA9   = 0;
    _ANSA4   = 0;


    // Configure SPI2 pins for 25LC256 (32Kbyte SPI EEPROM)
    // - Pin names are with respect to the PIC, which is the SPI Master.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   SPI hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512GM710 Device Pins
    // SPI2         PIM#, PICtail#, name        PIC#, name
    // !CS   (out)    79, 105, RD12               79, RPI76/RD12
    // SCK2  (out)    10,  35, RG6/PMPA5/SCK2     10, AN19/RP118/PMA5/RG6
    // SDI2  (in)     11,  37, RG7/PMPA4/SDI2     11, AN18/ASCL1/RPI119/PMA4/RG7
    // SDO2  (out)    12,  39, RG8/PMPA3/SDO2     12, AN17/ASDA1/RP120/PMA3/RG8
    _LATD12  = 1;               // 1 is inactive
    _TRISD12 = 0;               // !CS on RD12
    _CNPDG7  = 1;               // Turn on pull down on SDI2 so it doesn't float when SPI module tri-states it
    _ANSG6   = 0;
    _ANSG7   = 0;
    _ANSG8   = 0;
    _SDI2R   = 119;             // SDI2 (MISO2) on RPI119
    _RP118R  = _RPOUT_SCK2OUT;  // SCK2 on RP118
    _RP120R  = _RPOUT_SDO2;     // SDO2 (MOSI2) on RP120


#if defined(XPRJ_uart)  // Defined by MPLAB X on command line when compiling "uart" Build Configuration
    // Configure UART2 pins as UART.
    // - Pin names are with respect to the PIC.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   UART2 hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512GM710 Device Pins
    // UART2        PIM#, PICtail#, name        PIC#, name
    // U2RX  (in)     49, 34, RF4/PMPA9/U2RX      49, SDA2/RPI24/PMA9/RA8
    // U2TX  (out)    50, 36, RF5/PMPA8/U2TX      50, FLT32/SCL2/RP36/PMA8/RB4
    _CNPUA8  = 1;                           // Turn on weak pull up on U2RX so no spurious data arrives if nobody connected
    _U2RXR   = 24;                          // U2RX on RPI24
    _RP36R   = _RPOUT_U2TX;                 // U2TX on RP36
    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 1);
#endif


#if defined(XPRJ_i2c)   // Defined by MPLAB X on command line when compiling "i2c" Build Configuration
    // Configure I2C1 pins for MCP2221A
    // Function     Explorer 16 PIM Header      dsPIC33EP512GM710 Device Pins
    // I2C 1        PIM#, PICtail#, name        PIC#, name
    // SDA1 (in/out)  56, 8, RG3/SDA1            56, AN30/SDA1/RPI52/RC4
    // SCL1 (in/out)  57, 6, RG2/SCL1            57, AN31/SCL1/RPI53/RC5
    _CNPUC4 = 1;                          // Enable Weak pull up on SDA1 line so pins float to idle state if not connected or externally pulled up
    _CNPUC5 = 1;                          // Enable Weak pull up on SCL1 line so pins float to idle state if not connected or externally pulled up
    _ANSC4  = 0;
    _ANSC5  = 0;
    EZBL_COMBootIF = I2C_Reset(1, FCY, 0, EZBL_i2cSlaveAddr, 0);
#endif


    // Configure Analog Inputs for U4 TC1047A Temperature Sensor and R6 10K Potentiometer
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512GM710 Device Pins
    // Analog Input PIM#, PICtail#, name        PIC#, name
    // TC1047A Temp   21, 14, RB4/AN4             21, AN9/RPI27/RA11
    // 10K Pot        20, 77, RB5/AN5             19, AN20/RE9
    _TRISA11 = 1;
    _TRISE9  = 1;
    _ANSA11  = 1;
    _ANSE9   = 1;


    // Report 70 MIPS on dsPIC33E
    return FCY;
}


#endif //#if defined(__dsPIC33EP512GM710__) || defined(__dsPIC33EP256GM710__) || defined(__dsPIC33EP128GM710__) || defined(__dsPIC33EP512GM310__) || defined(__dsPIC33EP256GM310__) || defined(__dsPIC33EP128GM310__)
