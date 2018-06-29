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
 * Calls a Interrupt Service Routine C function pointer at the same IPL as
 * currently set.
 *
 * This function sets up a fake ISR call frame on the stack such that when the
 * called ISR executes the RETFIE instruction, execution will return to the
 * instruction after the call instruction that called EZBL_CallISR() and retain
 * the same IPL as you called EZBL_CallISR() at.
 *
 * This function can be used to call ISRs of lower or higher priority than the
 * current CPU IPL. However, be very careful when doing this as the called ISR
 * can become self-reentrant if the ISR is enabled. Higher priority ISRs will
 * become self-reentrant if the hardware interrupt flag becomes set. Lower
 * priority ISRs will become self-reentrant if the ISR was already active when
 * this call to EZBL_CallISR() is made.
 *
 * @param ISRFunctionName C Function pointer of type void (*ISRFuncName)(void);
 *                              Ex: EZBL_CallISR(_U2TXInterrupt);
 *                        The _U2TXInterrupt() function prototype should have
 *                        already been declared just like when calling normal
 *                        functions.
 */
 ; void EZBL_CallISR(void (*ISRFunctionName)(void));
    .pushsection    .text.EZBL_CallISR, code
    .global         _EZBL_CallISR
    .type           _EZBL_CallISR, @function
    .extern         SR
    .extern         CORCON
_EZBL_CallISR:
    ; Set up a fake interrupt stack frame to call and set the IPL simultaneously
    mov     SR, w1          ; Get current IPL
    sl      w1, #8, w1      ; Move existing SR<7:5> IPL0-IPL2 over to Interrupt_Return_Address<31:29>, which are the return IPL low 3 bits
    btsc    CORCON, #3      ; Copy CORCON<IPL3> into Interrupt_Return_Address<23> where it lives between fields
    bset    w1, #7
    ior     w1, [--w15], [w15++]
    goto    w0
    .size           _EZBL_CallISR, . - _EZBL_CallISR
    .popsection
