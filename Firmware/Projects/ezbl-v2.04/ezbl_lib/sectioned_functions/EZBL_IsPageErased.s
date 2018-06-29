;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 source file (ezbl_lib)
;
;  Summary:
;    Assembly language optimized APIs implemented in ezbl_lib
;
;  Description:
;    Checks if a Flash Page is in the erased state (reads back as all '1's).
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

	.equ	SR_C, 0x0	; SR<C> is bit 0
	.equ	SR_Z, 0x1	; SR<Z> is bit 1
	.equ	SR_OV, 0x2	; SR<OV> is bit 2
	.equ	SR_N, 0x3	; SR<N> is bit 3


/**
 * Tests if all bytes on a given Flash Page are erased. Returns 1 if the entire
 * Page has '1's on it, or 0 otherwise. No special address checking or
 * conditions are exempted.
 *
 * Execution time is about 18 + 9,728 cycles/0x400 sized Flash page on
 * dsPIC33E/PIC24E/dsPIC33D/dsPIC33C Flash, or about 11 + 5,632 cycles/0x400
 * sized Flash page on PIC24F/PIC24H/dsPIC33F/dsPIC33C PRAM devices. These
 * assumes the page is blank. Non-blank pages will return earlier.
 *
 * NOTE: This function requires the _EZBL_ADDRESSES_PER_SECTOR link time symbol
 * to be set to the number of program addresses that represents one Flash Erase
 * Page on your hardware. On most devices, this is either 0x400 or 0x800 and
 * will be automatically set by ezbl_tools.jar in the project's .gld linker
 * script or by the Bootloader project you link to. However, if you are not
 * using ezbl_tools.jar or an existing EZBL Bootloader, you can manually define
 * this value by using the EZBL_SetSYM() macro in one of your C source files:
 *      <p><code>EZBL_SetSYM(EZBL_ADDRESSES_PER_SECTOR, 0x400);</code></p>
 *
 * 0x400 program addresses is equivalent to 1536 bytes, or 512 24-bit
 * instruction words. Be sure and check the size in the device data sheet when
 * manually assigning this value.
 *
 * @param programAddress Flash program memory address to check. This address
 *                       does not have to be page aligned. All bytes on the page
 *                       will still be checked if an unaligned (or odd) address
 *                       is provided.
 *
 * @return  <ul>
 *              <li>1 - Page is erased and contains all 0xFF bytes</li>
 *              <li>0 - Page contains one or more '0' bits on it</li>
 *          </ul>
 */
 ; unsigned int EZBL_IsPageErased(unsigned long programAddress);
     .pushsection    .text.EZBL_IsPageErased, code
    .global         _EZBL_IsPageErased
    .type           _EZBL_IsPageErased, @function
    .extern         TBLPAG
    .extern         _NVMCON
    .extern         _EZBL_ADDRESSES_PER_SECTOR
    ; w0 = read address
    ; w1 = tblpag of read address, temporary
    ; w2 = TBLPAG save
    ; w3 = #_EZBL_ADDRESSES_PER_SECTOR-1 decrementing loop counter
    ; w4 = 0xFFFF accumulator
_EZBL_IsPageErased:
    mov         TBLPAG, w2
    mov         w1, TBLPAG
    mov         #(_EZBL_ADDRESSES_PER_SECTOR-1), w3
    com         w3, w1          ; w4 = Page select mask (gives upper bits when anded)
    and         w1, w0, w0      ; w0 = Aligned 16-bit read address on start of page
    setm        w4              ; Start with low word accumulator = 0xFFFF
0:  btsc        _NVMCON, #15
    bra         0b

1:  tblrdh.b    [w0], w4
    tblrdl      [w0++], w1
    and         w4, w1, w4
    com         w4, w1          ; Check 0xFFFF still present in w4
    bra         NZ, notErased
    dec2        w3, w3
    bra         C, 1b

erased:
    mov         w2, TBLPAG
    retlw       #1, w0          ; return 1 - Yes, the page is erased

notErased:
    mov         w2, TBLPAG
    retlw       #0, w0          ; return 0 - No, the page contains at least one programmed bit someplace

    .size       _EZBL_IsPageErased, . - _EZBL_IsPageErased
    .popsection

    