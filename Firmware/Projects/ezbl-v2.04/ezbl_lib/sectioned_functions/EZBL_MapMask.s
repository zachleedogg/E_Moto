;/*******************************************************************************
;  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM source file (ezbl_lib)
;
;  Summary:
;    Returns the (unsigned int) integral bitmask form of the map structure with
;    '1' LSbits indicating the bit is implemented in the map and '0' high order
;    bits indicating the map contains no mapping for the bit position.
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
    .equ    SR_RA, 0x4  ; SR<RA> is bit 4
    .macro  skip_next_instr
    btsc    w15, #0     ; Skips next instruction since w15 stack pointer can never be odd
    .endm


/**
 * Returns the (unsigned int) integral bitmask form of the map structure with
 * '1' LSbits indicating the bit is implemented in the map and '0' high order
 * bits indicating the map contains no mapping for the bit position.
 *
 * The none of the SFRs targetted by the map are read or written, only the
 * mappingArray contents are checked.
 *
 * @param mappingArray Pointer to a (unsigned int) array. Index 0 must contain
 *                     a count of how many bit mappings follow in the array.
 *
 *                     Index 1 up to 16 shall encode SFR addresses and bit
 *                     positions to map to. Bits <15:12> encode the bit position
 *                     while bits <11:0> encode the SFR target address that the
 *                     bit applies to.
 *
 *                     Index 1 corresponds to bit 0 (LSbit) of the invertMask,
 *                     index 2, is bit 1, etc.
 *
 *
 * @return Bitmask representing the bits within the map that have corresponding 
 *         SFR targets defined in the map structure. Unimplemented bits in the
 *         map return '0'.
 */
; unsigned int EZBL_MapMask(const unsigned int *mappingArray);
    .pushsection .text.EZBL_MapMask, code
    .global _EZBL_MapMask
    .type   _EZBL_MapMask, @function
_EZBL_MapMask:    ; unsigned int EZBL_MapMask(const unsigned int *mappingArray);
    mov     [w0], w1
    mov     #0x0001, w0
    sl      w0, w1, w0
    dec     w0, w0
    return
    .size _EZBL_MapMask, . - _EZBL_MapMask
    .popsection
