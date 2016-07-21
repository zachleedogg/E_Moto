
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
#include "bolt_i2c.h"
#include "bolt_uart.h"
#include "bolt_init.h"

#define DEBUG

#ifdef DEBUG
#define print(x) Uart1Write(x)
#else
#define print(x)
#endif

#define GOBBLE_DELAY .13
#define I2C_BUFFER_SIZE 32
#define I2C_QUEUE_SIZE 32


// *****************************************************************************
// *****************************************************************************
// Section: Variables
// *****************************************************************************
// *****************************************************************************

typedef enum _i2c_states {
    ADDRESSING,
    ACK_ADDRESS_HIGH,
    ACK_ADDRESS_LOW,
    COMMAND,
    RESTART,
    READ_REQUEST,
    READING,
    WRITING,
    END
} i2c_states;

static i2c_states i2cState = ADDRESSING;

typedef struct _transmission {
    uint8_t readWrite;
    uint8_t deviceAddress;
    uint8_t addressSpace;
    uint8_t addressHigh;
    uint8_t addressLow;
    char TX[I2C_BUFFER_SIZE];
    char *RX;
    uint8_t numberOfBytes;
} transmission;

static transmission thisTransmission;

typedef struct _i2cQueue {
    transmission Queue[I2C_QUEUE_SIZE];
    uint8_t queueHead;
    uint8_t queueTail;
} i2cQueue;

static i2cQueue __attribute__ ((unused)) i2c1Queue ;
static i2cQueue i2c2Queue;

static uint8_t index;
static uint8_t i2cReadDone;

// *****************************************************************************
// *****************************************************************************
// Section: Prototypes
// *****************************************************************************
// *****************************************************************************

/* Enable the Module*/
#define ON 1
#define OFF 0
#define I2C2(x) I2C2CONbits.I2CEN=x

/* Send Acknowledge bit*/
#define NACK 1
#define ACK 0
#define SEND_ACK2(x) I2C2CONbits.ACKDT=x;I2C2CONbits.ACKEN=1

/* Receive Data*/
#define GET_DATA2() I2C2CONbits.RCEN=1

/*Start condition*/
#define START2() I2C2CONbits.SEN=1

/*Stop condition*/
#define STOP2() I2C2CONbits.PEN=1

/*Repeat start condition*/
#define REP_START2() I2C2CONbits.RSEN=1;

uint8_t i2cInit(float baudRate) {
    //I2C Config 1 I2C2CONbits
    I2C2CONbits.I2CEN = 0;


    /*Config Baud rate*/
    double tempBRG = baudRate - GOBBLE_DELAY;
    tempBRG = tempBRG * ((double) clockFreq() / 2.0);
    tempBRG /= 1000000;
    tempBRG = tempBRG - 2;

    I2C2BRG = (uint16_t) (tempBRG + .5);
    //I2C2BRG = 17; //(uint16_t)tempBRG;

    I2C2TRN = 0x00;
    I2C2RCV = 0x00;

    /*Enable Interrupt*/
    _MI2C2IP = 7; /*priority*/
    _MI2C2IE = 1; /*enable*/
    //_MI2C2IF = 0;

    I2C2(ON);

    return 0;
}

uint8_t i2cSetDeviceID(uint8_t deviceID) {
    thisTransmission.deviceAddress = deviceID;
    return 0;
}

uint8_t i2cSetAddress(uint16_t address, uint8_t addressSpace) {
    if (addressSpace == ADDRESS_SPACE_8_BIT) {
        thisTransmission.addressHigh = (uint8_t) address;
    } else {
        thisTransmission.addressHigh = (uint8_t) (address >> 8);
        thisTransmission.addressLow = (uint8_t) (address);
    }
    thisTransmission.addressSpace = addressSpace;
    return 0;
}

uint8_t i2cWrite(char *message, uint8_t numberOfBytes) {
    int i = 0;
    for (i = 0; i < numberOfBytes; i++) {
        thisTransmission.TX[i] = message[i];
    }
    thisTransmission.readWrite = WRITE;
    thisTransmission.numberOfBytes = numberOfBytes;
    i2c2Queue.Queue[i2c2Queue.queueHead] = thisTransmission;
    i2c2Queue.queueHead++;
    if (i2c2Queue.queueHead == I2C_QUEUE_SIZE) {
        i2c2Queue.queueHead = 0;
    }
    START2(); /*Send start bit*/
    i2cState = ADDRESSING;
    return 0;
}

uint8_t i2cRead(char *message, uint8_t numberOfBytes) {
    thisTransmission.numberOfBytes = numberOfBytes;
    thisTransmission.readWrite = READ;
    thisTransmission.RX = message;
    i2c2Queue.Queue[i2c2Queue.queueHead] = thisTransmission;
    i2c2Queue.queueHead++;
    if (i2c2Queue.queueHead == I2C_QUEUE_SIZE) {
        i2c2Queue.queueHead = 0;
    }
    START2(); /*Send start bit*/
    i2cState = ADDRESSING;
    return 0;
}

uint8_t i2cReadReady(void) {
    uint8_t returnVal = 0;
    if (i2cReadDone) {
        i2cReadDone = 0;
        returnVal = 1;
    }
    return returnVal;
}

void __attribute__((__interrupt__, __auto_psv__)) _MI2C2Interrupt(void) {
    _MI2C2IF = 0;
    print("Int i2c\n");
    switch (i2cState) {
        case ADDRESSING:      
            if (I2C2STATbits.S) {/*If start bit has been sent*/
                /*send the address command with ~WRITE indication*/
                index = 0;
                uint8_t temp = (i2c2Queue.Queue[i2c2Queue.queueTail].deviceAddress << 1) | WRITE;
                I2C2TRN = temp;
                if (I2C2STATbits.TRSTAT) {
                    print("Device ID Sent\n");
                }
                i2cState = ACK_ADDRESS_HIGH;
            } else if (i2c2Queue.queueTail != i2c2Queue.queueHead) {
                print("Next Job...\n");
                START2(); /*Send start bit*/
            }
            break;
        case ACK_ADDRESS_HIGH:
            if (I2C2STATbits.ACKSTAT == ACK) {/*If ACK received*/
                /*send the command*/
                I2C2TRN = i2c2Queue.Queue[i2c2Queue.queueTail].addressHigh;
                if (i2c2Queue.Queue[i2c2Queue.queueTail].addressSpace == ADDRESS_SPACE_8_BIT) {
                    if (i2c2Queue.Queue[i2c2Queue.queueTail].readWrite == READ) {
                        print("READ\n");
                        i2cState = RESTART;
                    } else { // ==WRITE
                        print("WRITE\n");
                        i2cState = WRITING;
                    }

                } else {
                    i2cState = ACK_ADDRESS_LOW;
                }
                print("ack\n");
                print("sending address high\n");
            }
            break;

        case ACK_ADDRESS_LOW:
            if (I2C2STATbits.ACKSTAT == ACK) {/*If ACK received*/
                /*send the command*/
                I2C2TRN = i2c2Queue.Queue[i2c2Queue.queueTail].addressLow;
                if (i2c2Queue.Queue[i2c2Queue.queueTail].readWrite == READ) {
                    i2cState = RESTART;
                } else { // ==WRITE
                    i2cState = WRITING;
                }
                print("ack\n");
                print("sending address low\n");
            }
            break;

        case RESTART:
            if (I2C2STATbits.ACKSTAT == ACK) {/*If ACK received*/
                print("ack\n");
                REP_START2(); /*send restart*/
                print("restarting\n");
                i2cState = READ_REQUEST;
            }
            break;
        case READ_REQUEST:
            if (I2C2STATbits.S) {/*if restart is completed*/
                /*Send address*/
                print("sending read address\n");
                I2C2TRN = (i2c2Queue.Queue[i2c2Queue.queueTail].deviceAddress << 1) | READ;
                i2cState = READING;
            }
            break;

        case READING:
            if (I2C2STATbits.RBF) {/*If byte received*/
                //print("byte rcvd\n");
                i2c2Queue.Queue[i2c2Queue.queueTail].RX[index++] = I2C2RCV; /*get the data*/
                if (i2c2Queue.Queue[i2c2Queue.queueTail].numberOfBytes == index) {/*If no more bytes to receive, Stop transmission*/
                    i2c2Queue.Queue[i2c2Queue.queueTail].RX[index] = 0; /*set last item NULL*/
                    index = 0;
                    print("sending NACK\n");
                    SEND_ACK2(NACK);
                    i2cState = END;
                    break;
                } else { /*keep getting data*/
                    print("get another byte\n");
                    SEND_ACK2(ACK);
                    break;
                }
            }
            if (I2C2STATbits.ACKSTAT == ACK) {
                print("ack Read request\n");
                GET_DATA2();
            }
            break;

        case WRITING:
            if (I2C2STATbits.ACKSTAT == ACK) {/*If ACK received, send data.*/
                I2C2TRN = i2c2Queue.Queue[i2c2Queue.queueTail].TX[index++]; /*send the data*/
                print("writing Data\n");
                if (i2c2Queue.Queue[i2c2Queue.queueTail].numberOfBytes == index) {/*If no more bytes to send, Stop transmission*/
                    i2cState = END;
                    print("Done Writing\n");
                }
            }
            break;

        case END:
            /*End transmission*/
            print("end transmission\n");
            STOP2();
            if (i2c2Queue.Queue[i2c2Queue.queueTail].readWrite == READ) {
                i2cReadDone = 1;
            }
            i2c2Queue.queueTail++;
            if (i2c2Queue.queueTail == I2C_QUEUE_SIZE) {
                i2c2Queue.queueTail = 0;
            }

            i2cState = ADDRESSING;
            break;
        default:
            break;

    }

}
