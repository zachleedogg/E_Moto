#include "pins.h"

inline void PIN_Direction(PIN_pin thisPin, PIN_State_e dir) {
    thisPin.port = dir;
}

inline void PIN_Write(PIN_pin thisPin, PIN_Direction_e state){
    (thisPin.port = state);
}

inline void PIN_Toggle(PIN_pin thisPin) {
    thisPin.lat ? thisPin.port=0 : thisPin.port=1;
}

inline PIN_State_e IO_pinRead(PIN_pin thisPin) {
    return thisPin.lat;
}