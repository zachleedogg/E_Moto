/*******************************************************************************
  Easy Bootloader Library API Source File

  Summary:
    Weak implementation of NOW_TMR4_ResetConst() function (call NOW_Reset() 
    macro instead) that uses Timer 4 (16-bit mode).
 
    _T4Interrupt() ISR in ezbl_lib.a is added when using this implementation.
 *******************************************************************************/

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

#include "../ezbl.h"


// Peripheral Timer/CCP # for this NOW module to use. NOTE: Currently only
// tested with Timer modules, not CCPs.
#define TMR_IS_T                            // Uncomment to implement/use classic Timer module (ex: Timer 1, Timer 2, Timer 3, etc.)
//#define TMR_IS_CCP                        // Uncomment to implmeent/use MCCP/SCCP Capture Compare PWM Timer module type
#define NOW_TIMER_NUM                   4   // Peripheral instance number

EZBL_SetWeakSYM(NOW_TIMER_ISR_PRIORITY, 4); // CPU IPL (Interrupt Priority Level) to
                                            // assign the Timer's ISR to. 0 means main()
                                            // context, which is the lowest priority and
                                            // 7 is the highest, time-critical,
                                            // priority.
                                            //
                                            // NOTE: Do not assign a hardware Alternate Register Set to this IPL!!!
                                            // If set, you must use a different IPL for this timer (recommend going
                                            // up one priority).



#if defined(EZBL_LIB16CH) || defined(__dsPIC33CH__)
typedef struct
{
    volatile unsigned int :5;           // PMD1
    volatile unsigned int U1MD:1;       //      bit 5
    volatile unsigned int U2MD:1;
    volatile unsigned int I2C1MD:1;     //      bit 7
    volatile unsigned int :3;
    volatile unsigned int T1MD:1;       //      bit 11
    volatile unsigned int :4;
    volatile unsigned int CCP1MD:1;     // PMD2
    volatile unsigned int CCP2MD:1;
    volatile unsigned int CCP3MD:1;
    volatile unsigned int CCP4MD:1;
    volatile unsigned int CCP5MD:1;
    volatile unsigned int CCP6MD:1;
    volatile unsigned int CCP7MD:1;
    volatile unsigned int CCP8MD:1;
    volatile unsigned int :8;
    volatile unsigned int :1;           // PMD3
    volatile unsigned int I2C2MD:1;     //      bit 33
    volatile unsigned int :5;
    volatile unsigned int CRCMD:1;
    volatile unsigned int :8;
    volatile unsigned int :16;          // PMD4
} PMDFIELDS;

typedef struct
{
    volatile unsigned int INT0IE:1;      // IEC0     IEC0<0>     INT0 I/O
    volatile unsigned int T1IE:1;        //          IEC0<1>     Timer 1
    volatile unsigned int CNAIE:1;       //          IEC0<2>     Change Notice A
    volatile unsigned int CNBIE:1;       //          IEC0<3>     Change Notice B
    volatile unsigned int DMA0IE:1;      //          IEC0<4>     DMA Channel 0
    volatile unsigned int :1;
    volatile unsigned int CCP1IE:1;      //          IEC0<6>     Capture/Compare/PWM 1
    volatile unsigned int CCT1IE:1;      //          IEC0<7>     Capture/Compare/PWM 1 second timer
    volatile unsigned int DMA1IE:1;      //          IEC0<8>     DMA Channel 1
    volatile unsigned int SPI1RXIE:1;    //          IEC0<9>     SPI 1 Receive
    volatile unsigned int SPI1TXIE:1;    //          IEC0<10>    SPI 1 Transmit
    volatile unsigned int U1RXIE:1;      //          IEC0<11>    UART 1 Receive
    volatile unsigned int U1TXIE:1;      //          IEC0<12>    UART 1 Transmit
    volatile unsigned int ECCSBEIE:1;    //          IEC0<13>    NVM ECC Single Bit Error Corrected
    volatile unsigned int NVMIE:1;       //          IEC0<14>    NVM Controller
    volatile unsigned int INT1IE:1;      //          IEC0<15>    INT1 I/O
    volatile unsigned int SI2C1IE:1;     // IEC1     IEC1<0>     I2C 1 Slave
    volatile unsigned int MI2C1IE:1;     //          IEC1<1>     I2C 1 Master
    volatile unsigned int DMA2IE:1;      //          IEC1<2>     DMA Channel 2
    volatile unsigned int CNCIE:1;       //          IEC1<3>     Change Notice C
    volatile unsigned int INT2IE:1;      //          IEC1<4>     INT2 I/O
    volatile unsigned int DMA3IE:1;      //          IEC1<5>     DMA Channel 3
    volatile unsigned int DMA4IE:1;      //          IEC1<6>     DMA Channel 4
    volatile unsigned int CCP2IE:1;      //          IEC1<7>     Capture/Compare/PWM 2
    volatile unsigned int CCT2IE:1;      //          IEC1<8>     Capture/Compare/PWM 2 second timer
    volatile unsigned int C1IE:1;        //          IEC1<9>     CAN 1 Combined Error
    volatile unsigned int INT3IE:1;      //          IEC1<10>    INT3 I/O
    volatile unsigned int U2RXIE:1;      //          IEC1<11>    UART 2 Receive
    volatile unsigned int U2TXIE:1;      //          IEC1<12>    UART 2 Transmit
    volatile unsigned int SPI2RXIE:1;    //          IEC1<13>    SPI 2 Receive
    volatile unsigned int SPI2TXIE:1;    //          IEC1<14>    SPI 2 Transmit
    volatile unsigned int C1RXIE:1;      //          IEC1<15>    CAN 1 RX Data Ready
    volatile unsigned int :2;            // IEC2
    volatile unsigned int DMA5IE:1;      //          IEC2<2>     DMA Channel 5
    volatile unsigned int CCP3IE:1;      //          IEC2<3>     Capture/Compare/PWM 3
    volatile unsigned int CCT3IE:1;      //          IEC2<4>     Capture/Compare/PWM 3 second timer
    volatile unsigned int SI2C2IE:1;     //          IEC2<5>     I2C 2 Slave
    volatile unsigned int MI2C2IE:1;     //          IEC2<6>     I2C 2 Master
    volatile unsigned int :1;
    volatile unsigned int CCP4IE:1;      //          IEC2<8>     Capture/Compare/PWM 4
    volatile unsigned int CCT4IE:1;      //          IEC2<9>     Capture/Compare/PWM 4 second timer
    volatile unsigned int :1;
    volatile unsigned int CCP5IE:1;      //          IEC2<11>    Capture/Compare/PWM 5
    volatile unsigned int CCT5IE:1;      //          IEC2<12>    Capture/Compare/PWM 5 second timer
    volatile unsigned int DMTIE:1;       //          IEC2<13>    Deadman Timer
    volatile unsigned int CCP6IE:1;      //          IEC2<14>    Capture/Compare/PWM 6
    volatile unsigned int CCT6IE:1;      //          IEC2<15>    Capture/Compare/PWM 6 second timer
    volatile unsigned int QEI1IE:1;      // IEC3     IEC3<0>     Quadrature Encoder Position 1
    volatile unsigned int U1EIE:1;       //          IEC3<1>     UART 1 Error
    volatile unsigned int U2EIE:1;       //          IEC3<2>     UART 2 Error
    volatile unsigned int CRCIE:1;       //          IEC3<3>     CRC Generator
    volatile unsigned int C1TXIE:1;      //          IEC3<4>     CAN 1 TX Data Request
    volatile unsigned int :9;
    volatile unsigned int JTAGIE:1;      //          IEC3<14>    JTAG Programming
    volatile unsigned int PTGSTEPIE:1;   //          IEC3<15>    Peripheral Trigger Generator Step
    volatile unsigned int I2C1BCIE:1;    // IEC4     IEC4<0>     I2C 1 Bus Collision
    volatile unsigned int I2C2BCIE:1;    //          IEC4<1>     I2C 2 Bus Collision
    volatile unsigned int :1;
    volatile unsigned int PWM1IE:1;      //          IEC4<3>     High Resolution PWM 1
    volatile unsigned int PWM2IE:1;      //          IEC4<4>     High Resolution PWM 2
    volatile unsigned int PWM3IE:1;      //          IEC4<5>     High Resolution PWM 3
    volatile unsigned int PWM4IE:1;      //          IEC4<6>     High Resolution PWM 4
    volatile unsigned int :4;
    volatile unsigned int CNDIE:1;       //          IEC4<11>    Change Notice Port D
    volatile unsigned int CNEIE:1;       //          IEC4<12>    Change Notice Port E
    volatile unsigned int CMP1IE:1;      //          IEC4<13>    Analog Comparator 1
    volatile unsigned int :2;
    volatile unsigned int :1;            // IEC5
    volatile unsigned int PTGWDTIE:1;    //          IEC5<1>     Peripheral Trigger Generator Watchdog Timeout
    volatile unsigned int PTG0IE:1;      //          IEC5<2>     Peripheral Trigger Generator Trigger 0
    volatile unsigned int PTG1IE:1;      //          IEC5<3>     Peripheral Trigger Generator Trigger 1
    volatile unsigned int PTG2IE:1;      //          IEC5<4>     Peripheral Trigger Generator Trigger 2
    volatile unsigned int PTG3IE:1;      //          IEC5<5>     Peripheral Trigger Generator Trigger 3
    volatile unsigned int SENT1IE:1;     //          IEC5<6>     SENT 1 TX/RX
    volatile unsigned int SENT1EIE:1;    //          IEC5<7>     SENT 1 Error
    volatile unsigned int SENT2IE:1;     //          IEC5<8>     SENT 2 TX/RX
    volatile unsigned int SENT2EIE:1;    //          IEC5<9>     SENT 2 Error
    volatile unsigned int ADCIE:1;       //          IEC5<10>    ADC Global
    volatile unsigned int ADCAN0IE:1;    //          IEC5<11>    ADC AN0
    volatile unsigned int ADCAN1IE:1;    //          IEC5<12>    ADC AN1
    volatile unsigned int ADCAN2IE:1;    //          IEC5<13>    ADC AN2
    volatile unsigned int ADCAN3IE:1;    //          IEC5<14>    ADC AN3
    volatile unsigned int ADCAN4IE:1;    //          IEC5<15>    ADC AN4
    volatile unsigned int ADCAN5IE:1;    // IEC6     IEC6<0>     ADC AN5
    volatile unsigned int ADCAN6IE:1;    //          IEC6<1>     ADC AN6
    volatile unsigned int ADCAN7IE:1;    //          IEC6<2>     ADC AN7
    volatile unsigned int ADCAN8IE:1;    //          IEC6<3>     ADC AN8
    volatile unsigned int ADCAN9IE:1;    //          IEC6<4>     ADC AN9
    volatile unsigned int ADCAN10IE:1;   //          IEC6<5>     ADC AN10
    volatile unsigned int ADCAN11IE:1;   //          IEC6<6>     ADC AN11
    volatile unsigned int ADCAN12IE:1;   //          IEC6<7>     ADC AN12
    volatile unsigned int ADCAN13IE:1;   //          IEC6<8>     ADC AN13
    volatile unsigned int ADCAN14IE:1;   //          IEC6<9>     ADC AN14
    volatile unsigned int ADCAN15IE:1;   //          IEC6<10>    ADC AN15
    volatile unsigned int ADCAN16IE:1;   //          IEC6<11>    ADC AN16
    volatile unsigned int ADCAN17IE:1;   //          IEC6<12>    ADC AN17
    volatile unsigned int ADCAN18IE:1;   //          IEC6<13>    ADC AN18
    volatile unsigned int ADCAN19IE:1;   //          IEC6<14>    ADC AN19
    volatile unsigned int ADCAN20IE:1;   //          IEC6<15>    ADC AN20
    volatile unsigned int :3;            // IEC7
    volatile unsigned int ADFLTIE:1;     //          IEC7<3>    ADC Fault
    volatile unsigned int ADCMP0IE:1;    //          IEC7<4>    ADC Digital Comparator 0
    volatile unsigned int ADCMP1IE:1;    //          IEC7<5>    ADC Digital Comparator 1
    volatile unsigned int ADCMP2IE:1;    //          IEC7<6>    ADC Digital Comparator 2
    volatile unsigned int ADCMP3IE:1;    //          IEC7<7>    ADC Digital Comparator 3
    volatile unsigned int ADFLTR0IE:1;   //          IEC7<8>    ADC Oversample Filter 0
    volatile unsigned int ADFLTR1IE:1;   //          IEC7<9>    ADC Oversample Filter 1
    volatile unsigned int ADFLTR2IE:1;   //          IEC7<10>   ADC Oversample Filter 2
    volatile unsigned int ADFLTR3IE:1;   //          IEC7<11>   ADC Oversample Filter 3
    volatile unsigned int CLC1PIE:1;     //          IEC7<12>   CLC 1 Positive Edge
    volatile unsigned int CLC2PIE:1;     //          IEC7<13>   CLC 2 Positive Edge
    volatile unsigned int SPI1IE:1;      //          IEC7<14>   SPI 1 Error
    volatile unsigned int SPI2IE:1;      //          IEC7<15>   SPI 2 Error
    volatile unsigned int :1;            // IEC8
    volatile unsigned int MSISIE:1;      //          IEC8<1>    MSI Slave Initiated Interrupt
    volatile unsigned int MSIAIE:1;      //          IEC8<2>    MSI Protocol A
    volatile unsigned int MSIBIE:1;      //          IEC8<3>    MSI Protocol B
    volatile unsigned int MSICIE:1;      //          IEC8<4>    MSI Protocol C
    volatile unsigned int MSIDIE:1;      //          IEC8<5>    MSI Protocol D
    volatile unsigned int MSIEIE:1;      //          IEC8<6>    MSI Protocol E
    volatile unsigned int MSIFIE:1;      //          IEC8<7>    MSI Protocol F
    volatile unsigned int MSIGIE:1;      //          IEC8<8>    MSI Protocol G
    volatile unsigned int MSIHIE:1;      //          IEC8<9>    MSI Protocol H
    volatile unsigned int MSIDTIE:1;     //          IEC8<10>   MSI Master Read FIFO Data Ready
    volatile unsigned int MSIWFEIE:1;    //          IEC8<11>   MSI Master Write FIFO Empty
    volatile unsigned int MSIFLTIE:1;    //          IEC8<12>   MSI Read or Write FIFO Fault (Over/Underflow)
    volatile unsigned int MSISRSTIE:1;   //          IEC8<13>   MSI Slave 1 Reset
    volatile unsigned int :2;
    volatile unsigned int :2;            // IEC9
    volatile unsigned int S1BRKIE:1;     //          IEC9<2>    MSI Slave Debug Break
    volatile unsigned int :2;
    volatile unsigned int CCP7IE:1;      //          IEC9<5>    Capture/Compare/PWM 7
    volatile unsigned int CCT7IE:1;      //          IEC9<6>    Capture/Compare/PWM 7 second timer
    volatile unsigned int :1;
    volatile unsigned int CCP8IE:1;      //          IEC9<8>    Capture/Compare/PWM 8
    volatile unsigned int CCT8IE:1;      //          IEC9<9>    Capture/Compare/PWM 8 second timer
    volatile unsigned int :3;
    volatile unsigned int S1CLKFIE:1;    //          IEC9<13>    MSI Slave Clock Failure
    volatile unsigned int :2;
    volatile unsigned int ADC0EIE:1;     // IEC10    IEC10<0>    Reserved?
    volatile unsigned int ADC1EIE:1;     //          IEC10<1>    Reserved?
    volatile unsigned int :6;
    volatile unsigned int ADFIFOIE:1;    //          IEC10<8>    ADC FIFO Ready
    volatile unsigned int PEVTAIE:1;     //          IEC10<9>    High Resolution PWM Event A
    volatile unsigned int PEVTBIE:1;     //          IEC10<10>   High Resolution PWM Event B
    volatile unsigned int PEVTCIE:1;     //          IEC10<11>   High Resolution PWM Event C
    volatile unsigned int PEVTDIE:1;     //          IEC10<12>   High Resolution PWM Event D
    volatile unsigned int PEVTEIE:1;     //          IEC10<13>   High Resolution PWM Event E
    volatile unsigned int PEVTFIE:1;     //          IEC10<14>   High Resolution PWM Event F
    volatile unsigned int CLC3PIE:1;     //          IEC10<15>   CLC 3 Positive Edge
    volatile unsigned int CLC4PIE:1;     // IEC11    IEC11<0>    CLC 4 Positive Edge
    volatile unsigned int CLC1NIE:1;     //          IEC11<1>    CLC 1 Negative Edge
    volatile unsigned int CLC2NIE:1;     //          IEC11<2>    CLC 2 Negative Edge
    volatile unsigned int CLC3NIE:1;     //          IEC11<3>    CLC 3 Negative Edge
    volatile unsigned int CLC4NIE:1;     //          IEC11<4>    CLC 4 Negative Edge
    volatile unsigned int :8;
    volatile unsigned int U1EVTIE:1;     //          IEC11<13>   UART 1 Event
    volatile unsigned int U2EVTIE:1;     //          IEC11<14>   UART 2 Event
    volatile unsigned int :1;
} INTENFIELDS;

typedef struct
{
    volatile unsigned int INT0IF:1;      // IFS0     IFS0<0>     INT0 I/O
    volatile unsigned int T1IF:1;        //          IFS0<1>     Timer 1
    volatile unsigned int CNAIF:1;       //          IFS0<2>     Change Notice A
    volatile unsigned int CNBIF:1;       //          IFS0<3>     Change Notice B
    volatile unsigned int DMA0IF:1;      //          IFS0<4>     DMA Channel 0
    volatile unsigned int :1;
    volatile unsigned int CCP1IF:1;      //          IFS0<6>     Capture/Compare/PWM 1
    volatile unsigned int CCT1IF:1;      //          IFS0<7>     Capture/Compare/PWM 1 second timer
    volatile unsigned int DMA1IF:1;      //          IFS0<8>     DMA Channel 1
    volatile unsigned int SPI1RXIF:1;    //          IFS0<9>     SPI 1 Receive
    volatile unsigned int SPI1TXIF:1;    //          IFS0<10>    SPI 1 Transmit
    volatile unsigned int U1RXIF:1;      //          IFS0<11>    UART 1 Receive
    volatile unsigned int U1TXIF:1;      //          IFS0<12>    UART 1 Transmit
    volatile unsigned int ECCSBEIF:1;    //          IFS0<13>    NVM ECC Single Bit Error Corrected
    volatile unsigned int NVMIF:1;       //          IFS0<14>    NVM Controller
    volatile unsigned int INT1IF:1;      //          IFS0<15>    INT1 I/O
    volatile unsigned int SI2C1IF:1;     // IFS1     IFS1<0>     I2C 1 Slave
    volatile unsigned int MI2C1IF:1;     //          IFS1<1>     I2C 1 Master
    volatile unsigned int DMA2IF:1;      //          IFS1<2>     DMA Channel 2
    volatile unsigned int CNCIF:1;       //          IFS1<3>     Change Notice C
    volatile unsigned int INT2IF:1;      //          IFS1<4>     INT2 I/O
    volatile unsigned int DMA3IF:1;      //          IFS1<5>     DMA Channel 3
    volatile unsigned int DMA4IF:1;      //          IFS1<6>     DMA Channel 4
    volatile unsigned int CCP2IF:1;      //          IFS1<7>     Capture/Compare/PWM 2
    volatile unsigned int CCT2IF:1;      //          IFS1<8>     Capture/Compare/PWM 2 second timer
    volatile unsigned int C1IF:1;        //          IFS1<9>     CAN 1 Combined Error
    volatile unsigned int INT3IF:1;      //          IFS1<10>    INT3 I/O
    volatile unsigned int U2RXIF:1;      //          IFS1<11>    UART 2 Receive
    volatile unsigned int U2TXIF:1;      //          IFS1<12>    UART 2 Transmit
    volatile unsigned int SPI2RXIF:1;    //          IFS1<13>    SPI 2 Receive
    volatile unsigned int SPI2TXIF:1;    //          IFS1<14>    SPI 2 Transmit
    volatile unsigned int C1RXIF:1;      //          IFS1<15>    CAN 1 RX Data Ready
    volatile unsigned int :2;            // IFS2
    volatile unsigned int DMA5IF:1;      //          IFS2<2>     DMA Channel 5
    volatile unsigned int CCP3IF:1;      //          IFS2<3>     Capture/Compare/PWM 3
    volatile unsigned int CCT3IF:1;      //          IFS2<4>     Capture/Compare/PWM 3 second timer
    volatile unsigned int SI2C2IF:1;     //          IFS2<5>     I2C 2 Slave
    volatile unsigned int MI2C2IF:1;     //          IFS2<6>     I2C 2 Master
    volatile unsigned int :1;
    volatile unsigned int CCP4IF:1;      //          IFS2<8>     Capture/Compare/PWM 4
    volatile unsigned int CCT4IF:1;      //          IFS2<9>     Capture/Compare/PWM 4 second timer
    volatile unsigned int :1;
    volatile unsigned int CCP5IF:1;      //          IFS2<11>    Capture/Compare/PWM 5
    volatile unsigned int CCT5IF:1;      //          IFS2<12>    Capture/Compare/PWM 5 second timer
    volatile unsigned int DMTIF:1;       //          IFS2<13>    Deadman Timer
    volatile unsigned int CCP6IF:1;      //          IFS2<14>    Capture/Compare/PWM 6
    volatile unsigned int CCT6IF:1;      //          IFS2<15>    Capture/Compare/PWM 6 second timer
    volatile unsigned int QEI1IF:1;      // IFS3     IFS3<0>     Quadrature Encoder Position 1
    volatile unsigned int U1EIF:1;       //          IFS3<1>     UART 1 Error
    volatile unsigned int U2EIF:1;       //          IFS3<2>     UART 2 Error
    volatile unsigned int CRCIF:1;       //          IFS3<3>     CRC Generator
    volatile unsigned int C1TXIF:1;      //          IFS3<4>     CAN 1 TX Data Request
    volatile unsigned int :9;
    volatile unsigned int JTAGIF:1;      //          IFS3<14>    JTAG Programming
    volatile unsigned int PTGSTEPIF:1;   //          IFS3<15>    Peripheral Trigger Generator Step
    volatile unsigned int I2C1BCIF:1;    // IFS4     IFS4<0>     I2C 1 Bus Collision
    volatile unsigned int I2C2BCIF:1;    //          IFS4<1>     I2C 2 Bus Collision
    volatile unsigned int :1;
    volatile unsigned int PWM1IF:1;      //          IFS4<3>     High Resolution PWM 1
    volatile unsigned int PWM2IF:1;      //          IFS4<4>     High Resolution PWM 2
    volatile unsigned int PWM3IF:1;      //          IFS4<5>     High Resolution PWM 3
    volatile unsigned int PWM4IF:1;      //          IFS4<6>     High Resolution PWM 4
    volatile unsigned int :4;
    volatile unsigned int CNDIF:1;       //          IFS4<11>    Change Notice Port D
    volatile unsigned int CNEIF:1;       //          IFS4<12>    Change Notice Port E
    volatile unsigned int CMP1IF:1;      //          IFS4<13>    Analog Comparator 1
    volatile unsigned int :2;
    volatile unsigned int :1;            // IFS5
    volatile unsigned int PTGWDTIF:1;    //          IFS5<1>     Peripheral Trigger Generator Watchdog Timeout
    volatile unsigned int PTG0IF:1;      //          IFS5<2>     Peripheral Trigger Generator Trigger 0
    volatile unsigned int PTG1IF:1;      //          IFS5<3>     Peripheral Trigger Generator Trigger 1
    volatile unsigned int PTG2IF:1;      //          IFS5<4>     Peripheral Trigger Generator Trigger 2
    volatile unsigned int PTG3IF:1;      //          IFS5<5>     Peripheral Trigger Generator Trigger 3
    volatile unsigned int SENT1IF:1;     //          IFS5<6>     SENT 1 TX/RX
    volatile unsigned int SENT1EIF:1;    //          IFS5<7>     SENT 1 Error
    volatile unsigned int SENT2IF:1;     //          IFS5<8>     SENT 2 TX/RX
    volatile unsigned int SENT2EIF:1;    //          IFS5<9>     SENT 2 Error
    volatile unsigned int ADCIF:1;       //          IFS5<10>    ADC Global
    volatile unsigned int ADCAN0IF:1;    //          IFS5<11>    ADC AN0
    volatile unsigned int ADCAN1IF:1;    //          IFS5<12>    ADC AN1
    volatile unsigned int ADCAN2IF:1;    //          IFS5<13>    ADC AN2
    volatile unsigned int ADCAN3IF:1;    //          IFS5<14>    ADC AN3
    volatile unsigned int ADCAN4IF:1;    //          IFS5<15>    ADC AN4
    volatile unsigned int ADCAN5IF:1;    // IFS6     IFS6<0>     ADC AN5
    volatile unsigned int ADCAN6IF:1;    //          IFS6<1>     ADC AN6
    volatile unsigned int ADCAN7IF:1;    //          IFS6<2>     ADC AN7
    volatile unsigned int ADCAN8IF:1;    //          IFS6<3>     ADC AN8
    volatile unsigned int ADCAN9IF:1;    //          IFS6<4>     ADC AN9
    volatile unsigned int ADCAN10IF:1;   //          IFS6<5>     ADC AN10
    volatile unsigned int ADCAN11IF:1;   //          IFS6<6>     ADC AN11
    volatile unsigned int ADCAN12IF:1;   //          IFS6<7>     ADC AN12
    volatile unsigned int ADCAN13IF:1;   //          IFS6<8>     ADC AN13
    volatile unsigned int ADCAN14IF:1;   //          IFS6<9>     ADC AN14
    volatile unsigned int ADCAN15IF:1;   //          IFS6<10>    ADC AN15
    volatile unsigned int ADCAN16IF:1;   //          IFS6<11>    ADC AN16
    volatile unsigned int ADCAN17IF:1;   //          IFS6<12>    ADC AN17
    volatile unsigned int ADCAN18IF:1;   //          IFS6<13>    ADC AN18
    volatile unsigned int ADCAN19IF:1;   //          IFS6<14>    ADC AN19
    volatile unsigned int ADCAN20IF:1;   //          IFS6<15>    ADC AN20
    volatile unsigned int :3;            // IFS7
    volatile unsigned int ADFLTIF:1;     //          IFS7<3>    ADC Fault
    volatile unsigned int ADCMP0IF:1;    //          IFS7<4>    ADC Digital Comparator 0
    volatile unsigned int ADCMP1IF:1;    //          IFS7<5>    ADC Digital Comparator 1
    volatile unsigned int ADCMP2IF:1;    //          IFS7<6>    ADC Digital Comparator 2
    volatile unsigned int ADCMP3IF:1;    //          IFS7<7>    ADC Digital Comparator 3
    volatile unsigned int ADFLTR0IF:1;   //          IFS7<8>    ADC Oversample Filter 0
    volatile unsigned int ADFLTR1IF:1;   //          IFS7<9>    ADC Oversample Filter 1
    volatile unsigned int ADFLTR2IF:1;   //          IFS7<10>   ADC Oversample Filter 2
    volatile unsigned int ADFLTR3IF:1;   //          IFS7<11>   ADC Oversample Filter 3
    volatile unsigned int CLC1PIF:1;     //          IFS7<12>   CLC 1 Positive Edge
    volatile unsigned int CLC2PIF:1;     //          IFS7<13>   CLC 2 Positive Edge
    volatile unsigned int SPI1IF:1;      //          IFS7<14>   SPI 1 Error
    volatile unsigned int SPI2IF:1;      //          IFS7<15>   SPI 2 Error
    volatile unsigned int :1;            // IFS8
    volatile unsigned int MSISIF:1;      //          IFS8<1>    MSI Slave Initiated Interrupt
    volatile unsigned int MSIAIF:1;      //          IFS8<2>    MSI Protocol A
    volatile unsigned int MSIBIF:1;      //          IFS8<3>    MSI Protocol B
    volatile unsigned int MSICIF:1;      //          IFS8<4>    MSI Protocol C
    volatile unsigned int MSIDIF:1;      //          IFS8<5>    MSI Protocol D
    volatile unsigned int MSIEIF:1;      //          IFS8<6>    MSI Protocol E
    volatile unsigned int MSIFIF:1;      //          IFS8<7>    MSI Protocol F
    volatile unsigned int MSIGIF:1;      //          IFS8<8>    MSI Protocol G
    volatile unsigned int MSIHIF:1;      //          IFS8<9>    MSI Protocol H
    volatile unsigned int MSIDTIF:1;     //          IFS8<10>   MSI Master Read FIFO Data Ready
    volatile unsigned int MSIWFEIF:1;    //          IFS8<11>   MSI Master Write FIFO Empty
    volatile unsigned int MSIFLTIF:1;    //          IFS8<12>   MSI Read or Write FIFO Fault (Over/Underflow)
    volatile unsigned int MSISRSTIF:1;   //          IFS8<13>   MSI Slave 1 Reset
    volatile unsigned int :2;
    volatile unsigned int :2;            // IFS9
    volatile unsigned int S1BRKIF:1;     //          IFS9<2>    MSI Slave Debug Break
    volatile unsigned int :2;
    volatile unsigned int CCP7IF:1;      //          IFS9<5>    Capture/Compare/PWM 7
    volatile unsigned int CCT7IF:1;      //          IFS9<6>    Capture/Compare/PWM 7 second timer
    volatile unsigned int :1;
    volatile unsigned int CCP8IF:1;      //          IFS9<8>    Capture/Compare/PWM 8
    volatile unsigned int CCT8IF:1;      //          IFS9<9>    Capture/Compare/PWM 8 second timer
    volatile unsigned int :3;
    volatile unsigned int S1CLKFIF:1;    //          IFS9<13>    MSI Slave Clock Failure
    volatile unsigned int :2;
    volatile unsigned int ADC0EIF:1;     // IFS10    IFS10<0>    Reserved?
    volatile unsigned int ADC1EIF:1;     //          IFS10<1>    Reserved?
    volatile unsigned int :6;
    volatile unsigned int ADFIFOIF:1;    //          IFS10<8>    ADC FIFO Ready
    volatile unsigned int PEVTAIF:1;     //          IFS10<9>    High Resolution PWM Event A
    volatile unsigned int PEVTBIF:1;     //          IFS10<10>   High Resolution PWM Event B
    volatile unsigned int PEVTCIF:1;     //          IFS10<11>   High Resolution PWM Event C
    volatile unsigned int PEVTDIF:1;     //          IFS10<12>   High Resolution PWM Event D
    volatile unsigned int PEVTEIF:1;     //          IFS10<13>   High Resolution PWM Event E
    volatile unsigned int PEVTFIF:1;     //          IFS10<14>   High Resolution PWM Event F
    volatile unsigned int CLC3PIF:1;     //          IFS10<15>   CLC 3 Positive Edge
    volatile unsigned int CLC4PIF:1;     // IFS11    IFS11<0>    CLC 4 Positive Edge
    volatile unsigned int CLC1NIF:1;     //          IFS11<1>    CLC 1 Negative Edge
    volatile unsigned int CLC2NIF:1;     //          IFS11<2>    CLC 2 Negative Edge
    volatile unsigned int CLC3NIF:1;     //          IFS11<3>    CLC 3 Negative Edge
    volatile unsigned int CLC4NIF:1;     //          IFS11<4>    CLC 4 Negative Edge
    volatile unsigned int :8;
    volatile unsigned int U1EVTIF:1;     //          IFS11<13>   UART 1 Event
    volatile unsigned int U2EVTIF:1;     //          IFS11<14>   UART 2 Event
    volatile unsigned int :1;
} INTFLAGFIELDS;

typedef struct
{
    unsigned int INT0IP:4;      // IPC0
    unsigned int T1IP:4;
    unsigned int CNAIP:4;
    unsigned int CNBIP:4;
    unsigned int DMA0IP:4;      // IPC1
    unsigned int :4;
    unsigned int CCP1IP:4;
    unsigned int CCT1IP:4;
    unsigned int DMA1IP:4;      // IPC2
    unsigned int SPI1RXIP:4;
    unsigned int SPI1TXIP:4;
    unsigned int U1RXIP:4;
    unsigned int U1TXIP:4;      // IPC3
    unsigned int ECCSBEIP:4;
    unsigned int NVMIP:4;
    unsigned int INT1IP:4;
    unsigned int SI2C1IP:4;     // IPC4
    unsigned int MI2C1IP:4;
    unsigned int DMA2IP:4;
    unsigned int CNCIP:4;
    unsigned int INT2IP:4;      // IPC5
    unsigned int DMA3IP:4;
    unsigned int DMA4IP:4;
    unsigned int CCP2IP:4;
    unsigned int CCT2IP:4;      // IPC6
    unsigned int C1IP:4;
    unsigned int INT3IP:4;
    unsigned int U2RXIP:4;
    unsigned int U2TXIP:4;      // IPC7
    unsigned int SPI2RXIP:4;
    unsigned int SPI2TXIP:4;
    unsigned int C1RXIP:4;
    unsigned int :8;            // IPC8
    unsigned int DMA5IP:4;
    unsigned int CCP3IP:4;
    unsigned int CCT3IP:4;      // IPC9
    unsigned int SI2C2IP:4;
    unsigned int MI2C2IP:4;
    unsigned int :4;
    unsigned int CCP4IP:4;      // IPC10
    unsigned int CCT4IP:4;
    unsigned int :4;
    unsigned int CCP5IP:4;
    unsigned int CCT5IP:4;      // IPC11
    unsigned int DMTIP:4;
    unsigned int CCP6IP:4;
    unsigned int CCT6IP:4;
    unsigned int QEI1IP:4;      // IPC12
    unsigned int U1EIP:4;
    unsigned int U2EIP:4;
    unsigned int CRCIP:4;
    unsigned int C1TXIP:4;      // IPC13
    unsigned int :12;
    unsigned int :16;           // IPC14
    unsigned int :4;            // IPC15
    unsigned int ICDIP:4;
    unsigned int JTAGIP:4;
    unsigned int PTGSTEPIP:4;
    unsigned int I2C1BCIP:4;    // IPC16
    unsigned int I2C2BCIP:4;
    unsigned int :4;
    unsigned int PWM1IP:4;
    unsigned int PWM2IP:4;      // IPC17
    unsigned int PWM3IP:4;
    unsigned int PWM4IP:4;
    unsigned int :4;
    unsigned int :12;           // IPC18
    unsigned int CNDIP:4;
    unsigned int CNEIP:4;       // IPC19
    unsigned int CMP1IP:4;
    unsigned int :8;
    unsigned int :4;            // IPC20
    unsigned int PTGWDTIP:4;
    unsigned int PTG0IP:4;
    unsigned int PTG1IP:4;
    unsigned int PTG2IP:4;      // IPC21
    unsigned int PTG3IP:4;
    unsigned int SENT1IP:4;
    unsigned int SENT1EIP:4;
    unsigned int SENT2IP:4;     // IPC22
    unsigned int SENT2EIP:4;
    unsigned int ADCIP:4;
    unsigned int ADCAN0IP:4;
    unsigned int ADCAN1IP:4;    // IPC23
    unsigned int ADCAN2IP:4;
    unsigned int ADCAN3IP:4;
    unsigned int ADCAN4IP:4;
    unsigned int ADCAN5IP:4;    // IPC24
    unsigned int ADCAN6IP:4;
    unsigned int ADCAN7IP:4;
    unsigned int ADCAN8IP:4;
    unsigned int ADCAN9IP:4;    // IPC25
    unsigned int ADCAN10IP:4;
    unsigned int ADCAN11IP:4;
    unsigned int ADCAN12IP:4;
    unsigned int ADCAN13IP:4;   // IPC26
    unsigned int ADCAN14IP:4;
    unsigned int ADCAN15IP:4;
    unsigned int ADCAN16IP:4;
    unsigned int ADCAN17IP:4;   // IPC27
    unsigned int ADCAN18IP:4;
    unsigned int ADCAN19IP:4;
    unsigned int ADCAN20IP:4;
    unsigned int :12;           // IPC28
    unsigned int ADFLTIP:4;
    unsigned int ADCMP0IP:4;    // IPC29
    unsigned int ADCMP1IP:4;
    unsigned int ADCMP2IP:4;
    unsigned int ADCMP3IP:4;
    unsigned int ADFLTR0IP:4;   // IPC30
    unsigned int ADFLTR1IP:4;
    unsigned int ADFLTR2IP:4;
    unsigned int ADFLTR3IP:4;
    unsigned int CLC1PIP:4;     // IPC31
    unsigned int CLC2PIP:4;
    unsigned int SPI1IP:4;
    unsigned int SPI2IP:4;
    unsigned int :4;            // IPC32
    unsigned int MSISIP:4;
    unsigned int MSIAIP:4;
    unsigned int MSIBIP:4;
    unsigned int MSICIP:4;      // IPC33
    unsigned int MSIDIP:4;
    unsigned int MSIEIP:4;
    unsigned int MSIFIP:4;
    unsigned int MSIGIP:4;      // IPC34
    unsigned int MSIHIP:4;
    unsigned int MSIDTIP:4;
    unsigned int MSIWFEIP:4;
    unsigned int MSIFLTIP:4;    // IPC35
    unsigned int MSISRSTIP:4;
    unsigned int :8;
    unsigned int :8;            // IPC36
    unsigned int S1BRKIP:4;
    unsigned int :4;
    unsigned int :4;            // IPC37
    unsigned int CCP7IP:4;
    unsigned int CCT7IP:4;
    unsigned int :4;
    unsigned int CCP8IP:4;      // IPC38
    unsigned int CCT8IP:4;
    unsigned int :8;
    unsigned int :4;            // IPC39
    unsigned int S1CLKFIP:4;
    unsigned int :8;
    unsigned int :16;           // IPC40
    unsigned int :16;           // IPC41
    unsigned int ADFIFOIP:4;    // IPC42
    unsigned int PEVTAIP:4;
    unsigned int PEVTBIP:4;
    unsigned int PEVTCIP:4;
    unsigned int PEVTDIP:4;     // IPC43
    unsigned int PEVTEIP:4;
    unsigned int PEVTFIP:4;
    unsigned int CLC3PIP:4;
    unsigned int CLC4PIP:4;     // IPC44
    unsigned int CLC1NIP:4;
    unsigned int CLC2NIP:4;
    unsigned int CLC3NIP:4;
    unsigned int CLC4NIP:4;     // IPC45
    unsigned int :12;
    unsigned int :16;           // IPC46
    unsigned int :4;            // IPC47
    unsigned int U1EVTIP:4;
    unsigned int U2EVTIP:4;
    unsigned int :4;
} INTPRIFIELDS;

#elif defined(__XC16__) // Starting from: #if defined(EZBL_LIB16CH) || defined(__dsPIC33CH__)

typedef struct
{
    volatile unsigned int :5;           // PMD1
    volatile unsigned int U1MD:1;
    volatile unsigned int U2MD:1;
    volatile unsigned int I2C1MD:1;
    volatile unsigned int :3;
    volatile unsigned int T1MD:1;
    volatile unsigned int T2MD:1;
    volatile unsigned int T3MD:1;
    volatile unsigned int T4MD:1;
    volatile unsigned int T5MD:1;
    volatile unsigned int :16;          // PMD2
    volatile unsigned int :1;           // PMD3
    volatile unsigned int I2C2MD:1;
    volatile unsigned int :1;
    volatile unsigned int U3MD:1;
    volatile unsigned int :8;
    volatile unsigned int T6MD:1;
    volatile unsigned int :3;
    volatile unsigned int :5;           // PMD4
    volatile unsigned int U4MD:1;
    volatile unsigned int :10;
    volatile unsigned int CCP1MD:1;     // PMD5     CCPs on PIC24FJ1024GA610/GB610, PIC24FJ256GA412/GB412 and PIC24FJ256GA705 families (not PIC24F[V]xxK type CCPs which have PMD bits starting in PMD2<0>)
    volatile unsigned int CCP2MD:1;
    volatile unsigned int CCP3MD:1;
    volatile unsigned int CCP4MD:1;
    volatile unsigned int CCP5MD:1;
    volatile unsigned int CCP6MD:1;
    volatile unsigned int CCP7MD:1;
    volatile unsigned int :9;
} PMDFIELDS;

typedef struct
{
    volatile unsigned int :3;           // IEC0
    volatile unsigned int T1IE:1;       //      IEC0<3>
    volatile unsigned int :3;
    volatile unsigned int T2IE:1;       //      IEC0<7>
    volatile unsigned int T3IE:1;       //      IEC0<8>
    volatile unsigned int :2;
    volatile unsigned int U1RXIE:1;     //      IEC0<11>
    volatile unsigned int U1TXIE:1;     //      IEC0<12>
    volatile unsigned int :3;
    volatile unsigned int SI2C1IE:1;    // IEC1 IEC1<0>
    volatile unsigned int MI2C1IE:1;    //      IEC1<1>
    volatile unsigned int :9;
    volatile unsigned int T4IE:1;       //      IEC1<11>
    volatile unsigned int T5IE:1;       //      IEC1<12>
    volatile unsigned int :1;
    volatile unsigned int U2RXIE:1;     //      IEC1<14>
    volatile unsigned int U2TXIE:1;     //      IEC1<15>
    union                           // IEC2
    {
        struct
        {
            volatile unsigned int :15;
            volatile unsigned int T6IE:1;//     IEC2<15>
        };
        struct
        {
            volatile unsigned int :11;
            volatile unsigned int CCT3IE:1;//   IEC2<11>
            volatile unsigned int CCT4IE:1;//   IEC2<12>
            volatile unsigned int :2;
            volatile unsigned int CCT5IE:1;//   IEC2<15>    CCT5IE is located here on PIC24FJ1024GA610/GB610, PIC24FJ256GA412/GB412 and PIC24FJ256GA705 families. This overlaps T6IE present on other devices.
        };
    };
    volatile unsigned int CCT6IE:1;     // IEC3 IEC3<0>
    volatile unsigned int SI2C2IE:1;    //      IEC3<1>
    volatile unsigned int MI2C2IE:1;    //      IEC3<2>
    volatile unsigned int CCT7IE:1;     //      IEC3<3>
    volatile unsigned int :12;
    volatile unsigned int :16;          // IEC4
    volatile unsigned int :2;           // IEC5
    volatile unsigned int U3RXIE:1;     //      IEC5<2>
    volatile unsigned int U3TXIE:1;     //      IEC5<3>
    volatile unsigned int :4;
    volatile unsigned int U4RXIE:1;     //      IEC5<7>
    volatile unsigned int U4TXIE:1;     //      IEC5<8>
    volatile unsigned int :6;
    volatile unsigned int :5;           // IEC6
    volatile unsigned int CCT1IE:1;     //      IEC6<5>     CCP on PIC24FJ1024GA610/GB610, PIC24FJ256GA412/GB412 and PIC24FJ256GA705 families (not PIC24F[V]xxK type CCPs which have CCT1IF bits starting in IEC0<7>)
    volatile unsigned int CCT2IE:1;     //      IEC6<6>
    volatile unsigned int :9;
} INTENFIELDS;

typedef struct
{
    volatile unsigned int :3;           // IFS0
    volatile unsigned int T1IF:1;       //      IFS0<3>
    volatile unsigned int :3;
    volatile unsigned int T2IF:1;       //      IFS0<7>
    volatile unsigned int T3IF:1;       //      IFS0<8>
    volatile unsigned int :2;
    volatile unsigned int U1RXIF:1;     //      IFS0<11>
    volatile unsigned int U1TXIF:1;     //      IFS0<12>
    volatile unsigned int :3;
    volatile unsigned int SI2C1IF:1;    // IFS1 IFS1<0>
    volatile unsigned int MI2C1IF:1;    //      IFS1<1>
    volatile unsigned int :9;
    volatile unsigned int T4IF:1;       //      IFS1<11>
    volatile unsigned int T5IF:1;       //      IFS1<12>
    volatile unsigned int :1;
    volatile unsigned int U2RXIF:1;     //      IFS1<14>
    volatile unsigned int U2TXIF:1;     //      IFS1<15>
    union                           // IFS2
    {
        struct
        {
            volatile unsigned int :15;
            volatile unsigned int T6IF:1;//     IFS2<15>
        };
        struct
        {
            volatile unsigned int :11;
            volatile unsigned int CCT3IF:1;//   IFS2<11>
            volatile unsigned int CCT4IF:1;//   IFS2<12>
            volatile unsigned int :2;
            volatile unsigned int CCT5IF:1;//   IFS2<15>    CCT5IF is located here on PIC24FJ1024GA610/GB610, PIC24FJ256GA412/GB412 and PIC24FJ256GA705 families. This overlaps T6IF present on other devices.
        };
    };
    volatile unsigned int CCT6IF:1;     // IFS3 IFS3<0>
    volatile unsigned int SI2C2IF:1;    //      IFS3<1>
    volatile unsigned int MI2C2IF:1;    //      IFS3<2>
    volatile unsigned int CCT7IF:1;     //      IFS3<3>
    volatile unsigned int :12;
    volatile unsigned int :16;          // IFS4
    volatile unsigned int :2;           // IFS5
    volatile unsigned int U3RXIF:1;     //      IFS5<2>
    volatile unsigned int U3TXIF:1;     //      IFS5<3>
    volatile unsigned int :4;
    volatile unsigned int U4RXIF:1;     //      IFS5<7>
    volatile unsigned int U4TXIF:1;     //      IFS5<8>
    volatile unsigned int :6;
    volatile unsigned int :5;           // IFS6
    volatile unsigned int CCT1IF:1;     //      IFS6<5>     CCP on PIC24FJ1024GA610/GB610, PIC24FJ256GA412/GB412 and PIC24FJ256GA705 families (not PIC24F[V]xxK type CCPs which have CCT1IF bits starting in IFS0<7>)
    volatile unsigned int CCT2IF:1;     //      IFS6<6>
    volatile unsigned int :9;
} INTFLAGFIELDS;

// IPC bits here are implemented using chars here to allow better compiler
// optimization.
typedef struct
{
    unsigned char :4;           // IPC0
    unsigned char :4;
    unsigned char :4;
    unsigned char T1IP:4;       //          IPC0<12>
    unsigned char :4;           // IPC1
    unsigned char :4;
    unsigned char :4;
    unsigned char T2IP:4;       //          IPC1<12>
    unsigned char T3IP:4;       // IPC2     IPC2<0>
    unsigned char :4;
    unsigned char :4;
    unsigned char U1RXIP:4;     //          IPC2<12>
    unsigned char U1TXIP:4;     // IPC3     IPC3<0>
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char SI2C1IP:4;    // IPC4     IPC4<0>
    unsigned char MI2C1IP:4;    //          IPC4<4>
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC5
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC6
    unsigned char :4;
    unsigned char :4;
    unsigned char T4IP:4;       //          IPC6<12>
    unsigned char T5IP:4;       // IPC7     IPC7<0>
    unsigned char :4;
    unsigned char U2RXIP:4;     //          IPC7<8>
    unsigned char U2TXIP:4;     //          IPC7<12>
    unsigned char :4;           // IPC8
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC9
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC10
    unsigned char :4;
    unsigned char :4;
    unsigned char CCT3IP:4;     //          IPC10<12>   CCP on PIC24FJ1024GA610/GB610, PIC24FJ256GA412/GB412 and PIC24FJ256GA705 families (not PIC24F[V]xxK type CCPs which have CCT3IP bits starting in IPC6<12>)
    union                       // IPC11
    {
        struct
        {
            unsigned char :4;
            unsigned char :4;
            unsigned char :4;
            unsigned char T6IP:4;//         IPC11<12>
        };
        struct
        {
            unsigned char CCT4IP:4;//       IPC11<0>    CCP on PIC24FJ1024GA610/GB610, PIC24FJ256GA412/GB412 and PIC24FJ256GA705 families (not PIC24F[V]xxK type CCPs which have CCT4IP bits starting in IPC7<0>)
            unsigned char :4;
            unsigned char :4;
            unsigned char CCT5IP:4;//       IPC11<12>   CCP on PIC24FJ1024GA610/GB610, PIC24FJ256GA412/GB412 and PIC24FJ256GA705 families (not PIC24F[V]xxK type CCPs which have CCT5IP bits starting in IPC10<4>)
        };
    };
    unsigned char CCT6IP:4;     // IPC12    IPC12<0>
    unsigned char SI2C2IP:4;    //          IPC12<4>
    unsigned char MI2C2IP:4;    //          IPC12<8>
    unsigned char CCT7IP:4;     //          IPC12<12>
    unsigned char :4;           // IPC13
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC14
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC15
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC16
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC17
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC18
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC19
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC20
    unsigned char :4;
    unsigned char U3RXIP:4;     //          IPC20<8>
    unsigned char U3TXIP:4;     //          IPC20<12>
    unsigned char :4;           // IPC21
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char U4RXIP:4;     // IPC22    IPC22<0>
    unsigned char U4TXIP:4;     //          IPC22<4>
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC23
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC24
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;
    unsigned char :4;           // IPC25
    unsigned char CCT1IP:4;     //          IPC25<4>
    unsigned char CCT2IP:4;     //          IPC25<8>
    unsigned char :4;
} INTPRIFIELDS;

#endif  // Belongs to: #elif defined(__XC16__), which starts from: #if defined(EZBL_LIB16CH) || defined(__dsPIC33CH__)


#if defined(EZBL_LIB16CH) || defined(__dsPIC33CH__)
    #if   (NOW_TIMER_NUM == 1) && defined(TMR_IS_T)
        #define INT_REG_NUM     0   // IEC0/IFS0
        #define INT_BIT_POS     1   // T1IE/T1IF
    #elif (NOW_TIMER_NUM == 1) && defined(TMR_IS_CCP)
        #define INT_REG_NUM     0   // IEC0/IFS0
        #define INT_BIT_POS     7   // CCT1IE/CCT1IF
    #elif (NOW_TIMER_NUM == 2) && defined(TMR_IS_CCP)
        #define INT_REG_NUM     1   // IEC1/IFS1
        #define INT_BIT_POS     8   // CCT2IE/CCT2IF
    #elif (NOW_TIMER_NUM == 3) && defined(TMR_IS_CCP)
        #define INT_REG_NUM     2   // IEC2/IFS2
        #define INT_BIT_POS     4   // CCT3IE/CCT3IF
    #elif (NOW_TIMER_NUM == 4) && defined(TMR_IS_CCP)
        #define INT_REG_NUM     2   // IEC2/IFS2
        #define INT_BIT_POS     9   // CCT4IE/CCT4IF
    #elif (NOW_TIMER_NUM == 5) && defined(TMR_IS_CCP)
        #define INT_REG_NUM     2   // IEC2/IFS2
        #define INT_BIT_POS     12  // CCT5IE/CCT5IF
    #elif (NOW_TIMER_NUM == 6) && defined(TMR_IS_CCP)
        #define INT_REG_NUM     2   // IEC2/IFS2
        #define INT_BIT_POS     15  // CCT6IE/CCT6IF
    #elif (NOW_TIMER_NUM == 7) && defined(TMR_IS_CCP)
        #define INT_REG_NUM     9   // IEC9/IFS9
        #define INT_BIT_POS     6   // CCT7IE/CCT7IF
    #elif (NOW_TIMER_NUM == 8) && defined(TMR_IS_CCP)
        #define INT_REG_NUM     9   // IEC9/IFS9
        #define INT_BIT_POS     9   // CCT8IE/CCT8IF
    #else
        #error Support for timer combination TMR_IS_CCP/TMR_IS_T + NOW_TIMER_NUM not implemented
    #endif
#elif defined(__XC16__)
    #if   (NOW_TIMER_NUM == 1) && defined(TMR_IS_T)
        #define INT_REG_NUM     0   // IEC0/IFS0
        #define INT_BIT_POS     3   // T1IE/T1IF
    #elif (NOW_TIMER_NUM == 2) && defined(TMR_IS_T)         // dsPIC30F devices not supported (at IFS0, #6 instead)
        #define INT_REG_NUM     0   // IEC0/IFS0
        #define INT_BIT_POS     7   // T2IE/T2IF
    #elif (NOW_TIMER_NUM == 3) && defined(TMR_IS_T)         // dsPIC30F devices not supported (at IFS0, #7 instead)
        #define INT_REG_NUM     0   // IEC0/IFS0
        #define INT_BIT_POS     8   // T3IE/T3IF
    #elif (NOW_TIMER_NUM == 4) && defined(TMR_IS_T)         // dsPIC30F devices not supported (at IFS1, #5 instead)
        #define INT_REG_NUM     1   // IEC1/IFS1
        #define INT_BIT_POS     11  // T4IE/T4IF
    #elif (NOW_TIMER_NUM == 5) && defined(TMR_IS_T)         // dsPIC30F devices not supported (at IFS1, #6 instead)
        #define INT_REG_NUM     1   // IEC1/IFS1
        #define INT_BIT_POS     12  // T5IE/T5IF
    #elif (NOW_TIMER_NUM == 6) && defined(TMR_IS_T)
        #define INT_REG_NUM     2   // IEC2/IFS2
        #define INT_BIT_POS     15  // T6IE/T6IF
    #elif (NOW_TIMER_NUM == 1) && defined(TMR_IS_CCP)       // PIC24F[V]xxK "K-flash" devices not supported (at IFS0, #7 instead)
        #define INT_REG_NUM     6   // IEC6/IFS6
        #define INT_BIT_POS     5   // CCT1IE/CCT1IF
    #elif (NOW_TIMER_NUM == 2) && defined(TMR_IS_CCP)       // PIC24F[V]xxK "K-flash" devices not supported (at IFS0, #8 instead)
        #define INT_REG_NUM     6   // IEC6/IFS6
        #define INT_BIT_POS     6   // CCT2IE/CCT2IF
    #elif (NOW_TIMER_NUM == 3) && defined(TMR_IS_CCP)       // PIC24F[V]xxK "K-flash" devices not supported (at IFS1, #11 instead)
        #define INT_REG_NUM     2   // IEC2/IFS2
        #define INT_BIT_POS     11  // CCT3IE/CCT3IF
    #elif (NOW_TIMER_NUM == 4) && defined(TMR_IS_CCP)       // PIC24F[V]xxK "K-flash" devices not supported (at IFS1, #12 instead)
        #define INT_REG_NUM     2   // IEC2/IFS2
        #define INT_BIT_POS     12  // CCT4IE/CCT4IF
    #elif (NOW_TIMER_NUM == 5) && defined(TMR_IS_CCP)       // PIC24F[V]xxK "K-flash" devices not supported (at IFS2, #9 instead)
        #define INT_REG_NUM     2   // IEC2/IFS2
        #define INT_BIT_POS     15  // CCT5IE/CCT5IF
    #elif (NOW_TIMER_NUM == 6) && defined(TMR_IS_CCP)
        #define INT_REG_NUM     3   // IEC3/IFS3
        #define INT_BIT_POS     0   // CCT6IE/CCT6IF
    #elif (NOW_TIMER_NUM == 7) && defined(TMR_IS_CCP)
        #define INT_REG_NUM     3   // IEC3/IFS3
        #define INT_BIT_POS     3   // CCT7IE/CCT7IF
    #else
        #error Support for timer combination TMR_IS_CCP/TMR_IS_T + NOW_TIMER_NUM not implemented
    #endif
#endif


#if defined(TMR_IS_T)
typedef struct
{
    unsigned :1;
    unsigned TCS:1;
    unsigned TSYNC:1;
    unsigned :1;
    unsigned TCKPS:2;
    unsigned :1;
    unsigned TGATE:1;
    unsigned TECS:2;
    unsigned PRWIP:1;
    unsigned TMWIP:1;
    unsigned TMWDIS:1;
    unsigned SIDL:1;
    unsigned :1;
    unsigned ON:1;
} TxCONBITS;
#else
typedef struct
{
    unsigned MOD:4;     // CCPxCONL
    unsigned CCSEL:1;
    unsigned T32:1;
    unsigned TCKPS:2;
    unsigned CLKSEL:3;
    unsigned TSYNC:1;
    unsigned CCPSLP:1;  // r-0 Reserved bit
    unsigned SIDL:1;
    unsigned :1;
    unsigned ON:1;

    unsigned SYNC:5;  // CCPxCONH
    unsigned ALTSYNC:1;
    unsigned ONESHOT:1;
    unsigned TRIGEN:1;
    unsigned OPS:4;
    unsigned :2;
    unsigned RTRGEN:1;
    unsigned OPSSRC:1;
} TxCONBITS;
#endif

extern volatile PMDFIELDS       PMD1bits __attribute__((near));
extern volatile INTENFIELDS     IEC0bits __attribute__((near));
extern volatile INTFLAGFIELDS   IFS0bits __attribute__((near));
extern          INTPRIFIELDS    IPC0bits __attribute__((near));


// Static SFR renaming macros to make the code portable across different 
// peripheral instances using the macros above. These shouldn't need changes.
#if defined(TMR_IS_T)   // Classic Timer type
    #define Tx                      CAT(T,NOW_TIMER_NUM)
    #define TxCON                   CAT(Tx,CON)
    #define TxCONbits               CAT(Tx,CONbits)
    #define TxCONCLR                CAT(Tx,CONCLR)
    #define TxCONSET                CAT(Tx,CONSET)
    #define TxCONINV                CAT(Tx,CONINV)
    #define TMRx                    CAT(TMR,NOW_TIMER_NUM)
    #define PRx                     CAT(PR,NOW_TIMER_NUM)
    #define TxInterrupt             CAT(Tx,Interrupt)
    #define _TxInterrupt            CAT(_,Tx,Interrupt)
    #define _TxIP                   CAT(IPC0bits.Tx,IP)
    #define _TxIF                   CAT(IFS0bits.Tx,IF)
    #define _TxIE                   CAT(IEC0bits.Tx,IE)
    #define _TxMD                   CAT(PMD1bits.Tx,MD)
    #define _TIMER_x_VECTOR         CAT(_TIMER_,NOW_TIMER_NUM,_VECTOR)
    #define NOW_x_ResetConst        CAT(NOW_,TMRx,_ResetConst)
    #define NOW_x_ResetCalc         CAT(NOW_,TMRx,_ResetCalc)
#else                   // CCP Timer type
    #define Tx                      CAT(CCP,NOW_TIMER_NUM)
    #define TxCON                   CAT(Tx,CON1L)
    #define TxCONbits               CAT(Tx,CON1Lbits)
    #define TxCONCLR                CAT(Tx,CON1CLR)
    #define TxCONSET                CAT(Tx,CON1SET)
    #define TxCONINV                CAT(Tx,CON1INV)
    #define TMRx                    CAT(CCP,NOW_TIMER_NUM,TMRH)
    #define PRx                     CAT(CCP,NOW_TIMER_NUM,PRH)
    #define TxInterrupt             CAT(CCT,NOW_TIMER_NUM,Interrupt)
    #define _TxInterrupt            CAT(_CCT,NOW_TIMER_NUM,Interrupt)
    #define _TxIP                   CAT(IPC0bits.CCT,NOW_TIMER_NUM,IP)
    #define _TxIF                   CAT(IFS0bits.CCT,NOW_TIMER_NUM,IF)
    #define _TxIE                   CAT(IEC0bits.CCT,NOW_TIMER_NUM,IE)
    #define _TxMD                   CAT(PMD1bits.Tx,MD)
    #define _TIMER_x_VECTOR         CAT(_CCT_,NOW_TIMER_NUM,_VECTOR)
    #define NOW_x_ResetConst        CAT(NOW_,Tx,_ResetConst)
    #define NOW_x_ResetCalc         CAT(NOW_,Tx,_ResetCalc)
#endif


extern volatile unsigned int    TxCON       __attribute__((near));  // Note: This is actually an unsigned long on 16-bit devices and the CCP timer type. Referencing this value will only read/write the lower 16-bits (CCPxCONL) on 16-bit CPUs - use TxCONbits for upper bits.
extern volatile TxCONBITS       TxCONbits   __attribute__((near));
extern volatile unsigned int    TMRx        __attribute__((near));
extern volatile unsigned int    PRx         __attribute__((near));


EZBL_SetSYM(NOW_IFSReg, (INT_REG_NUM)*2);               // *2 since each IECx/IFSx register is 2-bytes wide
EZBL_SetSYM(NOW_IFSBit, (INT_BIT_POS));                 // Enable bit/Flag bit position (0 to 15)
EZBL_SetSYM(NOW_IFSBitMask, (1<<INT_BIT_POS));          // 16-bit Enable/Flag mask of all 0's except the enable/flag bit
EZBL_SetSYM(NOW_IRQ, INT_REG_NUM*16 + INT_BIT_POS);     // IRQ number for the NOW ISR
EZBL_KeepSYM(_TxInterrupt);                             // Make symbol reference to _T1Interrupt(), _T2Interrupt(), _CCT1Interrupt(), etc. so linker pulls the needed ISR code out of the ezbl_lib.a archive.



/**
 * NOW_timerCallbackHead - Internal pointer to the first element of a linked
 * list for tracking and launching user callback functions created with the
 * NOW_CreateTask() function. This variable should not be changed, and it is
 * attributed persistent so the CRT doesn't initialize it to zero either.
 * Instead, NOW_Init() nullifies it, with NOW_CreateTask(), NOW_EndTask(), and
 * the dispatching routines handling modification of it.
 */
extern void * __attribute__((weak, persistent, near)) NOW_timerCallbackHead;

// Flag to suppress recursion of the NOW_TaskDispatcher() function if excess
// task execution time results in Timer roll over (bit 0).
extern volatile char __attribute__((weak, near, persistent)) NOW_dispatcherActive;


// Structure to store internal bits 16 to 63 returned by NOW_32() and NOW_64()
// calls. 16 byte alignment and the specific order of these variables is
// required as the NOW_countPtr is swapped between the NOW_internalCountPing[]
// and NOW_internalCountPong[] addresses by XORing 0x8 or bit toggling bit 3 of
// the pointer.
struct
{
    volatile unsigned short NOW_internalCountPing[3];
    volatile unsigned short * volatile NOW_countPtr;
    volatile unsigned short NOW_internalCountPong[3];
} NOW_internalCount __attribute__((weak, persistent, aligned(16)));




/**
 * Pointer to the NOW module's hardware timer SFR/counter. This value is set by
 * NOW_Init() and should not be changed, nor should the pointer target be
 * written to outside of the NOW module.
 *
 * This pointer has the persistent attribute (meaning it won't be
 * initialized/zeroed by the CRT on reset), while the volatile near unsigned int
 * applies to the pointer target (the hardware SFR).
 *
 * Used for doing inline/macro NOW_16() calls without the overhead of a function
 * call and return, nor requirement to specify the hardware timer at the
 * pre-processor level.
 */
volatile unsigned int *NOW_timerSFRPtr __attribute__((weak, near, persistent));


/**
 * Reserved for future use in returning cycle accurate
 * NOW_16()/NOW_32()/NOW_64() values while allowing the timer period or count to
 * be adjusted to align the next interrupt to occur on an arbitrary boundary
 * shorter than the normal timer period.
 */
volatile unsigned int NOW_timeOffset __attribute__((weak, persistent));


/**
 * See NOW_x_ResetCalc(). This function is identical, but assumes NOW_sec, 
 * NOW_ms, NOW_us and NOW_Fcy have already been set externally.
 */
void __attribute__((weak)) NOW_x_ResetConst(void)
{
    TxCONBITS newCfg =
    {   // Use Timer/CCP with 1:1 prescale at FCY
        .SIDL  = 0,         // 0 = Keep running the Timer + input clock in Idle mode; 1 = Stop timer in Idle
        .TCKPS = 0x0,       // 0x0 = 1:1 prescalar; 0x1 = 1:8 prescalar; 0x2 = 1:64 prescalar; 0x3 = 1:256 prescalar (1:1, 1:4, 1:16; 1:64 prescalars for CCP)
#if defined(__XC32__) && defined(TMR_IS_T)
        .TSYNC = 1,         // 0 = let timer increment asynchronously (needed if counter and clock source should continue in Idle/Sleeping; best when period is 0xFFFF); 1 = synchronize clock edges into local clock domain for the lowest latency accesses (must use an external clock that is slower than half the peripheral clock bus speed (PBCLK/UPBCLK), unless the clock actually is PBCLK/UPBCLK. Also, this mode will not keep the clock and timer active when in Sleep mode and a regular Timer module is used (versus MCCP/SCCP timer module, which can probably auto-switch to asynchronous and back when Sleeping))
        .TECS = 0x0,        // External Clock source (when TCS = 1): 0x0 = SOSC; 0x1 = TxCK pin; 0x2 = LPRC
#elif (defined(EZBL_LIB16CH) || defined(__dsPIC33CH__)) && defined(TMR_IS_T)
        .TECS = 0x1,        // External Clock source on (when TCS = 1): 0x0 = T1CK pin (may be in PPS); 0x1 = FCY; 0x2 = 2*FCY; 0x3 = 8.0MHz FRC
        .TCS = 1,           // 0 = Use internal peripheral bus clock (PBCLK/UPBCLK); 1 = Use other clock source, as specified by TECS on PIC32/dsPIC33C or T1CK on other PIC24/dsPIC module versions)
        .TSYNC = 0,         // 0 = let timer increment asynchronously (needed if counter and clock source should continue in Idle/Sleeping; best when period is 0xFFFF); 1 = synchronize clock edges into local clock domain for the lowest latency accesses (must use an external clock that is slower than half the peripheral clock bus speed (PBCLK/UPBCLK), unless the clock actually is PBCLK/UPBCLK. Also, this mode will not keep the clock and timer active when in Sleep mode and a regular Timer module is used (versus MCCP/SCCP timer module, which can probably auto-switch to asynchronous and back when Sleeping))
#elif defined(TMR_IS_CCP)// CCP Timer settings on any device
        .ON     = 0,        // Start with CCP in off state so we can write to all CCPxCON bits
        .TSYNC  = 0,        // 0 = Synchronous module time base clock is selected and does not require synchronization (CLKSEL<2:0> = 0x0); 1 = Asynchronous module time base clock is selected and synchronized to the internal system clocks (CLKSEL<2:0> != 0x0)
        .CLKSEL = 0x0,      // 0x0 = FOSC/2(FP); 0x1 = Reference clock (REFO); 0x2 = 2TCY (Fosc-System clock); 0x3 = Master CLC1; 0x4 = Master CLC2; 0x5 = Slave CLC1; 0x6 = Slave CLC2; 0x7 = External TxCK input
        .T32    = 0,        // 0 = Uses 16-bit time base for timer, single edge output compare or input capture function; 1 = Uses 32-bit time base for timer, single edge output compare or input capture function
        .CCSEL  = 0,        // 0 = Output Compare/PWM/Timer peripheral (exact function is selected by the MOD<3:0> bits); 1 = Input Capture peripheral
        .MOD    = 0x0,      // 0x0 = Dual 16-Bit/32-Bit Timer mode, output functions are disabled; 0x1 = 16-Bit/32-Bit Single Edge mode, drives output high on compare match; 0x2 = 16-Bit/32-Bit Single Edge mode, drives output low on compare match; 0x3 = 16-Bit/32-Bit Single Edge mode, toggles output on compare match; 0x4 = Dual Edge Compare mode; 0x5 = Dual Edge Compare mode, buffered; 0xF = External Input mode: Pulse generator is disabled, source is selected by ICS<2:0>
        .SYNC   = 0x0,      // 0x0 = Restart counting on rollover from 0xFFFF to 0x0000
        .ONESHOT= 0,
        .TRIGEN = 0,
        .OPS    = 0x0,      // 0x0 = Interrupt after each time base period match or input capture event; 0x1 = Interrupt every 2nd time base period match or 2nd input capture event, 0x2 = 3rd time base period match, etc.
#elif defined(TMR_IS_T)  // Standard PIC24F/PIC24H/dsPIC33F/dsPIC33E settings with classic Timer
        .TSYNC = 1,        // 0 = let timer increment asynchronously (needed if counter and clock source should continue in Idle/Sleeping; best when period is 0xFFFF); 1 = synchronize clock edges into local clock domain for the lowest latency accesses (must use an external clock that is slower than half the peripheral clock bus speed (PBCLK/UPBCLK), unless the clock actually is PBCLK/UPBCLK. Also, this mode will not keep the clock and timer active when in Sleep mode and a regular Timer module is used (versus MCCP/SCCP timer module, which can probably auto-switch to asynchronous and back when Sleeping))
        .TECS = 0x0,       // External Clock source (when TCS = 1): 0x0 = SOSC; 0x1 = T1CK pin (may be in PPS); 0x2 = LPRC; 0x3 = unimplemented, do not use
        .TCS = 0,          // 0 = Use internal peripheral bus clock (PBCLK/UPBCLK); 1 = Use other clock source, as specified by TECS on PIC32/dsPIC33C or T1CK on other PIC24/dsPIC module versions)
#endif
    };

    //_TxMD = 1;  // Not really necessary

    NOW_timeOffset = 0;
    NOW_internalCount.NOW_countPtr = NOW_internalCount.NOW_internalCountPing;

    // Set a run time pointer to the timer's hardware SFR. This is useful for
    // faster NOW_16() access, which can be inlined as a pointer dereference,
    // and allows possible dynamic adjustments (future features).
    NOW_timerSFRPtr = &TMRx;

    //_TxMD = 0;

    // Set the NOW_CreateTask() linked list head pointer to null, invalidating
    // any existing timers if they exist. This is a weak variable that appears
    // only when NOW_CreateTask() is used, so we can't just blindly write to it.
    if(&NOW_timerCallbackHead)
    {
        NOW_timerCallbackHead = (void*)0;
        NOW_dispatcherActive = 0;   // Required if no CRT initialization done
    }

    TxCONbits = newCfg;
	PRx = 0xFFFF;           // Set period (actual period is 1 greater due to 0x0000 count). This ensures a 32-bit timer still overflows in 65536 cycles for NOW_TASKs to execute with better resolution.

    _TxIP = EZBL_SYM(NOW_TIMER_ISR_PRIORITY);   // Set Interrupt priority = default 4 (medium; 0 is main() context, 7 is maximum); Evaluates to something like: _T1IP = 4; or _CCT1IP = 4;
    _TxIE = 1;              // Turn on the interrupts (more efficient than calling NOW_IntEnable()
	TxCONbits.ON = 1;       // Begin counting
}


/**
 * Initializes a Timer for time keeping with the NOW_* API. Timing is
 * implemented relative to the timer's selected input clock frequency. This
 * function uses this value to sets the values stored in NOW_second,
 * NOW_millisecond and NOW_microsecond for direct use in calling code to
 * translate run-time adjustable NOW counts to physical seconds and other human
 * meaningful times.
 *
 * This function defaults to enabling the Timer/CCP Interrupt at priority
 * level 4. You can modify this value. The interrupt will fire every 65536
 * instructions and need around ~15 or 30 cycles per interrupt event on 16-bit
 * processors. i.e. less than 0.05% CPU use.
 *
 * Most of the NOW APIs should only be called at IPL levels below this ISR
 * priority if accurate and monotonic NOW counts are needed. If you wish to use
 * these APIs in higher priority ISRs, increase the timer ISR priority level
 * to one higher than the highest caller.
 *
 * @param FCYClock
 *          Number of input timer clock cycles per second of real time. For
 *          example, use 70000000 if your device is operating at 70 MIPS and you
 *          are using the system instruction clock to clock the timer. LPRC's
 *          will need ~31000 for 31kHz, and of course 32.768kHz SOSC crystals
 *          are also normally used with 32768.
 *
 *          However, if you know your crystal or clock input is biased above or
 *          below the optimal crystal spec, you can deliberately provide a value
 *          slightly faster or slower to automatically correct for this
 *          oscillation frequency error in software computations. At room
 *          temperature, crystals will oscillate slower than intended when you
 *          have oversized crystal loading capacitors (ex: more parasitic pin
 *          capacitance than you calculated for), and similarly, run fast when
 *          there is below optimal capacitance.
 */
void __attribute__((weak)) NOW_x_ResetCalc(unsigned long FCYClock)
{
    NOW_Fcy = FCYClock;
    NOW_SetTimeIntervals(FCYClock);     // Set NOW_sec, NOW_ms, and NOW_us values
    NOW_x_ResetConst();
}
