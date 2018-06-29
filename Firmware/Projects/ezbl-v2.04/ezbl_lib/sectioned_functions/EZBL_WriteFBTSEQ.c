/*******************************************************************************
  Easy Bootloader for PIC24/dsPIC and PIC32MM (ezbl_lib)

  Summary:
    Implements the EZBL_WriteFBTSEQ() ezbl_lib API

  Description:
    On dual-partition capable (and configured) devices, writes or changes the
    Inactive Partition or Active Partition's FBTSEQ Flash Configuration word.
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

extern XC16_NEAR struct // Removed volatile since compiler caching can't cross function boundaries
{
    unsigned NVMOP:4;
    unsigned :4;
    unsigned URERR:1;
    unsigned RPDF:1;
    unsigned P2ACTIV:1;
    unsigned SFTSWP:1;
    unsigned NVMSIDL:1;
    unsigned WRERR:1;
    unsigned WREN:1;
    unsigned WR:1;
} NVMCONbits;

extern int __attribute__((weak)) _FLASH_PARTITION;


/**
 * On a Dual Partition device, writes or rewrites a Partition's FBTSEQ value.
 *
 * This function can target either the Active Partition or the Inactive
 * Partition's FBTSEQ value. If targeting the Active
 * Partition, the FBTSEQ value in Flash must be blank (erase default value of
 * 0xFFFFFF) and be permitted by hardware. When targeting the Inactive
 * Partition's FBTSEQ value, any value can preexist, but success is still
 * dependent on hardware restrictions.
 *
 * The value written can be a relative offset from the currently Active
 * Partition's FBTSEQ value, or any legal 12-bit absolute FBTSEQ value. The
 * proper one's complement value in the upper 12 bits will be automatically
 * computed and programmed, so need not be provided.
 *
 * If the target partition's FBTSEQ value already matches that which is being
 * requested for programming, this function returns immediately.
 *
 * If the target partition is the Inactive Partition and it already has FBTSEQ
 * programmed (to a non-matching value), then the full Flash Page contents will
 * be pushed onto the stack, erased in Flash, and then restored back into the
 * Flash page with the new FBTSEQ value, allowing any value to be programmed at
 * any time the hardware allows it.
 *
 * This function is only available/applicable on devices with Dual Partition
 * hardware and with the Bootloader project compiled for Dual Partition mode.
 *
 * NOTE:
 * The hardware treats illegal FBTSEQ values (one with an unmatching one's
 * complement in the upper 12-bits of the 24-bit FBTSEQ instruction word) as
 * equivalent to 0x000FFF, the lowest possible boot priority. In the event both
 * partitions have illegal FBTSEQ values and/or the value 0x000FFF in them, then
 * Partition 1 will always be chosen for Activation on device reset. Therefore,
 * the first meaningful value that should be programmed with this function, is
 * 0x001FFE.
 *
 * @param target
 *      Flag specifying which partition's FBTSEQ value shall be programmed.
 *      Specify:
 *      <ul>
 *          <li>0 - Write Inactive Partition's FBTSEQ</li>
 *          <li>1 - Write Active Partition's FBTSEQ</li>
 *          <li>-1 - Write Partition 1's FBTSEQ</li>
 *          <li>-2 - Write Partition 2's FBTSEQ</li>
 *      </ul>
 * @param absoluteSEQ
 *      Absolute 12-bit sequence number to write to FBTSEQ. The upper 12-bit
 *      complement is internally computed to always be valid. Specify
 *      relativeSEQ as 0x0000 when using this parameter.
 *
 *      If you wish to program a relative FBTSEQ value, specify 0x0000 for this
 *      parameter.
 *
 * @param relativeSEQ
 *      signed integer added to the NON-targeted partition's FBTSEQ value.
 *      <ul>
 *          <li>Specify 0 when using the absoluteSEQ parameter instead.</li>
 *          <li>Specify -1 to make the target Partition Active at reset</li>
 *          <li>Specify +1 to make the target partition Inactive at reset</li>
 *          <li>Other values are legal, but are unlikely to be useful for
 *              anything.</li>
 *      </ul>
 *
 *      An error code will be returned without programming (or erasing) anything
 *      if the offset would result in underflow below 0xFFF000, the highest
 *      possible priority, the result would overflow above 0x000FFF, the lowest
 *      possible boot priority.
 *
 * @return
 *  <ul>
 *      <li> 1 if the target partition was successfully programmed. Reset
 *             the device or call EZBL_ResetCPU() to have the changes take
 *             effect.</li>
 *      <li>-1 if target partition did not program successfully (blocked by
 *             hardware due to Config segment write protection, or using
 *             Protected Dual Partition mode and targeting Partition 1)</li>
 *      <li>-2 if the target computed relative FBTSEQ value would have
 *             overflowed or underflowed</li>
 *      <li>-3 target parameter value invalid</li>
 *  </ul>
 */
int EZBL_WriteFBTSEQ(int target, int absoluteSEQ, int relativeSEQ)
{
    union
    {
        unsigned long  u32;
        unsigned short u16[2];
        unsigned char  u8[3];
    } targetFBTSEQ, nonTargetFBTSEQ, newFBTSEQ, addrFBTSEQ;

    addrFBTSEQ.u32 = EZBL_WeakSYM32(_FBTSEQ);

    // Return -3 target parameter value invalid if we are called from a Single 
    // Partition project. The __FLASH_PARTITION symbol is set in all XC16 linker 
    // scripts to the __DUAL_PARTITION macro only when compiling for a dual 
    // partition mode.
    if(!((unsigned int)&_FLASH_PARTITION) || !addrFBTSEQ.u32 || (target > 1) || (target < -2))
        return -3;

    // Encode target to mean 0 == Inactive Partition, 1 = Active Partition
    if(target == -1)    // -1 == Write Partition 1, invariant of Active Partition
    {
        target = !NVMCONbits.P2ACTIV;
    }
    if(target == -2)    // -2 == Write Partition 2, invariant of Active Partition
    {
        target = NVMCONbits.P2ACTIV;
    }
    if(!target)
    {
        addrFBTSEQ.u8[2] |= 0x40u;
    }

    // Read existing FBTSEQ values
    nonTargetFBTSEQ.u32 = EZBL_ReadFlash(0x400000u ^ addrFBTSEQ.u32);
    targetFBTSEQ.u32    = EZBL_ReadFlash(addrFBTSEQ.u32);

    // Compute what the new FBTSEQ value needs to be
    newFBTSEQ.u16[0] = absoluteSEQ & 0x0FFFu;
    if(relativeSEQ)
    {
        // Check if the nonTargetFBTSEQ value we're going to compute relative to is valid
        nonTargetFBTSEQ.u16[1] = ~((nonTargetFBTSEQ.u16[1]<<4) | (nonTargetFBTSEQ.u16[0]>>12));
        nonTargetFBTSEQ.u16[1] &= 0x0FFFu;
        nonTargetFBTSEQ.u16[0] &= 0x0FFFu;
        if(nonTargetFBTSEQ.u16[1] != nonTargetFBTSEQ.u16[0])
        {
            // Not valid, set to 0x000FFF since hardware treats invalid and 0x000FFF as identical
            nonTargetFBTSEQ.u16[0] = 0x0FFFu;
        }
        newFBTSEQ.u16[0] = nonTargetFBTSEQ.u16[0] + relativeSEQ; // Add 12-bit unsigned to 16-bit signed results in overflow into bits 12-15

        // Check for overflow/underflow
        if(newFBTSEQ.u16[0]>>12)
        {
            return -2;
        }
    }
    newFBTSEQ.u16[1] = ~newFBTSEQ.u16[0];
    newFBTSEQ.u16[0] |= newFBTSEQ.u16[1]<<12;
    newFBTSEQ.u16[1] = (newFBTSEQ.u16[1]>>4) & 0x00FFu;

    EZBL_WriteROMObj(addrFBTSEQ.u32, newFBTSEQ.u8); // Perform FBTSEQ write (self handles read-modify-write if flash page has to be erased to accommodate the write)

    // Verify write result
    if(EZBL_ReadFlash(addrFBTSEQ.u32) ^ newFBTSEQ.u32)
        return -1;

    return 1;   // Done
}
