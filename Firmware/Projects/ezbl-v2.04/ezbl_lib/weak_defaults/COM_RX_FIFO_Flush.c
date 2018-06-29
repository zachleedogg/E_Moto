/*
 * File:   COM_RX_FIFO_Flush.c
 *
 * Implements a weak default COM_RX_FIFO_Flush() function that removes all
 * current data from a specified FIFO and clears error activity flags, such as
 * overflow and framing errors.
 *
 * This same function is aliased to:
 *      UART_RX_FIFO_Flush()
 *      UART1_RX_FIFO_Flush()
 *      UART2_RX_FIFO_Flush()
 *      UART3_RX_FIFO_Flush()
 *      UART4_RX_FIFO_Flush()
 *      I2C_RX_FIFO_Flush()
 *      I2C1_RX_FIFO_Flush()
 *      I2C2_RX_FIFO_Flush()
 */

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

unsigned int EZBL_FIFORead(void *dest, EZBL_FIFO *srcFIFO, unsigned int dataLen);


/**
 * Throws all data in this RX FIFO away and clears any error conditions.
 *
 * @param *flushFIFO Pointer to the this RX FIFO that a flush request has been
 *                   made against.
 *
 *                   This parameter is included so the generic EZBL_FIFOFlush()
 *                   function can call a generic COM_RX_FIFO_Flush()
 *                   implementation shared by several run-time FIFOs via the
 *                   EZBL_FIFO.flushFunction() function pointer and still permit
 *                   special behavior for one or more FIFOs.
 *
 * @return RX FIFO's dataCount is cleared to zero and a successful return code
 *         of 1 is always returned.
 */
unsigned int __attribute__((weak)) COM_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout)
{
    EZBL_FIFORead(0, flushFIFO, flushFIFO->dataCount);  // Throw all existing FIFO data in the bit bucket
    flushFIFO->activity.rxOverflow   = 0;
    flushFIFO->activity.framingError = 0;
    flushFIFO->activity.parityError  = 0;
    return 1;
}


// Extra names for COM_RX_FIFO_Flush(). This is a public, exported symbol that can be linked to.
extern unsigned int __attribute__((weak, alias("COM_RX_FIFO_Flush"))) UART_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);
extern unsigned int __attribute__((weak, alias("COM_RX_FIFO_Flush"))) UART1_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);
extern unsigned int __attribute__((weak, alias("COM_RX_FIFO_Flush"))) UART2_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);
extern unsigned int __attribute__((weak, alias("COM_RX_FIFO_Flush"))) UART3_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);
extern unsigned int __attribute__((weak, alias("COM_RX_FIFO_Flush"))) UART4_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);

extern unsigned int __attribute__((weak, alias("COM_RX_FIFO_Flush"))) I2C_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);
extern unsigned int __attribute__((weak, alias("COM_RX_FIFO_Flush"))) I2C1_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);
extern unsigned int __attribute__((weak, alias("COM_RX_FIFO_Flush"))) I2C2_RX_FIFO_Flush(EZBL_FIFO *flushFIFO, unsigned long *startTime, unsigned long *timeout);
