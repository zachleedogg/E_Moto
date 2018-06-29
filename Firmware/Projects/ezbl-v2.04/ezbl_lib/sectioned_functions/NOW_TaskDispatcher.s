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
 * NOW_TASK * __attribute__((persistent, near)) NOW_timerCallbackHead;
 *
 * NOW_timerCallbackHead - Internal pointer to the first element of a linked
 * list for tracking and launching user callback functions created with the
 * NOW_CreateTask() function. This variable should not be changed, and it is
 * attributed persistent so the CRT doesn't initialize it to zero either.
 * Instead, NOW_Init() nullifies it, with NOW_CreateTask(), NOW_EndTask(), and
 * the dispatching routines handling modification of it.
 *
 * Referenced weakly by NOW_Init(), but strongly by NOW_TaskDispatcher.
 */
    .pushsection .NOW_timerCallbackHead, persist, near
    .global _NOW_timerCallbackHead  ; near, persistent, NOW_TASK head pointer
    .type   _NOW_timerCallbackHead, @object
_NOW_timerCallbackHead:
    .skip   2
    ;.size   _NOW_timerCallbackHead, . - _NOW_timerCallbackHead         ; Unneeded, so commentted out - can interfere with --preserve-all linker option
    .popsection

    
/**
 * unsigned char __attribute__((near, preserved, persistent)) NOW_dispatcherActive;
 * 
 * Initialized near variable flag to avoid recursion if timer ISR is called
 * again while we are still busy executing or dispatching prior tasks.
 */
    .pushsection    .NOW_dispatcherActive, persist, near, preserved
    .global         _NOW_dispatcherActive
    .type           _NOW_dispatcherActive, @object
    .equ            active, 0                       ; Bit 0: 1 = Indicates the NOW_TaskDispatcher() function is running due to timer interrupt; 0 = NOW_TaskDispatcher() is not currently running in any IPL.
_NOW_dispatcherActive:
    .skip  1
    ;.size           _NOW_dispatcherActive, . - _NOW_dispatcherActive   ; Unneeded, so commentted out - can interfere with --preserve-all linker option
    .popsection


/**
 * void NOW_TaskDispatcher(void);
 *
 * Internal function for dispatching tasks from NOW ISR. Do not call directly.
 */
    .pushsection    .text.NOW_TaskDispatcher, code
    .global         _NOW_TaskDispatcher
    .type           _NOW_TaskDispatcher, @function
    .extern         _NOW_dispatcherActive
    .extern         _NOW_timerCallbackHead  ; Must be allocated 'near'
    .weak           _NOW_internalCount
    .equ            countPtr, 6         ; Offset of countPtr element in NOW_internalCount structure
    .weak           _NOW_timerSFRPtr
    .weak           _NOW_timeOffset
    .extern         DSRPAG
    .extern         DSWPAG
    .extern         PSVPAG
    .weak           DSWPAG
    .weak           DSRPAG
    .weak           PSVPAG
    .extern         RCOUNT
    .weak           CTXTSTAT            ; Should perhaps handle this. Changing IPL corrupts RAM without saving/restoring proper registers and using CTXTSWP instructions.

    .equ            nextPtrOffset, 0        ;// 2 bytes wide    // EQUs for NOW_TASK structure byte offsets
    .equ            timeoutOffset,  2       ;// 6 bytes wide
    .equ            intervalOffset, 8       ;// 6 bytes wide
    .equ            callbackOffset, 14      ;// 2 bytes wide
    .equ            IPLOffset, 16           ;// 2 bytes wide
    .equ            returnSignalOffset, 18  ;// 2 bytes wide
_NOW_TaskDispatcher:
    mov     _NOW_timerCallbackHead      ;// NOW_timerCallbackHead = NOW_timerCallbackHead; Do an early test to avoid saving/restoring registers if there is guranteed no work to do
    bra	    Z, abort
    mov     [w15-8], w1                 ;// Only allow execution if we have a return address that is at IPL0. We don't want to inadvertently
    lsr     w1, #13, w1                 ;// trigger a recursive interrupt at lower IPLs if we switch from the Timer IPL to function dispatch IPLs.
    bra     NZ, abort
    btsc    CORCON, #3                  ;// Also include CORCON<IPL3>
    bra     NZ, abort                   
    btsts   _NOW_dispatcherActive, #active  ;// Check for self recursion - not allowed. Also atomically sets the flag.
    bra	    NZ, abort
    push    RCOUNT
    push    DSRPAG ;// may be w0        ;// Not defined on all parts, and is mutually exclusive with PSVPAG, so declared .weak. This will push address 0x0000 (which is the w0 register) onto the stack when DSRPAG is not present.
    push    DSWPAG ;// may be w0        ;// Not defined on all parts, and is mutually exclusive with PSVPAG, so declared .weak. This will push address 0x0000 (which is the w0 register) onto the stack when DSWPAG is not present.
    push    PSVPAG ;// may be w0        ;// Not defined on all parts, and is mutually exclusive with DSRPAG, so declared .weak. This will push address 0x0000 (which is the w0 register) onto the stack when PSVPAG is not present.
    push.d  w4                          ;// Save registers - we were called from an ISR. Note: _TxInterrupt() had to have saved w1, w2, and w3 for us. w0 was saved above by PSVPAG or DSRPAG pushing
    push.d  w6
    
    mov     _NOW_timerSFRPtr, w5        ;// Get an atomic copy of NOW_internalCount while we are at the Timer ISR IPL
    ;clr     w1                         ; Maintain w1 = 0x0000 (commented out since w1 already has zero from earlier not-zero abort test)
    mov     _NOW_timeOffset, w6
    mov     _NOW_internalCount+countPtr, w7
    subr    w6, [w5], w5                ; w5 = TMRx - NOW_timeOffset
    subbr   w1, [w7++], w6              ; w6 = NOW_internalCount[31:16] - 0 - Borrow
    subbr   w1, [w7], w7                ; w7 = NOW_internalCount[47:32] - 0 - Borrow
    
    ; Drop to CPU IPL0 to execute the dispatch polling loop
    clr.b   SR                          ;// Clears {IPL2,IPL1,IPL0,x,N,OV,Z,C}, where x is the Repeat Active bit, which is already clear
    
    mov     #_NOW_timerCallbackHead, w4	;// Get the Start pointer's address as we may need to write to it later
1:  mov	    w4, w3                      ;// Save a copy of the last pointer in w3 so we can pull this one out of the list if non-repeating
    subr    w1, [w4], w4                ;// Get NOW_TASK->next (Find the first structure's address for the first run). This is a {mov [w4+nextPtrOffset], w4} instruction, but sets SR<Z>.
    bra     Z, done_calling

    add     w4, #timeoutOffset, w0      ;// Get NOW_TASK->timeout[15:0]'s address
    cp      w5, [w0++]                  ;// now - timeout < 0 ?
    cpb     w6, [w0++]
    cpb     w7, [w0++]
    bra     N, 1b
    
    ; Task is ready for execution: terminate next call if one-shot interval, update the interval, dispatch the task callback
    push.d  w4                          ; Save NOW_TASK head pointer (w4) and atomically captured NOW_48 quantity (w7:w5)
    push.d  w6

    ; Terminate the task from repeating if the repeat interval is 0x000000000000
    ;add     w4, #intervalOffset, w0
    subr    w1, [w0++], w5              ;// Get NOW_DELAYED_EXE->interval[15:0] and compare with 0
    subbr   w1, [w0++], w6              ;// Get NOW_DELAYED_EXE->interval[31:16]
    subbr   w1, [w0++], w7              ;// Get NOW_DELAYED_EXE->interval[47:32]
    btsc    SR, #SR_Z
    mov     [w4], [w3]                  ;// interval == 0x000000000000, so: left->next = NOW_TASK->next; NOTE: This really is {mov [w4+nextPtrOffset], [w3+nextPtrOffset]}, but such an instruction isn't accepted by the assembler (even though nextPtrOffset is zero and can be simplified away).

    ; Update timeout by adding the interval
    add     w5, [++w4], [w4]            ;// timeout[47:0] += interval[47:0]
    addc    w6, [++w4], [w4]
    addc    w7, [++w4], [w4]

    ; Get the callback address, push our return address onto the stack, set up a long call frame, and dispatch the call
    mov     #packed_lo(dispatch_return_point), w6   ;// Push our return address onto the stack
    mov     #packed_hi(dispatch_return_point), w7
    push.d  w6
    mov     [w0++], [w15++]             ;// Push callbackFunction address low bits <15:0>
    mov     [w0++], [w15++]             ;// Push callbackFunction address high bits <23:16> and IPL
    retfie                              ;// Calls the function pointer. When it returns, PC goes to dispatch_return_point to start the loop over.
dispatch_return_point:
    clr.b   SR                          ;// Switch back to CPU IPL0
    pop.d   w6                          ;// Reget w7:w5 = "now" from before dispatching
    pop.d   w4                          ;// Reget NOW_TASK head pointer for this just executed task
    mov     w0, [w4+returnSignalOffset] ;// Save NOW_DELAYED_EXE->returnSignal
    clr     w1                          ; Maintain w1 = 0x0000
    bra     1b

done_calling:
    pop.d   w6
    pop.d   w4
    pop     PSVPAG
    pop     DSWPAG
    pop     DSRPAG
    pop	    RCOUNT
    bclr   _NOW_dispatcherActive, #active   ; // It is now safe for the NOW_TaskDispatcher() function to be executed again
abort:
    pop.d   w2                          ; w3:w1 pushed on the stack by _TxInterrupt()
    pop     w1
    retfie
    .size   _NOW_TaskDispatcher, . - _NOW_TaskDispatcher
    .popsection
    