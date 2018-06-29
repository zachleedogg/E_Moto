;/*******************************************************************************
;  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM source file (ezbl_lib)
;
;  Summary:
;    Implements EZBL_MapInv() and EZBL_MapInvEx() APIs. These are used by
;    LEDToggle() and possibly other APIs to read/write ordered bit maps in
;    arbitrary SFR locations.
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
 * Inverts (toggles) the state of an array of mapped SFR<bit> destinations using
 * a logical XOR of the invertMask provided. I.e. only '1' bits toggle a mapped
 * SFR state. '0' bits have no effect on the corresponding SFR<bit> value.
 *
 * @param invertMask A bitmask representing an array of ordered bits who's state
 *                   should be toggled. Each '1' bit in the bit field toggles
 *                   the corresponding bit in the arbitrary SFR map. Unmapped
 *                   high order bits are ignored.
 *
 *                   Specify a invertMask of 0x0 to read the bit map without
 *                   writing/altering existing state.
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
 * @param sfrAddrOffset Signed integer to add to each mappingArray SFR address.
 *                      Use this field to access related SFRs for the bitmap at
 *                      a known address offset while storing only one mapping
 *                      array for the hardware.
 *
 *                      For example, if the mappingArray was created against
 *                      LATx registers, and the TRISx register needs to be
 *                      read/toggled, specify sfrAddrOffset as:
 *                          (unsigned int)&TRISA - (unsigned int)&LATA
 *                      Note that TRISA comes before LATA on most device memory
 *                      maps, so the value computed will actually be a negative
 *                      number.
 *
 *                      Ensure the integral (unsigned int) cast is maintained
 *                      when referencing SFR pointers. Subtracting two pointers
 *                      directly yields a scaled quantity relative to the size
 *                      of the pointed to objects, which is not the address
 *                      offset.
 *
 * @return Bitmask representing the mapped SFR bit values read before issuing
 *         any toggle write backs. Higher order bits that are not specified in
 *         the mapping array are returned as '0'.
 */
; unsigned int EZBL_MapInv(unsigned int invertMask, const unsigned int *mappingArray);
; unsigned int EZBL_MapInvEx(unsigned int invertMask, const unsigned int *mappingArray, int sfrAddrOffset);
    .pushsection .text.EZBL_MapInv, code
    .global _EZBL_MapInv
    .type   _EZBL_MapInv, @function
    .global _EZBL_MapInvEx
    .type   _EZBL_MapInvEx, @function
_EZBL_MapInv:    ; unsigned int EZBL_MapInv(unsigned int invertMask, const unsigned int *mappingArray);
    clr     w2

_EZBL_MapInvEx:  ; unsigned int EZBL_MapInvEx(unsigned int invertMask, const unsigned int *mappingArray, int sfrAddrOffset);
    mul.uu  w4, #0, w4                          ; w4 = 0x0000 for register read back values, w5 = 0 for bit counter
    cp0     w1
    bra     Z, 9f
    dec     [w1++], [w15++]                     ; TOS = mappingArray item count - 1, and set SR bits
    bra     N, 8f
    mov     #0x0FFF, w3                         ; w3 = 0x0FFF, for masking off the bit number and just returning the register address

    ; Read the current SFR<bit> map states for return and toggle SFR<bit> bits which have a '1' stored in the invertMask parameter. Works according to the address ascending storage order in the mappingArray.
1:  mov     [w1++], w6                          ; Read encoded address and bit number for the SFR<bit> mapping. Bits <15:12> are the bit number, while bits <11:0> are the address of the SFR register.
    lsr     w6, #12, w7                         ; w7 = Mapped bit number
    and     w6, w3, w6                          ; w6 = Mapped register address
    add     w6, w2, w6
    btst.z  w0, w5                              ; !Z = Toggle will be need
    btst.c  [w6], w7                            ; SR<C> = SFR<curBit>
    bsw.c   w4, w5                              ; w0 = SFR<bit> states captured (before any toggle) for return
    btg     SR, #SR_C                           ; Generate toggled value in SR<C>
    btss    SR, #SR_Z
    bsw.c   [w6], w7                            ; Toggle was needed, so write back inverted value
    inc     w5, w5                              ; Next bit towards MSb
    dec     [--w15], [w15++]                    ; Decrement and test loop counter on TOS
    bra     NN, 1b

8:  dec2    w15, w15                            ; Remove loop counter variable from stack
9:  mov     w4, w0                              ; Return bits read before toggling
    return

    .size _EZBL_MapInv, . - _EZBL_MapInv
    .size _EZBL_MapInvEx, . - _EZBL_MapInvEx
    .popsection
