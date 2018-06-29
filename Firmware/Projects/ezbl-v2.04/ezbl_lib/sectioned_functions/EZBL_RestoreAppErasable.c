/*******************************************************************************
  Easy Bootloader for PIC24/dsPIC and PIC32MM (ezbl_lib)

  Summary:
    Implements the EZBL_RestoreAppErasable() ezbl_lib API

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

// Header for public API definitions that we are implementing in this file.
#include "../ezbl.h"


//#define EZBL_DEBUG     // Uncomment to allow debugging messages to be printed to stdout

#if !defined(EZBL_DEBUG)
#define EZBL_printf(...)        do{}while(0)
#define EZBL_FIFOFlush(...)     do{}while(0)
#endif


#if !defined(OSCCON)
extern volatile unsigned int XC16_NEAR  OSCCON;
typedef struct {
  union {
    struct {
      unsigned OSWEN:1;
      unsigned LPOSCEN:1;
      unsigned :1;
      unsigned CF:1;
      unsigned :1;
      unsigned LOCK:1;
      unsigned IOLOCK:1;
      unsigned CLKLOCK:1;
      unsigned NOSC:3;
      unsigned :1;
      unsigned COSC:3;
    };
    struct {
      unsigned :8;
      unsigned NOSC0:1;
      unsigned NOSC1:1;
      unsigned NOSC2:1;
      unsigned :1;
      unsigned COSC0:1;
      unsigned COSC1:1;
      unsigned COSC2:1;
    };
  };
} OSCCONBITS;
extern volatile OSCCONBITS XC16_NEAR OSCCONbits;
#endif


// Alternate, long name API export for the EZBL_RestoreAppErasable() function. This is for code compatibility with EZBL v1.xx versions.
extern void EZBL_RestoreAppErasableBootloaderItems(unsigned int restoreConfigBytes) __attribute__((alias("EZBL_RestoreAppErasable")));


/**
 * Restores any Bootloader components (defined in ".text.EZBLEraseRestoreTable"
 * code section when Bootloader was linked) that might have been erased by
 * a Flash erase operation.
 * 
 * For example, on a device with device Config words located in flash at the end
 * of Application space but with Bootloader defined Config words present, an
 * erase of the last flash page must have Bootloader content reprogrammed before
 * any Application data can be written there to minimize bricking risk to the
 * Bootloader.
 *
 * It is safe to call this function multiple times if the restoreConfigBytes 
 * parameter = 0, even if all other bootloader components are already 
 * programmed to flash. The function first checks the destination address to see
 * if data is present before attempting to program it. On the other hand, if the 
 * device has Config bytes (Config words at >= address 0xF80000) and the 
 * restoreConfigBytes flag is non-zero, then this function should only be called 
 * once when needing to restore these Config bytes. Non-Flash based Config bytes
 * are always reprogrammed since they don't have a detectable erased state 
 * (like EEPROM, you just reprogram them without erasing first).
 * 
 * If a Reserved Bit in a flash Config word exists that requires a '0' to
 * be programmed to it, this bit is written to '0' by the 
 * __EZBL_TOOLS_COMPUTED_ERASE_RESTORE_TABLE element that applies to it
 * (assuming such an element was generated).
 * 
 * @param restoreConfigBytes On devices with "Non-volatile" Config bytes (not 
 *          flash based Config Words), a non-zero value for this flag specifies
 *          that any Bootloader defined Config bytes should be reprogrammed by
 *          this function. As noted above, these values are always programmed 
 *          for every call to EZBL_RestoreAppErasable() when this flag is set, 
 *          so minimize unneeded restore operations with this flag set to avoid
 *          wasting Config byte endurance.
 * 
 *          On devices with Flash based Config words ("Volatile Config Words"), 
 *          this flag is not applicable and ignored.
 * 
 * @return Any Flash address that the Bootloader requires and has stored in the
 *         .text.EZBLEraseRestoreTable section is reprogrammed, assuming the
 *         destination flash doesn't already have mismatched data stored
 *         there. Additionally, if the device has "Non-volatile" Config bytes
 *         and the restoreConfigBytes parameter is non-zero, the Bootloader's
 *         Config bytes will be programmed.
 */
void EZBL_RestoreAppErasable(unsigned int restoreConfigBytes)
{
    unsigned char *buf;
    unsigned int bufSizeBytes;
    unsigned long destAddr;
    unsigned int byteLen;
    unsigned int flags;

    // Create a dummy 0 item section in case if there are no calls to EZBL_SetNoEraseRanges(). This takes no space.
    __asm__("     .pushsection    .text.EZBLEraseRestoreTable, code, keep, page\n"
            "     .popsection");


    bufSizeBytes = EZBL_Mul3Div2(__builtin_section_size(".text.EZBLEraseRestoreTable"));    // Find out how big the erase restore table is in program addresses, then convert to bytes
    buf          = __builtin_alloca(bufSizeBytes);                                          // Allocate sufficient size in RAM for the whole table
    EZBL_ReadPackedFlash(buf, __builtin_section_begin(".text.EZBLEraseRestoreTable"), 0, bufSizeBytes); // Read the whole table into RAM

    EZBL_printf("\n    .text.EZBLEraseRestoreTable stored at: [%06lX, %06lX)", __builtin_section_begin(".text.EZBLEraseRestoreTable"), __builtin_section_begin(".text.EZBLEraseRestoreTable") + __builtin_section_size(".text.EZBLEraseRestoreTable"));

    while(bufSizeBytes > 6u)
    {
        ((unsigned char*)&destAddr)[3] = 0x00;              // destAddr = buf[0-2];
        EZBL_RAMCopy(&destAddr, &buf[0], 3);
        EZBL_RAMCopy(&byteLen, &buf[3], sizeof(byteLen));   // byteLen = buf[3-4]; [5] is a dummy and discarded
        EZBL_printf("\n   destAddr = %06lX, byteLen = %u", destAddr, byteLen);

        flags = EZBL_FLAG_ZEROS;
        if(buf[2] == 0xF8u)
            flags = EZBL_FLAG_8BIT;
        if((buf[2] <= 0x7Fu) || restoreConfigBytes)
        {
            if(EZBL_ROMCompareEx(destAddr, &buf[6], byteLen, flags, 0))             // Check if flash already matches and we can skip programming. Not really needed since EZBL_WriteROMOnlyOnceEx() won't write to a non-blank (non 0xFFFFFF) location, but possibly needed for suppressing Config byte writes in the 0xF8xxxx address range on applicable devices.
            {
#if defined(EZBL_DEBUG)
                {
                    int i;
                    unsigned long wordData;

                    EZBL_printf("\n   No flash/restore match"); 

                    for(i = 0; i < byteLen; i += 3u)
                    {
                        ((unsigned char*)&wordData)[0] = buf[i+6];
                        ((unsigned char*)&wordData)[1] = buf[i+7];
                        ((unsigned char*)&wordData)[2] = buf[i+8];
                        ((unsigned char*)&wordData)[3] = 0x00;
                        EZBL_printf("\n      Expect: %06lX, Read: %06lX", wordData, EZBL_ReadFlash(destAddr + i*2u/3u));
                        EZBL_FIFOFlush(EZBL_STDOUT, (unsigned long)-1);
                    }
                }
#endif
                EZBL_NVMKey = 0x03DF;
                EZBL_WriteROMOnlyOnceEx(destAddr, &buf[6], byteLen, 0, 0xDEAD, 0);  // Use special 0xDEAD no-program range count to disable no-program range write suppression. This allows us to re-write to these shared Bootloader memory regions like Flash config words on last page.
                EZBL_printf("\n   Restore complete");
            }
        }
        bufSizeBytes -= byteLen + 6u;
        buf += byteLen + 6u;    // Leaks allocated *buf pointer base address, but since it is on the stack and gcc deallocs the stack frame anyway, this is no problem.
    }
}


                // Extra code might be useful on PIC24H/dsPIC33F devices which
                // cannot program _FOSC while running from the PLL, even if the
                // FOSC value is reprogrammed with the same value that is
                // already being used.
//                if(buf[2] == 0xF8u)
//                {
//                    // Switch to FRC clock (no PLL), in case if the device is a
//                    // PIC24H or dsPIC33FJ device. Writing to FOSC can't be done
//                    // unless using the FRC.
//                    unsigned int OSCCONSave = OSCCON;
//                    __builtin_write_OSCCONH(0x00);
//                    __builtin_write_OSCCONL(0x01);
//
//                    // Wait for clock switch to complete
//                    while(OSCCONbits.OSWEN && (OSCCONbits.COSC != OSCCONbits.NOSC));
//
//                    EZBL_NVMKey = 0x03DF;
//                    EZBL_WriteROM(destAddr, &buf[6], byteLen);
//                    __builtin_write_OSCCONH(OSCCONSave>>8);      // Initiate Clock Switch to previous oscillator setting
//                    __builtin_write_OSCCONL(((unsigned char)OSCCONSave) | 0x01);
//                }