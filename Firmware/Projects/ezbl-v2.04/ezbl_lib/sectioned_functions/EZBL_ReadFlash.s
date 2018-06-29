;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Source File
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

    .equ    SR_C, 0x0   ; SR<C> is bit 0
    .equ    SR_Z, 0x1   ; SR<Z> is bit 1
    .equ    SR_OV, 0x2  ; SR<OV> is bit 2
    .equ    SR_N, 0x3   ; SR<N> is bit 3


/**
 * Reads one instruction word (24 bits) from Flash at the specified Flash
 * program memory address. The returned uppermost 8 bits are always 0x00.
 *
 * Besides ordinary program memory locations, this function can be used to read
 * from Auxiliary Flash, Configuration fuse bytes, and other debug or
 * programming executive memory spaces. On applicable devices programmed for
 * multiple Flash partitions, this function can also be used to read from the
 * inactive Flash partition (starting at address 0x400000), but only when the
 * inactive Flash partition is idle (i.e. not undergoing an erase or programming
 * operation). If the inactive partition is busy with an erase or programming
 * operation, the return results will be indeterminate.
 *
 * Care should be taken to ensure that the target address is legal for the
 * device. Attempting to read from unimplemented memory locations will, in most
 * cases, result in an Address Error Trap rather than returning 0x000000 data.
 * Similarly, on devices that implement segmented Flash Code Protection
 * security, attempting to read from a Flash region that is dissallowed will
 * trigger a device reset.
 *
 * This function must temporarily change the TBLPAG register contents in order 
 * to issue the read. The TBLPAG contents are restored upon return.
 *
 * @param address   unsigned 32-bit long integer of the address to read from
 *                  (will actually be 24-bits max and the LSbit must be set to
 *                  0). The EZBL_SYM32() macro can be used if you wish to
 *                  discover the address of a link time symbol.
 *
 * @return  unsigned 32-bit long integer data read from the given program memory
 *          address. Since program data is always 24-bits wide, the upper 8 bits
 *          will always be 0x00.
 */
; unsigned long EZBL_ReadFlash(unsigned long address);
; unsigned long EZBL_ReadFlashInstruction(unsigned long address);   // Historical longer name for EZBL_ReadFlash(). This is now just an alias to EZBL_ReadFlash().
    .pushsection    .text.EZBL_ReadFlash, code, align(0x4)  ; align needed to ensure no back-to-back psv errata
    .global         _EZBL_ReadFlash
    .global         _EZBL_ReadFlashInstruction
    .type           _EZBL_ReadFlash, @function
    .type           _EZBL_ReadFlashInstruction, @function
_EZBL_ReadFlash:
_EZBL_ReadFlashInstruction:
    mov         TBLPAG, w2
    mov	        w1, TBLPAG
0:  btsc        NVMCON, #15 ; Ensure no asynchronous Flash erase/write operations are in progress against the same Flash target. You cannot read from a Flash memory during programming and the CPU will not stall for completion unless you actually branch to it..
    bra         0b
    tblrdh      [w0], w1
    tblrdl      [w0], w0
    mov	        w2, TBLPAG
    return
    .size   _EZBL_ReadFlash, . - _EZBL_ReadFlash
    .size   _EZBL_ReadFlashInstruction, . - _EZBL_ReadFlashInstruction
    .popsection
    