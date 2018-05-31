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
#define CAN_DEBUG 1
#if CAN_DEBUG
#include "bolt_UART.h"
#include <stdio.h>
static uint8_t debugEnable = 0;
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
#define NUM_OF_SW_CAN_BUFFERS 9

/* ECAN message buffer declaration, with buffer alignment */
volatile unsigned int ecan1MsgBuf[NUM_OF_ECAN_BUFFERS][8]
__attribute__((aligned(NUM_OF_ECAN_BUFFERS * 16)));

/* ECAN SW message buffer declaration*/
volatile unsigned int ecanSWMsgBuf[NUM_OF_SW_CAN_BUFFERS][8];

/* Rx data ready flag */
static volatile uint8_t CAN_RXdataReady = 0;

static uint8_t ThisTXBuffer = 0;

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
    /*Mask 2*/
    /* Configure FilterMask1 register to mask SID<10:0> for an EXACT MACH
     * . Mask bits (11-bits): 0b 111 111 1111 */
    C1RXM2SIDbits.SID = 0x7FF;
    C1RXM2SIDbits.MIDE = 0x1; /*Standard IDs only*/



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

uint8_t CAN_configureMailbox(uint8_t mBoxNumber, uint16_t CAN_ID) {
    if (mBoxNumber > 15 || mBoxNumber < 8) {
        return 1;
    }
    C1CTRL1bits.REQOP = 4;
    C1CTRL1bits.WIN = 1; /* set window bit to access ECAN filter registers */
    switch (mBoxNumber) {
        case 8:
            /*FILTERS*/
            /*Filter 0*/
            C1FMSKSEL1bits.F0MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF0SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF0SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT1bits.F0BP = 0x8; /*Store Hits in RX Buffer 8*/
            C1FEN1bits.FLTEN0 = 0x1; /* filter 0 enabled*/
            break;
        case 9:
            /*Filter 1*/
            C1FMSKSEL1bits.F1MSK = 0x2; /* select Mask 2* for EXACT MATCH*/
            C1RXF1SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF1SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT1bits.F1BP = 0x9; /*Store Hits in RX Buffer 9*/
            C1FEN1bits.FLTEN1 = 0x1; /* filter 1 enabled*/
            break;
        case 10:
            /*Filter 2*/
            C1FMSKSEL1bits.F2MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF2SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF2SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT1bits.F2BP = 0xA; /*Store Hits in BR Buffer 10*/
            C1FEN1bits.FLTEN2 = 0x1; /* filter 2 enabled*/
            break;
        case 11:
            /*Filter 3*/
            C1FMSKSEL1bits.F3MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF3SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF3SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT1bits.F3BP = 0xB; /*Store Hits in BR Buffer 11*/
            C1FEN1bits.FLTEN3 = 0x1; /* filter 3 enabled*/
            break;
        case 12:
            /*Filter 4*/
            C1FMSKSEL1bits.F4MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF4SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF4SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT2bits.F4BP = 0xC; /*Store Hits in BR Buffer 12*/
            C1FEN1bits.FLTEN4 = 0x1; /* filter 2 enabled*/
            break;
        case 13:
            /*Filter 5*/
            C1FMSKSEL1bits.F5MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF5SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF5SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT2bits.F5BP = 0xD; /*Store Hits in BR Buffer 13*/
            C1FEN1bits.FLTEN5 = 0x1; /* filter 5 enabled*/
            break;
        case 14:
            /*Filter 6*/
            C1FMSKSEL1bits.F6MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF6SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF6SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT2bits.F6BP = 0xE; /*Store Hits in BR Buffer 14*/
            C1FEN1bits.FLTEN6 = 0x1; /* filter 6 enabled*/
            break;
        case 15:
            /*Filter 7*/
            C1FMSKSEL1bits.F7MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF7SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF7SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT2bits.F7BP = 0xF; /*Store Hits in FIFO*/
            C1FEN1bits.FLTEN7 = 0x1; /* filter 7 enabled*/
            break;
        case 16:
            /*Filter 8*/
            C1FMSKSEL2bits.F8MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF8SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF8SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT3bits.F8BP = 0xF; /*Store Hits in FIFO*/
            C1FEN1bits.FLTEN8 = 0x1; /* filter 8 enabled*/
            break;
        case 17:
            /*Filter 9*/
            C1FMSKSEL2bits.F9MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF9SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF9SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT3bits.F9BP = 0xF; /*Store Hits in FIFO*/
            C1FEN1bits.FLTEN9 = 0x1; /* filter 9 enabled*/
            break;
        case 18:
            /*Filter 10*/
            C1FMSKSEL2bits.F10MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF10SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF10SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT3bits.F10BP = 0xF; /*Store Hits in FIFO*/
            C1FEN1bits.FLTEN10 = 0x1; /* filter 10 enabled*/
            break;
        case 19:
            /*Filter 11*/
            C1FMSKSEL2bits.F11MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF11SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF11SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT3bits.F11BP = 0xF; /*Store Hits in FIFO*/
            C1FEN1bits.FLTEN11 = 0x1; /* filter 11 enabled*/
            break;
        case 20:
            /*Filter 12*/
            C1FMSKSEL2bits.F12MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF12SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF12SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT4bits.F12BP = 0xF; /*Store Hits in FIFO*/
            C1FEN1bits.FLTEN12 = 0x1; /* filter 12 enabled*/
            break;
        case 21:
            /*Filter 13*/
            C1FMSKSEL2bits.F13MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF13SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF13SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT4bits.F13BP = 0xF; /*Store Hits in FIFO*/
            C1FEN1bits.FLTEN13 = 0x1; /* filter 13 enabled*/
            break;
        case 22:
            /*Filter 14*/
            C1FMSKSEL2bits.F14MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF14SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF14SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT4bits.F14BP = 0xF; /*Store Hits in FIFO*/
            C1FEN1bits.FLTEN14 = 0x1; /* filter 14 enabled*/
            break;
        case 23:
            /*Filter 15*/
            C1FMSKSEL2bits.F15MSK = 0x2; /* select Mask 2 for EXACT MATCH*/
            C1RXF15SIDbits.SID = CAN_ID; /* Filter Value*/
            C1RXF15SIDbits.EXIDE = 0x0; /*Standard IDs only*/
            C1BUFPNT4bits.F15BP = 0xF; /*Store Hits in FIFO*/
            C1FEN1bits.FLTEN15 = 0x1; /* filter 15 enabled*/
            break;
        default:
            break;

    }


    C1CTRL1bits.WIN = 0; /* clear window bit*/
    C1CTRL1bits.REQOP = opMode;
    while (C1CTRL1bits.OPMODE != opMode) {
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

    ecan1MsgBuf[thisBuffer][3] = data->word0;
    ecan1MsgBuf[thisBuffer][4] = data->word1;
    ecan1MsgBuf[thisBuffer][5] = data->word2;
    ecan1MsgBuf[thisBuffer][6] = data->word3;

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

uint16_t CAN_Read(CAN_message_S * RxMessage, uint8_t RXBufferNumber) {
    uint16_t thisBuff = RXBufferNumber;
    volatile uint16_t * bufferptr;
    if (thisBuff < 15) {
        bufferptr = &ecan1MsgBuf[0][0];
    } else {
        bufferptr = &ecanSWMsgBuf[0][0];
        thisBuff = thisBuff - 15;
    }
    uint16_t messageID = CAN_getCanIDFromMessage(bufferptr[thisBuff * 8 + 0]);
    RxMessage->nodeID = CAN_getNodeFromCanID(messageID);
    RxMessage->frequency = CAN_getFreqFromCanID(messageID);
    RxMessage->messageID = CAN_getMessageFromCanID(messageID);
    RxMessage->word0 = bufferptr[thisBuff * 8 + 3];
    RxMessage->word1 = bufferptr[thisBuff * 8 + 4];
    RxMessage->word2 = bufferptr[thisBuff * 8 + 5];
    RxMessage->word3 = bufferptr[thisBuff * 8 + 6];

    return 0; /* success */
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
        } else {/*if RX event was in the FIFO*/
            uint16_t thisBuff = C1FIFObits.FNRB;
            if (thisBuff == 15) {
                C1RXFUL1 = 0x7FF;
            } else {
                C1RXFUL2 = ~(1 << (temp - 0x10));
            }
            uint8_t filterHit = (ecan1MsgBuf[thisBuff][7] >> 8);
            int i = 0;
            for (i = 0; i < 8; i++) {
                ecanSWMsgBuf[filterHit - 7][i] = ecan1MsgBuf[thisBuff][i];
            }
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
