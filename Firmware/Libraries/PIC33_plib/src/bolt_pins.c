/* 
 * File:   pins.c
 * Author: Zachary Levenberg
 * Comments:
 * Revision history: initial build 2/4/16
 */

#include "bolt_pins.h"

static uint16_t IO_pinPortAInterruptStatus = 0;
static uint16_t IO_pinPortBInterruptStatus = 0;
#ifdef PORTC
static uint16_t IO_pinPortCInterruptStatus = 0;
#endif

uint8_t IO_setPinDir(pin_number pin, uint8_t direction) {

    uint8_t returnVal = 1;
    if (pin < NUMBER_PINS_PORTA) {
        if (direction) {
            TRISA |= (1 << pin);
        } else {
            TRISA &= ~(1 << pin);
        }
    } else if (pin < NUMBER_PINS_PORTB + NUMBER_PINS_PORTA) {
        pin = pin - NUMBER_PINS_PORTA;
        if (direction) {
            TRISB |= (1 << pin);
        } else {
            TRISB &= ~(1 << pin);
        }
    }
#ifdef PORTC
    else if (pin < NUMBER_PINS_PORTC + NUMBER_PINS_PORTB + NUMBER_PINS_PORTA) {
        pin = pin - NUMBER_PINS_PORTB - NUMBER_PINS_PORTA;
        if (direction) {
            TRISC |= (1 << pin);
        } else {
            TRISC &= ~(1 << pin);
        }
    }
#endif
    else { /*invalid pin*/
        returnVal = 0;
    }
    return returnVal;
}

/*
 Function: IO_setPortDir()
 Params: ports: which port, IO_PORTA or IO_PORTB\
         direction: INPUT or OUTPUT 
 Return: 1 = success, 0 = failure
 Description: sets all pins of an io port to a specific direction
 author: brdgordo; 2/16/16
 */
uint8_t IO_setPortDir(io_port port, uint8_t direction) {
    uint8_t returnVal = 1;
    switch (port) {
        case IO_PORTA:
            if (direction) {
                TRISA = 0xFFFF;
            } else {
                TRISA = 0x0000;
            }
            break;
        case IO_PORTB:
            if (direction) {
                TRISB = 0xFFFF;
            } else {
                TRISB = 0x0000;
            }
            break;
#ifdef PORTC
        case IO_PORTC:
            if (direction) {
                TRISC = 0xFFFF;
            } else {
                TRISC = 0x0000;
            }
            break;
#endif
        default:
            returnVal = 0;
            break;
    }
    return returnVal;
}

uint8_t IO_pinWrite(pin_number pin, uint8_t value) {
    uint8_t returnVal = 1;
    if (pin < NUMBER_PINS_PORTA) {
        if (value) {
            LATA |= (1 << pin);
        } else {
            LATA &= ~(1 << pin);
        }
    } else if (pin < NUMBER_PINS_PORTB + NUMBER_PINS_PORTA) {
        pin = pin - NUMBER_PINS_PORTA;
        if (value) {
            LATB |= (1 << pin);
        } else {
            LATB &= ~(1 << pin);
        }
    }
#ifdef PORTC
    else if (pin < NUMBER_PINS_PORTC + NUMBER_PINS_PORTB + NUMBER_PINS_PORTA) {
        pin = pin - NUMBER_PINS_PORTB - NUMBER_PINS_PORTA;
        if (value) {
            LATC |= (1 << pin);
        } else {
            LATC &= ~(1 << pin);
        }
    }
#endif
    else { /*invalid pin*/
        returnVal = 0;
    }
    return returnVal;
}

/*
 Function: IO_portWrite()
 Params: ports: which port, IO_PORTA or IO_PORTB
         value: HIGH or LOW
 Return: 1 = success, 0 = failure
 Description: writes to all pins of an io port
 author: brdgordo; 2/16/16
 */
uint8_t IO_portWrite(io_port port, uint8_t value) {
    uint8_t returnVal = 1;
    switch (port) {
        case IO_PORTA:
            if (value) {
                LATA = 0xFFFF;
            } else {
                LATA = 0x0000;
            }
            break;
        case IO_PORTB:
            if (value) {
                LATB = 0xFFFF;
            } else {
                LATB = 0x0000;
            }
            break;
#ifdef PORTC
        case IO_PORTC:
            if (value) {
                LATC = 0xFFFF;
            } else {
                LATC = 0x0000;
            }
            break;
#endif
        default:
            returnVal = 0;
            break;
    }
    return returnVal;
}

uint8_t IO_pinToggle(pin_number pin) {
    uint8_t returnVal = 1;
    if (pin < NUMBER_PINS_PORTA) {
        if (LATA & (1 << pin)) {
            LATA &= ~(1 << pin);
        } else {
            LATA |= (1 << pin);
        }
    } else if (pin < NUMBER_PINS_PORTB + NUMBER_PINS_PORTA) {
        pin = pin - NUMBER_PINS_PORTA;
        if (LATB & (1 << pin)) {
            LATB &= ~(1 << pin);
        } else {
            LATB |= (1 << pin);
        }
    }
#ifdef PORTC
    else if (pin < NUMBER_PINS_PORTC + NUMBER_PINS_PORTB + NUMBER_PINS_PORTA) {
        pin = pin - NUMBER_PINS_PORTB - NUMBER_PINS_PORTA;
        if (LATC & (1 << pin)) {
            LATC &= ~(1 << pin);
        } else {
            LATC |= (1 << pin);
        }
    }
#endif
    else { /*invalid pin*/
        returnVal = 0;
    }
    return returnVal;
}

uint8_t IO_pinRead(pin_number pin) {
    uint8_t returnVal = 1;
    if (pin < NUMBER_PINS_PORTA) {
        returnVal = ((PORTA >> pin) & 1);
    } else if (pin < NUMBER_PINS_PORTB + NUMBER_PINS_PORTA) {
        pin = pin - NUMBER_PINS_PORTA;
        returnVal = ((PORTB >> pin) & 1);
    }
#ifdef PORTC
    else if (pin < NUMBER_PINS_PORTC + NUMBER_PINS_PORTB + NUMBER_PINS_PORTA) {
        pin = pin - NUMBER_PINS_PORTB - NUMBER_PINS_PORTA;
        returnVal = ((PORTC >> pin) & 1);
    }
#endif
    else { /*invalid pin*/
        returnVal = 0;
    }
    return returnVal;
}

uint8_t IO_pinPullUpRes(pin_number pin) {
    uint8_t returnVal = 1;
    if (pin < NUMBER_PINS_PORTA) {
        if (TRISA & (1 << pin)) {
            CNPUA |= (1 << pin);
        } else {
            returnVal = 0;
        }
    } else if (pin < NUMBER_PINS_PORTA + NUMBER_PINS_PORTB) {
        pin -= NUMBER_PINS_PORTA;
        if (TRISB & (1 << pin)) {
            CNPUB |= (1 << pin);
        } else {
            returnVal = 0;
        }
    }
#ifdef PORTC
    else if (pin < NUMBER_PINS_PORTC + NUMBER_PINS_PORTB + NUMBER_PINS_PORTA) {
        pin = pin - NUMBER_PINS_PORTB - NUMBER_PINS_PORTA;
        if (TRISC & (1 << pin)) {
            CNPUC |= (1 << pin);
        } else {
            returnVal = 0;
        }
    }
#endif
    else { /* invalid pin */
        returnVal = 0;
    }
    return returnVal;
}

uint8_t IO_pinPullDownRes(pin_number pin) {
    uint8_t returnVal = 1;
    if (pin < NUMBER_PINS_PORTA) {
        if ((TRISA & (1 << pin)) == (1 << pin)) {
            CNPDA |= (1 << pin);
        } else {
            returnVal = 0;
        }
    } else if (pin < NUMBER_PINS_PORTA + NUMBER_PINS_PORTB) {
        pin -= NUMBER_PINS_PORTA;
        if ((TRISB & (1 << pin)) == (1 << pin)) {
            CNPDB |= (1 << pin);
        } else {
            returnVal = 0;
        }
    }
#ifdef PORTC
    else if (pin < NUMBER_PINS_PORTC + NUMBER_PINS_PORTB + NUMBER_PINS_PORTA) {
        pin = pin - NUMBER_PINS_PORTB - NUMBER_PINS_PORTA;
        if (TRISC & (1 << pin)) {
            CNPDC |= (1 << pin);
        } else {
            returnVal = 0;
        }
    }
#endif
    else { /* invalid pin */
        returnVal = 0;
    }
    return returnVal;
}

uint8_t IO_pinInterrupt(pin_number pin) {

    uint8_t returnVal = 1;
    if (pin < NUMBER_PINS_PORTA) {
        CNENA |= (1 << pin);
    } else if (pin < NUMBER_PINS_PORTA + NUMBER_PINS_PORTB) {
        pin -= NUMBER_PINS_PORTA;
        CNENB |= (1 << pin);
    }
#ifdef PORTC
    else if (pin < NUMBER_PINS_PORTC + NUMBER_PINS_PORTB + NUMBER_PINS_PORTA) {
        pin = pin - NUMBER_PINS_PORTB - NUMBER_PINS_PORTA;
        CNENC |= (1 << pin);
    }
#endif
    else { /* invalid pin */
        returnVal = 0;
    }

    _CNIE = 1; /*Enable Pin Interrupt*/
    _CNIF = 0; /*Clear Flags*/

    return returnVal;
}

void __attribute__((__interrupt__,auto_psv)) _CNInterrupt(void) {
    // Insert ISR code here
    IO_pinPortAInterruptStatus =  PORTA;
    IO_pinPortBInterruptStatus = PORTB;
#ifdef PORTC
    IO_pinPortCInterruptStatus = PORTC;
#endif
    _CNIF = 0; // Clear CN interrupt
}