#include "led_driver.h"
#include "pins.h"
#include "bolt_uart.h"

/*Output PPS registers*/
#define RP20_LED_PPS    _RP20R
#define RP35_LED_PPS    _RP35R
#define RP36_LED_PPS    _RP36R
#define RP37_LED_PPS    _RP37R
#define RP38_LED_PPS    _RP38R
#define RP39_LED_PPS    _RP39R
#define RP40_LED_PPS    _RP40R
#define RP41_LED_PPS    _RP41R
#define RP42_LED_PPS    _RP42R
#define RP43_LED_PPS    _RP43R
#define RP54_LED_PPS    _RP54R
#define RP55_LED_PPS    _RP55R
#define RP56_LED_PPS    _RP56R
#define RP57_LED_PPS    _RP57R
#define RP43_LED_PPS    _RP43R
#define RP54_LED_PPS    _RP54R
#define RP55_LED_PPS    _RP55R
#define RP56_LED_PPS    _RP56R
#define RP57_LED_PPS    _RP57R
#define RP97_LED_PPS    _RP97R
#define RP118_LED_PPS    _RP118R
#define RP120_LED_PPS    _RP120R


/*Output PPS functions*/
#define SCK2 0x09
#define SDO2 0x08


#define  SEC_PRESCAL_1_1        0b111  /* Secondary Prescale 1:1   */
#define  SEC_PRESCAL_2_1        0b110  /* Secondary Prescale 2:1   */
#define  SEC_PRESCAL_3_1        0b101  /* Secondary Prescale 2:1   */
#define  SEC_PRESCAL_4_1        0b100  /* Secondary Prescale 4:1   */
#define  SEC_PRESCAL_5_1        0b011  /* Secondary Prescale 5:1   */
#define  SEC_PRESCAL_6_1        0b010  /* Secondary Prescale 6:1   */
#define  SEC_PRESCAL_7_1        0b001  /* Secondary Prescale 7:1   */
#define  SEC_PRESCAL_8_1        0b000  /* Secondary Prescale 8:1   */

#define  PRI_PRESCAL_1_1        0b11  /* Primary Prescale 1:1     */
#define  PRI_PRESCAL_4_1        0b10  /* Primary Prescale 4:1     */
#define  PRI_PRESCAL_16_1       0b01  /* Primary Prescale 16:1    */
#define  PRI_PRESCAL_64_1       0b00  /* Primary Prescale 64:1    */

#define LED_PATTERN_SIZE 255

static const uint8_t ppsOut[2] = {SCK2, SDO2};

typedef struct _ledPatternBuffer {
    uint16_t patternArray[2][LED_PATTERN_SIZE];
    uint16_t Head;
    uint16_t Tail;
} ledPatternBuffer;

ledPatternBuffer LedBuffer = {};

static PINS_pin_S latch;

static uint8_t ledStatus = 0;

void spi2Write(uint16_t input);
uint8_t spi2Init(led_pin_number CLK, led_pin_number D_OUT);

uint8_t ledDriverInit(led_pin_number CLK, led_pin_number DIN, PINS_pin_S LE) {
    spi2Init(CLK, DIN); /*initalize SPI*/
    latch = LE;
    PINS_direction(latch, OUTPUT); /*set pin directions*/
    PINS_write(latch, LOW);
    return 0;
}

void ledDriverWrite(ledArray thisArray, uint16_t pattern) {
        spi2Write(pattern);
}

uint8_t ledDriverIsBusy(void) {
    return ledStatus;
}

void __attribute__((__interrupt__, auto_psv)) _SPI2Interrupt(void) {
    _SPI2IF = 0; /* Clear the Interrupt flag*/

    /*Latch the Data*/
    PINS_write(latch, HIGH);
    PINS_write(latch, LOW);

    /*Clear Rx FIFO buffer*/
//    while (SPI2STATbits.SPITBF == 1) {
//        uint16_t temp = SPI2BUF;
//        temp = 0;
//    }

}

void spi2Write(uint16_t input) {
    SPI2BUF = input;
}

uint8_t spi2Init(led_pin_number CLK, led_pin_number D_OUT) {
    
    //check duplicate pins
    if (CLK == D_OUT) {
        return 1;
    }
    
    //CLOCK and D_OUT
    led_pin_number curPin[] = {CLK, D_OUT};

    int i = 0;
    for (i = 0; i < 2; i++) {
        switch (curPin[i]) {
            case RP20_LED:
                RP20_LED_PPS = ppsOut[i];
                break;
            case RP35_LED:
                RP35_LED_PPS = ppsOut[i];
                break;
            case RP36_LED:
                RP36_LED_PPS = ppsOut[i];
                break;
            case RP37_LED:
                RP37_LED_PPS = ppsOut[i];
                break;
            case RP38_LED:
                RP38_LED_PPS = ppsOut[i];
                break;
            case RP39_LED:
                RP39_LED_PPS = ppsOut[i];
                break;
            case RP40_LED:
                RP40_LED_PPS = ppsOut[i];
                break;
            case RP41_LED:
                RP40_LED_PPS = ppsOut[i];
                break;
            case RP42_LED:
                RP42_LED_PPS = ppsOut[i];
                break;
            case RP43_LED:
                RP43_LED_PPS = ppsOut[i];
                break;
#ifdef _RP54R
            case RP54_LED:
                RP54_LED_PPS = ppsOut[i];
                break;
#endif
#ifdef _RP55R
            case RP55_LED:
                RP55_LED_PPS = ppsOut[i];
                break;
#endif
#ifdef _RP56R
            case RP56_LED:
                RP56_LED_PPS = ppsOut[i];
                break;
#endif
#ifdef _RP57R
            case RP57_LED:
                RP57_LED_PPS = ppsOut[i];
                break;
#endif
                #ifdef _RP97R
            case RP97_LED:
                RP97_LED_PPS = ppsOut[i];
                break;
#endif
#ifdef _RP118R
            case RP118_LED:
                RP118_LED_PPS = ppsOut[i];
                break;
#endif
#ifdef _RP120R
            case RP120_LED:
                RP120_LED_PPS = ppsOut[i];
                break;
#endif
            default:
                return 1;
                break;
        }
    }

    _SPI2IF = 0; // Clear the Interrupt flag
    _SPI2IE = 0; // Disable the interrupt

    SPI2CON1bits.DISSCK = 0; /*clock enabled*/
    SPI2CON1bits.DISSDO = 0; /*Data out enabled*/
    SPI2CON1bits.MODE16 = 1; /*16 bit mode*/
    SPI2CON1bits.CKE = 1; /*data out changes on falling edge*/
    SPI2CON1bits.CKP = 0; /*active high clock*/
    SPI2CON1bits.MSTEN = 1; /*master mode enable*/

    SPI2CON1bits.PPRE = 0b11;
    SPI2CON1bits.SPRE = 0b110;

    SPI2CON2bits.SPIBEN = 1; /*Enhanced Buffer Mode*/
    SPI2STATbits.SPIEN = 1; /*Enable SPI module*/
    SPI2STATbits.SISEL = 0b101; /*interupt on last bit out*/

    _SPI2IF = 0; // Clear the Interrupt flag
    _SPI2IE = 1; // Ensable the interrupt
    _SI2C1IP = 3; //priority 5

    return 0;
}

