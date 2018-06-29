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
 * Calls an Interrupt Service Routine at the same IPL as currently set. The ISR
 * function address is obtained by looking up the IRQ number in the IVT (or
 * AIVT if applicable and enabled), so the function called may not be valid or
 * be directed to a generic _DefaultInterrupt() function if unimplemented.
 *
 * This function sets up a fake ISR call frame on the stack such that when the
 * called ISR executes the RETFIE instruction, execution will return to the
 * instruction after the call instruction that called EZBL_CallIRQ() and retain
 * the same IPL as you called EZBL_CallIRQ() at.
 *
 * This function can be used to call ISRs of lower or higher priority than the
 * current CPU IPL. However, be very careful when doing this as the called ISR
 * can become self-reentrant if the ISR is enabled. Higher priority ISRs will
 * become self-reentrant if the hardware interrupt flag becomes set. Lower
 * priority ISRs will become self-reentrant if the ISR was already active when
 * this call to EZBL_CallIRQ() is made.
 *
 * @param interruptIRQ IRQ number of the ISR function to call. IRQ numbers
 *                     correspond to the bit position for the interrupt flag in
 *                     IFSx registers, or interrupt enable bits in IECx
 *                     registers.
 *
 *                     For example, on a dsPIC33EPXXGS50X family device, the
 *                     data sheet Interrupt Controller chapter shows IRQ 0 as
 *                     being the INT0 interrupt. It has IFS0<0>/IEC0<0> bits
 *                     dedicated to it. INT4 has IRQ 54 and is in
 *                     IFS3<6>/IEC3<6>: 3*16 + 6 = 54. '3' is the register
 *                     number, 16 is the bit width of a IFS/IEC register, and 6
 *                     is the bit position within the register.
 *
 *                     To call a trap, specify a negative IRQ number,
 *                     corresponding to the IVT ordering. Ex:
 *                     <ul>
 *                          <li>-1 = Reserved Trap (7)</li>
 *                          <li>-2 = Generic Soft Trap (6)</li>
 *                          <li>-3 = Reserved Trap (5)</li>
 *                          <li>-4 = Math Error Trap (4)</li>
 *                          <li>-5 = Stack Error Trap (3)</li>
 *                          <li>-6 = Generic Hard Trap (2)</li>
 *                          <li>-7 = Address Error Trap (1)</li>
 *                          <li>-8 = Oscillator Fail Trap (0)</li>
 *                     </ul>
 */
; void EZBL_CallIRQ(int interruptIRQ);
    .pushsection    .text.EZBL_CallIRQ, code
    .global         _EZBL_CallIRQ
    .type           _EZBL_CallIRQ, @function
    .extern         SR
    .extern         CORCON
    .extern         _INTCON2
    .extern         _EZBL_ReadFlash
    .weak           __AIVT_BASE
    .weak           __FSEC
    .weak           __FBSLIM
    .weak           _EZBL_ADDRESSES_PER_SECTOR
_EZBL_CallIRQ:  ; void EZBL_CallIRQ(int interruptIRQ);
    lsr     w0, #8, w1      ; Do nothing if interruptIRQ is negative or >255 (illegal IRQ number)
    btsc    SR, #SR_Z
    return

    ; Set up a fake interrupt stack frame to maintain the current IPL upon ISR return and branch directly back to the caller of this EZBL_CallIRQ() function.
    mov     SR, w2          ; Get current IPL
    sl      w2, #8, w2      ; Move existing SR<7:5> IPL0-IPL2 over to Interrupt_Return_Address<31:29>, which are the return IPL low 3 bits
    btsc    CORCON, #3      ; Copy CORCON<IPL3> into Interrupt_Return_Address<23> where it lives between fields
    bset    w2, #7
    ior     w2, [--w15], [w15++]

    ; Convert IRQ number to IVT entry address and get vector contents
    mul.uu  w0, #2, w0      ; w1:w0 = IRQ num * 2 (clears w1 high address bits)
    add     #0x14, w0
.ifdef ezbl_lib16
    mov     #(__AIVT_BASE-0x4), w2
    btsc    _INTCON2, #15   ; INTCON2<15> is ALTIVT bit position (PIC24H/dsPIC33F/older PIC24F); dsPIC33E/PIC24E/newer PIC24F have GIE bit here
    add     w0, w2, w0
.else
.ifdef ezbl_lib16da
    mov     #(__AIVT_BASE-0x4), w2
    cp0     w2
    bra     N, 1f           ; Newer PIC24F with GIE bit in INTCON2<15>, must use dsPIC33E/PIC24E/dsPIC33C method
    btsc    _INTCON2, #15   ; INTCON2<15> is ALTIVT bit position (PIC24H/dsPIC33F/older PIC24F); dsPIC33E/PIC24E/newer PIC24F have GIE bit here
    add     w0, w2, w0
    bra     2f
.endif
1:  ; Newer device with AIVTEN Config bit tied to Boot Segment and flash page size
    btss    _INTCON2, #8    ; INTCON2<8> is AIVTEN bit position (newer PIC24F/newer dsPIC33E with AIVT)
    bra     2f

    mov     #packed_lo(__FBSLIM), w2
    mov     #packed_hi(__FBSLIM), w1
    ior     w2, w1, w3
    bra     Z, 2f                       ; Older dsPIC33E and PIC24E don't have an AIVT, so won't have an __FBSLIM either
    push    w0
    mov     #packed_lo(__FBSLIM), w0
    call    _EZBL_ReadFlash
    com     w0, [w15++]                 ; TOS = (~FBSLIM)
    mov     #packed_lo(__FSEC), w0
    mov     #packed_hi(__FSEC), w1
    call    _EZBL_ReadFlash
    mov     w0, w2
    dec     [--w15], w3                 ; w3 = (~FBSLIM) - 1
    pop     w0                          ; w1:w0 = original IVT vector address
    clr     w1
    btss    w2, #15 ; FSEC<15> == FSEC<AIVTDIS>
    btsc    w2, #3  ; FSEC<3> == FSEC<BSEN> (inverted sense)
    bra     2f
    mov     #0x1FFF, w4
    and     w3, w4, w3          ; w3 = (~FBSLIM - 1) & 0x1FFF
    mov     #_EZBL_ADDRESSES_PER_SECTOR, w4
    mul.uu  w4, w3, w2
    add     w0, w2, w0
    addc    w1, w3, w1
.endif

2:  call    _EZBL_ReadFlash     ; Read branch address for IVT[IRQ] or AIVT[IRQ]
    push.d  w0                  ; Branch to the ISR
    return
    .size           _EZBL_CallIRQ, . - _EZBL_CallIRQ
    .popsection
