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

#define TRUE 1
#define FALSE 0

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

/*Character Conversion variables*/
static uint8_t charac = 0;
static uint8_t pixelRow = 0;
static uint16_t pixelCol = 0;

/*pin variables*/
static uint8_t CEPIN;
static uint8_t DCPIN;
static uint8_t RSTPIN;

static uint8_t SPIbusy = FALSE;

/*Initialization Data*/
static const uint8_t swreset_cmd = HX8357_SWRESET;
static const uint8_t setc_cmd = HX8357D_SETC;
static const uint16_t setc_data[] = {0xFF, 0x83, 0x57};
static const uint8_t setrgb_cmd = HX8357_SETRGB;
static const uint16_t setrgb_data[] = {0x80, 0x00, 0x06, 0x06};
static const uint8_t setcom_cmd = HX8357D_SETCOM;
static const uint16_t setcom_data[] = {0x25}; // -1.52V
static const uint8_t setosc_cmd = HX8357_SETOSC;
static const uint16_t setosc_data[] = {0x68}; // Normal mode 70Hz, Idle mode 55 Hz
static const uint8_t setpanel_cmd = HX8357_SETPANEL; //Set Panel
static const uint16_t setpanel_data[] = {0x05}; // BGR, Gate direction swapped
static const uint8_t setpwr1_cmd = HX8357_SETPWR1;
static const uint16_t setpwr1_data[] = {
    0x00, // Not deep standby
    0x15, //BT
    0x1C, //VSPR
    0x1C, //VSNR
    0x83, //AP
    0xAA
}; //FS
static const uint8_t setstba_cmd = HX8357D_SETSTBA;
static const uint16_t setstba_data[] = {
    0x50, //OPON normal
    0x50, //OPON idle
    0x01, //STBA
    0x3C, //STBA
    0x1E, //STBA
    0x08
}; //GEN
static const uint8_t setcyc_cmd = HX8357D_SETCYC;
static const uint16_t setcyc_data[] = {
    0x02, //NW 0x02
    0x40, //RTN
    0x00, //DIV
    0x2A, //DUM
    0x2A, //DUM
    0x0D, //GDON
    0x78
}; //GDOFF
static const uint8_t setgamma_cmd = HX8357D_SETGAMMA;
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
static const uint8_t colmod_cmd = HX8357_COLMOD;
static const uint16_t colmod_data[] = {0x55}; // 16 bit 
static const uint8_t madctl_cmd = HX8357_MADCTL;
static uint16_t madctl_data[] = {MADCTL_MV | MADCTL_MX | MADCTL_RGB}; //{MADCTL_MX | MADCTL_MV | MADCTL_RGB}//{0xC0};
static const uint8_t teon_cmd = HX8357_TEON; // TE off
static const uint16_t teon_data[] = {0x00};
static const uint8_t tearline_cmd = HX8357_TEARLINE; // tear line
static const uint16_t tearline_data[] = {0x00, 0x02};
static const uint8_t slpout_cmd = HX8357_SLPOUT; //Exit Sleep
static const uint8_t dispon_cmd = HX8357_DISPON; // display on
static const uint8_t caset_cmd = HX8357_CASET; // Column addr set
static const uint16_t caset_data[] = {0x00, 0x00, 0x01, 0x3F};
static const uint8_t paset_cmd = HX8357_PASET;
static const uint16_t paset_data[] = {0x00, 0x00, 0x01, 0xDF};
static const uint8_t rawr_cmd = HX8357_RAMWR; // write to RAM


static void writecommand(const uint8_t *commandString);
static void writedata(const uint16_t *dataString, uint32_t stringLength);
static void writeconst(uint16_t dataString, uint32_t stringLength);
static void Write();
static inline void spiWrite8(uint8_t input);
static inline void spiWrite16(uint16_t input);
static void setCanvas(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
static void tftBootUpSequence(void);

/*This will initialize the Nokia 5110 LCD screen*/
void TFT_LCD_INIT(uint8_t reset, uint8_t CE, uint8_t DC) {
    RSTPIN = reset;
    CEPIN = CE;
    DCPIN = DC;

    //uint16_t delay = 0;

    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    IEC0bits.SPI1IE = 0; // Disable the interrupt

    SPI1CON1bits.DISSCK = 0; // Internal serial clock is enabled
    SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 0; // Communication is word-wide (8 bits)
    SPI1CON1bits.MSTEN = 1; // Master mode enabled
    SPI1CON1bits.SMP = 0; // Input data is sampled at the middle of data output time
    SPI1CON1bits.CKE = 1; // Serial output data changes on transition from
    // active clock state to idle clock state
    SPI1CON1bits.CKP = 0; // Idle state for clock is a low level;

    SPI1CON2bits.SPIBEN = 1; /*Enable FIFO transmit buffer*/
    SPI1STATbits.SISEL = 0b101; /*interrupt on last bit out*/

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
    SPI1CON1bits.SPRE = 0b110;

    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    //IEC0bits.SPI1IE = 1; // Enable the interrupt
    IPC2bits.SPI1EIP = 7; //priority 5

    SPI1STATbits.SPIEN = 1; // Enable SPI module

    IO_setPinDir(RSTPIN, OUTPUT); /*Set direction to output for RB15 This is RST (Reset)*/
    IO_setPinDir(CEPIN, OUTPUT); /*Set direction to output for RB14 This is CE (Count Enable)*/
    IO_setPinDir(DCPIN, OUTPUT); /*Set direction to output for RB15 This is DC (Data = Mode Select)*/

    screenOrientation.status = HORIZONTAL;
    screenOrientation.height = 320;
    screenOrientation.width = 480;

    /*Write the initialization sequence*/
    tftBootUpSequence();
}

void TFT_LCD_ORIENTATION(orientation thisWay) {
    screenOrientation.status = thisWay;
    if (thisWay == HORIZONTAL) {
        madctl_data[0] = MADCTL_MX | MADCTL_MY | MADCTL_RGB;
        screenOrientation.height = 320;
        screenOrientation.width = 480;
    } else {
        madctl_data[0] = MADCTL_MX | MADCTL_MV | MADCTL_RGB;
        screenOrientation.height = 480;
        screenOrientation.width = 320;
    }
    writecommand(&madctl_cmd);
    writedata(madctl_data, 1);
}

void setCanvas(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    x1 = x1 - 1;
    y1 = y1 - 1;
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
    uint32_t size = ((uint32_t) (x1 - x0)*(uint32_t) (y1 - y0));
    writeconst(color, size);
}

void TFT_LCD_fillBackground(uint16_t color) {
    TFT_LCD_drawRect(0, 0, TFT_LCD_width(), TFT_LCD_height(), color);
}

void TFT_LCD_writeVariableString(char * anystring, uint16_t x, uint16_t y, uint16_t fillColor, uint16_t textColor, uint8_t size) {
    uint16_t length = 0;
    uint8_t* ptr = &stringBuffer.buffer[stringBuffer.head];
    do {
        stringBuffer.buffer[stringBuffer.head++] = anystring[length++];
    } while (anystring[length - 1] != 0);
    TFT_LCD_writeString((const char*) ptr, x, y, fillColor, textColor, size);
}

void TFT_LCD_writeString(const char * anystring, uint16_t x, uint16_t y, uint16_t fillColor, uint16_t textColor, uint8_t size) {
    uint16_t length = (uint16_t) strlen(anystring);
    setCanvas(x, y, (x + 2 * (length * ASCII_FONT_WIDTH)), (y + 2 * ASCII_FONT_HEIGHT));

    lcdData newItem;
    newItem.Length = length;
    newItem.Data = (uint16_t*) (anystring);
    newItem.Command = STRING;

    addToQueue(newItem); /*Adds what came in into Queue DC high*/
    /*Enter thread-Critical area*/
    IEC0bits.SPI1IE = 0; // Disable the interrupt
    if (SPIbusy == FALSE) {/*There is only only 1 item right now*/
        SPIbusy = TRUE;
        thisItem = deleteFromQueue();
        Write(); /*Writes from the Queue*/
    }
    IEC0bits.SPI1IE = 1; // Enable the interrupt
}

/*This function will write a command to the LCD screen ie. DC value is 0*/

/*First param is the command data, and the second param is the length of that data*/
void writecommand(const uint8_t* commandString) {

    lcdData cmdItem = {};
    cmdItem.Length = 1;
    cmdItem.Data = (uint16_t*) commandString;
    cmdItem.Command = COMMAND;
    addToQueue(cmdItem); /*Adds what came in into Queue DC low*/

    /*Enter Critical Thread*/
    IEC0bits.SPI1IE = 0; /* Disable the interrupt*/
    if (SPIbusy == FALSE) {/*if SPI is idle*/
        SPIbusy = TRUE;
        thisItem = deleteFromQueue();
        Write(); /*Writes from the Queue*/
    } else {
        ; /*Do nothing*/
    }
    IEC0bits.SPI1IE = 1; /* Enable the interrupt */

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
    IEC0bits.SPI1IE = 0; // Disable the interrupt
    if (SPIbusy == FALSE) {/*There is only only 1 item right now*/
        SPIbusy = TRUE;
        thisItem = deleteFromQueue();
        Write(); /*Writes from the Queue*/
    } else {
        ; /*Do nothing*/
    }
    IEC0bits.SPI1IE = 1; // Enable the interrupt
}

void writeconst(uint16_t dataString, uint32_t stringLength) {
    lcdData newItem;
    newItem.Length = stringLength;
    newItem.Data = (uint16_t*) dataString;
    newItem.Command = CONST;

    addToQueue(newItem); /*Adds what came in into Queue DC high*/
    /*Enter thread-Critical area*/
    IEC0bits.SPI1IE = 0; // Disable the interrupt
    if (SPIbusy == FALSE) {/*There is only only 1 item right now*/
        SPIbusy = TRUE;
        thisItem = deleteFromQueue();
        Write(); /*Writes from the Queue*/
    } else {
        ; /*Do nothing*/
    }
    IEC0bits.SPI1IE = 1; // Enable the interrupt
}

void Write() {
    /*CE Pin is low during transmission*/
    IO_pinWrite(CEPIN, LOW);

    switch (thisItem.Command) {
        case DATA:
            /*DC pin high or low*/
            IO_pinWrite(DCPIN, HIGH);
            SPI1STATbits.SPIEN = 0; // DIsable SPI mo
            SPI1CON1bits.MODE16 = 0; /*8 bit mode*/
            SPI1STATbits.SPIEN = 1; // Enable SPI mo
            /*Write byte to SPI module*/
            while ((SPI1STATbits.SPITBF == 0) && (dataIndex < thisItem.Length)) {
                spiWrite8((uint8_t) (thisItem.Data[dataIndex++]&0x00FF));
            }
            break;
        case COMMAND:
            IO_pinWrite(DCPIN, LOW);
            SPI1STATbits.SPIEN = 0; // Disable SPI mo
            SPI1CON1bits.MODE16 = 0; /*8 bit mode*/
            SPI1STATbits.SPIEN = 1; // Enable SPI mo
            /*Write byte to SPI module*/
            while ((SPI1STATbits.SPITBF == 0) && (dataIndex < thisItem.Length)) {
                uint8_t* temp = (uint8_t*) thisItem.Data;
                dataIndex++;
                spiWrite8(*temp);
            }
            break;
        case CONST:
            IO_pinWrite(DCPIN, HIGH);
            SPI1STATbits.SPIEN = 0; // Disable SPI mo
            SPI1CON1bits.MODE16 = 1; /*16 bit mode*/
            SPI1STATbits.SPIEN = 1; // Enable SPI mo
            /*Write byte to SPI module*/
            while ((SPI1STATbits.SPITBF == 0) && (dataIndex < thisItem.Length)) {
                dataIndex++;
                spiWrite16((uint16_t) thisItem.Data);
            }
            break;
        case STRING:
            IO_pinWrite(DCPIN, HIGH);
            SPI1STATbits.SPIEN = 0; // Disable SPI mo
            SPI1CON1bits.MODE16 = 1; /*16 bit mode*/
            SPI1STATbits.SPIEN = 1; // Enable SPI mo
            /*Write byte to SPI module*/
            uint8_t* thisString = (uint8_t*) thisItem.Data;
            while ((SPI1STATbits.SPITBF == 0) && (dataIndex < thisItem.Length)) {
                /*when we reach the last row, increament to data index so
                we can leave this function when string is done.*/
                if (pixelRow == ASCII_FONT_HEIGHT) {
                    dataIndex++;
                }

                /*Write A pixel from a row of pixels within a specific character*/
                if (font[thisString[charac] * ASCII_FONT_WIDTH + pixelCol]&(1 << pixelRow)) {
                    spiWrite16(TFT_LCD_BLACK);
                    spiWrite16(TFT_LCD_BLACK);
                } else {
                    spiWrite16(TFT_LCD_RED);
                    spiWrite16(TFT_LCD_RED);
                }
                /*When character width is reached, advance charPtr to the next character*/
                if (++pixelCol == ASCII_FONT_WIDTH) {
                    pixelCol = 0;
                    /*When string length has been reached, advance to rowptr and
                     write the next row of pixels*/
                    if (++charac == thisItem.Length) {
                        charac = 0;
                        pixelRow++;
                    }
                }
            }
            if (dataIndex >= thisItem.Length) {
                pixelCol = 0;
                pixelRow = 0;
                charac = 0;
            }
            break;

        default:
            break;
    }
}

/*This will start writing to the screen through the SPI*/
static inline void spiWrite8(uint8_t input) {
    SPI1BUF = input;
}

static inline void spiWrite16(uint16_t input) {
    SPI1BUF = input;
}

/*When the SPI has finished writing this function will be called immediately and
 decide whether to keep writing from the Queue or stop writing*/
void __attribute__((__interrupt__, __auto_psv__)) _SPI1Interrupt(void) {

    IFS0bits.SPI1IF = 0; /* Clear the Interrupt flag*/

    /*if there is still data in the current item*/
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

/*This function can be used for debugging but isnt used in the code. This will
 basically be the blocking code function if we need it*/
uint8_t SPIisReady(void) {
    if (SPIbusy) {
        return 0;
    } else {
        return 1;
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
    while (counter++ != 500000) {
        ;
    }
    counter = 0;
    IO_pinWrite(RSTPIN, 1); /*Sets reset low*/

    writecommand(&swreset_cmd);
    while (counter++ != 100000) {
        ;
    }
    counter = 0;
    // setextc
    writecommand(&setc_cmd);
    writedata(setc_data, 3);
    while (counter++ != 100000) {
        ;
    }
    counter = 0;
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
    while (counter++ != 100000) {
        ;
    }
    counter = 0;

    writecommand(&dispon_cmd); // display on

    while (counter++ != 100000) {
        ;
    }
    counter = 0;

    //    writecommand(&caset_cmd);
    //    writedata(caset_data, 4);
    //    writecommand(&paset_cmd);
    //    writedata(paset_data, 4);
    //    writecommand(&rawr_cmd);

}

