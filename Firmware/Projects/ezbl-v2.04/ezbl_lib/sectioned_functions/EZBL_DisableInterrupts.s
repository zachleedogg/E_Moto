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

	.equ	SR_C, 0x0	; SR<C> is bit 0
	.equ	SR_Z, 0x1	; SR<Z> is bit 1
	.equ	SR_OV, 0x2	; SR<OV> is bit 2
	.equ	SR_N, 0x3	; SR<N> is bit 3


/**
 * unsigned int EZBL_DisableInterrupts(void);
 *
 * Disables all interrupts, including IPL7 ones by setting the current CPU 
 * execution level to IPL8 (IPL8 is allocated to the lowest priority Generic 
 * Soft Error Trap).
 *
 * The return value should be saved and subsequently passed to 
 * EZBL_RestoreInterrupts() to restore the original CPU IPL and interrupt 
 * processing.
 *
 * This function differs from traditional methods of raising the CPU to IPL7 or
 * setting the INTCON2<GIE> bit to disable Interrupts because:
 *  - PIC24F/dsPIC33F/PIC24H devices don't have a GIE bit
 *  - Newer devices, like the dsPIC33EP64GS506 family, include W-REG array 
 *    shadow sets which can be automatically invoked by setting the IPL to the 
 *    preconfigured shadow context's activation level.
 * As IPL7 is a useful place to assign a shadow register context set and since
 * universal portability is desired, this function triggers a higher IPL8 
 * state that cannot have a shadow register set assigned to it. Execution will 
 * continue using the same context registers as were selected during the 
 * EZBL_DisableInterrupts() call.
 *    
 * As IPL8 is allocated as the Generic Soft Trap, any DO stack overflows, 
 * Illegal opcodes executed, or other mupltiplexed traps implemented in INTCON3 
 * will be queued but supressed after EZBL_DisableInterrupts() returns. When 
 * EZBL_RestoreInterrupts() is called, any accumulated true Generic Soft Trap 
 * events will trigger vectoring to the Generic Soft Trap interrupt handler.
 *
 * All other traps will continue to vector normally while interrupts are 
 * disabled. 
 *
 * This function executes in 13 cycles (dsPIC33E/PIC24E/dsPIC33C devices) or
 * 8 cycles (dsPIC30F/dsPIC33F/PIC24H/PIC24F devices), including the call and 
 * return.
 *
 * @return Prior SR state (with IPL information) to be used by 
 *         EZBL_RestoreInterrupts(). 
 *         
 *         This function only uses w0. When calling from assembly, it is not 
 *         necessary to preserve the w1-w7 registers.
 */
    .pushsection	.text.EZBL_DisableInterrupts, code
    .global         _EZBL_DisableInterrupts
    .type           _EZBL_DisableInterrupts, @function
    .extern         SR
_EZBL_DisableInterrupts:
    mov	    #0x0080, w0             ; Bit 7 of the high word of an Interrupt frame is the IPL3 bit (CORCON<3>); bits <15:8> are the low byte of SR
    ior     w0, [--w15], [w15++]
    mov     SR, w0
    retfie                          ; Fake an interrupt return to atomically clear SR<IPL2:0> and set CORCON<IPL3>
    .size           _EZBL_DisableInterrupts, . - _EZBL_DisableInterrupts
    .popsection
