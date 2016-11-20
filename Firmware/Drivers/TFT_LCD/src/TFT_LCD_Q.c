/*
 * File:   Queue.c
 * Author: Garrett Deguchi
 *
 * Created on February 20, 2016, 7:11 PM
 */


#include "TFT_LCD_Q.h"

//#define QUEUE_DEBUG

#ifdef QUEUE_DEBUG
#include <stdio.h>
#define qprint(...) ({char tempArray[100]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);})
#else
#define qprint(...) 
#endif



/*Change QUEUESIZE varible to work or test*/
#define QUEUESIZE 240
#define TRUE 1
#define FALSE 0

typedef struct _lcdQueue {
    LCD_Q_data_S thisData[QUEUESIZE];
    uint16_t Head;
    uint16_t Tail;
} lcdQueue;

static volatile lcdQueue thisQueue = {
    .thisData = {},
    .Head = 0,
    .Tail = 0,
};

/*This function will add to the current Queue whatever is incoming and put it
 into the Queue array.*/
void LCD_Q_addToQueue(LCD_Q_data_S newItem) {
    if (LCD_Q_checkQueueFull()) {/*The Queue is full so it does nothing*/
        qprint("queue full\n");
    } else {/*The Queue is not full so go along normal process*/
        thisQueue.thisData[thisQueue.Head].Data = newItem.Data;
        thisQueue.thisData[thisQueue.Head].Length = newItem.Length;
        thisQueue.thisData[thisQueue.Head].Command = newItem.Command;
        thisQueue.thisData[thisQueue.Head].color = newItem.color;
        thisQueue.thisData[thisQueue.Head].font = newItem.font;
        qprint("head: %d tail: %d\n", thisQueue.Head, thisQueue.Tail);
        thisQueue.Head++;
        if (thisQueue.Head >= QUEUESIZE) {
            thisQueue.Head = 0;
        }
    }
}

/*This function will delete the item from the Queue and clear what is in it*/
LCD_Q_data_S LCD_Q_deleteFromQueue(void) {
    qprint("del\n");
    LCD_Q_data_S thisItem = {};
    if (LCD_Q_checkQueueEmpty()) {/*If the Queue is empty and you delete it wont do anything*/
        qprint("q was empty\n");
        //do nothing
    } else {/*The Queue is not empty*/
        thisItem = thisQueue.thisData[thisQueue.Tail];
        /*Gets rid of the items at the currentIndex*/
        thisQueue.Tail++;
        /*This checks if the currentIndex is at the last spot*/
        if (thisQueue.Tail >= QUEUESIZE) {
            thisQueue.Tail = 0;
        }
    }
    return thisItem;
}

LCD_Q_data_S LCD_Q_peekAtQueue(void) {
    return thisQueue.thisData[thisQueue.Tail];
}

/*This function will check if the Queue is completely full and cannot add more
 data into the Queue. This always has one empty space in the Queue at all times
 mainly to make sure we know the emptyIndex has not passed the currentIndex*/
uint8_t LCD_Q_checkQueueFull(void) {
    /*If the emptyIndex is behind the currentIndex*/
    if ((thisQueue.Tail - 1) == thisQueue.Head) {
        return TRUE; /*The Queue IS full*/
    } else if ((thisQueue.Tail == 0) && (thisQueue.Head == (QUEUESIZE - 1))) {
        return TRUE; /*The Queue IS full*/
    } else {
        return FALSE; /*The Queue IS NOT full*/
    }
}

/*This function will check if the Queue is completely empty*/
uint8_t LCD_Q_checkQueueEmpty(void) {
    if (thisQueue.Head == thisQueue.Tail) {
        return TRUE; /*The Queue IS empty*/
    } else {
        return FALSE; /*The Queue IS NOT empty*/
    }
}