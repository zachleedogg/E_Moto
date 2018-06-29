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
 * Identical to EZBL_WriteROMRow() except that the address range is checked
 * first for overlap with any bootloader address. If any overlap is detected, 
 * the overlapped region data is converted to all '1's which is then skipped 
 * when Flash programming. srcData must be in RAM and must be writable. The data 
 * can change.
 */
; unsigned long EZBL_WriteROMRowChecked(unsigned long destFlashAddress, void *srcData, unsigned int dataByteCount);
    .pushsection    .text.EZBL_WriteROMRowChecked, code
    .global         _EZBL_WriteROMRowChecked
    .type           _EZBL_WriteROMRowChecked, @function
    .extern         _EZBL_MaskBootloaderRegions
    .extern         _EZBL_NVMKey
    .extern         _EZBL_WriteROMRow
    reset           ; Guard against possible function entry from PC accidentally falling into executing this function (ex: operating outside Vdd/Clock speed allowable region and device is mis-executing occasional instructions)
_EZBL_WriteROMRowChecked:
    ; Mask off overlapped regions and pass to write routine
    push        _EZBL_NVMKey    ; Going to do something that could takes a lot of time, so save unlock key to temporary place instead
    clr         _EZBL_NVMKey
    push.d      w0
    push.d      w2
    call        _EZBL_MaskBootloaderRegions
    pop.d       w2
    pop.d       w0
    pop         _EZBL_NVMKey
    clr         [w15]           ; Dummy write to destroy copy of NVMKey value on the stack
    goto        _EZBL_WriteROMRow
    .size       _EZBL_WriteROMRowChecked, . - _EZBL_WriteROMRowChecked + 2
    .popsection

	.end

