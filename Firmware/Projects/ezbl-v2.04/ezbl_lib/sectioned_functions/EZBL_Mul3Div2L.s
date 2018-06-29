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
 * Multiplies a 32-bit unsigned long number by 3/2. Normally, this is needed 
 * when converting a Flash address offset into a byte count. Each Flash address 
 * increment is normally '2', which represents '3' data bytes.
 *
 * Requires ~10 instruction cycles, not including the call, but including the
 * return. addrToByteNumber > 0xAAAAAAAA (2,863,311,530) will result in overflow 
 * (but the return result will still be correct in modulo 32-bit math; i.e. just 
 * add 0x100000000 if(addrToByteNumber > 0xAAAAAAAA) to get the correct result). 
 * Note: 0xAAAAAAAA is way beyond the 23-bit program address space of the
 * PIC24/dsPIC architecture, so this extreme can be ignored when the input 
 * argument is an actual address.
 *
 * @param addrToByteNumber unsigned 32-bit long integer to be multiplied by 3/2.
 *                         If your input will always be 16-bits or less, the
 *                         EZBL_Mul3Div2() function can be used instead to 
 *                         potentially save a bit of call overhead.
 *
 * @return unsigned 32-bit long integer of addrToByteNumber multiplied by 3 and
 *         divided by 2. If an odd number is provided as input, the result
 *         has the fractional 0.5 return portion truncated off. Ex:
 *         1*(3/2) = 1, 3*(3/2) = 4, 5*(3/2) = 7, etc. Even numbered inputs
 *         always result in perfect precision since they are always integer
 *         divisible by 2 without data loss.
 */
 ; unsigned long EZBL_Mul3Div2L(unsigned long addrToByteNumber);
    .pushsection    .text.EZBL_Mul3Div2L, code
    .global         _EZBL_Mul3Div2L
    .type           _EZBL_Mul3Div2L, @function
_EZBL_Mul3Div2L:
    lsr     w1, w3      ; Divide by 2
    rrc     w0, w2
    add     w0, w2, w0  ; Add to original number to get 1.5*original number
    addc    w1, w3, w1
    return
    .size           _EZBL_Mul3Div2L, . - _EZBL_Mul3Div2L
    .popsection
