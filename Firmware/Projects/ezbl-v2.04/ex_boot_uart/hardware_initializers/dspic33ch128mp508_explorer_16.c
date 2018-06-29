/*
 * File:   dspic33ch128mp508_explorer_16.c
 *
 * Initializes the device configuration fuses, clock frequency, UART2 pins,
 * LED I/O, Button I/O, and 25LC256 SPI2 pins for the dsPIC33CH128MP508 PIM on
 * the Explorer 16/32 development board.
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

#if defined(__dsPIC33CH128MP508__) || defined(__dsPIC33CH128MP506__) || defined(__dsPIC33CH128MP505__) || defined(__dsPIC33CH128MP503__) || defined(__dsPIC33CH128MP502__) ||   \
    defined(__dsPIC33CH128MP508S1__) || defined(__dsPIC33CH128MP506S1__) || defined(__dsPIC33CH128MP505S1__) || defined(__dsPIC33CH128MP503S1__) || defined(__dsPIC33CH128MP502S1__)

#include <xc.h>
#include "../ezbl_integration/ezbl.h"

#define PLL_IN_CLK             8000000ul    // Define PLL input: 8.0 MHz from FRC
#define FVCO                1400000000ul    // Choose raw VCO output: 1.4 GHz for High Resolution PWM and DAC. Can affect precision achieving target FCY.
#define FCY                   90000000ul    // Choose CPU speed: 90 MIPS clock (180 MHz) for CPU execution. Change this and PLL initialization will compute needed scalars to achieive this output clock.


// Macros to auto-compute optimal PLL dividers given the PLL_IN_CLK, chosen CPU
// FCY, and target FVCO settings above. You should change those
// named values and not have to do anything to the NET_POST_DIV and BEST_PLLDIV
// macros.
#define NET_POST_DIV    (4u*(FVCO/(4.0*FCY) >= 49 ? 7u*7u : \
                            (FVCO/(4.0*FCY) >= 42 ? 7u*6u : \
                            (FVCO/(4.0*FCY) >= 36 ? 6u*6u : \
                            (FVCO/(4.0*FCY) >= 35 ? 7u*5u : \
                            (FVCO/(4.0*FCY) >= 30 ? 6u*5u : \
                            (FVCO/(4.0*FCY) >= 28 ? 7u*4u : \
                            (FVCO/(4.0*FCY) >= 10 ? 5u*5u : \
                            (FVCO/(4.0*FCY) >= 21 ? 7u*3u : \
                            (FVCO/(4.0*FCY) >= 20 ? 5u*4u : \
                            (FVCO/(4.0*FCY) >= 18 ? 6u*3u : \
                            (FVCO/(4.0*FCY) >= 16 ? 4u*4u : \
                            (FVCO/(4.0*FCY) >= 14 ? 7u*2u : \
                            (FVCO/(4.0*FCY) >= 12 ? 6u*2u : \
                            (FVCO/(4.0*FCY) >= 10 ? 5u*2u : \
                            (FVCO/(4.0*FCY) >=  9 ? 3u*3u : \
                            (FVCO/(4.0*FCY) >=  8 ? 4u*2u : \
                            (FVCO/(4.0*FCY) >=  7 ? 7u*1u : \
                            (FVCO/(4.0*FCY) >=  6 ? 6u*1u : \
                            (FVCO/(4.0*FCY) >=  5 ? 5u*1u : \
                            (FVCO/(4.0*FCY) >=  4 ? 4u*1u : \
                            (FVCO/(4.0*FCY) >=  3 ? 3u*1u : \
                            (FVCO/(4.0*FCY) >=  2 ? 2u*1u : \
                                                    1u*1u)))))))))))))))))))))))

#define BEST_PLLDIV (FVCO/(4.0*FCY) >= 49 ? (7<<_PLLDIV_POST1DIV_POSITION) | (7<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 42 ? (7<<_PLLDIV_POST1DIV_POSITION) | (6<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 36 ? (6<<_PLLDIV_POST1DIV_POSITION) | (6<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 35 ? (7<<_PLLDIV_POST1DIV_POSITION) | (5<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 30 ? (6<<_PLLDIV_POST1DIV_POSITION) | (5<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 28 ? (7<<_PLLDIV_POST1DIV_POSITION) | (4<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 25 ? (5<<_PLLDIV_POST1DIV_POSITION) | (5<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 21 ? (7<<_PLLDIV_POST1DIV_POSITION) | (3<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 20 ? (5<<_PLLDIV_POST1DIV_POSITION) | (4<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 18 ? (6<<_PLLDIV_POST1DIV_POSITION) | (3<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 16 ? (4<<_PLLDIV_POST1DIV_POSITION) | (4<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 14 ? (7<<_PLLDIV_POST1DIV_POSITION) | (2<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 12 ? (6<<_PLLDIV_POST1DIV_POSITION) | (2<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >= 10 ? (5<<_PLLDIV_POST1DIV_POSITION) | (2<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >=  9 ? (3<<_PLLDIV_POST1DIV_POSITION) | (3<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >=  8 ? (4<<_PLLDIV_POST1DIV_POSITION) | (2<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >=  7 ? (7<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >=  6 ? (6<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >=  5 ? (5<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >=  4 ? (4<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >=  3 ? (3<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                    (FVCO/(4.0*FCY) >=  2 ? (2<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                                            (1<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION)))))))))))))))))))))))


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
    EZBL_SET_CONF(_FSEC, BWRP_OFF & BSS_DISABLED & BSEN_OFF & GWRP_OFF & GSS_DISABLED & CWRP_OFF & CSS_DISABLED & AIVTDIS_OFF);
    EZBL_SET_CONF(_FOSCSEL, FNOSC_FRC & IESO_OFF);
    EZBL_SET_CONF(_FOSC, POSCMD_XT & & XTCFG_G1 & XTBST_ENABLE & OSCIOFNC_ON & FCKSM_CSECME);
    EZBL_SET_CONF(_FWDT, RWDTPS_PS2048 & RCLKSEL_LPRC & WINDIS_ON & SWDTPS_PS2048 & FWDTEN_ON_SW);
    EZBL_SET_CONF(_FPOR, BSSO_NORMAL);              // BSSO_NORMAL = CPU resets to program address 0x000000; BSSO_BOOT = CPU resets to program address 0x000200 when Boot Segment is defined (first executable location after IVT);
    EZBL_SET_CONF(_FICD, ICS_PGD2 & JTAGEN_OFF);    // PGEC1/PGED1 is RB9/RB8, PGEC2/PGED2 is RB4/RB3, PGEC3/PGED3 is RB6/RB5
    EZBL_SET_CONF(_FDMT, DMTDIS_OFF);               // DMTDIS_OFF = Deadman Timer is disabled and can be enabled by software; DMTDIS_ON = Deadman Timer is enabled and cannot be disabled by software
    EZBL_SET_CONF(_FDEVOPT1, ALTI2C1_OFF & ALTI2C2_OFF & MAXTEMP_t12n0 & SMBEN_STANDARD & SPI2PIN_PPS); // 12ns = 105C max ambient, 16ns = 125C max ambient, 20ns = >125C max ambient
    EZBL_SET_CONF(_FALTREG, CTXT1_OFF & CTXT2_OFF & CTXT3_OFF & CTXT4_OFF);
    EZBL_SET_CONF(_FMBXM, MBXM0_M2S & MBXM1_S2M);
    EZBL_SET_CONF(_FMBXHS1, MBXHSA_MBX0 & MBXHSB_MBX1);

    // Set Master CPU core to own all Port I/O pins
    EZBL_SET_CONF(_FCFGPRA0, CPRA0_MSTR & CPRA1_MSTR & CPRA2_MSTR & CPRA3_MSTR & CPRA4_MSTR);
    EZBL_SET_CONF(_FCFGPRB0, CPRB0_MSTR & CPRB1_MSTR & CPRB2_MSTR & CPRB3_MSTR & CPRB4_MSTR & CPRB5_MSTR & CPRB6_MSTR & CPRB7_MSTR & CPRB8_MSTR & CPRB9_MSTR & CPRB10_MSTR & CPRB11_MSTR & CPRB12_MSTR & CPRB13_MSTR & CPRB14_MSTR & CPRB15_MSTR);
    EZBL_SET_CONF(_FCFGPRC0, CPRC0_MSTR & CPRC1_MSTR & CPRC2_MSTR & CPRC3_MSTR & CPRC4_MSTR & CPRC5_MSTR & CPRC6_MSTR & CPRC7_MSTR & CPRC8_MSTR & CPRC9_MSTR & CPRC10_MSTR & CPRC11_MSTR & CPRC12_MSTR & CPRC13_MSTR & CPRC14_MSTR & CPRC15_MSTR);
    EZBL_SET_CONF(_FCFGPRD0, CPRD0_MSTR & CPRD1_MSTR & CPRD2_MSTR & CPRD3_MSTR & CPRD4_MSTR & CPRD5_MSTR & CPRD6_MSTR & CPRD7_MSTR & CPRD8_MSTR & CPRD9_MSTR & CPRD10_MSTR & CPRD11_MSTR & CPRD12_MSTR & CPRD13_MSTR & CPRD14_MSTR & CPRD15_MSTR);

    // Slave CPU Configuration words
    EZBL_SET_CONF(_FS1OSCSEL, S1FNOSC_FRC & S1IESO_OFF);
    EZBL_SET_CONF(_FS1OSC, S1OSCIOFNC_ON & S1FCKSM_CSECME);
    EZBL_SET_CONF(_FS1WDT, S1RWDTPS_PS8192 & S1RCLKSEL_LPRC & S1WINDIS_ON & S1SWDTPS_PS8192 & S1FWDTEN_ON_SW);  // Normal watchdog timer with ~250ms timeout in run and sleep using LPRC. Watchdog is turned on by software with WDTCON.ON.
    EZBL_SET_CONF(_FS1ICD, S1ICS_PGD3 & S1DEBUG_OFF & S1ISOLAT_ON & S1NOBTSWP_OFF); // Single Partition mode on Slave PRAM - BOOTSWP instruction is disabled
    //EZBL_SET_CONF(_FS1ICD, S1ICS_PGD3 & S1DEBUG_OFF & S1ISOLAT_ON & S1NOBTSWP_ON);  // Dual Partition mode on Slave PRAM - BOOTSWP instruction is enabled
    EZBL_SET_CONF(_FS1DEVOPT, S1ALTI2C1_OFF & S1SPI1PIN_PPS & S1SSRE_OFF & S1MSRE_OFF);
    EZBL_SET_CONF(_FS1ALTREG, S1CTXT1_OFF & S1CTXT2_OFF & S1CTXT3_OFF & S1CTXT4_OFF);
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

    // Configure PLL for Fosc = 1200MHz/Fcy = 100MIPS using 8.0 MHz internal FRC oscillator
    CLKDIV = 0xB000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1
    CLKDIVbits.PLLPRE = 1u + ((PLL_IN_CLK)/64000000ul); // 1:1 PLL Input clock divider (use raw 8.0 MHz FRC clock); PLL Input legal range is 8-64 MHz; PLL Feedback Input must be between 8 MHz and FVCO/16 MHz; VCO Output limit is 400MHz to 1600MHz
    PLLDIV = BEST_PLLDIV;
    PLLFBD = NET_POST_DIV*FCY/((unsigned long)((PLL_IN_CLK)/((unsigned long)(1u + (PLL_IN_CLK/64000000ul)))));
    __builtin_write_OSCCONH(0x01);      // Initiate Clock Switch to use the FRC Oscillator + PLL (NOSC = 0b001)
    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);

    // Wait for clock switch and PLL locking to complete when not done
    // automatically in hardware
    while(_OSWEN);
    while(!OSCCONbits.LOCK);


    // Initialize/select 16-bit hardware timer for NOW time keeping/scheduling
    // APIs. This call selects the hardware timer resource (can be TMR1-TMR6 or
    // CCP1-CCP8, if available) and the _Tx/_CCTxInterrupt gets automatically
    // implemented by code in ezbl_lib.a.
    NOW_Reset(CCP4, FCY);


    // Set push buttons as GPIO inputs
    //
    // Function     dsPIC33CH128MP508 Device Pins
    // Button       PIC#, name
    // S4 (LSb)       44, RE9
    // S5             39, RE7   <-- shared with D10 MSb LED, so not enabled as a button
    // S6             42, RE8
    // S3 (MSb)        1, RP46/PWM1H/RB14
    EZBL_DefineButtonMap(RB14, RE8, RE9);
    _TRISE9  = 1;
    _TRISE8  = 1;
    _TRISB14 = 1;
    PG1IOCONHbits.PENH = 0; // Disable PWM1H function so RB14 GPIO function can be used


    // Set LED pins as GPIO outputs
    //
    // Function     dsPIC33CH128MP508 Device Pins
    // LED          PIC#, name
    // D3  (LSb)      2, RE0
    // D4             4, RE1
    // D5            17, RE2
    // D6            19, RE3
    // D7            22, RE4
    // D8            24, RE5
    // D9            37, RE6
    // D10 (MSb)     39, RE7    <-- shared with S5 push button
    EZBL_DefineLEDMap(RE7, RE6, RE5, RE4, RE3, RE2, RE1, RE0);  // Generates the EZBL_LEDMap[] array in flash that will be referenced by LEDToggle()/LEDSet()/LEDOn()/LEDOff() or EZBL_Map*() APIs
    ((volatile unsigned char*)&LATE)[0]  = 0x00;                // Write '0' values to all of the LED Map's LATx bits without affecting other bits
    ((volatile unsigned char*)&TRISE)[0] = 0x00;                // Write '0' values to all of the LED Map's TRISx bits without affecting other bits



// Slave core does not have UART2, so only the Master CPU should configure this
#if defined(XPRJ_uart) && defined(_RPOUT_U2TX)  // Defined by MPLAB X on command line when compiling "uart" Build Configuration
    // Configure Master UART2 to MCP2221A
    // Function         Device Pins
    // UART2            PIC#, name
    // U2RX  (in)         66, RP58/RC10
    // U2TX  (out)        67, RP59/RC11
    _CNPUC10 = 1;               // Enable Weak pull up on U2RX line so we don't receive spurious data while nobody is connected
    _U2RXR   = 58;              // U2RX = RP58 = RC10, pin 66
    _RP59R   = _RPOUT_U2TX;     // U2TX = RP59 = RC11, pin 67
    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 1);
#endif


//    // Configure UART1 to mikroBUS A P52/RX and P51/TX pins for debug or other use
//    // Function         Device Pins
//    // UART1            PIC#, name
//    // U1RX  (in)         74, RP64/RD0
//    // U1TX  (out)        73, RP65/RD1
//    _CNPUD0 = 1;            // Enable Weak pull up on U1RX line so we don't receive spurious data if nobody is connected
//    _U1RXR  = 64;           // U1RX = RP64 = RD0, pin 74
//    _RP65R  = _RPOUT_U1TX;  // U1TX = RP65 = RD1, pin 73
//    EZBL_COMBootIF = UART_Reset(1, FCY, EZBL_COMBaud, 1);


#if defined(XPRJ_i2c)   // Defined by MPLAB X on command line when compiling "i2c" Build Configuration
    // Configure I2C1 pins for MCP2221A
    // Function         Explorer 16 PIM Header      dsPIC33CH128MP508 Device Pins
    // I2C 1            PIM#, PICtail#, name        PIC#, name
    // SDA1 (in/out)      56, 8, RG3/SDA1             61, PGC1/AN11/RP41/SDA1/RB9
    // SCL1 (in/out)      57, 6, RG2/SCL1             60, PGD1/AN10/RP40/SCL1/RB8
    _CNPUB8  = 1;   // Enable Weak pull up on SCL1 line so pins float to idle state if not connected or externally pulled up
    _CNPUB9  = 1;   // Enable Weak pull up on SDA1 line so pins float to idle state if not connected or externally pulled up
    _ANSELB8 = 0;
    _ANSELB9 = 0;
    _RP40R   = 0;   // Null output function - disable any PPS assignment on this pin since SCL1 function comes after RP40 (I2C is lower priority)
    _RP41R   = 0;   // Null output function - disable any PPS assignment on this pin since SDA1 function comes after RP41 (I2C is lower priority)
    EZBL_COMBootIF = I2C_Reset(1, FCY, 0, EZBL_i2cSlaveAddr, 0);
#endif

    
    // Report 90 MIPS on dsPIC33C
    return FCY;
}


#endif // #if defined(__dsPIC33CH128MP508__) || defined(__dsPIC33CH128MP506__) || defined(__dsPIC33CH128MP505__) || defined(__dsPIC33CH128MP503__) || defined(__dsPIC33CH128MP502__) || defined(__dsPIC33CH128MP508S1__) || defined(__dsPIC33CH128MP506S1__) || defined(__dsPIC33CH128MP505S1__) || defined(__dsPIC33CH128MP503S1__) || defined(__dsPIC33CH128MP502S1__)
