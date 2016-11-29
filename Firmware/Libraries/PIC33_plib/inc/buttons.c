#include "buttons.h"
#include "bolt_uart.h"

BUTTONS_status_E BUTTONS_run(BUTTONS_object_S *thisButton) {
    
    uint8_t value = (uint8_t)PINS_read(thisButton->pin.port, thisButton->pin.pin);

    switch (thisButton->status) {
        case BUTTONS_STATUS_OFF:
            if (value == thisButton->polarity) {
                thisButton->debounce++;
                if (thisButton->debounce == thisButton->threshold) {
                    thisButton->status = BUTTONS_STATUS_ON;
                    thisButton->debounce = 0;
                }
            } else {
                thisButton->debounce = 0;
            }
            break;
            
        case BUTTONS_STATUS_ON:
            if (value != thisButton->polarity) {
                thisButton->debounce++;
                if (thisButton->debounce == thisButton->threshold) {
                    thisButton->status = BUTTONS_STATUS_OFF;
                    thisButton->debounce = 0;
                }
            } else {
                thisButton->debounce = 0;
            }
            break;
            
        default:
            break;
    }
    
    return thisButton->status;
}
