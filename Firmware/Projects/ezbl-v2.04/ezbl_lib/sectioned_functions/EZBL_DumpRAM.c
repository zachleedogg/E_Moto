/*******************************************************************************
  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM

  File Name:
    EZBL_DumpRAM.c

  Summary:
    Implements the EZBL_DumpRAM() API, useful for displaying bulk RAM/SFR
    contents on a UART or other stdout destination - typically for debugging
    purposes.

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

int EZBL_printf(const char *format, ...);


/**
 * Prints a given RAM/SFR data range in human readable hexadecimal words to
 * stdout.
 *
 * Use the UART_Reset() or I2C_Reset() macros to initialize the _write handler
 * for stdout. Also, for UARTs, ensure the TX I/O pin has a PPS assignment for
 * it, assuming it needs one. Ex:
 *         // Output on U2TX pin assigned to RP45 at 460800 baud, 8N1, with a
 *         // system clock of 70MIPS, and set this UART interface as the stdio
 *         // target.
 *         _RP45R = _RPOUT_U1TX;
 *         UART_Reset(2, 70000000, 460800, 1);
 *
 * @param startPtr Starting memory address pointer to begin reading and
 *                 printing. This value can be odd (high byte in a 16-bit
 *                 word).
 *
 *                 Pointers to 0x8000 and above will point to PSVPAG/DSRPAG
 *                 space (typically a 32KB chunk of flash contents instead of
 *                 RAM).
 *
 * @param byteLen   Number of bytes at startPtr to print
 */
void EZBL_DumpRAM(const void *startPtr, unsigned int byteLen)
{
    unsigned int i;

    if(byteLen == 0u)
        return;

    if(((unsigned int)startPtr) & 0x000Fu)
    {
        EZBL_printf("\n  %04X  ", ((unsigned int)startPtr) & 0xFFF0u);
        i = ((unsigned int)startPtr) & 0x000Eu;
        while(i)
        {
            EZBL_printf("     ");
            i -= 2u;
        }
        if(((unsigned int)startPtr) & 0x0001u)
        {
            EZBL_printf("%02X   ", (unsigned int)*((unsigned char*)startPtr));
            startPtr++;
            byteLen--;
        }
    }

    i = (((unsigned int)startPtr) + byteLen) & 0xFFFEu;
    while(byteLen >= 2u)
    {
        if((((unsigned int)startPtr) & 0xFu) == 0u)    // Start of new line?
        {
            EZBL_printf("\n  %04X  ", ((unsigned int)startPtr) & 0xFFF0u);
        }
        EZBL_printf("%04X ", *((unsigned int*)startPtr));
        startPtr += 2u;
        byteLen -= 2u;
    }

    if(byteLen)
    {
        EZBL_printf("  %02X", (unsigned int)(*((unsigned char*)(startPtr))));
    }
    EZBL_printf("\n");
}
