#include "can_iso_tp_lite.h"
#include "can_iso_tp_lite_config.h"
#include "bolt_can.h"

#include "mcc_generated_files/boot/boot_config.h"
#include "IO.h"

typedef enum {
    SINGLE,
    FIRST,
    CONSECUTIVE,
    FLOW_CONTROL 
} frametype_E;

frametype_E currentFrameType = SINGLE;
uint16_t payloadSize = 0;
uint16_t payloadIndex = 0;
uint8_t payloadMessage[28];
uint8_t consecutiveFrame_SN = 0;

const uint8_t boot_app_size_request[] = {0x0B, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define MESSAGE_BUFFER_SIZE 8
uint8_t messageBufferIndex = 0;
uint8_t messageBufferPointer = 0;
void (*messageBuffer[MESSAGE_BUFFER_SIZE])(void);

uint8_t get_frame_payload(void);
void set_flow_control(void);
void decode_message(void);
void messageBufferPush(void * message);
uint8_t messageBufferPop(void);
static void Reset(void);

uint8_t run_iso_tp_basic(void){
    uint8_t messageComplete = 0;
    if (CAN_boot_host_checkDataIsFresh()){
        switch(CAN_boot_host_type_get()){
            case SINGLE:
                currentFrameType = SINGLE;
                payloadSize = CAN_boot_host_code_get();
                if (payloadSize == 0){
                    break;
                }
                payloadIndex = 0;
                messageComplete = get_frame_payload();
                break;
            case FIRST:
                currentFrameType = FIRST;
                payloadSize = (CAN_boot_host_code_get() << 8) + CAN_boot_host_getBytesFp[0]();
                if (payloadSize == 0){
                    break;
                }
                payloadIndex = 0;
                consecutiveFrame_SN = 1;
                messageComplete = get_frame_payload();
                messageBufferPush(set_flow_control);
                break;
            case CONSECUTIVE:
                currentFrameType = CONSECUTIVE;
                if (consecutiveFrame_SN++ != CAN_boot_host_code_get()){
                    consecutiveFrame_SN = 0;
                    break;
                }
                if(consecutiveFrame_SN > 15) {
                    consecutiveFrame_SN = 1;
                }
                messageComplete = get_frame_payload();
                break;
            default:
                break;
        }
        if (messageComplete){
            decode_message();
        }
    }
    //If there is anything in the payload buffer, send it.
    return messageBufferPop();
}

uint8_t get_frame_payload(void){
    uint8_t byteIndex = 0;
    if(currentFrameType == FIRST){
        byteIndex = 1;
    }    
    while (payloadIndex < payloadSize){
        payloadMessage[payloadIndex++] = CAN_boot_host_getBytesFp[byteIndex++]();
        if (byteIndex >= 7){
            break;
        }
    }
    if (payloadIndex == payloadSize) {
        return 1 & payloadSize;
    }
    return 0;
}

void set_flow_control(void){
    CAN_boot_response_type_set(FLOW_CONTROL);
    CAN_boot_response_code_set(0);
    CAN_boot_response_byte1_set(0);
    CAN_boot_response_byte2_set(1);
    CAN_boot_response_byte3_set(0x55);
    CAN_boot_response_byte4_set(0x55);
    CAN_boot_response_byte5_set(0x55);
    CAN_boot_response_byte6_set(0x55);
    CAN_boot_response_byte7_set(0x55);
    CAN_boot_response_dlc_set(3);
    CAN_boot_response_send();
}

void set_app_size_response_1(void){
    CAN_boot_response_type_set(FIRST);
    CAN_boot_response_code_set(0);
    CAN_boot_response_byte1_set(0x14);
    CAN_boot_response_byte2_set(0x0B);
    CAN_boot_response_byte3_set(0x08);
    CAN_boot_response_byte4_set(0x00);
    CAN_boot_response_byte5_set(0x00);
    CAN_boot_response_byte6_set(0x00);
    CAN_boot_response_byte7_set(0x00);
    CAN_boot_response_dlc_set(8);
    CAN_boot_response_send();
}

void set_app_size_response_2(void){
    CAN_boot_response_type_set(CONSECUTIVE);
    CAN_boot_response_code_set(1);
    CAN_boot_response_byte1_set(0x00);
    CAN_boot_response_byte2_set(0x00);
    CAN_boot_response_byte3_set(0x00);
    CAN_boot_response_byte4_set(0x00);
    CAN_boot_response_byte5_set(0x00);
    CAN_boot_response_byte6_set(0x01);
    CAN_boot_response_byte7_set(0x00);
    CAN_boot_response_send();
}

void set_app_size_response_3(void){
    CAN_boot_response_type_set(CONSECUTIVE);
    CAN_boot_response_code_set(2);
    CAN_boot_response_byte1_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_LOW>>8);
    CAN_boot_response_byte2_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_LOW & 0xFF);
    CAN_boot_response_byte3_set(0x00);
    CAN_boot_response_byte4_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_HIGH & 0xFF);
    CAN_boot_response_byte5_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_HIGH>>8 & 0xFF);
    CAN_boot_response_byte6_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_HIGH>>16 & 0xFF);
    CAN_boot_response_byte7_set(BOOT_CONFIG_PROGRAMMABLE_ADDRESS_HIGH>>24 & 0xFF);
    CAN_boot_response_send();
}


void decode_message(void){
    uint8_t i =0;
    for (i=0; i < payloadSize; i++){
        if(payloadMessage[i] != boot_app_size_request[i]){
            return;
        }
    }
    messageBufferPush(set_app_size_response_1);
    messageBufferPush(set_app_size_response_2);
    messageBufferPush(set_app_size_response_3);
    messageBufferPush(Reset);
}

void messageBufferPush(void * message){
    messageBuffer[messageBufferPointer++] = message;
    if (messageBufferPointer > MESSAGE_BUFFER_SIZE - 1){
        messageBufferPointer = 0;
    }
}

uint8_t messageBufferPop(void){
    if (messageBufferPointer != messageBufferIndex){
        messageBuffer[messageBufferIndex++]();
    }
    if (messageBufferIndex > MESSAGE_BUFFER_SIZE - 1){
        messageBufferIndex = 0;
    }
//    if(messageBufferPointer == Reset){
//        return 1;
//    }
    return 0;
}

static void Reset(void){
    CAN_changeOpMode(CAN_DISABLE);
//    IO_SET_SW_EN(LOW);
//    IO_SET_CHARGEPUMP_PWM(0);
//    uint16_t i = 0;
//    for (i=0; i<30000; i++){
//        Nop();
//    }
    asm ("reset");
}
