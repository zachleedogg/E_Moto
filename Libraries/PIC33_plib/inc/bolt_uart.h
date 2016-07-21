/* 
 * File:   bolt_uart
 * Author: Zachary Levenberg
 * Comments:
 * Revision history: 
 */


#ifndef BOLT_UART_H
#define	BOLT_UART_H

#include <xc.h>
#include <stdint.h>


/*Select a Baud Rate*/
#define BAUD110     110
#define BAUD300     300
#define BAUD600     600
#define BAUD1200    1200
#define BAUD2400    2400
#define BAUD4800    4800
#define BAUD9600    9600
#define BAUD14400   14400
#define BAUD19200   19200
#define BAUD28800   28800
#define BAUD38400   38400
#define BAUD56000   56000
#define BAUD57600   57600
#define BAUD115200  115200 


/*Select Pin for RX*/
/*Not all of these are available on every device, check pin-out sheet*/
#define RP20_RX    0x14
#define RPI24_RX   0x18
#define RPI25_RX   0x19
#define RPI27_RX   0x1B
#define RPI28_RX   0x1C
#define RPI32_RX   0x20
#define RPI33_RX   0x21
#define RPI34_RX   0x22
#define RP35_RX    0x23
#define RP36_RX    0x24
#define RP37_RX    0x25
#define RP38_RX    0x26
#define RP39_RX    0x27
#define RP40_RX    0x28
#define RP41_RX    0x29
#define RP42_RX    0x2A
#define RP43_RX    0x2B
#define RPI44_RX   0x2C
#define RPI45_RX   0x2D
#define RPI46_RX   0x2E
#define RPI47_RX   0x2F
#define RPI51_RX   0x33
#define RPI52_RX   0x34
#define RPI53_RX   0x35
#define RP54_RX    0x36
#define RP55_RX    0x37
#define RP56_RX    0x38
#define RP57_RX    0x39
#define RPI58_RX   0x3A

/*Select pins for TX*/

/*Not all of these are available on every device, check pin-out sheet*/

typedef enum _tx_pin_number {
    RP20_TX,
    RP35_TX,
    RP36_TX,
    RP37_TX,
    RP38_TX,
    RP39_TX,
    RP40_TX,
    RP41_TX,
    RP42_TX,
    RP43_TX,
    RP54_TX,
    RP55_TX,
    RP56_TX,
    RP57_TX,
    NUMBER_OF_TX_PINS,
} tx_pin_number;


#define DELAY_105uS asm volatile ("REPEAT, #4201"); Nop(); // 105uS delay

/**
 * Initialize the UART1 module with pins assignments
 * @param TX_pin: use any RPxx or RPIxx pin
 * @param RX_pin: use any RPIxx pin
 * @param baudRate: use any define Baud rate
 * @return 
 */
uint8_t Uart1Init(tx_pin_number TX_pin, uint16_t RX_pin, uint32_t baudRate);

/**
 * 
 * @param inputString: string to send over UART: maximum 256 chars.
 * @param inputString: Always end add a newline to each string and NULL terminate.
 * @return success or failure
 */
uint8_t Uart1Write(char* inputString);

/**
 * 
 * @param returnString: string to place the read-in data
 * @return 
 */
uint8_t Uart1Read(char* returnString);

/**
 * checks if RX data is ready to be read out. based on newline char found.
 * @return 
 */
uint8_t Uart1RXdataReady(void);

/**
 * Checks if the UART TX module is in use
 * @return Busy (1) or Not Busy (0)
 */
uint8_t Uart1TXbusy(void);

/**
 * Initialize the UART1 module with pins assignments
 * @param TX_pin: use any RPxx or RPIxx pin
 * @param RX_pin: use any RPIxx pin
 * @param baudRate: use any define Baud rate
 * @return 
 */
uint8_t Uart2Init(tx_pin_number TX_pin, uint16_t RX_pin, uint32_t baudRate);

/**
 * 
 * @param inputString: string to send over UART: maximum 256 chars.
 * @param inputString: Always end add a newline to each string and NULL terminate.
 * @return success or failure
 */
uint8_t Uart2Write(char* inputString);

/**
 * 
 * @param returnString: string to place the read-in data
 * @return 
 */
uint8_t Uart2Read(char* returnString);

/**
 * checks if RX data is ready to be read out. based on newline char found.
 * @return 
 */
uint8_t Uart2RXdataReady(void);

/**
 * Checks if the UART TX module is in use
 * @return Busy (1) or Not Busy (0)
 */
uint8_t Uart2TXbusy(void);



#endif	/* BOLT_UART_H */

