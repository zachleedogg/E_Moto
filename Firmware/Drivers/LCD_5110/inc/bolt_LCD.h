/*
 * File:   LCD_SPI.c
 * Author: garrettd
 *
 * Created on February 20, 2016, 7:11 PM
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef LCD_SPI_H
#define	LCD_SPI_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

/*This initializes the screen*/
void LCDInit(uint8_t reset, uint8_t CE, uint8_t DC);

/*This clears the screen by writing a bunch of 0x00*/
void LCDClear(void);

/*This function will write a command to the LCD screen ie. DC value is 0*/
void LCDWriteCmd(uint8_t *commandString, uint16_t stringLength);

/*This function will write data to the LCD screen ie. DC value is 1*/
void LCDWriteData(uint8_t *dataString, uint16_t stringLength);

/*This function will set the cursor as designated by the user*/
void LCDCursor(uint8_t xCursor, uint8_t yCursor);

/*This will write a charater to the screen*/
void LCDWriteChar(char character);

/*This will write out the string by using the LCDWriteChar*/
void LCDWriteString(char *characters);

/*This will write out a picture to the screen depending on what is inside the
 function.*/
void LCDWriteBitmap(void);

#endif	/* LCD_SPI_H */

