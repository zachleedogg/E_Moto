/* 
 * File:   bolt_uart
 * Author: Zachary Levenberg
 * Comments:
 * Revision history: 
 */


#ifndef BOLT_UART_H
#define	BOLT_UART_H
/*******************************************************************************
 * Include
 * ****************************************************************************/
#include <xc.h>
#include <stdint.h>
#include "bolt_pps.h"

/*******************************************************************************
 * Defines & Datatypes
 * ****************************************************************************/

/*Select a Baud Rate*/
#define UART_BAUD_110     110
#define UART_BAUD_300     300
#define UART_BAUD_600     600
#define UART_BAUD_1200    1200
#define UART_BAUD_2400    2400
#define UART_BAUD_4800    4800
#define UART_BAUD_9600    9600
#define UART_BAUD_14400   14400
#define UART_BAUD_19200   19200
#define UART_BAUD_28800   28800
#define UART_BAUD_38400   38400
#define UART_BAUD_56000   56000
#define UART_BAUD_57600   57600
#define UART_BAUD_115200  115200 
#define UART_BAUD_230400  230400 
#define UART_BAUD_460800  460800
#define UART_BAUD_921600  921600

/*******************************************************************************
 * Function Prototypes
 * ****************************************************************************/

/**
 * Initialize the UART1 module with pins assignments
 * @param TX_pin: use any RPxx or RPIxx pin from bolt_pps.h
 * @param RX_pin: use any RPIxx pin from bolt_pps.h
 * @param baudRate: use any define Baud rate
 * @return 
 */
#define Uart1INIT(TX_PIN, RX_PIN, BAUD) \
_U1RXR=RX_PIN; \
TX_PIN=1; \
Uart1Init(BAUD) \

uint8_t Uart1Init(uint32_t baudRate);

/**
 * Stops the UART1 module
 * @return none
 */
void Uart1Stop(void);

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
uint8_t Uart1Read(unsigned char* returnString);

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
uint8_t Uart2Init(uint32_t baudRate);

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

