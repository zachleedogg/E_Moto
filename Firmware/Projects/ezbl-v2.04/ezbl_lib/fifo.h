/*******************************************************************************
  OBSOLETE - EZBL v1.13+ - DO NOT USE IN NEW CODE

  Multipurpose FIFO Source Library

  File Name:
    fifo.h

  Author:
    Howard Schlunder

  Summary:
    Definitions needed by and required for instancing the fifo.c.h module

  Description:
    Feature definitions needed by and required for instancing the fifo.c.h
    module. This header must be #included once before #including fifo.c.h for
    the first time. For a complete description of what each feature does, see
    fifo.c.h documentation.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Copyright (C) 2015 Microchip Technology Inc.

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
#if !defined(FIFO_H)
#define FIFO_H

//  OBSOLETE - EZBL v1.13+ - DO NOT USE IN NEW CODE


#define FIFO_FEATURE_READABLE               0x00000001ul    // Enables (FIFO_NAME)Read*() functions. If no readability is specified, you must externally implement all needed read functionality using the internal data structure directly.
#define FIFO_FEATURE_WRITABLE               0x00000002ul    // Enables (FIFO_NAME)Write*() functions. If no writability is specified, you must externally implement all needed write functionality using the internal data structure directly.
#define FIFO_FEATURE_PEEKABLE               0x00000004ul    // Enables (FIFO_NAME)Peek*() functions.
#define FIFO_FEATURE_ON_WRITE               0x00000008ul    // Enables the void (FIFO_NAME)OnWrite(void *writeData, unsigned int writeLength) callback function to be called after data is written to the FIFO.
#define FIFO_FEATURE_ON_WRITE_SUSPEND       0x00000010ul
#define FIFO_FEATURE_ON_READ                0x00000020ul    // Enables the void (FIFO_NAME)OnRead(void *readData, unsigned int dataLength) callback function to be called after data is read from the FIFO.
#define FIFO_FEATURE_ON_READ_SUSPEND        0x00000040ul
#define FIFO_FEATURE_ON_READ_LENGTH_CHECK   0x00000100ul
#define FIFO_FEATURE_ON_DATA                0x00000200ul    // void (FIFO_NAME)OnData(unsigned int dataLength); Use (FIFO_NAME)Read*() functions to actually obtain the data and remove them from the FIFO.
#define FIFO_FEATURE_ON_DATA_LINEAR         0x00000400ul    // unsigned int (FIFO_NAME)OnDataLinear(void *peekData, unsigned int dataLength); return value is the number of bytes to remove from the FIFO.
#define FIFO_FEATURE_ON_FREESPACE           0x00000800ul    // void (FIFO_NAME)OnFreespace(unsigned int dataLength); Use (FIFO_NAME)Write*() functions to actually fill the FIFO with data.
#define FIFO_FEATURE_ON_FREESPACE_LINEAR    0x00001000ul    // unsigned int (FIFO_NAME)OnFreespaceLinear(void *writeData, unsigned int dataLength); return value is the number of bytes written to the writeData pointer which need to be added to the FIFO.
#define FIFO_FEATURE_ON_RESET               0x00002000ul    // Enables the void (FIFO_NAME)OnReset(void) callback function.
#define FIFO_FEATURE_ON_EOF                 0x00004000ul    // Unimplemented
#define FIFO_FEATURE_ON_TIMER               0x00008000ul    // Unimplemented
#define FIFO_FEATURE_ON_OVERFLOW            0x00010000ul    // Unimplemented
#define FIFO_FEATURE_ON_UNDERFLOW           0x00020000ul    // Unimplemented
#define FIFO_FEATURE_OVERLAY                0x00040000ul    // Unimplemented
#define FIFO_FEATURE_PROVIDE_LINEAR         0x00080000ul    // Provide a linear chunk of data or freespace for use in the OnData() and OnFreespace() callback functions. Enables the void (FIFO_NAME)SetOnDataThreshold(unsigned int dataAvailable) and void (FIFO_NAME)SetOnFreespaceThreshold(unsigned int freespaceThreshold) APIs, as applicable.


// Static macros for concatenating tokens together without making them strings
// first. This is useful for prefixing letters or words onto functions,
// variables, and other symbol names.
// Ex: you can write 1 to the T2CONbits.ON register bit using:
// #define TIMER_SELECT  2
//     CAT3(T,TIMER_SELECT,CONbits.ON) = 1;
// The preprocessor will resolve this into:
//     T2CONbits.ON = 1;
#if !defined(CAT2)
#define CAT2_IN(a0,a1)              a0##a1                      // Not recommended to use this macro directly; macro expansion won't occur unless another macro wraps it.
#define CAT3_IN(a0,a1,a2)           a0##a1##a2                  // Not recommended to use this macro directly; macro expansion won't occur unless another macro wraps it.
#define CAT4_IN(a0,a1,a2,a3)        a0##a1##a2##a3              // Not recommended to use this macro directly; macro expansion won't occur unless another macro wraps it.
#define CAT5_IN(a0,a1,a2,a3,a4)     a0##a1##a2##a3##a4          // Not recommended to use this macro directly; macro expansion won't occur unless another macro wraps it.
#define CAT6_IN(a0,a1,a2,a3,a4,a5)  a0##a1##a2##a3##a4##a5      // Not recommended to use this macro directly; macro expansion won't occur unless another macro wraps it.
#define CAT2(a0,a1)                 CAT2_IN(a0,a1)              // Use this; allows macro expansion
#define CAT3(a0,a1,a2)              CAT3_IN(a0,a1,a2)           // Use this; allows macro expansion
#define CAT4(a0,a1,a2,a3)           CAT4_IN(a0,a1,a2,a3)        // Use this; allows macro expansion
#define CAT5(a0,a1,a2,a3,a4)        CAT5_IN(a0,a1,a2,a3,a4)     // Use this; allows macro expansion
#define CAT6(a0,a1,a2,a3,a4,a5)     CAT6_IN(a0,a1,a2,a3,a4,a5)  // Use this; allows macro expansion
#endif


// Macros for converting a macro value into a string representation of the
// macro value. This is needed for concatenating macro contents to other strings.
// Using STRINGIFY_INNER() directly does not allow macro expansion.
// Using STRINGIFY() does perform macro expansion of value before passing
// to the inner version.
// As an example, consider this code:
//  #define APP_VERSION_MAJOR    3
//  #define APP_VERSION_MINOR    14
//      UARTPrintROMString("Firmware version: " STRINGIFY(APP_VERSION_MAJOR) "." STRINGIFY(APP_VERSION_MINOR) "\r\n");
// Here, the version macros are integer compile-time constants. With
// stringification, the UARTPrintROMString() function doesn't need any logic to
// do binary to ASCII number conversion like the heavyweight printf() function.
#if !defined(STRINGIFY)
#define STRINGIFY_INNER(x)              #x
#define STRINGIFY(value)                STRINGIFY_INNER(value)
#endif

#endif  // #if !defined(FIFO_H)
