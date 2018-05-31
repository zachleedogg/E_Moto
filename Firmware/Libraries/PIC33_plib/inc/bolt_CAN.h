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

typedef struct {
   uint16_t word0;
   uint16_t word1;
   uint16_t word2;
   uint16_t word3;
} CAN_payload_S;

typedef struct CAN_message_S {
    uint16_t canID;
    volatile CAN_payload_S * payload;
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
 * CAN_configureMailbox configures mailboxes 0-15 to accept an exact ID match
 * This can be called no more than 16 times, when the processor runs out of mailboxes
 * Once configured, the returned struct will always have up-to-date data packets
 * @param newMessage: A pointer to a CAN Message struct
 * @return Success or Failure
 */
uint8_t CAN_configureMailbox(CAN_message_S * newMessage);

/*
 function: CAM_RxDataIsReady
 params: none
 return: 0 - Rx data not ready
         1 - Rx data ready
 */
uint8_t CAN_RxDataIsReady();

/**
 * Writes a message to the CANbus.
 * parmas: Data is a CAN_message_S object
 * @return 
 */
uint8_t CAN_write(CAN_message_S data);


#endif /* CAN_FUNC_H */
