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
#include "bolt_init.h"

/*DEBUGGING*/
#define CAN_DEBUG 0
#if CAN_DEBUG
#include "bolt_UART.h"
#include <stdio.h>
static uint8_t debugEnable = 1;
#define can_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
#define can_print(...) 
#endif


#define NUM_OF_ECAN_BUFFERS 32


/* Tx Pin-to-Register Mapping*/
#define CAN_RP20_PPS    _RP20R
#define CAN_RP35_PPS    _RP35R
#define CAN_RP36_PPS    _RP36R
#define CAN_RP37_PPS    _RP37R
#define CAN_RP38_PPS    _RP38R
#define CAN_RP39_PPS    _RP39R
#define CAN_RP40_PPS    _RP40R
#define CAN_RP41_PPS    _RP41R
#define CAN_RP42_PPS    _RP42R
#define CAN_RP43_PPS    _RP43R

#ifdef _RP54R
#define CAN_RP54_PPS    _RP54R
#endif
#ifdef _RP55R
#define CAN_RP55_PPS    _RP55R
#endif
#ifdef _RP56R
#define CAN_RP56_PPS    _RP56R
#endif
#ifdef _RP57R
#define CAN_RP57_PPS    _RP57R
#endif


#define CAN_TX_FIFO_BUFFER_SIZE 8


/* CAN Register Code */
#define CAN_PPS 14

#define NUM_OF_ECAN_BUFFERS 32

/* ECAN message buffer declaration, with buffer alignment */
unsigned int ecan1MsgBuf[NUM_OF_ECAN_BUFFERS][8]
__attribute__((aligned(NUM_OF_ECAN_BUFFERS * 16)));

/* Rx data ready flag */
static volatile uint8_t CAN_RXdataReady = 0;

static uint8_t ThisTXBuffer = 0;

uint8_t CAN_Configure(CAN_txPinNumberg TXpin, uint16_t RXpin, uint32_t baud, uint8_t mode) {

    /*select Rx Pin*/
    RPINR26bits.C1RXR = RXpin;

    /* select Tx Pin */
    switch (TXpin) {
        case CAN_TX_RP20:
            CAN_RP20_PPS = CAN_PPS;
            break;
        case CAN_TX_RP35:
            CAN_RP35_PPS = CAN_PPS;
            break;
        case CAN_TX_RP36:
            CAN_RP36_PPS = CAN_PPS;
            break;
        case CAN_TX_RP37:
            CAN_RP37_PPS = CAN_PPS;
            break;
        case CAN_TX_RP38:
            CAN_RP38_PPS = CAN_PPS;
            break;
        case CAN_TX_RP39:
            CAN_RP39_PPS = CAN_PPS;
            break;
        case CAN_TX_RP40:
            CAN_RP40_PPS = CAN_PPS;
            break;
        case CAN_TX_RP41:
            CAN_RP40_PPS = CAN_PPS;
            break;
        case CAN_TX_RP42:
            CAN_RP42_PPS = CAN_PPS;
            break;
        case CAN_TX_RP43:
            CAN_RP43_PPS = CAN_PPS;
            break;
#ifdef _RP54R
        case CAN_TX_RP54:
            CAN_RP54_PPS = CAN_PPS;
            break;
#endif
#ifdef _RP55R
        case CAN_TX_RP55:
            CAN_RP55_PPS = CAN_PPS;
            break;
#endif
#ifdef _RP56R
        case CAN_TX_RP56:
            CAN_RP56_PPS = CAN_PPS;
            break;
#endif
#ifdef _RP57R
        case CAN_TX_RP57:
            CAN_RP57_PPS = CAN_PPS;
            break;
#endif
        default:
            return 1;
            break;
    }

    /*Module must first be placed in configuration mode */
    C1CTRL1bits.REQOP = 4;

    /********************************************************************************************
     Transmit Configuration
     ********************************************************************************************/

    C1CTRL1bits.WIN = 0; /*set control window to configure baud & registers*/

    uint32_t fPerif = clockFreq(); /*peripheral clock Freq*/
    uint32_t timeQuanta = 10; /*define number TQ per CAN bit.*/
    uint32_t fTQ = timeQuanta*baud; /*define TQ frequency*/
    uint32_t canBaud = (fPerif / (2 * fTQ)) - 1; /*calculate baud prescalar*/
    C1CTRL1bits.CANCKS = 0x1; /* set clock source select bit so that Fcan = 2*Fp = ClockFreq*/
    C1CFG1bits.BRP = canBaud; //canBaud; /* Configure Baud Rate */
    C1CFG1bits.SJW = 1; /* SJW = 2 TQ */
    C1CFG2bits.SEG1PH = 3; /* legth of phase 1 segment = 4TQ */
    C1CFG2bits.SEG2PHTS = 1; /*phase 2 segment is freely programmable */
    C1CFG2bits.SEG2PH = 3; /* phase 2 segment = 4TQ */
    C1CFG2bits.PRSEG = 0; /* propogation segmeny = 1TQ */
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
    DMA0CONbits.CHEN = 0x1; /* DMA channel enable */

    /* Configure Message Buffer 0-7 for Transmission and assign priority */
    _TXEN0 = 0x1;
    _TX0PRI = 0x3;
    _TXEN1 = 0x1;
    _TX1PRI = 0x3;
    _TXEN2 = 0x1;
    _TX2PRI = 0x3;
    _TXEN3 = 0x1;
    _TX3PRI = 0x3;
    _TXEN4 = 0x1;
    _TX4PRI = 0x3;
    _TXEN5 = 0x1;
    _TX5PRI = 0x3;
    _TXEN6 = 0x1;
    _TX6PRI = 0x3;
    _TXEN7 = 0x1;
    _TX7PRI = 0x3;


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
    DMA1CONbits.CHEN = 0x1; /* DMA channel enable */

    C1CTRL1bits.WIN = 1; /* set window bit to access ECAN filter registers */

    /*MASKS*/

    /*Mask 0*/
    /* Configure FilterMask0 register to mask SID<10:8> to remove Freq section 
     * of CanID. Mask bits (11-bits): 0b 000 1111 1111 */
    C1RXM0SIDbits.SID = 0x0FF;
    C1RXM0SIDbits.MIDE = 0x1; /*Standard IDs only*/
    /*Mask 1*/
    /* Configure FilterMask1 register to mask SID<10:0> to accept all 
     * of CanID. Mask bits (11-bits): 0b 000 0000 0000 */
    C1RXM1SIDbits.SID = 0x000;
    C1RXM1SIDbits.MIDE = 0x1; /*Standard IDs only*/

    /*FILTERS*/
    /*Filter 0*/
    C1FMSKSEL1bits.F0MSK = 0x0; /* select Mask 0*/
    C1RXF0SIDbits.SID = (0x045); /* Filter Value*/
    C1RXF0SIDbits.EXIDE = 0x0; /*Standard IDs only*/
    C1BUFPNT1bits.F0BP = 0xF; /*Store Hits in FIFO*/
    C1FEN1bits.FLTEN0 = 0x1; /* filter 0 enabled*/
    /*Filter 1*/
    C1FMSKSEL1bits.F1MSK = 0x1; /* select Mask 1*/
    C1RXF1SIDbits.SID = 0x000; /* Filter Value*/
    C1RXF1SIDbits.EXIDE = 0x0; /*Standard IDs only*/
    C1BUFPNT1bits.F1BP = 0xF; /*Store Hits in FIFO*/
    C1FEN1bits.FLTEN1 = 0x1; /* filter 0 enabled*/
    /*Filter 2*/
    C1FMSKSEL1bits.F2MSK = 0x0; /* select Mask 0*/
    C1RXF2SIDbits.SID = 0x0FF; /* Filter Value*/
    C1RXF2SIDbits.EXIDE = 0x0; /*Standard IDs only*/
    C1BUFPNT1bits.F2BP = 0xF; /*Store Hits in FIFO*/
    C1FEN1bits.FLTEN2 = 0x1; /* filter 0 enabled*/
    /*
     .
     .
     .
     */
    /*Filter 15*/
    C1FMSKSEL2bits.F15MSK = 0x0; /* select Mask 0*/
    C1RXF15SIDbits.SID = 0x0FF; /* Filter Value*/
    C1RXF15SIDbits.EXIDE = 0x0; /*Standard IDs only*/
    C1BUFPNT4bits.F15BP = 0xF; /*Store Hits in FIFO*/
    C1FEN1bits.FLTEN15 = 0x1; /* filter 0 enabled*/

    /* clear window bit to access ECAN control registers */
    C1CTRL1bits.WIN = 0x0;

    /*FIFO start and end at 16-31*/
    _FSA = 16;
    _DMABS = 0b110;


    /*************************************************************************************
         INTERRUPT Configuration
     *************************************************************************************/

    /* Enable Event Interrupt ISR and select events*/
    _C1IE = 1;
    _C1IF = 0;
    _C1IP = 6;

    /* enable TX Buffer interrupt, but NOT the dedicated TX ISR */
    _TBIE = 1;
    _TBIF = 0;

    /* enable RX Buffer interrupt, but NOT the dedicated RX ISR */
    _RBIE = 1;
    _RBIF = 0;

    /* enable Error interrupt*/
    _ERRIE = 1;
    _ERRIF = 0;

    /* enable Wake interrupt*/
    _WAKIE = 1;
    _WAKIF = 0;

    /* CAN is ready for transmit / receive, place in normal or loopback mode */
    C1CTRL1bits.REQOP = mode;
    while (C1CTRL1bits.OPMODE != mode) {
        ;
    }
    return 0;
}

uint8_t CAN_write(CAN_message_S * data) {
    /* write to message buffer 0-7 */
    can_print("sendingCAN\n");
    uint8_t thisBuffer = ThisTXBuffer++;
    if (ThisTXBuffer == CAN_TX_FIFO_BUFFER_SIZE) {
        ThisTXBuffer = 0;
    }

    /* Here we right the standard ID to the buffer, plus bits for remot request
     * (off) and extended ID (off). We write frequency to <10:8>, then a Node ID 
     * to <7:4>, then a message ID code to <3:0> */
    ecan1MsgBuf[thisBuffer][0] = ((data->frequency << 8) | (data->nodeID << 4) | (data->messageID)) << 2;

    /* This is left blank because no extended ID */
    /* C1TRBnEID = 0bxxxx 0000 0000 0000
     EID<17:6> = 0b0000 0000 0000 */
    ecan1MsgBuf[thisBuffer][1] = 0x0000;

    /* No remote transmit, data length = 8 bytes */
    /* RTR = 0b0
     * RB1 = 0b0
     * RB0 = 0b0
     * DLC = 0b1000 */
    ecan1MsgBuf[thisBuffer][2] = 0x0008;

    /* write message to the data bytes */

    ecan1MsgBuf[thisBuffer][3] = data->byte0 | (data->byte1 << 8);
    ecan1MsgBuf[thisBuffer][4] = data->byte2 | (data->byte3 << 8);
    ecan1MsgBuf[thisBuffer][5] = data->byte4 | (data->byte5 << 8);
    ecan1MsgBuf[thisBuffer][6] = data->byte6 | (data->byte7 << 8);

    /* Request message buffer 0 transmission */
    switch (thisBuffer) {
        case 0:
            _TXREQ0 = 0x1;
            break;
        case 1:
            _TXREQ1 = 0x1;
            break;
        case 2:
            _TXREQ2 = 0x1;
            break;
        case 3:
            _TXREQ3 = 0x1;
            break;
        case 4:
            _TXREQ4 = 0x1;
            break;
        case 5:
            _TXREQ5 = 0x1;
            break;
        case 6:
            _TXREQ6 = 0x1;
            break;
        case 7:
            _TXREQ7 = 0x1;
            break;
        default:
            break;
    }
    return 0; /*message placed successfully on the bus */
}

uint16_t CAN_Read(CAN_message_S * RxMessage) {

    uint8_t returnVal = 0;
    uint16_t thisBuff = _FNRB;

    uint16_t messageID = CAN_getCanIDFromMessage(ecan1MsgBuf[thisBuff][0]);

    RxMessage->nodeID = CAN_getNodeFromCanID(messageID);
    RxMessage->frequency = CAN_getFreqFromCanID(messageID);
    RxMessage->messageID = CAN_getMessageFromCanID(messageID);
    RxMessage->byte0 = (unsigned char) (ecan1MsgBuf[thisBuff][3]);
    RxMessage->byte1 = (unsigned char) (ecan1MsgBuf[thisBuff][3] >> 8);
    RxMessage->byte2 = (unsigned char) (ecan1MsgBuf[thisBuff][4]);
    RxMessage->byte3 = (unsigned char) (ecan1MsgBuf[thisBuff][4] >> 8);
    RxMessage->byte4 = (unsigned char) (ecan1MsgBuf[thisBuff][5]);
    RxMessage->byte5 = (unsigned char) (ecan1MsgBuf[thisBuff][5] >> 8);
    RxMessage->byte6 = (unsigned char) (ecan1MsgBuf[thisBuff][6]);
    RxMessage->byte7 = (unsigned char) (ecan1MsgBuf[thisBuff][6] >> 8);

    uint16_t clearMask = (~(0x001 << (thisBuff - 0x10)));
    C1RXFUL2 = clearMask;
    CAN_RXdataReady = 0; /* reset message receive flag */
    return returnVal; /* success */
}

uint8_t CAN_RxDataIsReady() {
    if (C1RXFUL2 | C1RXFUL1) {
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
    if (_WAKIF) {
        uint16_t temp __attribute__((unused)) = _ICODE;
        _WAKIF = 0;
        can_print("CAN_WAKE %d\n", temp);
        /*do something here...*/
    }

    /*If Error Interrupt, do something*/
    if (_ERRIF) {
        uint16_t temp __attribute__((unused)) = _ICODE;
        _ERRIF = 0;
        can_print("ERROR %d\n", temp);
        /*do something here...*/
    }

    /*If Receive Buffer Interrupt, do something*/
    if (_RBIF) {
        uint16_t temp __attribute__((unused)) = _ICODE;
        _RBIF = 0;

        can_print("RX_INT %d\n", temp);
        /*do something here...*/
        return;
    }

    /*If Transmit Buffer Interrupt, do something*/
    if (_TBIF) {
        uint16_t temp __attribute__((unused)) = _ICODE;
        _TBIF = 0;
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
