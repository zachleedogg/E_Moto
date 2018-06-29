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
 * void EZBL_RAMSet(void *dest, int value, unsigned int length);
 *
 * Sets RAM efficiently, just like memset() but without dependence on a C
 * library that could go in application space or get optimized to inline code.
 * On the dsPIC30, PIC24F, PIC24H and dsPIC33F device families, 'length' is
 * limited to 14-bits + 1, or in other words, must be in the set of: {0..16,384}.
 *
 * @param *dest RAM pointer to write the repeating byte value to. Address can be 
 *              byte aligned. This pointer cannot be NULL (0x0000) except when 
 *              <i>length</i> is 0.
 * @param value 8-bit character to write to *dest. Upper bits are ignored.
 * @param length Number of bytes to write. This value must be <= 16384 on 
 *               dsPIC30, PIC24F, PIC24H, or dsPIC33F families. 0 is allowed, 
 *               resulting in no operation.
 */
	.pushsection    .text.EZBL_RAMSet, code
	.global         _EZBL_RAMSet
    .type           _EZBL_RAMSet, @function
_EZBL_RAMSet:
	cp0     w2
	bra     Z, 1f
	dec     w2, w2
	repeat	w2
	mov.b	w1, [w0++]
1:	return
    .size   _EZBL_RAMSet, . - _EZBL_RAMSet
    .popsection
