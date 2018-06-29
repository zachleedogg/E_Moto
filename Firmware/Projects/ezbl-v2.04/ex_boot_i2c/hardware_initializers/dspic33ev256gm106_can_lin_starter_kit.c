/*
 * File:   dspic33ev256gm106_can_lin_starter_kit.c
 *
 * Initializes the device configuration fuses, clock frequency, UART2 pins,
 * for the dsPIC33EV256GM106 soldered on the Microchip dsPIV33EV 5V CAN-LIN 
 * Starter Kit.
 *
 * As this starter kit does not have a native UART interface, an MCP2200 
 * (ADM00393) or MCP2221 (ADM00559) Breakout Module should be connected to J3 on
 * the starter kit. Ensure the breakout module power jumper is set for 5V and
 * then connect Pin 1 (TX) on the breakout module to the RB11 hole on J3. Pin 6
 * (RX) on the breakout module must mate with the RB4 hole on J3. RB7 and RB8
 * will be configured as dummy hi-Z pins, while RB9 and RB10 will be emulated
 * GND and VDD references by driving the I/O lines in software as outputs to 0
 * and 1.
 * 
 * dsPIC33EV256GM106 product page:
 * http://www.microchip.com/dspic33ev256gm106
 *
 * dsPIC33EV 5V CAN-LIN Starter Kit (DM330018) product page:
 * http://www.microchip.com/dm330018
 *
 * dsPIC33EV 5V CAN-LIN Starter Kit User's Guide (contains schematic):
 * http://ww1.microchip.com/downloads/en/DeviceDoc/50002311A.pdf
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

#if defined(__dsPIC33EV256GM106__) || defined(__dsPIC33EV256GM104__)  || defined(__dsPIC33EV256GM102__) ||     \
    defined(__dsPIC33EV128GM106__) || defined(__dsPIC33EV128GM104__)  || defined(__dsPIC33EV128GM102__) ||     \
    defined(__dsPIC33EV64GM106__)  || defined(__dsPIC33EV64GM104__)   || defined(__dsPIC33EV64GM102__)  ||     \
    defined(__dsPIC33EV32GM106__)  || defined(__dsPIC33EV32GM104__)   || defined(__dsPIC33EV32GM102__)  ||     \
    defined(__dsPIC33EV256GM006__) || defined(__dsPIC33EV256GM004__)  || defined(__dsPIC33EV256GM002__) ||     \
    defined(__dsPIC33EV128GM006__) || defined(__dsPIC33EV128GM004__)  || defined(__dsPIC33EV128GM002__) ||     \
    defined(__dsPIC33EV64GM006__)  || defined(__dsPIC33EV64GM004__)   || defined(__dsPIC33EV64GM002__)  ||     \
    defined(__dsPIC33EV32GM006__)  || defined(__dsPIC33EV32GM004__)   || defined(__dsPIC33EV32GM002__)

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
    EZBL_SET_CONF(_FSEC, BWRP_OFF & BSS_DISABLED & BSS2_OFF & GWRP_OFF & GSS_DISABLED & CWRP_OFF & CSS_DISABLED & AIVTDIS_DISABLE)
    EZBL_SET_CONF(_FOSCSEL, FNOSC_FRC & IESO_OFF)
    EZBL_SET_CONF(_FOSC, POSCMD_XT & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSECME & PLLKEN_ON)          // 8.000MHz XT crystal oscillator, allow multiple PPS remappings, enable clock switching and fail safe clock monitor, wait for PLL lock
    EZBL_SET_CONF(_FWDT, WDTPOST_PS1024 & WDTPRE_PR32 & FWDTEN_ON_SWDTEN & WINDIS_OFF & WDTWIN_WIN75)// ~1024ms timeout when watchdog is turned on in software (not forced on in hardware), use ordinary (non-windowed) Watchdog Timer mode
    EZBL_SET_CONF(_FPOR, BOREN0_ON)
    //EZBL_SET_CONF(_FICD, ICS_PGD3)  // For external 6-pin PICkit header
    EZBL_SET_CONF(_FICD, ICS_PGD2)   // For onboard PKOB Programmer/Debugger
    EZBL_SET_CONF(_FDMT, DMTEN_DISABLE)
    EZBL_SET_CONF(_FDEVOPT, PWMLOCK_OFF & ALTI2C1_OFF)
#else   // Compiling for an Application Project (EZBL_BOOT_PROJECT is not defined)
    EZBL_SET_CONF(_FALTREG, CTXT2_IPL7 & CTXT2_IPL6)                                              // Do assign a context register set to IPL4 (or whatever you have your NOW Timer set to) 
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
//    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);

    // Configure PLL for Fosc = 140MHz/Fcy = 70MIPS using 8.000 MHz crystal oscillator
    CLKDIV = 0xB000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 2:1
    PLLFBD = (FCY * 2u * 2u * 2u + 8000000u / 2u) / 8000000u - 2u; // 68 @ 70 MIPS (8.000 MHz input clock from crystal)
    __builtin_write_OSCCONH(0x03);      // Initiate Clock Switch to use the XT Oscillator + PLL (NOSC = 0b011)
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
    // Function     dsPIC33EV256GM106 Device Pins
    // Button       PIC#, name
    // S1             51, AN52/RP55/RC7
    // S2             52, AN51/RP56/RC8
    // S3             53, AN54/RP57/RC9
    EZBL_DefineButtonMap(RC9, RC8, RC7);
    _TRISC7 = 1;
    _TRISC8 = 1;
    _TRISC9 = 1;
    _ANSC7  = 0;
    _ANSC8  = 0;
    _ANSC9  = 0;


    // Set LED pins as GPIO outputs
    //
    // Function     dsPIC33EV256GM106 Device Pins
    // LED          PIC#, name
    // LED3 (LSb)     50, AN53/RP54/RC6
    // LED2           37, AN31/RPI53/RC5
    // LED1 (MSb)     36, AN30/CVREF+/RPI52/RC4
    EZBL_DefineLEDMap(RC4, RC5, RC6);
    _LATC6  = 0;
    _LATC5  = 0;
    _LATC4  = 0;
    _TRISC6 = 0;
    _TRISC5 = 0;
    _TRISC4 = 0;
    _ANSC6  = 0;
    _ANSC5  = 0;
    _ANSC4  = 0;


    // Configure GPIO's on J3 header as a "PICkit Serial" UART2 header for 
    // interfacing to an MCP2200/MCP2221 Breakout board
    // - Pin names are with respect to the PIC.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   UART2 hardware overrides it.
    //
    // Function         dsPIC33EV256GM106 Device Pins
    // UART2            PIC#, name
    // U2TX  (out)        32, FLT32/RP36/RB4    ("PICkit Serial" pinout pin 6)
    // Dummy (hi-Z)       46, OA5OUT/AN25/C5IN4-/RP39/INT0/RB7
    // Dummy (hi-Z)       48, AN26/CVREF1O/ASCL1/RP40/T4CK/RB8
    // GND (GPIO out)     49, OA5IN-/AN27//C5IN1-/ASDA1/RP41/RB9
    // VDD (GPIO out)     60, RP42/PWM1H3/RB10  ("PICkit Serial" pinout pin 2 -- NOTE: MCP2200/MCP2221 Breakout board must be jumpered for 5V!)
    // U2RX  (in)         61, RP43/PWM1L3/RB11  ("PICkit Serial" pinout pin 1)
    IOCON3bits.PENH = 0;// Disable PWM1H3 which is on by default and higher priority than PPS
    IOCON3bits.PENL = 0;// Disable PWM1L3 which is on by default and higher priority than PPS
    _LATB10  = 1;       // Need to output +5V for VDD signal on MCP2200/MCP2221 Breakout board
    _LATB9   = 0;       // Need to output 0V for GND signal on MCP2200/MCP2221 Breakout board
    _ANSB9   = 0;
    _ANSB8   = 1;
    _ANSB7   = 1;
    _TRISB4  = 1;
    _TRISB7  = 1;
    _TRISB8  = 1;
    _TRISB9  = 0;
    _TRISB10 = 0;
    _TRISB11 = 1;
    _CNPUB11 = 1;               // Enable Weak pull up on U2RX line so we don't receive spurious data while nobody is connected
    _U2RXR   = 43;              // U2RX on RP43
    _RP36R   = _RPOUT_U2TX;     // U2TX on RP36 (_RPOUT_U2TX = 0x03 in case if your compiler is too old to have the _RPOUT_xxx macros)
#if defined(XPRJ_uart)  // Defined by MPLAB X on command line when compiling "uart" Build Configuration
    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 1);
#endif


    // Configure Analog Inputs for U7 MCP9701A Temperature Sensor and R14 25K Potentiometer
    //
    // Function         dsPIC33EV256GM106 Device Pins
    // Analog Input     PIC#, name
    // MCP9701A Temp       5, AN18/RPI119/RG7
    // 25K Pot             4, AN19/RP118/RG6
    _TRISG7 = 1;
    _TRISG6 = 1;
    _ANSG7  = 1;
    _ANSG6  = 1;
    
    
    // Configure CAN1 TX/RX and transceiver STANDBY pins
    _TRISG9 = 0;                    // Drive the CAN transceiver STANDBY pin low
    _LATG9  = 0;
    RPINR26 = 96;                   // Connect CAN RX to RPI96
    RPOR9   = _RPOUT_C1TX;          // Connect CAN TX to RP97. NOTE: _RPOUT_C1TX is available only with XC16 v1.25b (w/Part Support Update) and later (includes v1.26+). If you must use an older compiler, try 0x000E


    // Report 70 MIPS on dsPIC33E
    return FCY;
}


#endif //#if defined(__dsPIC33EV256GM106__) || defined(__dsPIC33EV256GM104__)  || defined(__dsPIC33EV256GM102__) ... 
