#include <xc.h>
#include "TFT_LCD.h"

/*TFT LCD Screen Buffer*/
#define TFT_LCD_WIDTH 320
#define TFT_LCD_HIEGHT 480
#define TFT_RED 0xF800
#define TFT_GRN 0x07E0
#define TFT_BLU 0x001F
#define TFT_LCD_SCREEN_BUFFER_SIZE 16//TFT_LCD_WIDTH*TFT_LCD_HIEGHT*2
uint8_t TxBufferA[TFT_LCD_SCREEN_BUFFER_SIZE] __attribute__((space(xmemory)));

/*Pin allocations*/
static pin_number _CS_;
static pin_number _CD_;
static pin_number _WR_;
static pin_number _RD_;

/*spi peripheral status used for dumb blocking spi transfers*/
static uint8_t spi1Status = 1;

/*Private functions*/
static void writecommand(uint8_t command);
static void writedata(uint8_t data);
static void tftBootUpSequence(void);
static void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
static uint8_t spiTransfer(uint8_t byte);

/*SPI1 Transmission and Reception with DMA 
 * Setup for SPI1 Master Mode: 
 * Interrupt Controller Settings
 * */
uint8_t TFT_LCD_init(uint32_t systemClockSpeed, pin_number CS, pin_number CD, pin_number WR, pin_number RD) {

    /****SPI************/
    SPI1CON1bits.DISSCK = 0; // Internal serial clock is enabled
    SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 0; // Communication is byte-wide (8 bits)
    SPI1CON1bits.MSTEN = 1; // Master mode enabled
    SPI1CON1bits.SMP = 0; // Input data is sampled at the middle of data output time
    SPI1CON1bits.CKE = 1; // Serial output data changes on transition from
    // active clock state to Idle clock state
    SPI1CON1bits.CKP = 0; // Idle state for clock is a low level; 
    // active state is a high level
    SPI1STATbits.SPIEN = 1; // Enable SPI mo

    SPI1CON1bits.PPRE = 0b01; /*clock at slowest speed*/
    SPI1CON1bits.SPRE = 0b111;

    _SPI1IF = 0; // Clear the Interrupt flag
    _SPI1IE = 1; // Enable the interrupt
    _SPI1EIP = 5; //priority 5

    /****DMA************/
    DMA0CONbits.SIZE = 1; /* byte transfer*/
    DMA0CONbits.DIR = 1; /*Writes to peripheral*/
    DMA0CONbits.MODE = 0; /*Continuous mode*/
    DMA0STAL = (unsigned int) &TxBufferA; /*start address LOW*/
    DMA0STAH = (unsigned int) &TxBufferA; /*start address High*/
    DMA0PAD = (volatile unsigned int) &SPI1BUF; /*DMA destination address*/
    DMA0CNT = TFT_LCD_SCREEN_BUFFER_SIZE; /*DMA counter*/
    DMA0REQ = 0x000A; /*Transfer on SPI_1 transfer complete*/

    IFS0bits.DMA0IF = 0; /*Clear interupt flag*/
    IEC0bits.DMA0IE = 0; /*Do no enable interupts*/

    tftBootUpSequence();
    _SPI1IE = 0; /* Disable the SPI interrupt*/

    /*Force the system to start by transferring data to the SPI
     and set the chip select pin on permanent LOW*/
    DMA0CONbits.CHEN = 1; /*Enable the DMA*/
    IO_pinWrite(_CS_, LOW);
    IO_pinWrite(_WR_, HIGH);
    IO_pinWrite(_CD_, LOW);
    spiTransfer(HX8357_RAMWR); /* write to tftRAM to start DMAa*/
    IO_pinWrite(_CD_, HIGH);
    return 0;
}

void fillScreen(uint16_t color) {
  fillRect(0, 0, TFT_LCD_WIDTH, TFT_LCD_HIEGHT, color);
}

// fill a rectangle
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= TFT_LCD_WIDTH) || (y >= TFT_LCD_HIEGHT)) return;
  if((x + w - 1) >= TFT_LCD_WIDTH)  w = TFT_LCD_WIDTH  - x;
  if((y + h - 1) >= TFT_LCD_HIEGHT) h = TFT_LCD_HIEGHT - y;

  setAddrWindow(x, y, x+w-1, y+h-1);

  uint8_t hi = color >> 8, lo = color;

  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      spiTransfer(hi);
      spiTransfer(lo);
    }
  }
}

static void tftBootUpSequence(void) {
    writecommand(HX8357_SWRESET);
    asm volatile("REPEAT #300");
    // setextc
    writecommand(HX8357D_SETC);
    writedata(0xFF);
    writedata(0x83);
    writedata(0x57);
    asm volatile("REPEAT #300");
    // setRGB which also enables SDO
    writecommand(HX8357_SETRGB);
    writedata(0x80); //enable SDO pin!
    //    writedata(0x00);  //disable SDO pin!
    writedata(0x0);
    writedata(0x06);
    writedata(0x06);

    writecommand(HX8357D_SETCOM);
    writedata(0x25); // -1.52V

    writecommand(HX8357_SETOSC);
    writedata(0x68); // Normal mode 70Hz, Idle mode 55 Hz

    writecommand(HX8357_SETPANEL); //Set Panel
    writedata(0x05); // BGR, Gate direction swapped

    writecommand(HX8357_SETPWR1);
    writedata(0x00); // Not deep standby
    writedata(0x15); //BT
    writedata(0x1C); //VSPR
    writedata(0x1C); //VSNR
    writedata(0x83); //AP
    writedata(0xAA); //FS

    writecommand(HX8357D_SETSTBA);
    writedata(0x50); //OPON normal
    writedata(0x50); //OPON idle
    writedata(0x01); //STBA
    writedata(0x3C); //STBA
    writedata(0x1E); //STBA
    writedata(0x08); //GEN

    writecommand(HX8357D_SETCYC);
    writedata(0x02); //NW 0x02
    writedata(0x40); //RTN
    writedata(0x00); //DIV
    writedata(0x2A); //DUM
    writedata(0x2A); //DUM
    writedata(0x0D); //GDON
    writedata(0x78); //GDOFF

    writecommand(HX8357D_SETGAMMA);
    writedata(0x02);
    writedata(0x0A);
    writedata(0x11);
    writedata(0x1d);
    writedata(0x23);
    writedata(0x35);
    writedata(0x41);
    writedata(0x4b);
    writedata(0x4b);
    writedata(0x42);
    writedata(0x3A);
    writedata(0x27);
    writedata(0x1B);
    writedata(0x08);
    writedata(0x09);
    writedata(0x03);
    writedata(0x02);
    writedata(0x0A);
    writedata(0x11);
    writedata(0x1d);
    writedata(0x23);
    writedata(0x35);
    writedata(0x41);
    writedata(0x4b);
    writedata(0x4b);
    writedata(0x42);
    writedata(0x3A);
    writedata(0x27);
    writedata(0x1B);
    writedata(0x08);
    writedata(0x09);
    writedata(0x03);
    writedata(0x00);
    writedata(0x01);

    writecommand(HX8357_COLMOD);
    writedata(0x55); // 16 bit

    writecommand(HX8357_MADCTL);
    writedata(0xC0);

    writecommand(HX8357_TEON); // TE off
    writedata(0x00);

    writecommand(HX8357_TEARLINE); // tear line
    writedata(0x00);
    writedata(0x02);

    writecommand(HX8357_SLPOUT); //Exit Sleep
    asm volatile("REPEAT #150");

    writecommand(HX8357_DISPON); // display on
    asm volatile("REPEAT #150");

    setAddrWindow(0, 0, 320, 480);
}

static void writecommand(uint8_t command) {
    IO_pinWrite(_CS_, LOW);
    IO_pinWrite(_CD_, LOW);
    spiTransfer(command);
    IO_pinWrite(_CS_, HIGH);
    IO_pinWrite(_CD_, HIGH);
}

static void writedata(uint8_t data) {
    IO_pinWrite(_CS_, LOW);
    spiTransfer(data);
    IO_pinWrite(_CS_, HIGH);
}

static void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

    writecommand(HX8357_CASET); // Column addr set
    writedata(x0 >> 8);
    writedata(x0 & 0xFF); // XSTART 
    writedata(x1 >> 8);
    writedata(x1 & 0xFF); // XEND

    writecommand(HX8357_PASET); // Row addr set
    writedata(y0 >> 8);
    writedata(y0); // YSTART
    writedata(y1 >> 8);
    writedata(y1); // YEND
}

uint8_t spiTransfer(uint8_t byte) {
    spi1Status = 0;
    SPI1BUF = byte;
    while (!spi1Status) {
        ;
    }
    return (uint8_t) SPI1BUF;
}

void __attribute__((__interrupt__, auto_psv)) _SPI1Interrupt(void) {
    _SPI1IF = 0; /* Clear the Interrupt flag*/
    spi1Status = 1;
    uint16_t temp = SPI1BUF; /*clear input buffer because it just always fills up*/
    temp = 0; /*Suppress Warning*/
}