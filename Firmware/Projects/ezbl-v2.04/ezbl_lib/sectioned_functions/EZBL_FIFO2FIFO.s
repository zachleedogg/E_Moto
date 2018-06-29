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
 * Reads and removes data from one EZBL_FIFO and writes it to another EZBL_FIFO. 
 *
 * This is a wrapper function that internally calls EZBL_FIFORead(), storing the 
 * data linearly on the stack, then in turn calls EZBL_FIFOWrite(). Because the 
 * stack is used to temporarily store all requested copy data, it is not 
 * advisable to specify a very large dataLen unless you know that there is a 
 * large amount of free space on the stack to safely complete the transfer. 
 *
 * This function is attributed 'weak' such that it may be overridden or replaced 
 * in your code by an alternate implementation that copies data more directly 
 * between the two FIFOs. A future library implementation may also make this 
 * change. Therefore, although the source FIFO onReadCallback and destination 
 * FIFO onWriteCallback will presently be called exactly once each in this 
 * implementation, it is a good idea to assume that the two callbacks may be 
 * issued multiple times in the future for a piecewise data transfer directly 
 * between FIFOs.
 * 
 * @param *destFIFO EZBL_FIFO pointer to write the copied data to. If the 
 *                  srcFIFO underflows, the destFIFO will be written only with 
 *                  the data actually returned during reading. 
 *
 *                  Any applicable onWriteCallback will be triggered during one 
 *                  or more write cycles. 
 *
 *                  This pointer cannot be null (0x0000), even when 
 *                  <i>dataLen</i> is 0.
 *
 * @param *srcFIFO EZBL_FIFO pointer to read from.
 *
 *                 Any applicable onReadCallback will be triggered during one 
 *                 or more read cycles.
 *
 *                 This pointer cannot be null, even when <i>dataLen</i> is 0. 
 *
 * @param copyLen Number of bytes to copy. This value must be smaller than the 
 *                amount of free space on the stack, taking into account 
 *                additional margin for the onRead and onWrite callbacks, plus 
 *                any ISRs taking place.
 *
 *                Zero is allowed, resulting in no data transfer. However, the
 *                srcFIFO->onReadCallback() and destFIFO->onWriteCallback() 
 *                function will still be called, if not null.
 * 
 * @return Value returned by destFIFO->onWriteCallback() or the number of bytes 
 *         written to the FIFO. When unchanged by the callback, this parameter 
 *         will be less than <i>copyLen</i> when the source FIFO becomes empty 
 *         or the destination FIFO becomes full.
 */
; unsigned int __attribute__((weak)) EZBL_FIFO2FIFO(EZBL_FIFO *dest, EZBL_FIFO *source, unsigned int copyLen);
 
    .pushsection    .text.EZBL_FIFO2FIFO, code
    .global         _EZBL_FIFO2FIFO
    .weak           _EZBL_FIFO2FIFO
    .type           _EZBL_FIFO2FIF0, @function
    .extern         _EZBL_FIFOWrite
    .extern         _EZBL_FIFORead
_EZBL_FIFO2FIFO:
;   typedef struct
;   {
;       volatile unsigned int dataCount;    // Use EZBL_ATOMIC_ADD() and EZBL_ATOMIC_SUBTRACT() to change this value from C
;       unsigned char *headPtr;
;       unsigned char *tailPtr;
;       unsigned int fifoSize;
;       unsigned char *fifoRAM;
;       unsigned int (*onWriteCallback)(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);// Pointer to a function to be called anytime the FIFO is written (or attempted to be written)
;       unsigned int (*onReadCallback)(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *srcFIFO);  // Pointer to a function to be called anytime the FIFO is read (or attempted to be read)
;   } EZBL_FIFO;
;    push.d  w8
;    push.d  w0
;    push    w2
;    
;    ; Saturate copyLen to size we can write to destFIFO in one go
;    mov     [w0+6], w6  ; Get fifoSize
;    mov     [w0+8], w5  ; Kept: Get *fifoRAM base address
;    mov     [w0+2], w4  ; Kept: Get *headPtr
;    add     w6, w5, w3  ; Kept: Compute end address at looparound
;    push    w4
;    
;    sub     w3, [w0], w6; Compute total freeSpace
;    cp      w6, w2      ; if(freespace - copyLen < 0)
;    btss    SR, #SR_C
;    mov     w6, w2      ;       copyLen = freespace
;    sub     w3, w4, w6  ; Compute distance to wraparound
;    cp      w6, w2      ; if(dist_to_wrap - copyLen < 0)
;    btss    SR, #SR_C
;    mov     w6, w2      ;       copyLen = dist_to_wrap
;    
;
;    ; Saturate copyLen to size we can read from srcFIFO in one go
;    mov     [w1+6], w6  ; Get fifoSize
;    mov     [w1+8], w7  ; Kept: Get *fifoRAM base address
;    mov     [w1+4], w8  ; Kept: Get *tailPtr
;    add     w7, w6, w9  ; Kept: Compute end address before looparound
;    
;    sub     w9, w8, w6  ; Compute distance to wraparound
;    cp      w6, w2      ; if(dist_to_wrap - copyLen < 0)
;    btss    SR, #SR_C
;    mov     w6, w2      ;       copyLen = dist_to_wrap
;    mov     [w1+0], w6  ; Cache copy of dataCount for coherency
;    cp      w2, w6      ; if(copyLen - dataCount < 0)
;    btss    SR, #SR_C
;    mov     w6, w2      ;       copyLen = dataCount (cached copy)
;    
;    ; See if there is no work we can do
;    cp0     w2
;    bra     Z, processCallbacks
;    
;    ; Read directly into the destination FIFO
;    dec     w2, w6
;    repeat  w6
;    mov.b   [w8++], [w4++]
;    cp      w8, w9              ; if(tailPtr == wrap_address)
;    btsc    SR, #SR_Z
;    mov     w7, w8              ;       tailPtr = *fifoRAM
;    subr    w2, [w1], [w1]      ; Decrement dataCount in srcFIFO
;    mov     w8, [w1+4]          ; Update *tailPtr in srcFIFO
;    cp      w4, w3              ; if(headPtr == wrap_address)
;    btsc    SR, #SR_Z
;    mov     w5, w4              ;       headPtr = *fifoRAM
;    mov     w4, [w0+2]          ; Update *headPtr in destFIFO
;    add     w2, [w0], [w0]      ; Increment dataCount in destFIFO
;
;    
;    
;    ; Process onReadCallback
;;       unsigned int (*onReadCallback)(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *srcFIFO);  // Pointer to a function to be called anytime the FIFO is read (or attempted to be read)
;0:  mov     [w1+12], w6     ; Get onReadCallback() function pointer
;    mov     w1, w3          ; Align *srcFIFO to *srcFIFO parameter in callback
;    pop     w1              ; Get *dest into *readDest position
;    mov     w2, w0          ; Align copyLen to bytesPulled 
;    cp0     w6              
;    btss    SR, #SR_Z
;    call    w6
;    mov     w0, w2          ; Align returned read length with copyLen and reqWriteLen parameter
;
;    
;
;    
;    ; Process onWriteCallback
;;       unsigned int (*onWriteCallback)(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);// Pointer to a function to be called anytime the FIFO is written (or attempted to be written)
;    mov     [w0+12], w6     ; Get onWriteCallback() function pointer
;    mov     w0, w3          ; Align *dstFIFO to *dstFIFO parameter in callback
;    pop     w1              ; Get original srcFIFO->tailPtr into *writeSrc position
;    mov     [w1
;    mov     w2, w0          ; Align copyLen to bytesPushed
;    cp0     w6              
;    btss    SR, #SR_Z
;    call    w6
;    mov     w0, w2          ; Align returned read length with copyLen and reqWriteLen parameter
;    
;
;processCallbacks:    
;;       unsigned int (*onWriteCallback)(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);// Pointer to a function to be called anytime the FIFO is written (or attempted to be written)
;;       unsigned int (*onReadCallback)(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *srcFIFO);  // Pointer to a function to be called anytime the FIFO is read (or attempted to be read)
;0:  mov     [w1+12], w6     ; Get onReadCallback() function pointer
;    mov     w1, w3          ; Align *srcFIFO to *srcFIFO parameter in callback
;    pop     w1              ; Get *dest into *readDest position
;    mov     w2, w0          ; Align copyLen to bytesPulled 
;    cp0     w6              
;    btss    SR, #SR_Z
;    call    w6
    
    
    
    ;   typedef struct
;   {
;       volatile unsigned int dataCount;    // Use EZBL_ATOMIC_ADD() and EZBL_ATOMIC_SUBTRACT() to change this value from C
;       unsigned char *headPtr;
;       unsigned char *tailPtr;
;       unsigned int fifoSize;
;       unsigned char *fifoRAM;
;       unsigned int (*onWriteCallback)(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);// Pointer to a function to be called anytime the FIFO is written (or attempted to be written)
;       unsigned int (*onReadCallback)(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *srcFIFO);  // Pointer to a function to be called anytime the FIFO is read (or attempted to be read)
;   } EZBL_FIFO;
;
; unsigned int __attribute__((weak)) EZBL_FIFO2FIFO(EZBL_FIFO *dest, EZBL_FIFO *source, unsigned int copyLen);
; unsigned int EZBL_FIFORead(void *dest, EZBL_FIFO *srcFIFO, unsigned int dataLen);
; unsigned int EZBL_FIFOWrite(EZBL_FIFO *destFIFO, void *source, unsigned int dataLen);    
    
    
    ; w0 = *dest EZBL_FIFO
    ; w1 = *source EZBL_FIFO
    ; w2 = copyLen
    
    ; Trivial copy using Stack as a linear temporary buffer for everything in one go. Has problem if read data can't all fit in destFIFO.
    mov     w0, w6              ; w6 = *dest
    mov     w15, w0             ; w0 = temporary buffer address (and original stack pointer)
    btst.c  w2, #0              ; w7 = even adjusted copyLen
    addc    w15, w2, w15        ; w15 += even adjusted copyLen (copyLen + 1 if needed to make an even size)
    push    w0                  ; TOS = temporary buffer address (and original stack pointer)
    push    w6                  ; TOS = *dest for write phase
    call    _EZBL_FIFORead      ; w0 (copyLen_achieved) = EZBL_FIFORead(w0 = temporary buffer address = *dest, w1 = *source, w2 = copyLen);
    mov     w0, w2              ; w2 = propagated copyLen
    pop     w0                  ; w0 = *dest
    mov     [w15-2], w1         ; w1 = temporary buffer address (and original stack pointer)
    call    _EZBL_FIFOWrite     ; w0 (copyLen_achieved) = EZBL_FIFOWrite(w0 = *dest, w1 = temporary buffer address, w2 = copyLen_achieved);
    pop     w15                 ; nuke temporary buffer and restore w15 = original stack value
    return
    
    .size   _EZBL_FIFO2FIFO, . - _EZBL_FIFO2FIFO
    .popsection
