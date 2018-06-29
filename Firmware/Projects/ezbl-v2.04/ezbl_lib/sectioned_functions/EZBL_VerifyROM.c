/*******************************************************************************
  Easy Bootloader for PIC24/dsPIC and PIC32MM (ezbl_lib)

  Summary:
    Implements the EZBL_ROMVerify() ezbl_lib API

  Description:
    See function documentation below.
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

#include "../ezbl.h"

extern volatile unsigned int __attribute__((near)) TBLPAG;


/**
 * Compares an array of RAM/data space bytes to Flash/program space memory and
 * adjusts return results for Bootloading specific problems that make the Flash
 * behave as a non-ideal read/write memory, such as reserved zeros in Flash
 * Config bytes, unimplemented bits in non-volatile Config bytes, and don't-care
 * mismatches between compiler generated .hex data and MPLAB ICSP programming
 * tools (ex: Config bit masking).
 *
 * See EZBL_ROMCompare()/EZBL_ROMCompareEx() if you wish to do binary
 * comparisons without any equivalence adjustment. This is a wrapper function
 * that internally calls EZBL_ROMCompareEx().
 *
 * @param pgmAddress Even program space address to start verification at.
 *
 * @param *compareData Pointer to RAM/data space (including PSV/EDS) byte array
 *        containing the data that was attempted to be written to Flash. The
 *        data must be packed in byte form without phantom padding. I.e.
 *        byte 0 = pgmAddress low 8-bits,
 *        byte 1 = pgmAddress middle 8-bits,
 *        byte 2 = pgmAddress upper 8-bits,
 *        byte 3 = next pgmAddress low 8-bits, etc.
 *
 * @param length Maximum number of bytes to compare. A non-multiple of 3 is
 *               allowed.
 * @param status [out] Optional pointer to a EZBL_MISMATCH_STATUS structure to
 *        receive extended failure data in the event of a mismatch.
 *
 * @return 0x0000 on successful match, which may or may not have required
 *         adjustment.
 *
 *         Negative return codes for uncorrectable verification mismatches.
 *         -1: General failure with no exception allowed. Mismatch may be due to:
 *             - Attempting to overwrite a Bootloader reserved address that
 *               EZBL_WriteROM[Ex]() blocked.
 *             - Incorrect value in EZBL_NVMKey. See EZBL_WriteROM() API requirements.
 *             - Hardware write protect/Code protect settings
 *             - RAM corruption from an asynchronous ISR that isn't saving state or
 *               is causing incorrect concurrent execution of EZBL functions that
 *               require the same hardware.
 *             - Improper/unimplemented address
 *             - Bad voltage or other electricals/environmentals?
 *
 *         Positive return codes for verification mismatches of lessor
 *         significance, such as an expected failed attempt to overwrite a
 *         Bootloader reserved location.
 */
int EZBL_VerifyROM(unsigned long pgmAddress, const void *compareData, unsigned int length, EZBL_MISMATCH_STATUS *status)
{
    int compareFlags = 0;
    EZBL_MISMATCH_STATUS s;
    int pgmDataRestricted = 0;  // Default return status = 0 == success

    // Ensure status points to valid RAM
    if(status == 0)
        status = &s;

    while(1)
    {
        // Compare Flash to RAM data
        if(EZBL_ROMCompareEx(pgmAddress, compareData, length, compareFlags, status) == 0)
        {
            return pgmDataRestricted;   // Match confirmed
        }
        else if(EZBL_IsVerifyRestricted(status->pgmAddress))
        {
            // Match "confirmed", skip to next untested address and continue verification
        }
        else if(((unsigned char*)(&status->pgmAddress))[2] & 0x80u)   // Check for error in "non-volatile" Config bytes or FBOOT
        {
            // Fail if we've already retried with more relaxed settings
            // appropriate for Config bytes
            if(compareFlags)
                return -4;

            // Recompare using data masking to 8-bits and only verify the '0'
            // bits are programmed as '0's.
            pgmAddress = status->pgmAddress;
            compareData = status->dataAddress;
            length -= (unsigned int)status->dataAddress - (unsigned int)compareData;
            compareFlags = EZBL_FLAG_ZEROS | EZBL_FLAG_8BIT;
            continue;
        }
        else if(EZBL_IsProgramRestricted(status->pgmAddress))
        {
            pgmDataRestricted = 1;  // Log that a soft error was seen
        }
        else
        {
            return -1;  // Not a match
        }

        // Match "confirmed" via ignore cases, so skip to next untested address and continue verification
        if(length <= 3u)
            return pgmDataRestricted;
        length -= ((unsigned int)status->dataAddress - (unsigned int)compareData) + 3u;
        pgmAddress  = status->pgmAddress + 0x2;
        compareData = status->dataAddress + 3u;
    }
}
