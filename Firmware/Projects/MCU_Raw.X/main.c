/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  dsPIC33EP512MU810
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.50
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include "tsk.h"
#include <xc.h>

// Configuration bits: selected in the GUI

// FGS
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GSS = OFF    //General Segment Code-Protect bit->General Segment Code protect is disabled
#pragma config GSSK = OFF    //General Segment Key bits->General Segment Write Protection and Code Protection is Disabled

// FOSCSEL
#pragma config FNOSC = FRC    //Initial Oscillator Source Selection Bits->FRC
#pragma config IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FOSC
#pragma config POSCMD = XT    //Primary Oscillator Mode Select bits->XT Crystal Oscillator Mode
#pragma config OSCIOFNC = ON    //OSC2 Pin Function bit->OSC2 is general purpose digital I/O pin
#pragma config IOL1WAY = OFF    //Peripheral pin select configuration->Allow multiple reconfigurations
#pragma config FCKSM = CSECMD    //Clock Switching Mode bits->Clock switching is enabled,Fail-safe Clock Monitor is disabled

// FWDT
#pragma config WDTPOST = PS32768    //Watchdog Timer Postscaler Bits->1:32768
#pragma config WDTPRE = PR128    //Watchdog Timer Prescaler bit->1:128
#pragma config PLLKEN = ON    //PLL Lock Wait Enable bit->Clock switch to PLL source will wait until the PLL lock signal is valid.
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->Watchdog Timer in Non-Window mode
#pragma config FWDTEN = OFF    //Watchdog Timer Enable bit->Watchdog timer enabled/disabled by user software

// FPOR
#pragma config FPWRT = PWR128    //Power-on Reset Timer Value Select bits->128ms
#pragma config BOREN = ON    //Brown-out Reset (BOR) Detection Enable bit->BOR is enabled
#pragma config ALTI2C1 = ON    //Alternate I2C pins for I2C1->ASDA1/ASCK1 pins are selected as the I/O pins for I2C1
#pragma config ALTI2C2 = OFF    //Alternate I2C pins for I2C2->SDA2/SCK2 pins are selected as the I/O pins for I2C2

// FICD
#pragma config ICS = PGD2    //ICD Communication Channel Select bits->Communicate on PGEC2 and PGED2
#pragma config RSTPRI = PF    //Reset Target Vector Select bit->Device will obtain reset instruction from Primary flash
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled

// FAS
#pragma config AWRP = OFF    //Auxiliary Segment Write-protect bit->Auxiliary program memory is not write-protected
#pragma config APL = OFF    //Auxiliary Segment Code-protect bit->Aux Flash Code protect is disabled
#pragma config APLK = OFF    //Auxiliary Segment Key bits->Aux Flash Write Protection and Code Protection is Disabled

void CLOCK_Initialize(void)
{
    // FRCDIV FRC/1; PLLPRE 2; DOZE 1:8; PLLPOST 1:2; DOZEN disabled; ROI disabled; 
    CLKDIV = 0x3000;
    // TUN Center frequency; 
    OSCTUN = 0x00;
    // ROON disabled; ROSEL FOSC; RODIV 0; ROSSLP disabled; 
    REFOCON = 0x00;
    // PLLDIV 58; 
    PLLFBD = 0x3A;
    // ENAPLL disabled; APLLPOST 1:256; FRCSEL FRC; SELACLK Auxiliary Oscillators; ASRCSEL Primary Oscillator; AOSCMD AUX; APLLPRE 1:1; 
    ACLKCON3 = 0x2600;
    // APLLDIV 15; 
    ACLKDIV3 = 0x00;
    // AD1MD enabled; PWMMD enabled; T3MD enabled; T4MD enabled; T1MD enabled; U2MD enabled; T2MD enabled; U1MD enabled; QEI1MD enabled; SPI2MD enabled; SPI1MD enabled; C2MD enabled; C1MD enabled; DCIMD enabled; T5MD enabled; I2C1MD enabled; 
    PMD1 = 0x00;
    // OC5MD enabled; OC6MD enabled; OC7MD enabled; OC8MD enabled; OC1MD enabled; IC2MD enabled; OC2MD enabled; IC1MD enabled; OC3MD enabled; OC4MD enabled; IC6MD enabled; IC7MD enabled; IC5MD enabled; IC8MD enabled; IC4MD enabled; IC3MD enabled; 
    PMD2 = 0x00;
    // AD2MD enabled; PMPMD enabled; CMPMD enabled; U3MD enabled; QEI2MD enabled; RTCCMD enabled; T9MD enabled; T8MD enabled; CRCMD enabled; T7MD enabled; I2C2MD enabled; DAC1MD enabled; T6MD enabled; 
    PMD3 = 0x00;
    // U4MD enabled; USB1MD enabled; REFOMD enabled; 
    PMD4 = 0x00;
    // OC9MD enabled; OC16MD enabled; IC10MD enabled; IC11MD enabled; IC12MD enabled; IC13MD enabled; IC14MD enabled; IC15MD enabled; IC16MD enabled; IC9MD enabled; OC14MD enabled; OC15MD enabled; OC12MD enabled; OC13MD enabled; OC10MD enabled; OC11MD enabled; 
    PMD5 = 0x00;
    // PWM2MD enabled; PWM1MD enabled; SPI4MD enabled; PWM4MD enabled; SPI3MD enabled; PWM3MD enabled; PWM6MD enabled; PWM5MD enabled; 
    PMD6 = 0x00;
    // DMA8MD enabled; DMA4MD enabled; DMA12MD enabled; DMA0MD enabled; 
    PMD7 = 0x00;
    // CF no clock failure; NOSC PRIPLL; LPOSCEN disabled; CLKLOCK unlocked; OSWEN Switch is Complete; IOLOCK not-active; 
    __builtin_write_OSCCONH((uint8_t) (0x03));
    __builtin_write_OSCCONL((uint8_t) (0x01));
    // Wait for Clock switch to occur
    while (OSCCONbits.OSWEN != 0);
    while (OSCCONbits.LOCK != 1);
}


/*
                         Main application
 */
int main(void)
{
    //MSTRPR = 0x0020;
    // initialize the device
    CLOCK_Initialize();
    // put this in memory_partition.s
    //    .equ    RESERVED_MEMORY_START, (0x55012+2) //TODO: Why did I have to modify this value??? Old value was 0x557FE -ZL
    while (1)
    {
        Tsk_Run(120000000);
    }
    return 1; 
}
/**
 End of File
*/



