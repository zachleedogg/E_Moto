;/*******************************************************************************
;  Easy Bootloader Library API Source File
;
;  Summary:
;    Sets the Interrupt Enable bit for the timer peripheral selected by the
;    NOW_Reset() EZBL macro. This writes IECxbits.T1IE, IECxbits.CCT1IE, etc. as
;    applicable to '1'.
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
 * Sets the Interrupt Enable bit (ex: IFSx<T1IE> or IFSx<CCT1IE>) used by the
 * NOW timing module and scheduler.
 *
 * If you leave the interrupt disabled for too long and do not poll NOW_32() or
 * NOW_64() frequently enough, it is possible for the 16-bit hardware timer to
 * have rolled over more than once. In this case, you will have lost time,
 * effectively making various stored NOW_16()/NOW_32()/NOW_64() return values
 * less accurate and potentially non-monotonic. It is therefore recommended that
 * the NOW interrupt be disabled or masked for less than 128K instruction
 * cycles max.
 *
 * @return 0 if the prior state of the interrupt was disabled
 *         1 if the prior state of the interrupt was enabled
 *
 *         Timer/CCP Timer interrupt enable bit in IECx register is set to '1'.
 *
 *         If the NOW_Reset() macro or a NOW_TMRx_ResetCalc(),
 *         NOW_TMRx_ResetConst(), NOW_CCPx_ResetCalc(), or NOW_CCPx_ResetConst()
 *         function is never called anywhere in the project (or EZBL bootloader
 *         project), this function will effectively resolve to a series of
 *         no-operations without causing a linking error. I.e. No SFR writes
 *         will take place and no harm will be done, with a mere 0 always being
 *         returned.
 */
; int NOW_IntEnable(void);
; extern int __attribute__((alias("NOW_IntEnable"))) NOW_EnableInterrupts(void);
    .pushsection .text.NOW_IntEnable, code
    .global _NOW_IntEnable
    .type   _NOW_IntEnable, @function
    .global _NOW_EnableInterrupts               ; Alias to NOW_EnableInt() defined for backwards compatibility with earlier EZBL v1.xx versions
    .type   _NOW_EnableInterrupts, @function
    .extern SR
    .extern _IEC0
    .weak   _NOW_timerSFRPtr    ; Check to ensure NOW_Reset() was called so we don't write to IEC<0> (typically INT0IE) and corrupt anything
    .weak   _NOW_IFSReg         ; Declared weak so linking doesn't force inclusion of a libary file that defines this value, which generally won't be the correct file. Instead, the NOW_Tx_Reset()/NOW_CCPx_Reset() function must be called, which defines which is the correct file and therefore who defines this symbol.
    .weak   _NOW_IFSBit         ; Declared weak so linking doesn't force inclusion of a libary file that defines this value, which generally won't be the correct file. Instead, the NOW_Tx_Reset()/NOW_CCPx_Reset() function must be called, which defines which is the correct file and therefore who defines this symbol.
_NOW_IntEnable:         ; int NOW_IntEnable(void);
_NOW_EnableInterrupts:  ; extern int __attribute__((alias("NOW_IntEnable"))) NOW_EnableInterrupts(void);
    mov     #_NOW_timerSFRPtr, w0
    cp0     w0
    bra     Z, 9f
    mov     #_IEC0, w0
    add     #_NOW_IFSReg, w0
    btsts.c [w0], #_NOW_IFSBit  ; Read and atomically set the IECybits.TxIE/IECybits.CCTxIE bit
    btss    SR, #SR_C           ; Return 0 when prior state was disabled, 1 when prior state was enabled. NVMKEY is a 'f' near accessible
9:  retlw   #0, w0
    retlw   #1, w0
    .size _NOW_EnableInterrupts, . - _NOW_EnableInterrupts
    .size _NOW_IntEnable, . - _NOW_IntEnable
    .popsection
