/*
 * File:   LCD_SPI.c
 * Author: Zach Levenberg
 *
 * Created on February 20, 2016, 7:11 PM
 */

#include "TFT_LCD_2.h"
#include "bolt_pins.h"
#include "TFT_LCD_Q.h"

#define TRUE 1
#define FALSE 0

/*Variables to hold certain data*/
static lcdData thisItem;
static uint16_t dataIndex = 0;

/*pin variables*/
static uint8_t CEPIN;
static uint8_t DCPIN;
static uint8_t RSTPIN;

static uint8_t SPIbusy = FALSE;

static uint8_t swreset_cmd = HX8357_SWRESET;

// setextc
static uint8_t setc_cmd = HX8357D_SETC;
static uint8_t setc_data[] = {0xFF, 0x83, 0x57};

// setRGB which also enables SDO
static uint8_t setrgb_cmd = HX8357_SETRGB;
static uint8_t setrgb_data[] = {0x80, 0x00, 0x06, 0x06};

static uint8_t setcom_cmd = HX8357D_SETCOM;
static uint8_t setcom_data[] = {0x25}; // -1.52V

static uint8_t setosc_cmd = HX8357_SETOSC;
static uint8_t setosc_data[] = {0x68}; // Normal mode 70Hz, Idle mode 55 Hz

static uint8_t setpanel_cmd = HX8357_SETPANEL; //Set Panel
static uint8_t setpanel_data[] = {0x05}; // BGR, Gate direction swapped

static uint8_t setpwr1_cmd = HX8357_SETPWR1;
static uint8_t setpwr1_data[] = {
    0x00, // Not deep standby
    0x15, //BT
    0x1C, //VSPR
    0x1C, //VSNR
    0x83, //AP
    0xAA
}; //FS

static uint8_t setstba_cmd = HX8357D_SETSTBA;
static uint8_t setstba_data[] = {
    0x50, //OPON normal
    0x50, //OPON idle
    0x01, //STBA
    0x3C, //STBA
    0x1E, //STBA
    0x08
}; //GEN

static uint8_t setcyc_cmd = HX8357D_SETCYC;
static uint8_t setcyc_data[] = {
    0x02, //NW 0x02
    0x40, //RTN
    0x00, //DIV
    0x2A, //DUM
    0x2A, //DUM
    0x0D, //GDON
    0x78
}; //GDOFF

static uint8_t setgamma_cmd = HX8357D_SETGAMMA;
static uint8_t setgamma_data[] = {
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

static uint8_t colmod_cmd = HX8357_COLMOD;
static uint8_t colmod_data[] = {
    0x55 // 16 bit 
};

static uint8_t madctl_cmd = HX8357_MADCTL;
static uint8_t madctl_data[] = {
    0xC0
};

static uint8_t teon_cmd = HX8357_TEON; // TE off
static uint8_t teon_data[] = {
    0x00
};

static uint8_t tearline_cmd = HX8357_TEARLINE; // tear line
static uint8_t tearline_data[] = {
    0x00,
    0x02
};

static uint8_t slpout_cmd = HX8357_SLPOUT; //Exit Sleep

static uint8_t dispon_cmd = HX8357_DISPON; // display on

static uint8_t caset_cmd = HX8357_CASET; // Column addr set
static uint8_t caset_data[] = {0x00, 0x00, 0x01, 0x3F};

static uint8_t paset_cmd = HX8357_PASET;
static uint8_t paset_data[] = {0x00, 0x00, 0x01, 0xDF};

static uint8_t rawr_cmd = HX8357_RAMWR; // write to RAM

static void Write();
static void spiWrite(uint8_t input);
static void tftBootUpSequence(void);

/*This will initialize the Nokia 5110 LCD screen*/
void LCDInit(uint8_t reset, uint8_t CE, uint8_t DC) {
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
    SPI1STATbits.SISEL = 0b101; /*interupt on last bit out*/

    /*11 = Primary prescale 1:1
      10 = Primary prescale 4:1
      01 = Primary prescale 16:1
      00 = Primary prescale 64:1*/
    //SPI1CON1bits.PPRE = 0b01;
    SPI1CON1bits.PPRE = 0b11;

    /*111 = Secondary prescale 1:1
      110 = Secondary prescale 2:1
      101 = Secondary prescale 3:1
      100 = Secondary prescale 4:1
      011 = Secondary prescale 5:1
      010 = Secondary prescale 6:1
      001 = Secondary prescale 7:1
      000 = Secondary prescale 8:1*/
    //SPI1CON1bits.SPRE = 0b011;
    SPI1CON1bits.SPRE = 0b110;

    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    //IEC0bits.SPI1IE = 1; // Enable the interrupt
    IPC2bits.SPI1EIP = 7; //priority 5

    SPI1STATbits.SPIEN = 1; // Enable SPI mo

    IO_setPinDir(RSTPIN, OUTPUT); /*Set direction to output for RB15 This is RST (Reset)*/
    IO_setPinDir(CEPIN, OUTPUT); /*Set direction to output for RB14 This is CE (Count Enable)*/
    IO_setPinDir(DCPIN, OUTPUT); /*Set direction to output for RB15 This is DC (Data = Mode Select)*/




    /*Write the initialization sequence*/
    tftBootUpSequence();
}

/*This function will write a command to the LCD screen ie. DC value is 0*/

/*First param is the command data, and the second param is the length of that data*/
void writecommand(uint8_t* commandString) {

    lcdData cmdItem = {};
    cmdItem.Length = 1;
    cmdItem.Data = commandString;
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
void writedata(uint8_t *dataString, uint16_t stringLength) {

    lcdData newItem;
    newItem.Length = stringLength;
    newItem.Data = dataString;
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

void writeconst(uint8_t *dataString, uint16_t stringLength) {
    lcdData newItem;
    newItem.Length = stringLength;
    newItem.Data = dataString;
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
    //        IO_pinWrite(DCPIN, thisItem.Command);
    //        /*Write byte to SPI module*/
    //        while ((SPI1STATbits.SPITBF == 0) && (dataIndex < thisItem.Length)) {
    //            spiWrite(thisItem.Data[dataIndex++]);
    //        }

    switch (thisItem.Command) {
        case 0:
            /*DC pin high or low*/
            IO_pinWrite(DCPIN, HIGH);
            /*Write byte to SPI module*/
            while ((SPI1STATbits.SPITBF == 0) && (dataIndex < thisItem.Length)) {
                spiWrite(thisItem.Data[dataIndex++]);
            }
            break;
        case 1:
            IO_pinWrite(DCPIN, LOW);
            /*Write byte to SPI module*/
            while ((SPI1STATbits.SPITBF == 0) && (dataIndex < thisItem.Length)) {
                spiWrite(thisItem.Data[dataIndex++]);
            }
            break;
        case 2:
            IO_pinWrite(DCPIN, HIGH);
            /*Write byte to SPI module*/
            while ((SPI1STATbits.SPITBF == 0) && (dataIndex < thisItem.Length)) {
                dataIndex++;
                spiWrite(thisItem.Data[0]);
            }
            break;

        default:
            break;
    }
}

/*This will start writing to the screen through the SPI*/
static void spiWrite(uint8_t input) {
    SPI1BUF = input;
}

/*When the SPI has finished writing this function will be called immediately and
 decide whether to keep writing from the Queue or stop writing*/
void __attribute__((__interrupt__, __auto_psv__)) _SPI1Interrupt(void) {

    IFS0bits.SPI1IF = 0; /* Clear the Interrupt flag*/

    /*Clear Rx FIFO buffer*/
    while (SPI2STATbits.SPITBF == 1) {
        uint16_t temp = SPI2BUF;
    }

    /*if there is still data in the current item*/
    if (dataIndex < thisItem.Length) {
        //        while ((SPI1STATbits.SPITBF == 0) && (dataIndex < thisItem.Length)) {
        //            spiWrite(thisItem.Data[dataIndex++]);
        //        }
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

/*This clears the screen by writing a bunch of 0x00*/
void LCDClear(void) {

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

uint32_t counter = 0;

static void tftBootUpSequence(void) {

    /*Reset the display and gets the Nokia 5110 to work*/
    IO_pinWrite(RSTPIN, 0); /*Sets reset high*/
    while (counter++ != 100000) {
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

    writecommand(&caset_cmd);
    writedata(caset_data, 4);
    writecommand(&paset_cmd);
    writedata(paset_data, 4);
    writecommand(&rawr_cmd);

}

