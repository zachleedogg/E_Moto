/*
 * File:   pic24fj128gc010_analog_starter_kit.c
 *
 * Initializes the device configuration words, clock frequency, UART2 pins,
 * LED I/O and Button I/O for the PIC24FJ128GC010 on the MPLAB(R) Starter Kit
 * for Intelligent.Integrated Analog development board.
 *
 * For UART projects, UART 2 is configured for use with RP20/RP25:
 *   U2RX = AN48/OPA1N1/SEG24/RP20/PMRD/CN14/RD5 - "SDI" net
 *   U2TX = AN47/OPA1P4/SEG23/RP25/PMWR/CN13/RD4 - "SDO" net
 *
 * For I2C projects, I2C 2 is configured for use with the alternate pins:
 *   ASDA2 = AN11/OA2N3/SEG10/RP10/ASDA2/T3CK/PMA9/CN17/RF4  - "OPA_2-" net, PIC pin 49
 *   ASCL2 = CVREF/AN10/OA2P2/SEG11/RP17/ASCL2/PMA8/CN18/RF5 - "OPA_2+" net, PIC pin 50
 *
 * PIC24FJ128GC010 product page:
 *   http://www.microchip.com/pic24fj128gc010
 *
 * PIC24FJ128GC010 MPLAB Starter Kit for Intelligent.Integrated.Analog User's Guide and Schematic:
 *   http://ww1.microchip.com/downloads/en/DeviceDoc/50002172a.pdf
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


// NOTE: This initialization code is specific to the PIC24FJ128GC010 based
// MPLAB(R) Starter Kit for Integrated.Intelligent.Analog development board.
// Other devices are listed here only for convenience.
// Such devices may not compile without I/O or Config word changes.
#if defined(__PIC24FJ128GC010__) || defined(__PIC24FJ128GC006__) || defined(__PIC24FJ64GC010__) || defined(__PIC24FJ64GC006__)


#define FCY         16000000ul

#include <xc.h>
#include "../ezbl_integration/ezbl.h"



// Device Configuration Words
// Config words can be defined in a Bootloader project, Application project,
// or mixed between each other so long as any given flash/config write-word-
// size location has exactly one definition. On devices with flash Config
// words and a flash double word minimum programming size (0x4 program
// addresses), this means two adjacent Config words may not be mixed between
// projects and instead both Config words must be defined in the same project.
//
// These defaults place all (or almost all) in the Bootloader project as this
// is the safest from a bootloader-bricking standpoint.
// PIC24FJ128GC010 Configuration Bit Settings - defined using EZBL_SET_CONF() macro instead of #pragma config statements so you can CTRL+Click on defines and get the device header containing help text for each option
#if defined(EZBL_BOOT_PROJECT)  // Compiling for a Bootloader Project
    EZBL_SET_CONF(_CONFIG4, DSWDTPS_DSWDTPS10 & DSWDTOSC_LPRC & DSBOREN_ON & DSWDTEN_ON & DSSWEN_ON & RTCBAT_OFF & PLLDIV_DIV2 & I2C2SEL_SEC & IOL1WAY_OFF);
    EZBL_SET_CONF(_CONFIG3, WPFP_WPFP5 & SOSCSEL_ON & WDTWIN_PS75_0 & BOREN_ON & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPSTARTMEM);
    EZBL_SET_CONF(_CONFIG2, POSCMD_NONE & WDTCLK_LPRC & OSCIOFCN_ON & FCKSM_CSECME & FNOSC_FRCPLL & ALTADREF_AVREF_RA & ALTCVREF_CVREF_RA & WDTCMX_WDTCLK & IESO_OFF);
    EZBL_SET_CONF(_CONFIG1, WDTPS_PS512 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_WDT_SW & ICS_PGx3 & LPCFG_ON & GWRP_OFF & GCP_OFF & JTAGEN_OFF);
#else   // Compiling for an Application Project (EZBL_BOOT_PROJECT is not defined)

#endif  // Goes to: #if defined(EZBL_BOOT_PROJECT)



//const unsigned int EZBL_i2cSlaveAddr = 0x60;          // Define I2C Slave Address that this Bootloader will listen/respond to, applicable only if I2C_Reset() is called
EZBL_FIFO *EZBL_COMBootIF __attribute__((persistent));  // Pointer to RX FIFO to check activity on for bootloading
const long EZBL_COMBaud = -230400;                      // Communications baud rate: <= 0 means auto-baud with default value (for TX) set by the 2's complemented value; otherwise baud in bits/sec (ex: 460800)


// Set processor clock, configure LED/Button I/O pin states, PPS
// assignment, initialize NOW_*() API timer, UART/I2C communications, etc.

unsigned long InitializeBoard(void)
{
    // Configures clock, when possible, for FRC+PLL (may fail due to FCKSM being
    // set to disable run-time clock switching). Also sets FRC divide ratio to
    // 1:1, so you should get at least 4 MIPS (PIC24 with 8.0MHz FRC) or 3.685
    // MIPS (dsPIC with 7.37MHz FRC) when no Config words defined or have been
    // erased.
    EZBL_InitializeBoard();

    // Start a timer for the "NOW" timekeeping and task scheduling APIs
    NOW_Reset(TMR1, FCY);

    // Initialize LED1..LED2 EZBL_LEDMap[] (LED1 == bit 1, LED2 == bit 0)
    EZBL_DefineLEDMap(RE7, RB6);// LED1 = RE7/PMD7/LCDBIAS0/CN65, LED2 = PGEC2/AN6/OPA1P3/RP6/LCDBIAS3/CN24/RB6
    _ANSE7  = 0;
    _ANSB6  = 0;
    _TRISE7 = 0;
    _TRISB6 = 0;

    // Initialize SW1 EZBL_ButtonMap[] (SW1 = bit 0)
    EZBL_DefineButtonMap(RD0);  // SW1 = AN43/OPA2N0/SEG17/RP11/VCMPST3/DMH/INT0/CN49/RD0
    _ANSD0  = 0;
    _TRISD0 = 1;
    _SE17   = 0;                // 0 = SEGx disabled, pin is digital I/O; 1 = SEGx LCD Segment function active


#if defined(XPRJ_uart)  // Defined by MPLAB X on command line when compiling "uart" Build Configuration
    // U2TX = AN47/OPA1P4/SEG23/RP25/PMWR/CN13/RD4
    // U2RX = AN48/OPA1N1/SEG24/RP20/PMRD/CN14/RD5
    _CN14PUE = 1;               // Enable weak pull on U2RX pin so net is biased to the UART Idle state float when nobody is connected
    _ANSD4   = 0;               // 0 = Digital I/O; 1 = Analog I/O
    _ANSD5   = 0;               // 0 = Digital I/O; 1 = Analog I/O
    _SE23    = 0;               // 0 = SEGx disabled, pin is digital I/O; 1 = SEGx LCD Segment function active
    _SE24    = 0;               // 0 = SEGx disabled, pin is digital I/O; 1 = SEGx LCD Segment function active
    _U2RXR   = 20;              // U2RX on RP20
    _RP25R   = _RPOUT_U2TX;     // U2TX on RP25
    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 1);
#endif


//    // Debugging UART1
//    // U1RX = CVREF/AN10/OPA2P2/SEG11/RP17/SCL2/PMA8/CN18/RF5 - "OPA_2-" net
//    // U1TX = AN11/OPA2N3/SEG10/RP10/SDA2/T3CK/PMA9/CN17/RF4  - "OPA_2+" net
//    _CN18PUE = 1;               // Enable weak pull up on U1RX
//    _ANSF5   = 0;               // 0 = Digital I/O; 1 = Analog I/O
//    _ANSF4   = 0;               // 0 = Digital I/O; 1 = Analog I/O
//    _SE11    = 0;               // 0 = SEGx disabled, pin is digital I/O; 1 = SEGx LCD Segment function active
//    _SE10    = 0;               // 0 = SEGx disabled, pin is digital I/O; 1 = SEGx LCD Segment function active
//    _U1RXR   = 17;              // U1RX = RP17
//    _RP10R   = _RPOUT_U1TX;     // U1TX = RP10
//    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
//    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
//        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
//    }
//    UART_Reset(1, FCY, EZBL_COMBaud, 1);
//    //EZBL_printf("\n\n\nUART1 initialized");


#if defined(XPRJ_i2c)   // Defined by MPLAB X on command line when compiling "i2c" Build Configuration
    // Alternate SDA2 = AN11/OA2N3/SEG10/RP10/ASDA2/T3CK/PMA9/CN17/RF4  - "OPA_2-" net, PIC pin 49
    // Alternate SCL2 = CVREF/AN10/OA2P2/SEG11/RP17/ASCL2/PMA8/CN18/RF5 - "OPA_2+" net, PIC pin 50
    _CN17PUE = 1;   // Enable weak pull up on SDA2 line - NOTE: there is no pull up on-board for I2C; one needs to exist on your I2C bus partner board(s) since the internal one is relatively weak.
    _CN18PUE = 1;   // Enable weak pull up on SCL2 line - NOTE: there is no pull up on-board for I2C; one needs to exist on your I2C bus partner board(s) since the internal one is relatively weak.
    _ANSF4   = 0;   // 0 = Digital I/O; 1 = Analog I/O
    _ANSF5   = 0;   // 0 = Digital I/O; 1 = Analog I/O
    _SE10    = 0;   // 0 = SEGx disabled, pin is digital I/O; 1 = SEGx LCD Segment function active
    _SE11    = 0;   // 0 = SEGx disabled, pin is digital I/O; 1 = SEGx LCD Segment function active
    EZBL_COMBootIF = I2C_Reset(2, FCY, 0, EZBL_i2cSlaveAddr, 0);
#endif


    return FCY;
}

#endif  // defined(__PIC24FJ128GC010__) || defined(__PIC24FJ128GC006__) || defined(__PIC24FJ64GC010__) || defined(__PIC24FJ64GC006__)
