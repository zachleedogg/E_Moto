/*
 * File:   bolt_spi.c
 * Author: Zach Levenberg
 *
 * Created on February 10, 2016, 10:23 PM
 */

#include "bolt_spi.h"

/*Output PPS registers*/
#define RP20_SPI_PPS    _RP20R
#define RP35_SPI_PPS    _RP35R
#define RP36_SPI_PPS    _RP36R
#define RP37_SPI_PPS    _RP37R
#define RP38_SPI_PPS    _RP38R
#define RP39_SPI_PPS    _RP39R
#define RP40_SPI_PPS    _RP40R
#define RP41_SPI_PPS    _RP41R
#define RP42_SPI_PPS    _RP42R
#define RP43_SPI_PPS    _RP43R

/*Output PPS functions*/
#define SCK2 0x09
#define SDO2 0x08

#define  SEC_PRESCAL_1_1        0b111  /* Secondary Prescale 1:1   */
#define  SEC_PRESCAL_2_1        0b110  /* Secondary Prescale 2:1   */
#define  SEC_PRESCAL_3_1        0b101  /* Secondary Prescale 2:1   */
#define  SEC_PRESCAL_4_1        0b100  /* Secondary Prescale 4:1   */
#define  SEC_PRESCAL_5_1        0b011  /* Secondary Prescale 5:1   */
#define  SEC_PRESCAL_6_1        0b010  /* Secondary Prescale 6:1   */
#define  SEC_PRESCAL_7_1        0b001  /* Secondary Prescale 7:1   */
#define  SEC_PRESCAL_8_1        0b000  /* Secondary Prescale 8:1   */

#define  PRI_PRESCAL_1_1        0b11  /* Primary Prescale 1:1     */
#define  PRI_PRESCAL_4_1        0b10  /* Primary Prescale 4:1     */
#define  PRI_PRESCAL_16_1       0b01  /* Primary Prescale 16:1    */
#define  PRI_PRESCAL_64_1       0b00  /* Primary Prescale 64:1    */


static const uint8_t ppsOut[2] = {SCK2, SDO2};

static uint8_t spi1Status = 1;
static uint8_t spi2Status = 0;

void spi1Init(void) {

    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    IEC0bits.SPI1IE = 0; // Disable the interrupt

    SPI1CON1bits.DISSCK = 0; // Internal serial clock is enabled
    SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 0; // Communication is word-wide (8 bits)
    SPI1CON1bits.MSTEN = 1; // Master mode enabled
    SPI1CON1bits.SMP = 0; // Input data is sampled at the middle of data output time
    SPI1CON1bits.CKE = 1; // Serial output data changes on transition from
    // active clock state to idle clock state
    SPI1CON1bits.CKP = 0; // Idle state for clock is a low level;

    SPI1CON2bits.SPIBEN = 1; /*Enable FIFO transmit buffer*/
    SPI1STATbits.SISEL = 0b101; /*interrupt on last byte out*/

    /*11 = Primary prescale 1:1
      10 = Primary prescale 4:1
      01 = Primary prescale 16:1
      00 = Primary prescale 64:1*/
    SPI1CON1bits.PPRE = 0b01;

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
    IEC0bits.SPI1IE = 1; // Enable the interrupt

}

uint8_t spi2Init(spi_pin_number CLK, spi_pin_number D_OUT) {

    //check duplicate pins
    if (CLK == D_OUT) {
        return 1;
    }

    //CLOCK and D_OUT
    spi_pin_number curPin[] = {CLK, D_OUT};

    int i = 0;
    for (i = 0; i < 2; i++) {
        switch (curPin[i]) {
            case RP20_SPI:
                RP20_SPI_PPS = ppsOut[i];
                break;
            case RP35_SPI:
                RP35_SPI_PPS = ppsOut[i];
                break;
            case RP36_SPI:
                RP36_SPI_PPS = ppsOut[i];
                break;
            case RP37_SPI:
                RP37_SPI_PPS = ppsOut[i];
                break;
            case RP38_SPI:
                RP38_SPI_PPS = ppsOut[i];
                break;
            case RP39_SPI:
                RP39_SPI_PPS = ppsOut[i];
                break;
            case RP40_SPI:
                RP40_SPI_PPS = ppsOut[i];
                break;
            case RP41_SPI:
                RP40_SPI_PPS = ppsOut[i];
                break;
            case RP42_SPI:
                RP42_SPI_PPS = ppsOut[i];
                break;
            case RP43_SPI:
                RP43_SPI_PPS = ppsOut[i];
                break;
            default:
                return 1;
                break;
        }
    }

    _SPI2IF = 0; // Clear the Interrupt flag
    _SPI2IE = 0; // Disable the interrupt

    _SCK2R = 0x2A; //pin 42 clock in
    _RP42R = 0x09; //pin 42 clock out
    _RP43R = 0x08; //Dout to 43

    SPI2CON1bits.DISSCK = 0; /*clock enabled*/
    SPI2CON1bits.DISSDO = 0; /*Data out enabled*/
    SPI2CON1bits.MODE16 = 1; /*16 bit mode*/
    SPI2CON1bits.CKE = 1; /*data out changes on falling edge*/
    SPI2CON1bits.CKP = 0; /*active high clock*/
    SPI2CON1bits.MSTEN = 1; /*master mode enable*/

    SPI2STATbits.SPIEN = 1; /*Enable SPI module*/
    SPI2STATbits.SISEL = 0b101; /*interupt on last bit out*/

    _SPI2IF = 0; // Clear the Interrupt flag
    _SPI2IE = 1; // Enable the interrupt
    _SI2C1IP = 5; //priority 5

    return 0;
}

uint8_t spiTransfer(uint8_t byte) {
    spi1Status = 0;
    SPI1BUF = byte;
    while (!spi1Status) {
        ;
    }
    return (uint8_t) SPI1BUF;
}

void spi1Write(uint16_t input) {
    SPI1BUF = input;
}

uint8_t spi1Ready(void) {
    if(spi1Status == 1){
        spi1Status = 0;
        return 1;
    }
    return 0;;
}

void spi2Write(uint16_t input) {
    SPI2BUF = input;
}

uint8_t spi2Ready(void) {
    return spi2Status;
}

void __attribute__((__interrupt__, auto_psv)) _SPI1Interrupt(void) {
    _SPI1IF = 0; /* Clear the Interrupt flag*/
    spi1Status = 1;
    uint16_t temp = SPI1BUF; /*clear input buffer because it just always fills up*/
    temp = 0; /*Suppress Warning*/
}

void __attribute__((__interrupt__, auto_psv)) _SPI2Interrupt(void) {
    _SPI2IF = 0; /* Clear the Interrupt flag*/
    spi2Status = 1;
    uint16_t temp = SPI2BUF; /*clear input buffer because it just always fills up*/
    temp = 0; /*Suppress Warning*/
}
