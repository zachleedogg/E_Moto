/* 
 * File:   button.h
 * Author: kid group
 *
 * Created on June 12, 2018, 10:38 PM
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include <stdint.h>

typedef enum {
    BUTTON_NOT_PRESSED,
    BUTTON_PRESSED,
    BUTTON_HELD,
    BUTTON_RELEASED
} ButtonStatus_E;

typedef struct{
    uint16_t count;
    ButtonStatus_E state;
    uint16_t debounceTime;
    uint16_t holdTime;
    uint8_t (*pinFunction)(void);
} Button;


/**
 * NEW_BUTTON creates a Button Object
 * @param name: Name of button
 * @param debounceT: debounce counter, depends on how often function is called
 * @param holdT: hold time counter, depends on how often the function is called
 * @param function: The function that returns the GPIO value of the button
 */
#define NEW_BUTTON(name,debounceT,holdT,function) \
static Button name##Object = { \
    .count = 0, \
    .state = BUTTON_NOT_PRESSED, \
    .debounceTime = debounceT, \
    .holdTime = holdT, \
    .pinFunction = function \
}; \
static Button * name = &name##Object\

/**
 * Call buttonRun with a button object.
 * @param thisButton
 */
void buttonRun(Button * thisButton);

void buttonSetState(Button * thisButton, ButtonStatus_E state);

ButtonStatus_E buttonGetState(Button * thisButton);

#endif	/* BUTTON_H */

