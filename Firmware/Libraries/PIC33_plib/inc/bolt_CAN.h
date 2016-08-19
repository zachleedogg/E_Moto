/****************************************************************************************
 Module:
     boltCAN.h
 Purpose:
     Contains module defines and function prototypes for the CAN module's functions
 author: brdgordo
 
 History
 *************
 3/30/16: first write; brdgordo
 4/6/16: modularization; brdgordo
 5/15/16: fixed math error in module and app codes
 ****************************************************************************************/

#ifndef CAN_FUNC_H
#define CAN_FUNC_H

#include <xc.h>
#include <stdint.h>

/****************************************************************************************
 module defines
 ****************************************************************************************/

/* RX pin select */
#define RP20_CAN_RX    0x14
#define RPI24_CAN_RX   0x18
#define RPI25_CAN_RX   0x19
#define RPI27_CAN_RX   0x1B
#define RPI28_CAN_RX   0x1C
#define RPI32_CAN_RX   0x20
#define RPI33_CAN_RX   0x21
#define RPI34_CAN_RX   0x22
#define RP35_CAN_RX    0x23
#define RP36_CAN_RX    0x24
#define RP37_CAN_RX    0x25
#define RP38_CAN_RX    0x26
#define RP39_CAN_RX    0x27
#define RP40_CAN_RX    0x28
#define RP41_CAN_RX    0x29
#define RP42_CAN_RX    0x2A
#define RP43_CAN_RX    0x2B
#define RPI44_CAN_RX   0x2C
#define RPI45_CAN_RX   0x2D
#define RPI46_CAN_RX   0x2E
#define RPI47_CAN_RX   0x2F
#define RPI51_CAN_RX   0x33
#define RPI52_CAN_RX   0x34
#define RPI53_CAN_RX   0x35
#define RP54_CAN_RX    0x36
#define RP55_CAN_RX    0x37
#define RP56_CAN_RX    0x38
#define RP57_CAN_RX    0x39
#define RPI58_CAN_RX   0x3A

/* TX pin select */
typedef enum _CAN_tx_pin_number {
    RP20_CAN_TX,
    RP35_CAN_TX,
    RP36_CAN_TX,
    RP37_CAN_TX,
    RP38_CAN_TX,
    RP39_CAN_TX,
    RP40_CAN_TX,
    RP41_CAN_TX,
    RP42_CAN_TX,
    RP43_CAN_TX,
    RP54_CAN_TX,
    RP55_CAN_TX,
    RP56_CAN_TX,
    RP57_CAN_TX,
    NUMBER_OF_CAN_TX_PINS,
} CAN_tx_pin_number;

/* mode types */
#define NORMAL 0
#define LOOPBACK 2

/* Baud Rates */
#define CAN_BAUD_ONE_OVERF 0
#define CAN_BAUD_TWO_OVERF 1
#define CAN_BAUD_THREE_OVERF 2
#define CAN_BAUD_FOUR_OVERF 3
#define CAN_BAUD_EIGHT_OVERF 7
#define CAN_BAUD_SIXTEEN_OVERF 15
#define CAN_BAUD_THIRTYTWO_OVERF 31
#define CAN_BAUD_SIXTYFOUR_OVERF 63

/* module codes (maximum 0xF)*/
typedef enum _canModules {
    CAN_MODULE_MOTOR_CONTROLLER, /*MCU has hard-coded ID, so this must always be first*/
    CAN_MODULE_MCU,
    CAN_MODULE_BMS_1,
    CAN_MODULE_BMS_2,
    CAN_MODULE_DASH, /*Dash has highest priority, must always be last here...*/
} canModules;

/* application codes (maximum 0xF) */
typedef enum _canFlags {
    CAN_FLAG_GENERAL,
    CAN_FLAG_WAKEUP,
    CAN_FLAG_KILL,
    CAN_FLAG_IGNITION,
    CAN_FLAG_DRIVE_MODE,
    CAN_FLAG_SPEED,
    CAN_FLAG_TEL,
    CAN_FLAG_BATTERY_MSG_LEFT,
    CAN_FLAG_BATTERY_MSG_RIGHT,
    CAN_FLAG_VOLTAGE_MSG,
    CAN_FLAG_MOTOR_VOLTAGE_MSG,
    CAN_FLAG_MOTOR_CURRENT_MSG,
    CAN_FLAG_THREE_PHASE,
    CAN_FLAG_MOTOR_POWER_MSG,
    CAN_GBSALL_FLAG,
    CAN_FLAG_BATTERY_REQUEST,
    CAN_FLAG_CHARGER,
} canFlags;

#define CAN_CMD_KBL_SPEED 0x37
#define CAN_CMD_KBL_CURRENT_VOLTAGE 0x1a
#define CAN_CMD_KBL_OPERATION_VOLTAGE 0x1b
#define CAN_CMD_KBL_AVERAGE_VOLTAGE 0x33


/* receive return value masks */
#define MODULE_MSK 0x1e0
#define APP_MSK 0x1e
#define FAIL_MSK 0x1
#define MODULE_SHIFT 5
#define APP_SHIFT 1

/* This returns the CAN module type*/
#define getCANModule(x) ((x & MODULE_MSK)>>MODULE_SHIFT)

/*This returns the CAN app code type*/
#define getCANAppCode(x) ((x & APP_MSK)>>APP_SHIFT)
/******************************************************************************************
 Function Prototypes
 ******************************************************************************************/

/*
 function: CAN_PinSelect
 params: TXpin - the pin for CAN transmit
         RXpin - the pin for CAN receive
 return: success / failure
 */
uint8_t CAN_PinSelect(CAN_tx_pin_number TXpin, uint16_t RXpin);

/*
 function: CAN_Configure
 params: CAN mode (normal or loopback)
         baud rate
 return: success / failure
 */
uint8_t CAN_Configure(uint8_t mode, uint8_t baud);

/**
 * set the module ID as defined, ie, DASH, 
 * @param module
 */
void CAN_setModuleID(canModules thisModule);

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
uint8_t CAN_write(unsigned char * data);

/**
 * Send just a simple flag on the CANbus
 * @param thisFlag
 */
void CAN_sendFlag(canFlags thisFlag);

/**
 * Send a float value along with a flag
 * @param thisFlag
 * @param data
 */
void CAN_sendFloat(canFlags thisFlag, float data);

/**
 * Send a 16 bit unsigned int value with a floag
 * @param thisFlag
 * @param data
 */
void CAN_sendInt(canFlags thisFlag, uint16_t data);

/**
 * Reads a message and places the 8 bytes into a string.
 * @param RxMessage (an empty string to place the message into.)
 * @return the Module from which the message originated
 */
uint16_t CAN_Read(unsigned char * RxMessage);

/**
 * Gets a flag from a message
 * @param message (The message string that was called with CAN_Read)
 * @return 
 */
uint8_t CAN_getFlag(unsigned char *message);

/**
 * Gets a Float value from a message
 * @param message (The message string that was called with CAN_Read)
 * @return 
 */
float CAN_getFloat(unsigned char *message);

/**
 * Gets an INt value from a message
 * @param message (The message string that was called with CAN_Read)
 * @return 
 */
uint16_t CAN_getInt(unsigned char *message);

#endif /* CAN_FUNC_H */
