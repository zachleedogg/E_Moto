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
;  Copyright (C) 2017 Microchip Technology Inc.
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
 * void EZBL_EraseInactivePartition(void);
 *
 * Starts erasing the entire inactive partition and then returns without waiting
 * for completion. NVMCON<WREN> remains set once the erase is done.
 *
 * Unlike most NVM functions, you do not have to set EZBL_NVMKey to any
 * particular value to allow the erase. You don't have to set it externally since
 * erasing the inactive partition won't normally brick your device. You'll still
 * have what you're currently running, so less pairanoia is waranted. Traditional
 * single-boot applications shouldn't call this function, so the linker will be
 * able to remove this code and not store the NVM unlock sequence so close to the
 * NVM unlock operation.
 *
 * Upon function return, EZBL_NVMKey will be cleared to 0x0000.
 *
 * To check if the erase is done, check NVMCON<WR>. Zero means the erase is done.
 *
 * If this function is called while a prior NVM Flash erase or write operation is
 * already in progress, the function blocks until the prior operation completes.
 * The erase operation is then started before returning.
 */
    .pushsection    .text.EZBL_EraseInactivePartition, code
    .global         _EZBL_EraseInactivePartition
    .type           _EZBL_EraseInactivePartition, @function
    .extern         _EZBL_NVMKey
    .extern         _EZBL_WriteNVMCON
    reset       ; Guard against possible function entry from Program Counter accidentally falling into executing this function (ex: operating outside Vdd/Clock speed allowable region and device is mis-executing occassional instructions)
_EZBL_EraseInactivePartition:
    mov         #0x4004, w0         ; Set WREN; Set NVMOP = 0x4: Inactive Partition Erase
    clr         w2                  ; blockUntilDone = 0: Do not block but do restore interrupts as soon as possible
    mov         #0x03E5, w1         ; Set unlockKeyMultiplier = 0x03E5 as required
    mov         #0x03DF, w3
    mov         w3, _EZBL_NVMKey    ; Load the EZBL_NVMKey here
    goto        _EZBL_WriteNVMCON
    
    .size       _EZBL_EraseInactivePartition, . - _EZBL_EraseInactivePartition + 2  ; +2 for reset opcode before the label
    .popsection
