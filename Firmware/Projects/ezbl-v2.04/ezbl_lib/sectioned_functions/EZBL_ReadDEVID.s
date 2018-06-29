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
 * unsigned long EZBL_ReadDEVID(void);
 *
 * @return Returns the 24-bit contents of the "DEVID" Device ID program
 *         memory location at 0xFF0000. Upper 8-bits returned are always zeros.
 */

/**
 * unsigned long EZBL_ReadDEVREV(void);
 *
 * @return Returns the 24-bit contents of the "DEVREV" Device Revision program
 *         memory location at 0xFF0002. Upper 8-bits returned are always zeros.
 */
    .pushsection    .text.EZBL_ReadDEVREV, code
    .global         _EZBL_ReadDEVID
    .global         _EZBL_ReadDEVREV
    .type           _EZBL_ReadDEVID, @function
    .type           _EZBL_ReadDEVREV, @function
    .extern         TBLPAG
_EZBL_ReadDEVID:
    mov         #0x0000, w0
    btsc        w0, #0      ; Execute next instruction as a NOP
_EZBL_ReadDEVREV:
    mov         #0x0002, w0
    mov         TBLPAG, w2

    ; This is a common tail to both the EZBL_ReadDEVID() and EZBL_ReadDEVREV()
    ; functions. Reads from 0xFF0000 (EZBL_ReadDEVID()) or 0xFF0002 (EZBL_ReadDEVREV())
    setm.b      TBLPAG
    tblrdh      [w0], w1
    tblrdl      [w0], w0
    mov	        w2, TBLPAG
    return
    .size       _EZBL_ReadDEVREV, . - _EZBL_ReadDEVREV
    .size       _EZBL_ReadDEVID,  . - _EZBL_ReadDEVID
    .popsection



