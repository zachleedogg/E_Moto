/* 
 * File:   CAN_DBC.h
 * Author: Zachary S. Levenberg
 * Created on November 6, 2016, 12:51 PM
 */

#ifndef CAN_DBC_H
#define	CAN_DBC_H

#include <stdint.h>

/** CAN ID FORMAT
 * 11-bit ID
 * 0b fff nnnn mmmm
 * 
 * f = Frequency where
 *      000 = 1ms
 *      001 = 10ms
 *      010 = 100ms
 *      011 = 1000ms
 *      100 = Reserved
 *      101 = Reserved
 *      110 = UDS Response
 *      111 = UDS Command
 * 
 * n = node enum
 *
 * m = message enum
 */

#define NODE_STRUCT(X) CAN_##X##_S

#define CAN_DEFAULT_SCALE 1
#define CAN_DEAULT_OFFSET 0

/*Singals*/
#define CAN_NODE_0 DASH
#define CAN_NODE_0_MSG_0 STATUS
#define CAN_NODE_0_MSG_0_SIGNAL_0 STATE
#define CAN_NODE_0_MSG_0_SIGNAL_0_BIT_LENGTH 3
#define CAN_NODE_0_MSG_0_SIGNAL_0_SCALE CAN_DEFAULT_SCALE
#define CAN_NODE_0_MSG_0_SIGNAL_0_OFFSET CAN_DEAULT_OFFSET
#define CAN_NODE_0_MSG_0_SINGAL_0_VALUE CAN_NODE_0_MSG_0_SINGAL_0_VALUE_E

typedef enum {
    standby,
    safeMode,
    running,
    charging,
} CAN_dash_status_state_E;

inline uint8_t CAN_dash_status_state_populate(char* CANmessage, CAN_dash_status_state_E state){
    
}

inline CAN_dash_status_state_E CAN_dash_status_state_read(char* CANmessage){
    
}


#define CAN_NODE_0_MSG_0_SIGNAL_1
#define CAN_NODE_0_MSG_0_SIGNAL_2
#define CAN_NODE_0_MSG_0_SIGNAL_3
#define CAN_NODE_0_MSG_0_SIGNAL_4
#define CAN_NODE_0_MSG_0_SIGNAL_5
#define CAN_NODE_0_MSG_0_SIGNAL_6
#define CAN_NODE_0_MSG_0_SIGNAL_7



#define CAN_NODE_1 BODYCONTROLLER
#define CAN_NODE_2 BMS
#define CAN_NODE_3 MOTORCONTROLLER
#define CAN_NODE_4 LEDS

typedef struct {
    uint8_t sowthing;
} CAN_message_S;

typedef struct {
    CAN_message_S messages;
} NODE_STRUCT(CAN_NODE_0);

typedef struct {
    NODE_STRUCT(CAN_NODE_0) CAN_NODE_0;
    //    CAN_node_S BODYCONTROLLER;
    //    CAN_node_S BMS;
    //    CAN_node_S MOTORCONTROLLER;
    //    CAN_node_S LEDS;
} CAN_nodes_S;



/*NODE*/
#define CAN_DASH 0x0
/*Message*/
#define CAN_DASH_STATUS 0x0

typedef struct {
    uint8_t state : 3;
    uint8_t mode : 3;
} CAN_DASH_STATUS_S;




#endif	/* CAN_DBC_H */

