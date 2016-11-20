/* 
 * File:   Queue.h
 * Author: Zachary S. Levenberg
 * Created on September 6, 2016, 7:11 PM
 */

#ifndef LCD_QUEUE_H
#define	LCD_QUEUE_H

/*******************************************************************************
 * Include
 * ****************************************************************************/

#include <stdint.h>

/*******************************************************************************
 * Defines and Datatypes
 * ****************************************************************************/

typedef struct _LCD_Q_data_S {
    uint16_t *Data;
    uint8_t Command;
    uint32_t Length;
    uint8_t font;
    uint16_t color;
} LCD_Q_data_S;

/*******************************************************************************
 * Function Prototypes
 * ****************************************************************************/

/**
 * 
 * @param 
 */
void LCD_Q_addToQueue(LCD_Q_data_S);

/**
 * 
 * @return 
 */
LCD_Q_data_S LCD_Q_peekAtQueue(void);

/**
 * 
 * @return 
 */
LCD_Q_data_S LCD_Q_deleteFromQueue(void);

/**
 * 
 * @return 
 */
uint8_t LCD_Q_checkQueueFull(void);

/**
 * 
 * @return 
 */
uint8_t LCD_Q_checkQueueEmpty(void);

#endif	/* QUEUE_H */

