/*
 * File:   LCD_SPI.c
 * Author: Zach Levenberg
 *
 * Created on February 20, 2016, 7:11 PM
 */

#include <string.h>

#include "TFT_LCD.h"
#include "bolt_pins.h"
#include "TFT_LCD_Q.h"

#include "ASCII_5X7.h"
#include "bolt_uart.h"

#define MAX_XFER_SIZE 64

#define TRUE 1
#define FALSE 0

#define HEIGHT 319
#define WIDTH 479

#define setXferWidth_8bit() SPI1STATbits.SPIEN = 0; /*Disable SPI mo*/\
                            SPI1CON1bits.MODE16 = 0; /*8 bit mode*/\
                            SPI1STATbits.SPIEN = 1\
                            
#define setXferWidth_16bit() SPI1STATbits.SPIEN = 0; /*Disable SPI mo*/\
                            SPI1CON1bits.MODE16 = 1; /*16 bit mode*/\
                            SPI1STATbits.SPIEN = 1\
                            
#define setXferCount(x) (DMA2CNT = x-1)
#define setXferConstant() (DMA2CONbits.AMODE = 1)
#define setXferBlock() (DMA2CONbits.AMODE = 0)
#define startXfer() DMA2CONbits.CHEN = 1;\
                    DMA2REQbits.FORCE = 1\


/*Screen orientation*/
static struct {
    orientation status;
    uint16_t width;
    uint16_t height;
} screenOrientation;

/*Variables to hold Current Queue Item Data*/
static lcdData thisItem;
static uint32_t dataIndex = 0;

/*String Buffer*/
static struct {
    uint8_t buffer[255];
    uint8_t head;
} stringBuffer;

//Set up DMA Channel 0 to Transmit in Continuous Ping-Pong Mode:
static uint16_t TxBufferA[MAX_XFER_SIZE] __attribute__((aligned(MAX_XFER_SIZE)));

static uint16_t dummyRead __attribute__((aligned(1)));

/*Character Conversion variables*/
static uint8_t charPtr = 0;
static uint8_t rowPtr = 0;
static uint16_t colPtr = 0;
static uint8_t fontRowCtr = 0;
static uint8_t fontColCtr = 0;

/*pin variables*/
static uint8_t CEPIN;
static uint8_t DCPIN;
static uint8_t RSTPIN;

static uint8_t SPIbusy = FALSE;

/*Initialization Data*/
static const uint16_t swreset_cmd = HX8357_SWRESET;
static const uint16_t setc_cmd = HX8357D_SETC;
static const uint16_t setc_data[] = {0xFF, 0x83, 0x57};
static const uint16_t setrgb_cmd = HX8357_SETRGB;
static const uint16_t setrgb_data[] = {0x80, 0x00, 0x06, 0x06};
static const uint16_t setcom_cmd = HX8357D_SETCOM;
static const uint16_t setcom_data[] = {0x25}; // -1.52V
static const uint16_t setosc_cmd = HX8357_SETOSC;
static const uint16_t setosc_data[] = {0x68}; // Normal mode 70Hz, Idle mode 55 Hz
static const uint16_t setpanel_cmd = HX8357_SETPANEL; //Set Panel
static const uint16_t setpanel_data[] = {0x05}; // BGR, Gate direction swapped
static const uint16_t setpwr1_cmd = HX8357_SETPWR1;
static const uint16_t setpwr1_data[] = {
    0x00, // Not deep standby
    0x15, //BT
    0x1C, //VSPR
    0x1C, //VSNR
    0x83, //AP
    0xAA
}; //FS
static const uint16_t setstba_cmd = HX8357D_SETSTBA;
static const uint16_t setstba_data[] = {
    0x50, //OPON normal
    0x50, //OPON idle
    0x01, //STBA
    0x3C, //STBA
    0x1E, //STBA
    0x08
}; //GEN
static const uint16_t setcyc_cmd = HX8357D_SETCYC;
static const uint16_t setcyc_data[] = {
    0x02, //NW 0x02
    0x40, //RTN
    0x00, //DIV
    0x2A, //DUM
    0x2A, //DUM
    0x0D, //GDON
    0x78
}; //GDOFF
static const uint16_t setgamma_cmd = HX8357D_SETGAMMA;
static const uint16_t setgamma_data[] = {
    0x02,
    0x0A,
    0x11,
    0x1d,
    0x23,
    0x35,
    0x41,
    0x4b,
    0x4b,
    0x42,
    0x3A,
    0x27,
    0x1B,
    0x08,
    0x09,
    0x03,
    0x02,
    0x0A,
    0x11,
    0x1d,
    0x23,
    0x35,
    0x41,
    0x4b,
    0x4b,
    0x42,
    0x3A,
    0x27,
    0x1B,
    0x08,
    0x09,
    0x03,
    0x00,
    0x01
};
static const uint16_t colmod_cmd = HX8357_COLMOD;
static const uint16_t colmod_data[] = {0x55}; // 16 bit 
static const uint16_t madctl_cmd = HX8357_MADCTL;
static uint16_t madctl_data[] = {MADCTL_MV | MADCTL_MX |MADCTL_RGB}; //{MADCTL_MX | MADCTL_MV | MADCTL_RGB}//{0xC0};
static const uint16_t teon_cmd = HX8357_TEON; // TE off
static const uint16_t teon_data[] = {0x00};
static const uint16_t tearline_cmd = HX8357_TEARLINE; // tear line
static const uint16_t tearline_data[] = {0x00, 0x02};
static const uint16_t slpout_cmd = HX8357_SLPOUT; //Exit Sleep
static const uint16_t slpin_cmd = HX8357_SLPIN; //Enter Sleep
static const uint16_t dispon_cmd = HX8357_DISPON; // display on
static const uint16_t caset_cmd = HX8357_CASET; // Column addr set
static const uint16_t paset_cmd = HX8357_PASET;
static const uint16_t rawr_cmd = HX8357_RAMWR; // write to RAM


static void writecommand(const uint16_t *commandString);
static void writedata(const uint16_t *dataString, uint32_t stringLength);
static void writeconst(const uint16_t dataString, uint32_t stringLength);
static void Write();
static void setCanvas(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
static void tftBootUpSequence(void);

/*This will initialize the Nokia 5110 LCD screen*/
void TFT_LCD_INIT(uint8_t reset, uint8_t CE, uint8_t DC) {
    RSTPIN = reset;
    CEPIN = CE;
    DCPIN = DC;

    DMA2CONbits.SIZE = 0; //word
    DMA2CONbits.DIR = 1; //transfer from RAM to peripheral
    DMA2CONbits.HALF = 0; //Interrupt after data moved
    DMA2CONbits.NULLW = 0; //Normal operation
    DMA2CONbits.AMODE = 0; //indirect address mode with post increment
    DMA2CONbits.MODE = 1; //One-Shot

    DMA2STAL = (uint16_t) & TxBufferA;
    DMA2STAH = 0x0000;
    DMA2PAD = (volatile uint16_t) & SPI1BUF;
    DMA2CNT = MAX_XFER_SIZE - 1;
    DMA2REQ = 0x000A;

    IFS1bits.DMA2IF = 0;
    IEC1bits.DMA2IE = 1;

    DMA2CONbits.CHEN = 1;


    DMA3CONbits.SIZE = 0; //word
    DMA3CONbits.DIR = 0; //transfer from peripheral to RAM
    DMA3CONbits.HALF = 1; //Interupt after data moved
    DMA3CONbits.NULLW = 0; //Normal operation
    DMA3CONbits.AMODE = 1; //indirect address mode without post increment
    DMA3CONbits.MODE = 0; //continuous mode
    DMA3STAL = (uint16_t) & dummyRead;
    DMA3STAH = 0x0000;
    DMA3PAD = (volatile unsigned int) &SPI1BUF;
    DMA3CNT = 0xFFFF;
    DMA3REQ = 0x000A;

    DMA3CONbits.CHEN = 1;

    
    // SPI1CON1 Register Settings
    SPI1CON1bits.DISSCK = 0; // Internal serial clock is enabled
    SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 1; // Communication is word-wide (16 bits)
    SPI1CON1bits.MSTEN = 1; // Master mode enabled
    SPI1CON1bits.SMP = 0; // Input data is sampled at the middle of data output time
    SPI1CON1bits.CKE = 1; // Serial output data changes on transition from
    // active clock state to idle clock state
    SPI1CON1bits.CKP = 0; // Idle state for clock is a low level;

    /*11 = Primary prescale 1:1
      10 = Primary prescale 4:1
      01 = Primary prescale 16:1
      00 = Primary prescale 64:1*/
    SPI1CON1bits.PPRE = 0b11;

    /*111 = Secondary prescale 1:1
      110 = Secondary prescale 2:1
      101 = Secondary prescale 3:1
      100 = Secondary prescale 4:1
      011 = Secondary prescale 5:1
      010 = Secondary prescale 6:1
      001 = Secondary prescale 7:1
      000 = Secondary prescale 8:1*/
    SPI1CON1bits.SPRE = 0b101;


    IPC2bits.SPI1EIP = 3; //priority 5

    SPI1STATbits.SPIEN = 1; // Enable SPI module
    IFS0bits.SPI1IF = 0;
    //IEC0bits.SPI1IE = 1; // Enable the interrupt

    IO_setPinDir(RSTPIN, OUTPUT); /*Set direction to output for RB15 This is RST (Reset)*/
    IO_setPinDir(CEPIN, OUTPUT); /*Set direction to output for RB14 This is CE (Count Enable)*/
    IO_setPinDir(DCPIN, OUTPUT); /*Set direction to output for RB15 This is DC (Data = Mode Select)*/

    TFT_LCD_ORIENTATION(LANDSCAPE);

    /*Write the initialization sequence*/
    tftBootUpSequence();
}

void TFT_LCD_ORIENTATION(orientation thisWay) {
    screenOrientation.status = thisWay;
    if (thisWay == LANDSCAPE) {
        madctl_data[0] = 0b11101000;//MADCTL_MY | MADCTL_MV | MADCTL_RGB;
        screenOrientation.height = HEIGHT;
        screenOrientation.width = WIDTH;
    } else {
        madctl_data[0] = MADCTL_MX | MADCTL_MY | MADCTL_RGB;
        screenOrientation.height =WIDTH;
        screenOrientation.width = HEIGHT;
    }
    writecommand(&madctl_cmd);
    writedata((const uint16_t*)madctl_data, 1);
}

void setCanvas(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    writecommand(&caset_cmd);
    writeconst(x0, 1);
    writeconst(x1, 1);
    writecommand(&paset_cmd);
    writeconst(y0, 1);
    writeconst(y1, 1);
    writecommand(&rawr_cmd);
}

void TFT_LCD_drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    setCanvas(x0, y0, x1, y1);
    uint32_t size = ((uint32_t) (x1 - x0+1)*(uint32_t) (y1 - y0+1));
    writeconst(color, size);
}

void TFT_LCD_fillBackground(uint16_t color) {
    TFT_LCD_drawRect(0, 0, TFT_LCD_width(), TFT_LCD_height(), color);
}

void TFT_LCD_writeVariableString(char * anystring, uint16_t x, uint16_t y, uint16_t fillColor, uint16_t textColor, uint8_t size) {
    uint16_t length = (uint16_t) strlen(anystring);
    if (length == 0) {
        return;
    } else {
        length = 0;
    }
    uint8_t* ptr = &stringBuffer.buffer[stringBuffer.head];
    do {
        stringBuffer.buffer[stringBuffer.head++] = anystring[length++];
    } while (anystring[length - 1] != 0);
    TFT_LCD_writeString((const char*) ptr, x, y, fillColor, textColor, size);
}

void TFT_LCD_writeString(const char * anystring, uint16_t x, uint16_t y, uint16_t fillColor, uint16_t textColor, uint8_t size) {
    uint16_t length = strlen(anystring);
    lcdData newItem;
    if (length == 0) {
        return;
    }
    if (x == TFT_LCD_CENTER) {
        x = (TFT_LCD_width() / 2)-(length * ASCII_FONT_WIDTH * (uint16_t)size / 2);
    }

    setCanvas(x, y, (x + size * (length * ASCII_FONT_WIDTH))-1, (y + size * ASCII_FONT_HEIGHT)-1);

    
    newItem.Length = (uint32_t)length;
    newItem.Data = (uint16_t*) (anystring);
    newItem.Command = STRING;
    newItem.color = fillColor;
    newItem.font = size;

    addToQueue(newItem); /*Adds what came in into Queue DC high*/
    /*Enter thread-Critical area*/
    IEC1bits.DMA2IE = 0; /* Disable the interrupt*/
    if (SPIbusy == FALSE) {/*There is only only 1 item right now*/
        SPIbusy = TRUE;
        thisItem = deleteFromQueue();
        /*CE Pin is low during transmission*/
        IO_pinWrite(CEPIN, LOW);
        Write(); /*Writes from the Queue*/
    }
    IEC1bits.DMA2IE = 1; /* Enable the interrupt*/
}

void TFT_LCD_goToSleep() {
    writecommand(&slpin_cmd);
}

/*This function will write a command to the LCD screen ie. DC value is 0*/

/*First param is the command data, and the second param is the length of that data*/
void writecommand(const uint16_t* commandString) {

    lcdData cmdItem = {};
    cmdItem.Length = 1;
    cmdItem.Data = (uint16_t*) commandString;
    cmdItem.Command = COMMAND;
    addToQueue(cmdItem); /*Adds what came in into Queue DC low*/

    /*Enter Critical Thread*/
    IEC1bits.DMA2IE = 0; /* Disable the interrupt*/
    if (SPIbusy == FALSE) {/*if SPI is idle*/
        SPIbusy = TRUE;
        thisItem = deleteFromQueue();
        /*CE Pin is low during transmission*/
        IO_pinWrite(CEPIN, LOW);
        Write(); /*Writes from the Queue*/
    } else {
        ; /*Do nothing*/
    }
    IEC1bits.DMA2IE = 1; /* Enable the interrupt */

}

/*This function will write data to the LCD screen ie. DC value is 1*/

/*First param is the actual data, second is the length of that data*/
void writedata(const uint16_t *dataString, uint32_t stringLength) {

    lcdData newItem;
    newItem.Length = stringLength;
    newItem.Data = (uint16_t *) dataString;
    newItem.Command = DATA;

    addToQueue(newItem); /*Adds what came in into Queue DC high*/
    /*Enter thread-Critical area*/
    IEC1bits.DMA2IE = 0; // Disable the interrupt
    if (SPIbusy == FALSE) {/*There is only only 1 item right now*/
        SPIbusy = TRUE;
        thisItem = deleteFromQueue();
        /*CE Pin is low during transmission*/
        IO_pinWrite(CEPIN, LOW);
        Write(); /*Writes from the Queue*/
    } else {
        ; /*Do nothing*/
    }
    IEC1bits.DMA2IE = 1; // Enable the interrupt
}

void writeconst(const uint16_t dataString, uint32_t stringLength) {
    lcdData newItem;
    //stringLength *= 4;
    newItem.Data = (uint16_t*) dataString;
    newItem.Command = CONST;
    newItem.color = dataString;
    while (stringLength > 0x00003FFF) {
        newItem.Length = 0x3FFF;
        addToQueue(newItem); /*Adds what came in into Queue DC high*/
        stringLength -= 0x00003FFF;
    }
    newItem.Length = stringLength;
    addToQueue(newItem);
    /*Enter thread-Critical area*/
    IEC1bits.DMA2IE = 0; // Disable the interrupt
    if (SPIbusy == FALSE) {/*There is only only 1 item right now*/
        SPIbusy = TRUE;
        /*CE Pin is low during transmission*/
        IO_pinWrite(CEPIN, LOW);
        thisItem = deleteFromQueue();
        Write(); /*Writes from the Queue*/
    } else {
        ; /*Do nothing*/
    }
    IEC1bits.DMA2IE = 1; // Enable the interrupt
}

void Write() {
    uint16_t *ptr = TxBufferA;
    uint16_t counter = 0;
    switch (thisItem.Command) {
        case DATA:
            /*DC pin high or low*/
            IO_pinWrite(DCPIN, HIGH);
            setXferWidth_8bit();
            /*Write byte to SPI module*/
            while (dataIndex < thisItem.Length) {
                *ptr++ = thisItem.Data[dataIndex++];
            }
            setXferCount(thisItem.Length);
            setXferBlock();
            startXfer();
            break;
        case COMMAND:
            IO_pinWrite(DCPIN, LOW);
            setXferWidth_8bit();
            /*Write byte to SPI module*/
            /*Write byte to SPI module*/
            while (dataIndex < thisItem.Length) {
                *ptr++ = thisItem.Data[dataIndex++];
            }
            setXferCount(thisItem.Length);
            setXferConstant();
            startXfer();
            break;
        case CONST:
            IO_pinWrite(DCPIN, HIGH);
            setXferWidth_16bit();
            /*Write byte to SPI module*/
            *ptr++ = thisItem.color;
            dataIndex = thisItem.Length;
            setXferCount(thisItem.Length);
            setXferConstant();
            startXfer();
            break;
        case STRING:
            IO_pinWrite(DCPIN, HIGH);
            setXferWidth_16bit();
            /*Write byte to SPI module*/
            
            
            while ((counter<MAX_XFER_SIZE) && (dataIndex < thisItem.Length)) {
                counter++;
                uint8_t* thisString = (uint8_t*) thisItem.Data;
                /*when we reach the last row, increament to data index so
                we can leave this function when string is done.*/
                if (rowPtr == ASCII_FONT_HEIGHT) {
                    dataIndex++;
                }
                /*Write A pixel from a row of pixels within a specific character*/
                
                if (font[thisString[charPtr] * ASCII_FONT_WIDTH + colPtr]&(1 << rowPtr)) {
                    *ptr++ = TFT_LCD_BLACK;
                } else {
                    *ptr++ = thisItem.color;
                }

                /*For font size, repeat this pixel*/
                if (++fontColCtr == thisItem.font) {
                    fontColCtr = 0;
                    /*When character width is reached, advance charPtr to the next character*/
                    if (++colPtr == ASCII_FONT_WIDTH) {
                        colPtr = 0;

                        /*When string length has been reached, advance to rowptr and
                         write the next row of pixels*/
                        if (++charPtr == thisItem.Length) {
                            charPtr = 0;
                            if (++fontRowCtr == thisItem.font) {
                                fontRowCtr = 0;
                                rowPtr++;
                            }
                        }
                    }
                }

            }
            if (dataIndex >= thisItem.Length) {
                colPtr = 0;
                rowPtr = 0;
                charPtr = 0;
                fontRowCtr = 0;
                fontColCtr = 0;
            }
            setXferCount(counter);
            setXferBlock();
            startXfer();
            break;

        default:
            break;
    }
}

void __attribute__((__interrupt__, auto_psv)) _DMA2Interrupt(void) {
    IFS1bits.DMA2IF = 0; // Clear the DMA0 Interrupt flag
    if (dataIndex < thisItem.Length) {
        Write();
    } else {
        dataIndex = 0;
        if (!checkQueueEmpty()) {/*There are more items in the Queue*/
            thisItem = deleteFromQueue();
            Write();
        } else { /*Transmission is over for now*/
            /*CE Pin is high after transmission*/
            IO_pinWrite(CEPIN, HIGH);
            /*Status bit that the screen isnt writing anything anymore*/
            SPIbusy = FALSE;
            /*Do Nothing*/
        }
    }
}


uint16_t TFT_LCD_height(void) {
    return screenOrientation.height;
}

uint16_t TFT_LCD_width(void) {
    return screenOrientation.width;
}

uint32_t counter = 0;

static void tftBootUpSequence(void) {

    /*Reset the display and gets the Nokia 5110 to work*/
    IO_pinWrite(RSTPIN, 0); /*Sets reset high*/
    while (counter++ != 1000) {
        ;
    }
    counter = 0;
    IO_pinWrite(RSTPIN, 1); /*Sets reset low*/

    writecommand(&swreset_cmd);
    while (counter++ != 1000) {
        ;
    }
    counter = 0;
    // setextc
    writecommand(&setc_cmd);
    writedata(setc_data, 3);
    
    // setRGB which also enables SDO
    writecommand(&setrgb_cmd);
    writedata(setrgb_data, 4); //enable SDO pin!

    writecommand(&setcom_cmd);
    writedata(setcom_data, 1); // -1.52V

    writecommand(&setosc_cmd);
    writedata(setosc_data, 1); // Normal mode 70Hz, Idle mode 55 Hz

    writecommand(&setpanel_cmd); //Set Panel
    writedata(setpanel_data, 1); // BGR, Gate direction swapped

    writecommand(&setpwr1_cmd);
    writedata(setpwr1_data, 6); // Not deep standby

    writecommand(&setstba_cmd);
    writedata(setstba_data, 6); //OPON normal

    writecommand(&setcyc_cmd);
    writedata(setcyc_data, 7); //NW 0x02

    writecommand(&setgamma_cmd);
    writedata(setgamma_data, 34);

    writecommand(&colmod_cmd);
    writedata(colmod_data, 1); // 16 bit

    writecommand(&madctl_cmd);
    writedata(madctl_data, 1);

    writecommand(&teon_cmd); // TE off
    writedata(teon_data, 1);

    writecommand(&tearline_cmd); // tear line
    writedata(tearline_data, 2);

    writecommand(&slpout_cmd); //Exit Sleep

    writecommand(&dispon_cmd); // display on
}

