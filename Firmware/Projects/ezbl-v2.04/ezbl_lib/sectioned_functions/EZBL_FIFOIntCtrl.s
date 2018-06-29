;/*******************************************************************************
;  Easy Bootloader Library for PIC24 and dsPIC33
;
;  Summary:
;    Implements the EZBL_FIFOClearInt() and EZBL_FIFODisableInt() functions
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

	.equ	SR_C, 0x0	; SR<C> is bit 0
	.equ	SR_Z, 0x1	; SR<Z> is bit 1
	.equ	SR_OV, 0x2	; SR<OV> is bit 2
	.equ	SR_N, 0x3	; SR<N> is bit 3
    .macro  skip_next_instr
    btsc    w15, #0     ; Skips next instruction since w15 stack pointer can never be odd
    .endm


/**
 * Clears the IECx<y> Interrupt Enable bit for the given FIFO. The quantities of
 * x and y are computed based on fifo->irqNum.
 *  
 * IECx register bit change is done atomically.
 *
 * @param *fifo Pointer to the FIFO to disable the interrupt on. If null,
 *              function will always return 0 without doing anything.
 *
 * @return Prior IECx Interrupt Enable state, not taking into account global
 *         interrupt enable/disable states, such as a DISI instruction or
 *         GIE == 0 condition.
 *          <ul>
 *              <li>0 = Interrupt Enable bit was previously clear</li>
 *              <li>1 = Interrupt Enable bit was previously set</li>
 *          </ul>
 */
; int EZBL_FIFOIntDisable(EZBL_FIFO *fifo);


/**
 * Clears the IFSx<y> Interrupt Flag bit for the given FIFO. The quantities of
 * x and y are computed based on fifo->irqNum.
 *
 * IFSx register bit change is done atomically.
 *
 * @param *fifo Pointer to the FIFO to clear the interrupt flag on. If null,
 *              function will always return 0 without doing anything.
 *
 * @return Prior IFSx Interrupt Flag state
 *          <ul>
 *              <li>0 = No interrupt was pending</li>
 *              <li>1 = Interrupt was pending</li>
 *          </ul>
 */
; int EZBL_FIFOIntClear(EZBL_FIFO *fifo);

    .pushsection    .text.EZBL_FIFOIntCtrl, code
    .global         _EZBL_FIFOIntEnable
	.global         _EZBL_FIFOIntDisable
	.global         _EZBL_FIFOIntEnableSet
	.global         _EZBL_FIFOIntRaise
    .global         _EZBL_FIFOIntClear
    .global         _EZBL_FIFOIntFlagSet
    .type           _EZBL_FIFOIntEnable, @function
    .type           _EZBL_FIFOIntDisable, @function
    .type           _EZBL_FIFOIntEnableSet, @function
    .type           _EZBL_FIFOIntRaise, @function
    .type           _EZBL_FIFOIntClear, @function
    .type           _EZBL_FIFOIntFlagSet, @function
    .equ            irqNumOffset, 16    ; Bit offset to irqNum in EZBL_FIFO structure
    .extern         _IEC0
    .extern         _IFS0
_EZBL_FIFOIntEnable:    ; int EZBL_FIFOIntEnable(EZBL_FIFO *fifo);
    mov     #1, w1      ;     return EZBL_FIFOIntEnableSet(fifo, 1);
    skip_next_instr
_EZBL_FIFOIntDisable:   ; int EZBL_FIFOIntDisable(EZBL_FIFO *fifo);
    mov     #0, w1      ;     return EZBL_FIFOIntEnableSet(fifo, 0);
_EZBL_FIFOIntEnableSet: ; int EZBL_FIFOIntEnableSet(EZBL_FIFO *fifo, int enable);
    mov     #_IEC0, w2
    bra     1f
_EZBL_FIFOIntRaise:     ; int EZBL_FIFOIntRaise(EZBL_FIFO *fifo);
    mov     #1, w1      ;     return EZBL_FIFOIntFlagSet(fifo, 1);
    skip_next_instr
_EZBL_FIFOIntClear:     ; int EZBL_FIFOIntClear(EZBL_FIFO *fifo);
    mov     #0, w1      ;     return EZBL_FIFOIntFlagSet(fifo, 0);
_EZBL_FIFOIntFlagSet:   ; int EZBL_FIFOIntFlagSet(EZBL_FIFO *fifo, int flag);
    cp0     w0
    btsc    SR, #SR_Z
    retlw   #0, w0
    mov     #_IFS0, w2              ; w2 = &IFS0
1:  mov     [w0+irqNumOffset], w3   ; w3 = irqNum
    lsr     w3, #8, w0              ; Abort write if irqNum is > 255 or negative (illegal) IRQ number; Also sets w0 = 0x0000 for later bit shif
    btss    SR, #SR_Z
    retlw   #0, w0

    lsr     w3, #3, w4              ; w4 = irqNum/8 (IRQs per byte) = Byte address offset from IFS0/IEC0 to proper interrupt bit
    bclr    w4, #0                  ; Clear LSbit - we want a word address
    add     w2, w4, w2              ; w2 = &IFSx, also SR<C> = 0
    and     w3, #0xF, w3            ; w3 = bit number y
    btst.c  [w2], w3                ; Read SR<C> = IF/IE value before modification
    rlc     w0, w0                  ; Save in w0 for return. w0 was 0x0000 based on earlier abort test output
    add     w1, #0, w1              ; SR<Z> = !IF/!IE
    bsw.z   [w2], w3                ; Atomic read-modify-write bit clear/set. New value is !Z.
    return

    .size   _EZBL_FIFOIntEnable, . - _EZBL_FIFOIntEnable
	.size   _EZBL_FIFOIntDisable, . - _EZBL_FIFOIntEnable
	.size   _EZBL_FIFOIntEnableSet, . - _EZBL_FIFOIntEnable
	.size   _EZBL_FIFOIntRaise, . - _EZBL_FIFOIntEnable
    .size   _EZBL_FIFOIntClear, . - _EZBL_FIFOIntEnable
    .size   _EZBL_FIFOIntFlagSet, . - _EZBL_FIFOIntEnable
    .popsection
