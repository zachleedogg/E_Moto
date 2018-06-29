/*******************************************************************************
  Easy Bootloader for PIC24 and dsPIC33 Internal Definitions

  File Name:
    ezbl_sha1.h

  Summary:
    Internal definitions for the EZBL SHA-1 hashing submodule

  Description:
    These are internal functions of the bootloader SHA-1 hashing submodule. You
    should not #include this file in any source files unless you are writing a
    EZBL submodule. For ordinary application use of the EZBL bootloader,
    #include "ezbl.h" instead.
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

#ifndef EZBL_SHA1_H
#define	EZBL_SHA1_H


// EZBL_SHA1_FIFO_vars.opFlags flags for interfacing with the FIFO
#define EZBL_SHA1_HASH              0x0001u
#define EZBL_SHA1_ZERO_BLOBS_HASH   0x0002u
#define EZBL_SHA1_ERROR             0x0004u
#define EZBL_SHA1_PASS_TO_PGM       0x0008u

void EZBL_SHA1_FIFO_Reset(void);
void EZBL_SHA1_FIFO_Write8(unsigned char writeData);
void EZBL_SHA1_FIFO_Write16(unsigned short writeData);
void EZBL_SHA1_FIFO_WriteUINT(unsigned int writeData);
void EZBL_SHA1_FIFO_Write24(unsigned long writeData);
void EZBL_SHA1_FIFO_Write32(unsigned long writeData);
void EZBL_SHA1_FIFO_Write64(unsigned long long writeData);
unsigned int EZBL_SHA1_FIFO_Write(void *data, unsigned int length);
//unsigned int EZBL_SHA1_FIFO_WritePGM(unsigned long programAddress, unsigned int byteOffset, unsigned int byteLength);
void EZBL_SHA1_FIFO_Flush(void);

#if !defined(EZBL_SHA1_C)   // Access to certain struct members is needed in ezbl_core.c, but declaring the struct extern first and then locally in ezbl_sha1.c gives a redefinition compiler error.
extern struct
{
    volatile unsigned int dataCount; // Use ATOMIC_ADD() and ATOMIC_SUBTRACT() to change this value
    unsigned char *headPtr;
    unsigned char *tailPtr;
    unsigned int dataThreshold; // Value to compare against dataCount for issuing OnData() callback
    unsigned int opFlags;       /* Bitfield of EZBL_SHA1_* flags defined in ezbl_sha1.h */
    unsigned long length;       /* Number of bytes seen so far */
    unsigned long hash[5];      /* 20-byte SHA1 hash state, memory aligned */
    unsigned char fifoRAM[64];
} EZBL_SHA1_FIFO_vars;
#endif


#endif	/* EZBL_SHA1_H */

