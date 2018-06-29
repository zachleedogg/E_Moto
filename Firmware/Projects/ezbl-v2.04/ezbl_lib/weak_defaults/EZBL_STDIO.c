/*******************************************************************************
  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM API Source File

  Summary:
    Instances the EZBL_STDOUT and EZBL_STDIN pointers plus the 
    _write()/_read() or _mon_putc()/_mon_getc() functions for XC16/XC32
    redirection.

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

#include <stdio.h>


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

unsigned int EZBL_FIFOWrite(EZBL_FIFO *destFIFO, const void *source, unsigned int dataLen);
unsigned int EZBL_FIFORead(void *dest, EZBL_FIFO *srcFIFO, unsigned int dataLen);
static inline unsigned int __attribute__((always_inline)) EZBL_FIFOWrite8(EZBL_FIFO *dest, unsigned char writeData)
{
    return EZBL_FIFOWrite(dest, &writeData, 1);
}
static inline unsigned char __attribute__((always_inline, optimize(1))) EZBL_FIFORead8(EZBL_FIFO *source)
{
    unsigned char readData;
    EZBL_FIFORead(&readData, source, 1);
    return readData;
}


EZBL_FIFO *EZBL_STDOUT;
EZBL_FIFO *EZBL_STDIN;



/**
 * Generic write()/read() function for printf()/scanf() and other C stdlib file
 * I/O operations. This function overrides the internal write()/read() function
 * of the library so whenever a higher layer printf() or similar call is made,
 * these functions get called automatically. This allows stdout/stdin to map to
 * these software FIFO functions instead of directly to a UART peripheral.
 *
 * For further information and info on how these functions interact, with C
 * stdlib file read/write IO, see the XC16 C library user's guide (DS50002071),
 * ex:
 * C:\Program Files (x86)\Microchip\xc16\v1.30\docs\MPLAB_XC16_C_Compiler_Users_Guide.pdf
 *
 * @param handle File handle specifying where the write is intended to go (ex:
 *               stdout, stderr, file, etc.) or read comes from (ex: stdin).
 *               handle is not checked here, so this function will wrap ALL
 *               writes to stdout, stderr, and all other file handles. This
 *               function should be modified or replaced if needing other handle
 *               destinations besides this UART module.
 *
 * @param *buffer Pointer to source data being written to the handle, or for
 *                read() pointer to the destination RAM buffer to write the read
 *                data to.
 *
 * @param len Number of bytes that need to be copied from *buffer to the output
 *            handle, or for read(), number of bytes read from the handle.
 *
 * @return int Number of bytes copied from *buffer to the UART TX FIFO (or from
 *         the UART RX FIFO). This function blocks if there is insufficient
 *         space directly in the software TX FIFO until space is freed up.
 *         However, it returns immediately after all bytes have been buffered
 *         allowing efficient asynchronous and pipelined transmission.
 */
#if defined(__XC16__)
int __attribute__((weak)) write2(int handle, void *buffer, unsigned int len);

extern int EZBL_write(int handle, void *buffer, unsigned int len) __attribute__((alias("write")));  // Make EZBL_write() symbol, which is equal to the write() function address
int __attribute__((section(".libc.write"))) write(int handle, void *buffer, unsigned int len)
{
    // Tee data to a write2() function, if the project contains such a function
    if(write2)
        write2(handle, buffer, len);

    // Now write to the EZBL_STDOUT EZBL_FIFO implementation
    return EZBL_FIFOWrite(EZBL_STDOUT, buffer, len);
}


extern int EZBL_read(int handle, void *buffer, unsigned int len) __attribute__((alias("read")));  // Make EZBL_read() symbol, which is equal to the read() function address
int __attribute__((section(".libc.read"))) read(int handle, void *buffer, unsigned int len)
{
    return EZBL_FIFORead(buffer, EZBL_STDIN, len);
}


#elif defined(__XC32__)
void __attribute__((weak)) _mon_putc2(char c);

extern void EZBL_mon_putc(char c)       __attribute__((alias("_mon_putc")));    // Make EZBL_mon_putc() symbol, which is equal to the _mon_putc() function address
void _mon_putc(char c)
{
    // Tee data to a _mon_putc2() function, if the project contains such a function
    if(_mon_putc2)
        _mon_putc2(c);

    EZBL_FIFOWrite8(EZBL_STDOUT, c);
}

extern int EZBL_mon_getc(int canblock)  __attribute__((alias("_mon_getc")));    // Make EZBL_mon_getc() symbol, which is equal to the _mon_getc() function address
int _mon_getc(int canblock)
{
    if(canblock == 0)
    {
        if(EZBL_STDIN->dataCount == 0u)
            return -1;
    }
    return (int)(unsigned int)EZBL_FIFORead8(EZBL_STDIN);
}
#endif
