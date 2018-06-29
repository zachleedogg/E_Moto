;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Source File
;
;  Summary:
;    Assembly language optimized routines implementing the core EZBL API
;
;  Description:
;    __reset/__resetPRI alternative implementation from XC16. Functionality is 
;    intended to be identical, but may be subject to minor change.
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

    .equ    SR_C, 0x0   ; SR<C> is bit 0
    .equ    SR_Z, 0x1   ; SR<Z> is bit 1
    .equ    SR_OV, 0x2  ; SR<OV> is bit 2
    .equ    SR_N, 0x3   ; SR<N> is bit 3


;    .ifndef ffunction
;        .pushsection .init, code
;    .else
;        .pushsection .init.resetPRI, code
;    .endif
    .pushsection .text.EZBL_CRTReset, code
    
    .extern _main
    .extern __crt_start_mode
    .extern __SP_init
    .extern __SPLIM_init
    .extern _WREG4
    .extern _CORCON
    .weak   _SPLIM
    .weak   __enable_fixed
    .weak   __user_init
    .weak   __restart_dinit_tbloffset
    .weak   __restart_dinit_tblpage
    .weak   __dinit_tbloffset
    .weak   __dinit_tblpage
    .weak   __has_user_init
    .weak   __const_psvpage
    .weak   _DSRPAG                    
    .weak   _PSVPAG
        
    .global __resetPRI
    .weak   __reset
    .global _EZBL_CRTReset

    .ifdef  __C30ELF
        .type   __resetPRI, @function
        .type   __reset, @function
        .type   _EZBL_CRTReset, @function
    .endif
__resetPRI:    ; This symbol incompatible if --no-data-init is used at linking ("Init data sections" is unchecked in xc16-ld project options). --no-data-init seems to force define and include its own __resetPRI symbol.
__reset:
_EZBL_CRTReset:
    mov     #__SP_init, w15             ; Initialize w15 stack pointer
    mov     #__SPLIM_init, w14
    mov     w14, _SPLIM                 ; Initialize SPLIM stack upper limit. Note: next instruction must not reference the w15 stack pointer
    
    mov     #__enable_fixed, w0
    sl      w0, #4, w0                  ; CORCON = 0x0010 when fixed point enabled, or w0 = 0x0000 when fixed point disabled
    bset    w0, #2                      ; Set CORCON<2>, which is the PSV enable bit on dsPIC33F/PIC24H and PIC24F devices (without EDS). On PIC24E/dsPIC33E/etc, this is a read-only SFA bit, so setting it won't harm anything.
    ior     _CORCON

    mov     #__const_psvpage, w0       ; Optional code for source compatibility across all 16-bit device families
    mov     w0, _DSRPAG                ; Writes w0 = w0 if DSRPAG does not exist on this device (_DSRPAG declared weak)
    mov     w0, _PSVPAG                ; Writes w0 = w0 if PSVPAG does not exist on this device (_PSVPAG declared weak)

    call    __crt_start_mode            ; Initialize from .dinit or .rdinit based on _crt_start_mode() return value, or skip initialization altogether when neither section present
    mul.uu  w0, #_WREG4, w4
    mov     #__restart_dinit_tbloffset, w0
    mov     #__restart_dinit_tblpage, w1
    mov     #__dinit_tbloffset, w2
    mov     #__dinit_tblpage, w3
    mov.d   w2, [w4]
    ior     w0, w1, w2                  ; w2 is throw away register
    btss    SR, #SR_Z
    call    __data_init

    mov     #__has_user_init, w0
    btsc    w0, #0
    call    __user_init
    call    _main
    .pword  0xDA4000                    ; BREAK opcode (needs XC16 v1.30+ to use the 'break' mneumonic)
    reset

    .size   __reset, . - __reset
    .size   __resetPRI, . - __reset
    .size   _EZBL_CRTReset, . - _EZBL_CRTReset
    .popsection

    
    
    .pushsection .text._crt_start_mode, code
    .global     __crt_start_mode
    .weak       __crt_start_mode
    .extern     _NVMCON
    .type       __crt_start_mode, @function
__crt_start_mode:
    btsc    _NVMCON, #11    ; NVMCON<11> is SFTSWP: Partition Soft Swap Status bit on applicable dual partition devices
    retlw   #1, w0          ; 0x0001 returned when BOOTSWP executed --> initialize using .rdinit table
    retlw   #0, w0          ; 0x0000 returned for ordinary reset    --> initialize using  .dinit table
    
    .size   __crt_start_mode, . - __crt_start_mode
    .popsection


;    .pushsection    .text.data_init, code
;    ; w0:w1 = .dinit/.rdinit table record start program address (as a PSV/EDS pointer, not TBLPAG based one)
;    push    DSRPAG
;    push    DSWPAG
;    mov     w1, DSRPAG
;init_record_parse:
;    mov     #0x0000, w7         ; w7 = 0x0000
;    mov     #0x007F, w6
;next_record:
;    mov     #WREG2, w1
;    repeat  #2
;    mov     [w0++], [w1++]      ; w2 = dest RAM address, w3 = byte len, w4 = {page[15:7], format[6:0]}
;    cp0     w2
;    bra     Z, init_done
;    and     w6, w4, w5          ; w5 = format
;    lsr     w4, #7, w4          ; w4 = page
;    dec     w3, w3
;        
;call_function:
;    cp      w5, #3
;    bra     NZ, generate_zeros
;    push    w0
;    push    DSRPAG
;    mov     #packed_lo(1f), w0
;    mov     #packed_hi(1f), w1
;    push.d  w0
;    push    w2
;    push    w4
;    return
;1:  pop     DSRPAG
;    pop     w0
;    bra     init_record_parse
;    
;generate_zeros:
;    mov     w4, DSWPAG
;    cp      w5, #0
;    bra     NZ, copy_packed
;    repeat  w3
;    mov.b   w7, [w2++]
;
;    
;copy_packed:
;    cp      w5, #2
;    bra     NZ, copy_unpacked
;    mov     #43691, w1          ; 43691 = 65536*2/3 (+iota to avoid integer truncation precision loss)
;    mul.uu  w3, w1, w6
;    dec     w7, w6
;    repeat  w6
;    mov     [w0++], [w15++]
;    
;copy_unpacked:
;    cp      w5, #1
;    bra     NZ, unknown_record
;
;    repeat  w3
;    mov.b   [w0++], [w2++]
;    
;unknown_record:
;    bra     next_record
;    
;init_done:
;    pop     DSWPAG
;    pop     DSRPAG
;    return
;    
;    .popsection
    