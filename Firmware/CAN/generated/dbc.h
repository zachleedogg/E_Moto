#ifndef DBC_H
#define DBC_H

#include <stdint.h>

typedef enum{
    DASH,
    BODYCONTROLLER,
    BMS,
} CAN_nodes_E;


/**********************************************************
 * DASH node stuff
 */
#define CAN_DASH_status 0x700
typedef union{
    struct {
        uint16_t word0;
        uint16_t word1;
        uint16_t word2;
        uint16_t word3;
    } bytes;
    uint16_t state: 3;
} CAN_DASH_status_U;

void CAN_DASH_status_state(uint16_t* mailbox, uint16_t state);

#define CAN_DASH_command 0x701
typedef union{
    struct {
        uint16_t word0;
        uint16_t word1;
        uint16_t word2;
        uint16_t word3;
    } bytes;
    uint16_t dothis: 3;
    uint16_t dothat: 3;
} CAN_DASH_command_U;

void CAN_DASH_command_dothis(uint16_t* mailbox, uint16_t dothis);

void CAN_DASH_command_dothat(uint16_t* mailbox, uint16_t dothat);




/**********************************************************
 * BODYCONTROLLER node stuff
 */
#define CAN_BODYCONTROLLER_status 0x710
typedef union{
    struct {
        uint16_t word0;
        uint16_t word1;
        uint16_t word2;
        uint16_t word3;
    } bytes;
    uint16_t state: 3;
} CAN_BODYCONTROLLER_status_U;

void CAN_BODYCONTROLLER_status_state(uint16_t* mailbox, uint16_t state);

#define CAN_BODYCONTROLLER_command 0x711
typedef union{
    struct {
        uint16_t word0;
        uint16_t word1;
        uint16_t word2;
        uint16_t word3;
    } bytes;
    uint16_t doSomthingElse: 1;
} CAN_BODYCONTROLLER_command_U;

void CAN_BODYCONTROLLER_command_doSomthingElse(uint16_t* mailbox, uint16_t doSomthingElse);




/**********************************************************
 * BMS node stuff
 */
#define CAN_BMS_status 0x720
typedef union{
    struct {
        uint16_t word0;
        uint16_t word1;
        uint16_t word2;
        uint16_t word3;
    } bytes;
    uint16_t state: 3;
    uint16_t SOC: 7;
    uint16_t packVoltage: 12;
    uint16_t packCurrent: 10;
    uint16_t minTemp: 12;
    uint16_t maxTemp: 12;
} CAN_BMS_status_U;

void CAN_BMS_status_state(uint16_t* mailbox, uint16_t state);

void CAN_BMS_status_SOC(uint16_t* mailbox, uint16_t SOC);

void CAN_BMS_status_packVoltage(uint16_t* mailbox, uint16_t packVoltage);

void CAN_BMS_status_packCurrent(uint16_t* mailbox, uint16_t packCurrent);

void CAN_BMS_status_minTemp(uint16_t* mailbox, uint16_t minTemp);

void CAN_BMS_status_maxTemp(uint16_t* mailbox, uint16_t maxTemp);

#define CAN_BMS_cellVoltages 0x721
typedef union{
    struct {
        uint16_t word0;
        uint16_t word1;
        uint16_t word2;
        uint16_t word3;
    } bytes;
    uint16_t cell1: 12;
    uint16_t cell2: 12;
    uint16_t cell3: 12;
    uint16_t cell4: 12;
    uint16_t cell5: 12;
} CAN_BMS_cellVoltages_U;

void CAN_BMS_cellVoltages_cell1(uint16_t* mailbox, uint16_t cell1);

void CAN_BMS_cellVoltages_cell2(uint16_t* mailbox, uint16_t cell2);

void CAN_BMS_cellVoltages_cell3(uint16_t* mailbox, uint16_t cell3);

void CAN_BMS_cellVoltages_cell4(uint16_t* mailbox, uint16_t cell4);

void CAN_BMS_cellVoltages_cell5(uint16_t* mailbox, uint16_t cell5);



#endif /*DBC_H*/
