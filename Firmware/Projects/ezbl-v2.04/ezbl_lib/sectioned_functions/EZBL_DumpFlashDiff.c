/*******************************************************************************
  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM

  File Name:
    EZBL_DumpFlashDiff.c

  Summary:
    Implements the EZBL_DumpFlashDiff() API, useful for displaying an expected
    byte array of flash/program space contents compared with a second RAM array
    that may be different. Flash instruction words that differ and highlighted 
    in red on a UART or other stdout destination - function typically used for
    debugging purposes.

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


#if defined(__XC16__)
int EZBL_printf(const char *format, ...);
#elif defined(__XC32__) || defined(__PIC32__)
#include <stdio.h>
#define EZBL_printf(...)                        printf(__VA_ARGS__)
#endif


/**
 * On stdout, displays one packed RAM byte array of flash instruction words
 * compared against a second RAM byte array of instruction words. Differences
 * are highlighted in red on a per-instruction basis on ANSI compatible terminal
 * emulation software.
 *
 * Data is displayed for both arrays an hex ASCII word groups of 24-bits each
 * (little endian). Ex:
 *
 * EZBL_DumpFlashDiff("Expect: ", "Read-back: ", 0x001004, &expectData[0], &readData[0], sizeof(expectData));
 *
 *   Expect:    001000                FDC000 60036F DE0044 E1306A AF0042 434307
 *              001010  436C03 E11C18 AE2042 EB0100 540402 E90285 320003 628363
 *              001020  3AFFF2 37FFED E9014F 57807C 540200 AE0042 E80204 97B41F
 *              001030  510284 3C0002 A14001 370011 E90105 200207 A34801 3A000D
 *              001040  A33801 320009
 *   Read-back: 001000                FDC000 60036F DE0044 E1306A AF0042 434307
 *              001010  436C03 E11C18 AE2000 EB0100 540402 E90285 320003 628363
 *              001020  3AFFF2 37FFED E9014F 57807C 540200 AE0042 E80204 97B41F
 *              001030  510284 3C0002 A14001 370011 E90105 200207 A34801 3A000D
 *              001040  A338FF 320009
 *
 *
 * @param refMsgPreFix Null terminated string to prefix blocks of the reference
 *                     array during printing. Ex: "Expect: ".
 *
 *                     This parameter can be null if no prefixing is desired.
 *
 *                     Space padding is added such that refMsgPreFix and
 *                     cmpMsgPreFix are both the same length and the printed
 *                     data is easily viewed in columns.
 *
 * @param cmpMsgPreFix Null terminated string to prefix blocks of the compared
 *                     array during printing. Ex: "Read-back: ".
 *
 *                     This parameter can be null if no prefixing is desired.
 *
 *                     Space padding is added such that refMsgPreFix and
 *                     cmpMsgPreFix are both the same length and the printed
 *                     data is easily viewed in columns.
 *
 * @param addr Program memory start address to display in the right-most column
 *             of each line. This address is not used to do a flash read-back
 *             but instead is merely used to make sense of each line's compared
 *             data within the physical device.
 *
 *             Each line increments by 0x10 program addresses (8 instruction
 *             words), and a non-multiple of 0x10 is allowed for addr. In such a
 *             case, the address will become aligned and a gap of spaces will be
 *             inserted to align all subsequent columns.
 *
 *             addr should be an even number (odd numbers will have the LSbit
 *             cleared internally for address printing).
 *
 * @param dataRef RAM located byte array in packed form where each 3 bytes
 *                represents one instruction word. This pointer defines the dump
 *                data which will always be neutrally colored (i.e. white text).
 *
 *                This parameter cannot be null unless byteLen is zero.
 *
 * @param dataCmp Compare data located in RAM as a RAM byte array synonymous
 *                with the dataRef pointer. However, any mismatched instruction
 *                words will be displayed in red text.
 *
 *                If a regular dump of flash is desired without any compare
 *                data, this parameter can be null.
 *
 * @param byteLen Number of bytes in both the dataRef and dataCmp (when
 *                applicable) arrays to display/compare. This length can be a
 *                non-multiple of 3 bytes.
 *
 * @return void.
 *
 *         dataRef and dataCmp (when not null) is printed to stdout, as defined
 *         by the write() XC16 function or _mon_putc() XC32 function.
 */
#if defined(__XC16__)
void EZBL_DumpFlashDiff(const char *refMsgPreFix, const char *cmpMsgPreFix, unsigned long addr, const void *dataRef, const void *dataCmp, unsigned int byteLen)
{
    unsigned long originalAddr;
    unsigned int originalByteLen;
    unsigned int lineStartPadding;
    unsigned char *buf, *refBuf;
    const char *msgPrefix;
    int red;
    unsigned int i;
    unsigned int j;
    union
    {
        unsigned long u32;
        unsigned short u16[2];
        unsigned char u8[4];
    } word, refWord;


    originalByteLen = byteLen;
    originalAddr = addr;
    red = 0;
    word.u32    = 0x00000000;
    refWord.u32 = 0x00000000;

    // Find field width for padding start of lines
    lineStartPadding = __builtin_strlen(refMsgPreFix);
    if(cmpMsgPreFix)
    {
        j = __builtin_strlen(cmpMsgPreFix);
        if(j > lineStartPadding)
            lineStartPadding = j;
    }

    msgPrefix = refMsgPreFix;
    buf = (unsigned char*)dataRef;
    for(j = 0; j <= (((int)dataCmp) != 0); j++)
    {
        addr = originalAddr & 0xFFFFFFF0;
        EZBL_printf("\n%-*s%06lX  ", lineStartPadding, msgPrefix, addr);
        refBuf = (unsigned char*)dataRef;
        red = (((unsigned short)originalAddr & 0xFFFEu) - (unsigned short)addr)/2u;
        for(i = 0; i < red; i++)
        {
            EZBL_printf("       ");
            addr += 0x2u;
        }
        red = 0;

        while(byteLen >= 3u)
        {
            word.u8[0] = *buf++;
            word.u8[1] = *buf++;
            word.u8[2] = *buf++;
            if(j)
            {
                refWord.u8[0] = *refBuf++;
                refWord.u8[1] = *refBuf++;
                refWord.u8[2] = *refBuf++;

                if(word.u32 != refWord.u32)
                {
                    if(!red)
                    {
                        EZBL_printf("\x1B[31m");// \x1B[31m = ANSI red text
                        red = 1;
                    }
                }
                else if(red)
                {
                    EZBL_printf("\x1B[0m");// \x1B[0m = ANSI Reset All Attributes
                    red = 0;
                }
            }
            EZBL_printf("%06lX ", word.u32);
            addr += 0x2u;
            byteLen -= 3u;
            if((++i == 8u) && byteLen) // Print 8 word groups then make a new line
            {
                EZBL_printf("\n%-*s%06lX  ", lineStartPadding, "", addr);
                i = 0;
            }
        }
        
        if(byteLen)
        {
            word.u8[0] = *buf++;
            word.u8[1] = 0x00;
            if(byteLen >= 2u)
                word.u8[1] = *buf++;

            if(j)
            {
                refWord.u8[0] = *refBuf++;
                word.u8[1] = 0x00;
                if(byteLen >= 2u)
                    refWord.u8[1] = *refBuf++;
                if(word.u16[0] != refWord.u16[0])
                {
                    if(!red)
                    {
                        EZBL_printf("\x1B[31m");// \x1B[31m = ANSI red text
                        red = 1;
                    }
                }
                else if(red)
                {
                    EZBL_printf("\x1B[0m");// \x1B[0m = ANSI Reset All Attributes
                    red = 0;
                }
            }
            EZBL_printf("%*s%0*X ", 6u-byteLen*2u, "", byteLen*2u, word.u16[0]);
        }

        if(red)
        {
            EZBL_printf("\x1B[0m");// \x1B[0m = ANSI Reset All Attributes
        }

        addr = originalAddr;
        byteLen = originalByteLen;
        buf = (unsigned char*)dataCmp;
        msgPrefix = cmpMsgPreFix;
        if(msgPrefix == 0)
           msgPrefix = "";
    }
}
#endif


#if defined(__PIC32__) || defined(__XC32__)
void EZBL_DumpFlashDiff(const char *refMsgPreFix, const char *cmpMsgPreFix, unsigned long addr, const void *dataRef, const void *dataCmp, unsigned int byteLen)
{
    unsigned int i;
    unsigned int j;
    unsigned int lineStartPadding;
    unsigned char *buf;
    unsigned int originalByteLen;
    unsigned long originalAddr;
    const char *msgPreFix = "";
    int red;

    lineStartPadding  = 0;
    if(refMsgPreFix)
    {
        lineStartPadding = __builtin_strlen(refMsgPreFix);
        msgPreFix = refMsgPreFix;
    }
    originalByteLen = byteLen;
    originalAddr = addr;
    red = 0;
    if(cmpMsgPreFix)
    {
        j = __builtin_strlen(cmpMsgPreFix);
        if(j > lineStartPadding)
            lineStartPadding = j;
    }

    for(j = 0; j < 1u + (((int)dataCmp) != 0); j++)
    {
        if(j > 0)
        {
            msgPreFix = "";
            if(cmpMsgPreFix)
                msgPreFix = cmpMsgPreFix;
        }
        EZBL_printf("\n%-*s%08X  ", lineStartPadding, msgPreFix, (unsigned int)addr);
        buf = (unsigned char*)(j == 0 ? dataRef : dataCmp);
        for(i = 0; byteLen >= 4u; byteLen -= 4u)
        {
            if(j == 1u)
            {
                if((((unsigned char*)dataRef)[i+3] != ((unsigned char*)dataCmp)[i+3]) || (((unsigned char*)dataRef)[i+2] != ((unsigned char*)dataCmp)[i+2]) || (((unsigned char*)dataRef)[i+1] != ((unsigned char*)dataCmp)[i+1]) || (((unsigned char*)dataRef)[i+0] != ((unsigned char*)dataCmp)[i+0]))
                {
                    if(!red)
                    {
                        EZBL_printf("\x1B[31m");// \x1B[31m = ANSI red text
                        red = 1;
                    }
                }
                else if(red)
                {
                    EZBL_printf("\x1B[0m");// \x1B[0m = ANSI Reset All Attributes
                    red = 0;
                }
            }
            EZBL_printf("%02X%02X%02X%02X ", (unsigned int)buf[i+3], (unsigned int)buf[i+2], (unsigned int)buf[i+1], (unsigned int)buf[i+0]);
            i += 4u;
            addr += 0x4u;
            if(byteLen > 4u)
            {
                if(i % 16u == 0u)   // Print 4 word groups then make a new line
                    EZBL_printf("\n%-*s%08X  ", lineStartPadding, "", (unsigned int)addr);
            }
        }
        if(byteLen == 3u)
        {
            if(j == 1u)
            {
                if((((unsigned char*)dataRef)[i+2] != ((unsigned char*)dataCmp)[i+2]) || (((unsigned char*)dataRef)[i+1] != ((unsigned char*)dataCmp)[i+1]) || (((unsigned char*)dataRef)[i+0] != ((unsigned char*)dataCmp)[i+0]))
                {
                    if(!red)
                    {
                        EZBL_printf("\x1B[31m");// \x1B[31m = ANSI red text
                        red = 1;
                    }
                }
                else if(red)
                {
                    EZBL_printf("\x1B[0m");// \x1B[0m = ANSI Reset All Attributes
                    red = 0;
                }
            }
            EZBL_printf("  %02X%02X%02X", (unsigned int)buf[i+2], (unsigned int)buf[i+1], (unsigned int)buf[i]);
        }
        else if(byteLen == 2u)
        {
            if(j == 1u)
            {
                if((((unsigned char*)dataRef)[i+1] != ((unsigned char*)dataCmp)[i+1]) || (((unsigned char*)dataRef)[i+0] != ((unsigned char*)dataCmp)[i+0]))
                {
                    if(!red)
                    {
                        EZBL_printf("\x1B[31m");// \x1B[31m = ANSI red text
                        red = 1;
                    }
                }
                else if(red)
                {
                    EZBL_printf("\x1B[0m");// \x1B[0m = ANSI Reset All Attributes
                    red = 0;
                }
            }
            EZBL_printf("    %02X%02X", (unsigned int)buf[i+1], (unsigned int)buf[i]);
        }
        else if(byteLen == 1u)
        {
            if(j == 1u)
            {
                if((((unsigned char*)dataRef)[i+0] != ((unsigned char*)dataCmp)[i+0]))
                {
                    if(!red)
                    {
                        EZBL_printf("\x1B[31m");// \x1B[31m = ANSI red text
                        red = 1;
                    }
                }
                else if(red)
                {
                    EZBL_printf("\x1B[0m");// \x1B[0m = ANSI Reset All Attributes
                    red = 0;
                }
            }
            EZBL_printf("      %02X", (unsigned int)buf[i]);
        }
        if(red)
        {
            EZBL_printf("\x1B[0m");// \x1B[0m = ANSI Reset All Attributes
            red = 0;
        }

        byteLen = originalByteLen;
        addr = originalAddr;
    }
}
#endif