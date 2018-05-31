/****************************************************************************************
 Module:
     bolt_CAN.h
 Purpose:
     Contains module defines and function prototypes for the CAN module's functions
 author: Zlevenberg
 
 History
 *************
 3/30/16: first write; 
 ****************************************************************************************/

#ifndef CAN_H
#define CAN_H

#include <xc.h>
#include <stdint.h>
#include "bolt_pps.h"

/****************************************************************************************
 module defines
 ****************************************************************************************/

typedef enum {
    CAN_RX_MAILBOX1 = 8,
    CAN_RX_MAILBOX2,
    CAN_RX_MAILBOX3,
    CAN_RX_MAILBOX4,
    CAN_RX_MAILBOX5,
    CAN_RX_MAILBOX6,
    CAN_RX_MAILBOX7,
    CAN_RX_MAILBOX8,
    CAN_RX_MAILBOX9,
    CAN_RX_MAILBOX10,
    CAN_RX_MAILBOX11,
    CAN_RX_MAILBOX12,
    CAN_RX_MAILBOX13,
    CAN_RX_MAILBOX14,
    CAN_RX_MAILBOX15,
    CAN_RX_MAILBOX16,
} CAN_RX_MAILBOXES_E;

typedef struct CAN_message_S {
    uint8_t nodeID;
    uint8_t messageID;
    uint8_t frequency;
    uint16_t word0;
    uint16_t word1;
    uint16_t word2;
    uint16_t word3;
} CAN_message_S;

/* mode types */
#define CAN_NORMAL 0
#define CAN_LOOPBACK 2

/* Baud Rates */
#define CAN_BAUD_125k 125000
#define CAN_BAUD_500k 500000
#define CAN_BAUD_1M 1000000

/* receive return value masks */
#define CAN_NODE_MASK 0x0080
#define CAN_NODE_SHIFT 4
#define CAN_MESSAGE_MASK 0x0008

#define CAN_getCanIDFromMessage(x) (x>>2)

#define CAN_getFreqFromCanID(x) ((x & 0x0700))
/* This returns the CAN node type*/
#define CAN_getNodeFromCanID(x) ((x & CAN_NODE_MASK)>>CAN_NODE_SHIFT)

/*This returns the CAN massage type*/
#define CAN_getMessageFromCanID(x) (x & CAN_MESSAGE_MASK)
/******************************************************************************************
 Function Prototypes
 ******************************************************************************************/

/*
 function: CAN_init
 params: CAN mode (normal or loopback)
         baud rate
 return: success / failure
 */
#define CAN_Init(TX_PIN, RX_PIN, BAUD, MODE) \
_C1RXR=RX_PIN; \
TX_PIN=14; \
CAN_init(BAUD, MODE) \

uint8_t CAN_init(uint32_t baud, uint8_t mode);

/**
 * CAN_configureMailbox configures mailboxes 8-15 to accept an exact ID match
 * @param mBoxNumber
 * @param CAN_ID
 * @return 
 */
uint8_t CAN_configureMailbox(uint8_t mBoxNumber, uint16_t CAN_ID);

/*
 function: CAM_RxDataIsReady
 params: none
 return: 0 - Rx data not ready
         1 - Rx data ready
 */
uint8_t CAN_RxDataIsReady();

/**
 * Writes an 8 byte messegae to the CANbus. data[0] must be a flag
 * @param data (is an 8 byte string)
 * @return 
 */
uint8_t CAN_write(CAN_message_S * data);

/**
 * Reads a message and places the 8 bytes into a string.
 * @param RxMessage (an empty string to place the message into.)
 * @return the Module from which the message originated
 */
uint16_t CAN_Read(CAN_message_S * data, uint8_t RXBufferNumber);



#endif /* CAN_FUNC_H */
