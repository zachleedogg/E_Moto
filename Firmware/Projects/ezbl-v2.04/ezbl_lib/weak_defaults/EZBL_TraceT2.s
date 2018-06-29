;/*******************************************************************************
;  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM source file (ezbl_lib)
;
;  Summary:
;    Test code - do not use
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

    .equ    _SPIxSTAT_SPIRBF_POSITION, 0x0  ; Receive Buffer Full
    .equ    _SPIxSTAT_SPITBF_POSITION, 0x1  ; Transmit Buffer Full
    .equ    _SPIxSTAT_SPITBE_POSITION, 0x3  ; Transmit Buffer Empty
    .equ    _SPIxSTAT_SPIRBE_POSITION, 0x5  ; Receive Buffer Empty
    .equ    _SPIxSTAT_SRMT_POSITION,   0x7  ; Shift Register Empty

    .equ    T1IF, 3     ; T1IF always in IFS0<3>
    .equ    T2IF, 7     ; T2IF always in IFS0<7>
    .equ    T3IF, 8     ; T3IF always in IFS0<8>
    .equ    T4IF, 11    ; T4IF always in IFS1<11>
    .equ    T5IF, 12    ; T5IF always in IFS1<12>
    .equ    T6IF, 15    ; T6IF always in IFS2<15>, but note: not all devices have Timer 6



    .pushsection .npbss.EZBL_traceSave, persist, near
    .global _EZBL_traceSave
    .global _EZBL_lastRCOUNT
    .global _EZBL_linearIncCount
    .type   _EZBL_traceSave, @object
    .type   _EZBL_lastRCOUNT, @object
    .type   _EZBL_traceNeg2, @object
    .type   _EZBL_linearIncCount, @object
_EZBL_traceSave:
    .space 4
_EZBL_lastRCOUNT:
    .space 2
_EZBL_linearIncCount:
    .space 2
    .popsection

/**
 * Writes the current PC, in an encoded/compressed form to an Ethernet UDP
 * socket. This is test code that should not be used (will not work on
 * dsPIC33E/PIC24E devices, may not work on others, requires other Ethernet code
 * that isn't distributed, causes poor execution rate that can change program
 * flow, etc.).
 */
;void __attribute__((interrupt, keep, weak)) _T2Interrupt(void);
    .pushsection .text._T12nterrupt, code, keep
    .weak   __T2Interrupt
    .type   __T2Interrupt, @function
    .global _EZBL_TraceT2
    .type   _EZBL_TraceT2, @function
    .extern _IFS0
    .extern _SPI4STATL
    .extern _EZBL_traceSave
    .extern _EZBL_lastRCOUNT
    .extern _EZBL_linearIncCount
    .extern _nextTXLen
    .extern _UDP_EndPacket
__T2Interrupt:
_EZBL_TraceT2:
    push    w0
    bclr    _T2CON, #15         ; Turn off TMR2
    bclr    _IFS0, #T2IF
    mov     #-2, w0
    mov     w0, _TMR2
    cp0     RCOUNT              ; Allow repeats to keep getting sent
    btss    SR, #SR_Z
    bset    _EZBL_traceSave, #0
    mov     RCOUNT, w0          ; But not if we aren't actually making progress
    cp      _EZBL_lastRCOUNT
    btsc    SR, #SR_Z
    bclr    _EZBL_traceSave, #0
    mov     w0, _EZBL_lastRCOUNT
    mov     [w15-6], w0         ; w0 = LSbits of PC return address
    bclr    w0, #0
    cp      _EZBL_traceSave
    bra     Z, extra_cycle_return
    inc2    _EZBL_traceSave
    cp      _EZBL_traceSave
    bra     NZ, branchHappened
regularInc:
    inc     _EZBL_linearIncCount
    mov     #0x007F, w0
    cp      _EZBL_linearIncCount
    bra     Z, flushIncCount
    pop     w0
    bset    _T2CON, #15     ; Turn TMR2 back on
    retfie

flushIncCount:
    mov     _EZBL_linearIncCount, w0
0:  btsc    _SPI4STATL, #_SPIxSTAT_SPITBF_POSITION
    bra     0b
    mov.b   WREG, _SPI4BUFL   ; SPIxBUF = EZBL_linearIncCount;
    clr     _EZBL_linearIncCount
    pop     w0
    bset    _T2CON, #15     ; Turn TMR2 back on
    retfie

extra_cycle_return:
    dec     _TMR2
    pop     w0
    bset    _T2CON, #15     ; Turn TMR2 back on
    retfie

branchHappened:
    push    w0
    cp0     _EZBL_linearIncCount
    bra     Z, 1f
    push    _EZBL_linearIncCount
0:  btsc    _SPI4STATL, #_SPIxSTAT_SPITBF_POSITION
    bra     0b
    pop     _SPI4BUFL       ; SPIxBUF = EZBL_linearIncCount;
    inc     _nextTXLen
    clr     _EZBL_linearIncCount
1:  mov     [w15-6], w0     ; w0 = PC<22:16>;
    bset    w0, #7          ; w1<7> = 1 to flag this is a 23-bit PC value and not a linear inc count
0:  btsc    _SPI4STATL, #_SPIxSTAT_SPITBF_POSITION
    bra     0b
    mov     w0, _SPI4BUFL   ; SPIxBUF = PC<22:16>;
    inc2    _nextTXLen
    pop     w0              ; w0 = PC<15:1> with bit 0 always cleared to '0'
    mov     w0, _EZBL_traceSave
    swap    w0
0:  btsc    _SPI4STATL, #_SPIxSTAT_SPITBF_POSITION
    bra     0b
    mov     w0, _SPI4BUFL   ; SPIxBUF = PC<15:8>;
    cp0     RCOUNT          ; Allow repeats to keep getting sent
    btss    SR, #SR_Z
    bset    _EZBL_traceSave, #0
    inc     _nextTXLen
    swap    w0
0:  btsc    _SPI4STATL, #_SPIxSTAT_SPITBF_POSITION
    bra     0b
    mov     w0, _SPI4BUFL   ; SPIxBUF = PC<7:0>;
    mov     #1460, w0
    cp      _nextTXLen
    bra     C, flushUDP
    pop     w0
    bset    _T2CON, #15     ; Turn TMR2 back on
    retfie

flushUDP:
    push    w1
    push.d  w2
    push.d  w4
    push.d  w6
    push    RCOUNT
0:  btss   _SPI4STATL, #_SPIxSTAT_SPITBE_POSITION
    bra 0b
0:  btss   _SPI4STATL, #_SPIxSTAT_SRMT_POSITION
    bra 0b
    call    _UDP_EndPacket
    pop     RCOUNT
    pop.d   w6
    pop.d   w4
    pop.d   w2
    pop.d   w0
    bset    _T2CON, #15     ; Turn TMR2 back on
    retfie


    .size __T2Interrupt, . - __T2Interrupt
    .popsection
    