/*
 * File:   i2c2_fifo.c
 * Author: Howard Schlunder
 *
 * An easily ported interrupt based I2C TX and RX buffering implementation.
 * Implements independent software FIFOs for TX and RX directions.
 *
 * Presently designed for I2C Slave operation with a 7-bit address only.
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
#define I2C_NUMBER              2               // Hardware I2C module # to apply this 
                                                // software FIFO to. 1 means I2C1, 2 
                                                // means I2C2, etc.
#define I2C_TX_FIFO_SIZE        16u             // Bytes for software TX FIFO buffering
#define I2C_RX_FIFO_SIZE        120u            // Bytes for software RX FIFO buffering

EZBL_SetWeakSYM(I2C2_MASTER_ISR_PRIORITY, 1);   // CPU IPL (Interrupt Priority Level) to
EZBL_SetWeakSYM(I2C2_SLAVE_ISR_PRIORITY, 1);    // assign to the TX and RX ISRs. 0 means
                                                // main() context, which is the lowest
                                                // priority and 7 is the highest,
                                                // time-critical, priority.

// Exact match RX character sequence to trigger assignment of EZBL_COM_RX to
// this UART. Comment this line out to disable auto-bootloader wake up on this UART.
#define BOOTLOADER_WAKE_KEY     {0x434D, 0x5055, 0x4348, 0x454D} //{'M','C','U','P','H','C','M','E'}



// All Registers/Bits for I2C1/I2C2 peripherals on all 16-bit devices except dsPIC30F, which is excluded

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
    #define SLAVE_IRQ  ((I2C_NUMBER == 1) ? 16u*1u + 0u :       /* IFS1<0> */   \
                        (I2C_NUMBER == 2) ? 16u*2u + 5u : 0u)   /* IFS2<5> */
    #define MASTER_IRQ (SLAVE_IRQ+1)
#elif defined(__XC16__)
    #define SLAVE_IRQ  ((I2C_NUMBER == 1) ? 16u*1u + 0u :       /* IFS1<0> */   \
                        (I2C_NUMBER == 2) ? 16u*3u + 1u : 0u)   /* IFS3<1> */
    #define MASTER_IRQ (SLAVE_IRQ+1)
#endif


// Static token concatenation macros that make the code easier to read.
// Don't directly change these unless porting to different hardware with 
// different register names. Instead change the I2C_NUMBER macro above to change 
// which physical I2C hardware module this FIFOing library applies to.
#define I2CxCON         CAT(I2C,I2C_NUMBER,CON)
#define I2CxCONbits     CAT(I2C,I2C_NUMBER,CONbits)
#define I2CxCON1        CAT(I2C,I2C_NUMBER,CON1)
#define I2CxCON1bits    CAT(I2C,I2C_NUMBER,CON1bits)
#define I2CxCON2        CAT(I2C,I2C_NUMBER,CON2)
#define I2CxCON2bits    CAT(I2C,I2C_NUMBER,CON2bits)
#define I2CxSTAT        CAT(I2C,I2C_NUMBER,STAT)
#define I2CxSTATbits    CAT(I2C,I2C_NUMBER,STATbits)
#define I2CxMSK         CAT(I2C,I2C_NUMBER,MSK)
#define I2CxRCV         CAT(I2C,I2C_NUMBER,RCV)
#define I2CxADD         CAT(I2C,I2C_NUMBER,ADD)
#define I2CxTRN         CAT(I2C,I2C_NUMBER,TRN)
#define I2CxBRG         CAT(I2C,I2C_NUMBER,BRG)
#define _I2CxMD         CAT(PMD1bits.I2C,I2C_NUMBER,MD)
#define _MI2CxIF        CAT(IFS0bits.MI2C,I2C_NUMBER,IF)
#define _SI2CxIF        CAT(IFS0bits.SI2C,I2C_NUMBER,IF)
#define _MI2CxIE        CAT(IEC0bits.MI2C,I2C_NUMBER,IE)
#define _SI2CxIE        CAT(IEC0bits.SI2C,I2C_NUMBER,IE)
#define _MI2CxIP        CAT(IPC0bits.MI2C,I2C_NUMBER,IP)
#define _SI2CxIP        CAT(IPC0bits.SI2C,I2C_NUMBER,IP)
#define _MI2CxInterrupt CAT(_MI2C,I2C_NUMBER,Interrupt)
#define _SI2CxInterrupt CAT(_SI2C,I2C_NUMBER,Interrupt)


typedef struct
{
    unsigned SEN:1;
    unsigned RSEN:1;
    unsigned PEN:1;
    unsigned RCEN:1;
    unsigned ACKEN:1;
    unsigned ACKDT:1;
    unsigned STREN:1;
    unsigned GCEN:1;
    unsigned SMEN:1;
    unsigned DISSLW:1;
    unsigned A10M:1;
    unsigned STRICT:1;  // This is IPMIEN instead of STRICT on earlier devices and has different meaning
    unsigned SCLREL:1;
    unsigned I2CSIDL:1;
    unsigned :1;
    unsigned I2CEN:1;
} I2CCONBITS;           // Called I2CxCONL or I2CxCON1 on newer peripheral variations, but same register
typedef struct
{
    unsigned DHEN:1;
    unsigned AHEN:1;
    unsigned SBCDE:1;
    unsigned SDAHT:1;
    unsigned BOEN:1;
    unsigned SCIE:1;
    unsigned PCIE:1;
} I2CCON2BITS;           // Sometimes called I2CxCONH instead, but same register. NOTE: Register does not exist on all devices.
typedef struct
{
    unsigned TBF:1;
    unsigned RBF:1;
    unsigned R_W:1;
    unsigned S:1;
    unsigned P:1;
    unsigned D_A:1;
    unsigned I2COV:1;
    unsigned IWCOL:1;
    unsigned ADD10:1;
    unsigned GCSTAT:1;
    unsigned BCL:1;
    unsigned :2;
    unsigned ACKTIM:1;  // Bit not on earlier devices
    unsigned TRSTAT:1;
    unsigned ACKSTAT:1;
} I2CSTATBITS;


// Original I2C peripheral register order (ex: PIC24FJ128GA010/i2c_v3, PIC24FJ128GA310/i2c_v2)
// Other versions, such as the dsPIC33EP64GS506/i2c_upb_v1 are different
extern volatile unsigned int    I2CxRCV             __attribute__((near));
extern volatile unsigned int    I2CxTRN             __attribute__((near));
extern          unsigned int    I2CxBRG             __attribute__((near));
extern volatile unsigned int    I2CxCON             __attribute__((near, weak));
extern volatile I2CCONBITS      I2CxCONbits         __attribute__((near, weak));
extern volatile unsigned int    I2CxCON1            __attribute__((near, weak));
extern volatile I2CCONBITS      I2CxCON1bits        __attribute__((near, weak));
extern volatile unsigned int    I2CxCON2            __attribute__((near));
extern volatile I2CCON2BITS     I2CxCON2bits        __attribute__((near));
extern volatile unsigned int    I2CxSTAT            __attribute__((near));
extern volatile I2CSTATBITS     I2CxSTATbits        __attribute__((near));
extern          unsigned int    I2CxADD             __attribute__((near));
extern          unsigned int    I2CxMSK             __attribute__((near));

extern volatile INTFLAGFIELDS   IFS0bits            __attribute__((near));
extern volatile INTENFIELDS     IEC0bits            __attribute__((near));
extern          INTPRIFIELDS    IPC0bits            __attribute__((near));
extern volatile PMDFIELDS       PMD1bits            __attribute__((near));




// Our prototypes
void I2C2_ResetCalc(unsigned long peripheralClockSpeed, long baudRate, int slaveAddr);
void I2C2_ResetBRG(int I2CxBRGVal, int slaveAddr);
void I2C2_TX_FIFO_WaitUntilFlushed(void);
void I2C2_FIFO_EnableInterrupts(void);
void I2C2_FIFO_DisableInterrupts(void);
unsigned int I2C2_TX_FIFO_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *writeFIFO);// EZBL_FIFOWrite() OnWrite callback function
unsigned int I2C2_RX_FIFO_OnRead(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *readFIFO);   // EZBL_FIFORead() OnRead callback function
unsigned int I2C2_TX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);
void __attribute__((interrupt, no_auto_psv)) _MI2CxInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _SI2CxInterrupt(void);


// Allocate RAM for the TX and RX software FIFOs
EZBL_FIFO __attribute__((weak, persistent))                                     I2C2_TxFifo;
EZBL_FIFO __attribute__((weak, persistent))                                     I2C2_RxFifo;
unsigned char __attribute__((weak, noload, section(".bss.I2C2_TxFifoBuffer")))  I2C2_TxFifoBuffer[I2C_TX_FIFO_SIZE];    // Memory array storing the actual FIFO data contents; Must be in the same named section that I2C2_TX_FIFO_Reset() inline code uses.
unsigned char __attribute__((weak, noload, section(".bss.I2C2_RxFifoBuffer")))  I2C2_RxFifoBuffer[I2C_RX_FIFO_SIZE];    // Memory array storing the actual FIFO data contents; Must be in the same named section that I2C2_RX_FIFO_Reset() inline code uses.
#if defined(BOOTLOADER_WAKE_KEY)
unsigned int __attribute__((weak)) I2C2_wakeKeyIndex = 0;     // Bootloader wake up key decoder position
#endif

static unsigned int             __attribute__((persistent)) bytesToFrameHeader; // Number of bytes we can send out the I2C before we need to send another framing byte count. Used in Slave ISR
static volatile unsigned int    __attribute__((persistent)) *pI2CxCON;          // Pointer to the I2CxCON or I2CxCON1 register. These are effectively the register, but because they have different symbol names, we must compute the correct one at run time.
#define pI2CxCONbits ((I2CCONBITS*)pI2CxCON)    // Type-casted reference to the same pointer as I2CxCON since they are actually the same register


/** 
 * Wrapper function for I2C2_ResetBRG() that pre-computes an appropriate I2CxBRG
 * value from a given clock speed and target bit rate.
 * 
 * @param peripheralClockSpeed Frequency, in hertz, that the I2C peripheral is
 *                             operating with. On PIC24 and dsPIC products, this 
 *                             is the same as the instructions executed/second 
 *                             while not dozing. Ex: set to 70000000 if running 
 *                             at 70 MIPS.
 *
 *                             If operating in slave mode, this parameter is
 *                             ignored.
 *
 * @param baudRate Desired baud rate for the I2C communications when operating 
 *                 in master mode. Ex: set to 400000 for 400kHz (bits/second).
 *                 The actual baud rate programmed will be as close as possible
 *                 to the requested value, but could still result in appreciable
 *                 error if specifying very fast baud rates or when operating at
 *                 a slow peripheral clock speed. See the I2C chapter in the
 *                 device data sheet or Family Reference Manual documentation to
 *                 understand the underlying error limits.
 *
 *                 For slave mode, this parameter is ignored.
 *
 * @param slaveAddr I2C Slave address (7-bit or 10-bit integer) for this node.
 *
 *                 If there are no I2C bus masters that may talk to us in this
 *                 hardware design, this parameter can be set to any value, but
 *                 it should be chosen to avoid spurious events (ex: not all 1's
 *                 and not all 0's, which are both reserved addresses anyway).
 */
void __attribute__((weak)) I2C2_ResetCalc(unsigned long peripheralClockSpeed, long baudRate, int slaveAddr)
{
    if(baudRate == 0)
        I2C2_ResetBRG(0, slaveAddr);
    else
        I2C2_ResetBRG(((int)(peripheralClockSpeed + (((unsigned long)baudRate)<<1))/(((unsigned long)baudRate)<<2)) - 1, slaveAddr);    // NOTE: This is not correct on some or all devices. I2C Master mode not supported right now.
}


/**
 * Resets the hardware I2C and software FIFOs. All FIFO data, if any, is
 * discarded. The I2C is configured and enabled afterwards and ready for
 * communications/interrupts.
 *
 * This function does not initialize I2C I/O pins. Ensure port I/O and PPS
 * settings are set to a passive state and any multiplexed analog functions are
 * turned off so they don't interfere with the I2C hardware taking control of
 * the applicable SDA/SCL I/O pins.
 *
 * @param I2CxBRGVal Value to write to the I2CxBRG register to set the baud
 *                   clock for master-mode transmission. To have this value
 *                   automatically computed for a given clock speed and target
 *                   bit rate, call the I2C_Reset() wrapper function instead.
 *
 *                   If operating in slave mode, this parameter is ignored.
 *
 * @param slaveAddr I2C Slave address (7-bit or 10-bit integer) for this node.
 *
 *                 If there are no I2C bus masters that may talk to us in this
 *                 hardware design, this parameter can be set to any value, but
 *                 it should be chosen to avoid spurious events (ex: not all 1's
 *                 and not all 0's, which are both reserved addresses anyway).
 */
void __attribute__((weak)) I2C2_ResetBRG(int I2CxBRGVal, int slaveAddr)
{
    I2CCONBITS initCON =
    {
        .I2CEN   = 0,
        .I2CSIDL = 0,
        .SCLREL  = 1,
        .STRICT  = 0,
        .A10M    = (slaveAddr > 0x7F),
        .DISSLW  = 0,
        .SMEN    = 0,
        .GCEN    = 0,
        .STREN   = 1,
        .ACKDT   = 0,
        .ACKEN   = 0,
        .RCEN    = 0,
        .PEN     = 0,
        .RSEN    = 0,
        .SEN     = 0
    };
    
    pI2CxCON = (volatile unsigned int *)((unsigned int)&I2CxCON | (unsigned int)&I2CxCON1);// Initialize the pI2CxCON pointer to point to I2CxCON. These are both weak and mutually exclusive, so one of them will return a 0x0000 pointer.
    *pI2CxCON = 0;                  // Disable the I2C module if already enabled. This will suppress interrupts and allow us to reconfigure the module.
    _I2CxMD    = 1;                 // Set the Peripheral Module Disable bit to fully reset the peripheral and all SFRs specific to the peripheral
    bytesToFrameHeader = 0;
    _I2CxMD    = 0;                 // Restore peripheral existence with SFRs reset

    EZBL_FIFOReset(&I2C2_TxFifo, I2C2_TxFifoBuffer,  __builtin_section_size(".bss.I2C2_TxFifoBuffer"), I2C2_TX_FIFO_OnWrite, 0);
    EZBL_FIFOReset(&I2C2_RxFifo, I2C2_RxFifoBuffer,  __builtin_section_size(".bss.I2C2_RxFifoBuffer"), 0, I2C2_RX_FIFO_OnRead);
    I2C2_TxFifo.irqNum = SLAVE_IRQ; // Assign to Slave mode ISR, bit position of IEC0bits.SI2CxIE/IFS0bits.SI2CxIF (actually evaluates to IEC1/IFS1/IEC3/IFS3)
    I2C2_RxFifo.irqNum = SLAVE_IRQ; // Assign to Slave mode ISR, bit position of IEC0bits.SI2CxIE/IFS0bits.SI2CxIF (actually evaluates to IEC1/IFS1/IEC3/IFS3)
    I2C2_TxFifo.flushFunction = I2C2_TX_FIFO_Flush;
    I2C2_RxFifo.flushFunction = I2C2_RX_FIFO_Flush;

    *pI2CxCONbits = initCON;            // Set SCLREL, STREN = 1, all other bits = 0. Keeps I2C module disabled, but configures to enable clock stretching. Uses normal 7-bit address mode if slaveAddr fits in 7-bits, but otherwise use 10-bit mode.
    I2CxSTAT = 0x0000;                  // Clear Write Collision and Receive Overflow sticky status bits
    I2CxMSK  = 0x0000;                  // Set RX Address mask to use all 7 lower-order bits
    I2CxADD  = (unsigned int)slaveAddr; // Set I2C slave address of this node for RX filtering
    I2CxBRG  = I2CxBRGVal;              // Program Master mode baud rate generator to what was set outside this file
    //_MI2CxIF = 0;                     // Clear Master Interrupt Flag
    _SI2CxIF = 0;                       // Clear Slave Interrupt Flag
    //_MI2CxIP = EZBL_SYM(I2C2_MASTER_ISR_PRIORITY);// Set Master interrupt to Priority 1 (0 is main context, 7 is time-critical highest priority)
    _SI2CxIP = EZBL_SYM(I2C2_SLAVE_ISR_PRIORITY);   // Set Slave interrupt to Priority 1 (0 is main context, 7 is time-critical highest priority)
    I2C2_FIFO_EnableInterrupts();

    pI2CxCONbits->I2CEN = 1;    // Enable the I2C module
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
 *                   function can call a generic TX_FIFO_Flush() 
 *                   implementation shared by several run-time FIFOs via the 
 *                   EZBL_FIFO->flushFunction() function pointer and still permit 
 *                   special behavior for one or more FIFOs.
 * 
 * @return If successful, the software TX FIFO and the hardware communications
 *         TX FIFO (if any) are drained to 0 bytes before returning with a value
 *         of 1. On timeout, a value of 1 is returned.
 */
unsigned int __attribute__((weak)) I2C2_TX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout)
{
    while(flushFIFO->dataCount && (NOW_32() - *startTime <= *timeout))  // Wait for software TX FIFO to empty
    {
        if(EZBL_FIFOGetInt(flushFIFO))  // If ISR flag is staying set, this means we are executing from within an ISR @ CPU IPL >= _UxTXIP or the Interrupt is disabled, so we need to fall-back to polling. This could cause coherency bugs, if you reorder the clearing of _UxTXIF in the ISR, so be very careful if doing so!
        {
            EZBL_CallIRQ(flushFIFO->irqNum);
        }
    }
    while(I2CxSTATbits.TBF)            // Wait for TX hardware shift register to empty
    {
        if(NOW_32() - *startTime > *timeout)
            return 0;
    }
    return 1;
}


/**
 * Blocks execution until everything pending is finished being physically
 * transmitted. The software TX FIFO is allowed to drain to 0 bytes and any
 * shifting data in the hardware is completed before returning.
 */
void  __attribute__((weak)) I2C2_TX_FIFO_WaitUntilFlushed(void)
{
    EZBL_FIFOFlush(&I2C2_TxFifo, (unsigned long)-1);   // Indirectly calls I2C2_TX_FIFO_Flush() with an infinite timeout.
}


/**
 * Enables interrupts used by the I2C FIFO TX and RX routines by setting the
 * associated IECx bits.
 */
void  __attribute__((weak)) I2C2_FIFO_EnableInterrupts(void)
{
    //_MI2CxIE = 1;
    _SI2CxIE = 1;
}


/**
 * Disables all interrupts used by the I2C FIFO routines by clearing the
 * associated IECx bits.
 */
void  __attribute__((weak)) I2C2_FIFO_DisableInterrupts(void)
{
    //_MI2CxIE = 0;
    _SI2CxIE = 0;
}


/**
 * Callback function automatically invoked anytime a write is made against the
 * I2C TX FIFO (with or without any data actually getting written).
 * 
 * This function is called after the write has taken place and is 
 * a convenient place to trigger necessary hardware actions, spy on the data 
 * passing through, possibly teeing it to another communications interface, or 
 * implement additional FIFO features, such as a blocking/almost guaranteed 
 * write with timeout mechanism to simplify other communications code.
 * 
 * @param bytesPushed Number of bytes that actually got written to the FIFO 
 *                    before this callback was invoked.
 * @param *writeSrc Pointer to the memory originally used when the FIFO write 
 *                  call was made. As no other code has executed yet, ordinary 
 *                  RAM data sources can be re-read here to tee the data.
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
unsigned int  __attribute__((weak)) I2C2_TX_FIFO_OnWrite(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO)
{
    I2C2_TxFifo.activity.softTx = 1;

    // For Slave mode, we can't transmit until a Master issues a read to us, so
    // no ISR to trigger.

    return bytesPushed;
}


/**
 * Callback function automatically invoked anytime a software read attempt is
 * made against the RX FIFO, but before the underlying EZBL_FIFORead() call
 * returns data to the caller. Provides an early opportunity to block for
 * more data being available, spy on the data passing from the I2C hardware
 * into software, auto-generate flow control/status information for the remote
 * node, or potentially allows manipulation of returned data.
 *
 * This function is called at the same IPL context as the caller for
 * EZBL_FIFORead().
 *
 * @param bytesPulled Number of bytes extracted from the FIFO and being
 *                    presently returned to the caller of EZBL_FIFORead(). This
 *                    is the number of valid bytes present at *readDest.
 *
 * @param readDest Pointer to a caller allocated buffer where read data has been
 *                 written for the original call to EZBL_FIFORead(). This
 *                 pointer may be null if the caller is discarding data from the
 *                 FIFO without actually reading it.
 *
 * @param reqReadLen Number of bytes requested to be read when EZBL_FIFORead()
 *                   was called. This number will be < bytesPulled if the FIFO
 *                   did not have enough data to satisfy the original
 *                   EZBL_FIFORead() request. This value may be zero.
 *
 * @param readFIFO Pointer to the EZBL_FIFO that EZBL_FIFORead() was attempted
 *                 against. As the same callback handler could potentially be
 *                 used for multiple FIFOs, this pointer provides a context
 *                 specific handle back to the FIFO that called this callback
 *                 function.
 *
 * @return Specify the number of bytes of data now available at *readDest for
 *         the caller to consume. If you do not add or subtract any data from
 *         the stream within this callback, always return bytesPulled,
 *         unchanged.
 *
 *         This example callback implementation is non-blocking and will simply
 *         return with less than the requested number of bytes if a read is made
 *         for more than already exists in the FIFO.
 */
unsigned int __attribute__((weak)) I2C2_RX_FIFO_OnRead(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *readFIFO)
{
    I2C2_RxFifo.activity.softRx = 1;
    return bytesPulled;
}

///**
// * I2C master mode interrupt handler. This is an incomplete skeleton. Add
// * what you wish to do here.
// */
//void __attribute__((weak, interrupt, no_auto_psv)) _MI2CxInterrupt(void)
//{
//    union
//    {
//        I2CCONBITS bits;   // Local copy of the I2CxCONbits to ensure concurrency of bit states and avoid many volatile reads of the SFR
//        unsigned int val;
//    } state;
//    union
//    {
//        I2CSTATBITS bits;   // Local copy of the I2CxSTATbits to ensure concurrency of bit states and avoid many volatile reads of the SFR
//        unsigned int val;
//    } stat;
//
//    _MI2CxIF = 0;   // Acknowledge this interrupt
//
//    state.val = *pI2CxCON;
//    stat.val  = I2CxSTAT;
//    switch(state.val & 0x1Fu)
//    {
//        case 0x00:  // Bus Idle or Wait, OR, Master Transmitting
//            if(stat.bits.TRSTAT)
//            {   // Master Transmitting state (i.e. Finished sending Start bit, need data now)
//            }
//            break;
//
//        case 0x01:  // Sending Start Event
//            break;
//
//        case 0x02:  // Sending Repeated Start Event
//            break;
//
//        case 0x04:  // Sending Stop Event
//            break;
//
//        case 0x08:  // Master Reception
//            break;
//
//        case 0x10:  // Master Acknowledgment
//            break;
//    }
//}


/*********************************************************************
 * Function:        void _ISR _SI2CxInterrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Records hardware receive data into the software RX FIFO for
 *                  later processing elsewhere, such as the main() context.
 *
 *                  Copies any pending data from the TX FIFO to the I2C shift
 *                  transmit register whenever possible and needed. 
 *                  This TX data is framed using a 1 byte frame header
 *                  representing the number of pending TX bytes in 
 *                  the frame so that the master knows how many I2C 
 *                  clocks it should give us. This framing byte is sent anytime
 *                  a read request is started/restarted (i.e. just after the
 *                  address byte is on the wire).
 *
 *                  Sets EZBL_COM_RX and EZBL_COM_TX pointers to point to the
 *                  software EZBL_FIFOs for this I2C module if the
 *                  BOOTLOADER_WAKE_KEY sequence has been defined and is
 *                  received from a remote node.
 *
 * Side Effects:    None
 *
 * Overview:        Receives a physical RX byte from the I2C hardware 
 *                  and places it in a local RAM FIFO for software to
 *                  read it at its leisure. Saves any errors, and 
 *                  places TX FIFO data into the transmit shift 
 *                  hardware.
 *
 * Note:            None
 ********************************************************************/
void __attribute__((weak, interrupt, no_auto_psv)) _SI2CxInterrupt(void)
{
    unsigned int i;
    union
    {
        I2CSTATBITS bits;   // Local copy of the I2CxSTATbits to ensure concurrency of bit states and avoid many volatile reads of the SFR
        unsigned int val;
    } stat;

    _SI2CxIF = 0;           // Clear interrupt

    stat.val = I2CxSTAT;    // Get a concurrent snapshot of the I2C status bits

    if(stat.bits.RBF)       // Receive Buffer Full
    {
        i = I2CxRCV;
        if(stat.bits.D_A && !stat.bits.R_W) // Data phase write byte
        {   // PC Writing data to us
            I2C2_RxFifo.activity.rx = 1;
            if(I2C2_RxFifo.dataCount >= I2C2_RxFifo.fifoSize)
            {
                I2C2_RxFifo.activity.rxOverflow = 1;
            }
            else
            {
                *I2C2_RxFifo.headPtr++ = i;
                if(I2C2_RxFifo.headPtr >= I2C2_RxFifo.fifoRAM + I2C2_RxFifo.fifoSize)
                {
                    I2C2_RxFifo.headPtr = I2C2_RxFifo.fifoRAM;
                }
                EZBL_ATOMIC_ADD(I2C2_RxFifo.dataCount, 1);
            }
#if defined(BOOTLOADER_WAKE_KEY)
            {
                // Check if Bootloader needs waking up
                short bootloaderWakeKey[] = BOOTLOADER_WAKE_KEY;// An exact-match RX string to allow EZBL Bootloader code to come alive and start Bootloader command processing when the Application is running.

                I2C2_wakeKeyIndex &= 0x0007;                        // Restrict index to 0-7 so we don't have to bounds check if no crt data initialization is performed (saves perhaps 200 bytes of flash to disable this linker option)
                if(((char)i) == ((char*)bootloaderWakeKey)[I2C2_wakeKeyIndex])
                {   // Might be external node trying to contact Bootloader, so decode the "MCUPHCME" first few bytes to decide if we should activate Bootloader command processing or not
                    if(++I2C2_wakeKeyIndex >= sizeof(bootloaderWakeKey))
                    {   // All 8 bytes matched exactly, enable the EZBL command processor task
                        I2C2_RxFifo.activity.bootkeyRx = 1;
                        EZBL_COM_RX = &I2C2_RxFifo;
                        EZBL_COM_TX = &I2C2_TxFifo;
                    }
                }
                else
                {
                    I2C2_wakeKeyIndex = 0;
                    if(((char)i) == ((char*)bootloaderWakeKey)[0])
                        I2C2_wakeKeyIndex = 1;
                }
            }
#endif
        }
    }
    if(stat.bits.R_W && !stat.bits.TBF) // Read and I2C TX space available
    {
        if(!stat.bits.D_A)
        {   // First TX byte of PC read sequence (we just received our address)
            bytesToFrameHeader = I2C2_TxFifo.dataCount;
            if(bytesToFrameHeader>>8)
                bytesToFrameHeader = 0xFF;
            I2CxTRN = bytesToFrameHeader;
            I2C2_TxFifo.activity.other = 1;
        }
        else if(!stat.bits.ACKSTAT)
        {   // PC continuing to read data from us
            if(bytesToFrameHeader == 0u)
            {
                bytesToFrameHeader = I2C2_TxFifo.dataCount;
                if(bytesToFrameHeader>>8)
                    bytesToFrameHeader = 0xFF;
                I2CxTRN = bytesToFrameHeader;
                I2C2_TxFifo.activity.other = 1;
            }
            else
            {
                I2CxTRN = (unsigned int)(*I2C2_TxFifo.tailPtr++);
                if(I2C2_TxFifo.tailPtr >= I2C2_TxFifo.fifoRAM + I2C2_TxFifo.fifoSize)
                    I2C2_TxFifo.tailPtr = I2C2_TxFifo.fifoRAM;
                EZBL_ATOMIC_SUB(I2C2_TxFifo.dataCount, 1);
                I2C2_TxFifo.activity.tx = 1;
                bytesToFrameHeader--;
            }
        }
    }

    if(!pI2CxCONbits->SCLREL)
    {
        pI2CxCONbits->SCLREL = 1; // Release clock low hold
    }
}
