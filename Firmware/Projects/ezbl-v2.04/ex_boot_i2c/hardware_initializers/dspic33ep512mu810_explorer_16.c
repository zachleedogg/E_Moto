/*
 * File:   dspic33ep512mu810_explorer_16.c
 *
 * Initializes the device Configuration bytes, clock frequency, UART2 pins,
 * LED I/O, Button I/O, 25LC256 SPI2 pins, and clock frequency for the
 * dsPIC33EP512MU810 PIM on the Explorer 16 or Explorer 16/32 development 
 * board. For I2C projects, I2C 1 is configured for use with ASCL1/ASDA1, but 
 * these pins are not accessible via the Explorer 16/32's MCP2221A interface
 * without using air-wire bridges.
 *
 * dsPIC33EP512MU810 product page:
 *   http://www.microchip.com/dspic33ep512mu810
 *
 * dsPIC33EP512MU810 PIM Info Sheet and schematic:
 *   http://ww1.microchip.com/downloads/en/DeviceDoc/PIM_810_infosheet.pdf
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

#if defined(__dsPIC33EP512MU810__) || defined(__dsPIC33EP256MU810__) || defined(__PIC24EP512GU810__) || defined(__PIC24EP256GU810__)


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
    while(!_LOCK && ((OSCCONbits.COSC & 0x5) == 0x1));  // COSC = 0x0 = Fast RC Oscillator (FRC); 0x1 = Fast RC Oscillator (FRC) with Divide-by-N and PLL; 0x2 = Primary Oscillator (XT, HS, EC); 0x3 = Primary Oscillator (XT, HS, EC) with PLL; 0x4 = Secondary Oscillator (SOSC); 0x5 = Low-Power RC Oscillator (LPRC); 0x6 = Fast RC Oscillator (FRC) with Divide-by-16; 0x7 = Fast RC Oscillator (FRC) with Divide-by-N
    

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
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512MU810 Device Pins
    // Button       PIM#, PICtail#, name        PIC#, name
    // S4 (LSb)       80, 106, RD13               80, RP77/RD13
    // S5             92,  74, RA7                92, AN23/RP23/RA7         <- Pin function is muxed with LED D10; S5 button can't be used because LED clamps weak 10k pull up voltage too low
    // S6             84, 100, RD7                84, C3INA/VCMPST3/RP71/RD7
    // S3 (MSb)       83,  99, RD6                83, C3INB/RP70/RD6
    EZBL_DefineButtonMap(RD6, RD7, RD13);
    _TRISD13 = 1;
    //_TRISA7  = 1; // BUTTON1 + LED D10 (bit 7)
    _TRISD7  = 1;
    _TRISD6  = 1;
    //_ANSA7   = 0; // BUTTON1 + LED D10 (bit 7)
    _ANSD7   = 0;
    _ANSD6   = 0;


    // Set LED pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512MU810 Device Pins
    // LED          PIM#, PICtail#, name        PIC#, name
    // D3 (LSb)       17, 69, RA0/TMS             17, TMS/RP16/RA0
    // D4             38, 70, RA1/TCK             38, TCK/RP17/RA1
    // D5             58, 38, RA2/SCL2            58, ASCL2/RP18/RA2
    // D6             59, 40, RA3/SDA2            59, ASDA2/RP19/RA3
    // D7             60, 71, RA4/TDI             60, TDI/RP20/RA4
    // D8             61, 72, RA5/TDO             61, TDO/RP21/RA5
    // D9             91, 73, RA6                 91, AN22/RP22/RA6
    // D10 (MSb)      92, 74, RA7                 92, AN23/RP23/RA7   <- Pin function is muxed with button S5; we will use it as an LED output only
    EZBL_DefineLEDMap(RA7, RA6, RA5, RA4, RA3, RA2, RA1, RA0);
    ((volatile unsigned char *)&LATA)[0]   = 0x00;  // Write bits LATA<7:0> simultaneously, don't change LATA<15:8>
    ((volatile unsigned char *)&TRISA)[0]  = 0x00;
    ((volatile unsigned char *)&ANSELA)[0] = 0x00;


    // Set 16x2 character LCD pins as GPIO outputs
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512MU810 Device Pins       PIC24EP512GU810 Device Pins
    // 16x2 LCD     PIM#, PICtail#, name        PIC#, name                          PIC#, name
    // Data 0         93, 109, RE0/PMPD0          93, AN24/PWM1L/PMD0/RP80/RE0        93, AN24/PMD0/RP80/RE0
    // Data 1         94, 110, RE1/PMPD1          94, AN25/PWM1H/PMD1/RP81/RE1        94, AN25/PMD1/RPI81/RE1
    // Data 2         98, 111, RE2/PMPD2          98, AN26/PWM2L/PMD2/RP82/RE2        98, AN26/PMD2/RP82/RE2
    // Data 3         99, 112, RE3/PMPD3          99, AN27/PWM2H/PMD3/RP83/RE3        99, AN27/PMD3/RPI83/RE3
    // Data 4        100, 113, RE4/PMPD4         100, AN28/PWM3L/PMD4/RP84/RE4       100, AN28/PMD4/RP84/RE4
    // Data 5          3, 114, RE5/PMPD5           3, AN29/PWM3H/PMD5/RP85/RE5         3, AN29/PMD5/RP85/RE5
    // Data 6          4, 115, RE6/PMPD6           4, AN30/PWM4L/PMD6/RP86/RE6         4, AN30/PMD6/RPI86/RE6
    // Data 7          5, 116, RE7/PMPD7           5, AN31/PWM4H/PMD7/RP87/RE7         5, AN31/PMD7/RP87/RE7
    // E (Enable)     81,  97, RD4/PMPWR          81, PMWR/RP68/RD4                   81, PMWR/RP68/RD4
    // R/!W           82,  98, RD5/PMPRD          82, PMRD/RP69/RD5                   82, PMRD/RP69/RD5
    // RS (Reg Sel)   44,  84, RB15/PMPA0         44, AN15/PMA0/RP47/RB15             44, AN15/PMA0/RPI47/RB15
    ((volatile unsigned char *)&LATE)[0]   = 0x00;  // Write bits LATE<7:0> simultaneously, don't change LATE<15:8>
    ((volatile unsigned char *)&TRISE)[0]  = 0x00;
    ((volatile unsigned char *)&ANSELE)[0] = 0x00;
#if defined(IOCON1) // PIC24EP512GU8xx/PIC24EP512GP8xx/PIC24EP256GU8xx/PIC24EP256GP8xx/dsPIC33EP512GP806 devices don't have Motor Control High Speed PWM
    IOCON1bits.PENL = 0;    // Disable PWM1L control so RE0 GPIO can be used
    IOCON1bits.PENH = 0;    // Disable PWM1H control so RE1 GPIO can be used
    IOCON2bits.PENL = 0;    // Disable PWM2L control so RE2 GPIO can be used
    IOCON2bits.PENH = 0;    // Disable PWM2H control so RE3 GPIO can be used
    IOCON3bits.PENL = 0;    // Disable PWM3L control so RE4 GPIO can be used
    IOCON3bits.PENH = 0;    // Disable PWM3H control so RE5 GPIO can be used
    IOCON4bits.PENL = 0;    // Disable PWM4L control so RE6 GPIO can be used
    IOCON4bits.PENH = 0;    // Disable PWM4H control so RE7 GPIO can be used
#endif
    _LATD4   = 0;
    _LATD5   = 0;
    _TRISD4  = 0;
    _TRISD5  = 0;
    _LATB15  = 0;
    _TRISB15 = 0;
    _ANSB15  = 0;


    // Configure SPI2 pins for 25LC256 (32Kbyte SPI EEPROM)
    // - Pin names are with respect to the PIC, which is the SPI Master.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   SPI hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512MU810 Device Pins
    // SPI2         PIM#, PICtail#, name        PIC#, name
    // !CS   (out)    79, 105, RD12               79, RP76/RD12
    // SCK2  (out)    10,  35, RG6/PMPA5/SCK2     10, C1IND/SCK2/PMA5/RP118/RG6
    // SDI2  (in)     11,  37, RG7/PMPA4/SDI2     11, C1INC/SDI2/PMA4/RP119/RG7
    // SDO2  (out)    12,  39, RG8/PMPA3/SDO2     12, C2IND/SDO2/PMA3/RP120/RG8
    _LATD12  = 1;               // 1 is inactive
    _TRISD12 = 0;               // !CS on RD12
    _CNPDG7  = 1;               // Turn on pull down on SDI2 so it doesn't float when SPI module tri-states it
    _ANSG6   = 0;               // SCK2 
    _ANSG7   = 0;               // SDI2 (MISO2) on dedicated pin function
    _ANSG8   = 0;               // SDO2 (MOSI2) on dedicated pin function


#if defined(XPRJ_uart)  // Defined by MPLAB X on command line when compiling "uart" Build Configuration
    // Configure UART2 pins as UART.
    // - Pin names are with respect to the PIC.
    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
    //   UART2 hardware overrides it.
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512MU810 Device Pins
    // UART2        PIM#, PICtail#, name        PIC#, name
    // U2RX  (in)     49, 34, RF4/PMPA9/U2RX      49, SDA2/PMA9/RP100/RF4
    // U2TX  (out)    50, 36, RF5/PMPA8/U2TX      50, SCL2/PMA8/RP101/RF5
    _CNPUF4    = 1;                         // Turn on weak pull up on U2RX so no spurious data arrives if nobody connected
    _U2RXR     = 100;                       // U2RX on RP100/RF4
    _RP101R    = _RPOUT_U2TX;               // U2TX on RP101/RF5
    if(EZBL_COMBaud <= 0)       // If auto-baud enabled, delay our UART initialization so MCP2221A POR timer and init
    {                           // is complete before we start listening. POR timer max spec is 140ms, so MCP2221A TX
        NOW_Wait(140u*NOW_ms);  // pin glitching could occur long after we have enabled our UART without this forced delay.
    }
    EZBL_COMBootIF = UART_Reset(2, FCY, EZBL_COMBaud, 1);
#endif


#if defined(XPRJ_i2c)   // Defined by MPLAB X on command line when compiling "i2c" Build Configuration
    // Configure additional pins for I2C use on ASCL1/ASDA1 pins. You need 
    // air-wires to make use of this. On the Explorer 16/32, place jumper
    // wires on the J48 socket to short P70 with P57 and P69 with P56 to use of
    // the on-board MCP2221A I2C interface. Additionally ensure the J41 and J42
    // jumper headers have shunts installed to actually connect "P56_SDA" and
    // "P57_SCL" to the MCP2221A.
    //
    // Function         Explorer 16 PIM Header      dsPIC33EP512MU810 Device Pins
    // I2C1             PIM#, PICtail#, name        PIC#, name
    // ASDA1 (in/out)    69,       102, RD9           69, ASDA1/DPLN/RPI73/RD9
    // ASCL1 (in/out)    70,       103, RD10/PMPCS2   70, ASCL1/PMCS2/RPI74/RD10
    _CNPUD9  = 1;   // Only adds ~13K pull-up resistance, so externally should have more current
    _CNPUD10 = 1;   // Only adds ~13K pull-up resistance, so externally should have more current
    EZBL_COMBootIF = I2C_Reset(1, FCY, 0, EZBL_i2cSlaveAddr, 0);
#endif


    // Configure Analog Inputs for U4 TC1047A Temperature Sensor and R6 10K Potentiometer
    //
    // Function     Explorer 16 PIM Header      dsPIC33EP512MU810 Device Pins
    // Analog Input PIM#, PICtail#, name        PIC#, name
    // TC1047A Temp   21, 14, RB4/AN4             21, AN4/C1INB/USBOEN/RP36/RB4
    // 10K Pot        20, 77, RB5/AN5             20, AN5/C1INA/VBUSON/VBUSST/RP37/RB5        <- Only connected in NON-USB jumpered mode
    _TRISB4 = 1;
    _TRISB5 = 1;
    _ANSB4  = 1;
    _ANSB5  = 1;



//    // Configure additional pins for Serial SuperFlash Kit 1 PICtail Plus
//    // (SST25VF016B) connected to the SPI2 middle slot of the Explorer 16.
//    // - Pin names are with respect to the PIC, which is the SPI Master.
//    // - Outputs bits in TRIS registers are all set as inputs because the PPS or
//    //   SPI hardware overrides it.
//    //
//    // Function     Explorer 16 PIM Header      dsPIC33EP512MU810 Device Pins
//    // SPI2         PIM#, PICtail#, name        PIC#, name
//    // /CS   (out)    14,  33, RG9/PMPA2/SS2      14, C2INC/PMA2/RP121/RG9
//    // SCK2  (out)    10,  35, RG6/PMPA5/SCK2     10, C1IND/SCK2/PMA5/RP118/RG6
//    // SDI2  (in)     11,  37, RG7/PMPA4/SDI2     11, C1INC/SDI2/PMA4/RP119/RG7
//    // SDO2  (out)    12,  39, RG8/PMPA3/SDO2     12, C2IND/SDO2/PMA3/RP120/RG8
//    // /WP   (out)    90,  60, RG0                57, D+/RG2 (USB jumpered)     90, RP112/RG0 (Non-USB jumpered)
//    // /HOLD (out)    89,  62, RG1                56, D-/RG3 (USB jumpered)     89, RP113/RG1 (Non-USB jumpered)
//    _LATG9  = 1;    // !CS
//    _TRISG9 = 0;
//    _ANSG9  = 0;
//    _LATG0  = 1;    // Write Protect - Deassert (Non-USB jumpered)
//    _TRISG0 = 0;
//    _LATG1  = 1;    // HOLD - Deassert (Non-USB jumpered)
//    _TRISG1 = 0;
//    //_LATG2  = 1;    // Write Protect - Deassert (USB jumpered)
//    //_TRISG2 = 0;
//    //_LATG3  = 1;    // HOLD - Deassert (USB jumpered)
//    //_TRISG3 = 0;
//    // I/O pins for SPI2 already configured above for 25LC256 pins, so no need to repeat
//    //_TRISG6  = 1;
//    //_TRISG7  = 1;
//    //_TRISG8  = 1;
//    //_ANSG6   = 0;   // SCK2
//    //_ANSG7   = 0;   // SDI2 (MISO2)
//    //_ANSG8   = 0;   // SDO2 (MOSI2)


//    // Configure CAN1, CAN2, LIN1 communications pins for
//    // AC164130-2: CAN/LIN/J2602 PICtail/PICtail Plus Daughter board
//    //
//    // Function     Explorer 16 PICtail Plus      dsPIC33EP512MU810 Device Pins
//    //              PIM#, PICtail#, name          PIC#, name
//    // CAN1TX         88,       30, RF1             88, VCMPST2/RP97/RF1
//    // CAN1RX         87,       28, RF0             87, VCMPST1/RP96/RF0
//    // CAN2TX         89,       62, RG1             89/56, RP113/RG1 for non-USB; D-/RG3 for USB
//    // CAN2RX         90,       60, RG0             90/57, RP112/RG0 for non-USB; D+/RG2 for USB
//    // LIN1TX         51,        4, RF3/U1TX_E      51/NC, USBID/RP99/RF3 for non-USB; No Connect when jumpered for USB
//    // LIN1RX         52,        2, RF2/U1RX_E      52, RP98/RF2
//    // LIN1CS         18,       18, RE8/INT1        18, AN20/RP88/RE8
//    // LIN1TXE        48,       20, RD15/U1RTS_E    48, RP79/RD15
//    // LIN2TX         50,       36, RF5/PMA8/U2TX   50, SCL2/PMA8/RP101/RF5     // Note: UART2 RS232 transceiver muxed pin
//    // LIN2RX         49,       34, RF4/PMA9/U2RX   49, SDA2/PMA9/RP100/RF4     // Note: UART2 RS232 transceiver muxed pin
//    // LIN2CS         66,       50, RA14/INT3       66, RP30/RA14
//    // LIN2TXE        39        52, RF13/U2RTS      39, RP109/RF13              // Note: UART2 RS232 transceiver muxed pin
//    _TRISF1  = 1;
//    _TRISF0  = 1;
//    _RP97R   = _RPOUT_C1TX;     // C1TX function on RP97
//    _C1RXR   = 96;              // C1RX function on RP96
//    _LATE8   = 1;               // Deassert LIN1CS (drive logic high)
//    _TRISE8  = 0;
//    _ANSE8   = 0;
//    _LATA14  = 1;               // Deassert LIN2CS (drive logic high)
//    _TRISA14 = 0;


//    // Configure UART1 as an extra interface for debugging via an MCP2221A
//    // Breakout Board connected (vertically) to the mikroBUS A, left pin
//    // header, MCP2221A chip faces right, away from PIM:
//    // Function     Explorer 16 PIM Header      dsPIC33EP512MU810 Device Pins
//    //              PIM#, PICtail#, name        PIC#, name
//    // MCP2221A TX    25,         , AN/P25        25, PGED3/AN0/RPI32/RB0
//    // MCP2221A VDD   95,         , RST/P95       51, USBID/RP99/RF3 (USB-jumper settings); 95, RP126/RG14 (non-USB jumpers); (software must drive to '1' for direct plug-in convenience)
//    // MCP2221A GND   14,         , CS/P14        14, C2IN1-/PMA2/RPI121/RG9    (software must drive to '0' for direct plug-in convenience)
//    // MCP2221A SDA   10,         , SCK/P10       10, C1IN3-/SCK2/PMA5/RP118/RG6
//    // MCP2221A SCL   11,         , MISO/P11      11, C1IN1-/SDI2/PMA4/RPI119/RG7
//    // MCP2221A RX    12,         , MOSI/P12      12, C2IN3-/SDO2/PMA3/RP120/RG8
//    _CNPUB0 = 1;                // Turn on weak pull up on U1RX so no spurious data arrives if nobody connected
//    _U1RXR  = 32;               // U1RX on RPI32
//    _ANSB0   = 0;
//    _LATF3   = 1;               // GPIO driving to '1' for VDD - USB-jumper settings
//    _TRISF3  = 0;
//    _LATG14  = 1;               // GPIO driving to '1' for VDD - Non-USB jumper settings
//    _TRISG14 = 0;
//    _LATG9  = 0;                // GPIO driving to '0' for VSS
//    _TRISG9 = 0;
//    _ANSG9  = 0;
//    _TRISG8  = 1;
//    _RP120R  = _RPOUT_U1TX;     // U1TX on RP120 - Clobbers SPI2
//    _ANSG8   = 0;
//    UART_Reset(1, FCY, EZBL_COMBaud, 1);

    // Report 70 MIPS on dsPIC33E
    return FCY;
}


#endif //#if defined(__dsPIC33EP512MU810__) || defined(__dsPIC33EP256MU810__) || defined(__PIC24EP512GU810__) || defined(__PIC24EP256GU810__)
