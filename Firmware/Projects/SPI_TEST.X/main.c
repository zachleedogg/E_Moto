/* 
 * File:   main.c
 * Author: Zach
 *
 * Created on September 20, 2016, 12:07 AM
 */

#include <stdio.h>
#include <stdlib.h>

#include <xc.h>


#include "bolt_uart.h"
#include "bolt_init.h"

static uint16_t i = 0;

#define MAX_XFER_SIZE 2

/*
 * 
 */
int main(void) {
    Micro_Init();
    clockInit(FREQ_32MHZ, EXTERNAL);
    Uart1Init(RP36_TX, RP20_UART_RX, BAUD115200);

    Uart1Write("SPI test program\n\n");
    _RB10 = 1;
    _TRISB10 = 0;

    //Set up DMA Channel 0 to Transmit in Continuous Ping-Pong Mode:
    uint16_t TxBufferA[MAX_XFER_SIZE] __attribute__((aligned(MAX_XFER_SIZE)));
    for (i = 0; i < MAX_XFER_SIZE; i++) {
        TxBufferA[i] = i+32;
    }


    DMA2CONbits.SIZE = 0; //word
    DMA2CONbits.DIR = 1; //transfer from RAM to peripheral
    DMA2CONbits.HALF = 0; //Interrupt after data moved
    DMA2CONbits.NULLW = 0; //Normal operation
    DMA2CONbits.AMODE = 1; //indirect address mode with post increment
    DMA2CONbits.MODE = 1; //One-Shot

    DMA2STAL = (uint16_t) & TxBufferA;
    DMA2STAH = 0x0000;
    DMA2PAD = (volatile uint16_t) & SPI1BUF;
    DMA2CNT = 0xFFFF;
    DMA2REQ = 0x000A;

    IFS1bits.DMA2IF = 0;
    IEC1bits.DMA2IE = 1;

    DMA2CONbits.CHEN = 1;


    uint16_t dummyRead __attribute__((aligned(1)));

    DMA3CONbits.SIZE = 0; //word
    DMA3CONbits.DIR = 0; //transfer from peripheral to RAM
    DMA3CONbits.HALF = 1; //Interupt after data moved
    DMA3CONbits.NULLW = 0; //Normal operation
    DMA3CONbits.AMODE = 1; //indirect address mode without post increment
    DMA3CONbits.MODE = 0; //continuous mode

    DMA3STAL = (uint16_t) & dummyRead;
    DMA3STAH = 0x0000;
    DMA3PAD = (volatile unsigned int) &SPI1BUF;
    DMA3CNT = 0xFFFF;
    DMA3REQ = 0x000A;


    DMA3CONbits.CHEN = 1;




    // SPI1CON1 Register Settings
    SPI1CON1bits.DISSCK = 0; // Internal serial clock is enabled
    SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 1; // Communication is word-wide (16 bits)
    SPI1CON1bits.MSTEN = 1; // Master mode enabled
    SPI1CON1bits.SMP = 0; // Input data is sampled at the middle of data output time
    SPI1CON1bits.CKE = 1; // Serial output data changes on transition from
    // active clock state to idle clock state
    SPI1CON1bits.CKP = 0; // Idle state for clock is a low level;
    //SPI1CON2bits.SPIBEN = 1; /*Enable FIFO transmit buffer*/
    SPI1STATbits.SISEL = 0b100; /*interrupt on last byte out*/

    /*11 = Primary prescale 1:1
      10 = Primary prescale 4:1
      01 = Primary prescale 16:1
      00 = Primary prescale 64:1*/
    SPI1CON1bits.PPRE = 0b11;

    /*111 = Secondary prescale 1:1
      110 = Secondary prescale 2:1
      101 = Secondary prescale 3:1
      100 = Secondary prescale 4:1
      011 = Secondary prescale 5:1
      010 = Secondary prescale 6:1
      001 = Secondary prescale 7:1
      000 = Secondary prescale 8:1*/
    SPI1CON1bits.SPRE = 0b110;


    IPC2bits.SPI1EIP = 3; //priority 5

    SPI1STATbits.SPIEN = 1; // Enable SPI module
    IFS0bits.SPI1IF = 0;
    //IEC0bits.SPI1IE = 1; // Enable the interrupt

    // Force First Word After Enabling SPI
    _RB10 = 0;
    DMA2REQbits.FORCE = 1;
    //while (DMA2REQbits.FORCE == 1);
    i = 0;

    for (i = 0; i < 10000; i++) {

    }
    DMA2CONbits.CHEN = 1;
    DMA2REQbits.FORCE = 1;

    /*Sit and Spin*/
    while (1) {
    }

    /*Never get here....*/
    return (0);
}

void __attribute__((__interrupt__, auto_psv)) _DMA2Interrupt(void) {
    IFS1bits.DMA2IF = 0; // Clear the DMA0 Interrupt flag
    Uart1Write("DMA0int\n");
}