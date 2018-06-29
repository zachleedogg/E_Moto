;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Source File
;
;  Summary:
;    Assembly language optimized helper routines distributed with ezbl.
;
;  Description:
;    64-bit, 32-bit, and 16-bit signed and unsigned ASCII conversion routines. 
;    Outputs ASCII decimal values in base 10 radix and no leading zeros.
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
    .macro  skip_next_instr
    btsc    w15, #0     ; Skips next instruction since w15 stack pointer can never be odd
    .endm

    .ifdef  ezbl_lib16ch    ; __GENERIC-16DSP-CH target, but can't test this symbol since it has dashes in it
    .equ    DIV_CYCLES, #5
    .else
    .equ    DIV_CYCLES, #17
    .endif


/**
 * EZBL's decimal to ASCII conversion routines up to 64-bits signed/unsigned
 */
	.pushsection    .text.EZBL_itoa, code
	.global         _EZBL_itoa
    .global         _EZBL_ltoa
	.global         _EZBL_lltoa
	.global         _EZBL_uitoa
    .global         _EZBL_ultoa
    .global         _EZBL_ulltoa
    .type           _EZBL_itoa, @function
    .type           _EZBL_ltoa, @function
    .type           _EZBL_lltoa, @function
    .type           _EZBL_uitoa, @function
    .type           _EZBL_ultoa, @function
    .type           _EZBL_ulltoa, @function

/**
 * Converts an unsigned 16-bit integer into an ASCII decimal (base 10) string 
 * representation. The string is NOT null terminated, but leading zeros are 
 * omitted.
 * 
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9 
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset 
 * value added.
 * 
 * @param i unsigned (up to) 16-bit integer to write in ASCII form.
 * 
 * @param *dest Pointer to a memory buffer to receive the generated ASCII 
 *              characters. This buffer should be at least 5 bytes long when 
 *              converting integers >=10000, 4 bytes for <=9999 and >= 1000, 3 
 *              bytes for >=999 and <=100, etc.
 * 
 *              Characters are written in print form, i.e. most significant 
 *              character first.
 * 
 *              Each buffer byte will be internally written up to 11 times, so 
 *              do not specify a peripheral SFR pointer that will do something 
 *              in hardware when written with arbitrary contents.
 * 
 * @return Number of ASCII characters written to *dest. The value will always be 
 *         >= 1 and <= 5. Values less than 5 indicate the number had leading 
 *         zero digits that were omitted from being written.
 * 
 *         An input of zero is outputted as a single '0' digit.
 */
; unsigned int EZBL_uitoa(unsigned int i, void *dest);
_EZBL_uitoa:
    mov     w1, w2
    clr     w1
    
/**
 * Converts an unsigned 32-bit integer into an ASCII decimal (base 10) string 
 * representation. The string is NOT null terminated, but leading zeros are 
 * omitted.
 * 
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9 
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset 
 * value added.
 * 
 * @param i unsigned (up to) 32-bit integer to write in ASCII form.
 * 
 * @param *dest Pointer to a memory buffer to receive the generated ASCII 
 *              characters. This buffer should be at least 10 bytes long when 
 *              converting integers >= 1 billion, 9 bytes for <=999 million and 
 *              >= 100 million, etc.
 * 
 *              Characters are written in print form, i.e. most significant 
 *              character first.
 * 
 *              Each buffer byte will be internally written up to 11 times, so 
 *              do not specify a peripheral SFR pointer that will do something 
 *              in hardware when written with arbitrary contents.
 * 
 * @return Number of ASCII characters written to *dest. The value will always be 
 *         >= 1 and <= 10. Values less than 10 indicate the number had leading 
 *         zero digits that were omitted from being written.
 * 
 *         An input of zero is outputted as a single '0' digit.
 */
; unsigned int EZBL_ultoa(unsigned long i, void *dest);
_EZBL_ultoa:
    mov     w2, w4
    mul.uu  w2, #0, w2

/**
 * Converts an unsigned 64-bit integer into an ASCII decimal (base 10) string 
 * representation. The string is NOT null terminated, but leading zeros are 
 * omitted.
 * 
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9 
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset 
 * value added.
 * 
 * @param i unsigned (up to) 64-bit integer to write in ASCII form.
 * 
 * @param *dest Pointer to a memory buffer to receive the generated ASCII 
 *              characters. This buffer should be at least 20 bytes long when 
 *              converting integers >= 1*10^19, 19 bytes for < 1*10^19, 18 bytes 
 *              for < 1*10^18, etc.
 * 
 *              Characters are written in print form, i.e. most significant 
 *              character first.
 * 
 *              Each buffer byte will be internally written up to 11 times, so 
 *              do not specify a peripheral SFR pointer that will do something 
 *              in hardware when written with arbitrary contents.
 * 
 * @return Number of ASCII characters written to *dest. The value will always be 
 *         >= 1 and <= 20. Values less than 20 indicate the number had leading 
 *         zero digits that were omitted from being written. 
 * 
 *         An input of zero is outputted as a single '0' digit.
 */
; unsigned int EZBL_ulltoa(unsigned long long i, void *dest);
_EZBL_ulltoa:
    clr     w5
    bra     lltoa

/**
 * Converts a signed 16-bit integer into an ASCII decimal (base 10) string 
 * representation. The string is NOT null terminated, but leading zeros are 
 * omitted.
 * 
 * For negative numbers, a leading '-' negative sign is written. Zero and 
 * positive values do not have a leading sign character applied.
 * 
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9 
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset 
 * value added. 
 * 
 * @param i signed (up to) 16-bit integer to write in ASCII form.
 * 
 * @param *dest Pointer to a memory buffer to receive the generated ASCII 
 *              characters. This buffer should be at least 6 bytes long when 
 *              converting integers <= -10000, 5 bytes for >= -9999 or >= 10000, 
 *              4 bytes for >= -999 or >= 1000, etc.
 * 
 *              Characters are written in print form, i.e. sign (only if 
 *              negative), followed by most significant character first.
 * 
 *              Each buffer byte will be internally written up to 11 times, so 
 *              do not specify a peripheral SFR pointer that will do something 
 *              in hardware when written with arbitrary contents.
 * 
 * @return Number of ASCII characters written to *dest, including the negative 
 *         sign if applicable. The value will always be >= 1 and <= 6. Values 
 *         less than 6 indicate the number had leading positive sign and/or zero 
 *         digits that were omitted from being written.
 * 
 *         An input of zero is outputted as a single '0' digit.
 */
; unsigned int EZBL_itoa(int i, void *dest);
_EZBL_itoa:
    mov     w1, w2
    setm    w1
    btss    w0, #15
    clr     w1

/**
 * Converts a signed 32-bit long integer into an ASCII decimal (base 10) 
 * string representation. The string is NOT null terminated, but leading zeros 
 * are omitted.
 * 
 * For negative numbers, a leading '-' negative sign is written. Zero and 
 * positive values do not have a leading sign character applied.
 * 
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9 
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset 
 * value added. 
 * 
 * @param i signed (up to) 32-bit integer to write in ASCII form.
 * 
 * @param *dest Pointer to a memory buffer to receive the generated ASCII 
 *              characters. This buffer should be at least 11 bytes long when 
 *              converting integers <= -1 billion, 10 bytes for > -999 million 
 *              or >= 1 billion, 9 bytes for <= -10 million or >= 100 million, 
 *              etc.
 * 
 *              Characters are written in print form, i.e. sign (only if 
 *              negative), followed by most significant character first.
 * 
 *              Each buffer byte will be internally written up to 11 times, so 
 *              do not specify a peripheral SFR pointer that will do something 
 *              in hardware when written with arbitrary contents.
 * 
 * @return Number of ASCII characters written to *dest, including the negative 
 *         sign if applicable. The value will always be >= 1 and <= 11. Values 
 *         less than 11 indicate the number had leading positive sign and/or 
 *         zero digits that were omitted from being written.
 * 
 *         An input of zero is outputted as a single '0' digit.
 */
; unsigned int EZBL_ltoa(long i, void *dest);
_EZBL_ltoa:
    mov     w2, w4
    setm    w2
    setm    w3
    btss    w1, #15
    mul.uu  w2, #0, w2

/**
 * Converts a signed 64-bit long long integer into an ASCII decimal (base 10) 
 * string representation. The string is NOT null terminated, but leading zeros 
 * are omitted.
 * 
 * For negative numbers, a leading '-' negative sign is written. Zero and 
 * positive values do not have a leading sign character applied.
 * 
 * An ASCII decimal string is essentially Binary Coded Decimal, with each 0-9 
 * digit encoded into an 8-bit field and with the '0' (0x30) character offset 
 * value added.
 * 
 * @param i signed (up to) 64-bit integer to write in ASCII form.
 * 
 * @param *dest Pointer to a memory buffer to receive the generated ASCII 
 *              characters. This buffer should be at least 21 bytes long when 
 *              converting integers <= -1*10^19, 20 bytes for > -1*10^19 or >= 
 *              1*10^19, 19 bytes for > -1*10^18 or 1*10^18, etc.
 * 
 *              Characters are written in print form, i.e. sign (only if 
 *              negative), followed by most significant character first.
 * 
 *              Each buffer byte will be internally written up to 11 times, so 
 *              do not specify a peripheral SFR pointer that will do something 
 *              in hardware when written with arbitrary contents.
 * 
 * @return Number of ASCII characters written to *dest, including the negative 
 *         sign if applicable. The value will always be >= 1 and <= 21. Values 
 *         less than 21 indicate the number had leading positive sign and/or 
 *         zero digits that were omitted from being written.
 * 
 *         An input of zero is outputted as a single '0' digit.
 */
; unsigned int EZBL_lltoa(unsigned long i, void *dest);
_EZBL_lltoa:
    bset    w5, #0

/**
 * Internal common work function with prototype:
 *     unsigned int lltoa(long long i, void *dest, unsigned int flags);
 * flags<0> == 0 means i is unsigned, == 1 means i is 2's complement signed
 * w3:w0 == i, w4 == *dest, w5 == flags
 */
lltoa:  ;unsigned int lltoa(long long i, void *dest, unsigned int flags); w3:w0 == i, w4 == *dest, w5 == flags
    push.d  w8
    push.d  w10
    dec     w4, [w15++]     ; Pre-dec destination and stash on TOS for return character count generation
    dec     w4, w8
    mov     #19, w9
    mov     #'0', w10
    mov     #1, w11
    
    btss    w5, #0          ; Is this an signed conversion or unsigned (default)
    bra     2f
    
    btss    w3, #15         ; Test MSbit
    bra     2f              ; Number is positive
    
    ; Number is negative, so print a '-', 2's complement the whole number, then print it as a positive number with the MSbit cleared
    sub.b   w10, #('0'-'-'), [++w8] ; Print '-' sign

    clr     w6
    sub     w6, w0, w0      ; i = 0 - i;
    subb    w6, w1, w1
    subb    w6, w2, w2
    subb    w6, w3, w3
    
2:  ; Positive number
    ;0x               000A  ; w4:w7 =                         10
    ;0x               0064  ; w4:w7 =                        100
    ;0x               03E8  ; w4:w7 =                      1,000
    ;0x               2710  ; w4:w7 =                     10,000
    ;0x          0001 86A0  ; w4:w7 =                    100,000
    ;0x          000F 4240  ; w4:w7 =                  1,000,000
    ;0x          0098 9680  ; w4:w7 =                 10,000,000
    ;0x          05F5 E100  ; w4:w7 =                100,000,000
    ;0x          3B9A CA00  ; w4:w7 =              1,000,000,000
    ;0x     0002 540B E400  ; w4:w7 =             10,000,000,000
    ;0x     0017 4876 E800  ; w4:w7 =            100,000,000,000
    ;0x     00E8 D4A5 1000  ; w4:w7 =          1,000,000,000,000    
    ;0x     0918 4E72 A000  ; w4:w7 =         10,000,000,000,000
    ;0x     5AF3 107A 4000  ; w4:w7 =        100,000,000,000,000
    ;0x0003 8D7E A4C6 8000  ; w4:w7 =      1,000,000,000,000,000
    ;0x0023 86F2 6FC1 0000  ; w4:w7 =     10,000,000,000,000,000
    ;0x0163 4578 5D8A 0000  ; w4:w7 =    100,000,000,000,000,000
    ;0x0DE0 B6B3 A764 0000  ; w4:w7 =  1,000,000,000,000,000,000
    ;0x8AC7 2304 89E8 0000  ; w4:w7 = 10,000,000,000,000,000,000
    
process64Bit:
    mov     #0x000A, w4 ;0x               000A  ; w4:w7 =                         10
    mov     #0x0000, w5
    push.d  w4
    mul.uu  w4, #10, w4 ;0x               0064  ; w4:w7 =                        100
    push.d  w4
    mul.uu  w4, #10, w4 ;0x               03E8  ; w4:w7 =                      1,000
    push.d  w4
    mul.uu  w4, #10, w4 ;0x               2710  ; w4:w7 =                     10,000
    push.d  w4
    mul.uu  w4, #10, w4 ;0x          0001 86A0  ; w4:w7 =                    100,000
    push.d  w4
    mov     #0x4240, w4 ;0x          000F 4240  ; w4:w7 =                  1,000,000
    mov     #0x000F, w5
    push.d  w4
    mov     #0x9680, w4 ;0x          0098 9680  ; w4:w7 =                 10,000,000  
    mov     #0x0098, w5
    push.d  w4
    mov     #0xE100, w4 ;0x          05F5 E100  ; w4:w7 =                100,000,000
    mov     #0x05F5, w5
    push.d  w4
    mov     #0xCA00, w4 ;0x          3B9A CA00  ; w4:w7 =              1,000,000,000
    mov     #0x3B9A, w5
    push.d  w4
    mov     #0xE400, w4 ;0x     0002 540B E400  ; w4:w7 =             10,000,000,000
    mov     #0x540B, w5
    push.d  w4
    mov     #0x0002, w6
    mov     #0x0000, w7
    push.d  w6
    mov     #0xE800, w4 ;0x     0017 4876 E800  ; w4:w7 =            100,000,000,000
    mov     #0x4876, w5
    push.d  w4
    mov     #0x0017, w6
    ;mov     #0x0000, w7
    push.d  w6
    mov     #0x1000, w4 ;0x     00E8 D4A5 1000  ; w4:w7 =          1,000,000,000,000    
    mov     #0xD4A5, w5
    push.d  w4
    mov     #0x00E8, w6
    ;mov     #0x0000, w7
    push.d  w6
    mov     #0xA000, w4 ;0x     0918 4E72 A000  ; w4:w7 =         10,000,000,000,000
    mov     #0x4E72, w5
    push.d  w4
    mov     #0x0918, w6
    ;mov     #0x0000, w7
    push.d  w6    
    mov     #0x4000, w4 ;0x     5AF3 107A 4000  ; w4:w7 =        100,000,000,000,000
    mov     #0x107A, w5
    push.d  w4
    mov     #0x5AF3, w6
    ;mov     #0x0000, w7
    push.d  w6
    mov     #0x8000, w4 ;  1,000,000,000,000,000
    mov     #0xA4C6, w5
    push.d  w4
    mov     #0x8D7E, w6
    mov     #0x0003, w7
    push.d  w6
    mov     #0x0000, w4 ; 10,000,000,000,000,000
    mov     #0x6FC1, w5
    push.d  w4
    mov     #0x86F2, w6
    mov     #0x0023, w7 
    push.d  w6
    ;mov     #0x0000, w4 ; 100,000,000,000,000,000   
    mov     #0x5D8A, w5
    push.d  w4
    mov     #0x4578, w6
    mov     #0x0163, w7 
    push.d  w6
    ;mov     #0x0000, w4 ; 1,000,000,000,000,000,000
    mov     #0xA764, w5
    push.d  w4
    mov     #0xB6B3, w6
    mov     #0x0DE0, w7 
    push.d  w6
    ;mov     #0x0000, w4 ; 10,000,000,000,000,000,000
    mov     #0x89E8, w5
    mov     #0x2304, w6
    mov     #0x8AC7, w7
    bra     3f
    
2:  pop.d   w6
4:  pop.d   w4
3:  dec.b   w10, [++w8]
1:  inc.b   [w8], [w8]  ; Iteratively add 1 at a time
    sub     w0, w4, w0
    subb    w1, w5, w1
    subb    w2, w6, w2
    subb    w3, w7, w3
    bra     C, 1b
    
    add     w0, w4, w0  ; Correct underflow
    addc    w1, w5, w1
    addc    w2, w6, w2
    addc    w3, w7, w3
    
    cp.b    w10, [w8]   ; Do not keep leading zeros
    btss    SR, #SR_Z
    clr     w11
    sub     w8, w11, w8
    
    dec     w9, w9
    cp      w9, #10
    bra     C, 2b
    mul.uu  w6, #0, w6
    cp0     w9
    bra     NZ, 4b
    
    add.b   w10, w0, [++w8]  ; Final 1's place digit
    
    sub     w8, [--w15], w0  ; Return count of characters generated in output (leading zeros were not printed)
    pop.d   w10
    pop.d   w8
    return

    .popsection
