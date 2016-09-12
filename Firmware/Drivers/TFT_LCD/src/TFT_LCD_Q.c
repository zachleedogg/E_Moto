/*
 * File:   Queue.c
 * Author: Garrett Deguchi
 *
 * Created on February 20, 2016, 7:11 PM
 */

#include <string.h>
#include <stdio.h>
#include "TFT_LCD_Q.h"

//#define QUEUE_DEBUG

#ifdef QUEUE_DEBUG
#define qprint(...) ({char tempArray[100]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);})
#else
#define qprint(...) 
#endif



/*Change QUEUESIZE varible to work or test*/
#define QUEUESIZE 64
#define TRUE 1
#define FALSE 0

typedef struct _lcdQueue {
    lcdData thisData[QUEUESIZE];
    uint16_t Head;
    uint16_t Tail;
} lcdQueue;

static lcdQueue thisQueue;

/*This function will add to the current Queue whatever is incoming and put it
 into the Queue array.*/
void addToQueue(lcdData newItem) {
    qprint("ins\n");

    if (checkQueueFull()) {/*The Queue is full so it does nothing*/
        qprint("queue full\n");
        return;

    } else {/*The Queue is not full so go along normal process*/
        /*Puts the dataPointer into the dataArray*/
        thisQueue.thisData[thisQueue.Head].Data = newItem.Data;
        /*Puts the length into the lengthArray*/
        thisQueue.thisData[thisQueue.Head].Length = newItem.Length;
        /*Puts the DC value into the DCArray :: DC == 1 means data and
         DC == 0 means commands*/
        thisQueue.thisData[thisQueue.Head].Command = newItem.Command;
        thisQueue.thisData[thisQueue.Head].color = newItem.color;
        thisQueue.thisData[thisQueue.Head].font = newItem.font;
        /*Increment the emptyIndex because it is not longer empty anymore*/
        //qprint("head: %d tail: %d length: %d DC %d\n", thisQueue.Head, thisQueue.Tail, thisQueue.thisData[thisQueue.Head].Length, thisQueue.thisData[thisQueue.Head].Command);
        qprint("head: %d tail: %d\n", thisQueue.Head, thisQueue.Tail);
        thisQueue.Head++;
        if (thisQueue.Head >= QUEUESIZE) {
            thisQueue.Head = 0;
        }
    }

    /*This checks if the emptyIndex is at the last spot*/

}

/*This function will delete the item from the Queue and clear what is in it*/
lcdData deleteFromQueue(void) {
    qprint("del\n");
    lcdData thisItem = {};
    if (checkQueueEmpty()) {/*If the Queue is empty and you delete it wont do anything*/
        qprint("q was empty\n");
        //do nothing
    } else {/*The Queue is not empty*/
        thisItem = thisQueue.thisData[thisQueue.Tail];
        /*Gets rid of the items at the currentIndex*/
//        thisQueue.thisData[thisQueue.Tail].Data = 0;
//        thisQueue.thisData[thisQueue.Tail].Length = 0;
//        thisQueue.thisData[thisQueue.Tail].Command = 0;
        /*Increments the current index because it has no information*/
        thisQueue.Tail++;
        /*This checks if the currentIndex is at the last spot*/
        if (thisQueue.Tail >= QUEUESIZE) {
            thisQueue.Tail = 0;
        }
    }
    return thisItem;
}

lcdData peekAtQueue(void) {
    return thisQueue.thisData[thisQueue.Tail];
}

/*This function will check if the Queue is completely full and cannot add more
 data into the Queue. This always has one empty space in the Queue at all times
 mainly to make sure we know the emptyIndex has not passed the currentIndex*/
uint8_t checkQueueFull(void) {
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
uint8_t checkQueueEmpty(void) {
    if (thisQueue.Head == thisQueue.Tail) {
        return TRUE; /*The Queue IS empty*/
    } else {
        return FALSE; /*The Queue IS NOT empty*/
    }
}