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
 * Repeatedly writes a single character to an SPI peripheral (SPIxBUF register),
 * returning all data shifted in during the same transfer. Data is shifted in
 * as fast as possible (normally back-to-back operation is achieved) with the
 * function blocking until all specified bytes are read.
 *
 * @param SPIInstanceNum SPI hardware peripheral number to perform the read
 *                       on. Set to 1 for SPI1, 2 for SPI2, 3 for SPI3, etc. Do
 *                       not specify an unimplemented peripheral number as this
 *                       function treats this number like a pointer (i.e.
 *                       incorrect memory locations will be read/written if
 *                       SPIInstanceNum is invalid).
 *
 * @param *rxDataDest Pointer to RAM to write bytes read from the SPIxBUF
 *                    register. This pointer can be set to null if no RX data
 *                    should be saved (i.e. just generates SPI clocks for the
 *                    slave device to process something internally).
 *
 * @param txVal 8-bit character value to transmit in order to generate SCK
 *              clocks to receive data. This character is repeated byteLength
 *              times.
 *
 * @param byteLength Number of bytes to read.
 *
 * @return Pointer is written by exactly byteLength elements (when not null) and
 *         SPIxBUF is written/read by exactly byteLength elements.
 *
 *         The SPI interrupt flag(s) are not accessed (read or written).
 */
    .pushsection    .text.EZBL_SPIRead, code
    .global         _EZBL_SPIRead
    .type           _EZBL_SPIRead, @function
    .extern         _EZBL_SPIExchange
_EZBL_SPIRead:          ;void EZBL_SPIRead(unsigned int SPIInstanceNum, void *rxDataDest, unsigned char txVal, unsigned int byteLength);
    cp0         w3
    btsc        SR, #SR_Z
    retlw       #0, w0

    lnk         #0              ; Save stack pointer
    mov         w15, w5
    btst.c      w3, #0          ; Ensure even length for stack buffer
    addc        w15, w3, w15
    dec         w3, w6
    repeat      w6
    mov.b       w2, [w5++]      ; EZBL_RAMSet(TOS, txVal, byteLength);
    mov         w14, w2         ; w2 = base address of stack array
    call        _EZBL_SPIExchange
    ulnk                        ; Restore stack pointer
    return

    .size           _EZBL_SPIRead, . - _EZBL_SPIRead
    .popsection
