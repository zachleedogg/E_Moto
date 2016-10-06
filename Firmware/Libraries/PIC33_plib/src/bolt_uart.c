#include "bolt_uart.h"
#include "bolt_init.h"

#define BUFFER_SIZE 36
#define QUEUE_SIZE 8

#define STOP_CHAR '\n'
#define NULL 0

#define UART1_ENABLE 1
#define UART2_ENABLE 0

typedef struct _uartBuffer {
    char buff[BUFFER_SIZE];
    uint16_t pointCurrent;
    uint16_t pointEnd;
} uartBuffer;

typedef struct _uartDataQueue {
    uint8_t buff[QUEUE_SIZE];
    uint16_t tail;
    uint16_t head;
} uartDataQueue;

#define RP20_TX_PPS    _RP20R
#define RP35_TX_PPS    _RP35R
#define RP36_TX_PPS    _RP36R
#define RP37_TX_PPS    _RP37R
#define RP38_TX_PPS    _RP38R
#define RP39_TX_PPS    _RP39R
#define RP40_TX_PPS    _RP40R
#define RP41_TX_PPS    _RP41R
#define RP42_TX_PPS    _RP42R
#define RP43_TX_PPS    _RP43R
#ifdef _RP54R
#define RP54_TX_PPS    _RP54R
#endif
#ifdef _RP55R
#define RP55_TX_PPS    _RP55R
#endif
#ifdef _RP56R
#define RP56_TX_PPS    _RP56R
#endif
#ifdef _RP57R
#define RP57_TX_PPS    _RP57R
#endif

#define UART1_PPS 1 /*From PPS map in data sheet*/
#define UART2_PPS 3 /*From PPS map in data sheet*/

void enQ(uartDataQueue * thisQ);
uint8_t deQ(uartDataQueue * thisQ);

#if UART1_ENABLE
static uartBuffer RX1buffer = {
    .buff = {},
    .pointCurrent = 0,
    .pointEnd = 0,
};
static uartBuffer TX1buffer = {
    .buff = {},
    .pointCurrent = 0,
    .pointEnd = 0,
};
static uartDataQueue RX1Q = {
    .buff = {},
    .head = 0,
    .tail = 0,
};
static uint8_t RX1dataReady = 0;
static uint8_t TX1status = 0;
#endif

#if UART2_ENABLE
static uartBuffer RX2buffer = {
    .buff = {},
    .pointCurrent = 0,
    .pointEnd = 0,
};
static uartBuffer TX2buffer = {
    .buff = {},
    .pointCurrent = 0,
    .pointEnd,
};
static uartDataQueue RX2Q = {
    .buff = {},
    .head = 0,
    .tail = 0,
};
static uint8_t RX2dataReady = 0;
static uint8_t TX2status = 0;
#endif

static uint16_t delayTime;

uint8_t Uart1Init(UART_tx_pin_number TX_pin, uint16_t RX_pin, uint32_t baudRate) {
    // Configure oscillator as needed
    uint32_t FP = clockFreq() / 2;
    if (FP <= FREQ_250KHZ) {
        return 0; //too slow for UART
    }
    uint16_t BRGVAL = (uint16_t) (((FP / baudRate) / 16) - 1);
    double tempVal;
    tempVal = (FP / baudRate);
    tempVal = tempVal / 16;
    tempVal = tempVal - 1;
    double roundVal = (double) ((uint32_t) tempVal);
    roundVal = tempVal - roundVal + 1;
    uint16_t rounded = (uint16_t) roundVal;
    if (rounded == 1) {
        tempVal = tempVal + 1;
    }
    rounded = (uint16_t) tempVal;

    BRGVAL = rounded;
    //set up Pins for UART I/O

    //RX
    _U1RXR = RX_pin;

    //TX
    switch (TX_pin) {
        case RP20_TX:
            RP20_TX_PPS = UART1_PPS;
            break;
        case RP35_TX:
            RP35_TX_PPS = UART1_PPS;
            break;
        case RP36_TX:
            RP36_TX_PPS = UART1_PPS;
            break;
        case RP37_TX:
            RP37_TX_PPS = UART1_PPS;
            break;
        case RP38_TX:
            RP38_TX_PPS = UART1_PPS;
            break;
        case RP39_TX:
            RP39_TX_PPS = UART1_PPS;
            break;
        case RP40_TX:
            RP40_TX_PPS = UART1_PPS;
            break;
        case RP41_TX:
            RP41_TX_PPS = UART1_PPS;
            break;
        case RP42_TX:
            RP42_TX_PPS = UART1_PPS;
            break;
        case RP43_TX:
            RP43_TX_PPS = UART1_PPS;
            break;
#ifdef _RP54R
        case RP54_TX:
            RP54_TX_PPS = UART1_PPS;
            break;
#endif
#ifdef _RP55R
        case RP55_TX:
            RP55_TX_PPS = UART1_PPS;
            break;
#endif
#ifdef _RP56R
        case RP56_TX:
            RP56_TX_PPS = UART1_PPS;
            break;
#endif
#ifdef _RP57R
        case RP57_TX:
            RP57_TX_PPS = UART1_PPS;
            break;
#endif
            
        default:
            return 1;
            break;
    }


    /*UART MODE SLECTION*/
    U1MODEbits.STSEL = 0; // 1-Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode (16x sampling)
    U1MODEbits.RXINV = 0; //Use pull-up res on RX

    /*Baud Rate selection*/
    U1BRG = BRGVAL; // Baud Rate setting for 9600

    /*Uart Status settings*/
    U1STAbits.UTXISEL0 = 0; // Interrupt after last char is transmitted to shift register
    U1STAbits.UTXISEL1 = 1; //bit field 2

    U1STAbits.URXISEL = 0x0; //Interupt on RX char rcvd

    /*Enable the UART interrupts*/
    //_U1TXIE = 1; // do not enable UART TX interrupt yet
    _U1TXIP = 1; //set priority
    _U1RXIE = 1; // Enable UART RX interrupt
    _U1RXIP = 1; //set priority



    /*Enable the module*/
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX

    /*Clear the RX buffer*/
    uint8_t temp = U1RXREG;
    temp = 0; /*Suppress Warning*/

    /*delay for 1 byte worth of uart TX*/
    delayTime = FP / baudRate;
    asm volatile("REPEAT #_delayTime");
    Nop();

    return 0;
}

uint8_t Uart1Write(char* inputString) {
    TX1status = 1; /*Module is now busy writing*/
    uint16_t i = 0;
    while (inputString[i] != NULL) { /*check for newline termination*/
        TX1buffer.buff[TX1buffer.pointEnd] = inputString[i]; /*capture character in buffer*/
        TX1buffer.pointEnd++; /*increment buffer endpoint*/
        i++; /*increment input string*/
        if (TX1buffer.pointEnd == BUFFER_SIZE) { /*wrap-around protection*/
            TX1buffer.pointEnd = 0;
        }
    }

    _U1TXIE = 1; // Enable UART TX interrupt
    //U1TXREG = 0xAA; // Transmit one character
    return 0;
}

uint8_t Uart1Read(char* returnString) {
    int i = 0;
    while (RX1buffer.pointCurrent != RX1buffer.pointEnd) {/*check for termination*/
        returnString[i] = RX1buffer.buff[RX1buffer.pointCurrent]; /*capture character in buffer*/
        RX1buffer.buff[RX1buffer.pointCurrent] = 0; /*clear data left behind*/
        RX1buffer.pointCurrent++; /*increment buffer current point*/
        i++; /*increment return string*/
        if (RX1buffer.pointCurrent == BUFFER_SIZE) { /*wrap-around protection*/
            RX1buffer.pointCurrent = 0;
        }
        if (RX1buffer.buff[RX1buffer.pointCurrent] == '\n') {/*check for stop char*/
            returnString[i++] = RX1buffer.buff[RX1buffer.pointCurrent]; /*Assign Stop Char*/
            RX1buffer.buff[RX1buffer.pointCurrent] = 0; /*clear data left behind*/
            RX1buffer.pointCurrent++; /*increment buffer current point*/
            returnString[i] = 0; /*NULL terminate*/
            if (RX1buffer.pointCurrent != RX1buffer.pointEnd) {/*check if there is more data..*/
                RX1dataReady = 1; /*if so, set the flag high again...*/
            }
            break;
        }
    }
    RX1dataReady = 0;
    return 0;
}

uint8_t Uart1RXdataReady(void) {
    uint8_t returnVal = 0;
    if (deQ(&RX1Q)) {
        returnVal = 1;
        RX1dataReady = 0;
    }
    return returnVal;
}

/**
 * Checks if the UART TX module is in use
 * @return Busy (1) or Not Busy (0)
 */
uint8_t Uart1TXbusy(void) {
    return TX1status;
}

void __attribute__((__interrupt__, auto_psv)) _U1TXInterrupt(void) {
    _U1TXIF = 0; // Clear TX Interrupt flag
    while (!U1STAbits.UTXBF) { /*while TX buffer is empty*/
        U1TXREG = TX1buffer.buff[TX1buffer.pointCurrent]; /* Transmit one character*/
        TX1buffer.pointCurrent++; /*increment TX pointer*/
        if (TX1buffer.pointCurrent == BUFFER_SIZE) { /*wrap-around protection*/
            TX1buffer.pointCurrent = 0;
        }
        if (TX1buffer.pointCurrent == TX1buffer.pointEnd) { /*end of transmission*/
            _U1TXIE = 0; /*Disable UART TX interrupt*/
            TX1status = 0; /*Module done writing*/
            break;
        }
    }
}

void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void) {
    _U1RXIF = 0; /* Clear RX Interrupt flag*/
    while (U1STAbits.URXDA) {/*while data is available*/
        RX1buffer.buff[RX1buffer.pointEnd] = U1RXREG; // receive one character
        if (RX1buffer.buff[RX1buffer.pointEnd] == STOP_CHAR) {
            enQ(&RX1Q);
            RX1dataReady = 1;
        }
        RX1buffer.pointEnd++;
        if (RX1buffer.pointEnd == BUFFER_SIZE) {
            RX1buffer.pointEnd = 0;
        }
    }
}

#if UART2_ENABLE
uint8_t Uart2Init(UART_tx_pin_number TX_pin, uint16_t RX_pin, uint32_t baudRate) {
    U2MODEbits.UARTEN = 0; // Enable UART
    // Configure oscillator as needed
    uint32_t FP = clockFreq() / 2;
    uint16_t BRGVAL = (uint16_t) (((FP / baudRate) / 16) - 1);
    double tempVal;
    tempVal = (FP / baudRate);
    tempVal = tempVal / 16;
    tempVal = tempVal - 1;
    double roundVal = (double) ((uint32_t) tempVal);
    roundVal = tempVal - roundVal + 1;
    uint16_t rounded = (uint16_t) roundVal;
    if (rounded == 1) {
        tempVal = tempVal + 1;
    }
    rounded = (uint16_t) tempVal;

    BRGVAL = rounded;
    //set up Pins for UART I/O

    //RX
    _U2RXR = RX_pin;

    //TX
    switch (TX_pin) {
        case RP20_TX:
            RP20_TX_PPS = UART2_PPS;
            break;
        case RP35_TX:
            RP35_TX_PPS = UART2_PPS;
            break;
        case RP36_TX:
            RP36_TX_PPS = UART2_PPS;
            break;
        case RP37_TX:
            RP37_TX_PPS = UART2_PPS;
            break;
        case RP38_TX:
            RP38_TX_PPS = UART2_PPS;
            break;
        case RP39_TX:
            RP39_TX_PPS = UART2_PPS;
            break;
        case RP40_TX:
            RP40_TX_PPS = UART2_PPS;
            break;
        case RP41_TX:
            RP41_TX_PPS = UART2_PPS;
            break;
        case RP42_TX:
            RP42_TX_PPS = UART2_PPS;
            break;
        case RP43_TX:
            RP43_TX_PPS = UART2_PPS;
            break;
#ifdef _RP54R
        case RP54_TX:
            RP54_TX_PPS = UART2_PPS;
            break;
#endif
#ifdef _RP55R
        case RP55_TX:
            RP55_TX_PPS = UART2_PPS;
            break;
#endif
#ifdef _RP56R
        case RP56_TX:
            RP56_TX_PPS = UART2_PPS;
            break;
#endif
#ifdef _RP57R
        case RP57_TX:
            RP57_TX_PPS = UART2_PPS;
            break;
#endif
        default:
            return 1;
            break;
    }


    /*UART MODE SLECTION*/
    U2MODEbits.STSEL = 0; // 1-Stop bit
    U2MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U2MODEbits.ABAUD = 0; // Auto-Baud disabled
    U2MODEbits.BRGH = 0; // Standard-Speed mode (16x sampling)
    U2MODEbits.RXINV = 0; //Use pull-up res on RX

    /*Baud Rate selection*/
    U2BRG = BRGVAL; // Baud Rate setting for 9600

    /*Uart Status settings*/
    U2STAbits.UTXISEL0 = 0; // Interrupt after last char is transmitted to shift register
    U2STAbits.UTXISEL1 = 1; //bit field 2

    U2STAbits.URXISEL = 0x0; //Interupt on RX 3/4 full

    /*Enable the UART interrupts*/
    //IEC0bits.U2TXIE = 1; // do not enable UART TX interrupt yet
    _U2TXIP = 2; //set priority
    _U2RXIE = 1; // Enable UART RX interrupt
    _U2RXIP = 3; //set priority

    /*Enable the module*/
    U2MODEbits.UARTEN = 1; // Enable UART
    U2STAbits.UTXEN = 1; // Enable UART TX

    /*Clear the RX buffer*/
    uint8_t temp = U2RXREG;
    temp = 0; /*Suppress warning*/

    /*delay for 1 byte worth of uart TX*/
    delayTime = FP / baudRate;
    asm volatile("REPEAT #_delayTime");
    Nop();


    return 0;
}

uint8_t Uart2Write(char* inputString) {
    TX2status = 1; /*Module now busy writing*/
    uint16_t i = 0;
    while (inputString[i] != NULL) { /*check for newline termination*/
        TX2buffer.buff[TX2buffer.pointEnd] = inputString[i]; /*capture character in buffer*/
        TX2buffer.pointEnd++; /*increment buffer endpoint*/
        i++; /*increment input string*/
        if (TX2buffer.pointEnd == BUFFER_SIZE) { /*wrap-around protection*/
            TX2buffer.pointEnd = 0;
        }
    }

    _U2TXIE = 1; // Enable UART TX interrupt
    //U2TXREG = 0xAA; // Transmit one character
    return 0;
}

uint8_t Uart2Read(char* returnString) {
    int i = 0;
    while (RX2buffer.pointCurrent != RX2buffer.pointEnd) {/*check for termination*/
        returnString[i] = RX2buffer.buff[RX2buffer.pointCurrent]; /*capture character in buffer*/
        RX2buffer.buff[RX2buffer.pointCurrent] = 0; /*clear data left behind*/
        RX2buffer.pointCurrent++; /*increment buffer current point*/
        i++; /*increment return string*/
        if (RX2buffer.pointCurrent == BUFFER_SIZE) { /*wrap-around protection*/
            RX2buffer.pointCurrent = 0;
        }
        if (RX2buffer.buff[RX2buffer.pointCurrent] == '\n') {/*check for stop char*/
            returnString[i++] = RX2buffer.buff[RX2buffer.pointCurrent]; /*Assign Stop Char*/
            RX2buffer.buff[RX2buffer.pointCurrent] = 0; /*clear data left behind*/
            RX2buffer.pointCurrent++; /*increment buffer current point*/
            returnString[i] = 0; /*NULL terminate*/
            if (RX2buffer.pointCurrent != RX2buffer.pointEnd) {/*check if there is more data..*/
                RX2dataReady = 1; /*if so, set the flag high again...*/
            }
            break;
        }
    }
    return 0;
}

uint8_t Uart2RXdataReady(void) {
    uint8_t returnVal = 0;
    //if (RX2dataReady) {
    if (deQ(&RX2Q)) {
        returnVal = 1;
        RX2dataReady = 0;
    }
    return returnVal;
}

/**
 * Checks if the UART TX module is in use
 * @return Busy (1) or Not Busy (0)
 */
uint8_t Uart2TXbusy(void) {
    return TX2status;
}

void __attribute__((__interrupt__, auto_psv)) _U2TXInterrupt(void) {
    _U2TXIF = 0; // Clear TX Interrupt flag
    while (!U2STAbits.UTXBF) { /*while TX buffer is empty*/
        U2TXREG = TX2buffer.buff[TX2buffer.pointCurrent]; /* Transmit one character*/
        TX2buffer.pointCurrent++; /*increment TX pointer*/
        if (TX2buffer.pointCurrent == BUFFER_SIZE) { /*wrap-around protection*/
            TX2buffer.pointCurrent = 0;
        }
        if (TX2buffer.pointCurrent == TX2buffer.pointEnd) { /*end of transmission*/
            _U2TXIE = 0; /*Disable UART TX interrupt*/
            TX2status = 0; /*Module done writing*/
            break;
        }
    }
}

void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void) {
    _U2RXIF = 0; /* Clear RX Interrupt flag*/
    while (U2STAbits.URXDA) {/*while data is available*/
        RX2buffer.buff[RX2buffer.pointEnd] = U2RXREG; // receive one character
        if (RX2buffer.buff[RX2buffer.pointEnd] == STOP_CHAR) {
            enQ(&RX2Q);
            RX2dataReady = 1;
        }
        RX2buffer.pointEnd++;
        if (RX2buffer.pointEnd == BUFFER_SIZE) {
            RX2buffer.pointEnd = 0;
        }
    }
}

#endif

void enQ(uartDataQueue * thisQ) {
    thisQ->buff[thisQ->head++] = 1; /*Put Data in the Q*/
    if (thisQ->head >= QUEUE_SIZE) {/*wrap-around protection*/
        thisQ->head = 0;
    }
}

uint8_t deQ(uartDataQueue * thisQ) {
    if (thisQ->tail != thisQ->head) {/*If the Queue is not empty*/
        thisQ->buff[thisQ->tail++] = 0; /*Dequeue the data*/
        if (thisQ->tail >= QUEUE_SIZE) {/*wrap-around protection*/
            thisQ->tail = 0;
        }
        return 1; /*Return */
    } else {
        return 0;/*Return true*/
    }
}