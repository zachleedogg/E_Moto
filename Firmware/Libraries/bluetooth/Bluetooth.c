/*
 * File:   Bluetooth.c
 * Author: abartels
 *
 * Created on April 7, 2016, 5:24 PM
 */


#include <xc.h>
#include <string.h>
#include "Bluetooth.h"
#include "bolt_uart.h"
#include "flags.h"

static char BTBuffer[BTLE_BUFF_SIZE] = {};

static uint8_t BTisConnected = 0;

void BTLEWrite(unsigned char flag, float value) {
    if (BTisConnected) {
        char buffer[256];
        sprintf(buffer, "SUW,2A1C,%02x%08lx\r\n", flag, *(unsigned long *) &value);
        //Uart1Write(buffer);
        Uart2Write(buffer);
    }
}

uint8_t BTLEDataIsReady() {
    if (Uart2RXdataReady()) {
        return 1;
    }
    return 0;
}

void BTLESleep(){
    Uart2Write("K\r\n");
}

uint8_t BlueToothGetData(uint16_t* thisFlag, uint32_t* data) {
    uint8_t returnVal = 0;
    /*read data from Uart*/
    Uart2Read(BTBuffer);
    //Uart1Write(BTBuffer);
    /*Check for a command or specific message from BT module*/
    int i = 0;
    char temp[30] = {};
    while ((BTBuffer[i] != ' ') && (BTBuffer[i] != ',') && (BTBuffer[i] != '\r') && (BTBuffer[i] != ':')) {
        temp[i] = BTBuffer[i];
        i++;
    }
    temp[i] = 0;
    //Uart1Write("the parsed value is: ");
    //Uart1Write(temp);
    if (strcmp(temp, "Connected") == 0) {/*Connection is made*/
        //BTisConnected = 1;
        returnVal = 1;
        //Uart1Write("Connected\n");
        return returnVal;
    } else if (strcmp(temp, "Connection") == 0) {/*Connection End*/
        //BTisConnected = 0;
        returnVal = 1;
        //Uart1Write("Connection End\n");
        return returnVal;
    } else if (strcmp(temp, "WC") == 0) {
        //Uart1Write("WC\n");
        //BTisConnected = 1;
        returnVal = 1;
    } else if (strcmp(temp, "WV") == 0) {
        //Uart1Write("WV\n");
        /*Parse for Command code and Data message*/
        float thisVal = 0;
        uint32_t thisHex = 0;
        uint16_t thisHex2 = 0;
        float *floatptr2;
        uint8_t tryToScan = 0;
        tryToScan = sscanf(&BTBuffer[i+6], "%4d%4X%4X",thisFlag, (uint16_t*) & thisHex, &thisHex2); //%2s%*c%*5c
        //Uart1Write(&BTBuffer[i+6]);
        /*If succesful, extract data from HEX to float*/
        if (tryToScan == 3) {
            /*Hex to float*/
            thisHex = thisHex << 16;
            thisHex |= thisHex2;
            floatptr2 = (float*) (&thisHex); /*pointer-cast int to float*/
            thisVal = *floatptr2;
            *data = (uint32_t) thisVal;
            returnVal = 1;
        }
    }
    return returnVal;
}

uint8_t BTLEIsConnected() {
    return BTisConnected;
}

void BTLEConnection(uint8_t value){
    BTisConnected = value;
}