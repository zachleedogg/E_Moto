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

    .equ    _SPIxSTAT_SPIRBF_POSITION, 0x0  ; Receive Buffer Full
    .equ    _SPIxSTAT_SPITBF_POSITION, 0x1  ; Transmit Buffer Full
    .equ    _SPIxSTAT_SPITBE_POSITION, 0x3  ; Transmit Buffer Empty
    .equ    _SPIxSTAT_SPIRBE_POSITION, 0x5  ; Receive Buffer Empty


/**
 * Writes an array of bytes to an SPI peripheral (SPIxBUF register), optionally
 * returning all data shifted in during the same transfer. Data is shifted out
 * as fast as possible (normally back-to-back operation is achieved) with the
 * function blocking until all specified bytes make it completely out of the
 * device.
 *
 * @param SPIInstanceNum SPI hardware peripheral number to perform the exchange
 *                       on. Set to 1 for SPI1, 2 for SPI2, 3 for SPI3, etc. Do
 *                       not specify an unimplemented peripheral number as this
 *                       function treats this number like a pointer (i.e.
 *                       incorrect memory locations will be read/written if
 *                       SPIInstanceNum is invalid).
 *
 * @param *rxDataDest Pointer to RAM to write bytes read from the SPIxBUF
 *                    register. This pointer can match *txDataSrc to write the
 *                    RX data to the same buffer used for the the TX data or can
 *                    be set to null if no RX data should be saved (SPIxBUF will
 *                    still be popped a correct number of times to ensure SPIROV
 *                    doesn't become set and no extra data is left in the
 *                    SPIxBUF path).
 *
 * @param *txDataSrc Pointer to array of bytes to write to the SPIxBUF register.
 *
 * @param byteLength Number of bytes to read and write.
 *
 * @return Pointers are read/written by exactly byteLength elements and SPIxBUF
 *         is written/read by exactly byteLength elements.
 *
 *         The SPI interrupt flag(s) are not accessed (read or written).
 */
;void EZBL_SPIExchange(unsigned int SPIInstanceNum, void *rxDataDest, void *txDataSrc, unsigned int byteLength);
    .pushsection    .text.EZBL_SPIExchange, code
    .global         _EZBL_SPIExchange
    .global         _EZBL_SPIWrite
    .type           _EZBL_SPIExchange, @function
    .type           _EZBL_SPIWrite, @function
    .weak           _SPI1STAT
    .weak           _SPI1BUF
    .weak           _SPI1STATL
    .weak           _SPI1BUFL
    .weak           _SPI2STAT
    .weak           _SPI2STATL
_EZBL_SPIWrite:         ;void EZBL_SPIWrite(unsigned int SPIInstanceNum, void *txDataSrc, unsigned int byteLength);
    mov         w2, w3
    mov         w1, w2
    clr         w1
_EZBL_SPIExchange:      ;void EZBL_SPIExchange(unsigned int SPIInstanceNum, void *rxDataDest, void *txDataSrc, unsigned int byteLength);
    cp0         w3              ; Terminate immediately if byteLength == 0
    bra         Z, done
    cp0         w1              ; Change *rxDataDest pointer to stack TOS if null to throw the RX data away
    btsc        SR, #SR_Z
    mov         w15, w1
    mov         #_SPI1STAT, w4
    mov         #_SPI1STATL, w5
    add         w4, w5, w4
    mov         #_SPI1BUF, w5
    mov         #_SPI1BUFL, w6
    add         w5, w6, w5
    mov         #_SPI2STAT, w7
    mov         #_SPI2STATL, w6
    add         w7, w6, w7
    bra         Z, 0f
    dec         w0, w0          ; w0 = SPI peripheral number, rebased so 0 = SPI1, 1 = SPI2, 2 = SPI3, etc.
    sub         w7, w4, w7      ; w7 temp = &SPI2STAT - &SPI1STAT
    mul.uu      w7, w0, w6      ; w6 temp = address offset to this SPIxSTAT/SPIxBUF from SPI1STAT/SPI1BUF
    add         w4, w6, w4      ; w4 = &SPIxSTAT
    add         w5, w6, w5      ; w5 = &SPIxBUF
0:  add         w1, w3, w6      ; w6 = terminate *rxDataDest pointer position
    add         w2, w3, w7      ; w7 = terminate *txDataSrc pointer position
    ; w0 = SPIInstanceNum (temp), then SPIxSTAT value
    ; w1 = *rxDataDest
    ; w2 = *txDataSrc
    ; w3 = byteLen (temp), then free
    ; w4 = &SPIxSTAT
    ; w5 = &SPIxBUF
    ; w6 = terminate *rxDataDest pointer position
    ; w7 = terminate *txDataSrc pointer position

    ; Flush anything sitting in RX FIFO
0:  mov         [w5], w3
    btss        [w4], #_SPIxSTAT_SPIRBE_POSITION
    bra         0b
  

tryXferByte:
    btss        [w4], #_SPIxSTAT_SPITBF_POSITION
    mov.b       [w2++], [w5]                        ; SPIxBUF = *txDataSrc++;
    btss        [w4], #_SPIxSTAT_SPIRBE_POSITION
    mov.b       [w5], [w1++]                        ; *rxDataDest++ = SPIxBUF;
1:  cp          w7, w2                              ; if(txDataSrc == txDataSrc_original + byteLength)
    bra         NZ, tryXferByte
  
finalRX:
    btss        [w4], #_SPIxSTAT_SPIRBE_POSITION
    mov.b       [w5], [w1++]                        ; *rxDataDest++ = SPIxBUF;
    cp          w6, w1                              ; if(rxDataDest == rxDataDest_original + byteLength)
    bra         NZ, finalRX

done:
    retlw       #0, w0

    .size           _EZBL_SPIExchange, . - _EZBL_SPIExchange
    .popsection
