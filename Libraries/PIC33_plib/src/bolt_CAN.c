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
#include <string.h>

/*DEBUGGING*/

//#define CAN_DEBUG

#ifdef CAN_DEBUG
#include "bolt_UART.h"
#define DEBUG
#define can_print(...) ({unsigned char tempArray[100]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);})
#else
#define can_print(...) 
#endif


#define NUM_OF_ECAN_BUFFERS 32


/* Tx Pin-to-Register Mapping*/
#define RP20_CAN_TX_PPS    _RP20R
#define RP35_CAN_TX_PPS    _RP35R
#define RP36_CAN_TX_PPS    _RP36R
#define RP37_CAN_TX_PPS    _RP37R
#define RP38_CAN_TX_PPS    _RP38R
#define RP39_CAN_TX_PPS    _RP39R
#define RP40_CAN_TX_PPS    _RP40R
#define RP41_CAN_TX_PPS    _RP41R
#define RP42_CAN_TX_PPS    _RP42R
#define RP43_CAN_TX_PPS    _RP43R

#ifdef _RP54R
#define RP54_CAN_TX_PPS    _RP54R
#endif
#ifdef _RP55R
#define RP55_CAN_TX_PPS    _RP55R
#endif
#ifdef _RP56R
#define RP56_CAN_TX_PPS    _RP56R
#endif
#ifdef _RP57R
#define RP57_CAN_TX_PPS    _RP57R
#endif

/* set operating freq as 40MHz */
#define FP = 40000000

#define CAN_TX_FIFO_BUFFER_SIZE 8


/* CAN Register Code */
#define CAN_PPS 14

#define NUM_OF_ECAN_BUFFERS 32

/* ECAN message buffer declaration, with buffer alignment */
unsigned int ecan1MsgBuf[NUM_OF_ECAN_BUFFERS][8]
__attribute__((aligned(NUM_OF_ECAN_BUFFERS * 16)));

/* Rx data ready flag */
static uint8_t CAN_RXdataReady = 0;

static uint8_t ThisModule = 0;

static uint8_t ThisTXBuffer = 0;

uint8_t CAN_PinSelect(CAN_tx_pin_number TXpin, uint16_t RXpin) {
    /*select Rx Pin*/
    RPINR26bits.C1RXR = RXpin;

    /* select Tx Pin */
    switch (TXpin) {
        case RP20_CAN_TX:
            RP20_CAN_TX_PPS = CAN_PPS;
            break;
        case RP35_CAN_TX:
            RP35_CAN_TX_PPS = CAN_PPS;
            break;
        case RP36_CAN_TX:
            RP36_CAN_TX_PPS = CAN_PPS;
            break;
        case RP37_CAN_TX:
            RP37_CAN_TX_PPS = CAN_PPS;
            break;
        case RP38_CAN_TX:
            RP38_CAN_TX_PPS = CAN_PPS;
            break;
        case RP39_CAN_TX:
            RP39_CAN_TX_PPS = CAN_PPS;
            break;
        case RP40_CAN_TX:
            RP40_CAN_TX_PPS = CAN_PPS;
            break;
        case RP41_CAN_TX:
            RP40_CAN_TX_PPS = CAN_PPS;
            break;
        case RP42_CAN_TX:
            RP42_CAN_TX_PPS = CAN_PPS;
            break;
        case RP43_CAN_TX:
            RP43_CAN_TX_PPS = CAN_PPS;
            break;
#ifdef _RP54R
        case RP54_CAN_TX:
            RP54_CAN_TX_PPS = CAN_PPS;
            break;
#endif
#ifdef _RP55R
        case RP55_CAN_TX:
            RP55_CAN_TX_PPS = CAN_PPS;
            break;
#endif
#ifdef _RP56R
        case RP56_CAN_TX:
            RP56_CAN_TX_PPS = CAN_PPS;
            break;
#endif
#ifdef _RP57R
        case RP57_CAN_TX:
            RP57_CAN_TX_PPS = CAN_PPS;
            break;
#endif
        default:
            return 1;
            break;
    }
    return 0;
}

uint8_t CAN_Configure(uint8_t mode, uint8_t baud) {
    /* Set up ECAN1 module to operate at 250 kbps. Module must first be placed in configuration
     mode */
    C1CTRL1bits.REQOP = 4;

    /********************************************************************************************
     Transmit Configuration
     ********************************************************************************************/

    C1CTRL1bits.WIN = 0;

    /* Set up CAN module for 250kbps speed w/ 10 TQ per bit */
    C1CTRL1bits.CANCKS = 0x0; /* clear clock source select bit so that Fcan = Fp = 40MHz */
    C1CFG1bits.SJW = 1; /* SJW = 2 TQ */
    C1CFG1bits.BRP = baud; /* Configure Baud Rate */
    C1CFG2bits.SEG1PH = 7; /* legth of phase 1 segment = 8TQ */
    C1CFG2bits.SEG2PHTS = 1; /*phase 2 segment is freely programmable */
    C1CFG2bits.SEG2PH = 7; /* phase 2 segment = 8TQ */
    C1CFG2bits.PRSEG = 2; /* propogation segmeny = 3TQ */
    C1CFG2bits.SAM = 0; /* bus line sampled one time at sampple point */
    C1CFG2bits.WAKFIL = 1; /* use CAN bus line filter for wakeup */
    //C1FCTRL = 0xC01F; /* No FIFO, 32 Buffers */

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

    /* enable Tx interrrupt, but NOT the dedicated TX ISR */
    _TBIE = 1;
    _TBIF = 0;



    /*************************************************************************************
     Receive Configuration
     *************************************************************************************/

    C1CTRL1bits.WIN = 1;

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

    /* select acceptance filter mask 0 for acceptance filter 0 */
    C1FMSKSEL1bits.F0MSK = 0x0;

    /* Configure Acceptance filter mask 0 register to mask SID<2:0>
     Mask bits (11-bits): 0x488 */
    C1RXM0SIDbits.SID = 0x700;

    /* Configure acceptance filter 0 to match our SID */
    C1RXF0SIDbits.SID = 0x700;

    /* acceptance filter 0 to check for standard ID */
    C1RXM0SIDbits.MIDE = 0x1;
    C1RXF0SIDbits.EXIDE = 0x0;

    /* acceptance filter 0 to FIFO store message */
    C1BUFPNT1bits.F0BP = 0xF;

    /* filter 0 enabled for ID match w/ incoming message */
    C1FEN1bits.FLTEN0 = 0x1;
    /* clear window bit to access ECAN control registers */
    C1CTRL1bits.WIN = 0x0;

    /* enable Rx interrrupt */
    _C1RXIE = 1;
    _C1RXIF = 0;
    _RBIE = 1;
    _RBIF = 0;
    _C1RXIP = 7;

    /* enable error interrupts */
    _C1IE = 1;
    _ERRIE = 1;
    _ERRIF = 0;
    _C1IP = 6;

    /*FIFO start and end at 16-31*/
    _FSA = 16;
    _DMABS = 0b110;

    /* CAN is ready for transmit / receive, place in normal or loopback mode */
    C1CTRL1bits.REQOP = mode;
    while (C1CTRL1bits.OPMODE != mode) {
        ;
    }
    return 0;
}

void CAN_setModuleID(canModules thisModule) {
    ThisModule = thisModule;
}

void CAN_sendFlag(canFlags thisFlag) {
    unsigned char temp[8] = {};
    temp[0] = thisFlag;
    CAN_write(temp);
}

void CAN_sendFloat(canFlags thisFlag, float data) {
    unsigned char* dataptr = (unsigned char*) &data;
    unsigned char temp[8] = {};
    temp[0] = thisFlag;
    uint8_t i = 0;
    for (i = 0; i < 4; i++) {
        temp[i + 1] = *dataptr++;
    }
    CAN_write(temp);
}

void CAN_sendInt(canFlags thisFlag, uint16_t data) {
    unsigned char temp[8] = {};
    temp[0] = thisFlag;
    temp[1] = (unsigned char) (data>>8);
    temp[2] = (unsigned char) data;
    CAN_write(temp);
}

uint8_t CAN_write(unsigned char * data) {
    /* write to message buffer 0-7 */

    uint8_t thisBuffer = ThisTXBuffer++;
    if (ThisTXBuffer == CAN_TX_FIFO_BUFFER_SIZE) {
        ThisTXBuffer = 0;
    }

    /* Here we right the standard ID to the buffer, plus bits for remot request
     (off) and extended ID (off). We alway right 0b111 to 3 MSb, then a module
     ID code to the next bits, then a specific application ID code to the last 4 */
    ecan1MsgBuf[thisBuffer][0] = (0x700 | (ThisModule << 4)) << 2;

    /* This is left blank because no extended ID */
    /* C1TRBnEID = 0bxxxx 0000 0000 0000
     EID<17:6> = 0b0000 0000 0000 */
    ecan1MsgBuf[thisBuffer][1] = 0x0000;

    /* No remote transmit, data length = 8 bytes */
    /* CiTRBnDLC = 0b0000 0000 xxx0 1111
     EID<17:6> = 0b000000
     RTR = 0b0
     RB1 = 0b0
     RB0 = 0b0
     DLC = 0b1111 */
    ecan1MsgBuf[thisBuffer][2] = 0x0008;

    /* write message to the data bytes */
    int i = 0;
    for (i = 0; i < 4; i++) {
        ecan1MsgBuf[thisBuffer][i + 3] = data[2 * i] | (data[2 * i + 1] << 8); /* feed messages in big endian */
    }
    
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

uint8_t CAN_getFlag(unsigned char *message) {
    return (message[0]);
}

float CAN_getFloat(unsigned char *message) {
    float temp = 0.0;
    unsigned char * dataptr = (unsigned char*) &temp;
    uint8_t i = 0;
    for (i = 0; i < 4; i++) {
        dataptr[i] = message[i + 1];
    }
    return (temp);
}

uint16_t CAN_getInt(unsigned char *message) {
    uint16_t temp = 0;
    uint8_t * byte1 = &message[1];
    uint8_t * byte2 = &message[2];
    temp = (((uint16_t)*byte1<<8) | ((uint16_t)*byte2));
    return (temp);
}

uint16_t CAN_Read(unsigned char * RxMessage) {
    uint16_t module = 0;
    uint8_t returnVal = 0;
    memset(RxMessage, 0, 8);
    uint16_t thisBuff = _FNRB;
//    uint16_t nextBuff = _FBP;
//    uint16_t currentFull = C1RXFUL2;
//    uint16_t currentOF = C1RXOVF2;
    int i = 0;
    for (i = 0; i < 4; i++) { /* Extract Message byte by byte*/
        RxMessage[2 * i] = (unsigned char) (ecan1MsgBuf[thisBuff][i + 3]);
        RxMessage[2 * i + 1] = (unsigned char) (ecan1MsgBuf[thisBuff][i + 3]>>8);
    }
    RxMessage[2 * i] = '\n'; /*end in newline*/
    RxMessage[2 * i + 1] = 0; /*NULL terminate the string*/
    uint16_t clearMask = (~(0x001 << (thisBuff - 0x10)));
    C1RXFUL2 = clearMask;
    CAN_RXdataReady = 0; /* reset message receive flag */
    module = ecan1MsgBuf[thisBuff][0] & 0x3c0; /* extract module code from MsgBuf word 0 */
    returnVal = (uint8_t) (module >> 6); /* module, application, and success codes all included */
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


void __attribute__((__interrupt__, auto_psv)) _C1TxReqInterrupt(void) {
    //    static int i = 0;
    //    unsigned char myString[20];
    //    sprintf(myString, "Xmittion %d\n", i++);
    //    Uart1Write(myString);
    _TBIF = 0; /* clear the TX buffer int flag */
    _C1TXIF = 0; /* clear hardware flag */
}

/* Error interrupt */
void __attribute__((__interrupt__, auto_psv)) _C1Interrupt(void) {
    can_print("CAN Int\n");
    uint16_t Error = 0;
    uint16_t Code;
    uint16_t BufferInt;
    /* ICODE INTERUPT SOURCES
        1000100 = FIFO almost full interrupt
        1000011 = Receiver overflow interrupt
        1000010 = Wake-up interrupt
        1000001 = Error interrupt
        1000000 = No interrupt
        0100000-0111111 = Reserved; do not use
        0011111 = RB31 buffer Interrupt 
        0011110 = RB30 buffer Interrupt 
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
    Code = _ICODE;

    if (_ERRIF) {
        Error = Code & 0x7F;
    }
    BufferInt = Code & 0x1F;

    can_print("Error: %d, Buffer interupt: %d\n", (Error & 0x7), BufferInt);
    _ERRIF = 0;
    _C1IF = 0;
    _TBIF = 0;
    _RBIF = 0;

}

void __attribute__((__interrupt__, auto_psv)) _C1RxRdyInterrupt(void) {
    can_print("Rx Ready Interrupt\n");
    //    uint16_t temp = _FSA;
    //    if(temp == 0){
    //        _FSA = 16;
    //    }

    _RBIF = 0; /* clear the RX buffer int flag */
    _C1RXIF = 0;
    CAN_RXdataReady = 1;
}
