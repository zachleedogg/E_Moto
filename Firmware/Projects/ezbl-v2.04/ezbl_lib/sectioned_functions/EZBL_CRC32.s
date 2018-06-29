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
 * Computes or continues a CRC-32 (IEEE 802.3 Ethernet version) over RAM data
 *
 * The polynomial implemented is:
 * x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
 * Commonly this is represented as 0xEDB88320 (when right shifting) or 
 * 0x04C11DB7 (left shifting).
 *
 * @param crcSeed Starting value to use in the shift register. Set to 0x00000000
 *                when starting a new CRC32 computation.
 * @param data Pointer to RAM memory to start reading from. There are no
 *             alignment restrictions.
 * @param byteLength Count of how many bytes to include in this CRC computation
 *                   starting at the specified pointer address.
 *
 * @return 32-bit CRC computed
 */
; unsigned long EZBL_CRC32(unsigned long crcSeed, void *data, unsigned int byteLength);
    .pushsection    .text.EZBL_CRC32, code
    .global         _EZBL_CRC32
    .type           _EZBL_CRC32, @function
_EZBL_CRC32:
    ; Invert CRC since we are adding to it
    com     w0, w0
    com     w1, w1

    ; Make w5 a pointer to w4, so we can do post inc/dec byte moves,
    ; and set up the polynomial XOR value in w6 and w7
    mov     #WREG4, w5
    mov     #0x8320, w6
    mov     #0xEDB8, w7

    ; Goto common word loop check
    bra     3f

    ; w1:w0   wCRC ^= *data++;      ~12 cycles*16 bits      7cy*16bit on PIC24F, PRAM
1:  mov.b   [w2++], [w5++]
    mov.b   [w2++], [w5--]
    xor     w0, w4, w0

    ; Loop 8*2 times to process each bit
    mov     #8, w4

    ; w1:w0   CRC >>= 1;   16*8 cycles best/16 bits  20*8 cycles worst/16 bits      13*8/16 PIC24F worst, 11*8/16 best
    ; Process 2 bits at a time (one loop unrolling done: adds 5 instructions)
2:  lsr     w1, w1
    rrc     w0, w0
    bra     NC, 6f          ; CRC ^= 0xEDB88320 if LSbit carry out is set
    xor     w0, w6, w0
    xor     w1, w7, w1
6:  lsr     w1, w1
    rrc     w0, w0
    bra     NC, 6f          ; CRC ^= 0xEDB88320 if LSbit carry out is set
    xor     w0, w6, w0
    xor     w1, w7, w1
6:  dec     w4, w4
    bra     NZ, 2b

    ; Shift in 16 bits if present
3:  sub     w3, #2, w3
    bra     NN, 1b

    ; Underflow occurred when decrementing bytelength
4:  add     w3, #2, w3
    bra     Z, 5f

    ; Have one last byte, process it too
    xor.b   w0, [w2], w0    ; CRC ^= (char)*data;
    mov     #4, w4          ; Only do 4 loop iterations this time instead of 8 since each iteration processes 2 bits
    mov     #0, w3          ; Adjust bytelength so we will terminate the outer word processing next time label 4b is reached.
    bra     2b

    ; Reinvert seed for return
5:  com     w0, w0
    com     w1, w1
    return
    .size       _EZBL_CRC32, . - _EZBL_CRC32
    .popsection
    
    
 ; Here is the same function in C (no external dependencies) if you want to 
 ; have this same API on a PIC32 or other non-16-bit Microchip processor.
/*
unsigned long EZBL_CRC32(unsigned long crcSeed, void *data, unsigned int byteLength)
{
    unsigned int k;
    unsigned int crcIn;
    unsigned int crcNext;

    crcSeed = ~crcSeed;

    // Read each chunk of the sector from Flash
    while(byteLength--)
    {
        // Shift in 8 bits
        crcIn = (unsigned int)(*(unsigned char*)data++);
        ((unsigned char*)&crcSeed)[0] ^= ((unsigned char*)&crcIn)[0];
        for(k = 0; k < 8u; k++)
        {
            crcNext = (unsigned int)crcSeed;
            crcNext &= 0x01;
            crcSeed >>= 1;
            if(crcNext)
                crcSeed ^= 0xEDB88320;
        }
    }

    return ~crcSeed;
}
*/