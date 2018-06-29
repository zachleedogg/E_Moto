/*******************************************************************************
  OBSOLETE - EZBL v1.13+ - DO NOT USE IN NEW CODE

  Multipurpose FIFO Source Library

  File Name:
    fifo.c.h (not a typo - really lives a confused source and header existence)

  Summary:
    Implements a multipurpose, multinstancable circular buffering FIFO

  Description:
    A multi-use FIFO (circular buffer) for transferring data into and out of
    peripherals, "crossing clock domains" (ex: communicating between main()
    context code and Interrupt context code), and other buffering needs.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Copyright (C) 2016 Microchip Technology Inc.

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

/*
 * ATTENTION: Do not #include this file like an ordinary .h file! It contains
 *            code!!! fifo.h is the regular header file.
 *            This module is meant to be instantiated by #including this .c.h
 *            file into another .c file exactly where you want the FIFO. It is
 *            modular so it can be instanced multiple times for multiple FIFOs,
 *            each with different names, callback functions, and buffer
 *            sizes.
 */

// Input Parameters:
// - FIFO_NAME macro
//   Default when not defined: FIFO_
//   Description:
//      Defines the prefix for all symbols (functions/variables) of the FIFO.
// - FIFO_SIZE macro
//   Default when not defined: 16
//   Description:
//      Defines the size, in bytes, of the FIFO buffer RAM. Exactly this many
//      bytes of unread data can be written to the FIFO before write calls will
//      block. Similarly, at most this many bytes of data can be read from the
//      FIFO in a single read call before blocking for more data is necessary.
//
// - FIFO_EXTRA_VARS macro
//   Default when not defined: (none)
//   Description:
//      Adds extra variables to the (FIFO_NAME)vars structure specified by
//      FIFO_EXTRA_VARS. For example if FIFO_EXTRA_VARS is defined as:
//          #define FIFO_EXTRA_VARS     unsigned int state[2];
//      Then you can then access (FIFO_NAME)vars.state[0] and
//      (FIFO_NAME)vars.state[1] in your application code. The variables have
//      no internal function, use, or reset value, but can be used to logically
//      group data elements that have a 1:1 meaning with the instanced FIFO. Ex:
//      if extending the FIFO to include flow control and need flow control
//      state variables, it is helpful for debugging and organization to keep
//      them attached to the FIFO.
//
// - FIFO_FEATURES macro
//   Default when not defined: FIFO_FEATURE_READABLE | FIFO_FEATURE_WRITABLE | FIFO_FEATURE_PEEKABLE
//   Description:
//      A bitmask of FIFO_FEATURE_* flags defined in fifo.h which control what
//      features and behaviors the FIFO should have. Callback functions and
//      other options can be specified using these flags.
// - unsigned int OnData(void *dataPointer, unsigned int dataLength) macro
//      remapping to a real function
//   Default when not defined:
//      Associated threshold compare code and variables are disabled and
//      deleted. Attempts to call (FIFO_NAME)SetOnDataThreshold() will cause a
//      compiler error since the needed code will no longer exist.
//   Description:
//      Callback function to be called when a pre-specified number of bytes of
//      data are present in the FIFO. dataPointer points to a contiguous block
//      of memory containing the pre-specified number of bytes. Because the FIFO
//      is internally implemented as a circular buffer and must "wrap around",
//      the FIFO will sometimes have to copy the FIFO data into a temporary RAM
//      buffer to allow linear, contiguous addressing. As a result, the memory
//      pointed to by dataPointer must be treated as temporary and assumed to
//      dissapear once you return from this callback. dataLength specifies the
//      data length passed earlier to FIFO_SetOnDataThreshold() and also the
//      number of bytes that can be contiguously read from *dataPointer.
//      On return, specify the number of bytes you want to remove from the FIFO.
//      If you do not remove any data, or call FIFO_SetOnDataThreshold() to
//      raise the compare threshold, this callback will be immediately called
//      again.
//
//
// Dependancies: memcpy() C stdlib function (string.h). Can be overridden
//               externally by #defining FIFO_memcpy(dst,src,len) as something
//               else.
//
// Usage: Define FIFO_NAME (must be unique if using more than one in a given
//        application), define FIFO_SIZE, then #include "fifo.h" into your
//        instancing .c source file. See uart_fifo.c for an example using 2
//        instances of this module (one TX and one RX) with different FIFO
//        buffer sizes (256 byte RX, 128 byte TX).

#include "fifo.h"


//  OBSOLETE - EZBL v1.13+ - DO NOT USE IN NEW CODE

// Default parameters if not specified by the instancee
#ifndef FIFO_NAME
#define FIFO_NAME   FIFO_
#endif
#ifndef FIFO_SIZE
#define FIFO_SIZE   16
#endif
#ifndef FIFO_ON_DATA_THRESHOLD_RESET_VALUE
#define FIFO_ON_DATA_THRESHOLD_RESET_VALUE  0u      // 0 is disabled
#endif
#ifndef FIFO_ON_FREESPACE_THRESHOLD_RESET_VALUE
#define FIFO_ON_FREESPACE_THRESHOLD_RESET_VALUE  0u // 0 is disabled
#endif

#ifndef FIFO_FEATURES
#define FIFO_FEATURES  (FIFO_FEATURE_READABLE | FIFO_FEATURE_WRITABLE | FIFO_FEATURE_PEEKABLE)
#endif
#ifndef FIFO_EXTRA_VARS
#define FIFO_EXTRA_VARS
#endif


// Check if the instantiating module is overridding memcpy() with their own
// macro. If they aren't then ensure we have the string.h header included so we
// can use the C stdlib memcpy() function.
#if !defined(FIFO_memcpy)
#define FIFO_memcpy(dest, source, length)   memcpy(dest, source, length)
#include <string.h>     // For memcpy() prototype used in fifo.h
#endif

// Macro for putting the CPU into idle mode for ReadSuspendUntil() and
// WriteSuspendUntil() functionality. You may override this Suspend() function 
// call in the instantiating module. This macro resolves to same function of the
// normal Idle() macro provided in device header files, but since this is a
// module which shouldn't depend on any external files for library compilation
// purposes, the actual code for Idle() is duplicated here as a renamed macro.
// The name Idle() isn't reused since undefining it later would destroy the
// device header file's implementation.
#if !defined(FIFO_Suspend)
#define FIFO_Suspend()   {__asm__ volatile ("pwrsav #1" : : : "memory");}
#endif


// FIFO function prototypes
// These are all only visible to the .c file that is instantiating this FIFO,
// so we've tagged them as near so that the compiler can generate single
// instruction word RCALL instructions instead of the two word CALL instruction.
// To externally access any of these functions, simply expose the function
// prototypes in a header file.
void CAT2(FIFO_NAME, Reset)(void);
unsigned int CAT2(FIFO_NAME, WritableLength)(void);
unsigned int CAT2(FIFO_NAME, ReadableLength)(void);
void CAT2(FIFO_NAME, ReadSuspendUntil)(unsigned int bytesAvailable);
void CAT2(FIFO_NAME, WriteSuspendUntil)(unsigned int bytesFree);
unsigned int CAT2(FIFO_NAME, Read)(void *destPtr, unsigned int readLength);
unsigned char CAT2(FIFO_NAME, Read8)(void);
unsigned short CAT2(FIFO_NAME, Read16)(void);
unsigned int CAT2(FIFO_NAME, ReadUINT)(void);
unsigned long CAT2(FIFO_NAME, Read24)(void);
unsigned long CAT2(FIFO_NAME, Read32)(void);
unsigned long long CAT2(FIFO_NAME, Read64)(void);
unsigned int CAT2(FIFO_NAME, Peek)(void *destPtr, unsigned int readLength);
unsigned char CAT2(FIFO_NAME, Peek8)(void);
unsigned short CAT2(FIFO_NAME, Peek16)(void);
unsigned int CAT2(FIFO_NAME, PeekUINT)(void);
unsigned long CAT2(FIFO_NAME, Peek24)(void);
unsigned long CAT2(FIFO_NAME, Peek32)(void);
unsigned long long CAT2(FIFO_NAME, Peek64)(void);
void CAT2(FIFO_NAME, Fill)(unsigned char fillChar, unsigned int writeLength);
unsigned int CAT2(FIFO_NAME, Write)(void *sourcePtr, unsigned int writeLength);
void CAT2(FIFO_NAME, Write8)(unsigned char writeData);
void CAT2(FIFO_NAME, Write16)(unsigned short writeData);
void CAT2(FIFO_NAME, WriteUINT)(unsigned int writeData);
void CAT2(FIFO_NAME, Write24)(unsigned long writeData);
void CAT2(FIFO_NAME, Write32)(unsigned long writeData);
void CAT2(FIFO_NAME, Write64)(unsigned long long writeData);
#if((FIFO_FEATURES) & (FIFO_FEATURE_ON_DATA | FIFO_FEATURE_ON_DATA_LINEAR))
void CAT2(FIFO_NAME, SetOnDataThreshold)(unsigned int dataThreshold);
#endif
#if((FIFO_FEATURES) & (FIFO_FEATURE_ON_FREESPACE | FIFO_FEATURE_ON_FREESPACE_LINEAR))
void CAT2(FIFO_NAME, SetOnFreespaceThreshold)(unsigned int freespaceThreshold);
#endif



#if defined(__PIC24__) || defined(__dsPIC__) || defined(__XC16__)
#define ATOMIC_ADD(left_var, right_val) __asm__ volatile("  mov #%0, W1  \n" \
                                                         "  add %1, [W1], [W1]  " : : "i"(&left_var), "r"(right_val) : "memory", "w1", "cc")
#define ATOMIC_SUB(left_var, right_val) __asm__ volatile("  mov #%0, W1  \n" \
                                                         "  subr %1, [W1], [W1] " : : "i"(&left_var), "r"(right_val) : "memory", "w1", "cc")
#define ATOMIC_MOV(dest, source)        __asm__ volatile("  mov #%0, W1 \n"  \
                                                         "  mov #%1, W2 \n"  \
                                                         "  mov [W2], [W1]"  : : "i"(&dest), "i"(&source) : "memory", "w1", "w2")
#elif defined(__PIC32__) || defined(__XC32__)
#define ATOMIC_ADD(left_var, right_val) do                                      \
                                        {                                       \
                                                __builtin_disable_interrupts(); \
                                                left_var += right_val;          \
                                                __builtin_enable_interrupts();  \
                                        } while(0)

#define ATOMIC_SUB(left_var, right_val) do                                      \
                                        {                                       \
                                            __builtin_disable_interrupts();     \
                                            left_var -= right_val;              \
                                            __builtin_enable_interrupts();      \
                                        } while(0)
#define ATOMIC_MOV(dest, source)        do                                      \
                                        {                                       \
                                            __builtin_disable_interrupts();     \
                                            dest = source;                      \
                                            __builtin_enable_interrupts();      \
                                        } while(0)
#endif

// Structure to allocate memory for this FIFO
// NOTE: All of these elements (except the fifoRAM) should be considered 
// volatile fields which can change in an ISR and main() context simultaneously. 
// Therefore, you MUST only do read-modify-write operations to these variable 
// using the ATOMIC_* macros defined above. Reading can be fine, but you must 
// accept that the value may be stale (i.e. don't write back to the var using 
// it in a calculation).

struct
{
    volatile unsigned int dataCount; // Use ATOMIC_ADD() and ATOMIC_SUBTRACT() to change this value
    unsigned char *headPtr;
    unsigned char *tailPtr;
#if((FIFO_FEATURES) & (FIFO_FEATURE_ON_DATA | FIFO_FEATURE_ON_DATA_LINEAR))
    unsigned int dataThreshold; // Value to compare against dataCount for issuing OnData() callback
#endif
#if((FIFO_FEATURES) & (FIFO_FEATURE_ON_FREESPACE | FIFO_FEATURE_ON_FREESPACE_LINEAR))
    unsigned int freespaceThreshold; // Value to compare against (sizeof(fifoRAM) - dataCount) for issuing OnFreespace() callback
#endif
    FIFO_EXTRA_VARS // Include any instancing application defined variables that are logically tied to this FIFO
    unsigned char fifoRAM[FIFO_SIZE];   // As a quasi variable length field, this should be at the bottom of the structure
} CAT2(FIFO_NAME, vars);


#if((FIFO_FEATURES) & FIFO_FEATURE_ON_DATA)

void CAT2(FIFO_NAME, _DispatchOnData)(void)
{
    if(CAT2(FIFO_NAME, vars).dataThreshold)
    {
        while(CAT2(FIFO_NAME, vars).dataCount >= CAT2(FIFO_NAME, vars).dataThreshold)
        {
            CAT2(FIFO_NAME, OnData)(CAT2(FIFO_NAME, vars).dataThreshold);
        }
    }
}
#endif  // FIFO_FEATURE_ON_DATA

#if ((FIFO_FEATURES) & FIFO_FEATURE_ON_DATA_LINEAR)

void CAT2(FIFO_NAME, _DispatchOnData)(void)
{
    while(CAT2(FIFO_NAME, vars).dataCount >= CAT2(FIFO_NAME, vars).dataThreshold)
    {
        unsigned int dataUsed;

        // Check if the dataThreshold is disabled (zero), or has become disabled since the the last loop iteration
        if(CAT2(FIFO_NAME, vars).dataThreshold == 0u)
        {
            return;
        }

        // See if we can pass a pointer directly to the FIFO RAM
        if(CAT2(FIFO_NAME, vars).fifoRAM + sizeof(CAT2(FIFO_NAME, vars).fifoRAM) - CAT2(FIFO_NAME, vars).tailPtr >= CAT2(FIFO_NAME, vars).dataThreshold)
        {
            // Yes, enough space, call the callback and update the Tail Pointer/FIFO count upon return
            dataUsed = CAT2(FIFO_NAME, OnData)(CAT2(FIFO_NAME, vars).tailPtr, CAT2(FIFO_NAME, vars).dataThreshold);
            CAT2(FIFO_NAME, vars).tailPtr += dataUsed;

            // Check for wrap around in case if the the dataThreshold exactly matched the number of bytes available until wrap around.
            if(CAT2(FIFO_NAME, vars).tailPtr >= CAT2(FIFO_NAME, vars).fifoRAM + sizeof(CAT2(FIFO_NAME, vars).fifoRAM))
            {
                // Correct wrap around
                CAT2(FIFO_NAME, vars).tailPtr -= sizeof(CAT2(FIFO_NAME, vars).fifoRAM);
            }
            ATOMIC_SUB(CAT2(FIFO_NAME, vars).dataCount, dataUsed);
        }
        else
        {
            // No, FIFO wraps. Create temporary buffer on the stack to allow the client to read data as if a linear data buffer existed. The returned data count will be thrown away from the FIFO.
            __asm__("   mov    %1, W1          \n" // Get a copy of the dataThreshold
                    "   add    W1, #1, W2      \n" // Add one to dataThreshold so that if it's odd, it'll become even
                    "   bclr   W2, #0          \n" // Clear the LSbit to guarantee it is even
                    "   mov    W15, W0         \n" // Move stack pointer to first function call parameter
                    "   add    W2, W15, W15    \n" // Reserve enough bytes on the stack for client read data
                    "   push   W0              \n" // Save the pointer, we are going to need it again
                    "   call   _" STRINGIFY(CAT2(FIFO_NAME, Peek)) "\n" // Copy all of the requested data out of the FIFO and into the linear stack space
                    "   mov    W0, W1          \n" // Retrieve the number of bytes peeked (should be same as we passed in)
                    "   mov    [W15-2], W0     \n" // Retrieve the original data pointer to the stack area
                    "   call   _" STRINGIFY(CAT2(FIFO_NAME, OnData)) "\n" // Call the OnData(dataPointer, dataLength) callback function
                    "   mov    W0, %0          \n" // Move read data count to dataThreshold so we can properly update the FIFO dataCount and tailPtr using C code
                    "   mov    [W15-2], W15    \n" // Restore the original stack pointer
                    : "=g"(dataUsed) : "g"(CAT2(FIFO_NAME, vars).dataThreshold) : "cc", "w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7", "memory");

            // Update the Tail Pointer/FIFO count upon return
            CAT2(FIFO_NAME, vars).tailPtr += dataUsed;

            // Check for wrap around, which is likely
            if(CAT2(FIFO_NAME, vars).tailPtr >= CAT2(FIFO_NAME, vars).fifoRAM + sizeof(CAT2(FIFO_NAME, vars).fifoRAM))
            {
                // Correct wrap around
                CAT2(FIFO_NAME, vars).tailPtr -= sizeof(CAT2(FIFO_NAME, vars).fifoRAM);
            }

            ATOMIC_SUB(CAT2(FIFO_NAME, vars).dataCount, dataUsed);
        }
    }

}
#endif  // FIFO_FEATURE_ON_DATA_LINEAR

#if((FIFO_FEATURES) & (FIFO_FEATURE_ON_DATA | FIFO_FEATURE_ON_DATA_LINEAR))

void CAT2(FIFO_NAME, SetOnDataThreshold)(unsigned int dataThreshold)
{
    CAT2(FIFO_NAME, vars).dataThreshold = dataThreshold;
}
#endif

// Suspends the current thread until the specified number of bytes are available
// to be read. Without an RTOS and real threading, this equates to a simple
// power saving function.
void CAT2(FIFO_NAME, ReadSuspendUntil)(unsigned int bytesAvailable)
{
    while(CAT2(FIFO_NAME, vars).dataCount < bytesAvailable)
    {
        // Call the user OnReadSuspend() call back function to signal that a read is waiting for a specified amount of data
#if((FIFO_FEATURES) & FIFO_FEATURE_ON_READ_SUSPEND)
        CAT2(FIFO_NAME, OnReadSuspend)(bytesAvailable);
#endif
        FIFO_Suspend(); // Issue Idle() call
    }
}

// Suspends the current thread until the specified number of bytes are free in
// the write buffer. Without an RTOS and real threading, this equates to a simple
// power saving function.
void CAT2(FIFO_NAME, WriteSuspendUntil)(unsigned int bytesFree)
{
    while(sizeof(CAT2(FIFO_NAME, vars).fifoRAM) - CAT2(FIFO_NAME, vars).dataCount < bytesFree)
    {
        // Call the user OnWriteSuspend() call back function to signal that a write is suspended because there isn't the requested number of bytes free in the FIFO
#if((FIFO_FEATURES) & FIFO_FEATURE_ON_WRITE_SUSPEND)
        CAT2(FIFO_NAME, OnWriteSuspend)(bytesFree);
#endif
        FIFO_Suspend(); // Issue Idle() call
    }
}


// Reset the FIFO state, disables threshold compares, and clears any data
// presently in the FIFO. This must be called once before using the FIFO. It
// can be called again at a later time if desired.
void CAT2(FIFO_NAME, Reset)(void)
{
    // Initialize the FIFO RAM variables
    CAT2(FIFO_NAME, vars).headPtr = CAT2(FIFO_NAME, vars).fifoRAM;
    CAT2(FIFO_NAME, vars).tailPtr = CAT2(FIFO_NAME, vars).fifoRAM;
    CAT2(FIFO_NAME, vars).dataCount = 0;

#if((FIFO_FEATURES) & (FIFO_FEATURE_ON_DATA | FIFO_FEATURE_ON_DATA_LINEAR))
    CAT2(FIFO_NAME, vars).dataThreshold = FIFO_ON_DATA_THRESHOLD_RESET_VALUE;
#endif
#if((FIFO_FEATURES) & (FIFO_FEATURE_ON_FREESPACE | FIFO_FEATURE_ON_FREESPACE_LINEAR))
    CAT2(FIFO_NAME, vars).freespaceThreshold = FIFO_ON_FREESPACE_THRESHOLD_RESET_VALUE;
#endif
}


// Returns the number of bytes that can be read from the FIFO without
// blocking for more data.
unsigned int CAT2(FIFO_NAME, ReadableLength)(void)
{
    #if((FIFO_FEATURES) & (FIFO_FEATURE_ON_READ_LENGTH_CHECK))
    CAT2(FIFO_NAME,OnReadLengthCheck());
    #endif
    return CAT2(FIFO_NAME, vars).dataCount;
}

// Returns the number of bytes that can be written to the FIFO without blocking
// for more space to become available.
unsigned int CAT2(FIFO_NAME, WritableLength)(void)
{
    return sizeof(CAT2(FIFO_NAME, vars).fifoRAM) - CAT2(FIFO_NAME, vars).dataCount;
}

#if((FIFO_FEATURES) & FIFO_FEATURE_READABLE)
// Read 8 bits of data from the FIFO. Use ReadableLength() to determine how
// many bytes can be read without blocking. If an attempt to read more data than
// exists in the FIFO is made, then this function blocks until enough data
// becomes available. Care should be made to ensure this doesn't deadlock your
// application.
// This function is fairly inefficient as it just calls Read() internally.
// Unless you are only interested in one byte, consider calling Read() instead
// and specify a larger block of data for greater performance.
unsigned char CAT2(FIFO_NAME, Read8)(void)
{
    unsigned char i;
    CAT2(FIFO_NAME, Read)(&i, sizeof(i));
    return i;
}

// Just like Read8(), but for 16 bits of data
unsigned short CAT2(FIFO_NAME, Read16)(void)
{
    unsigned short i;
    CAT2(FIFO_NAME, Read)(&i, sizeof(i));
    return i;
}

// Just like Read8(), but for sizeof(unsigned int) bytes of data
unsigned int CAT2(FIFO_NAME, ReadUINT)(void)
{
    unsigned int i;
    CAT2(FIFO_NAME, Read)(&i, sizeof(i));
    return i;
}

// Just like Read8(), but for 24 bits of data

unsigned long CAT2(FIFO_NAME, Read24)(void)
{
    unsigned long i;
    i = 0;
    CAT2(FIFO_NAME, Read)(&i, 3);
    return i;
}

// Just like Read8(), but for 32 bits of data
unsigned long CAT2(FIFO_NAME, Read32)(void)
{
    unsigned long i;
    CAT2(FIFO_NAME, Read)(&i, sizeof(i));
    return i;
}

// Just like Read8(), but for 64-bits bytes of data, assuming your compiler supports the unsigned long long type as 64-bits
unsigned long long CAT2(FIFO_NAME, Read64)(void)
{
    unsigned long long i;
    CAT2(FIFO_NAME, Read)(&i, sizeof(i));
    return i;
}

// Reads an array of bytes from the FIFO. This function will block if you
// attempt to read more bytes than exist in the FIFO. Use ReadableLength() to
// determine how many bytes can be read without blocking. Returns early if it
// was blocking and EOF is reached, the remote FIFO endpoint disconnected, or the
// FIFO is otherwise unavailable.
// Returns the number of bytes actually read. In all but error cases, the number
// of bytes read will match the readLength specified.
unsigned int CAT2(FIFO_NAME, Read)(void *destPtr, unsigned int readLength)
{
    unsigned int maxData, chunkSize;
    unsigned int dataToGo = readLength;
    void *dataPtr = destPtr;

    while(dataToGo)
    {
        chunkSize = dataToGo;
        maxData = (unsigned int)CAT2(FIFO_NAME, vars).fifoRAM + sizeof(CAT2(FIFO_NAME, vars).fifoRAM) - (unsigned int)CAT2(FIFO_NAME, vars).tailPtr;
        if(chunkSize > maxData)
            chunkSize = maxData;
        CAT2(FIFO_NAME, ReadSuspendUntil)(chunkSize);
        FIFO_memcpy(dataPtr, CAT2(FIFO_NAME, vars).tailPtr, chunkSize);
        dataPtr += chunkSize;
        ATOMIC_ADD(CAT2(FIFO_NAME, vars).tailPtr, chunkSize);
        if(chunkSize == maxData)
            CAT2(FIFO_NAME, vars).tailPtr = CAT2(FIFO_NAME, vars).fifoRAM;
        ATOMIC_SUB(CAT2(FIFO_NAME, vars).dataCount, chunkSize);
        dataToGo -= chunkSize;
    }

#if((FIFO_FEATURES) & FIFO_FEATURE_ON_READ)
    // Call the user OnRead() call back function to signal that a read is 
    // complete. The data read and read length is also provided so it can be
    // examined or (possibly) manipulated before passing control to the original
    // code that requested the read. Note that the data pointer points to the
    // caller's RAM space, so trying to read or write from the pointer may have
    // special consequences or behaviors (ex: if pointing to an SFR).
    CAT2(FIFO_NAME, OnRead)(destPtr, readLength);
#endif

    return readLength;
}
#endif


// Reads data from the FIFO, but does not remove it. The OnRead() callback is
// NOT called when peeking at data. The OnReadSuspend() callback is still
// executed, however, if enabled and needed. Additionally, since peeking at data 
// does not remove any data from the FIFO, attempting to peek more data than can 
// fit in the FIFO will result in a deadlock situation and must be avoided. The
// Peek*() functions are otherwise identical to their Read*() counterparts.
#if((FIFO_FEATURES) & (FIFO_FEATURE_PEEKABLE | FIFO_FEATURE_ON_DATA_LINEAR))    // OnDataLinear() requires peeking capability

unsigned char CAT2(FIFO_NAME, Peek8)(void)
{
    unsigned char i;
    CAT2(FIFO_NAME, Peek)(&i, sizeof(i));
    return i;
}

unsigned short CAT2(FIFO_NAME, Peek16)(void)
{
    unsigned short i;
    CAT2(FIFO_NAME, Peek)(&i, sizeof(i));
    return i;
}

unsigned int CAT2(FIFO_NAME, PeekUINT)(void)
{
    unsigned int i;
    CAT2(FIFO_NAME, Peek)(&i, sizeof(i));
    return i;
}

unsigned long CAT2(FIFO_NAME, Peek24)(void)
{
    unsigned long i;
    i = 0;
    CAT2(FIFO_NAME, Peek)(&i, 3);
    return i;
}

unsigned long CAT2(FIFO_NAME, Peek32)(void)
{
    unsigned long i;
    CAT2(FIFO_NAME, Peek)(&i, sizeof(i));
    return i;
}

unsigned long long CAT2(FIFO_NAME, Peek64)(void)
{
    unsigned long long i;
    CAT2(FIFO_NAME, Peek)(&i, sizeof(i));
    return i;
}

unsigned int CAT2(FIFO_NAME, Peek)(void *destPtr, unsigned int readLength)
{
    unsigned int maxData, chunkSize;
    unsigned int dataToGo = readLength;
    unsigned char *rdPtr = CAT2(FIFO_NAME, vars).tailPtr;

    CAT2(FIFO_NAME, ReadSuspendUntil)(readLength);

    while(dataToGo)
    {
        chunkSize = dataToGo;
        maxData = (unsigned int)CAT2(FIFO_NAME, vars).fifoRAM + sizeof(CAT2(FIFO_NAME, vars).fifoRAM) - (unsigned int)rdPtr;
        if(chunkSize > maxData)
            chunkSize = maxData;
        FIFO_memcpy(destPtr, rdPtr, chunkSize);
        destPtr += chunkSize;
        rdPtr += chunkSize;
        if(chunkSize == maxData)
            rdPtr = CAT2(FIFO_NAME, vars).fifoRAM;
        dataToGo -= chunkSize;
    }

    return readLength;
}
#endif  //((FIFO_FEATURES) & (FIFO_FEATURE_PEEKABLE | FIFO_FEATURE_ON_DATA_LINEAR))


// Write a byte to the FIFO. NOTE: This function blocks if you try to Write
// data and there isn't space available for it yet. Use WriteableLength()
// to determine how many bytes can be written before blocking would occur.
// This function is inefficient as it just calls Write() internally.
#if((FIFO_FEATURES) & FIFO_FEATURE_WRITABLE)
void CAT2(FIFO_NAME, Fill)(unsigned char fillChar, unsigned int writeLength)
{
    while(writeLength--)
    {
        CAT2(FIFO_NAME, Write8)(fillChar);
    }
}

void CAT2(FIFO_NAME, Write8)(unsigned char writeData)
{
    CAT2(FIFO_NAME, Write)(&writeData, sizeof(writeData));
}

void CAT2(FIFO_NAME, Write16)(unsigned short writeData)
{
    CAT2(FIFO_NAME, Write)(&writeData, sizeof(writeData));
}

void CAT2(FIFO_NAME, WriteUINT)(unsigned int writeData)
{
    CAT2(FIFO_NAME, Write)(&writeData, sizeof(writeData));
}

void CAT2(FIFO_NAME, Write24)(unsigned long writeData)
{
    CAT2(FIFO_NAME, Write)(&writeData, 3);
}

void CAT2(FIFO_NAME, Write32)(unsigned long writeData)
{
    CAT2(FIFO_NAME, Write)(&writeData, sizeof(writeData));
}

void CAT2(FIFO_NAME, Write64)(unsigned long long writeData)
{
    CAT2(FIFO_NAME, Write)(&writeData, sizeof(writeData));
}

// Writes an array of bytes into the FIFO. This function will block if you
// attempt to write more bytes than can fit in the FIFO. Use WritableLength() to
// determine how many bytes can be written without blocking. Returns early if it
// was blocking and the remote FIFO endpoint disconnected, or the FIFO is
// otherwise unavailable.
// Returns the number of bytes actually written. In all but error cases, the
// number of bytes written will match the writeLength specified.
unsigned int CAT2(FIFO_NAME, Write)(void *sourcePtr, unsigned int writeLength)
{
    unsigned int maxData, chunkSize;
    unsigned int dataToGo;
    void *rdPtr = sourcePtr;

    dataToGo = writeLength;

    while(1)
    {
#if((FIFO_FEATURES) & (FIFO_FEATURE_ON_DATA | FIFO_FEATURE_ON_DATA_LINEAR))
        CAT2(FIFO_NAME, _DispatchOnData)();
#endif
        // Exit once all data has been written and all possible data has been
        // dispatched
        if(dataToGo == 0u)
        {
            break;
        }

        // Check if the FIFO is completely full; suspend if so
        if(CAT2(FIFO_NAME, vars).dataCount == sizeof(CAT2(FIFO_NAME, vars).fifoRAM))
        {
            CAT2(FIFO_NAME, WriteSuspendUntil)(1);
        }

        // At least 1 byte of free space exists, let's move 1 or more bytes into the FIFO
        chunkSize = dataToGo;
        maxData = sizeof(CAT2(FIFO_NAME, vars).fifoRAM) - CAT2(FIFO_NAME, vars).dataCount;
        if(chunkSize > maxData)
        {
            chunkSize = maxData;
        }
        maxData = (unsigned int)CAT2(FIFO_NAME, vars).fifoRAM + sizeof(CAT2(FIFO_NAME, vars).fifoRAM) - (unsigned int)CAT2(FIFO_NAME, vars).headPtr;
        if(chunkSize > maxData)
        {
            chunkSize = maxData;
        }
        FIFO_memcpy(CAT2(FIFO_NAME, vars).headPtr, rdPtr, chunkSize);
        rdPtr += chunkSize;
        ATOMIC_ADD(CAT2(FIFO_NAME, vars).headPtr, chunkSize);
        if(chunkSize == maxData)
        {
            CAT2(FIFO_NAME, vars).headPtr = CAT2(FIFO_NAME, vars).fifoRAM;
        }
        ATOMIC_ADD(CAT2(FIFO_NAME, vars).dataCount, chunkSize);
        dataToGo -= chunkSize;
    }

    // Call the user OnWrite() call back function to signal that a write just took take place (although it is possible that 0 bytes were written - check the writeLength if you care)
#if((FIFO_FEATURES) & FIFO_FEATURE_ON_WRITE)
    CAT2(FIFO_NAME, OnWrite)(sourcePtr, writeLength);
#endif

    return writeLength;
}
#endif


#if((FIFO_FEATURES) & (FIFO_FEATURE_ON_FREESPACE | FIFO_FEATURE_ON_FREESPACE_LINEAR))

void CAT2(FIFO_NAME, SetOnFreespaceThreshold)(unsigned int freespaceThreshold)
{
    CAT2(FIFO_NAME, vars).freespaceThreshold = freespaceThreshold;
    if(freespaceThreshold)
    {
        while(CAT2(FIFO_NAME, WritableLength)() >= freespaceThreshold)
        {
            // See if we can pass a pointer directly to the FIFO RAM
            if(CAT2(FIFO_NAME, vars).fifoRAM + sizeof(CAT2(FIFO_NAME, vars).fifoRAM) - CAT2(FIFO_NAME, vars).headPtr <= freespaceThreshold)
            {
                // Yes, enough space, call the callback and update the Head Pointer upon return
                CAT2(FIFO_NAME, vars).headPtr += CAT2(FIFO_NAME, OnFreespace)(CAT2(FIFO_NAME, vars).headPtr, freespaceThreshold);

                // Check for wrap around in case if the the freespaceThreshold exactly matched the number of bytes available until wrap around.
                if(CAT2(FIFO_NAME, vars).headPtr >= CAT2(FIFO_NAME, vars).fifoRAM + sizeof(CAT2(FIFO_NAME, vars).fifoRAM)
                {
                    // Correct wrap around
                    CAT2(FIFO_NAME, vars).headPtr -= sizeof(CAT2(FIFO_NAME, vars).fifoRAM;
                }
            }
            else
            {
                // No, insufficient space. Create temporary buffer on the stack to allow the client to write data as if a linear data buffer existed. The returned data count will be copied off the stack and into the FIFO with a Write() call.
                __asm__("   add    %0, #1, W1      \n" // Add one to freespaceThreshold so that if it's odd, it'll become even
                        "   bclr   W1, #0          \n" // Clear the LSbit to guarantee it is even
                        "   mov    W15, W0         \n" // Move stack pointer to first function call parameter
                        "   add    W1, W15, W15    \n" // Reserve enough bytes on the stack to client write return data
                        "   push   W0              \n" // Save the pointer, we are going to need it again
                        "   call   _" STRINGIFY(CAT2(FIFO_NAME, OnFreespace)) "\n" // Call the OnFreespace(dataPointer, freespaceLength) callback function
                        "   mov    W0, W1          \n" // Move write data count to second calling parameter
                        "   mov    [W15-2], W0     \n" // Retrieve the original data pointer to the stack area
                        "   call   _" STRINGIFY(CAT2(FIFO_NAME, Write)) "\n" // Write the data actually into the FIFO
                        "   mov    [W15-2], W15    \n" // Restore the original stack pointer
                        : : "r"(freespaceThreshold) : "cc", "w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7", "memory");
            }
        }
    }
}
#endif


// Get rid of these definitions defined in the instancing module so that they
// can define them again and instance us more.
#undef FIFO_NAME
#undef FIFO_SIZE
#undef FIFO_ON_DATA_THRESHOLD_RESET_VALUE
#undef FIFO_ON_FREESPACE_THRESHOLD_RESET_VALUE
#undef FIFO_FEATURES
#undef FIFO_EXTRA_VARS
#undef FIFO_memcpy
#undef FIFO_Suspend
