/* 
 * File: Bluetooth.h  
 * Author: Alexis Bartels
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BLUETOOTH_H
#define	BLUETOOTH_H

#include <xc.h> 
#include "Init.h"
//#include "freq.h"
#include "bolt_uart.h"
#include <stdio.h>

#define  BTLE_BUFF_SIZE 50 

#define CONNECTED 0
#define DISCONNECTED 1
#define CMD 2
#define AOK 3
#define WC 4
#define WV 5

#define NONE 0
#define BT_PASSWORD 1
#define BT_CONNECTION_START 2
#define BT_CONNECTION_END 3
#define BT_LIGHTS 4




/*
 Sends a value via UART to the RN4020 module
 * @param flag: The bluetooth flag value for the data being sent
 * @param value: The data being sent. Should be a float
 */
void BTLEWrite(unsigned char flag, float value);

uint8_t BTLEDataIsReady();

uint8_t BlueToothGetData(uint16_t* thisFlag, uint32_t* data);

uint8_t BTLEIsConnected();

void BTLEConnection(uint8_t value);


#endif	/* BLUETOOTH_H */

