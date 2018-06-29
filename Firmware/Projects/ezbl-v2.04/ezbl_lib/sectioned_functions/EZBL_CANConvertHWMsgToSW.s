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
 * void EZBL_CANConvertHWMsgToSW(EZBL_CAN_SW_MSG *softwareMsgDest, EZBL_CAN_HW_MSG *hardwareMsgSrc);
 *
 * Copies and reorders CAN message bytes from a CAN EZBL_CAN_HW_MSG 
 * structure used by CAN TX/RX hardware to the EZBL_CAN_SW_MSG structure format 
 * that is more efficient for reading/writing in software.
 *
 * @param *softwareMsgDest Pointer to the caller allocated software 
                    EZBL_CAN_SW_MSG structure buffer to write to.
 * @param *hardwareMsgSrc Pointer to the hardware EZBL_CAN_HW_MSG structure 
 *                  to read from.
 */
     .pushsection   .text.EZBL_CANConvertHWMsgToSW, code
     .global        _EZBL_CANConvertHWMsgToSW
     .type          _EZBL_CANConvertHWMsgToSW, @function
     .extern        DSRPAG
     .weak          DSRPAG
_EZBL_CANConvertHWMsgToSW:
    ; Set DSRPAG = 0x001 so we can access up to 64KB of RAM, which is where DMA RAM could be on devices like dsPIC33EP512MU810 (48KB of RAM total)
    push    DSRPAG          ; Could actually push w0 since some parts have PSVPAG instead
    mov     w0, w7
    mov     #0x0001, w0
    mov     w0, DSRPAG      ; May overwrite w0
    
    ; Read everything but the data bytes into CPU working registers:
    mov.d   [w1++], w2		; w2 = {:3, SID:11, SRR:1, IDE:1}
                            ; w3 = {EID17_6}
    mov     [w1++], w4		; w4 = {EID5_0:6, RTR:1, RB1:1, :3, RB0:1, DLC:4}
    mov     [w1+8], w5      ; w5 = {:3, FILHIT[4:0], :8}
    
    btst    w2, #0          ; Check if this is a Standard frame or an Extended frame
    bra     NZ, 0f

    ; SID_Present
    btsc    w2, #1          ; Test SRR bit
    bset    w5, #1          ; Set remoteTransmitRequest = 1
    lsr     w2, #2, w2      ; Align SID[11:0] to SID[15:0]
    mov     #0x07FF, w0
    and     w0, w2, [w7++]  ; Save ID[15:0]
    clr     [w7++]          ;        [31:16]
    bra     1f
    
    ; EID_Present:
0:  bset    w5, #0          ; Set extendedID = 1
    btsc    w4, #9          ; Test RTR bit
    bset    w5, #1          ; Set remoteTransmitRequest = 1, done
    lsr     w2, #2, w2      ; w2 = {:5, EID[10:0]}
    sl      w4, w6          ; w6 = {EID[15:11]:5, RTR:1, RB1:1, :3, RB0:1, DLC:4, :1}, C = EID[16]
    mov     #0xFC00, w0
    and     w0, w6, w0      ; w0 = {EID[15:11]:5, :11}
    ior     w0, w2, [w7++]  ; Save (w0 | w2 == {EID[15:0]})
    rlc     w3, [w7++]      ; Save (w3<<1 | C) == {EID[28:16]})
1:  and     w4, #0xF, [w7++]; Save (w4 = {:12, Count:4})
    repeat  #3              ; Copy data fields unchanged (8 bytes)
    mov     [w1++], [w7++]  
    mov     w5, [w7]        ; Save (w5 == {FILTHIT[7:0], '0':2, RemoteFrame:1, ExtendedID:1}
    pop     DSRPAG
    return
    .size       _EZBL_CANConvertHWMsgToSW, . - _EZBL_CANConvertHWMsgToSW
    .popsection
    