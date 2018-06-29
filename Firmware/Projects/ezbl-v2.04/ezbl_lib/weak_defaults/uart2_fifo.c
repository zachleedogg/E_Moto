/*
 * File:   uart2_fifo.c
 * Author: Howard Schlunder
 *
 * Created on 2017 March 09
 *
 * An easily ported interrupt based UART TX and RX buffering implementation.
 * Implements independent software FIFOs for TX and RX channels.
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

#include "../ezbl.h"


// Configuration parameters
#define UART_MODULE_NUM         2       // Hardware UART module # to apply this     // NOTE: Only supports 1, 2, 3 or 4
                                        // software FIFO to. 1 means UART1, 2
                                        // means UART2, etc.
#define UART_TX_FIFO_SIZE       32u     // Bytes for software TX FIFO buffering
#define UART_RX_FIFO_SIZE       96u     // Bytes for software RX FIFO buffering

EZBL_SetWeakSYM(UART2_TX_ISR_PRIORITY, 1);  // CPU IPL (Interrupt Priority Level) to 
EZBL_SetWeakSYM(UART2_RX_ISR_PRIORITY, 2);  // assign to the TX and RX ISRs. 0 means 
                                            // main() context, which is the lowest
                                            // priority and 7 is the highest,
                                            // time-critical, priority.



// Exact match RX character sequence to trigger assignment of EZBL_COM_RX to
// this UART. Comment this line out to disable auto-bootloader wake up on this UART.
#define BOOTLOADER_WAKE_KEY     {0x434D, 0x5055, 0x4348, 0x454D} //{'M','C','U','P','H','C','M','E'}

// NOTE: UxTX and UxRX I/O pin PPS settings need to be configured outside of
// this module if the selected UART does not have fixed device I/O pins.


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
    #define RX_IRQ  ((UART_MODULE_NUM == 1) ? (16u*0u + 11u) /* IFS0<11> */ :   \
                     (UART_MODULE_NUM == 2) ? (16u*1u + 11u) /* IFS1<11> */ :   \
                     0u)
    #define TX_IRQ  (RX_IRQ+1u)
#elif defined(__XC16__)
    #define RX_IRQ  ((UART_MODULE_NUM == 1) ? (16u*0u + 11u) /* IFS0<11> */ :   \
                     (UART_MODULE_NUM == 2) ? (16u*1u + 14u) /* IFS1<14> */ :   \
                     (UART_MODULE_NUM == 3) ? (16u*5u +  2u) /* IFS5<2>  */ :   \
                     (UART_MODULE_NUM == 4) ? (16u*5u +  7u) /* IFS5<7>  */ :   \
                     0u)
    #define TX_IRQ  (RX_IRQ+1u)
#endif



// Static token concatenation macros that make the code easier to read.
// Don't directly change these unless porting to different hardware with 
// different register names. Instead change the UART_MODULE_NUM macro above to
// change which physical UART hardware module this FIFOing library applies to.
#define UxMODE          CAT(U,UART_MODULE_NUM,MODE)
#define UxSTA           CAT(U,UART_MODULE_NUM,STA)
#define UxTXREG         CAT(U,UART_MODULE_NUM,TXREG)
#define UxRXREG         CAT(U,UART_MODULE_NUM,RXREG)
#define UxBRG           CAT(U,UART_MODULE_NUM,BRG)
#define _UxMD           CAT(PMD1bits.U,UART_MODULE_NUM,MD)
#define _UxTXIP         CAT(IPC0bits.U,UART_MODULE_NUM,TXIP)
#define _UxTXIF         CAT(IFS0bits.U,UART_MODULE_NUM,TXIF)
#define _UxTXIE         CAT(IEC0bits.U,UART_MODULE_NUM,TXIE)
#define IPLtxSOFT       CAT(IPL,UART_TX_ISR_PRIORITY,SOFT)
#define _UxRXIP         CAT(IPC0bits.U,UART_MODULE_NUM,RXIP)
#define _UxRXIF         CAT(IFS0bits.U,UART_MODULE_NUM,RXIF)
#define _UxRXIE         CAT(IEC0bits.U,UART_MODULE_NUM,RXIE)
#define IPLrxSOFT       CAT(IPL,UART_RX_ISR_PRIORITY,SOFT)


#if defined(EZBL_LIB16CH) || defined(__dsPIC33CH__)

#define _RXDA  !Uxbits.URXBE

typedef struct
{
    // UxMODE
    unsigned MOD:4;
    unsigned URXEN:1;
    unsigned UTXEN:1;
    unsigned ABAUD:1;
    unsigned BRGH:1;
    unsigned UTXBRK:1;
    unsigned BRKOVR:1;
    unsigned :1;
    unsigned RXBIMD:1;
    unsigned WAKE:1;
    unsigned USIDL:1;
    unsigned :1;
    unsigned UARTEN:1;

    // UxMODEH
    unsigned FLO:2;
    unsigned UTXINV:1;
    unsigned C0EN:1;
    unsigned STSEL:2;
    unsigned URXINV:1;
    unsigned RUNOVF:1;
    unsigned HALFDPLX:1;
    unsigned BCLKSEL:2;
    unsigned BCLKMOD:1;
    unsigned :2;
    volatile unsigned ACTIVE:1;
    unsigned SLPEN:1;

    // UxSTA
    volatile unsigned TXCIF:1;
    volatile unsigned OERR:1;
    volatile unsigned RXBKIF:1;
    volatile unsigned FERR:1;
    volatile unsigned CERIF:1;
    volatile unsigned ABDOVF:1;
    volatile unsigned PERR:1;
    volatile unsigned TRMT:1;
    unsigned TXCIE:1;
    unsigned OERIE:1;
    unsigned RXBKIE:1;
    unsigned FERIE:1;
    unsigned CERIE:1;
    volatile unsigned ABDOVE:1;
    unsigned PERIE:1;
    unsigned TXMTIE:1;

    // UxSTAH
    volatile unsigned URXBF:1;
    volatile unsigned URXBE:1;
    unsigned XON:1;
    volatile unsigned RIDLE:1;
    volatile unsigned UTXBF:1;
    volatile unsigned UTXBE:1;
    volatile unsigned STPMD:1;
    volatile unsigned TXWRE:1;
    unsigned URXISEL:3;
    unsigned :1;
    unsigned UTXISEL:3;
} EZBL_UxBITS;

#define _UxSTA_OERR_MASK        0x00000002
#define _UxSTA_FERR_MASK        0x00000008
#define _UxSTA_PERR_MASK        0x00000040

extern volatile unsigned long   UxMODE      __attribute__((near));
static volatile EZBL_UxBITS     Uxbits      __attribute__((near, weakref(STRINGIFY(UxMODE))));
extern volatile unsigned long   UxSTA       __attribute__((near));
extern volatile unsigned int    UxTXREG     __attribute__((near));
extern volatile unsigned int    UxRXREG     __attribute__((near));
extern          unsigned long   UxBRG       __attribute__((near));
extern volatile INTFLAGFIELDS   IFS0bits    __attribute__((near));
extern volatile INTENFIELDS     IEC0bits    __attribute__((near));
extern          INTPRIFIELDS    IPC0bits    __attribute__((near));
extern volatile PMDFIELDS       PMD1bits    __attribute__((near));


#else   // else to #if defined(EZBL_LIB16CH) || defined(__dsPIC33CH__)

#define _RXDA  Uxbits.RXDA

typedef struct
{
    // UxMODE
    unsigned STSEL:1;   // STSEL: Stop Bit Selection bit <br>1 = Two Stop bits <br>0 = One Stop bit
    unsigned PDSEL:2;   // PDSEL<1:0>: Parity and Data Selection bits <br>0x3 = 9-bit data, no parity <br>0x2 = 8-bit data, odd parity <br>0x1 = 8-bit data, even parity <br>0x0 = 8-bit data, no parity
    unsigned BRGH:1;    // BRGH: High Baud Rate Enable bit <br>1 = BRG generates 4 clocks per bit period (4x baud clock, High-Speed mode) <br>0 = BRG generates 16 clocks per bit period (16x baud clock, Standard mode)
    unsigned URXINV:1;  // URXINV: Receive Polarity Inversion bit (AKA 'RXINV') <br>1 = UxRX Idle state is ‘0’ <br>0 = UxRX Idle state is ‘1’
    unsigned ABAUD:1;   // ABAUD: Auto-Baud Enable bit <br>1 = Enables baud rate measurement on the next character – requires reception of a Sync field (55h) before other data; cleared in hardware upon completion <br>0 = Baud rate measurement is disabled or completed
    unsigned LPBACK:1;  // LPBACK: UARTx Loopback Mode Select bit <br>1 = Enables Loopback mode <br>0 = Loopback mode is disabled
    unsigned WAKE:1;    // WAKE: Wake-up on Start bit Detect During Sleep Mode Enable bit <br>1 = UARTx continues to sample the UxRX pin, interrupt is generated on the falling edge; bit is cleared in hardware on the following rising edge <br>0 = No wake-up is enabled
    unsigned UEN:2;     // UEN<1:0>: UARTx Enable bits <br>0x3 = UxTX, UxRX and BCLKx pins are enabled and used; UxCTS pin is controlled by PORT latches <br>0x2 = UxTX, UxRX, UxCTS and UxRTS pins are enabled and used <br>0x1 = UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin is controlled by PORT latches <br>0x0 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLKx pins are controlled by PORT latches
    unsigned :1;        // Unimplemented: Read as ‘0’
    unsigned RTSMD:1;   // RTSMD: Mode Selection for UxRTS Pin bit <br>1 = UxRTS pin is in Simplex mode <br>0 = UxRTS pin is in Flow Control mode
    unsigned IREN:1;    // IREN: IrDA® Encoder and Decoder Enable bit <br>1 = IrDA encoder and decoder are enabled <br>0 = IrDA encoder and decoder are disabled
    unsigned SIDL:1;    // SIDL: Stop in Idle Mode bit <br>1 = Discontinues module operation when device enters Idle mode <br>0 = Continues module operation in Idle mode
    unsigned FREEZE:1;  // Unimplemented: Read as ‘0’, OR in Debug Mode: Freeze peripheral during debug halt
    unsigned UARTEN:1;  // UARTEN: UARTx Enable bit (AKA 'ON') <br>1 = UARTx is enabled; all UARTx pins are controlled by UARTx as defined by UEN<1:0> <br>0 = UARTx is disabled; all UARTx pins are controlled by PORT latches; UARTx power consumption is minimal

#if defined(__XC32__)
             unsigned OVFDIS:1;          // OVFDIS: Run During Overflow Condition Mode bit <br>1 = When an Overflow Error (OERR) condition is detected, the Shift register continues to run to remain synchronized <br>0 = When an Overflow Error (OERR) condition is detected, the Shift register stops accepting new data (Legacy mode)
             unsigned CLKSEL:2;          // CLKSEL<1:0>: UARTx Clock Selection bits <br>0x3 = The UARTx clock is the Reference Clock Output (REFCLKO) <br>0x2 = The UARTx clock is the FRC oscillator clock <br>0x1 = The UARTx clock is the SYSCLK <br>0x0 = The UARTx clock is the PBCLK (1:1 with SYSCLK)
             unsigned :3;
    volatile unsigned ACTIVE:1; // ACTIVE: UARTx Running Status bit <br>1 = UARTx is active (UxMODE register shouldn't be updated) <br>0 = UARTx is not active (UxMODE register can be updated)
             unsigned SLPEN:1;           // SLPEN: Run During Sleep Enable bit <br>1 = UARTx clock runs during Sleep <br>0 = UARTx clock is turned off during Sleep
             unsigned :8;
#endif

    // UxSTA
    volatile unsigned RXDA:1;      // bit 0 RXDA: Receive Buffer Data Available bit (read-only) <br>1 = Receive buffer has data, at least one more character can be read <br>0 = Receive buffer is empty
    volatile unsigned OERR:1;      // bit 1 OERR: Receive Buffer Overrun Error Status bit (read/clear only) <br>1 = Receive buffer has overflowed <br>0 = Receive buffer has not overflowed. Clearing a previously set OERR bit (1 → 0 transition) resets the receiver buffer and the UxRSR to the empty state.
    volatile unsigned FERR:1;      // bit 2 FERR: Framing Error Status bit (read-only) <br>1 = Framing error has been detected for the current character (character at the top of the receive FIFO) <br>0 = Framing error has not been detected
    volatile unsigned PERR:1;      // bit 3 PERR: Parity Error Status bit (read-only) <br>1 = Parity error has been detected for the current character (character at the top of the receive FIFO) <br>0 = Parity error has not been detected
    volatile unsigned RIDLE:1;     // bit 4 RIDLE: Receiver Idle bit (read-only) <br>1 = Receiver is Idle <br>0 = Receiver is active
    volatile unsigned ADDEN:1;     // bit 5 ADDEN: Address Character Detect bit (bit 8 of received data = 1) <br>1 = Address Detect mode enabled. If 9-bit mode is not selected, this does not take effect <br>0 = Address Detect mode disabled
             unsigned URXISEL:2;   // bit 7-6 URXISEL<1:0>: Receive Interrupt Mode Selection bits <br>0x3 = Interrupt is set on UxRSR transfer making the receive buffer full (i.e., has 4 data characters) <br>0x2 = Interrupt is set on UxRSR transfer making the receive buffer 3/4 full (i.e., has 3 data characters) <br>0x0 or 0x1 = Interrupt is set when any character is received and transferred from the UxRSR to the receive buffer. Receive buffer has one or more characters.
    volatile unsigned TRMT:1;      // bit 8 TRMT: Transmit Shift Register Empty bit (read-only) <br>1 = Transmit Shift Register is empty and transmit buffer is empty (the last transmission has completed) <br>0 = Transmit Shift Register is not empty, a transmission is in progress or queued
    volatile unsigned UTXBF:1;     // bit 9 UTXBF: Transmit Buffer Full Status bit (read-only) <br>1 = Transmit buffer is full <br>0 = Transmit buffer is not full, at least one more character can be written
             unsigned UTXEN:1;     // bit 10 UTXEN: Transmit Enable bit <br>1 = Transmit enabled, UxTX pin controlled by UARTx <br>0 = Transmit disabled, any pending transmission is aborted and buffer is reset. UxTX pin controlled by port
    volatile unsigned UTXBRK:1;    // bit 11 UTXBRK: Transmit Break bit <br>1 = Send Sync Break on next transmission – Start bit, followed by twelve ‘0’ bits, followed by Stop bit; cleared by hardware upon completion <br>0 = Sync Break transmission disabled or completed
             unsigned URXEN:1;     // bit 12 Varies by device: Unimplemented: Read as ‘0’, OR, URXEN: UARTx Receiver Enable bit <br>1 = UARTx receiver is enabled, UxRX pin is controlled by UARTx (if ON = 1) <br>0 = UARTx receiver is disabled, UxRX pin is ignored by the UARTx module
#if defined(__XC16__)   // NOTE: compiler check only is probably overly generic. Must update for your device!
    // dsPIC33EP64GS506 data
             unsigned UTXISEL0:1;  // bit 15,13 UTXISEL<1:0>: Transmission Interrupt Mode Selection bits <br>0x3 = Reserved; do not use <br>0x2 = Interrupt when a character is transferred to the Transmit Shift register, and as a result, the transmit buffer becomes empty <br>0x1 = Interrupt when the last character is shifted out of the Transmit Shift register; all transmit operations are completed <br>0x0 = Interrupt when a character is transferred to the Transmit Shift register (this implies there is at least one character open in the transmit buffer)
             unsigned UTXINV:1;    // bit 14 UTXINV: Transmit Polarity Inversion bit <br>If IREN = 0: <br>1 = UxTX Idle state is ‘0’ <br>0 = UxTX Idle state is ‘1’ <br>If IREN = 1: <br>1 = IrDA® encoded UxTX Idle state is ‘1’ <br>0 = IrDA® encoded UxTX Idle state is ‘0’
             unsigned UTXISEL1:1;  // bit 15,13 UTXISEL<1:0>: Transmission Interrupt Mode Selection bits <br>0x3 = Reserved; do not use <br>0x2 = Interrupt when a character is transferred to the Transmit Shift register, and as a result, the transmit buffer becomes empty <br>0x1 = Interrupt when the last character is shifted out of the Transmit Shift register; all transmit operations are completed <br>0x0 = Interrupt when a character is transferred to the Transmit Shift register (this implies there is at least one character open in the transmit buffer)
#else
    // PIC32MM0064GPL036 data
             unsigned UTXINV:1;    // bit 14 UTXINV: Transmit Polarity Inversion bit <br>If IREN = 0: <br>1 = UxTX Idle state is ‘0’ <br>0 = UxTX Idle state is ‘1’ <br>If IREN = 1: <br>1 = IrDA® encoded UxTX Idle state is ‘1’ <br>0 = IrDA® encoded UxTX Idle state is ‘0’
             unsigned UTXISEL0:1;  // bit 15-14 UTXISEL<1:0>: Transmission Interrupt Mode Selection bits <br>0x3 = Reserved; do not use <br>0x2 = Interrupt when a character is transferred to the Transmit Shift register, and as a result, the transmit buffer becomes empty <br>0x1 = Interrupt when the last character is shifted out of the Transmit Shift register; all transmit operations are completed <br>0x0 = Interrupt when a character is transferred to the Transmit Shift register (this implies there is at least one character open in the transmit buffer)
             unsigned UTXISEL1:1;  // bit 15-14 UTXISEL<1:0>: Transmission Interrupt Mode Selection bits <br>0x3 = Reserved; do not use <br>0x2 = Interrupt when a character is transferred to the Transmit Shift register, and as a result, the transmit buffer becomes empty <br>0x1 = Interrupt when the last character is shifted out of the Transmit Shift register; all transmit operations are completed <br>0x0 = Interrupt when a character is transferred to the Transmit Shift register (this implies there is at least one character open in the transmit buffer)
#endif
} EZBL_UxBITS;

#define _UxSTA_OERR_MASK        0x00000002
#define _UxSTA_FERR_MASK        0x00000004
#define _UxSTA_PERR_MASK        0x00000008


extern volatile unsigned int    UxMODE      __attribute__((near));
static volatile EZBL_UxBITS     Uxbits      __attribute__((near, weakref(STRINGIFY(UxMODE))));
extern volatile unsigned int    UxSTA       __attribute__((near));
extern volatile unsigned int    UxTXREG     __attribute__((near));
extern volatile unsigned int    UxRXREG     __attribute__((near));
extern          unsigned int    UxBRG       __attribute__((near));
extern volatile INTFLAGFIELDS   IFS0bits    __attribute__((near));
extern volatile INTENFIELDS     IEC0bits    __attribute__((near));
extern          INTPRIFIELDS    IPC0bits    __attribute__((near));
extern volatile PMDFIELDS       PMD1bits    __attribute__((near));

#endif  // End of #if defined(EZBL_LIB16CH) || defined(__dsPIC33CH__)




// Macros to relate public API names to API names local to this file. This makes
// multiple instances of a common API set more comparable/maintainable between
// uart1_fifo.c/uart2_fifo.c/uart3_fifo.c/etc. files
//      Local name                  // Exported/public variable/function name
#define ResetBRG                    UART2_ResetBRG                  // void __attribute__((weak)) UART2_ResetBRG(int UxBRGVal);
#define TX_FIFO_WaitUntilFlushed    UART2_TX_FIFO_WaitUntilFlushed  // void __attribute__((weak)) UART2_TX_FIFO_WaitUntilFlushed(void);
#define FIFO_EnableInterrupts       UART2_FIFO_EnableInterrupts     // void __attribute__((weak)) UART2_FIFO_EnableInterrupts(void);
#define FIFO_DisableInterrupts      UART2_FIFO_DisableInterrupts    // void __attribute__((weak)) UART2_FIFO_DisableInterrupts(void);
#define FIFO_ArmAutoBaud            UART2_FIFO_ArmAutoBaud          // void __attribute__((weak)) UART2_FIFO_ArmAutoBaud(void);
#define FIFO_QueryAutoBaud          UART2_FIFO_QueryAutoBaud        // unsigned int __attribute__((weak)) UART2_FIFO_QueryAutoBaud(void);
#define TX_FIFO_OnWrite             UART2_TX_FIFO_OnWrite           // unsigned int __attribute__((weak)) UART2_TX_FIFO_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);
#define RX_FIFO_OnRead              UART2_RX_FIFO_OnRead            // unsigned int __attribute__((weak)) UART2_RX_FIFO_OnRead(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *readFIFO);
#define TX_FIFO_Flush               UART2_TX_FIFO_Flush             // unsigned int __attribute__((weak)) UART2_TX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);
#define RX_FIFO_Flush               UART2_RX_FIFO_Flush             // unsigned int __attribute__((weak)) UART2_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);
#define _UxTXInterrupt              _U2TXInterrupt                  // void __attribute__((weak, interrupt, no_auto_psv)) _U2TXInterrupt(void);
#define _UxRXInterrupt              _U2RXInterrupt                  // void __attribute__((weak, interrupt, no_auto_psv)) _U2RXInterrupt(void);
#define TxFifo                      UART2_TxFifo                    // EZBL_FIFO __attribute__((weak, persistent))  UART2_TxFifo;
#define RxFifo                      UART2_RxFifo                    // EZBL_FIFO __attribute__((weak, persistent))  UART2_RxFifo;
#define TxFifoBuffer                UART2_TxFifoBuffer              // unsigned char __attribute__((weak, section(".bss.UART2_TxFifoBuffer"), noload)) UART2_TxFifoBuffer[UART_TX_FIFO_SIZE];
#define RxFifoBuffer                UART2_RxFifoBuffer              // unsigned char __attribute__((weak, section(".bss.UART2_RxFifoBuffer"), noload)) UART2_RxFifoBuffer[UART_RX_FIFO_SIZE];
#define TxFifoBuffer_sectName       ".bss.UART2_TxFifoBuffer"
#define RxFifoBuffer_sectName       ".bss.UART2_RxFifoBuffer"
#define wakeKeyIndex                UART2_wakeKeyIndex              // unsigned int UART2_wakeKeyIndex = 0;
#define TX_ISR_PRIORITY             UART2_TX_ISR_PRIORITY           // EZBL_SetWeakSYM(UART2_TX_ISR_PRIORITY, 1);
#define RX_ISR_PRIORITY             UART2_RX_ISR_PRIORITY           // EZBL_SetWeakSYM(UART2_RX_ISR_PRIORITY, 2);


// Our prototypes
void ResetBRG(int UxBRGVal);
void TX_FIFO_WaitUntilFlushed(void);
void FIFO_EnableInterrupts(void);
void FIFO_DisableInterrupts(void);
void FIFO_ArmAutoBaud(void);
unsigned int FIFO_QueryAutoBaud(void);
unsigned int TX_FIFO_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);
unsigned int RX_FIFO_OnRead(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *readFIFO);
unsigned int TX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);
void __attribute__((interrupt, no_auto_psv)) _UxTXInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _UxRXInterrupt(void);


// Allocate RAM for the TX and RX software FIFOs
EZBL_FIFO __attribute__((weak, persistent))                                 TxFifo;
EZBL_FIFO __attribute__((weak, persistent))                                 RxFifo;
unsigned char __attribute__((weak, noload, section(TxFifoBuffer_sectName))) TxFifoBuffer[UART_TX_FIFO_SIZE]; // 'noload' allows 'persistent' behavior and works on named sections ('persistent' does not)
unsigned char __attribute__((weak, noload, section(RxFifoBuffer_sectName))) RxFifoBuffer[UART_RX_FIFO_SIZE]; // 'noload' allows 'persistent' behavior and works on named sections ('persistent' does not)
#if defined(BOOTLOADER_WAKE_KEY)
unsigned int __attribute__((weak)) wakeKeyIndex = 0;  // Bootloader wake up key decoder position
#endif



// For UART2 treated as the default UART communications interface, export alternate names without peripheral instance numbers.
#if UART_MODULE_NUM == 2
void UART_ResetBRG(int UxBRGVal)                                                __attribute__((alias("UART2_ResetBRG"), weak));
void UART_TX_FIFO_WaitUntilFlushed(void)                                        __attribute__((alias("UART2_TX_FIFO_WaitUntilFlushed"), weak));
void UART_FIFO_EnableInterrupts(void)                                           __attribute__((alias("UART2_FIFO_EnableInterrupts"), weak));
void UART_FIFO_DisableInterrupts(void)                                          __attribute__((alias("UART2_FIFO_DisableInterrupts"), weak));
void UART_FIFO_ArmAutoBaud(void)                                                __attribute__((alias("UART2_FIFO_ArmAutoBaud"), weak));
unsigned int UART_FIFO_QueryAutoBaud(void)                                      __attribute__((alias("UART2_FIFO_QueryAutoBaud"), weak));
unsigned int UART_TX_FIFO_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO)  __attribute__((alias("UART2_TX_FIFO_OnWrite"), weak));
unsigned int UART_RX_FIFO_OnRead(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *readFIFO)    __attribute__((alias("UART2_RX_FIFO_OnRead"), weak));
unsigned int UART_TX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout)                     __attribute__((alias("UART2_TX_FIFO_Flush"), weak));
void _UTXInterrupt(void)                                                        __attribute__((alias("_U2TXInterrupt"), weak, interrupt, no_auto_psv));
void _URXInterrupt(void)                                                        __attribute__((alias("_U2RXInterrupt"), weak, interrupt, no_auto_psv));

extern EZBL_FIFO     UART_TxFifo                                                __attribute__((alias("UART2_TxFifo"), weak, persistent));
extern EZBL_FIFO     UART_RxFifo                                                __attribute__((alias("UART2_RxFifo"), weak, persistent));
extern unsigned char UART_TxFifoBuffer[UART_TX_FIFO_SIZE]                       __attribute__((alias("UART2_TxFifoBuffer"), weak, noload, section(TxFifoBuffer_sectName)));
extern unsigned char UART_RxFifoBuffer[UART_RX_FIFO_SIZE]                       __attribute__((alias("UART2_RxFifoBuffer"), weak, noload, section(RxFifoBuffer_sectName)));
#if defined(BOOTLOADER_WAKE_KEY)
extern unsigned int  UART_wakeKeyIndex                                          __attribute__((alias("UART2_wakeKeyIndex"), weak));
#endif
#endif  // belongs to #if UART_MODULE_NUM == 2


void __attribute__((weak)) ResetBRG(int UxBRGVal)
{
    EZBL_UxBITS newCfg =
    {
        .URXEN   = 1,       // UxMODE: Enable RX (device dependent function)
        .UTXEN   = 1,       // UxMODE: Enable TX
        .ABAUD   = (UxBRGVal <= 0), // UxMODE: Auto-baud enable (enable for negative or zero UxBRGVal parameter input; disable when positive)
        .BRGH    = 1,       // UxMODE: Set BRGH for better baud rate resolution/less error at high baud rates.  Should/must be done before setting UTXEN on PIC32MM?
        .UARTEN  = 0,       // UxMODE: Leave UARTEN off for now. Should/need to finish writing all registers before turning on the UART.  Should/must be set = '1' before setting UTXEN on PIC32MM?
        .URXISEL = 0x0,     // UxSTAH: 0x0 = One or more RX characters ready; 0x2 = UART RX FIFO 3/4 full; 0x3 = RX FIFO completely full
#if defined(__XC32__)
        .CLKSEL  = 0x0,     // UxMODE: 0x0 = PBCLK/SYSCLK; 0x1 = Reserved; 0x2 = 2x PBCLK/SYSCLK; 0x3 = 4x PBCLK/SYSCLK - NOTE: Device dependent, check UART in data sheet
        .OVFDIS  = 1,       // UxMODE: 1 = After RX Overflow Error (OERR), keep RX running; 0 = On OERR, halt RX until the OERR bit is cleared to acknowledge the RX data stream discontinuity
#elif defined(EZBL_LIB16CH) || defined(__dsPIC33CH__)
        .MOD     = 0x0,     // UxMODE: 0x0 = Standard 8-bit UART; 0x1 = 7-bit UART; others, see data sheet
        .BCLKMOD = 0,       // UxMODE: When using 16x clock mode, use fractional baud clocks that insert an extra input clock occasionally to maintain a more accurate baud rate over the span of 2+ bits
        .RUNOVF  = 1,       // UxMODEH: Ignore RX overflow flag (don't require software to clear it to start receiving again)
        .BCLKSEL = 0x2,     // UxMODEH: 0x0 = PB/UPB clock; 0x1 = DC; 0x2 = FOSC (2*FCY); 0x3 = Aux PLL/3
        .UTXISEL = -4,      // UxSTAH: Interrupt when 4 locations are free in hardware TX FIFO (bitfield encoding is 2's complemented, so expressed as negative number)
#else   // Standard PIC24F/PIC24H/dsPIC33F/dsPIC33E settings
        .PDSEL  = 0x0,      // UxMODE: 8-bit data, no parity
        .UTXISEL0 = 0,      // 0x2 = TX FIFO just became empty as we shift out the last byte
        .UTXISEL1 = 1,
#endif
    };

    _UxMD   = 1;            // Disable the module using the Peripheral Module Disable bit. This clears all internal state and SFRs in the peripheral. When un-disabled, they come back with the reset default values.
    _UxTXIE = 0;            // Clear interrupt enable; we are about to clear the corresponding FIFO
    _UxRXIE = 0;            // Clear interrupt enable; we are about to clear the corresponding FIFO
    _UxRXIF = 0;            // Clear Interrupt Flags
    _UxTXIF = 0;            // Clear Interrupt Flags
    _UxMD   = 0;            // Restore module existence

    EZBL_FIFOReset(&RxFifo, RxFifoBuffer, __builtin_section_size(RxFifoBuffer_sectName), 0, RX_FIFO_OnRead);
    EZBL_FIFOReset(&TxFifo, TxFifoBuffer, __builtin_section_size(TxFifoBuffer_sectName), TX_FIFO_OnWrite, 0);
    RxFifo.irqNum = RX_IRQ; // Bit position of IEC0bits.UxRXIE/IFS0bits.UxRXIF
    TxFifo.irqNum = TX_IRQ; // Bit position of IEC0bits.UxTXIE/IFS0bits.UxTXIF
    RxFifo.flushFunction = RX_FIFO_Flush;
    TxFifo.flushFunction = TX_FIFO_Flush;

    // Write UxMODE, UxSTA, UxBRG registers
    UxBRG = UxBRGVal < 0 ? -UxBRGVal : UxBRGVal;
    Uxbits = newCfg;
    newCfg.UARTEN = 1;
    Uxbits = newCfg;        // PIC24F/dsPIC33F/dsPIC33E UART needs UARTEN = 1 before UTXEN and URXEN can be set, so rewrite all bits

    _UxTXIP = EZBL_SYM(TX_ISR_PRIORITY);    // TX Interrupt priority = 1 by default (7 is highest priority, 0 is main() context)
    _UxRXIP = EZBL_SYM(RX_ISR_PRIORITY);    // RX Interrupt priority = 2 by default (7 is highest priority, 0 is main() context)
    FIFO_EnableInterrupts();
}


/**
 * Blocks execution until everything pending is finished being physically
 * transmitted or a specified NOW timeout is reached, whichever occurs first.
 *
 * This function will actively call the FIFO's interrupt if interrupts are 
 * blocked or the specific applicable IECx bit is clear, allowing data to 
 * continue to be transmitted at any time. 
 * 
 * @param *flushFIFO Pointer to the this TX FIFO that a flush request has been 
 *                   made against.
 * 
 *                   This parameter is included so the generic EZBL_FIFOFlush() 
 *                   function can call a generic COM_TX_FIFO_Flush() 
 *                   implementation shared by several run-time FIFOs via the 
 *                   EZBL_FIFO.flushFunction() function pointer and still permit 
 *                   special behavior for one or more FIFOs.
 *
 * @param *startTime Pointer to a caller provided unsigned long timestamp
 *                   indicating the present time (i.e. return value of NOW_32()).
 *
 * @param *timeout Pointer to a caller provided unsigned long NOW count variable
 *                 indicating how long this function can block before timing out
 *                 and returning. The value is a multiple or fraction of
 *                 NOW_sec/NOW_ms/NOW_us values.
 * 
 * @return If successful, the software TX FIFO and the hardware communications
 *         TX FIFO (if any) are drained to 0 bytes before returning with a value
 *         of 1. On timeout, a value of 1 is returned.
 */
unsigned int __attribute__((weak)) TX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout)
{
    while(flushFIFO->dataCount && (NOW_32() - *startTime <= *timeout))  // Wait for software TX FIFO to empty
    {
        if(EZBL_FIFOGetInt(flushFIFO))  // If ISR flag is staying set, this means we are executing from within an ISR @ CPU IPL >= _UxTXIP or the Interrupt is disabled, so we need to fall-back to polling. This could cause coherency bugs, if you reorder the clearing of _UxTXIF in the ISR, so be very careful if doing so!
        {
            EZBL_CallIRQ(flushFIFO->irqNum);
        }
    }
    while(!Uxbits.TRMT)                 // Wait for TX hardware FIFO to empty
    {
        if(NOW_32() - *startTime > *timeout)
            return 0;
    }
    return 1;
}


/**
 * Blocks execution until everything pending is finished being physically
 * transmitted. The software TX FIFO and the hardware UART TX FIFO are drained
 * to 0 bytes before returning.
 */
void __attribute__((weak)) TX_FIFO_WaitUntilFlushed(void)
{
    EZBL_FIFOFlush(&TxFifo, (unsigned long)-1);   // Indirectly calls TX_FIFO_Flush() with an infinite timeout.
}


/**
 * Enables interrupts used by the UART FIFO TX and RX routines by setting the
 * associated IECx bits.
 */
void __attribute__((weak)) FIFO_EnableInterrupts(void)
{
    _UxRXIE = 1;
    _UxTXIE = 1;
}


/**
 * Disables all interrupts used by the UART FIFO routines by clearing the
 * associated IECx bits.
 */
void __attribute__((weak)) FIFO_DisableInterrupts(void)
{
    _UxRXIE = 0;
    _UxTXIE = 0;
}


/**
 * Turns on hardware Auto-baud rate detection. Next character received must be 
 * 0x55 ('U'), after which UART RX will begin operating normally. UART TX rate
 * can change if transmitting while the auto-baud character is received. The
 * 0x55 character used for baud rate detection is not returned in the software
 * RX FIFO (nor hardware RX FIFO either).
 */
void __attribute__((weak)) FIFO_ArmAutoBaud(void)
{
    // Clear framing and other error accumulator if going to auto-baud mode
    RxFifo.activity.rxOverflow   = 0;
    RxFifo.activity.framingError = 0;
    RxFifo.activity.parityError  = 0;
    RxFifo.activity.other        = 1; // State change will break any ongoing RX, so signal in 'other'

    // Enable hardware Auto-baud mode
    Uxbits.ABAUD = 1;
}


/**
 * Tests if the RX hardware is armed for Auto-baud detection. If armed, the next
 * RX character is used to set the baud-rate and therefore must be the value 
 * 0x55 ('U'). This character does not get inserted into the RX stream when
 * it is used for auto-baud.
 *
 * @return 1 if auto-baud mode is set (FIFO_ArmAutoBaud() has been called
 *         and a timing character has not been received yet). 
 *         0 if auto-baud has completed or has never been armed.
 */
unsigned int __attribute__((weak)) FIFO_QueryAutoBaud(void)
{
    return Uxbits.ABAUD;
}


/**
 * Callback function automatically invoked anytime a write is made against the
 * UART TX FIFO (with or without any data actually getting written). Writes
 * occur when the EZBL_FIFOWrite() function is called (possibly indirectly
 * through a wrapper function).
 * 
 * This callback executes after the write has already taken place and is a
 * convenient place to trigger necessary hardware actions, spy on the data
 * passing through, tee it to another communications or storage interface, or
 * implement additional FIFO features, such as a blocking/almost guaranteed 
 * write with timeout mechanism to simplify other communications code.
 * 
 * If a write is made without enough FIFO space to store all of the data, this 
 * callback executes after the FIFO has been completely filled and provides an 
 * opportunity to process the residual data.
 * 
 * The default callback implementation will block when the FIFO is full and
 * trigger the UART TX ISR as needed to ensure the data doesn't have to be
 * thrown away. However, blocking will abort and throw unbuffered data away
 * after a fixed timeout is reached (ex: UART's 'ON' bit is clear, resulting in
 * clearance of data from the FIFO) The default timeout is set to 250ms (2,880
 * bytes @ 115200 baud, 480 bytes @ 19200), which can be changed in the callback
 * code.
 * 
 * @param bytesPushed Number of bytes that actually got written to the software
 *                    FIFO before this callback was invoked.
 * @param *writeSrc Pointer to the memory originally used when the FIFO write 
 *                  call was made. As no other code has executed yet, ordinary 
 *                  RAM data pointers can be re-read here to see or peek at all
 *                  of the requested write data.
 * @param regWriteLen Number of bytes that were requested to be written when the 
 *                    FIFO write call was made. Generally this will match the 
 *                    bytesPushed value unless the TX FIFO is completely full.
 * @param *destFIFO Pointer to the EZBL_FIFO that called this callback function.
 * 
 * @return Number of bytes that you want to return back to the EZBL_FIFOWrite() 
 *         caller as reportedly being successfully written. Generally you should
 *         return bytesPushed unless you've taken some action to transfer more 
 *         data (or perhaps stolen some back out of the FIFO).
 */
unsigned int __attribute__((weak)) TX_FIFO_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO)
{
    unsigned long startTime;

    // Trigger the TX ISR if we have TX hardware FIFO space to stuff this 
    // new data into
    if(!Uxbits.UTXBF)
    {
        _UxTXIF = 1;
    }
    
    // If the number of bytes written doesn't match what was requested, the FIFO 
    // is full. For simple and efficient procedural flow, we shall block on 
    // UART TX until all bytes are always written.
    if(bytesPushed != reqWriteLen)
    {
        startTime = NOW_32();
        
        // Nullify the onWriteCallback pointer to avoid recursion
        destFIFO->onWriteCallback = (void*)0;

        // Try to write some more residual bytes requested, but not previously 
        // possible. Give up after 250ms.
        do
        {
            if(_UxTXIF)
            {
                EZBL_CallISR(_UxTXInterrupt);
            }
            bytesPushed += EZBL_FIFOWrite(destFIFO, writeSrc + bytesPushed, reqWriteLen - bytesPushed);
        } while((bytesPushed != reqWriteLen) && (NOW_32() - startTime < NOW_sec/4u));

        // Restore onWriteCallback pointer
        destFIFO->onWriteCallback = TX_FIFO_OnWrite;
    }

    TxFifo.activity.softTx = 1;
    return bytesPushed;
}


/**
 * Callback function automatically invoked anytime a software read attempt is
 * made against the UART RX FIFO, but before the underlying EZBL_FIFORead() or
 * EZBL_FIFOPeek() call returns data to the caller. Provides an early
 * opportunity to block for more data being available, spy on the data passing
 * from the UART hardware into software, auto-generate flow control/status
 * information for the remote node, or potentially allows manipulation of
 * returned data.
 *
 * This function is called at the same IPL context as the caller for
 * EZBL_FIFORead()/EZBL_FIFOPeek().
 *
 * @param bytesPulled Number of bytes peeked or extracted from the FIFO and
 *                    being presently returned to the caller of
 *                    EZBL_FIFORead()/EZBL_FIFOPeek(). This is the number of
 *                    valid bytes present at *readDest.
 *
 * @param readDest Pointer to a caller allocated buffer where read data has been
 *                 written for the original call to 
 *                 EZBL_FIFORead()/EZBL_FIFOPeek(). This pointer may be null if
 *                 the caller is discarding (flushing) data from the FIFO
 *                 without actually reading it.
 *
 * @param reqReadLen Number of bytes requested to be read when 
 *                   EZBL_FIFORead()/EZBL_FIFOPeek() was called. This number
 *                   will be < bytesPulled if the FIFO did not have enough data
 *                   to satisfy the original request. This value may be zero.
 *
 * @param readFIFO Pointer to the EZBL_FIFO that EZBL_FIFORead()/EZBL_FIFOPeek()
 *                 was attempted against. As the same callback handler could
 *                 potentially be used for multiple FIFOs, this pointer provides
 *                 a context specific handle back to the FIFO that called this
 *                 callback function.
 *
 * @return Specify the number of bytes of data now available at *readDest for
 *         the caller to consume. If you do not add or subtract any data from
 *         the stream within this callback, always return bytesPulled,
 *         unchanged.
 *
 *         The default callback implementation blocks for up to 1 second if a
 *         read for data is made when insufficient data exists in the FIFO to
 *         fulfill the read request. While waiting, any just-received data is
 *         copied to the *readDest buffer to allow the original reqReadLen value
 *         to be met.
 *
 *         When blocking is required, this callback can force polled execution
 *         of the RX ISR if hardware has data available and the RX Interrupt is
 *         disabled or masked due to present execution above the RX ISR
 *         priority.
 */
unsigned int __attribute__((weak)) RX_FIFO_OnRead(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *readFIFO)
{
    unsigned long startTime;

    // Check if number of bytes read doesn't match what was requested -- i.e.
    // FIFO empty. For simple and efficient procedural flow, we shall block for
    // up to 1 second waiting for the requested data to show up.
    if(bytesPulled != reqReadLen)
    {
        startTime = NOW_32();
        
        // Nullify the onReadCallback pointer to avoid recursion
        readFIFO->onReadCallback = (void*)0;
        
        // Try to read some more residual bytes, but not previously possible. 
        // Give up after 1 second.
        do
        {
            if(_UxRXIF) // If ISR flag is staying set, this means we are executing from within an ISR @ CPU IPL >= _UxRXIP or the Interrupt is disabled, so we need to fall-back to polling. This could cause coherency bugs, if you reorder the clearing of _UxRXIF in the ISR, so be very careful if doing so!
            {
                EZBL_CallISR(_UxRXInterrupt);
            }
            bytesPulled += EZBL_FIFORead(readDest + bytesPulled, readFIFO, reqReadLen - bytesPulled);
        } while((reqReadLen != bytesPulled) && (NOW_32() - startTime < NOW_sec));

        // Restore onReadCallback pointer
        readFIFO->onReadCallback = RX_FIFO_OnRead;
    }

    RxFifo.activity.softRx = 1; // Record sticky status bit that someone called EZBL_FIFORead()/EZBL_FIFOPeek() against this FIFO. This is for top-level project use.
    return bytesPulled;
}


/*********************************************************************
 * Function:        void __attribute__((weak, vector(_UARTx_VECTOR), interrupt(IPLrxSOFT), keep)) UxRXInterrupt(void)
 *                  void _ISR _UxRXInterrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Records hardware overflow/framing/etc errors
 *                  into RxFifo.activity
 *
 * Side Effects:    None
 *
 * Overview:        Receives a physical RX byte from the UART and
 *                  places it in a local RAM FIFO for software to
 *                  read it at its leisure.
 *
 * Note:            None
 ********************************************************************/
#if defined(__XC32__)
void __attribute__((weak, vector(CAT(_UART,UART_MODULE_NUM,_RX_VECTOR)), interrupt(IPLrxSOFT), keep)) UxRXInterrupt(void)
#else
void __attribute__((weak, interrupt, no_auto_psv)) _UxRXInterrupt(void)
#endif
{
    unsigned int i;
    unsigned int status;
    EZBL_COM_ACTIVITY newFlags = {.any = EZBL_COM_ACTIVITY_RX_MASK};    // Set activity.rx = 1 always

    // Clear the interrupt flag so we don't keep entering this ISR
    _UxRXIF = 0;


    // Read all available bytes. This is checked before reading anything because
    // in the event of an auto-baud 0x55 reception, we'll get this RX interrupt,
    // but won't actually have any data to put into the software RX FIFO.
    while(_RXDA)
    {
        // Get the byte
        i = UxRXREG;
        
        // Collect any Parity (not used, so never should be set), Framing, 
        // and Overrun errors. Parity/Framing errors persist only while the 
        // applicable byte is at the top of the hardware RX FIFO, so we need to 
        // collect it for every byte.
        status = UxSTA;
        if(status & _UxSTA_OERR_MASK)
        {
            Uxbits.OERR = 0;    // Clear overflow flag if it it has become set. This bit prevents future reception so it must be cleared.
            newFlags.rxOverflow = 1;
        }
        if(status & _UxSTA_FERR_MASK)
            newFlags.framingError = 1;
        if(status & _UxSTA_PERR_MASK)
            newFlags.parityError = 1;

#if defined(BOOTLOADER_WAKE_KEY)
        {
            // Check if Bootloader needs waking up
            short bootloaderWakeKey[] = BOOTLOADER_WAKE_KEY;    // An exact-match RX string to allow EZBL Bootloader code to come alive and start Bootloader command processing when the Application is running.

            wakeKeyIndex &= 0x0007;                             // Restrict index to 0-7 so we don't have to bounds check if no crt data initialization is performed (saves perhaps 200 bytes of flash to disable this linker option)
            if(((char)i) == ((char*)bootloaderWakeKey)[wakeKeyIndex])
            {   // Might be external node trying to contact Bootloader, so decode the "MCUPHCME" first few bytes to decide if we should activate Bootloader command processing or not
                if(++wakeKeyIndex >= sizeof(bootloaderWakeKey))
                {   // All 8 bytes matched exactly, enable the EZBL command processor task
                    newFlags.bootkeyRx = 1;
                    EZBL_COM_RX = &RxFifo;
                    EZBL_COM_TX = &TxFifo;
                }
            }
            else
            {
                wakeKeyIndex = 0;
                if(((char)i) == ((char*)bootloaderWakeKey)[0])
                    wakeKeyIndex = 1;
            }
        }
#endif

        // Throw this byte away if it would cause overflow
        if(RxFifo.dataCount >= RxFifo.fifoSize)
        {
            newFlags.rxOverflow = 1;
            continue;
        }

        // Copy the byte into the local FIFO
        // NOTE: The FIFO internal data structures are being accessed directly
        // here rather than calling the RX_FIFO_Write*() functions because
        // any function call in an ISR will trigger a whole lot of compiler
        // context saving overhead. The compiler has no way of knowing what
        // registers any given function will clobber, so it has to save them
        // all. For efficiency, the needed write-one-byte code is duplicated
        // here.
        *RxFifo.headPtr++ = i;
        if(RxFifo.headPtr >= RxFifo.fifoRAM + RxFifo.fifoSize)
        {
            RxFifo.headPtr = RxFifo.fifoRAM;
        }
        EZBL_ATOMIC_ADD(RxFifo.dataCount, 1);
    }
    
    // Update all the activity flags we accumulated, including RxFifo.activity.rx = 1
    RxFifo.activity.any |= newFlags.any;
}


/*********************************************************************
 * Function:        void __attribute__((weak, vector(_UARTx_TX_VECTOR), interrupt(IPLtxSOFT), keep)) UxTXInterrupt(void)
 *                  void _ISR _UxTXInterrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           Zero or more bytes are removed from the software TX FIFO
 *
 * Output:          Bytes removed from the software TX FIFO are written to the 
 *                  hardware UART TX FIFO. On return, the data has not 
 *                  physically been transmitted onto the wire yet.
 *
 * Side Effects:    None
 *
 * Overview:        Transmits physical TX FIFOed bytes out of the UART
 *
 * Note:            None
 ********************************************************************/
#if defined(__XC32__)
void __attribute__((weak, vector(CAT(_UART,UART_MODULE_NUM,_TX_VECTOR)), interrupt(IPLtxSOFT), keep)) UxTXInterrupt(void)
#else
void __attribute__((weak, interrupt, no_auto_psv)) _UxTXInterrupt(void)
#endif
{
    _UxTXIF = 0;

    // Transmit a byte, if possible, if pending
    // NOTE: The FIFO internal data structures are being accessed directly
    // here rather than calling the TX_FIFO_Read*() functions because
    // any function call in an ISR will trigger a whole lot of compiler 
    // context saving overhead. The compiler has no way of knowing what 
    // registers any given function will clobber, so it has to save them 
    // all. For efficiency, the needed read-one-byte code is duplicated here.
    while(!Uxbits.UTXBF)
    {
        if(TxFifo.dataCount == 0u)
            return;

        // Transfer a byte from software FIFO to hardware TX FIFO
        UxTXREG = *TxFifo.tailPtr++;
        if(TxFifo.tailPtr >= TxFifo.fifoRAM + TxFifo.fifoSize)
            TxFifo.tailPtr = TxFifo.fifoRAM;
        EZBL_ATOMIC_SUB(TxFifo.dataCount, 1);
        TxFifo.activity.tx = 1;
    }
}
