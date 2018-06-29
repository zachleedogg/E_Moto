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
 * Multiplies a 16-bit unsigned integer number by 3/2. Normally, this is needed 
 * when converting a Flash address into a contiguous byte offset. Each Flash 
 * address increment is normally '2', which represents '3' data bytes.
 * Requires ~10 instruction cycles, not including the call, but including the
 * return.
 * 
 * @param addrToByteNumber unsigned 16-bit integer to be multiplied by 3/2. If
 *                         your input may be longer than 16-bits, use the
 *                         EZBL_Mul3Div2L() function instead.
 *
 * @return unsigned 32-bit long integer of addrToByteNumber multiplied by 3 and 
 *         divided by 2. If an odd number is provided as input, the result 
 *         has the fractional 0.5 return portion truncated off. Ex: 
 *         1*(3/2) = 1, 3*(3/2) = 4, 5*(3/2) = 7, etc. Even numbered inputs 
 *         always result in perfect precision since they are always integer 
 *         divisible by 2 without data loss.
 */
; unsigned long EZBL_Mul3Div2(unsigned int addrToByteNumber);
    .pushsection    .text.EZBL_Mul3Div2, code
    .global         _EZBL_Mul3Div2
    .type           _EZBL_Mul3Div2, @function
_EZBL_Mul3Div2:
    mul.uu  w0, #3, w0
    lsr     w1, w1      ; Divide by 2, and shift bit 16 into C
    rrc     w0, w0      ; Divide low 16 bits by 2 and capture C
    return
    
    .size           _EZBL_Mul3Div2, . - _EZBL_Mul3Div2
    .popsection
