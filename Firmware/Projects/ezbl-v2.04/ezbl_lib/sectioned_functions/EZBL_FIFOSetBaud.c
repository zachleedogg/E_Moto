/*******************************************************************************
  Multipurpose FIFO Source Library

  Summary:
    Implements the EZBL_FIFOSetBaud() API in ezbl_lib for configuring UART FIFO
    baud rates and/or enabling hardware auto-baud.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Copyright (C) 2017 Microchip Technology Inc.

  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
  derivatives created by any person or entity by or on your behalf, exclusively
  with Microchip's products.  Microchip and its licensors retain all ownership
  and intellectual property rights in the accompanying software and in all
  derivatives here to.

  This software and any accompanying information is for suggestion only.  It
  does not modify Microchip's standard warranty for its products.  You agree
  that you are solely responsible for testing the software and determining its
  suitability.  Microchip has no obligation to modify, test, certify, or
  support the software.

  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
  TERMS.
*******************************************************************************/
// DOM-IGNORE-END


#if defined(EZBL_LIB16CH) || defined(__dsPIC33CH__) || defined(__generic_16dsp_ch__) // All dsPIC33C devices
    #define UART_SILICON    3
#elif defined(__XC32__)     // All PIC32MM devices
    #define UART_SILICON    2
#elif defined(__XC16__)     // All 16-bit devices except dsPIC33C class devices (generic-16bit, generic-16bit-da, generic-16bit-ep)
    #define UART_SILICON    1
#endif


#if UART_SILICON == 1   // All 16-bit devices except dsPIC33C (generic-16bit, generic-16bit-da, generic-16bit-ep)
    typedef struct
    {
        // UxMODE
        unsigned STSEL:1;   // STSEL: Stop Bit Selection bit <br>1 = Two Stop bits <br>0 = One Stop bit
        unsigned PDSEL:2;   // PDSEL<1:0>: Parity and Data Selection bits <br>0x3 = 9-bit data, no parity <br>0x2 = 8-bit data, odd parity <br>0x1 = 8-bit data, even parity <br>0x0 = 8-bit data, no parity
        unsigned BRGH:1;    // BRGH: High Baud Rate Enable bit <br>1 = BRG generates 4 clocks per bit period (4x baud clock, High-Speed mode) <br>0 = BRG generates 16 clocks per bit period (16x baud clock, Standard mode)
        unsigned URXINV:1;  // URXINV: Receive Polarity Inversion bit (AKA 'RXINV') <br>1 = UxRX Idle state is ‘0’ <br>0 = UxRX Idle state is ‘1’
        unsigned ABAUD:1;   // ABAUD: Auto-Baud Enable bit <br>1 = Enables baud rate measurement on the next character – requires reception of a Sync field (55h) before other data; cleared in hardware upon completion <br>0 = Baud rate measurement is disabled or completed
        unsigned LPBACK:1;  // LPBACK: UARTx Loopback Mode Select bit <br>1 = Enables Loopback mode <br>0 = Loopback mode is disabled
        unsigned WAKE:1;    // WAKE: Wake-up on Start bit Detect During Sleep Mode Enable bit <br>1 = UARTx continues to sample the UxRX pin, interrupt is generated on the falling edge; bit is cleared in hardware on the following rising edge <br>0 = No wake-up is enabled
        unsigned UEN:2;     // UEN<1:0>: UARTx Enable bits <br>0x3 = UxTX, UxRX and BCLKx pins are enabled and used; UxCTS pin is controlled by PORT latches <br>0x2 = UxTX, UxRX, UxCTS and UxRTS pins are enabled and used <br>0x1 = UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin is controlled by PORT latches <br>0x0 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLKx pins are controlled by PORT latches
        unsigned :1;        // Unimplemented: Read as ‘0’
        unsigned RTSMD:1;   // RTSMD: Mode Selection for UxRTS Pin bit <br>1 = UxRTS pin is in Simplex mode <br>0 = UxRTS pin is in Flow Control mode
        unsigned IREN:1;    // IREN: IrDA® Encoder and Decoder Enable bit <br>1 = IrDA encoder and decoder are enabled <br>0 = IrDA encoder and decoder are disabled
        unsigned SIDL:1;    // SIDL: Stop in Idle Mode bit <br>1 = Discontinues module operation when device enters Idle mode <br>0 = Continues module operation in Idle mode
        unsigned FREEZE:1;  // Unimplemented: Read as ‘0’, OR in Debug Mode: Freeze peripheral during debug halt
        unsigned UARTEN:1;  // UARTEN: UARTx Enable bit (AKA 'ON') <br>1 = UARTx is enabled; all UARTx pins are controlled by UARTx as defined by UEN<1:0> <br>0 = UARTx is disabled; all UARTx pins are controlled by PORT latches; UARTx power consumption is minimal

        // UxSTA
        unsigned RXDA:1;      // bit 0 RXDA: Receive Buffer Data Available bit (read-only) <br>1 = Receive buffer has data, at least one more character can be read <br>0 = Receive buffer is empty
        unsigned OERR:1;      // bit 1 OERR: Receive Buffer Overrun Error Status bit (read/clear only) <br>1 = Receive buffer has overflowed <br>0 = Receive buffer has not overflowed. Clearing a previously set OERR bit (1 → 0 transition) resets the receiver buffer and the UxRSR to the empty state.
        unsigned FERR:1;      // bit 2 FERR: Framing Error Status bit (read-only) <br>1 = Framing error has been detected for the current character (character at the top of the receive FIFO) <br>0 = Framing error has not been detected
        unsigned PERR:1;      // bit 3 PERR: Parity Error Status bit (read-only) <br>1 = Parity error has been detected for the current character (character at the top of the receive FIFO) <br>0 = Parity error has not been detected
        unsigned RIDLE:1;     // bit 4 RIDLE: Receiver Idle bit (read-only) <br>1 = Receiver is Idle <br>0 = Receiver is active
        unsigned ADDEN:1;     // bit 5 ADDEN: Address Character Detect bit (bit 8 of received data = 1) <br>1 = Address Detect mode enabled. If 9-bit mode is not selected, this does not take effect <br>0 = Address Detect mode disabled
        unsigned URXISEL:2;   // bit 7-6 URXISEL<1:0>: Receive Interrupt Mode Selection bits <br>0x3 = Interrupt is set on UxRSR transfer making the receive buffer full (i.e., has 4 data characters) <br>0x2 = Interrupt is set on UxRSR transfer making the receive buffer 3/4 full (i.e., has 3 data characters) <br>0x0 or 0x1 = Interrupt is set when any character is received and transferred from the UxRSR to the receive buffer. Receive buffer has one or more characters.
        unsigned TRMT:1;      // bit 8 TRMT: Transmit Shift Register Empty bit (read-only) <br>1 = Transmit Shift Register is empty and transmit buffer is empty (the last transmission has completed) <br>0 = Transmit Shift Register is not empty, a transmission is in progress or queued
        unsigned UTXBF:1;     // bit 9 UTXBF: Transmit Buffer Full Status bit (read-only) <br>1 = Transmit buffer is full <br>0 = Transmit buffer is not full, at least one more character can be written
        unsigned UTXEN:1;     // bit 10 UTXEN: Transmit Enable bit <br>1 = Transmit enabled, UxTX pin controlled by UARTx <br>0 = Transmit disabled, any pending transmission is aborted and buffer is reset. UxTX pin controlled by port
        unsigned UTXBRK:1;    // bit 11 UTXBRK: Transmit Break bit <br>1 = Send Sync Break on next transmission – Start bit, followed by twelve ‘0’ bits, followed by Stop bit; cleared by hardware upon completion <br>0 = Sync Break transmission disabled or completed
        unsigned URXEN:1;     // bit 12 Varies by device: Unimplemented: Read as ‘0’, OR, URXEN: UARTx Receiver Enable bit <br>1 = UARTx receiver is enabled, UxRX pin is controlled by UARTx (if ON = 1) <br>0 = UARTx receiver is disabled, UxRX pin is ignored by the UARTx module
        unsigned UTXISEL0:1;  // bit 15,13 UTXISEL<1:0>: Transmission Interrupt Mode Selection bits <br>0x3 = Reserved; do not use <br>0x2 = Interrupt when a character is transferred to the Transmit Shift register, and as a result, the transmit buffer becomes empty <br>0x1 = Interrupt when the last character is shifted out of the Transmit Shift register; all transmit operations are completed <br>0x0 = Interrupt when a character is transferred to the Transmit Shift register (this implies there is at least one character open in the transmit buffer)
        unsigned UTXINV:1;    // bit 14 UTXINV: Transmit Polarity Inversion bit <br>If IREN = 0: <br>1 = UxTX Idle state is ‘0’ <br>0 = UxTX Idle state is ‘1’ <br>If IREN = 1: <br>1 = IrDA® encoded UxTX Idle state is ‘1’ <br>0 = IrDA® encoded UxTX Idle state is ‘0’
        unsigned UTXISEL1:1;  // bit 15,13 UTXISEL<1:0>: Transmission Interrupt Mode Selection bits <br>0x3 = Reserved; do not use <br>0x2 = Interrupt when a character is transferred to the Transmit Shift register, and as a result, the transmit buffer becomes empty <br>0x1 = Interrupt when the last character is shifted out of the Transmit Shift register; all transmit operations are completed <br>0x0 = Interrupt when a character is transferred to the Transmit Shift register (this implies there is at least one character open in the transmit buffer)

        // UxTXREG
        unsigned TXDATA:9;
        unsigned :7;

        // UxRXREG
        unsigned RXDATA:9;
        unsigned :7;

        // UxBRG
        unsigned int BRG;
    } UART_SFRS;

#elif UART_SILICON == 2 // All PIC32MM devices
    typedef struct
    {
        // UxMODE
        unsigned STSEL:1;   // STSEL: Stop Bit Selection bit <br>1 = Two Stop bits <br>0 = One Stop bit
        unsigned PDSEL:2;   // PDSEL<1:0>: Parity and Data Selection bits <br>0x3 = 9-bit data, no parity <br>0x2 = 8-bit data, odd parity <br>0x1 = 8-bit data, even parity <br>0x0 = 8-bit data, no parity
        unsigned BRGH:1;    // BRGH: High Baud Rate Enable bit <br>1 = BRG generates 4 clocks per bit period (4x baud clock, High-Speed mode) <br>0 = BRG generates 16 clocks per bit period (16x baud clock, Standard mode)
        unsigned URXINV:1;  // URXINV: Receive Polarity Inversion bit (AKA 'RXINV') <br>1 = UxRX Idle state is ‘0’ <br>0 = UxRX Idle state is ‘1’
        unsigned ABAUD:1;   // ABAUD: Auto-Baud Enable bit <br>1 = Enables baud rate measurement on the next character – requires reception of a Sync field (55h) before other data; cleared in hardware upon completion <br>0 = Baud rate measurement is disabled or completed
        unsigned LPBACK:1;  // LPBACK: UARTx Loopback Mode Select bit <br>1 = Enables Loopback mode <br>0 = Loopback mode is disabled
        unsigned WAKE:1;    // WAKE: Wake-up on Start bit Detect During Sleep Mode Enable bit <br>1 = UARTx continues to sample the UxRX pin, interrupt is generated on the falling edge; bit is cleared in hardware on the following rising edge <br>0 = No wake-up is enabled
        unsigned UEN:2;     // UEN<1:0>: UARTx Enable bits <br>0x3 = UxTX, UxRX and BCLKx pins are enabled and used; UxCTS pin is controlled by PORT latches <br>0x2 = UxTX, UxRX, UxCTS and UxRTS pins are enabled and used <br>0x1 = UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin is controlled by PORT latches <br>0x0 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLKx pins are controlled by PORT latches
        unsigned :1;        // Unimplemented: Read as ‘0’
        unsigned RTSMD:1;   // RTSMD: Mode Selection for UxRTS Pin bit <br>1 = UxRTS pin is in Simplex mode <br>0 = UxRTS pin is in Flow Control mode
        unsigned IREN:1;    // IREN: IrDA® Encoder and Decoder Enable bit <br>1 = IrDA encoder and decoder are enabled <br>0 = IrDA encoder and decoder are disabled
        unsigned SIDL:1;    // SIDL: Stop in Idle Mode bit <br>1 = Discontinues module operation when device enters Idle mode <br>0 = Continues module operation in Idle mode
        unsigned FREEZE:1;  // Unimplemented: Read as ‘0’, OR in Debug Mode: Freeze peripheral during debug halt
        unsigned UARTEN:1;  // UARTEN: UARTx Enable bit (AKA 'ON') <br>1 = UARTx is enabled; all UARTx pins are controlled by UARTx as defined by UEN<1:0> <br>0 = UARTx is disabled; all UARTx pins are controlled by PORT latches; UARTx power consumption is minimal

        unsigned OVFDIS:1;  // OVFDIS: Run During Overflow Condition Mode bit <br>1 = When an Overflow Error (OERR) condition is detected, the Shift register continues to run to remain synchronized <br>0 = When an Overflow Error (OERR) condition is detected, the Shift register stops accepting new data (Legacy mode)
        unsigned CLKSEL:2;  // CLKSEL<1:0>: UARTx Clock Selection bits <br>0x3 = The UARTx clock is the Reference Clock Output (REFCLKO) <br>0x2 = The UARTx clock is the FRC oscillator clock <br>0x1 = The UARTx clock is the SYSCLK <br>0x0 = The UARTx clock is the PBCLK (1:1 with SYSCLK)
        unsigned :3;
        unsigned ACTIVE:1;  // ACTIVE: UARTx Running Status bit <br>1 = UARTx is active (UxMODE register shouldn't be updated) <br>0 = UARTx is not active (UxMODE register can be updated)
        unsigned SLPEN:1;   // SLPEN: Run During Sleep Enable bit <br>1 = UARTx clock runs during Sleep <br>0 = UARTx clock is turned off during Sleep
        unsigned :8;

        unsigned int MODECLR;
        unsigned int MODESET;
        unsigned int MODEINV;


        // UxSTA
        unsigned RXDA:1;    // bit 0 RXDA: Receive Buffer Data Available bit (read-only) <br>1 = Receive buffer has data, at least one more character can be read <br>0 = Receive buffer is empty
        unsigned OERR:1;    // bit 1 OERR: Receive Buffer Overrun Error Status bit (read/clear only) <br>1 = Receive buffer has overflowed <br>0 = Receive buffer has not overflowed. Clearing a previously set OERR bit (1 → 0 transition) resets the receiver buffer and the UxRSR to the empty state.
        unsigned FERR:1;    // bit 2 FERR: Framing Error Status bit (read-only) <br>1 = Framing error has been detected for the current character (character at the top of the receive FIFO) <br>0 = Framing error has not been detected
        unsigned PERR:1;    // bit 3 PERR: Parity Error Status bit (read-only) <br>1 = Parity error has been detected for the current character (character at the top of the receive FIFO) <br>0 = Parity error has not been detected
        unsigned RIDLE:1;   // bit 4 RIDLE: Receiver Idle bit (read-only) <br>1 = Receiver is Idle <br>0 = Receiver is active
        unsigned ADDEN:1;   // bit 5 ADDEN: Address Character Detect bit (bit 8 of received data = 1) <br>1 = Address Detect mode enabled. If 9-bit mode is not selected, this does not take effect <br>0 = Address Detect mode disabled
        unsigned URXISEL:2; // bit 7-6 URXISEL<1:0>: Receive Interrupt Mode Selection bits <br>0x3 = Interrupt is set on UxRSR transfer making the receive buffer full (i.e., has 4 data characters) <br>0x2 = Interrupt is set on UxRSR transfer making the receive buffer 3/4 full (i.e., has 3 data characters) <br>0x0 or 0x1 = Interrupt is set when any character is received and transferred from the UxRSR to the receive buffer. Receive buffer has one or more characters.
        unsigned TRMT:1;    // bit 8 TRMT: Transmit Shift Register Empty bit (read-only) <br>1 = Transmit Shift Register is empty and transmit buffer is empty (the last transmission has completed) <br>0 = Transmit Shift Register is not empty, a transmission is in progress or queued
        unsigned UTXBF:1;   // bit 9 UTXBF: Transmit Buffer Full Status bit (read-only) <br>1 = Transmit buffer is full <br>0 = Transmit buffer is not full, at least one more character can be written
        unsigned UTXEN:1;   // bit 10 UTXEN: Transmit Enable bit <br>1 = Transmit enabled, UxTX pin controlled by UARTx <br>0 = Transmit disabled, any pending transmission is aborted and buffer is reset. UxTX pin controlled by port
        unsigned UTXBRK:1;  // bit 11 UTXBRK: Transmit Break bit <br>1 = Send Sync Break on next transmission – Start bit, followed by twelve ‘0’ bits, followed by Stop bit; cleared by hardware upon completion <br>0 = Sync Break transmission disabled or completed
        unsigned URXEN:1;   // bit 12 Varies by device: Unimplemented: Read as ‘0’, OR, URXEN: UARTx Receiver Enable bit <br>1 = UARTx receiver is enabled, UxRX pin is controlled by UARTx (if ON = 1) <br>0 = UARTx receiver is disabled, UxRX pin is ignored by the UARTx module
        unsigned UTXINV:1;  // bit 14 UTXINV: Transmit Polarity Inversion bit <br>If IREN = 0: <br>1 = UxTX Idle state is ‘0’ <br>0 = UxTX Idle state is ‘1’ <br>If IREN = 1: <br>1 = IrDA® encoded UxTX Idle state is ‘1’ <br>0 = IrDA® encoded UxTX Idle state is ‘0’
        unsigned UTXISEL0:1;// bit 15-14 UTXISEL<1:0>: Transmission Interrupt Mode Selection bits <br>0x3 = Reserved; do not use <br>0x2 = Interrupt when a character is transferred to the Transmit Shift register, and as a result, the transmit buffer becomes empty <br>0x1 = Interrupt when the last character is shifted out of the Transmit Shift register; all transmit operations are completed <br>0x0 = Interrupt when a character is transferred to the Transmit Shift register (this implies there is at least one character open in the transmit buffer)
        unsigned UTXISEL1:1;// bit 15-14 UTXISEL<1:0>: Transmission Interrupt Mode Selection bits <br>0x3 = Reserved; do not use <br>0x2 = Interrupt when a character is transferred to the Transmit Shift register, and as a result, the transmit buffer becomes empty <br>0x1 = Interrupt when the last character is shifted out of the Transmit Shift register; all transmit operations are completed <br>0x0 = Interrupt when a character is transferred to the Transmit Shift register (this implies there is at least one character open in the transmit buffer)

        unsigned int STACLR;
        unsigned int STASET;
        unsigned int STAINV;

        // UxTXREG
        unsigned TXREG:9;
        unsigned :23;

        unsigned int TXREGCLR;
        unsigned int TXREGSET;
        unsigned int TXREGINV;


        // UxRXREG
        unsigned RXREG:9;
        unsigned :23;

        unsigned int RXREGCLR;
        unsigned int RXREGSET;
        unsigned int RXREGINV;

        // UxBRG
        unsigned int BRG;

        unsigned int BRGCLR;
        unsigned int BRGSET;
        unsigned int BRGINV;
    } UART_SFRS;

#elif UART_SILICON == 3 // All dsPIC33C devices
    typedef struct
    {
        // UxMODE
        unsigned MOD:4;
        unsigned URXEN:1;
        unsigned UTXEN:1;
        unsigned ABAUD:1;
        unsigned BRGH:1;
        unsigned UTXBRK:1;
        unsigned BRKOVR:1;
        unsigned :1;
        unsigned RXBIMD:1;
        unsigned WAKE:1;
        unsigned USIDL:1;
        unsigned :1;
        unsigned UARTEN:1;

        // UxMODEH
        unsigned FLO:2;
        unsigned UTXINV:1;
        unsigned C0EN:1;
        unsigned STSEL:2;
        unsigned URXINV:1;
        unsigned RUNOVF:1;
        unsigned HALFDPLX:1;
        unsigned BCLKSEL:2;
        unsigned BCLKMOD:1;
        unsigned :2;
        unsigned ACTIVE:1;
        unsigned SLPEN:1;

        // UxSTA
        unsigned TXCIF:1;
        unsigned OERR:1;
        unsigned RXBKIF:1;
        unsigned FERR:1;
        unsigned CERIF:1;
        unsigned ABDOVF:1;
        unsigned PERR:1;
        unsigned TRMT:1;
        unsigned TXCIE:1;
        unsigned OERIE:1;
        unsigned RXBKIE:1;
        unsigned FERIE:1;
        unsigned CERIE:1;
        unsigned ABDOVE:1;
        unsigned PERIE:1;
        unsigned TXMTIE:1;

        // UxSTAH
        unsigned URXBF:1;
        unsigned URXBE:1;
        unsigned XON:1;
        unsigned RIDLE:1;
        unsigned UTXBF:1;
        unsigned UTXBE:1;
        unsigned STPMD:1;
        unsigned TXWRE:1;
        unsigned URXISEL:3;
        unsigned :1;
        unsigned UTXISEL:3;

        // UxBRG
        unsigned int BRG;

        // UxRXREG
        unsigned RXDATA:8;
        unsigned :8;

        // UxTXREG
        unsigned TXDATA:9;
        unsigned :7;
    } UART_SFRS;
#endif




// Bitfield structure for signaling communication FIFO state changes, especially
// for code operating in different asynchronous contexts.
typedef union
{
    struct
    {
        unsigned int tx:1;          // One or more bytes moved from the TX software FIFO to the TX hardware (ex: U2TX ISR wrote to U2TXREG)
        unsigned int rx:1;          // One or more bytes moved from RX hardware into the software RX FIFO (ex: _U2RXInterrupt() copied data out of U2RXREG)
        unsigned int softTx:1;      // Zero or more bytes moved into the TX software FIFO using a call to EZBL_FIFOWrite() or a wrapper function. Ex: UART2_TX_FIFO_OnWrite() callback executed.
        unsigned int softRx:1;      // Zero or more bytes peeked or read from the RX software FIFO as a result of a call to EZBL_FIFORead() or EZBL_FIFOPeek(). Ex: UART2_RX_FIFO_OnRead() callback executed.
        unsigned int rxOverflow:1;  // Hardware or software RX FIFO overflow/data lost
        unsigned int framingError:1;// UART RX Framing Error detected
        unsigned int parityError:1; // RX Parity Error detected
        unsigned int bootkeyRx:1;   // Bootloader synchronization/wakeup key detected (normally "MCUPHCME")
        unsigned int :7;
        unsigned int other:1;       // Status bit indicating other (ill-defined) hardware events or software calls have taken place that affect the state of the software TX/RX FIFOs or underlying hardware. Example captured status updates include: FIFO reset/initialized, framing error observed and software arming of auto-baud.
    };
    unsigned int any;           // Integral union form of the tx, rx, softTx, softRx, and other flags for testing/clearing multiple flags at once.
} EZBL_COM_ACTIVITY;


// Circular FIFO buffer structure for use with EZBL_FIFOWrite()/EZBL_FIFORead() and related APIs
typedef struct EZBL_FIFO EZBL_FIFO;
struct EZBL_FIFO
{
    volatile unsigned int dataCount;    // Number of bytes that exist in the FIFO. i.e. headPtr - tailPtr, adjusted for wraparound and made atomically readable. You must use EZBL_ATOMIC_ADD() and EZBL_ATOMIC_SUB() if this value is to be modified.
    unsigned char *headPtr;             // Pointer to fifoRAM area for writing
    unsigned char *tailPtr;             // Pointer to fifoRAM area for reading
    unsigned int fifoSize;              // Number of bytes allocated to the fifoRAM
    unsigned char *fifoRAM;             // Main FIFO buffer RAM pointer (or const PSV pointer if only doing reading)
    unsigned int (*onWriteCallback)(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);   // Pointer to a function to be called anytime the FIFO is written (or attempted to be written)
    unsigned int (*onReadCallback)(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *srcFIFO);      // Pointer to a function to be called anytime the FIFO is read (or attempted to be read)
    unsigned int (*flushFunction)(EZBL_FIFO *fifo, unsigned long *startTime, unsigned long *timeout); // Pointer to a function to be called anytime the EZBL_FIFOFlush() function is called
    unsigned int irqNum;                // IRQ Number to associate with this FIFO. This number is used to decode the correct IFSx/IECx/IPCx register/bit(s) to access when calling EZBL_FIFOIntEnable()/EZBL_FIFOIntDisable()/EZBL_FIFOIntClear()/EZBL_FIFOIntRaise()/EZBL_FIFOIntEnableSet()/EZBL_FIFOIntFlagSet()/EZBL_FIFOGetIntEn()/EZBL_FIFOGetIntPri() functions. IRQ numbers are zero-based where 0 indicates the first ordinary peripheral hardware interrupt (i.e. not a trap)
    volatile EZBL_COM_ACTIVITY activity;   // Status bit flags capturing various software and hardware state change events like RX interrupt byte, software RX FIFO read, TX interrupt byte, software TX FIFO write, RX bootloader wake key detection, HW/SW FIFO RX overflow and other activity
};


int EZBL_CalcUxBRGH(unsigned long peripheralClock, long baud);


extern unsigned long      NOW_Fcy __attribute__((persistent));

extern volatile UART_SFRS U1MODE  __attribute__((near));
extern volatile UART_SFRS U2MODE  __attribute__((near));
extern volatile UART_SFRS U3MODE  __attribute__((near, weak));
extern volatile UART_SFRS U4MODE  __attribute__((near, weak));
extern volatile UART_SFRS U5MODE  __attribute__((near, weak));
extern volatile UART_SFRS U6MODE  __attribute__((near, weak));



/**
 * Sets the baud rate associated with a UART TX or RX EZBL_FIFO and/or enables
 * hardware auto-baud. Existing FIFO contents are unchanged.
 *
 * @param *TxRxFifo Pointer to an EZBL_FIFO structure associated with a UART TX
 *                  or UART RX FIFO. Usually, this will be a &UARTx_TxFifo or
 *                  &UARTx_RxFifo reference, or a saved pointer like
 *                  EZBL_STDOUT or EZBL_COM_RX. Typically these are instanced by
 *                  a prior call to the UART_Reset() macro.
 *
 *                  The TxRxFifo->irqNum parameter must be set before calling
 *                  this function so that the correct UxBRG register (and
 *                  optionally UxMODEbits.ABAUD bit) are referenced. This
 *                  parameter is automatically set if the FIFO was previously
 *                  initialized using UART_Reset(), UART_ResetBRG(), or
 *                  UART_ResetCalc().
 *
 *                  This pointer can be null, resulting in no operation.
 *                  However, this pointer must not point to a non-UART FIFO
 *                  type (doing so will result in data corruption of an
 *                  undefined UART's SFRs).
 *
 * @param newBaud   Desired baud rate to set the hardware UART peripheral to.
 *                  Set as a <= 0 value to enable hardware auto-baud.
 *
 *                  A negative newBaud value is defined to mean: enable
 *                  auto-baud, but still compute and assign UxBRG to the
 *                  positive equivalent newBaud value. This typically allows TX
 *                  operations at a fixed, known baud rate until a 0x55 ('U') RX
 *                  character is received to redefine the baud rate.
 *
 *                  The NOW_Fcy global variable is referenced to run-time
 *                  compute the necessary UxBRG value based on the current
 *                  system clock setting. If newBaud == 0, this reference is
 *                  still generated, but the NOW_Fcy value is unused.
 *
 * @return Prior value contained in UxBRG of the UART that was updated. NOTE:
 *         this is returned as a signed integer, so if the hardware implements
 *         UxBRG with 16 or more bits on a 16-bit platform, UxBRG values > 32767
 *         will be returned as a negative value. Any higher order bits would not
 *         be returned.
 *
 *         The TxRxFifo->activity.other status bit is set to '1'.
 *
 *         If TxRxFifo is a null pointer or TxRxFifo->irqNum is an illegal IRQ
 *         number (i.e. > 255), -1 is returned.
 */
int EZBL_FIFOSetBaud(EZBL_FIFO *TxRxFifo, long newBaud)
{
    volatile UART_SFRS *UARTRegs = &U1MODE;
    int oldBRG = -1;
    int newBRG;

    if(!TxRxFifo)
        return oldBRG;
    if(TxRxFifo->irqNum>>8)
        return oldBRG;


#if defined(EZBL_LIB16CH) || defined(__dsPIC33CH__) || defined(__generic_16dsp_ch__) // All dsPIC33C class devices
    //generic-16dsp-ch has:
    //    IEC0<U1RXIE>, #11	// 1 bit, IRQ 11
    //    IEC1<U2RXIE>, #11	// 1 bit, IRQ 27
    if(TxRxFifo->irqNum >= 27u)
        UARTRegs = &U2MODE;
#elif defined(EZBL_LIB16EP) || defined(__dsPIC33E__) || defined(__PIC24EP__) || defined(__generic_16bit_ep__) // All dsPIC33E/PIC24E class devices
    //generic-16bit-ep has:
    //    IEC0<U1RXIE>, #11	// 1 bit, IRQ 11
    //    IEC1<U2RXIE>, #14	// 1 bit, IRQ 30
    //    IEC5<U3RXIE>, #2	// 1 bit, IRQ 82
    //    IEC5<U4RXIE>, #8	// 1 bit, IRQ 88
    if(TxRxFifo->irqNum >= 30u)
        UARTRegs = &U2MODE;
    if(TxRxFifo->irqNum >= 82u)
        UARTRegs = &U3MODE;
    if(TxRxFifo->irqNum >= 88u)
        UARTRegs = &U4MODE;
#elif defined(EZBL_LIB16DA) || (defined(__PIC24FJ__) && defined(DSRPAG)) || defined(__generic_16bit_da__) // All PIC24F class devices with EDS (DSRPAG/DSWPAG, not PSVPAG)
    //generic-16bit-da has:
    //    IEC0<U1RXIE>, #11	// 1 bit, IRQ 11
    //    IEC1<U2RXIE>, #14	// 1 bit, IRQ 30
    //    IEC5<U3RXIE>, #2	// 1 bit, IRQ 82
    //    IEC5<U4RXIE>, #8	// 1 bit, IRQ 88
    //    IEC6<U5RXIE>, #15	// 1 bit, IRQ 111
    //    IEC7<U6RXIE>, #2	// 1 bit, IRQ 114
    if(TxRxFifo->irqNum >= 30u)
        UARTRegs = &U2MODE;
    if(TxRxFifo->irqNum >= 82u)
        UARTRegs = &U3MODE;
    if(TxRxFifo->irqNum >= 88u)
        UARTRegs = &U4MODE;
    if(TxRxFifo->irqNum >= 111u)
        UARTRegs = &U5MODE;
    if(TxRxFifo->irqNum >= 114u)
        UARTRegs = &U6MODE;
#elif defined(EZBL_LIB16) || (defined(__XC16__) && defined(PSVPAG)) || defined(__generic_16bit__) // All dsPIC30F/dsPIC33F/PIC24H class devices + PIC24F class devices with PSVPAG (not DSRPAG/DSWPAG)
    //generic-16bit has:
    //    IEC0<U1RXIE>, #11	// 1 bit, IRQ 11
    //    IEC0<U1RXIE>, #9	// 1 bit, IRQ 9
    //    IEC1<U2RXIE>, #14	// 1 bit, IRQ 30
    //    IEC1<U2RXIE>, #8	// 1 bit, IRQ 24
    //    IEC5<U3RXIE>, #2	// 1 bit, IRQ 82
    //    IEC5<U4RXIE>, #8	// 1 bit, IRQ 88
    if(TxRxFifo->irqNum >= 24u)
        UARTRegs = &U2MODE;
    if(TxRxFifo->irqNum >= 82u)
        UARTRegs = &U3MODE;
    if(TxRxFifo->irqNum >= 88u)
        UARTRegs = &U4MODE;
#else
    #error "Unknown mapping between UxRX IRQ number and UxBRG register"
#endif


    newBRG = EZBL_CalcUxBRGH(NOW_Fcy, newBaud);
    oldBRG = (int)(UARTRegs->BRG);
    if(newBRG > 0)
    {
        UARTRegs->BRG = newBRG;
        if(UARTRegs->ABAUD)
        {
            UARTRegs->ABAUD = 0;
            if(UARTRegs->UARTEN)
            {
                UARTRegs->UARTEN = 0;
                UARTRegs->ABAUD  = 0;
                UARTRegs->UARTEN = 1;
                UARTRegs->UTXEN  = 1;
            }
        }
    }
    else
    {
        UARTRegs->BRG = -newBRG;
        UARTRegs->ABAUD = 1;
    }
    TxRxFifo->activity.other = 1;
    TxRxFifo->activity.framingError = 0;    // Delete record of a framing error when changing the baud rate

    return oldBRG;
}
