/****************************************************************************************
 Module:
     bolt_CAN.c
 Purpose:
     Contains the module functions for CAN communication on the dsPIC33E microcontroller
 author: brdgordo
 
 History
 *************
 3/30/16: first write; brdgordo
 4/6/16: successful communication and modularization; brdgordo
 5/15/16: fixed math error in module and app codes
 ****************************************************************************************/
#include "bolt_CAN.h"
#include "bolt_clock.h"
#include <math.h>

/*DEBUGGING*/
#define CAN_DEBUG 0
#if CAN_DEBUG
#include "bolt_uart.h"
#include <stdio.h>
static uint8_t debugEnable = 0;
#define can_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
#define can_print(...) 
#endif


#define NUM_OF_ECAN_BUFFERS 32

#define CAN_TX_FIFO_BUFFER_SIZE 8


/* CAN Register Code */
#define CAN_PPS 14

#define NUM_OF_ECAN_BUFFERS 32
#define NUM_OF_SW_CAN_BUFFERS 9

/* ECAN message buffer declaration, with buffer alignment */
volatile unsigned int ecan1MsgBuf[NUM_OF_ECAN_BUFFERS][8] __attribute__((aligned(NUM_OF_ECAN_BUFFERS * 16)));

/* ECAN SW message buffer declaration*/
volatile unsigned int ecanSWMsgBuf[NUM_OF_SW_CAN_BUFFERS][8];

/* Rx data ready flag */
static volatile uint8_t CAN_RXdataReady = 0;

static uint8_t ThisTXBuffer = 0;

static uint8_t mBoxNumber = 0;

static uint8_t opMode = 0;

uint8_t CAN_init(uint32_t baud, uint8_t mode) {
    opMode = mode;

    /*Module must first be placed in configuration mode */
    C1CTRL1bits.REQOP = 4;

    /********************************************************************************************
     Transmit Configuration
     ********************************************************************************************/

    C1CTRL1bits.WIN = 0; /*set control window to configure baud & registers*/

    uint32_t fPerif = clockFreq(); /*peripheral clock Freq*/
    uint32_t timeQuanta = 20; /*define number TQ per CAN bit.*/
    uint32_t fTQ = timeQuanta*baud; /*define TQ frequency*/
    uint32_t canBaud = (uint32_t)((((double)(fPerif) / (2.0 * (double)(fTQ))) - 1.0) +0.5); /*calculate baud prescalar*/
    uint8_t prop_seg = timeQuanta*.15;
    uint8_t seg2 = timeQuanta*.3;
    uint8_t seg1 = timeQuanta - (4 + prop_seg + seg2);
    C1CTRL1bits.CANCKS = 0x1; /* set clock source select bit so that Fcan = 2*Fp = ClockFreq*/
    C1CFG1bits.BRP = canBaud; //canBaud; /* Configure Baud Rate */
    C1CFG1bits.SJW = 1; /* SJW = 2 TQ */
    C1CFG2bits.PRSEG = prop_seg; /* propogation segmeny = 1TQ */

    C1CFG2bits.SEG2PHTS = 1; /*phase 2 segment is freely programmable */
    C1CFG2bits.SEG2PH = seg2; /* phase 2 segment = 4TQ */
    C1CFG2bits.SEG1PH =  seg1;/* legth of phase 1 segment = 4TQ */
    C1CFG2bits.SAM = 0; /* bus line sampled one time at sampple point */
    C1CFG2bits.WAKFIL = 1; /* use CAN bus line filter for wakeup */


    /* Assign 32*8 word message buffers for ECAN1 in device RAM (uses DMA0 for TX) */
    DMA0CONbits.SIZE = 0x0; /* word */
    DMA0CONbits.DIR = 0x1; /* read from RAM, write to periph */
    DMA0CONbits.AMODE = 0x2; /* peripheral indirect addressing mode */
    DMA0CONbits.MODE = 0x0; /* continuous, ping -pong disabled */
    DMA0REQ = 70; /*DMA request for CAN transmit */
    DMA0CNT = 7; /* transfer count = 8 */
    DMA0PAD = (volatile unsigned int) &C1TXD; /* peripheral address register */
    /* DMA Start adresses */
    DMA0STAL = (unsigned int) &ecan1MsgBuf;
    DMA0STAH = (unsigned int) &ecan1MsgBuf;
    //DMA0STAL = __builtin_dmaoffset(&ecan1MsgBuf);
    //DMA0STAH = 0;
    DMA0CONbits.CHEN = 0x1; /* DMA channel enable */

    /* Configure Message Buffer 0-7 for Transmission and assign priority */
    C1TR01CONbits.TXEN0 = 0x1;
    C1TR01CONbits.TX0PRI = 0x3;
    C1TR01CONbits.TXEN1 = 0x1;
    C1TR01CONbits.TX1PRI = 0x3;
    C1TR23CONbits.TXEN2 = 0x1;
    C1TR23CONbits.TX2PRI = 0x3;
    C1TR23CONbits.TXEN3 = 0x1;
    C1TR23CONbits.TX3PRI = 0x3;
    C1TR45CONbits.TXEN4 = 0x1;
    C1TR45CONbits.TX4PRI = 0x3;
    C1TR45CONbits.TXEN5 = 0x1;
    C1TR45CONbits.TX5PRI = 0x3;
    C1TR67CONbits.TXEN6 = 0x1;
    C1TR67CONbits.TX6PRI = 0x3;
    C1TR67CONbits.TXEN7 = 0x1;
    C1TR67CONbits.TX7PRI = 0x3;


    /*************************************************************************************
     Receive Configuration
     *************************************************************************************/
    /* Assign 32*8 word message buffers for ECAN1 in device RAM. (Uses DMA1 for RX) */
    DMA1CONbits.SIZE = 0x0; /* word */
    DMA1CONbits.DIR = 0x0; /* read from periph, write to RAM */
    DMA1CONbits.AMODE = 0x2; /* PID mode */
    DMA1CONbits.MODE = 0x0; /*continuous, pin-pong disabled */
    DMA1REQ = 34; /*DMA request for CAN receive */
    DMA1CNT = 7; /* transfer count = 8 */
    DMA1PAD = (volatile unsigned int) &C1RXD; /* peripheral address register */
    /* DMA Start adresses */
    DMA1STAL = (unsigned int) &ecan1MsgBuf;
    DMA1STAH = (unsigned int) &ecan1MsgBuf;
//    DMA1STAL = __builtin_dmaoffset(&ecan1MsgBuf);
//    DMA1STAH = 0;
    DMA1CONbits.CHEN = 0x1; /* DMA channel enable */

    C1CTRL1bits.WIN = 1; /* set window bit to access ECAN filter registers */

    /*MASKS*/

    /*Mask 0*/
    /* Configure FilterMask0 register to mask SID<10:8> to remove Freq section 
     * of CanID. Mask bits (11-bits): 0b 000 1111 1111 */
    C1RXM0SIDbits.SID = 0x0FF;
    C1RXM0SIDbits.MIDE = 0x1; /*Match standard and extended (0) or match ID type in filter (1)*/
    /*Mask 1*/
    /* Configure FilterMask1 register to mask SID<10:0> to accept all 
     * of CanID. Mask bits (11-bits): 0b 000 0000 0000 */
    C1RXM1SIDbits.SID = 0x000;
    C1RXM1SIDbits.MIDE = 0x0; /*Match standard and extended (0) or match ID type in filter (1)*/
    /*Mask 2*/
    /* Configure FilterMask1 register to mask SID<10:0> for an EXACT MACH
     * . Mask bits (11-bits): 0b 111 111 1111 */
    C1RXM2SIDbits.SID = 0x7FF;
    C1RXM2SIDbits.EID = 0x3;
    C1RXM2EIDbits.EID = 0xFFFF;
    C1RXM2SIDbits.MIDE = 0x1; /*Match standard and extended (0) or match ID type in filter (1)*/



    /* clear window bit to access ECAN control registers */
    C1CTRL1bits.WIN = 0x0;

    /*FIFO start and end at 16-31*/
    C1FCTRLbits.FSA = 15;
    C1FCTRLbits.DMABS = 0b110;


    /*************************************************************************************
         INTERRUPT Configuration
     *************************************************************************************/

    /* Enable Event Interrupt ISR and select events*/
    _C1IE = 1;
    _C1IF = 0;
    _C1IP = 6;

    /* enable TX Buffer interrupt, but NOT the dedicated TX ISR */
    C1INTEbits.TBIE = 1;
    C1INTFbits.TBIF = 0;

    /* enable RX Buffer interrupt, but NOT the dedicated RX ISR */
    C1INTEbits.RBIE = 1;
    C1INTFbits.RBIF = 0;

    /* enable Error interrupt*/
    C1INTEbits.ERRIE = 1;
    C1INTFbits.ERRIF = 0;

    /* enable Wake interrupt*/
    C1INTEbits.WAKIE = 1;
    C1INTFbits.WAKIF = 0;

    /* CAN is ready for transmit / receive, place in normal or loopback mode */
    C1CTRL1bits.REQOP = opMode;
        while (C1CTRL1bits.OPMODE != opMode) {
            ;
        }
    return 0;
}

CAN_changeOpMode(uint8_t opMode){
    /* CAN is ready for transmit / receive, place in normal or loopback mode */
    C1CTRL1bits.REQOP = opMode;
        while (C1CTRL1bits.OPMODE != opMode) {
            ;
        }
    return 0;
}

uint8_t CAN_configureMailbox(CAN_message_S * newMessage) {
    uint8_t returnVal = 1;
    uint16_t SID;
    uint32_t XID_upper = 0;
    uint32_t XID_lower = 0;
    if (mBoxNumber > 15) {
        returnVal = 0;
    } else {
        C1CTRL1bits.REQOP = 4;
        C1CTRL1bits.WIN = 1; /* set window bit to access ECAN filter registers */
        

        if (newMessage->canXID){
            SID = newMessage->canID >> 18;
            XID_upper = (newMessage->canID & 0x0003FFFF) >> 16;
            XID_lower = newMessage->canID & 0x0000FFFF;
        } else {
            SID = newMessage->canID;
        }
        switch (mBoxNumber) {
            case 0:
                /*FILTERS*/
                /*Filter 0*/
                C1FMSKSEL1bits.F0MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF0SIDbits.SID = SID; /* Filter Value*/
                C1RXF0SIDbits.EID = XID_upper;
                C1RXF0EIDbits.EID = XID_lower;
                C1RXF0SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT1bits.F0BP = 0x8; /*Store Hits in RX Buffer 8*/
                C1FEN1bits.FLTEN0 = 0x1; /* filter 0 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecan1MsgBuf[8][3];
                newMessage->canMessageStatus = (uint8_t*) & ecan1MsgBuf[8][7];
                break;
            case 1:
                /*Filter 1*/
                C1FMSKSEL1bits.F1MSK = 0x2; /* select Mask 2* for EXACT MATCH*/
                C1RXF1SIDbits.SID = SID; /* Filter Value*/
                C1RXF1SIDbits.EID = XID_upper;
                C1RXF1EIDbits.EID = XID_lower;
                C1RXF1SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT1bits.F1BP = 0x9; /*Store Hits in RX Buffer 9*/
                C1FEN1bits.FLTEN1 = 0x1; /* filter 1 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecan1MsgBuf[9][3];
                newMessage->canMessageStatus = (uint8_t*) & ecan1MsgBuf[9][7];
                break;
            case 2:
                /*Filter 2*/
                C1FMSKSEL1bits.F2MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF2SIDbits.SID = SID; /* Filter Value*/
                C1RXF2SIDbits.EID = XID_upper;
                C1RXF2EIDbits.EID = XID_lower;
                C1RXF2SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT1bits.F2BP = 0xA; /*Store Hits in BR Buffer 10*/
                C1FEN1bits.FLTEN2 = 0x1; /* filter 2 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecan1MsgBuf[10][3];
                newMessage->canMessageStatus = (uint8_t*) & ecan1MsgBuf[10][7];
                break;
            case 3:
                /*Filter 3*/
                C1FMSKSEL1bits.F3MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF3SIDbits.SID = SID; /* Filter Value*/
                C1RXF3SIDbits.EID = XID_upper;
                C1RXF3EIDbits.EID = XID_lower;
                C1RXF3SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT1bits.F3BP = 0xB; /*Store Hits in BR Buffer 11*/
                C1FEN1bits.FLTEN3 = 0x1; /* filter 3 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecan1MsgBuf[11][3];
                newMessage->canMessageStatus = (uint8_t*) & ecan1MsgBuf[11][7];
                break;
            case 4:
                /*Filter 4*/
                C1FMSKSEL1bits.F4MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF4SIDbits.SID = SID; /* Filter Value*/
                C1RXF4SIDbits.EID = XID_upper;
                C1RXF4EIDbits.EID = XID_lower;
                C1RXF4SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT2bits.F4BP = 0xC; /*Store Hits in BR Buffer 12*/
                C1FEN1bits.FLTEN4 = 0x1; /* filter 2 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecan1MsgBuf[12][3];
                newMessage->canMessageStatus = (uint8_t*) & ecan1MsgBuf[12][7];
                break;
            case 5:
                /*Filter 5*/
                C1FMSKSEL1bits.F5MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF5SIDbits.SID = SID; /* Filter Value*/
                C1RXF5SIDbits.EID = XID_upper;
                C1RXF5EIDbits.EID = XID_lower;
                C1RXF5SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT2bits.F5BP = 0xD; /*Store Hits in BR Buffer 13*/
                C1FEN1bits.FLTEN5 = 0x1; /* filter 5 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecan1MsgBuf[13][3];
                newMessage->canMessageStatus = (uint8_t*) & ecan1MsgBuf[13][7];
                break;
            case 6:
                /*Filter 6*/
                C1FMSKSEL1bits.F6MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF6SIDbits.SID = SID; /* Filter Value*/
                C1RXF6SIDbits.EID = XID_upper;
                C1RXF6EIDbits.EID = XID_lower;
                C1RXF6SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT2bits.F6BP = 0xE; /*Store Hits in BR Buffer 14*/
                C1FEN1bits.FLTEN6 = 0x1; /* filter 6 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecan1MsgBuf[14][3];
                newMessage->canMessageStatus = (uint8_t*) & ecan1MsgBuf[14][7];
                break;
            case 7:
                /*Filter 7*/
                C1FMSKSEL1bits.F7MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF7SIDbits.SID = newMessage->canID; /* Filter Value*/
                C1RXF7SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT2bits.F7BP = 0xF; /*Store Hits in FIFO*/
                C1FEN1bits.FLTEN7 = 0x1; /* filter 7 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecanSWMsgBuf[0][3];
                newMessage->canMessageStatus = (uint8_t*) & ecanSWMsgBuf[0][7];
                break;
            case 8:
                /*Filter 8*/
                C1FMSKSEL2bits.F8MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF8SIDbits.SID = newMessage->canID; /* Filter Value*/
                C1RXF8SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT3bits.F8BP = 0xF; /*Store Hits in FIFO*/
                C1FEN1bits.FLTEN8 = 0x1; /* filter 8 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecanSWMsgBuf[1][3];
                newMessage->canMessageStatus = (uint8_t*) & ecanSWMsgBuf[1][7];
                break;
            case 9:
                /*Filter 9*/
                C1FMSKSEL2bits.F9MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF9SIDbits.SID = newMessage->canID; /* Filter Value*/
                C1RXF9SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT3bits.F9BP = 0xF; /*Store Hits in FIFO*/
                C1FEN1bits.FLTEN9 = 0x1; /* filter 9 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecanSWMsgBuf[2][3];
                newMessage->canMessageStatus = (uint8_t*) & ecanSWMsgBuf[2][7];
                break;
            case 10:
                /*Filter 10*/
                C1FMSKSEL2bits.F10MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF10SIDbits.SID = newMessage->canID; /* Filter Value*/
                C1RXF10SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT3bits.F10BP = 0xF; /*Store Hits in FIFO*/
                C1FEN1bits.FLTEN10 = 0x1; /* filter 10 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecanSWMsgBuf[3][3];
                newMessage->canMessageStatus = (uint8_t*) & ecanSWMsgBuf[3][7];
                break;
            case 11:
                /*Filter 11*/
                C1FMSKSEL2bits.F11MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF11SIDbits.SID = newMessage->canID; /* Filter Value*/
                C1RXF11SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT3bits.F11BP = 0xF; /*Store Hits in FIFO*/
                C1FEN1bits.FLTEN11 = 0x1; /* filter 11 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecanSWMsgBuf[4][3];
                newMessage->canMessageStatus = (uint8_t*) & ecanSWMsgBuf[4][7];
                break;
            case 12:
                /*Filter 12*/
                C1FMSKSEL2bits.F12MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF12SIDbits.SID = newMessage->canID; /* Filter Value*/
                C1RXF12SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT4bits.F12BP = 0xF; /*Store Hits in FIFO*/
                C1FEN1bits.FLTEN12 = 0x1; /* filter 12 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecanSWMsgBuf[5][3];
                newMessage->canMessageStatus = (uint8_t*) & ecanSWMsgBuf[5][7];
                break;
            case 13:
                /*Filter 13*/
                C1FMSKSEL2bits.F13MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF13SIDbits.SID = newMessage->canID; /* Filter Value*/
                C1RXF13SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT4bits.F13BP = 0xF; /*Store Hits in FIFO*/
                C1FEN1bits.FLTEN13 = 0x1; /* filter 13 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecanSWMsgBuf[6][3];
                newMessage->canMessageStatus = (uint8_t*) & ecanSWMsgBuf[6][7];
                break;
            case 14:
                /*Filter 14*/
                C1FMSKSEL2bits.F14MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF14SIDbits.SID = newMessage->canID; /* Filter Value*/
                C1RXF14SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT4bits.F14BP = 0xF; /*Store Hits in FIFO*/
                C1FEN1bits.FLTEN14 = 0x1; /* filter 14 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecanSWMsgBuf[7][3];
                newMessage->canMessageStatus = (uint8_t*) & ecanSWMsgBuf[7][7];
                break;
            case 15:
                /*Filter 15*/
                C1FMSKSEL2bits.F15MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
                C1RXF15SIDbits.SID = newMessage->canID; /* Filter Value*/
                C1RXF15SIDbits.EXIDE = newMessage->canXID; /*Standard (0) or Extended (1) IDs*/
                C1BUFPNT4bits.F15BP = 0xF; /*Store Hits in FIFO*/
                C1FEN1bits.FLTEN15 = 0x1; /* filter 15 enabled*/
                newMessage->payload = (CAN_payload_S*) & ecanSWMsgBuf[8][3];
                newMessage->canMessageStatus = (uint8_t*) & ecanSWMsgBuf[8][7];
                break;
            default:
                break;
        }
        C1CTRL1bits.WIN = 0; /* clear window bit*/
        C1CTRL1bits.REQOP = opMode;
        while (C1CTRL1bits.OPMODE != opMode) {
            ;
        }
        mBoxNumber++; /*RX mailboxe added*/
    }
    return returnVal;
}

uint8_t CAN_write(CAN_message_S data) {
    /* write to message buffer 0-7 */
    can_print("sendingCAN\n");
    uint8_t thisBuffer = ThisTXBuffer++;
    if (ThisTXBuffer == CAN_TX_FIFO_BUFFER_SIZE) {
        ThisTXBuffer = 0;
    }
    uint16_t SID;
    uint32_t XID_upper = 0;
    uint32_t XID_lower = 0;
    if (data.canXID){
        SID = data.canID >> 18;
        XID_upper = (data.canID & 0x0003FFFF) >> 6;
        XID_lower = data.canID & 0x3F;
    } else {
        SID = data.canID;
    }

    /* Here we right the standard ID to the buffer, plus bits for remot request
     * (off) and extended ID (off). We write frequency to <10:8>, then a Node ID 
     * to <7:4>, then a message ID code to <3:0> */
    ecan1MsgBuf[thisBuffer][0] = SID << 2;
    ecan1MsgBuf[thisBuffer][0] |= data.canXID << 1;
    ecan1MsgBuf[thisBuffer][0] |= data.canXID;

    /* This is extended ID upper bits*/
    /* C1TRBnEID = 0bxxxx 0000 0000 0000
     EID<17:6> = 0b0000 0000 0000 */
    ecan1MsgBuf[thisBuffer][1] = XID_upper;

    /* No remote transmit, data length = 8 bytes */
    /* RTR = 0b0
     * RB1 = 0b0
     * RB0 = 0b0
     * DLC = 0b1000 */
    ecan1MsgBuf[thisBuffer][2] = 0x0008 | (XID_lower << 10);

    /* write message to the data bytes */

    ecan1MsgBuf[thisBuffer][3] = data.payload->word0;
    ecan1MsgBuf[thisBuffer][4] = data.payload->word1;
    ecan1MsgBuf[thisBuffer][5] = data.payload->word2;
    ecan1MsgBuf[thisBuffer][6] = data.payload->word3;

    /* Request message buffer 0 transmission */
    switch (thisBuffer) {
        case 0:
            C1TR01CONbits.TXREQ0 = 0x1;
            break;
        case 1:
            C1TR01CONbits.TXREQ1 = 0x1;
            break;
        case 2:
            C1TR23CONbits.TXREQ2 = 0x1;
            break;
        case 3:
            C1TR23CONbits.TXREQ3 = 0x1;
            break;
        case 4:
            C1TR45CONbits.TXREQ4 = 0x1;
            break;
        case 5:
            C1TR45CONbits.TXREQ5 = 0x1;
            break;
        case 6:
            C1TR67CONbits.TXREQ6 = 0x1;
            break;
        case 7:
            C1TR67CONbits.TXREQ7 = 0x1;
            break;
        default:
            break;
    }
    return 0; /*message placed successfully on the bus */
}

uint8_t CAN_checkDataIsFresh(CAN_message_S * data){
    uint8_t ret = *(data->canMessageStatus);
    *(data->canMessageStatus) = 0;
    return ret;
}

uint8_t CAN_RxDataIsReady() {
    return 0;
    if (C1RXFUL2 || (C1RXFUL1 & 0x8000)) {
        return 1;
    }
    return 0;
}

/********************************************************************************************************
 Interupts:  TX and RX interrupts may be used as needed
 ********************************************************************************************************/

/* CAN Event Interrupt*/
void __attribute__((__interrupt__, auto_psv)) _C1Interrupt(void) {

    /*Clear Interrupt flag*/
    _C1IF = 0;

    can_print("CAN EVENT ");

    /*If WAKE-UP Interrupt, do something*/
    if (C1INTFbits.WAKIF) {
        uint16_t temp __attribute__((unused)) = C1VECbits.ICODE;
        C1INTFbits.WAKIF = 0;
        can_print("CAN_WAKE %d\n", temp);
        /*do something here...*/
        return;
    }

    /*If Error Interrupt, do something*/
    if (C1INTFbits.ERRIF) {
        uint16_t temp __attribute__((unused)) = C1VECbits.ICODE;
        C1INTFbits.ERRIF = 0;
        can_print("ERROR %d\n", temp);
        /*do something here...*/
        return;
    }

    /*If Receive Buffer Interrupt, do something*/
    if (C1INTFbits.RBIF) {
        uint16_t temp __attribute__((unused)) = C1VECbits.ICODE;
        C1INTFbits.RBIF = 0;
        if (temp < 15) {/*if RX event was to a static buffer*/
            C1RXFUL1 &= ~(1 << temp);
            ecan1MsgBuf[temp][7] |= 0x01;
        } else {/*if RX event was in the FIFO*/
            uint16_t thisBuff = C1FIFObits.FNRB;
            if (thisBuff == 15) {
                C1RXFUL1 &= ~(1 << 15);
            } else {
                C1RXFUL2 &= ~(1 << (temp - 16));
            }
            uint8_t filterHit = (ecan1MsgBuf[thisBuff][7] >> 8);
            int i = 0;
            for (i = 0; i < NUM_OF_SW_CAN_BUFFERS; i++) {
                ecanSWMsgBuf[filterHit - 7][i] = ecan1MsgBuf[thisBuff][i];
            }
            ecanSWMsgBuf[filterHit - 7][7] |= 0x01;
        }
        can_print("RX_INT %d\n", temp);
        /*do something here...*/
        return;
    }

    /*If Transmit Buffer Interrupt, do something*/
    if (C1INTFbits.TBIF) {
        uint16_t temp __attribute__((unused)) = C1VECbits.ICODE;
        C1INTFbits.TBIF = 0;
        can_print("TX_INT %d\n", temp);

        /*do something here...*/
        return;
    }

    /* ICODE INTERUPT SOURCES
        1000101-1111111 = Reserved; do not use
     
        1000100 = FIFO almost full interrupt
        1000011 = Receiver overflow interrupt
        1000010 = Wake-up interrupt
        1000001 = Error interrupt
        1000000 = No interrupt
     
        0100000-0111111 = Reserved; do not use
     
        0011111 = RB31 buffer Interrupt 
        0011110 = RB30 buffer Interrupt 
        .
        .
        .
        0001001 = RB9 buffer interrupt 
        0001000 = RB8 buffer interrupt 
     
        0000111 = TRB7 buffer interrupt
        0000110 = TRB6 buffer interrupt
        0000101 = TRB5 buffer interrupt
        0000100 = TRB4 buffer interrupt
        0000011 = TRB3 buffer interrupt
        0000010 = TRB2 buffer interrupt
        0000001 = TRB1 buffer interrupt
        0000000 = TRB0 Buffer interrupt
     */
}

/*The transmit data request interrupt represents the transmission of a single 
 * word in an ECAN message through the ECAN Transmit Data register */
void __attribute__((__interrupt__, auto_psv)) _C1TxReqInterrupt(void) {

    _C1TXIF = 0; /* clear hardware flag */
    can_print("CAN SENT\n");
}

/*The receive data request interrupt represents the reception of a single word
 * of an ECAN message through the ECAN Receive Data register*/
void __attribute__((__interrupt__, auto_psv)) _C1RxRdyInterrupt(void) {
    _C1RXIF = 0;
    can_print("CAN RCVD\n");
}

