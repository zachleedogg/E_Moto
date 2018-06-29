;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Source File with external symbol 
;  dependencies
;
;  Summary:
;    Assembly language optimized helper routines required by the EZBL core
;
;  Description:
;    Assembly language optimized helper routines required by the EZBL core
;    and optionally also callable from EZBL optional submodules.
;
;*******************************************************************************/
;
;// DOM-IGNORE-BEGIN
;/*******************************************************************************
;  Copyright (C) 2016 Microchip Technology Inc.
;
;  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
;  derivatives created by any person or entity by or on your behalf, exclusively
;  with Microchip's products.  Microchip and its licensors retain all ownership
;  and intellectual property rights in the accompanying software and in all
;  derivatives here to.
;
;  This software and any accompanying information is for suggestion only.  It
;  does not modify Microchip's standard warranty for its products.  You agree
;  that you are solely responsible for testing the software and determining its
;  suitability.  Microchip has no obligation to modify, test, certify, or
;  support the software.
;
;  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
;  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
;  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
;  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
;  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
;
;  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
;  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
;  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
;  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
;  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
;  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
;  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
;  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
;  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
;
;  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
;  TERMS.
;*******************************************************************************/
;// DOM-IGNORE-END

	.equ	SR_C, 0x0	; SR<C> is bit 0
	.equ	SR_Z, 0x1	; SR<Z> is bit 1
	.equ	SR_OV, 0x2	; SR<OV> is bit 2
	.equ	SR_N, 0x3	; SR<N> is bit 3


/**
 * DEPRECATED: Use EZBL_WriteROM() or EZBL_WriteROMEx() instead, which do
 * address checking against .text.EZBLNoProgramRanges table data instead.
 *
 * Identical to EZBL_WriteROM() except that the address range is checked
 * first for overlap with any bootloader addresses. Like EZBL_WriteROM(),
 * set EZBL_NVMKey = 0x03DF to permit the write. Any other value in
 * EZBL_NVMKey will cause all function behavior to execute, but without
 * actually writing anything to Flash or Config Words.
 *
 * If any overlap in Flash memory is detected, the overlapped region data is
 * converted to all '1's which is then skipped when Flash programming to
 * preserve all bootloader memory region contents.
 *
 * If any overlap in Non-volatile Config Word memory forced by the bootloader is
 * detected (addresses in 0xF8xxxx range), the overlapped region data is
 * converted to match the values coded in the bootloader so as not to allow the
 * bootloader specified values to change. This is done instead of all '1's
 * masking since all '1's is a valid Configuration Word value and can't be
 * assumed to already exist due to a prior erase operation. I.e. writing all
 * '1's are not skipped during programming.
 *
 * @param destProgAddr 24-bit unsigned long program memory address to start
 *                     writing to. The LSbit must always be '0'. When
 *                     programming flash memory with a native 48-bit flash word
 *                     size, the second LSbit must also be '0' to align to the
 *                     needed 0x4 boundary.
 *
 * @param *srcData Pointer to source data in RAM which should be copied to Flash
 *                 or Config Words. This data should be packed in 24-bit chunks
 *                 without phantom bytes. srcData must be writable. The data can
 *                 change while masking off overlapped bootloader regions. This
 *                 pointer does not have any alignment requirements.
 *
 * @param byteCount unsigned integer specifying how many bytes to sequentially
 *                  copy from *srcData to the Program Memory address range. This
 *                  value need not result in write termination on a perfect
 *                  flash word or Config Word address boundary. Automatic 0xFF
 *                  padding will be done to fill the word before programming.
 *
 * @return 24-bit unsigned long program memory address after the just programmed
 *         Flash Word or Config Word. *srcData RAM contents may also have
 *         changed according to the bootloader masking behavior.
 */
 ; unsigned long EZBL_WriteROMChecked(unsigned long destProgAddr, void *srcData, unsigned int byteCount);
    .pushsection    .text.EZBL_WriteROMChecked, code
    .global         _EZBL_WriteROMChecked
    .type           _EZBL_WriteROMChecked, @function
    .extern         _EZBL_MaskBootloaderRegions
    .extern         _EZBL_WriteROM
    .extern         _EZBL_NVMKey
    .weak           __FBOOT
    reset       ; Guard against possible function entry from PC accidentally falling into executing this function (ex: operating outside Vdd/Clock speed allowable region and device is mis-executing occasional instructions)
_EZBL_WriteROMChecked:
    ; Check if we are targetting an inactive partition address, and if so, skip 
    ; all bootloader region masking. We should be able to write to the entire 
    ; Inactive Partition
    mov         #packed_hi(__FBOOT), w4     ; Check if __FBOOT symbol is defined (will always be at address >0x800000 if it exists; weak attribution causes 0x00000000 to be returned when not present)
    cp0         w4
    bra         Z, 1f                       ; No __FBOOT symbol on this device, so can't have an Inactive Partition
    btsc        w1, #6                      ; if(destFlashAddress & 0x00400000)  // Check if we we are targetting the inactive partition
    goto        _EZBL_WriteROM              ; Yes, so skip all address/data masking for bootloader regions
    
    ; Mask off overlapped regions and pass to write routine
1:  push        _EZBL_NVMKey    ; Going to do something that could takes a lot of time, so save unlock key to temporary place instead
    clr         _EZBL_NVMKey
    push.d      w0
    push.d      w2
    call        _EZBL_MaskBootloaderRegions
    pop.d       w2
    pop.d       w0
    pop         _EZBL_NVMKey
    clr         [w15]           ; Dummy write to destroy copy of NVMKey value on the stack
    goto        _EZBL_WriteROM
    .size       _EZBL_WriteROMChecked, . - _EZBL_WriteROMChecked + 2
    .popsection
    