/*
 * File:   LCD_SPI.c
 * Author: Zach Levenberg
 *
 * Created on February 20, 2016, 7:11 PM
 */

#include "bolt_LCD.h"
#include "bolt_pins.h"
#include "LCDBits.h"
#include "bolt_LCD_queue.h"

#define TRUE 1
#define FALSE 0

#define CURSOR_BUFFER_SIZE 255

//#define CONTRAST 0xBF
#define CONTRAST 0xB0
/*This is the buffer that does the initialization of our LCD Screen*/
static const uint8_t commandBuffer[] = {0x21, CONTRAST, 0x04, 0x10, 0x20, 0x0C};

/*Variables to hold certain data*/
static lcdData thisItem;
static uint16_t dataIndex = 0;

/*pin variables (not static, these are shared with Queue as Externs*/
static uint8_t CEPIN;
static uint8_t DCPIN;
static uint8_t RSTPIN;

static uint8_t SPIbusy = FALSE;

/*Used for the cursor*/
static uint16_t cursorIndex = 0;
static uint8_t cursorArray[CURSOR_BUFFER_SIZE];



void Write();
void spiWrite(uint8_t input);

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
    SPI1CON1bits.PPRE = 0b01;

    /*111 = Secondary prescale 1:1
      110 = Secondary prescale 2:1
     * 
     * 
     * 
      000 = Secondary prescale 8:1*/
    SPI1CON1bits.SPRE = 0b011;

    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    //IEC0bits.SPI1IE = 1; // Enable the interrupt
    IPC2bits.SPI1EIP = 4; //priority 5

    SPI1STATbits.SPIEN = 1; // Enable SPI mo

    IO_setPinDir(RSTPIN, OUTPUT); /*Set direction to output for RB15 This is RST (Reset)*/
    IO_setPinDir(CEPIN, OUTPUT); /*Set direction to output for RB14 This is CE (Count Enable)*/
    IO_setPinDir(DCPIN, OUTPUT); /*Set direction to output for RB15 This is DC (Data = Mode Select)*/


    /*Reset the display and gets the Nokia 5110 to work*/
    IO_pinWrite(RSTPIN, 0); /*Sets reset high*/
    IO_pinWrite(RSTPIN, 1); /*Sets reset low*/

    LCDWriteCmd((uint8_t*) commandBuffer, 6); /*Writes the commands*/
}

/*This function will write a command to the LCD screen ie. DC value is 0*/

/*First param is the command data, and the second param is the length of that data*/
void LCDWriteCmd(uint8_t *commandString, uint16_t stringLength) {

    lcdData cmdItem = {};
    cmdItem.Length = stringLength;
    cmdItem.Data = commandString;
    cmdItem.Command = 0;
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
void LCDWriteData(uint8_t *dataString, uint16_t stringLength) {

    lcdData newItem;
    newItem.Length = stringLength;
    newItem.Data = dataString;
    newItem.Command = 1;

    addToQueue(newItem); /*Adds what came in into Queue DC high*/
    /*Enter thread-Critical area*/
    IEC0bits.SPI1IE = 0; // Disable the interrupt
    if (SPIbusy == FALSE) {/*There is only only 1 item right now*/
        thisItem = deleteFromQueue();
        dataIndex = 0;
        Write(); /*Writes from the Queue*/
    } else {
        ; /*Do nothing*/
    }
    IEC0bits.SPI1IE = 1; // Enable the interrupt
}

void Write() {
    /*DC pin high or low*/
    IO_pinWrite(DCPIN, thisItem.Command);

    /*CE Pin is low during transmission*/
    IO_pinWrite(CEPIN, 0);

    /*Write byte to SPI module*/
    while ((SPI1STATbits.SPITBF == 0) && (dataIndex < thisItem.Length)) {
        spiWrite(thisItem.Data[dataIndex++]);
    }
}

/*This will start writing to the screen through the SPI*/
void spiWrite(uint8_t input) {
    SPI1BUF = input;
}

/*When the SPI has finished writing this function will be called immediately and
 decide whether to keep writing from the Queue or stop writing*/
void __attribute__((__interrupt__, __auto_psv__)) _SPI1Interrupt(void) {

    IFS0bits.SPI1IF = 0; /* Clear the Interrupt flag*/
    //    uint16_t temp = SPI1BUF; /*Clear input buffer because it just always fills up*/
    //    temp = 0; /*Clears the temp buffer*/

    /*Clear Rx FIFO buffer*/
    while (SPI2STATbits.SPITBF == 1) {
        uint16_t temp = SPI2BUF;
    }

    /*if there is still data in the curent item*/
    if (dataIndex < thisItem.Length) {
        while ((SPI1STATbits.SPITBF == 0) && (dataIndex < thisItem.Length)) {
            spiWrite(thisItem.Data[dataIndex++]);
        }
    } else {
        dataIndex = 0;
        if (!checkQueueEmpty()) {/*There are more items in the Queue*/
            thisItem = deleteFromQueue();
            Write();
        } else { /*Transmission is over for now*/
            /*CE Pin is high after transmission*/
            IO_pinWrite(CEPIN, 1);
            /*Status bit that the screen isnt writing anything anymore*/
            SPIbusy = FALSE;
            /*Do Nothing*/
        }
    }
}

/*This clears the screen by writing a bunch of 0x00*/
void LCDClear(void) {
    LCDCursor(0, 0);
    LCDWriteData((uint8_t*) clearArray, 504);
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

/*This function will set the cursor as designated by the user*/
void LCDCursor(uint8_t xCursor, uint8_t yCursor) {

    /*The cursorIndex will be brought back to 0 when it reaches the max array size.
     It will also increment by 2 so that it can save the x and y cursor spaces.*/
    if (cursorIndex >= CURSOR_BUFFER_SIZE) {
        cursorIndex = 0;
    } else {
        cursorIndex = cursorIndex + 2;
    }

    /*Saves the x and y coordinate addresses to be written to the LCD Screen*/
    uint8_t theXCursor = 0x00;
    uint8_t theYCursor = 0x00;

    /*Needs to bitwise with 0x80 since it is a command and designates the x
     coordinate*/
    theXCursor = (0x80 | xCursor);
    /*Needs to bitwise with 0x40 since it is a command and designates the y
     coordinate*/
    theYCursor = (0x40 | yCursor);

    /*It is saving the cursor in the array*/
    cursorArray[cursorIndex] = theXCursor;
    cursorArray[cursorIndex + 1] = theYCursor;

    /*Sets x and y address*/
    LCDWriteCmd((uint8_t*) & cursorArray[cursorIndex], 2);
}

/*This will write a charater to the screen*/
void LCDWriteChar(char character) {
    /*Takes the character from the ASCII array and puts on screen, this is with SPACE*/
    LCDWriteData((uint8_t*) & ASCII_SPACE[character - 0x20][0], 6);

}

/*This will write out the string by using the LCDWriteChar*/
void LCDWriteString(char *characters) {
    while (*characters) {/*Writes out the string of characters*/
        LCDWriteChar(*characters++);
    }
}

/*This will write out a picture to the screen depending on what is inside the
 function.*/
void LCDWriteBitmap(void) {

    LCDWriteData((uint8_t*) BoltZapLogo, 504);

    //LCDWriteData((uint8_t*)Meter, 504);

    //This will need to be changed to work for any specific bitmap we want to
    //use. We will probably implement this by using a series of if statements to
    //check a specific map and the function will have a specific number to write
    //the certain bitmap to draw out to.
}
