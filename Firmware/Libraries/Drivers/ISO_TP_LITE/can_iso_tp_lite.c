#include "can_iso_tp_lite.h"
#include "can_iso_tp_lite_config.h"
#include "bolt_can.h"
#include "ringBuffer.h"
#include "mcc_generated_files/boot/boot_config.h"
#include "IO.h"

typedef enum {
    SINGLE,
    FIRST,
    CONSECUTIVE,
    FLOW_CONTROL
} frametype_E;

typedef enum {
    CONTINUE_TO_SEND,
    WAIT,
    OVERFLOW_ABORT
} flowcontrol_E;

#define BLOCK_SIZE 0
#define SEPARATION_TIME 2

frametype_E currentFrameType = SINGLE;
uint16_t payloadSize = 0;
uint16_t payloadIndex = 0;
uint8_t payloadMessage[64];
uint8_t consecutiveFrame_SN = 0;
isoTP_command_E currentCommand = ISO_TP_NONE;
uint16_t timeoutCounter = 0;
#define ISO_TP_TIMOUT 10

#define MESSAGE_BUFFER_SIZE 8
uint8_t messageBufferIndex = 0;
uint8_t messageBufferPointer = 0;
uint8_t(*messageBuffer[MESSAGE_BUFFER_SIZE])(void);

uint8_t get_frame_payload(void);
uint8_t set_flow_control(void);
void decode_message(void);
void messageBufferPush(void * message);
uint8_t messageBufferPop(void);
uint8_t isoTP_Reset(void);
uint8_t isoTP_Sleep(void);
uint8_t isoTP_TesterPresent(void);

void isoTP_init(void){
    currentFrameType = SINGLE;
    payloadSize = 0;
    payloadIndex = 0;
    consecutiveFrame_SN = 0;
    currentCommand = ISO_TP_NONE;
    timeoutCounter = 0;

    messageBufferIndex = 0;
    messageBufferPointer = 0;
}

uint8_t run_iso_tp_basic(void) {
    uint8_t messageComplete = 0;
    
    if (timeoutCounter++ > ISO_TP_TIMOUT) {
        isoTP_init();
    }

    if (CAN_boot_host_checkDataIsFresh()) {
        timeoutCounter = 0;
        //Switch statement based on bits 7...4 of the first byte in the CAN message
        uint16_t code = CAN_boot_host_code_get();
        switch (code) {
            case SINGLE:
                currentFrameType = SINGLE;
                //payload side here is from the 3...0 bits of the first byte.
                payloadSize = CAN_boot_host_type_get();
                if (payloadSize == 0) {
                    break;
                }
                payloadIndex = 0;
                messageComplete = get_frame_payload();
                break;
            case FIRST:
                currentFrameType = FIRST;
                payloadSize = (CAN_boot_host_type_get() << 8) | CAN_boot_host_getBytesFp[0]();
                if (payloadSize == 0) {
                    break;
                }
                payloadIndex = 0;
                consecutiveFrame_SN = 1;
                messageComplete = get_frame_payload();
                messageBufferPush(set_flow_control);
                break;
            case CONSECUTIVE:
                currentFrameType = CONSECUTIVE;
                if (consecutiveFrame_SN != CAN_boot_host_type_get()) {
                    consecutiveFrame_SN = 0;
                    break;
                }
                consecutiveFrame_SN++;
                if (consecutiveFrame_SN > 15) {
                    consecutiveFrame_SN = 0;
                }
                messageComplete = get_frame_payload();
                break;
            default:
                break;
        }
        if (messageComplete) {
            decode_message();
        }
    }
    //If there is anything in the payload buffer, send it.
    return messageBufferPop();
}

uint8_t isoTP_getCommand(void) {
    isoTP_command_E command = currentCommand;
    return command;
}

uint8_t * get_payload(void) {
    return payloadMessage;
}

uint8_t get_payloadLength(void) {
    return payloadSize;
}

uint8_t set_flow_control(void) {
    CAN_boot_response_type_set(0);
    CAN_boot_response_code_set(FLOW_CONTROL);
    CAN_boot_response_byte1_set(BLOCK_SIZE);
    CAN_boot_response_byte2_set(SEPARATION_TIME);
    CAN_boot_response_byte3_set(0x55);
    CAN_boot_response_byte4_set(0x55);
    CAN_boot_response_byte5_set(0x55);
    CAN_boot_response_byte6_set(0x55);
    CAN_boot_response_byte7_set(0x55);
    CAN_boot_response_dlc_set(3);
    CAN_boot_response_send();
    currentCommand = ISO_TP_NONE;
    return currentCommand;
}

uint8_t set_app_size_response_1(void) {
    CAN_boot_response_type_set(0);
    CAN_boot_response_code_set(FIRST);
    CAN_boot_response_byte1_set(0x14);
    CAN_boot_response_byte2_set(0x0B);
    CAN_boot_response_byte3_set(0x08);
    CAN_boot_response_byte4_set(0x00);
    CAN_boot_response_byte5_set(0x00);
    CAN_boot_response_byte6_set(0x00);
    CAN_boot_response_byte7_set(0x00);
    CAN_boot_response_dlc_set(8);
    CAN_boot_response_send();
    currentCommand = ISO_TP_NONE;
    return currentCommand;
}

uint8_t set_app_size_response_2(void) {
    CAN_boot_response_type_set(1);
    CAN_boot_response_code_set(CONSECUTIVE);
    CAN_boot_response_byte1_set(0x00);
    CAN_boot_response_byte2_set(0x00);
    CAN_boot_response_byte3_set(0x00);
    CAN_boot_response_byte4_set(0x00);
    CAN_boot_response_byte5_set(0x00);
    CAN_boot_response_byte6_set(0x01);
    CAN_boot_response_byte7_set(0x00);
    CAN_boot_response_send();
    currentCommand = ISO_TP_NONE;
    return currentCommand;
}

uint8_t set_app_size_response_3(void) {
    CAN_boot_response_type_set(2);
    CAN_boot_response_code_set(CONSECUTIVE);
    CAN_boot_response_byte1_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_LOW >> 8);
    CAN_boot_response_byte2_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_LOW & 0xFF);
    CAN_boot_response_byte3_set(0x00);
    CAN_boot_response_byte4_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_HIGH & 0xFF);
    CAN_boot_response_byte5_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_HIGH >> 8 & 0xFF);
    CAN_boot_response_byte6_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_HIGH >> 16 & 0xFF);
    CAN_boot_response_byte7_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_HIGH >> 24 & 0xFF);
    CAN_boot_response_send();
    currentCommand = ISO_TP_NONE;
    return currentCommand;
}

uint8_t isoTP_Reset(void) {
    currentCommand = ISO_TP_RESET;
    asm ("reset");
    return currentCommand;
}

uint8_t isoTP_Sleep(void) {
    currentCommand = ISO_TP_SLEEP;
    return currentCommand;
}

uint8_t isoTP_IOControl(void) {
    CAN_boot_response_type_set(7);
    CAN_boot_response_code_set(SINGLE);
    CAN_boot_response_byte1_set(0x00);
    CAN_boot_response_byte2_set(0x01);
    CAN_boot_response_byte3_set(0x02);
    CAN_boot_response_byte4_set(0x03);
    CAN_boot_response_byte5_set(0x04);
    CAN_boot_response_byte6_set(0x05);
    CAN_boot_response_byte7_set(0x06);
    CAN_boot_response_send();
    currentCommand = ISO_TP_IO_CONTROL;
    return currentCommand;
}

uint8_t isoTP_TesterPresent(void) {
    CAN_boot_response_type_set(7);
    CAN_boot_response_code_set(SINGLE);
    CAN_boot_response_byte1_set(0x0A);
    CAN_boot_response_byte2_set(0x0B);
    CAN_boot_response_byte3_set(0x0C);
    CAN_boot_response_byte4_set(0x0D);
    CAN_boot_response_byte5_set(0x0E);
    CAN_boot_response_byte6_set(0x0F);
    CAN_boot_response_byte7_set(0x0F);
    CAN_boot_response_send();
    currentCommand = ISO_TP_TESTER_PRESENT;
    return currentCommand;
}

uint8_t get_frame_payload(void) {
    uint8_t byteIndex = 0;
    if (currentFrameType == FIRST) {
        byteIndex = 1;
    }
    while (payloadIndex < payloadSize) {
        payloadMessage[payloadIndex++] = CAN_boot_host_getBytesFp[byteIndex++]();
        if (byteIndex >= 7) {
            break;
        }
    }
    if (payloadIndex == payloadSize) {
        return 1;
    }
    return 0;
}

void decode_message(void) {

    if (payloadMessage[0] == 0x0B) {
        messageBufferPush(set_app_size_response_1);
        messageBufferPush(set_app_size_response_2);
        messageBufferPush(set_app_size_response_3);
        messageBufferPush(isoTP_Reset);
    }
    if (payloadMessage[0] == 0x0A) {
        messageBufferPush(isoTP_Sleep);
    }
    if (payloadMessage[0] == 0x0C) {
        messageBufferPush(isoTP_IOControl);
    }
    if (payloadMessage[0] == 0x0D) {
        messageBufferPush(isoTP_TesterPresent);
    }

}

void messageBufferPush(void * message) {
    messageBuffer[messageBufferPointer++] = message;
    if (messageBufferPointer > MESSAGE_BUFFER_SIZE - 1) {
        messageBufferPointer = 0;
    }
}

uint8_t messageBufferPop(void) {
    uint8_t returnVal = 0;
    if (messageBufferPointer != messageBufferIndex) {
        returnVal = messageBuffer[messageBufferIndex++]();
    }
    if (messageBufferIndex > MESSAGE_BUFFER_SIZE - 1) {
        messageBufferIndex = 0;
    }

    return returnVal;
}