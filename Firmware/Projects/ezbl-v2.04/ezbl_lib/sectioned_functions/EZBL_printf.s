;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Source File
;
;  Summary:
;    Assembly language optimized helper routines required by the EZBL core API
;
;  Description:
;    printf() semi-equivalent function, but faster and smaller than the full 
;    C stdlib library implementation containing floating point support.

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
 * EZBL's printf() implementation (actually _printf_cdnopuxX() for no floating point support)
 */
	.pushsection    .text.EZBL_printf, code
	.global         _EZBL_printf
	.global         _EZBL_vprintf
    .global         _EZBL_sprintf
    .global         _EZBL_vsprintf
    .type           _EZBL_printf, @function
    .type           _EZBL_vprintf, @function
    .type           _EZBL_sprintf, @function
    .type           _EZBL_vsprintf, @function
    ;.weak           _printf
    ;.weak           _vprintf
    ;.weak           _sprintf
    ;.weak           _vsprintf
    .weak           _write               ; For writing to stdout
    .extern         _EZBL_FIFOWrite
    .weak           _EZBL_STDOUT
    .equ            outputBufSize, 64    ; Number of bytes to reserve on the stack for printing. Must be an even number greater than the largest non-string field width to ever be printed.

; int EZBL_fprintf(FILE *stream, const char *format, ...);
 ; Variadic function parameters passed on stack with TOS-2 = Return addr [23:16], TOS-4 = Return addr [15:0], TOS-6 = Parameter 1 (format pointer), TOS-8 = first variadic parameter, TOS-10 = second variadic parameter, ...
;_EZBL_fprintf:   ; Output will be printed to *stream via write(), or if unimplemented, EZBL FIFO (with stream casted to an *EZBL_FIFO), or if also unimplemented, thrown away
;_fprintf:
    ;push    w0  ; Save *stream handle

    
; int EZBL_printf(const char *format, ...);
 ; Variadic function parameters passed on stack with TOS-2 = Return addr [23:16], TOS-4 = Return addr [15:0], TOS-6 = Parameter 1 (format pointer), TOS-8 = first variadic parameter, TOS-10 = second variadic parameter, ...
_EZBL_printf:    ; Output will be printed to stdout via write(), or if unimplemented, EZBL_STDOUT FIFO, or if also unimplemented, thrown away
;_printf:
    sub     w15, #6, w1         ; w1 = Temp pointer to stack parameters to consume as we find escape sequences (Points to parameter 1 right now)
    mov     [w1--], w0          ; w0 = Temp const char *format string ptr

; int EZBL_vprintf(const char *format, va_list args);
; Variadic function parameters passed by pointer with w0 = format const pointer, w1 = args pointer
_EZBL_vprintf:   ; Output will be printed to stdout via write(), or if unimplemented, EZBL_STDOUT FIFO, or if also unimplemented, thrown away
;_vprintf:
    mov     #outputBufSize, w3  ; w3 = Number of bytes of buffer space allocated on the stack for temporary buffering
    mov     w1, w2              ; w2 = Temp pointer to stack parameters to consume as we find escape sequences (Points to parameter 1 right now)
    mov     w0, w1              ; w1 = Temp const char *format string ptr
    add     w15, #12, w0        ; w0 = Temp pointer to output write buffer base address (#12 is space needed for saving w8-w13)
    bra     printfInit

; int EZBL_sprintf(char *outputBuf, const char *format, ...);
; Variadic function parameters passed on stack with w0 = dest pointer, TOS-2 = Return addr [23:16], TOS-4 = Return addr [15:0], TOS-6 = Parameter 2 (format pointer), TOS-8 = first variadic parameter, TOS-10 = second variadic parameter, ...
_EZBL_sprintf:  ; Output printed to w0 pointer (first string output buffer) with null termination
;_sprintf:
    sub     w15, #6, w2         ; w2 = Temp pointer to stack parameters to consume as we find escape sequences (Points to parameter 1 right now)
    
; int EZBL_vsprintf(char *outputBuf, const char *format, va_list args);
; Variadic function parameters passed by pointer with w0 = dest pointer, w1 = format const pointer, w2 = args pointer
_EZBL_vsprintf:   ; Output printed to w0 pointer (first string output buffer) with null termination
;_vsprintf:
    mov     [w2--], w1          ; w1 = temp const char *format string ptr
    mov     #0xFFFE, w3         ; w3 = Number of bytes of buffer space allocated on the stack for temporary buffering
    
printfInit:
    push.d  w8
    push.d  w10
    push.d  w12
    dec     w0, w8              ; w8 = Output writing pointer (points to buffer base address - 1 right now for pre-inc)
    mov     w1, w9              ; w9 = current *format input pointer
    mov.d   w2, w10             ; w10 = pointer to stack va_args list we have yet to consume,
                                ; w11 = buf size allocated on the stack - 2
    mul.uu  w8, #1, w12         ; w12 = base buffer output pointer - 1,
                                ; w13 = num bytes flushed (0x0000 right now)
    add     w3, #2, w3          ; Reserve 2 extra bytes in the output buffer for \n to \r\n conversion that could be needed
    add     w15, w3, w15        ; Allocate temporary buffer on the stack (0 bytes for sprintf()/vsprintf() version)
    bra     StartDecode
    
NeedFlush:
    rcall   FlushOutput

        
; w0 = working reg, w1-w4 = decoded formatting, w10 = input operand read pointer, w8 = output printing pointer, w9 = input format string read pointer, w5 = decoded input operand byte count field, w6-w7 = working registers, w11 = size of temp output buffer on stack, w12 = base output pointer - 1, w10 = variadic parameter input stack pointer, w14 = not used (not saved)
StartDecode:
    sub     w8, w12, w2         ; w2 = (WrPtr) - (BaseBufAddr) = bytes of buffer space used
    sub     w11, w2, w2         ; w2 = bytes of buffer space left - 2 (will be 0xFFFE - bufUsed for sprintf() case)
    mov     #'%', w3            ; w3 = '%', a temporary value for doing single cycle compares for '%' escape sequences
    mul.uu  w4, #0, w4          ; w4, w5 = 0x0000. w4 = a temporary value for doing single cycle mov operations using subr instruction and simultaneously checking for SR.Z
    
    inc2    w11, w0             ; Test if sprintf() version was called
    btss    SR, #SR_Z
    mov     #'\n', w5           ; w5 = '\n'
    
    ; Copy bytes verbatim into the output array until: a) hit null terminator, b) output buffer needs flushing, c) hit '%' substitution character
0:  dec     w2, w2              ; Decrease free space in buffer
    bra     NC, NeedFlush       ; Flush when next write would cause -1 bytes of free space
    subr.b  w4, [w9++], [++w8]  ; Move character and test for '\0' null terminator
    bra     Z, NullFound
    cp.b    w3, [w8]            ; Character == '%' ?
    bra     Z, 1f
    cp.b    w5, [w8]            ; Character == '\n' (printf) or '\0' (sprintf)?
    bra     NZ, 0b
    add.b   w5, #('\r'-'\n'), [w8]  ; Set character = '\r'
    mov.b   w5, [++w8]              ; Set next character = '\n'
    bra     NeedFlush           ; New line might have significance to upper layer protocols, so let's flush this promptly

1:  dec     w8, w8              ; Don't normally want the '%' escape character in the output
    sub     w8, w12, w2         ; w2 = (WrPtr-1) - (BaseBufAddr-1) = bytes of buffer space used
    cp      w2, #8              ; Do a flush if there are at least 8 bytes sitting in the output buffer to maximize asynchronous communications bandwidth on UART/SPI/I2C/USB/etc. This also maximizes the amount of buffer space for the decoded field to be written.
    btsc    SR, #SR_C
    rcall   FlushOutput
    

    .equ    prefixFlag, 0       ; #
    .equ    spacePadFlag, 1     ;  
    .equ    plusFlag, 2         ; +
    .equ    zeroPadFlag, 3      ; 0
    .equ    leftJustifyFlag, 4  ; -
DecodeFlags:   ; flags --> w1
    ; Decode all flags into w1. Can be {-, 0, +, space, #}, none, or combinations of the given flags
    clr     w1
1:  ze      [w9++], w0              ; Read the next character from the format string
    bra     Z, NullFound
    mov     w1, w2                  ; Save a before parsing flags snapshot so we can tell if we found any for this character
    sub.b   #' ', w0                ; ' ' generate a space or signed values that have neither a plus nor a minus sign
    btsc    SR, #SR_Z
    bset    w1, #spacePadFlag
    cp.b    w0, #('#'-' ')          ; '#' prefix 0 on octal, 0x or 0X on hexadecimal, or generate a decimal point and fraction digits that are otherwise suppressed on a floating-point conversion
    btsc    SR, #SR_Z
    bset    w1, #prefixFlag
    cp.b    w0, #('+'-' ')          ; '+' generate a plus sign for positive signed values
    btsc    SR, #SR_Z
    bset    w1, #plusFlag   
    cp.b    w0, #('0'-' ')          ; '0' Use 0 for the pad character instead of space (which is the default)
    btsc    SR, #SR_Z
    bset    w1, #zeroPadFlag
    cp.b    w0, #('-'-' ')          ; '-' left justify the value within a given field width
    btsc    SR, #SR_Z
    bset    w1, #leftJustifyFlag
    cp.b    w1, w2              ; See if we found anything this round and set a flag
    bra     NZ, 1b              ; Yes we did, get another character and try decoding it
    
DecodeFieldWidth:   ; width --> w2
    ; Decode the Field Width into w2
    mul.uu  w2, #0, w2      ; w2, w3 = 0x0000
    clr     w4
    cp.b    w0, #('*'-' ')  ; See if * value was provided, meaning the width is a parameter
    bra     NZ, 1f
    mov     [w10--], w2     ; Yes, get the decoded width directly from the parameter stack
    ze      [w9++], w0      ; Read next character into w0
    bra     Z, NullFound
    sub.b   #' ', w0
    bra     DecodePrecisionDot

1:  cp.b    w0, #('9'-' ')
    bra     GT, DecodePrecisionDot
    cp.b    w0, #('0'-' ')
    bra     LT, DecodePrecisionDot
    sub.b   w0, #('0'-' '), w0
    add.b   w0, w4, w2
    mul.uu  w2, #10, w4         ; Left shift by 10 power if we find another valid width character
    
    ze      [w9++], w0          ; Read next character into w0
    bra     Z, NullFound
    sub.b   #' ', w0
    bra     1b
    
DecodePrecisionDot:
    clr     w4                  ; Preclear w4 for when it isn't specified
    cp.b    w0, #('.'-' ')
    bra     NZ, DecodeOperandSize
    ze      [w9++], w0          ; Read next character into w0
    bra     Z, NullFound
    sub.b   #' ', w0
    
DecodePrecision:    ; precision --> w4
    clr     w6
    cp.b    w0, #('*'-' ')
    bra     NZ, 1f
    mov     [w10--], w4
    ze      [w9++], w0          ; Read next character into w0
    bra     Z, NullFound
    sub.b   #' ', w0
    bra     DecodeOperandSize
    
1:  cp.b    w0, #('9'-' ')
    bra     GT, DecodeOperandSize
    cp.b    w0, #('0'-' ')
    bra     LT, DecodeOperandSize
    sub.b   w0, #('0'-' '), w0
    add.b   w0, w6, w4
    mul.uu  w4, #10, w6         ; Left shift by 10 power if we find another valid width character

    ze      [w9++], w0          ; Read next character into w0
    bra     Z, NullFound
    sub.b   #' ', w0
    bra     1b

DecodeOperandSize:   ; operand size --> w5
    ; Decode input operand size (in bytes) into w5
    add.b   #' ', w0        ; Regenerate original character in w0. DecodeType routine also wants this as an original
    mov     #2, w5
    mov     w0, w6
    xor.b   #'h', w6
    bra     Z, 1f
    xor.b   #('L' ^ 'h'), w6
    bra     Z, 2f
    xor.b   #('l' ^ 'L'), w6
    bra     NZ, DecodeType
    mov     #4, w5
    
    ze      [w9++], w0       ; Read next character into w0
    bra     Z, NullFound
    
    mov     #'l', w6
    cp.b    w0, w6
    bra     NZ, DecodeType
    mov     #(8-6), w5
    
2:  add     #6, w5
1:  ze      [w9++], w0       ; Read next character into w0
    bra     Z, NullFound
        
DecodeType: ; type --> PC branch
    ; Decode the type into a branch to the right printing function. Available types are {d, i, o, u, x, X, e, E, f, g, G, c, s, p, n, %}
    xor.b   #('%'), w0                  ; %
    bra     Z, PrintPercent
    xor.b   #('c' ^ '%'), w0            ; c
    bra     Z, PrintChar
    ; X, x, u, d, i, s, c, f, e, E, g, p, and o have potential padding and alignment requirements, so allocate space on the stack to pre-buffer what we create before writing to the w8 output pointer)
    push    w8
    dec     w15, w8                     ; Allocate space on stack for temporary output, padding, and ordering before writing to the real w8 output buffer
    add     #28, w15
    xor.b   #('X' ^ 'c'), w0            ; X
    bra     Z, PrintHEXUINT
    xor.b   #('x' ^ 'X'), w0            ; x
    bra     Z, PrintHexUINT
    xor.b   #('u' ^ 'x'), w0            ; u
    bra     Z, PrintDecimalUINT
    xor.b   #('d' ^ 'u'), w0            ; d
    bra     Z, PrintDecimalINT
    xor.b   #('i' ^ 'd'), w0            ; i
    bra     Z, PrintDecimalINT
    xor.b   #('p' ^ 'i'), w0            ; p
    bra     Z, PrintHEXUINT
    xor.b   #('o' ^ 'p'), w0            ; o
    bra     Z, PrintHEXUINT
    sub     #28, w15                    ; Don't need special formatting buffer on the stack for s, n, f, e, E, g, G, default right now
    pop     w8
    xor.b   #('s' ^ 'o'), w0            ; s
    bra     Z, PrintString
    xor.b   #('n' ^ 's'), w0            ; n
    bra     Z, PrintNoneWriteByteCount
    xor.b   #('f' ^ 'n'), w0            ; f
    bra     Z, PrintDecimalDouble
    xor.b   #('e' ^ 'f'), w0            ; e
    bra     Z, PrintDecimalDouble
    xor.b   #('E' ^ 'e'), w0            ; E
    bra     Z, PrintDecimalDouble
    xor.b   #('g' ^ 'E'), w0            ; g
    bra     Z, PrintDecimalDouble
    xor.b   #('G' ^ 'g'), w0            ; G
    bra     Z, PrintDecimalDouble    
    bra     PrintDefault                ; Not a legal print value
    

PrintPercent:   ; %%
    inc     w8, w8          ; '%' escape character was aready written to the buffer, so just recover it by incrementing the pointer
    bra     StartDecode     ; Nothing to do since we already wrote the '%' escape character to the buffer at the beginning and never removed it

;;; Alternate code for %d, %i, %u format generation using EZBL_ltoa()/EZBL_itoa()/EZBL_lltoa()/EZBL_uitoa()/EZBL_ulltoa() helper functions
;;; Ths is commented out since the helpers increase the overall code size as compared to the inline versions in this function when not already using the helpers elsewhere in the project.
;PrintDecimalINT:    ; %d and %i
;    cp      w5, #2
;    bra     NZ, 1f
;    mov     [w10--], w0
;    inc     w8, w1
;    call    _EZBL_itoa
;    add     w8, w0, w8
;    bra     StartDecode
;    
;1:  cp      w5, #4
;    bra     NZ, 1f
;    mov     [w10--], w1
;    mov     [w10--], w0
;    inc     w8, w2
;    call    _EZBL_ltoa
;    add     w8, w0, w8
;    bra     StartDecode
;
;1:  cp      w5, #8
;    bra     NZ, _PrintDecimalINT    
;
;    push    w1
;    push.d  w4  
;    mov     [w10--], w3
;    mov     [w10--], w2
;    mov     [w10--], w1
;    mov     [w10--], w0
;    inc     w8, w4
;    call    _EZBL_lltoa
;    add     w8, w0, w8
;    pop.d   w4
;    pop     w1
;    bra     StartDecode
;    
;PrintDecimalUINT:  ; %u
;    cp      w5, #2
;    bra     NZ, 1f
;    mov     [w10--], w0
;    inc     w8, w1
;    call    _EZBL_uitoa
;    add     w8, w0, w8
;    bra     StartDecode
;
;1:  cp      w5, #4
;    bra     NZ, 1f
;    mov     [w10--], w1
;    mov     [w10--], w0
;    inc     w8, w2
;    call    _EZBL_ultoa
;    add     w8, w0, w8
;    bra     StartDecode
;
;1:  cp      w5, #8
;    bra     NZ, _PrintDecimalUINT
;    
;    push    w1
;    push.d  w4
;    mov     [w10--], w3
;    mov     [w10--], w2
;    mov     [w10--], w1
;    mov     [w10--], w0
;    inc     w8, w4
;    call    _EZBL_ulltoa
;    add     w8, w0, w8
;    pop.d   w4
;    pop     w1
;    bra     StartDecode

    
PrintDecimalINT:    ; %d and %i
    btss    [w10], #15      ; Test MSbit
    bra     2f              ; Number is positive
    
    ; Number is negative, so print a '-', 2's complement the whole number, then print it as a positive number with the MSbit cleared
    mov     #'-', w7        ; Print '-' sign
    mov.b   w7, [++w8]

    mov     #1, w0          ; w0  = +1 carry holder
    sub     w10, w5, w7     ; w7  = stack input operand pointer, repositioned to the LSbytes
1:  com     [++w7], [w7]
    add     w0, [w7], [w7]  ; Add carry and initial +1 for 2's complement
    mov     #0, w0
    rlc     w0, w0          ; Save carry out
    cp      w10, w7
    bra     NZ, 1b
    bra     PrintDecimalUINT 

    ; Number is positive
2:  mov     #'+', w7
    btsc    w1, #plusFlag
    mov.b   w7, [++w8]      ; Write the + sign
    mov     #' ', w7
    btsc    w1, #spacePadFlag
    mov.b   w7, [++w8]      ; Write the space padding character

PrintDecimalUINT:   ; %u    ; w0,w3 free, w1,2,5 used, w4 precision isn't needed for this format?, w6-w7 free
    push    w2              ; Save decoded field width
    push    w1              ; Save decoded formatting Flags
    mov     #1, w2          ; w2 = 0x0001 post-ouput address decrement flag to supress leading zeros
    clr     w1              ; Clear upper 16 bits if doing only a small 16/16-bit divide
    mov     [w10--], w0     ; Get 16 MSbits
    mov     #'0', w3        ; w3 = '0' to convert BCD to printable ASCII
    mov     #5, w4          ; 5 decimal digits going to be generated for a 16-bit SmallDivide number, also 5 loop iterations needed for 32-bit divides
    sub     w5, #2, w5      ; w5 = input data len - 2 already consumed
    bra     Z, SmallDivide
    mov     [w10], w1       ; Gets 16 LSbits
    exch    w1, w0          ; Need little endian byte/word order
    sub     w10, w5, w10    ; Throw away upper 32 bits of 64-bit values since we only have code for decoding 32-bit decimal numbers

    mov     #(100000 & 0xFFFF), w6
    mov     #(100000>>16), w7
    push.d  w6
    mov     #(1000000 & 0xFFFF), w6
    mov     #(1000000>>16), w7
    push.d  w6
    mov     #(10000000 & 0xFFFF), w6
    mov     #(10000000>>16), w7
    push.d  w6
    mov     #(100000000 & 0xFFFF), w6
    mov     #(100000000>>16), w7
    push.d  w6
    mov     #(1000000000 & 0xFFFF), w6
    mov     #(1000000000>>16), w7
    btsc    w15, #0         ; Skip next pop.d instruction
    
0:  pop.d   w6              ; Start a digit
    dec.b   w3, [++w8]      ; '0' (- 1 since we will overflow iterative add to it)
    
1:  inc.b   [w8], [w8]      ; Iteratively subtract up to 10 times
    sub     w0, w6, w0
    subb    w1, w7, w1
    bra     C, 1b
    
    add     w0, w6, w0      ; Repair underflow
    addc    w1, w7, w1
    cp.b    w3, [w8]        ; See if this was a zero digit, and if so, have we begun printing already?
    btss    SR, #SR_Z
    clr     w2
    sub     w8, w2, w8      
    dec     w4, w4          ; Done with digits > 10K?
    bra     NZ, 0b
    
    mov     #10, w4         ; 10 decimal digits generated for a 32-bit number
    
SmallDivide:
    ; Push next set of digits and use faster hardware 32/16 divider
    mov     #10, w6
    mov     #100, w7
    push.d  w6              ; Push #10, #100
    mov     #1000, w6
    push    w6              ; Push #1000
    mov     #10000, w6
    btsc    w15, #0         ; Always skip next instruction
    
1:  pop     w6
    repeat  #DIV_CYCLES
    div.ud  w0, w6          ; 32-bit/16-bit unsigned integer divide
    add.b   w3, w0, [++w8]  ; Write dividend with '0' added
    cp0     w0              ; See if this was a zero digit, and if so, have we begun printing already?
    btss    SR, #SR_Z
    clr     w2
    sub     w8, w2, w8      
    mul.uu  w1, #1, w0      ; Move w1 remainder to w0 numerator and clear w1 numerator high
    cp      w6, #10
    bra     NZ, 1b
    add.b   w3, w0, [++w8]  ; Write 1's place digit

    pop     w1                      ; Restore w1 = decoded Flags
    bra     PrunePadToWidth
    
    
PrintHexUINT:   ; %x
    mov     #('a' -'9'-1), w7       ; a-f offset after '0'
    btsc    w15, #0                 ; w15 stack pointer is always even, so this always skips the next instruction. Needed to nullify the first flagging opcode in PrintHEXUINT.
PrintHEXUINT:   ; %X, %p, and %o
    mov     #('A' -'9'-1), w7       ; A-F offset after '0'
    push    w2
    mov     #1, w2
    mov     #'0', w3                ; BCD to ASCII conversion char for 0-9
    btst    w1, #prefixFlag
    bra     Z, 1f
    mov.b   w3, [++w8]
    mov     #'x', w0                ; Add the 'x' or 'X' prefix character when '#' flag is used to prefix with 0x and 0X sequence
    add.b   w7, w0, [++w8]
1:  sl      w5, #1, w5              ; 2 nibles to print per byte we will be reading
    mov     w5, w4                  ; w4 = save nibble count so we can pad/prune to the w2 field width value
3:  mov     [w10--], w0             ; Get 16 MSbits
    swap.b  w0                      ; Nibble endianness swap
    swap    w0
    swap.b  w0
    
1:  and     w0, #0xF, w6
    lsr     w0, #4, w0
    cp      w6, #10
    btsc    SR, #SR_C
    add.b   w6, w7, w6              ; A-F (or a-f) range
    add.b   w6, w3, [++w8]          ; 0-9 range and write to dest
    cp.b    w3, [w8]                ; See if this was a zero digit, and if so, have we begun printing already?
    btss    SR, #SR_Z
    clr     w2
    sub     w8, w2, w8
    dec     w5, w5                  ; Consume one nibble count
    bra     Z, PrunePadToWidth
    and     w5, #0x3, w6            ; See if we've processed 4 nibbles
    bra     NZ, 1b                  ; Have at least one more nibble for this input data
    bra     3b                      ; Need next 16-bits of input data
    
PrunePadToWidth:    ; Need TOS = field width, TOS-30 temporary buffer where we generated chars already, TOS-32 pointer to the final desitnation address with padding and justification, w8 = temp output buffer write pointer, w1 = justification/padding/formatting char flags
    dec     [--w15], w2             ; Restore w2 = decoded field width-1
    sub     w15, #28, w0            ; w0 = base address of our temporary buffer containing outputted important chars
    sub     w8, w0, w4              ; w4 = count of important chars generated-1
    btss    SR, #SR_C               ; In case if we outputted 0 characters, we will underflow, so add 1 to recover the last one
    inc     w4, w4
    mov     [w15-30], w8            ; w8 = final destination address to write our padded important chars (-1) to
    sub     w2, w4, w5              ; w5 = (FieldWidth-1(w2)) - (importantChars-1(w4)) = Pad chars needed (can be negative, meaning how many leading dummy characters we should remove, assuming they are dummies)
    bra     GT, 1f

    bclr    w1, #leftJustifyFlag    ; Just want to copy, so flag to skip gen pad on right
    bra     2f
    
; Generate some padding chars
1:  dec     w5, w2
    mov     #' ', w7
    btst    w1, #leftJustifyFlag
    bra     NZ, 2f
    btst    w1, #zeroPadFlag
    bra     Z, 3f
    mov     #'0', w7

    mov     #'-', w6    ; Handle +/- sign with left side '0' padding (sign should be first char, then zeros)
    cp.b    w6, [w0]
    bra     Z, 1f
    mov     #'+', w6
    cp.b    w6, [w0]
    bra     NZ, 3f
1:  mov.b   [w0++], [++w8]
    dec     w4, w4
3:  
    repeat  w2                      ; Gen pad on left
    mov.b   w7, [++w8]
2:  repeat  w4                      ; Copy from temporary stack buffer to the the final location and remove stack buffer
    mov.b   [w0++], [++w8]
    sub     w15, #30, w15
    btst    w1, #leftJustifyFlag
    bra     Z, StartDecode
    repeat  w2                      ; Gen pad on right
    mov.b   w7, [++w8]
    bra     StartDecode

    
PrintStringFlushNeeded:
    push.d  w0  ; w0 = input string ptr, w1 = flags
    push    w2  ; w2 = field width for padding gen
    push    w4  ; w4 = precision for max print chars
    rcall   FlushOutput
    pop     w4
    pop     w2
    pop.d   w0
    bra     1f
PrintString:    ; %s            ; w11 = bytes of temp free buffer space on stack for output available - 2
    cp0     w4                  ; If the precision was unspecified, then 0x0000 was recorded and we should use an infinite character limit
    btsc    SR, #SR_Z
    mov     #0xFFFF, w4

    mov     [w10--], w0         ; Get string pointer
1:  mul.uu  w6, #0, w6          ; w6, w7 = 0x0000
    inc2    w11, [w15]          ; sprintf() called?
    btss    SR, #SR_Z
    mov     #'\n', w6           ; No, generate \r\n for every \n generated
2:  sub     w8, w12, w3         ; w3 = (WrPtr) - (BaseBufAddr) = bytes of buffer space used
    sub     w11, w3, w3         ; w3 = buffer space free - 2
0:  dec     w3, w3
    bra     NC, PrintStringFlushNeeded
;    btst    w1, #leftJustifyFlag
;    bra     NZ, 1f
;    dec     w2, w2              ; See if we need to generate any padding
;    bra     N, 1f
;    mov     #' ', w5
;    btsc    w1, #zeroPadFlag
;    mov     #'0', w5
;    mov.b   w5, [++w8]
;    bra     2f
    ;rcall   GenerateStringPad   ; TODO: Generate left-padding as needed to fill the w2 width before printing the actual string
    dec     w2, w2
    subr.b  w7, [w0++], [++w8]
    bra     Z, 1f
2:  dec     w4, w4              ; Abort outputting if the precision field limit has been reached
    bra     Z, StartDecode
    cp.b    w6, [w8]            ; Character == '\n' (printf) or '\0' (sprintf)?
    bra     NZ, 0b
    add.b   w6, #('\r'-'\n'), [w8]  ; Set character = '\r'
    dec     w4, w4              ; Abort outputting if the precision field limit has been reached
    bra     Z, StartDecode
    mov.b   w6, [++w8]          ; Set next character = '\n'
    bra     2b
1:  dec     w8, w8              ; Remove null terminator that was copied

    btss    w1, #leftJustifyFlag; See if we need to generate any right-side padding
    bra     StartDecode
    inc     w2, w2              ; Null teminator write sucks a byte away, even though we erase it, so undo this
    bra     LE, StartDecode
    mov     #' ', w5
    btsc    w1, #zeroPadFlag
    mov     #'0', w5
    mov     #WREG5, w0
    bra     0b

    
PrintChar:  ; %c
    mov     [w10--], w0         ; Read the word from the stack, which can't be byte accessed
    mov.b   w0, [++w8]          ; Save low 8-bit char to the output
    swap    w0
    dec2    w5, w5
    btss    SR, #SR_Z
    mov.b   w0, [++w8]          ; Save high 8-bit char to the output for a 16-bit wide char
    bra     StartDecode


PrintNoneWriteByteCount:    ; %n
    mov     [w10--], w0     ; w0 = output ptr
    sub     w8, w12, w1     ; w1 = (WrPtr-1) - (BaseBufAddr-1) = bytes of buffer space used in the output buffer right now
    add     w13, w1, [w0++] ; Write count of flushed + currently used buffer space to provided pointer
1:  dec2    w5, w5
    bra     Z, StartDecode
    clr     [w0++]          ; We only support up to 65535 bytes written. Beyond this, 0x0000 filler will be written to long and long long output pointers.
    bra     1b


PrintDecimalDouble: ; %f, %e, %E, %g, or %G
    mov     #0x255B, w0             ;[%unimpl] --> "[%       "
    mov     #0x6E75, w1             ;[%unimpl] --> "  un     "
    mov     #0x6D69, w2             ;[%unimpl] --> "    im   "
    mov     #0x6C70, w3             ;[%unimpl] --> "      pl "
    bra     1f
PrintDefault:
    mov     #0x625B, w0             ;[bad fmt] --> "[b       "
    mov     #0x6461, w1             ;[bad fmt] --> "  ad     "
    mov     #0x6620, w2             ;[bad fmt] --> "     f   "
    mov     #0x746D, w3             ;[bad fmt] --> "      mt "
1:  mov     #0x005D, w4             ;[bad fmt] --> "        ]"
    mov     #WREG0, w5
    repeat  #8
    mov.b   [w5++], [++w8]
    bra     StartDecode

NullFound:
    inc2    w11, w0             ; Remove the final null terminator if EZBL_printf() called (keep for EZBL_sprintf()).
    bra     Z, 1f
    dec     w8, w8
    rcall   FlushOutput
    inc2    w11, w11            ; Undo the -2 offset for \n to \r\n conversion or sprintf flagging
    sub     w15, w11, w15       ; Remove our temporary buffer + 2 from the stack
1:  sub     w8, w12, w0         ; w0 = count of unflushed bytes written, computed from WrPtr-1 pointer difference
    add     w13, w0, w0         ; w0 = return total number of bytes written, including flushed ones
    pop.d   w12
    pop.d   w10
    pop.d   w8
    return

    
FlushOutput:    ; w8 = last printed character, w9 = format string ptr, w13 = bytes previously flushed, w11 = temporary buffer size/function variant flag, w12 = base buffer pointer - 1, w10 = pointer to printf stack arguments (not important for this function)
    inc2    w11, [w15]          ; Test if sprintf() version was called, where there is no temporary buffer on the stack, and no stdout to write to
    bra     Z, 1f

    inc     w12, w1             ; w1 = Write data base address
    sub     w8, w12, w2         ; w2 = count of bytes to write, computed from WrPtr-1 pointer difference
    add     w13, w2, w13        ; w13 += flush bytes (to keep track for %n escape sequence)
    mov     w12, w8             ; Get the base output buffer pointer - 1 since we are flushing it all

    mov     #_EZBL_STDOUT, w0   ; Check for EZBL_STDOUT pointer presense and if present, if the pointer is not null.
    mov     [w0], w0            ; w0 = *_EZBL_STDOUT (note: EZBL_STDOUT declared weak, so this may be a mov [0], w0, which is w0 = w0 or a nop equivalent)
    cp0     w0
    btss    SR, #SR_Z
    goto    _EZBL_FIFOWrite     ; goto instead of call used so that when _EZBL_FIFOWrite returns, execution will return to our caller and not us. We have nothing on the stack so can do this.

    mov     #1, w0              ; 0 = stdin (probably), 1 = stdout, 2 = stderr (probably)
    mov     #handle(_write), w3 ; Check for int write(int handle, void *buffer, unsigned int len) user callback implementation
    cp0     w3
    btss    SR, #SR_Z
    goto    w3                  ; goto instead of call used so that when _write returns, execution will return to our caller and not us. We have nothing on the stack so can do this.    
    
1:  return


    ; NOT TESTED, NOT LIKELY TO WORK, NEEDS WORK so commented out
;GenerateStringPad:  ; w0 = string start pointer, w1 = flags, w2 = width, w4 = precision max output string length limit (0xFFFF if no limit)
;    cp0     w2          ; Do early test for 0 width, which is most common and negates any need to find the string length
;    btsc    SR, #SR_Z
;    return
;    push.d  w0
;    push    w3
;    push    w4
;    mov     ' '<<8, w3
;    btsc    w1, #zeroPadFlag
;    mov     '0'<<8, w3
;0:  subr.b  w3, [w0++], [w15]   ; Find null terminator to determine how many pad chars are needed
;    bra     NZ, 0b
;    dec     w0, w0
;    mov     [w15-8], w1 ; w1 = original string start pointer
;    sub     w1, w0, w1  ; w1 = strlen(string)
;    sub     w4, w1, w4  ; PrecisionMaxSize - strlen(string)
;    bra     LE, 8f
;    sub     w2, w1, w1  ; w1 = width - strlen(string)
;    bra     LE, 8f
;    swap    w3, w3
;
;1:  dec     w4, w4      ; PrecisionMaxSize space available?
;    bra     NC, 8f
;
;    push.d  w0
;    push.d  w2
;    push.d  w4
;    push.d  w6
;    dec     w11, w11
;    btsc    SR, #SR_Z
;    rcall   FlushOutput
;    pop.d  w0
;    pop.d  w2
;    pop.d  w4
;    pop.d  w6
;
;    mov.b   w3, [w8++]  ; Generate a padding character to the output
;    dec     w1, w1      ; w1 = padding chars needed
;    bra     NZ, 1b
;
;8:  pop     w4
;    pop     w3
;    pop.d   w0
;    return

    .size   _EZBL_printf, . - _EZBL_printf
    .popsection
