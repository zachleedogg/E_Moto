#include "pins.h"


#define set(reg,val) (*(reg) |= (val))
#define clear(reg, val) (*(reg) &= ~(val))
#define toggle(reg, val) (*(reg) ^= (val))
#define get(reg,val) (*(reg) & (val))

typedef struct _PINS_s {
    volatile uint16_t* tris;
    volatile uint16_t* port;
    volatile uint16_t* lat;
    volatile uint16_t* pu;
    volatile uint16_t* pd;
    volatile uint16_t* inter;
} PINS_s;

/*******************************************************************************
 * PORTA
 */
static const PINS_s ports[NUMBER_OF_PORTS] = {
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


void PIN_Direction(PINS_portNumber_e port, uint8_t pin, PINS_direction_e dir) {
    uint16_t thisPin = 1 << pin;
    if (dir == 0) {
        clear(ports[port].tris, thisPin);
    } else {
        set(ports[port].tris, thisPin);
    }
}

void PIN_Write(PINS_portNumber_e port, uint8_t pin, PINS_state_e state) {
    uint16_t thisPin = (1 << pin);
    switch (state) {
        case LOW:
            clear(ports[port].lat, thisPin);
            break;
        case HIGH:
            set(ports[port].lat, thisPin);
            break;
        case TOGGLE:
            toggle(ports[port].lat, thisPin);
            break;
        default:
            break;
    }
}

PINS_state_e PIN_Read(PINS_portNumber_e port, uint8_t pin) {
    return get(ports[port].port, 1<<pin) >> pin;
}

void PIN_Pulll_Up(PINS_portNumber_e port, uint8_t pin, PINS_state_e state) {
    uint16_t thisPin = 1 << pin;
    if (state == 0) {
        clear(ports[port].pu, thisPin);
    } else {
        set(ports[port].pu, thisPin);
    }
}

void PIN_Pulll_Down(PINS_portNumber_e port, uint8_t pin, PINS_state_e state) {
    uint16_t thisPin = 1 << pin;
    if (state == 0) {
        clear(ports[port].pd, thisPin);
    } else {
        set(ports[port].pd, thisPin);
    }
}

void PINS_set_Interrupt(PINS_portNumber_e port, uint8_t pin, PINS_state_e state) {
    uint16_t thisPin = 1 << pin;
    if (state == 0) {
        clear(ports[port].inter, thisPin);
    } else {
        set(ports[port].inter, thisPin);
    }
}
