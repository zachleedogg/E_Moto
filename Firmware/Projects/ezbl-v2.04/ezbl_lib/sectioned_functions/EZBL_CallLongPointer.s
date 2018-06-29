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


; void EZBL_CallLongPointer0(unsigned long functionAddress);
; void EZBL_CallLongPointer1(unsigned long functionAddress, unsigned int param1);
; void EZBL_CallLongPointer2(unsigned long functionAddress, unsigned int param1, unsigned int param2);
;
; Calls a long (23-bit) function pointer. For a null pointer, this function does 
; nothing and returns immediately rather than trying to branch to address 
; 0x000000. This is particularly useful when using weak function declarations 
; and you want a function call to disappear when the applicable function is not 
; linked into the project (as opposed to blindly calling a null pointer and 
; instead effectively resetting the device). This function first masks off the 
; top 8 bits of the functionPointer so as to be compatible with 
; __builtin_tblpage() and #tblpage() which may set bit 24 on devices with EDS 
; instead of PSV architecture features (ex: dsPIC33E/PIC24E and some PIC24F 
; products).
;
; Return type is specified as void, but it will actually exactly match the
; called function return type and have the called function's return data.
;
; Function optional parameters correspond to the parameters that the called
; function will receive. They are realigned so that param1 goes to w0, param2 to
; w1, etc. like the called function will normally expect.
    .pushsection    .text.EZBL_CallLongPointer, code
    .global         _EZBL_CallLongPointer
    .global         _EZBL_CallLongPointer0
    .global         _EZBL_CallLongPointer1
    .global         _EZBL_CallLongPointer2
    .type           _EZBL_CallLongPointer, @function
    .type           _EZBL_CallLongPointer0, @function
    .type           _EZBL_CallLongPointer1, @function
    .type           _EZBL_CallLongPointer2, @function
_EZBL_CallLongPointer:
_EZBL_CallLongPointer0:
_EZBL_CallLongPointer1:
_EZBL_CallLongPointer2:
    ; Set up a fake call frame on the stack so we can use the return instruction
    ; to call the function pointer
    ; High Word: SR<7:0>, CORCON<IPL3>, PC<22:16>
    ; Low Word:  PC<15:1>, SR<SFA>
    and     #0xFF, w1
    ior     w0, w1, [w15]
    bra     Z, 1f

    push.d  w0  ; Function pointer address; will be used by "return" to call the pointer

    ; Copy function parameters to the correct locations
    mov.d   w2, w0
    mov.d   w4, w2
    mov.d   w6, w4

    ; "Call" the function pointer. On their return, we will
    ; actually branch back to the function that called
    ; EZBL_CallLongPointer in the first place, not the 1: return line
    ; below that issues the call in the first place.
1:  return
    .size           _EZBL_CallLongPointer,  . - _EZBL_CallLongPointer
    .size           _EZBL_CallLongPointer0, . - _EZBL_CallLongPointer0
    .size           _EZBL_CallLongPointer1, . - _EZBL_CallLongPointer1
    .size           _EZBL_CallLongPointer2, . - _EZBL_CallLongPointer2
    .popsection
    