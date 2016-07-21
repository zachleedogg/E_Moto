/* 
 * File:   bolt_spi
 * Author: Zach Levenberg
 * Comments: 
 * Revision history: initial build 2/9/16
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BOLT_SPI_H
#define	BOLT_SPI_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>
#include "Init.h"

#define SPI_4MHZ FREQ

/*Select Pin for D_IN*/
/*Not all of these are available on every device, check pin-out sheet*/
#define RP20_D_IN    0x14
#define RPI24_D_IN   0x18
#define RPI25_D_IN   0x19
#define RPI27_D_IN   0x1B
#define RPI28_D_IN   0x1C
#define RPI32_D_IN   0x20
#define RPI33_D_IN   0x21
#define RPI34_D_IN   0x22
#define RP35_D_IN    0x23
#define RP36_D_IN    0x24
#define RP37_D_IN    0x25
#define RP38_D_IN    0x26
#define RP39_D_IN    0x27
#define RP40_D_IN    0x28
#define RP41_D_IN    0x29
#define RP42_D_IN    0x2A
#define RP43_D_IN    0x2B
#define RPI44_D_IN   0x2C
#define RPI45_D_IN   0x2D
#define RPI46_D_IN   0x2E
#define RPI47_D_IN   0x2F
#define RPI51_D_IN   0x33
#define RPI52_D_IN   0x34
#define RPI53_D_IN   0x35
#define RP54_D_IN    0x36
#define RP55_D_IN    0x37
#define RP56_D_IN    0x38
#define RP57_D_IN    0x39
#define RPI58_D_IN   0x3A

/*Select pins for SPI CLK, D_OUT, and SSX*/
/*Not all of these are available on every device, check pin-out sheet*/

typedef enum _spi_pin_number {
RP20_SPI,
RP35_SPI,
RP36_SPI,
RP37_SPI,
RP38_SPI,
RP39_SPI,
RP40_SPI,
RP41_SPI,
RP42_SPI,
RP43_SPI,
RP54_SPI,
RP55_SPI,
RP56_SPI,
RP57_SPI,
NUMBER_OF_SPI_PINS,
} spi_pin_number;

/**
 * Initailize the SPI module 1. The pins are NOT remappable, you must use
 * SCK1, SDO1, SDI1, and SS1. At this time only SCK1 and SDO1 and SDI1 are active.
 */
void spi1Init(void);

/**
 * @function: spiTransfer, this is a blocking function that returns the value
 * shifted into the DIN
 * @param byte
 * @return 
 */
uint8_t spiTransfer(uint8_t byte);

/**
 * @function: spiWrite
 * @param moduleNumber: Select SPI 1 or 2
 * @param input: 8 or 16 bit word to transmit
 */
void spi1Write(uint16_t input);

/**
 * @function: spiReady check if the module is ready to send
 */
uint8_t spi1Ready(void);

/**
 * @function: spi2Init
 * @param CLK: choose the CLOCK output from the SPI pins above
 * @param D_OUT: choose the DATA_OUT from the SPI pins above
 * @return: success or failure, check for valid pins
 */
uint8_t spi2Init(spi_pin_number CLK, spi_pin_number D_OUT);

/**
 * @function: spiWrite
 * @param input: 8 or 16 bit word to transmit
 */
void spi2Write(uint16_t input);

/**
 * @function: spiReady check if the module is ready to send
 */
uint8_t spi2Ready(void);

#endif	/* BOLT_SPI_H */

