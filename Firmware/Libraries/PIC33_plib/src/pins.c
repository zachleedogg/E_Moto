#include "pins.h"


#define set(reg,val) (*(reg) |= (val))
#define clear(reg, val) (*(reg) &= ~(val))
#define toggle(reg, val) (*(reg) ^= (val))
#define get(reg,val) (*(reg) & (val))

typedef struct _PINS_internalRegisters_S {
    volatile uint16_t* const tris;
    volatile uint16_t* const port;
    volatile uint16_t* const lat;
    volatile uint16_t* const pu;
    volatile uint16_t* const pd;
    volatile uint16_t* const inter;
} PINS_internalRegisters_S;

/*******************************************************************************
 * PORTA
 */
static const PINS_internalRegisters_S PINS_portsArray[PINS_NUMBER_OF_PORTS] = {
#ifdef PORTA
    [PIN_PORTA].tris = &TRISA,
    [PIN_PORTA].port = &PORTA,
    [PIN_PORTA].lat = &LATA,
    [PIN_PORTA].pu = &CNPUA,
    [PIN_PORTA].pd = &CNPDA,
    [PIN_PORTA].inter = &CNENA,
#endif
#ifdef PORTB
    [PIN_PORTB].tris = &TRISB,
    [PIN_PORTB].port = &PORTB,
    [PIN_PORTB].lat = &LATB,
    [PIN_PORTB].pu = &CNPUB,
    [PIN_PORTB].pd = &CNPDB,
    [PIN_PORTB].inter = &CNENB,
#endif
#ifdef PORTC
    [PIN_PORTC].tris = &TRISC,
    [PIN_PORTC].port = &PORTC,
    [PIN_PORTC].lat = &LATC,
    [PIN_PORTC].pu = &CNPUC,
    [PIN_PORTC].pd = &CNPDC,
    [PIN_PORTC].inter = &CNENC,
#endif
#ifdef PORTD
    [PIN_PORTD].tris = &TRISD,
    [PIN_PORTD].port = &PORTD,
    [PIN_PORTD].lat = &LATD,
    [PIN_PORTD].pu = &CNPUD,
    [PIN_PORTD].pd = &CNPDD,
    [PIN_PORTD].inter = &CNEND,
#endif
#ifdef PORTE
    [PIN_PORTE].tris = &TRISE,
    [PIN_PORTE].port = &PORTE,
    [PIN_PORTE].lat = &LATE,
    [PIN_PORTE].pu = &CNPUE,
    [PIN_PORTE].pd = &CNPDE,
    [PIN_PORTE].inter = &CNENE,
#endif
#ifdef PORTF
    [PIN_PORTF].tris = &TRISF,
    [PIN_PORTF].port = &PORTF,
    [PIN_PORTF].lat = &LATF,
    [PIN_PORTF].pu = &CNPUF,
    [PIN_PORTF].pd = &CNPDF,
    [PIN_PORTF].inter = &CNENF,
#endif
#ifdef PORTG
    [PIN_PORTG].tris = &TRISG,
    [PIN_PORTG].port = &PORTG,
    [PIN_PORTG].lat = &LATG,
    [PIN_PORTG].pu = &CNPUG,
    [PIN_PORTG].pd = &CNPDG,
    [PIN_PORTG].inter = &CNENG,
#endif
};


void PINS_direction(PINS_pin_S pin, PINS_direction_E dir) {
    uint16_t thisPin = 1 << pin.pin;
    if (dir == 0) {
        clear(PINS_portsArray[pin.port].tris, thisPin);
    } else {
        set(PINS_portsArray[pin.port].tris, thisPin);
    }
}

void PINS_write(PINS_pin_S pin, PINS_internalRegisters_State_E state) {
    uint16_t thisPin = (1 << pin.pin);
    switch (state) {
        case LOW:
            clear(PINS_portsArray[pin.port].lat, thisPin);
            break;
        case HIGH:
            set(PINS_portsArray[pin.port].lat, thisPin);
            break;
        case TOGGLE:
            toggle(PINS_portsArray[pin.port].lat, thisPin);
            break;
        default:
            break;
    }
}

PINS_internalRegisters_State_E PINS_read(PINS_pin_S pin) {
    return get(PINS_portsArray[pin.port].port, 1<<pin.pin) >> pin.pin;
}

void PINS_pullUp(PINS_pin_S pin, PINS_internalRegisters_State_E state) {
    uint16_t thisPin = 1 << pin.pin;
    if (state == 0) {
        clear(PINS_portsArray[pin.port].pu, thisPin);
    } else {
        set(PINS_portsArray[pin.port].pu, thisPin);
    }
}

void PINS_pullDown(PINS_pin_S pin, PINS_internalRegisters_State_E state) {
    uint16_t thisPin = 1 << pin.pin;
    if (state == 0) {
        clear(PINS_portsArray[pin.port].pd, thisPin);
    } else {
        set(PINS_portsArray[pin.port].pd, thisPin);
    }
}

void PINS_internalRegisters_SetInterrupt(PINS_pin_S pin, PINS_internalRegisters_State_E state) {
    uint16_t thisPin = 1 << pin.pin;
    if (state == 0) {
        clear(PINS_portsArray[pin.port].inter, thisPin);
    } else {
        set(PINS_portsArray[pin.port].inter, thisPin);
    }
}
