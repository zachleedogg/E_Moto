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
#define UART_RX_RP20    0x14
#define UART_RX_RPI24   0x18
#define UART_RX_RPI25   0x19
#define UART_RX_RPI27   0x1B
#define UART_RX_RPI28   0x1C
#define UART_RX_RPI32   0x20
#define UART_RX_RPI33   0x21
#define UART_RX_RPI34   0x22
#define UART_RX_RP35    0x23
#define UART_RX_RP36    0x24
#define UART_RX_RP37    0x25
#define UART_RX_RP38    0x26
#define UART_RX_RP39    0x27
#define UART_RX_RP40    0x28
#define UART_RX_RP41    0x29
#define UART_RX_RP42    0x2A
#define UART_RX_RP43    0x2B
#define UART_RX_RPI44   0x2C
#define UART_RX_RPI45   0x2D
#define UART_RX_RPI46   0x2E
#define UART_RX_RPI47   0x2F
#define UART_RX_RPI51   0x33
#define UART_RX_RPI52   0x34
#define UART_RX_RPI53   0x35
#define UART_RX_RP54    0x36
#define UART_RX_RP55    0x37
#define UART_RX_RP56    0x38
#define UART_RX_RP57    0x39
#define UART_RX_RPI58   0x3A

/*Select pins for TX*/

/*Not all of these are available on every device, check pin-out sheet*/

typedef enum _UART_tx_pin_number {
    UART_TX_RP20,
    UART_TX_RP35,
    UART_TX_RP36,
    UART_TX_RP37,
    UART_TX_RP38,
    UART_TX_RP39,
    UART_TX_RP40,
    UART_TX_RP41,
    UART_TX_RP42,
    UART_TX_RP43,
    UART_TX_RP54,
    UART_TX_RP55,
    UART_TX_RP56,
    UART_TX_RP57,
    NUMBER_OF_TX_PINS,
} UART_tx_pin_number;


#define DELAY_105uS asm volatile ("REPEAT, #4201"); Nop(); // 105uS delay

/**
 * Initialize the UART1 module with pins assignments
 * @param TX_pin: use any RPxx or RPIxx pin
 * @param RX_pin: use any RPIxx pin
 * @param baudRate: use any define Baud rate
 * @return 
 */
uint8_t Uart1Init(UART_tx_pin_number TX_pin, uint16_t RX_pin, uint32_t baudRate);

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
uint8_t Uart2Init(UART_tx_pin_number TX_pin, uint16_t RX_pin, uint32_t baudRate);

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

