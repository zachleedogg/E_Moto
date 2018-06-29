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
 * Asserts or deasserts reset on the specified slave CPU core.
 *
 * @param cpuID Slave CPU identifier. Must be set to 1 for devices with only one 
 *              slave CPU.
 *
 * @param holdInReset <ul>
 *                      <li>0 - Slave released from reset and allowed to execute</li>
 *                      <li>1 - Slave held in reset</li>
 *                    </ul>
 *
 * @return Toggles the MSIxCON<SLVEN> bit as needed to hold the slave CPU 
 *         core in reset or releases reset for ordinary slave execution. If the 
 *         Slave is already in the requested state, then no change occurs.
 * 
 *         Interrupts are globally disabled temporarily in order to set/clear 
 *         the MSIxCON<SLVST> bit, which requires cycle exact/uninterrupted 
 *         write access to the MSIxKEY register.
 */
; void EZBL_SlaveReset(int cpuID, int holdInReset);
    .pushsection    .text.EZBL_SlaveReset, code
    .global         _EZBL_SlaveReset
    .type           _EZBL_SlaveReset, @function
    .extern         _MSI1CON
    .weak           _MSI2CON
    .extern         _EZBL_DisableInterrupts
    .extern         _EZBL_RestoreInterrupts
_EZBL_SlaveReset:
    ; w0 = cpuID
    ; w1 = Assert Slave RESET (0 = release reset, 1 = hold in reset)
    mov     #_MSI2CON, w2
    mov     #_MSI1CON, w4
    sub     w2, w4, w2
    dec     w0, w0                  ; cpuID--;
    mul.uu  w2, w0, w2              ; w2 = (&MSI2CON-&MSI1CON)*cpuID
    add     w4, w2, w2              ; w2 = &MSIxCON
    
    ; Compute needed xor value to xor with SLVST to match requested reset state
    sl      w1, #15, w1             ; w1 = holdInReset<<15
    btss    [w2], #15               ; if(MSIxCON<SLVST> == 0)
    btg     w1, #15                 ;     w1 ^= 0x8000
    
    call    _EZBL_DisableInterrupts ; w0 = data needed to restore interrupts
    mov     #0x55, w3               ; Unlock writes to MSIxCON
    mov     w3, [w2+4]              ; MSIxKEY = 0x0055
    sl      w3, #1, w3
    mov     w3, [w2+4]              ; MSIxKEY = 0x00AA
    xor     w1, [w2], [w2]          ; MSIxCON ^= 0x8000 or 0x0000 (depending on previous SLVST state and and requested reset state)
    call    _EZBL_RestoreInterrupts ; Uses w0 data to restore interrupts
    clr     w3                      ; Kill 0x00AA unlock keying data
    return

    .size   _EZBL_SlaveReset, . - _EZBL_SlaveReset
    .popsection



