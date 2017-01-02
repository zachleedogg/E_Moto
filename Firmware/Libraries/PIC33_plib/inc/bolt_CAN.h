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

/****************************************************************************************
 module defines
 ****************************************************************************************/

/* RX pin select */
#define CAN_RX_RP20    0x14
#define CAN_RX_RPI24   0x18
#define CAN_RX_RPI25   0x19
#define CAN_RX_RPI27   0x1B
#define CAN_RX_RPI28   0x1C
#define CAN_RX_RPI32   0x20
#define CAN_RX_RPI33   0x21
#define CAN_RX_RPI34   0x22
#define CAN_RX_RP35    0x23
#define CAN_RX_RP36    0x24
#define CAN_RX_RP37    0x25
#define CAN_RX_RP38    0x26
#define CAN_RX_RP39    0x27
#define CAN_RX_RP40    0x28
#define CAN_RX_RP41    0x29
#define CAN_RX_RP42    0x2A
#define CAN_RX_RP43    0x2B
#define CAN_RX_RPI44   0x2C
#define CAN_RX_RPI45   0x2D
#define CAN_RX_RPI46   0x2E
#define CAN_RX_RPI47   0x2F
#define CAN_RX_RPI51   0x33
#define CAN_RX_RPI52   0x34
#define CAN_RX_RPI53   0x35
#define CAN_RX_RP54    0x36
#define CAN_RX_RP55    0x37
#define CAN_RX_RP56    0x38
#define CAN_RX_RP57    0x39
#define CAN_RX_RPI58   0x3A

/* TX pin select */
typedef enum _CAN_txPinNumberg {
    CAN_TX_RP20,
    CAN_TX_RP35,
    CAN_TX_RP36,
    CAN_TX_RP37,
    CAN_TX_RP38,
    CAN_TX_RP39,
    CAN_TX_RP40,
    CAN_TX_RP41,
    CAN_TX_RP42,
    CAN_TX_RP43,
    CAN_TX_RP54,
    CAN_TX_RP55,
    CAN_TX_RP56,
    CAN_TX_RP57,
    CAN_NUMBER_OF_TX_PINS,
} CAN_txPinNumberg;

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
uint8_t CAN_init(CAN_txPinNumberg TXpin, uint16_t RXpin, uint32_t baud, uint8_t mode);

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
