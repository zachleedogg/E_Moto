/*******************************************************************************
  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM

  File Name:
    EZBL_DumpFlash.c

  Summary:
    Implements the EZBL_DumpFlash() API, useful for displaying bulk
    flash/program space contents on a UART or other stdout destination -
    typically for debugging purposes.

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
unsigned long EZBL_ReadFlash(unsigned long address);
#elif defined(__XC32__) || defined(__PIC32__)
#define EZBL_printf(...)             printf(__VA_ARGS__)
#define EZBL_ReadFlash(address)      (*((unsigned long*)((address) | 0x80000000ul)))
#endif

/**
 * Prints a given flash/program space range in human readable hexadecimal words
 * to stdout.
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
 * @param startAddr Starting memory address to begin reading and printing. This
 *                  value should be even since program memory addresses always
 *                  increment by 0x2 addresses.
 * 
 *                  Must be a legal memory address for the device (address
 *                  beyond the end of flash up to 0x7FFFFF will typically cause
 *                  an Address Error Trap).
 *
 * @param endAddr   End memory address to stop printing when reached. This
 *                  address is NOT printed (i.e. set this to one location after
 *                  the memory of interest).
 */
void EZBL_DumpFlash(unsigned long startAddr, unsigned long endAddr)
{
    unsigned int i;
    unsigned long word;
    unsigned long limitAddr;

    if(endAddr == startAddr)
        return;

    if(startAddr & 0x00000Fu)
    {
        EZBL_printf("\n  %06lX  ", startAddr & 0xFFFFF0u);
        i = startAddr & 0x000Eu;
        while(i)
        {
            EZBL_printf("       ");
            i -= 2u;
        }
        if(startAddr & 0x1u)
        {
            word = EZBL_ReadFlash(startAddr);
            EZBL_printf("%04X   ", (unsigned int)(word>>8));
            startAddr++;
        }
    }

    limitAddr = endAddr & 0xFFFFFEu;
    while(startAddr < limitAddr)
    {
        if((((unsigned int)startAddr) & 0xEu) == 0u)    // Start of new line?
        {
            EZBL_printf("\n  %06lX  ", startAddr & 0xFFFFF0u);
        }
        word = EZBL_ReadFlash(startAddr);
        EZBL_printf("%06lX ", word);
        startAddr += 0x2;
    }

    if(endAddr & 0x1)
    {
        word = EZBL_ReadFlash(endAddr);
        EZBL_printf("  %04X", (unsigned int)(word));
    }
    EZBL_printf("\n");
}
