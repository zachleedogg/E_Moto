/* 
 * File:   Queue.h
 * Author: Garrett Deguchi
 *
 * Created on February 20, 2016, 7:11 PM
 */

#ifndef LCD_QUEUE_H
#define	LCD_QUEUE_H

#include <stdint.h>



typedef struct _lcdData {
    uint8_t *Data;
    uint8_t Command;
    uint16_t Length;
} lcdData;

/*This function will add to the current Queue whatever is incoming and put it
 into the Queue array.*/
void addToQueue(lcdData);

lcdData peekAtQueue(void);

/*This function will delete the item from the Queue and clear what is in it*/
lcdData deleteFromQueue(void);

/*This function will check if the Queue is completely full and cannot add more
 data into the Queue. This always has one empty space in the Queue at all times
 mainly to make sure we know the emptyIndex has not passed the currentIndex*/
uint8_t checkQueueFull(void);

/*This function will check if the Queue is completely empty*/
uint8_t checkQueueEmpty(void);

/*This function writes to the SPI and checks the DC values*/
void queueWrite(uint16_t theIndex);

/*This will return the length of the array that is held at currentIndexQueue*/
uint16_t getArrayLength(void);

/*Checks if the empty index is right after currentIndex*/
uint8_t emptyIndexNext(void);

#endif	/* QUEUE_H */

