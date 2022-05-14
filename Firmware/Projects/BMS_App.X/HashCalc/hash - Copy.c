#include "stdio.h"

int main() {
    
    char str[] = "gpio v3 12";
    char port;
    uint16_t pin;
    uint16_t state;
    uint8_t num = sscanf(str, "%*s %c%u %u", &port, &pin, &state);

    printf("num %d\nport %c \npin %d \nstate %d", num, port, pin, state);
}
