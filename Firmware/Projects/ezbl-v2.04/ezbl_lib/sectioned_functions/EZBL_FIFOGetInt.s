;/*******************************************************************************
;  Easy Bootloader Library for PIC24 and dsPIC33
;
;  Summary:
;    Implements the EZBL_FIFOGetIntEn() and EZBL_FIFOGetInt() functions
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
 * Returns the IECx<y> Interrupt Enable bit for the given FIFO. The quantities
 * of x and y are computed based on fifo->irqNum.
 *
 * @param *fifo Pointer to the FIFO to return the Interrupt Enable status on.
 *
 * @return IECx Interrupt Enable state, not taking into account global
 *         interrupt enable/disable states, such as a DISI instruction or
 *         GIE == 0 condition.
 *          <ul>
 *              <li>0 = Interrupt Disabled</li>
 *              <li>1 = Interrupt Enabled</li>
 *          </ul>
 */
; int EZBL_FIFOGetIntEn(EZBL_FIFO *fifo);


/**
 * Returns the IFSx<y> Interrupt Flag bit for the given FIFO. The quantities of
 * x and y are computed based on fifo->irqNum.
 *
 * @param *fifo Pointer to the FIFO to return the flag status on.
 *
 * @return  <ul>
 *              <li>0 = No interrupt pending</li>
 *              <li>1 = Interrupt pending</li>
 *          </ul>
 */
; int EZBL_FIFOGetInt(EZBL_FIFO *fifo);


	.pushsection    .text.EZBL_FIFOGetIntEn, code
	.global         _EZBL_FIFOGetIntEn
    .global         _EZBL_FIFOGetInt
    .type           _EZBL_FIFOGetIntEn, @function
    .type           _EZBL_FIFOGetInt, @function
    .equ            irqNumOffset, 16
    .extern         _IFS0
    .extern         _IEC0
_EZBL_FIFOGetIntEn:    ; int EZBL_FIFOGetIntEn(EZBL_FIFO *fifo);
    mov     #_IEC0, w1              ; w1 = &_IEC0
    skip_next_instr
_EZBL_FIFOGetInt:       ; int EZBL_FIFOGetInt(EZBL_FIFO *fifo);
    mov     #_IFS0, w1              ; w1 = &_IFS0
    mov     [w0+irqNumOffset], w2   ; w2 = irqNum
    lsr     w2, #3, w3
    and     w2, #0x7, w2
    mov.b   [w1+w3], w0
    lsr     w0, w2, w0
    and     w0, #0x1, w0
    return
    .size   _EZBL_FIFOGetIntEn, . - _EZBL_FIFOGetIntEn
    .size   _EZBL_FIFOGetInt, . - _EZBL_FIFOGetIntEn
    .popsection
