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
 * Writes data into a circular EZBL_FIFO RAM buffer from a normal data array, 
 * updating the headPtr and dataCount in the FIFO structure. dataCount is 
 * updated atomically without disabling interrupts.
 *
 * After the write is complete and the headPtr/dataCount FIFO parameters 
 * updated, an optional destFIFO->onWriteCallback() function pointer is called.
 * This callback is invoked only when the pointer is non-null.
 *
 * The onWrite callback function must have type:
 *      unsigned int yourOnWriteFuncName(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen);
 * The return value of this callback is returned unchanged by EZBL_FIFOWrite(), 
 * giving you an opportunity to manipulate the FIFO contents, return value, 
 * and/or trigger other code to start reading the data out of the FIFO (ex: to a 
 * UART TX or other communications ISR). The bytesPushed parameter specifies the 
 * byte count actually added to the FIFO, *writeSrc is the original value of 
 * *source, and reqWriteLen is the original value of dataLen.
 *   
 * Note: the onWrite callback is called after the write has already taken place.
 * Therefore, any manipulation done to the writeSrc array will have no effect on 
 * the FIFO. However, as writing to the FIFO does not destroy or manipulate the 
 * source data, it is possible to re-read the source data from the original 
 * pointer if you wish to save the data elsewhere (ex: a second FIFO connected 
 * to a debug terminal or other "communications tee").
 * 
 * @param *destFIFO EZBL_FIFO pointer to write the copied data to. FIFO overflow 
 *                  is supressed. FIFO wraparound and write pointer update is 
 *                  internally handled. 
 *
 *                  If this pointer is null (0x0000), all processing is skipped
 *                  and no data is read from *source. However, EZBL_FIFOWrite()
 *                  returns dataLen, emulating an infinite bit-bucket.
 * @param *source RAM or PSV pointer to read data from. Address can be byte 
 *                aligned. This pointer cannot be null except when <i>length</i> 
 *                is 0.
 * @param dataLen Number of bytes to copy. This value must be <= 16384 on 
 *                dsPIC30, PIC24F, PIC24H, or dsPIC33F families.
 *
 *                Zero is allowed, resulting in no write operation. However, the
 *                destFIFO->onWriteCallback() function will still be called, if 
 *                not null.
 * 
 * @return Value returned by destFIFO->onWriteCallback() or the number of bytes 
 *         written to the FIFO. When unchanged by the callback, this parameter 
 *         will be less than <i>length</i> only when the FIFO becomes completely 
 *         full and the write is terminated early to avoid FIFO overflow.
 *
 *         If *destFIFO is null, dataLen is returned, emulating a successful
 *         write.
 */
; unsigned int EZBL_FIFOWrite(EZBL_FIFO *destFIFO, void *source, unsigned int dataLen);
 
	.pushsection    .text.EZBL_FIFOWrite, code
	.global         _EZBL_FIFOWrite
    .type           _EZBL_FIFOWrite, @function
    .equ            dataCount, 0        ; Byte offsets to the EZBL_FIFO structure elements
    .equ            headPtr, 2
    .equ            tailPtr, 4
    .equ            fifoSize, 6
    .equ            fifoRAM, 8
    .equ            onWriteCallback, 10
    .equ            onReadCallback, 12
    .equ            flushFunction, 14
    .equ            irqNum, 16
    .equ            activity, 18
; w2 should point to one of these:
;struct EZBL_FIFO
;{
;    volatile unsigned int dataCount;    // Number of bytes that exist in the FIFO. i.e. headPtr - tailPtr, adjusted for wraparound and made atomically readable. You must use EZBL_ATOMIC_ADD() and EZBL_ATOMIC_SUB() if this value is to be modified.
;    unsigned char *headPtr;             // Pointer to fifoRAM area for writing
;    unsigned char *tailPtr;             // Pointer to fifoRAM area for reading
;    unsigned int fifoSize;              // Number of bytes allocated to the fifoRAM
;    unsigned char *fifoRAM;             // Main FIFO buffer RAM pointer (or const PSV pointer if only doing reading)
;    unsigned int (*onWriteCallback)(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);   // Pointer to a function to be called anytime the FIFO is written (or attempted to be written)
;    unsigned int (*onReadCallback)(unsigned int bytesPulled, void *readDest, unsigned int reqReadLen, EZBL_FIFO *srcFIFO);      // Pointer to a function to be called anytime the FIFO is read (or attempted to be read)
;    unsigned int (*flushFunction)(EZBL_FIFO *fifo, unsigned long timeout); // Pointer to a function to be called anytime the EZBL_FIFOFlush() function is called
;    unsigned int irqNum;                // IRQ Number to associate with this FIFO. This number is used to decode the correct IFSx/IECx/IPCx register/bit(s) to access when calling EZBL_FIFOIntEnable()/EZBL_FIFOIntDisable()/EZBL_FIFOIntClear()/EZBL_FIFOIntRaise()/EZBL_FIFOIntEnableSet()/EZBL_FIFOIntFlagSet()/EZBL_FIFOGetIntEn()/EZBL_FIFOGetIntPri() functions. IRQ numbers are zero-based where 0 indicates the first ordinary peripheral hardware interrupt (i.e. not a trap)
;    volatile EZBL_COM_ACTIVITY activity;// Status bit flags capturing various software and hardware state change events like RX interrupt byte, software RX FIFO read, TX interrupt byte, software TX FIFO write, RX bootloader wake key detection, HW/SW FIFO RX overflow and other activity
;};

_EZBL_FIFOWrite:    ; unsigned int EZBL_FIFOWrite(EZBL_FIFO *destFIFO, void *source, unsigned int dataLen);
    add     w0, #0, w3  ; w3 = EZBL_FIFO *dest and test for SR<Z>
    mov     w2, w0      ; w0 = dataLen
    bra     Z, done     ; Abort if *destFIFO is a null pointer (effectively sends data to bit bucket)
 
    ; w0: dataLen, then secondChunkSize, , then min(length, freespaceInFIFO), then secondChunkSize?
    ; w1: *source
    ; w2: dataLen
    ; w3: EZBL_FIFO *dest
    ; w4: headPtr
    ; w5: fifoBaseAddress, then freespaceInFIFO, then firstChunkSize
    ; w6: fifoSize, then fifoEndAddress
    ; w7: bytesUntilWrap, then temporary repeat counter

    mov     [w3+fifoRAM], w7    ; fifoBaseAddress = EZBL_FIFO->fifoRAM;
    mov     [w3+fifoSize], w6   ; fifoSize = EZBL_FIFO->fifoSize;
    mov     [w3+headPtr], w4    ; headPtr = EZBL_FIFO->headPtr;
    sub     w6, [w3], w5        ; freespaceInFIFO = fifoSize - dataCount;
    add     w7, w6, w6          ; fifoEndAddress = fifoBaseAddress + fifoSize;
    sub     w6, w4, w7          ; bytesUntilWrap = fifoEndAddress - headPtr;

    cp      w5, w0              ; if(freespaceInFIFO - writeLen)
    btss    SR, #SR_C           ;       (skip when no borrow required)
    mov     w5, w0              ;       writeLen = freespaceInFIFO;

    mov     w0, w5              ; firstChunkSize = writeLen;
    cp      w7, w5              ; if(bytesUntilWrap - firstChunkSize)
    btss    SR, #SR_C           ;       (skip when no borrow required)
    mov     w7, w5              ;       firstChunkSize = bytesUntilWrap;
    
    ; Do first copy
    dec     w5, w7              ; if(firstChunkSize-- == 0)
    bra     NC, 2f              ;       skip mov repeat loop
	repeat	w7                  ; for(w7 = 0; w7 < firstChunkSize; w7++)
	mov.b	[w1++], [w4++]      ;       *headPtr++ = *source++;
2:  cp      w4, w6              ; if(headPtr >= fifoEndAddress)
    btsc    SR, #SR_C           ;     headPtr = EZBL_FIFO->fifoRAM; // skip if we borrow on (headPtr - fifoEndAddress)
    mov     [w3+fifoRAM], w4
    
    ; Do second copy, if needed
    sub     w0, w5, w6              ; secondChunkSize = writeLen - firstChunkSize;
    bra     Z, 1f                   ; if(secondChunkSize != 0)
    dec     w6, w7                  ;       for(w7 = 0; w7 < secondChunkSize; w7++)
    repeat  w7                      ;
    mov.b   [w1++], [w4++]          ;           *headPtr++ = *source++;
    
1:  add     w0, [w3], [w3]          ; EZBL_FIFO->dataCount += writeLen;     // atomic add
    mov     w4, [w3+headPtr]        ; EZBL_FIFO->headPtr = headPtr;
    mov     [w3+onWriteCallback], w7; Check for non-null onWriteCallback() function pointer
    sub     w1, w0, w1              ; Regenerate original *source pointer for callback
    cp0     w7
    btss    SR, #SR_Z
    call    w7                      ; unsigned int onWriteCallback(unsigned int bytesPushed, void *writeSrc, unsigned int reqWriteLen, EZBL_FIFO *destFIFO);
done:
    return

    .size   _EZBL_FIFOWrite, . - _EZBL_FIFOWrite
    .popsection
