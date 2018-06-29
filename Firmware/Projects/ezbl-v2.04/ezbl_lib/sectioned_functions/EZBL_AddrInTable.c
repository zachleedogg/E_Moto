/*******************************************************************************
  Easy Bootloader for PIC24/dsPIC and PIC32MM (ezbl_lib)

  Summary:
    Implements the EZBL_AddrInTable() ezbl_lib API
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


typedef union
{
    unsigned long long  u64;
    unsigned long       u32[2];
    unsigned short      u16[4];
    unsigned char       u8[8];
} EZBL_UNION_64;


#if defined(__XC16__)
unsigned long long EZBL_ReadTablePair(unsigned long *pgmReadAddr);
#elif defined(__XC32__) || defined(__PIC32__)
static inline unsigned long long __attribute__((always_inline)) EZBL_ReadTablePair(unsigned long *pgmReadAddr)
{
    EZBL_UNION_64 data;
    data.u32[0] = *pgmReadAddr++;
    data.u32[1] = *pgmReadAddr++;
    return data.u64;
}
#endif


/**
 * Checks the given program space address lies within a Bootloader defined
 * special address range defined by instances of EZBL_SetNoEraseRange(),
 * EZBL_SetNoProgramRange(), EZBL_SetNoVerifyRange() or EZBL_SetNoReadRange().
 *
 * @param tableBaseAddr Unsigned long start address for an EZBL program table.
 *                      Use the __builtin_section_begin() function to obtain the
 *                      base address for a table.
 * @param tablePgmAddrLen Unsigned int size of the EZBL program table, in
 *                      program space address. Use __builtin_section_size() to
 *                      retrieve this value.
 * @param testAddress Program space address to test. Unaligned addresses are
 *                    testable.
 *
 * @return 0 if the test address was not found in the given program table. 1
 *         otherwise.
 */
int EZBL_AddrInTable(unsigned long tableBaseAddr, unsigned int tablePgmAddrLen, unsigned long testAddress)
{
    EZBL_UNION_64 pair;

    while(tablePgmAddrLen)
    {
        pair.u64 = EZBL_ReadTablePair(&tableBaseAddr);
        if((testAddress >= pair.u32[0]) && (testAddress < pair.u32[1]))
            return 1;
        tablePgmAddrLen -= 0x4u;
    }

    return 0;
}
