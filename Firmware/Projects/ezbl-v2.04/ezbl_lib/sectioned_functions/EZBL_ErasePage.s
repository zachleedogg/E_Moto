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
 * void EZBL_ErasePage(unsigned long addressOnPage);
 *
 * Erases the page containing the specified address. Be very careful if calling
 * this function as the function will erase any legal page, including ones
 * inside your bootloader and also possibly including the same page that this
 * function itself is stored on.
 *
 * EZBL_NVMKey must have been externally set to 0x03DF or this function will
 * do nothing. Immediately upon use or return, EZBL_NVMKey will be set to
 * 0x0000.
 *
 * Do not call this function on PIC24FxxKxxx, or dsPIC30F devices. They have 
 * very different NVMCON bit encodings and are not supported at this time.
 *
 * NOTE: This function can take upwards of 45ms on some device families and 
 * target pages to complete. At frequencies appreciably slower than the maximum 
 * device frequency, even longer may be required. Therefore, you may need to 
 * issue a ClrWdt() call before calling this function, assuming the Watchdog 
 * timer is enabled. This also means that you should not configure a watchdog 
 * timeout of less than ~64ms, even when you pre-clear the timeout. This 
 * function does NOT internally clear the watchdog for you as it is packaged as 
 * a library routine where not everyone would desire such behavior.
 * 
 * @param addressOnPage 24-bit (unsigned long) specifying an address on the page
 *                      to be erased. Anywhere on the page will work as the
 *                      hardware will mask off the lower bits within the page.
 * 
 * @return Page erase operation will have started if EZBL_NVMKey was properly 
 *         set and no hardware Write Protect or other hardware features prevent 
 *         it. This function will not block the CPU if the hardware supports a 
 *         non-blocking page erase of the given address. You therefore should 
 *         call EZBL_WaitForNVMOpComplete() before attempting to modify any 
 *         NVMCON/NVMADR/NVMSRCADR/etc. state, including issuing any Table Write 
 *         (tblwth, tblwtl) instructions in your code. All EZBL NVM erase/write 
 *         operations include an internal check at their start that does the 
 *         equivalent of EZBL_WaitForNVMOpComplete(), including this 
 *         EZBL_ErasePage() function. Therefore, it is safe to call EZBL 
 *         implemented NVM functions without doing an explicit call to 
 *         EZBL_WaitForNVMOpComplete() beforehand.
 */
    .pushsection    .text.EZBL_ErasePage, code
    .global         _EZBL_ErasePage
    .type           _EZBL_ErasePage, @function
    .extern         TBLPAG
    .extern         NVMCON
    .weak           _NVMADR     ; Extern SFR register that doesn't exist on all devices
    .weak           _NVMADRU    ; Extern SFR register that doesn't exist on all devices
    reset       ; Guard against possible function entry from PC accidentally falling into executing this function (ex: operating outside Vdd/Clock speed allowable region and device is mis-executing occassional instructions)
_EZBL_ErasePage:
    btsc        NVMCON, #15     ; Loop, blocking until last NVM operation is complete (WR is clear)
    bra         _EZBL_ErasePage

    ; Issue a dummy Table Write to set the erase address should we be using 
    ; older dsPIC30/PIC24F/PIC24H/dsPIC33F style addressing without
    ; NVMADR/NVMADRU registers
    mov     TBLPAG, w2
    mov     w1, TBLPAG
    tblwtl  w3, [w0]
    mov     w2, TBLPAG

    ; Set erase address using NVMADR and NVMADRU. These are effectively NOPs on
    ; devices without _NVMADR, since _NVMADR and _NVMADRU will resolve to 0x0000
    ; and w0 is at address 0x0000, which we don't need anymore
1:  mov     w0, _NVMADR
    mov     w1, _NVMADRU

    ; See if NVMCON<6> is implemented. On devices where it is, this is the 
    ; ERASE bit and we must use the NVOP[3:0] = 4'b0010 encoding for Page erase; 
    ; otherwise NVMCON<6> is unimplemented and will stay '0'. These devices use
    ; Page Erase encoding of NVOP[3:0] = 4'b0011
    mov     #0x4042, w0     ; Set WREN, ERASE, and assume NVMOP = 0x2 for Flash Page Erase Operation to start
    bset    NVMCON, #6
    btss    NVMCON, #6
    xor     #0x0041, w0     ; Clear non-existent ERASE bit and set NVMOP = 0x3 since we are using the newer encoding for Page Erase

    clr     w2              ; Return after starting the erase, so we don't block interrupts waiting for completion
    mov     #0x03E5, w1     ; Set second NVMKey unlocking material (EZBL_NVMKey must have externally been set to 0x03DF)
    call    _EZBL_WriteNVMCON
0:  btsc    NVMCON, #15     ; Loop, blocking until last NVM operation is complete (WR is clear)
    bra     0b
    bclr    NVMCON, #14     ; Clear WREN 
    return
    
    .size   _EZBL_ErasePage, . - _EZBL_ErasePage + 2
    .popsection
    