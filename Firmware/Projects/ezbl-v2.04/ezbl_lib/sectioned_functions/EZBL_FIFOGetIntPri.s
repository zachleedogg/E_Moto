;/*******************************************************************************
;  Easy Bootloader Library for PIC24 and dsPIC33
;
;  Summary:
;    Gets the IPCx<> Interrupt Priority bitfield for the specified FIFO
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
 * Returns the IPCx<y> Interrupt Priority bitfield for the given FIFO. The
 * quantities of x and y are computed based on fifo->irqNum.
 *
 * @param *fifo Pointer to the FIFO to return the interrupt priority on
 *
 * @return IPCx Interrupt Priority bits for this FIFO with all other bits
 *         sharing the register masked off. The bitfield is always LSb
 *         justified (i.e. starts in bit position 0, regardless of original
 *         position in the IPCx register).
 */
 ; unsigned int EZBL_FIFOGetIntPri(EZBL_FIFO *fifo);
	.pushsection    .text.EZBL_FIFOGetIntPri, code
	.global         _EZBL_FIFOGetIntPri
    .type           EZBL_FIFOGetIntPri, @function
    .equ            irqNumOffset, 16
    .extern         _IPC0
_EZBL_FIFOGetIntPri:    ; unsigned int EZBL_FIFOGetIntPri(EZBL_FIFO *fifo);
    mov     [w0+irqNumOffset], w2   ; w2 = irqNum
    mov     #_IPC0, w4              ; w4 = &IPC0
    lsr     w2, #1, w3
    mov.b   [w4+w3], w0             ; w0 = *(&IPC0 + irqNum/2)
    btsc    w2, #0
    lsr     w0, #4, w0
    and     w0, #0x0F, w0
    return
    .size   _EZBL_FIFOGetIntPri, . - _EZBL_FIFOGetIntPri
    .popsection
