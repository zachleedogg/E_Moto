/*******************************************************************************
  Easy Bootloader for PIC24/dsPIC and PIC32MM (ezbl_lib)

  Summary:
    Implements the EZBL_EraseAll() ezbl_lib API

  Description:
    Sequentially erases all non-Bootloader pages of Flash.
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
#define XC16_NEAR            __attribute__((near))
#elif defined(__XC32__) || defined(__PIC32__)
#define XC16_NEAR
#endif

extern volatile unsigned int XC16_NEAR EZBL_NVMKey;
int EZBL_EraseAppSpace(unsigned int *eraseState);


/**
 * Erases all non-bootloader Flash pages. The function generally blocks until
 * completion. Pages that are already erased are skipped. Configuration Fuse
 * words in Flash memory are also erased if the bootloader has not reserved the
 * entire last page. However, Config Word values defined in the bootloader are
 * automatically restored. Config Bytes on devices with "Non-volatile" Config
 * Words/Bytes are not erased as they do not have an explicit erased state (i.e.
 * they don't need to be erased in order to reprogram them).
 *
 * @remarks EZBL_NVMKey
 *      The EZBL_NVMKey global variable must be set to a value of
 *      <b>0xFC21</b> before calling this function or the erase will be aborted.
 *      Upon success or abort return, EZBL_NVMKey will be cleared to 0x0000.
 *
 * @param clearWatchdog
 *      Flag specifying if the watchdog timer should be cleared before
 *      each page erase. A non-zero value will trigger a ClrWdt() call before
 *      each internal page erase operation.
 */
void EZBL_EraseAll(int clearWatchdog)
{
    unsigned int i;
    volatile unsigned int originalKey;

    originalKey = EZBL_NVMKey - 5;      // Each call to EZBL_EraseAppSpace() kills EZBL_NVMKey, so must cache the value
    EZBL_NVMKey = 0x0000;
    i = 0;
    do
    {
        if(clearWatchdog)
        {
            __builtin_clrwdt();
        }
        EZBL_NVMKey += originalKey + 5; // Required unlock value for EZBL_EraseAppSpace() is 0xFC21
    } while(EZBL_EraseAppSpace(&i));
}
