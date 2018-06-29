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
 * void EZBL_CANConvertSWMsgToHW(EZBL_CAN_HW_MSG *hardwareMsgDest, EZBL_CAN_SW_MSG *softwareMsgSrc);
 *
 * Copies and reorders CAN message bytes from a CAN EZBL_CAN_SW_MSG 
 * structure used by software to the EZBL_CAN_SW_MSG structure format used by 
 * CAN TX hardware.
 *
 * @param *hardwareMsgDest Pointer to the caller allocated EZBL_CAN_HW_MSG 
 *                  structure buffer to write to.
 * @param *softwareMsgSrc Pointer to the software EZBL_CAN_SW_MSG structure 
 *                  to read from.
 */
     .pushsection   .text.EZBL_CANConvertSWMsgToHW, code
     .global        _EZBL_CANConvertSWMsgToHW
     .type          _EZBL_CANConvertSWMsgToHW, @function
     .extern        DSRPAG
     .weak          DSRPAG
     .extern        SR
_EZBL_CANConvertSWMsgToHW:
    ; Set DSRPAG = 0x001 so we can access up to 64KB of RAM, which is where DMA RAM could be on devices like dsPIC33EP512MU810 (48KB of RAM total)
    push    DSRPAG          ; Could actually push w0 since some parts have PSVPAG instead
    mov     w0, w7
    mov     #0x0001, w0
    mov     w0, DSRPAG      ; May overwrite w0

    ; Read everything but the data bytes into CPU working registers:
    mov.d   [w1++], w2      ; Read {ID} -> w2, w3
    mov	    [w1++], w4      ; Read {dataCount} -> w4
    mov     [w1+8], w5      ; Read {rxFilterHit[7:0], unused[5:0], remoteTransmitRequest, extendedID} -> w5

    btst    w5, #0          ; Check if this is a Standard frame or an Extended frame
    bra     NZ, 0f
    
    ; SID_Present
    sl      w2, #5, w2      ; w2 = {SID[10:0], :5}, overshifted by 3 to enforce zeros in top 3 bits
    lsr     w2, #3, w2      ; w2 = {:3, SID[10:0], :2}, shifted back down to align to 11-bit SID position
    clr     w3              ; w3 = 0x0000 to clear EID bits
    btsc    w5, #1          ; Test remoteTransmitRequest bit
    bset    w2, #1          ; Set SRR = 1
    bra     1f

    
0:  ; EID_Present:
    ;Goal is:
        ;     w2 = {:3, SID10_0:11, SRR:1, IDE:1}               <- from w2={ID[15:0]}
        ;     w3 = {:4, EID17_6:12}                             <- from w3={ID[28:16]};  NOTE: EID[17:6] is really ID[28:17]
        ;     w4 = {EID5_0:6, RTR:1, RB1:1, :3, RB0:1, DLC:4}   <- from w4={:12, DLC:4};       EID[5:0]  is really ID[16:11]
        ;     w5 = {:3, FILHIT[4:0], :8}                        <- from w5={rxFilterHit[7:0], unused[5:0], remoteTransmitRequest, extendedID}
    lsr     w2, #11, w6     ; w6 = {:11, ID[15:11]}
    sl      w2, #5, w2      ; w2 = {ID[10:0], :5}. Only need to shift 2, but overshift 3 to nuke the top three MSbits.
    lsr     w2, #3, w2      ; Realign back down to SID[10:0] field. w2 = {:3, SID10_0:11, :2}
    bset    w2, #0          ; Set IDE since this is an Extended ID frame. w2 now contains {:3, SID:11, SRR:1, IDE:1}, done
    sl      w6, #11, w6     ; w6 = {ID[15:11], :11}
    lsr     w3, w3          ; w3 = {EID17_6}, done; C = EID5
    rrc     w6, w6          ; w6 = {EID5_0:6, :10}
    ior     w6, w4, w4      ; w4 = {EID5_0:6, :1, RB1:1, :3, RB0:1, DLC:4}
    btsc    w5, #1          ; Test remoteTransmitRequest bit
    bset    w4, #9          ; w4 = {EID5_0:6, RTR:1, RB1:1, :3, RB0:1, DLC:4}, done

1:  ; Common processing
    clr.b   w5              ; w5 = {:3, FILHIT[4:0], :8}, done       
    mov.d   w2, [w7++]		; Save {:3, SID10_0:11, SRR:1, IDE:1} and {:4, EID17_6}
    mov	    w4, [w7++]		; Save {EID5_0:6, RTR:1, RB1:1, :3, RB0:1, DLC:4}
    repeat  #3              ; Copy data fields unchanged (8 bytes)
    mov     [w1++], [w7++]  
    mov     w5, [w7]		; Save {:3, FILHIT[4:0], :8}

    pop     DSRPAG
    return
    .size       _EZBL_CANConvertSWMsgToHW, . - _EZBL_CANConvertSWMsgToHW
    .popsection
    