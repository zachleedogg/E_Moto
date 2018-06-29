;/*******************************************************************************
;  Easy Bootloader Library for PIC24 and dsPIC33
;
;  Summary:
;    Implements the EZBL_FIFOFlush() default function
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
 * Blocks until the FIFO holds 0 bytes, a timeout is reached, or an
 * implementation defined flush callback function returns.
 *
 * When the fifo->flushFunction pointer callback has been configured (set to a
 * non-null value), a timer is started, a TimeIsUp() function pointer is
 * generated, and execution is passed to the callback. If the callback chooses
 * to obey the timeout parameter, it can call the TimeIsUp() function to
 * determine when it should return.
 *
 * Only when fifo->flushFunction is zero, this function internally blocks until
 * the FIFO contents reach zero or the specified NOW timeout is reached.
 *
 * @param *fifo Pointer to the FIFO to "flush". If null, no operation is
 *              performed.
 *
 * @param timeout Maximum length of time the flush operation can block for,
 *                specified in NOW counts.
 *
 *                Use a multiple of NOW_second, NOW_millisecond, and/or
 *                NOW_microsecond to choose this timeout.
 *
 *                The special value (unsigned long)-1, or 0xFFFFFFFF, indicates
 *                an infinite wait time.
 *
 * @return flushFunction()'s callback return value, or, when null:
 *         <ul>
 *              <li>1 - All bytes in the FIFO were flushed (0 bytes remain) or
 *                      fifo was a null pointer</li>
 *              <li>0 - Timeout occurred</li>
 *         </ul>
 */
; int EZBL_FIFOFlush(EZBL_FIFO *fifo, unsigned long timeout);
	.pushsection    .text.EZBL_FIFOFlush, code
	.global         _EZBL_FIFOFlush
    .type           _EZBL_FIFOFlush, @function
    .extern         _NOW_32
    .equ            flushFuncOffset, 14
    .equ            dataCountOffset, 0
_EZBL_FIFOFlush:       ; int EZBL_FIFOFlush(EZBL_FIFO *fifo, unsigned long timeout);
                       ;                               w0                  w3:w2    ; NOTE: w1 skipped!
    cp0     w0
    btsc    SR, #SR_Z
    retlw   #1, w0      ; Successfully flushed to 0 bytes

    push    w8

    push.d  w2                          ; TOS = timeout
    mov     w0, w8                      ; w8 = *fifo pointer
    call    _NOW_32
    push.d  w0                          ; TOS = startTime value
    mov     [w8+flushFuncOffset], w7    ; w7 = flushFunction() ptr
    cp0     w7
    bra     Z, 1f

    ; Call fifo->flushFunction(fifo, &startTime, &timeout);
    mov     w8, w0          ; w0 = *fifo pointer
    sub     w15, #4, w1     ; w1 = *startTime pointer on stack
    sub     w15, #8, w2     ; w2 = *timeout pointer on stack
    call    w7              ; unsigned int (*flushFunction)(EZBL_FIFO *fifo, unsigned long *startTime, unsigned long *timeout); // Pointer to a function to be called anytime the EZBL_FIFOFlush() function is called
done:
    sub     w15, #8, w15    ; Remove startTime and timeout from TOS
    pop     w8
    return

    ; No flushFunction provided, just wait until dataCount == 0 or timeout occurs
1:  mov     #1, w0      ; Speculatively load success return code
    cp0     [w8]
    bra     Z, done
    call    _NOW_32     ; Get timestamp
    mov     [w15-2], w2 ; w3:w2 = startTime
    mov     [w15-4], w3
    sub     w0, w2, w0  ; w1:w0 = NOW_32() - startTime
    subb    w1, w3, w1
    mov     [w15-6], w2 ; w3:w2 = timeout
    mov     [w15-8], w3
    cp      w0, w2      ;(NOW_32() - startTime) - timeout
    cpb     w1, w3
    bra     LE, 1b      ; now - startTime <= timeout ? (note: for infinite timeout, nothing is > 0xFFFFFFFF, so we always loop)
    clr     w0          ; Load timeout fail return code
    bra     done

    .size   _EZBL_FIFOFlush, . - _EZBL_FIFOFlush
    .popsection
