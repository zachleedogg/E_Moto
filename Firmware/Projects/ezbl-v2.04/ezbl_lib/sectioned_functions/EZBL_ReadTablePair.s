;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Library Source File (ezbl_lib)
;
;  Summary:
;    Assembly language optimized EZBL APIs
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
 * Reads two consecutive instructions words (24-bits each + 8-bits of filler on
 * 16-bit devices, 64-bits total) from program space and increments a read
 * address pointer by 0x4 (for 16-bit devices) or 0x8 (32-bit devices).
 *
 * TBLPAG on 16-bit devices is internally saved, used and restored before return.
 *
 * Function blocks if an outstanding NVMCON<WR> operation is underway (16-bit
 * dual partition devices).
 *
 * The read address range can straddle 64K TBLPAG boundaries.
 *
 * @param *pgmReadAddr Pointer to a RAM variable containing the address which
 *                     will be consecutively read from flash memory. Note: this
 *                     is effectively a RAM pointer to a flash pointer and the
 *                     function will return with *pgmReadAddr (the RAM pointer)
 *                     incremented.
 *
 * @return  2 x 32-bits of aligned data read from flash. The high bytes of both
 *          32-bit instruction words on 16-bit platforms are returned as 0x00.
 *
 *          *pgmReadAddr is incremented by 0x4 or 0x8, as appropriate for the
 *          device architecture.
 */
; unsigned long long EZBL_ReadTablePair(unsigned long *pgmReadAddr);
    .pushsection    .text.EZBL_ReadTablePair, code, align(0x4)
    .global         _EZBL_ReadTablePair
    .type           _EZBL_ReadTablePair, @function
    .extern         TBLPAG
    .extern         NVMCON
_EZBL_ReadTablePair:            ; unsigned long long EZBL_ReadTablePair(unsigned long *pgmReadAddr);
    mov         TBLPAG, w7
    mov         [w0++], w4      ; w4 = tablePgmAddr<15:0>
    mov         [w0--], w5      ; w5 = tablePgmAddr<23:16>
    nop                         ; Needed for back-to-back psv errata
    add         w4, #4, [w0++]  ; *pgmReadAddr += 0x4
    addc        w5, #0, [w0]
    mov         w5, TBLPAG
    inc2        w4, w6
0:  btsc        NVMCON, #15     ; Ensure no asynchronous Flash erase/write operations are in progress against the same Flash target. You cannot read from a Flash memory during programming and the CPU will not stall for completion unless you actually branch to it..
    bra         0b
    tblrdl      [w4], w0
    tblrdh      [w4], w1
    btsc        SR, #SR_C
    inc         TBLPAG
    tblrdl      [w6], w2
    tblrdh      [w6], w3
    mov	        w7, TBLPAG
    return
    .size   _EZBL_ReadTablePair, . - _EZBL_ReadTablePair
    .popsection
