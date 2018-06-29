/*
 * File:   pic24fj128ga310_explorer_16.c
 *
 * Initializes the device configuration fuses, clock frequency, UART2 pins,
 * LED I/O, Button I/O, and 25LC256 SPI2 pins for the PIC24FJ128GA310 PIM on
 * the Explorer 16 or Explorer 16/32 development board. For I2C projects, I2C 1
 * is configured for use with SCL1/SDA1.
 *
 * PIC24FJ128GA310 product page:
 *   http://www.microchip.com/pic24fj128ga310
 *
 * PIC24FJ128GA310 PIM Info Sheet and schematic:
 *   http://ww1.microchip.com/downloads/en/DeviceDoc/52052a.pdf
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

// NOTE: This initialization code is specific to the PIC24FJ128GA310 PIM, so
// GA308/GA306 devices are listed here only for convenience. Such devices may
// not compile without I/O or Config word changes.
#if defined(__PIC24FJ128GA310__) || defined(__PIC24FJ64GA310__) || \
    defined(__PIC24FJ128GA308__) || defined(__PIC24FJ64GA308__) || \
    defined(__PIC24FJ128GA306__) || defined(__PIC24FJ64GA306__)

#define FCY         16000000ul      // Changing this automatically changes the PLL settings to run at this target frequency

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
    EZBL_SET_CONF(_CONFIG4, DSWDTPS_DSWDTPS12 & DSWDTOSC_LPRC & DSBOREN_ON & DSWDTEN_ON & DSSWEN_ON)
    EZBL_SET_CONF(_CONFIG3, WPFP_WPFP6 & VBTBOR_OFF & SOSCSEL_ON & WDTWIN_PS75_0 & BOREN_ON & WPDIS_WPDIS & WPCFG_WPCFGEN & WPEND_WPSTARTMEM)   // NOTE: Keep VBTBOR_OFF according to the DS80000532H errata item 4
    EZBL_SET_CONF(_CONFIG2, POSCMD_XT & BOREN1_EN & IOL1WAY_OFF & OSCIOFCN_ON & FCKSM_CSECME & FNOSC_FRC & ALTVREF_DLT_AV_DLT_CV & IESO_OFF)
    EZBL_SET_CONF(_CONFIG1, WDTPS_PS1024 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_WDT_SW & ICS_PGx2 & LPCFG_ON & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
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
    // Function     Explorer 16 PIM Header      PIC24FJ128GA310 Device Pins
    // Button       PIM#, PICtail#, name        PIC#, name
    // S4 (LSb)       80, 106, RD13               80, SEG45/PMD13/CN19/RD13
    // S5             92,  74, RA7                92, AN22/SEG59/PMA17/CN40/RA7   <- Pin function is muxed with LED D10; S5 button won't be used because LED clamps weak 10k pull up voltage so may not work
    // S6             84, 100, RD7                84, C3INA/SEG26/PMD15/CN16/RD7
    // S3 (MSb)       83,  99, RD6                83, C3INB/SEG25/PMD14/CN15/RD6
    EZBL_DefineButtonMap(RD6, RD7, RD13);
    _TRISD13 = 1;
    //_TRISA7  = 1;
    _TRISD7  = 1;
    _TRISD6  = 1;
    _ANSA7   = 0;
    _ANSD7   = 0;
    _ANSD6   = 0;


    // Set LED pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      PIC24FJ128GA310 Device Pins
    // LED          PIM#, PICtail#, name        PIC#, name
    // D3 (LSb)       17, 69, RA0/TMS             17, TMS/CTED0/SEG49/CN33/RA0
    // D4             38, 70, RA1/TCK             38, TCK/CN34/RA1
    // D5             58, 38, RA2/SCL2            58, SCL2/SEG56/CN35/RA2
    // D6             59, 40, RA3/SDA2            59, SDA2/SEG57/PMA20/CN36/RA3
    // D7             60, 71, RA4/TDI             60, TDI/PMA21/CN37/RA4
    // D8             61, 72, RA5/TDO             61, TDO/CN38/RA5
    // D9             91, 73, RA6                 91, AN23/SEG58/CN39/RA6
    // D10 (MSb)      92, 74, RA7                 92, AN22/SEG59/PMA17/CN40/RA7   <- Pin function is muxed with button S5; we will use it as an LED output only
    EZBL_DefineLEDMap(RA7, RA6, RA5, RA4, RA3, RA2, RA1, RA0);
    *((volatile unsigned char *)&LATA)  = 0x00; // Write bits LATA<7:0> simultaneously, don't change LATA<15:8>
    *((volatile unsigned char *)&TRISA) = 0x00;
    *((volatile unsigned char *)&ANSA)  = 0x00;


    // Set 16x2 character LCD pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      PIC24FJ128GA310 Device Pins
    // 16x2 LCD     PIM#, PICtail#, name        PIC#, name
    // Data 0         93, 109, RE0/PMPD0          93, COM3/PMD0/CN58/RE0
    // Data 1         94, 110, RE1/PMPD1          94, COM2/PMD1/CN59/RE1
    // Data 2         98, 111, RE2/PMPD2          98, COM1/PMD2/CN60/RE2
    // Data 3         99, 112, RE3/PMPD3          99, COM0/PMD3/CTED9/CN61/RE3
    // Data 4        100, 113, RE4/PMPD4         100, SEG63/PMD4/HLVDIN/CTED8/CN62/RE4
    // Data 5          3, 114, RE5/PMPD5           3, CTED4/PMD5/LCDBIAS2/CN63/RE5
    // Data 6          4, 115, RE6/PMPD6           4, PMD6/LCDBIAS1/CN64/RE6
    // Data 7          5, 116, RE7/PMPD7           5, PMD7/LCDBIAS0/CN65/RE7
    // E (Enable)     81,  97, RD4/PMPWR          81, RP25/SEG23/PMWR/CN13/RD4
    // R/!W           82,  98, RD5/PMPRD          82, RP20/SEG24/PMRD/CN14/RD5
    // RS (Reg Sel)   44,  84, RB15/PMPA0         44, AN15/RP29/SEG9/REFO/CTED6/PMA0/CN12/RB15
    *((volatile unsigned char *)&LATE)  = 0x00; // Write bits LATE<7:0> simultaneously, don't change LATE<15:8>
    *((volatile unsigned char *)&TRISE) = 0x00;
    *((volatile unsigned char *)&ANSE)  = 0x00;
    _LATD4   = 0;
    _LATD5   = 0;
    _LATB15  = 0;
    _TRISD4  = 0;
    _TRISD5  = 0;
    _TRISB15 = 0;
    _ANSB15  = 0;


    // Configure pins for 25LC256 (32Kbyte SPI EEPROM)
    // - Pin names are with respect to the PIC, which is the SPI Master.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   SPI hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      PIC24FJ128GA310 Device Pins
    // SPI2         PIM#, PICtail#, name        PIC#, name
    // !CS   (out)    79, 105, RD12               79, RPI42/SEG44/PMD12/CN57/RD12
    // SCK2  (out)    10,  35, RG6/PMPA5/SCK2     10, AN17/C1IND/RP21/SEG0/PMA5/CN8/RG6
    // SDI2  (in)     11,  37, RG7/PMPA4/SDI2     11, VLCAP1/AN18/C1INC/RP26/PMA4/CN9/RG7
    // SDO2  (out)    12,  39, RG8/PMPA3/SDO2     12, VLCAP2/AN19/C2IND/RP19/PMA3/CN10/RG8
    _LATD12  = 1;       // 1 is inactive
    _TRISD12 = 0;       // !CS on RD12
    _CN9PUE  = 1;       // Turn on pull up on SDI2 so it doesn't float when SPI module tri-states it
    _TRISG6  = 1;
    _ANSG6   = 0;
    _RP21R   = _RPOUT_SCK2OUT;  // SCK2 = RP21
    _TRISG7  = 1;
    _ANSG7   = 0;
    _SDI2R   = 26;              // SDI2 = RP26
    _TRISG8  = 1;
    _ANSG8   = 0;
    _RP19R   = _RPOUT_SDO2;     // SDO2 = RP19


#if defined(XPRJ_uart)  // Defined by MPLAB X on command line when compiling "uart" Build Configuration
    // Configure UART2 pins as UART.
    // - Pin names are with respect to the PIC.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   UART2 hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      PIC24FJ128GA310 Device Pins
    // UART2        PIM#, PICtail#, name        PIC#, name
    // U2RX  (in)     49, 34, RF4/PMPA9/U2RX      49, RP10/SEG10/PMA9/CN17/RF4
    // U2TX  (out)    50, 36, RF5/PMPA8/U2TX      50, RP17/SEG11/PMA8/CN18/RF5
    _CN17PUE = 1;               // Turn on weak pull up on U2RX so no spurious data arrives if nobody connected
    _U2RXR   = 10;              // U2RX on RP10
    _RP17R   = _RPOUT_U2TX;     // U2TX on RP17
    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 1);
#endif


#if defined(XPRJ_i2c)   // Defined by MPLAB X on command line when compiling "i2c" Build Configuration
    // Configure I2C1 pins for MCP2221A
    // Function     Explorer 16 PIM Header      PIC24FJ128GA310 Device Pins
    // I2C 1        PIM#, PICtail#, name        PIC#, name
    // SDA1 (in/out)  56, 8, RG3/SDA1             56, SDA1/SEG47/CN73/RG3
    // SCL1 (in/out)  57, 6, RG2/SCL1             57, SCL1/SEG28/CN72/RG2
    _CN73PUE = 1;                               // Enable Weak pull up on SDA1 line so pins float to idle state if not connected or externally pulled up
    _CN72PUE = 1;                               // Enable Weak pull up on SCL1 line so pins float to idle state if not connected or externally pulled up
    EZBL_COMBootIF = I2C_Reset(1, FCY, 0, EZBL_i2cSlaveAddr, 0);
#endif


    // Configure Analog Inputs for U4 TC1047A Temperature Sensor and R6 10K Potentiometer
    //
    // Function     Explorer 16 PIM Header      PIC24FJ128GA310 Device Pins
    // Analog Input PIM#, PICtail#, name        PIC#, name
    // TC1047A Temp 21, 14, RB4/AN4             21, PGED3/AN4/C1INB/RP28/SEG3/CN6/RB4
    // 10K Pot      20, 77, RB5/AN5             20, PGEC3/AN5/C1INA/RP18/SEG2/CN7/RB5
    _ANSB4  = 1;
    _ANSB5  = 1;
    _TRISB4 = 1;
    _TRISB5 = 1;


    // Report 16 MIPS on PIC24F
    return FCY;
}


#endif //#if defined(__PIC24FJ128GA310__) || defined(__PIC24FJ64GA310__) || defined(__PIC24FJ128GA308__) || defined(__PIC24FJ64GA308__) || defined(__PIC24FJ128GA306__) || defined(__PIC24FJ64GA306__)
