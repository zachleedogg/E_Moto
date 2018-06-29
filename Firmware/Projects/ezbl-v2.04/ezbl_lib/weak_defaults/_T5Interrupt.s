;/*******************************************************************************
;  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM source file (ezbl_lib)
;
;  Summary:
;    Implements the _T5Interrupt() ISR for the NOW timing APIs
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
    .macro  skip_next_instr
    btsc    w15, #0     ; Skips next instruction since w15 stack pointer can never be odd
    .endm


/**
 * Increments the upper bits of the internal NOW timer count based on roll over
 * of the lower 16-bits contained in the hardware timer. If applicable, this ISR
 * also calls NOW_TaskDispatcher() to launch (at IPL0) NOW tasks that are due
 * for execution.
 *
 * To ensure a pseudo-atomic update of bits [63:16] of the NOW count,
 * NOW_countPtr is used combined with two copies of the high bits. Rather than
 * incrementing and performing cary propagation on the "live" bits that could be
 * in the process of being read by lower priority code, this ISR instead
 * increments the non-live buffer and comits the change by updating
 * NOW_countPtr. The NOW_32()/NOW_64() functions used for reading the current
 * count need only check that the NOW_countPtr value did not change (relative to
 * when first read) by the time all upper bits are read.
 *
 * This ISR is structured such that this ISR itself can be called from two or
 * more places in code simultaneously without corrupting the output result or
 * any internal state. For example, if the hardware interrupt calls this ISR
 * starting from the main() context, a higher priority ISR preempts execution
 * and then a polling call is made from the high priority ISR to this
 * _TxInterrupt() ISR, then two instances of this ISR will be simultaneously
 * executed without resulting in a double increment of the upper bits (or any
 * non-coherent increments to some of the component words).
 *
 * @return NOW_internalCountPing or NOW_internalCountPong is incremented by 1
 *         and NOW_countPtr is updated to point to the updated/now-live buffer.
 *
 *         Additionally, if this ISR is executed without any lower priority ISRs
 *         currently active (i.e. interrupt occurs starting from main() IPL0
 *         context), then the NOW_TaskDispatcher() internal function is called
 *         to launch any pending tasks. The NOW_TaskDispatcher() function
 *         executes at IPL0, so only a handful of instructions outside of the
 *         _TxInterrupt() ISR will block the CPU at the timer's configured IPL.
 */
;void __attribute__((interrupt, keep, weak)) _T5Interrupt(void);
    .pushsection .text._T5Interrupt, code, keep
    .weak   __T5Interrupt
    .type   __T5Interrupt, @function
    .weak   _NOW_ISR
    .type   _NOW_ISR, @function
    .extern _IFS0
    .weak   _NOW_IFSReg
    .weak   _NOW_IFSBit
    .weak   _NOW_internalCount  ; Must be aligned(16)
    .equ    countPtr, 6
    .weak   _NOW_TaskDispatcher
__T5Interrupt:
_NOW_ISR:
    push    w1
    push.d  w2
    clr     w3                              ; w3 = 0 (for carry propagation)
    mov     _NOW_internalCount+countPtr, w1 ; w1 = source ptr
    xor     w1, #0x8, w2                    ; w2 = dest ptr  (pong pointer when w1 = ping; ping pointer when w1 = pong)
    inc     [w1++], [w2++]                  ; *dest++ = *source++ + 1;
    addc    w3, [w1++], [w2++]              ; *dest++ = 0 + *source++ + C;
    addc    w3, [w1], [w2]                  ; *dest   = 0 + *source + C;

    sub     w2, #4, w2
    mov     w2, _NOW_internalCount+countPtr ; NOW_internalCount.NOW_countPtr = &NOW_internalCountPing or &NOW_internalCountPong (whichever we wrote to as the destination)
    mov     #_IFS0, w1
    add     #_NOW_IFSReg, w1
    bclr    [w1], #_NOW_IFSBit              ; IFS1bits.TxIF = 0;

    mov     #handle(_NOW_TaskDispatcher), w1
    cp0     w1
    btss    SR, #SR_Z   ; Bit 1
    goto    w1                              ;// Branch to lightweight scheduler in ezbl_lib.a if this symbol is defined (gets defined automatically if anything calls EZBL_CreateTask()).
    pop.d   w2
    pop     w1
    retfie

    .size __T5Interrupt, . - __T5Interrupt
    .size _NOW_ISR, . - _NOW_ISR
    .popsection
