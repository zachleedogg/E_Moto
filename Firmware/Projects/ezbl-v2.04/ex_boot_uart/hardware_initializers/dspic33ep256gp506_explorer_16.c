/*
 * File:   dspic33ep256gp506_explorer_16.c
 *
 * Initializes the device configuration fuses, clock frequency, UART2 pins,
 * LED I/O, Button I/O, and 25LC256 SPI2 pins for the dsPIC33EP256GP506 PIM on
 * the Explorer 16 or Explorer 16/32 development board. For I2C projects, I2C ?
 * is configured for use with SCL?/SDA?.
 *
 * dsPIC33EP256GP506 product page:
 *   http://www.microchip.com/dspic33ep256gp506
 *
 * dsPIC33EP256GP506 PIM Info Sheet and schematic:
 *   ?
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

#if defined(__dsPIC33EP512GP506__) || defined(__dsPIC33EP256GP506__) || defined(__dsPIC33EP128GP506__) || defined(__dsPIC33EP64GP506__) || defined(__dsPIC33EP32GP506__) || \
    defined(__dsPIC33EP512MC506__) || defined(__dsPIC33EP256MC506__) || defined(__dsPIC33EP128MC506__) || defined(__dsPIC33EP64MC506__) || defined(__dsPIC33EP32MC506__)

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
    EZBL_SET_CONF(_FPOR, ALTI2C1_OFF & ALTI2C2_OFF & WDTWIN_WIN75)                            // Use primary I2C1 and I2C2 pin mappings, set watchdog window to 75% (not important in non-window mode, but included for completness)
    EZBL_SET_CONF(_FWDT, WDTPOST_PS1024 & WDTPRE_PR32 & PLLKEN_ON & WINDIS_OFF & FWDTEN_OFF)  // ~1024ms timeout when watchdog is turned on in software (not forced on in hardware), wait for PLL lock on clock switch, use ordinary (non-windowed) Watchdog Timer mode
    EZBL_SET_CONF(_FOSCSEL, FNOSC_FRC & IESO_OFF)                                             // Start with FRC, then switch to FRC+PLL at run time
    EZBL_SET_CONF(_FOSC, POSCMD_XT & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSECME)                // XT primary oscillator, allow multiple PPS remappings, enable clock switching and fail safe clock monitor
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

    while(OSCCONbits.OSWEN);    // Wait for clock switch to complete

    // Configure PLL for Fosc = 140MHz/Fcy = 70MIPS using 7.37 MHz internal FRC oscillator
    CLKDIV = 0xB000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 2:1
    PLLFBD = (FCY * 2u * 2u * 2u + 7370000u / 2u) / 7370000u - 2u; // 74 @ 70 MIPS (7.37 MHz input clock from FRC)
    __builtin_write_OSCCONH(0x01);      // Initiate Clock Switch to use the FRC Oscillator + PLL (NOSC = 0b001)
    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);

//    // Configure PLL for Fosc = 140MHz/Fcy = 70MIPS using 8.000 MHz Primary Oscillator (make sure to set Configuration Fuse to enable the OSC1/OSC2 oscillator)
//    CLKDIV = 0xB000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 2:1
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
    // Function     Explorer 16 PIM Header      dsPIC33EP256GP506 Device Pins
    // Button       PIM#, PICtail#, name        PIC#, name
    // S4 (LSb)     80, 106, RD13               53, RD5
    // S5           92,  74, RA7                52, RP56/RC8             <- Pin function is muxed with LED D10; S5 button shouldn't be used because LED clamps weak 10k pull up voltage too low
    // S6           84, 100, RD7                51, RP55/RC7
    // S3 (MSb)     83,  99, RD6                54, RD6
    EZBL_DefineButtonMap(RD6, RC7, RD5);
    _TRISD5 = 1;
    //_TRISC8 = 1;    // LED D10 (bit 7)
    _TRISC7 = 1;
    _TRISD6 = 1;


    // Set LED pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP256GP506 Device Pins
    // LED          PIM#, PICtail#, name        PIC#, name
    // D3 (LSb)       17, 69, RA0/TMS             21, AN6/OA3OUT/C4IN1+/OCFB/RC0
    // D4             38, 70, RA1/TCK             22, AN7/C3IN1-/C4IN1-/RC1
    // D5             58, 38, RA2/SCL2            32, SCL2/RP36/RB4
    // D6             59, 40, RA3/SDA2            31, SDA2/RPI24/RA8
    // D7             60, 71, RA4/TDI             27, AN12/C2IN2-/U2RTS/BCLK2/RE12
    // D8             61, 72, RA5/TDO             28, AN13/C3IN2-/U2CTS/RE13
    // D9             91, 73, RA6                 29, AN14/RPI94/RE14
    // D10 (MSb)      92, 74, RA7                 52, RP56/RC8             <- Pin function is muxed with button S5; we will use it as an LED output only
    EZBL_DefineLEDMap(RC8, RE14, RE13, RE12, RA8, RB4, RC1, RC0);
    LEDSet(0x00);   // Clear all LATx bits for the map
    _TRISC0  = 0;
    _TRISC1  = 0;
    _TRISB4  = 0;
    _TRISA8  = 0;
    _TRISE12 = 0;
    _TRISE13 = 0;
    _TRISE14 = 0;
    _TRISC8  = 0;
    _ANSC0   = 0;
    _ANSC1   = 0;
    _ANSE12  = 0;
    _ANSE13  = 0;
    _ANSE14  = 0;


    // Set 16x2 character LCD pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP256GP506 Device Pins
    // 16x2 LCD     PIM#, PICtail#, name        PIC#, name
    // Data 0         93, 109, RE0/PMPD0          48, TCK/CVREF1O/ASCL1/RP40/T4CK/RB8
    // Data 1         94, 110, RE1/PMPD1          49, TMS/ASDA1/RP41/RB9
    // Data 2         98, 111, RE2/PMPD2          60, RP42/RB10
    // Data 3         99, 112, RE3/PMPD3          61, RP43/RB11
    // Data 4        100, 113, RE4/PMPD4          62, RPI44/RB12
    // Data 5          3, 114, RE5/PMPD5          63, RPI45/CTPLS/RB13
    // Data 6          4, 115, RE6/PMPD6           2, RPI46/T3CK/RB14
    // Data 7          5, 116, RE7/PMPD7           3, RPI47/T5CK/RB15
    // E (Enable)     81,  97, RD4/PMPWR          50, RP54/RC6
    // R/!W           82,  98, RD5/PMPRD          47, RC13
    // RS (Reg Sel)   44,  84, RB15/PMPA0         64, TDO/RA10
    ((volatile unsigned char *)&LATB)[1]  = 0x00; // Write bits LATB<15:8> simultaneously, don't change LATB<7:0>
    ((volatile unsigned char *)&TRISB)[1] = 0x00;
    _LATC6   = 0;
    _LATC13  = 0;
    _LATA10  = 0;
    _TRISC6  = 0;
    _TRISC13 = 0;
    _TRISA10 = 0;
    _ANSB8   = 0;


    // Configure Pins for SPI1 on Explorer 16 (nothing attached on the Explorer
    // 16 directly, but useful for various PICtails).
    // - Pin names are with respect to the PIC, which is the SPI Master.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   SPI hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP256GP506 Device Pins
    // SPI1         PIM#, PICtail#, name        PIC#, name
    // !CS   (out)    23,   1, RB2/SS1/AN2        59, RP97/RF1
    // SCK1  (out)    55,   3, RF6/SCK1           35, SCK1/RPI51/RC3
    // SDI1  (in)     54,   5, RF7/SDI1           34, SDI1/RPI25/RA9
    // SDO1  (out)    53,   7, RF8/SDO1           33, CVREF2O/SDO1/RP20/T1CK/RA4
    _LATF1  = 1;    // 1 is inactive
    _TRISF1 = 0;    // !CS on RF1
    _CNPDA9 = 1;    // Turn on SDI1 pull down so it doesn't float when slave chip is inactive
    _TRISC3 = 1;
    _TRISA9 = 1;
    _TRISA4 = 1;
    _ANSA4  = 0;


    // Configure pins for 25LC256 (32Kbyte SPI EEPROM)
    // - Pin names are with respect to the PIC, which is the SPI Master.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   SPI hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP256GP506 Device Pins
    // SPI2         PIM#, PICtail#, name        PIC#, name
    // !CS   (out)    79, 105, RD12                8, RPI121/RG9
    // SCK2  (out)    10,  35, RG6/PMPA5/SCK2      4, RP118/RG6
    // SDI2  (in)     11,  37, RG7/PMPA4/SDI2      5, RPI119/RG7
    // SDO2  (out)    12,  39, RG8/PMPA3/SDO2      6, RP120/RG8
    _LATG9   = 1;               // 1 is inactive
    _TRISG9  = 0;               // !CS on RG9
    _CNPDG7  = 1;               // Turn on pull down on SDI2 so it doesn't float when SPI module tri-states it
    _TRISG6  = 1;
    _TRISG7  = 1;
    _TRISG8  = 1;
    _SDI2R   = 119;             // SDI2 (MISO2) on RPI119
    _RP118R  = _RPOUT_SCK2OUT;  // SCK2 on RP118
    _RP120R  = _RPOUT_SDO2;     // SDO2 (MOSI2) on RP120


    // Configure UART2 pins as UART.
    // - Pin names are with respect to the PIC.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   UART2 hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP256GP506 Device Pins
    // UART2        PIM#, PICtail#, name        PIC#, name
    // U2RX  (in)     49, 34, RF4/PMPA9/U2RX      44, PGEC2/ASCL2/RP38/RB6
    // U2TX  (out)    50, 36, RF5/PMPA8/U2TX      43, PGED2/ASDA2/RP37/RB5
    _CNPUB6    = 1;             // Turn on weak pull up on U2RX so no spurious data arrives if nobody connected
    _U2RXR     = 38;            // U2RX on RP38
    _RP37R     = _RPOUT_U2TX;   // U2TX on RP37
#if defined(XPRJ_uart)  // Defined by MPLAB X on command line when compiling "uart" Build Configuration
    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 1);
#endif


#if defined(XPRJ_i2c)   // Defined by MPLAB X on command line when compiling "i2c" Build Configuration
    // Configure I2C1 pins for MCP2221A
    // Function     Explorer 16 PIM Header      dsPIC33EP256GP506 Device Pins
    // I2C 1        PIM#, PICtail#, name        PIC#, name
    // SDA1 (in/out)  56, 8, RG3/SDA1            ?,? 
    // SCL1 (in/out)  57, 6, RG2/SCL1            ?,?
    //_CNPUC4 = 1;                          // Enable Weak pull up on SDA1 line so pins float to idle state if not connected or externally pulled up
    //_CNPUC5 = 1;                          // Enable Weak pull up on SCL1 line so pins float to idle state if not connected or externally pulled up
    //_ANSC4  = 0;
    //_ANSC5  = 0;
    EZBL_COMBootIF = I2C_Reset(1, FCY, 0, EZBL_i2cSlaveAddr, 0);
#endif


    // Configure Analog Inputs for U4 TC1047A Temperature Sensor and R6 10K Potentiometer
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP256GP506 Device Pins
    // Analog Input PIM#, PICtail#, name        PIC#, name
    // TC1047A Temp   21, 14, RB4/AN4             14, AN1/C2IN1+/RA1
    // 10K Pot        20, 77, RB5/AN5             13, AN0/OA2OUT/RA0
    _TRISA1  = 1;
    _TRISA0  = 1;
    _ANSA1   = 1;
    _ANSA0   = 1;


    // Configure CAN1, CAN2, LIN1 communications pins for 
    // AC164130-2: CAN/LIN/J2602 PICtail/PICtail Plus Daughter board
    //
    // Function     Explorer 16 PICtail Plus      dsPIC33EP256GP506 Device Pins
    //              PIM#, PICtail#, name          PIC#, name
    // CAN1TX         88,       30, RF1             55, RP57/RC9
    // CAN1RX         87,       28, RF0             58, RPI96/RF0
    // CAN2TX         89,       62, RG1             NC, -
    // CAN2RX         90,       60, RG0             NC, -
    // LIN1TX         51,        4, RF3/U1TX_E      NC, -
    // LIN1RX         52,        2, RF2/U1RX_E      NC, -
    // LIN1CS         18,       18, RE8/INT1        46, RP39/INT0/RB7
    // LIN1TXE        48,       20, RD15/U1RTS_E    45, RPI58/RC10
    // LIN2TX         50,       36, RF5/PMA8/U2TX   43, PGED2/ASDA2/RP37/RB5    // Note: Muxed with UART2 RS232 level transceiver
    // LIN2RX         49,       34, RF4/PMA9/U2RX   44, PGEC2/ASCL2/RP38/RB6    // Note: Conflicts with UART2 RS232 level transceiver
    // LIN2CS         66,       50, RA14/INT3       NC, -
    // LIN2TXE        39        52, RF13/U2RTS      23, AN8/C3IN1+/U1RTS/BCLK/RC2 // Note: Conflicts with UART2 RS232 level transceiver
    _TRISC9  = 1;
    _TRISF0  = 1;
    _RP57R   = _RPOUT_C1TX;
    _C1RXR   = 96;


    // Report 70 MIPS on dsPIC33E
    return FCY;
}


#endif //#if defined(__dsPIC33EP512GP506__) || defined(__dsPIC33EP256GP506__) || defined(__dsPIC33EP128GP506__) || defined(__dsPIC33EP64GP506__) || defined(__dsPIC33EP32GP506__) || defined(__dsPIC33EP512MC506__) || defined(__dsPIC33EP256MC506__) || defined(__dsPIC33EP128MC506__) || defined(__dsPIC33EP64MC506__) || defined(__dsPIC33EP32MC506__)

