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
 * void __attribute__((noreturn)) EZBL_PartitionSwap(void);
 *
 * Disables all interrupts by clearing all IECx registers, swaps to the 
 * presently inactive partition, and branches to the 0x000000 reset vector. 
 *
 * CORCON<IPL3>, SR<7:0> are all cleared atomically during the branch, resulting
 * in the CPU starting execution on the new partition in IPL0 (main() context) 
 * with the SR<RA> Repeat Active status bit cleared, so it is possible to call 
 * this function from within an ISR if needed to synchronize the swap to occur
 * just after some critical ISR code is done executing.
 *    
 * Interrupts are NOT restored in this function. The needed IECx bits need to be 
 * restored by the new partition's Application since we can't have any 
 * interrupts occurring while doing the partition swap unlock and bootswap, nor 
 * would it be safe to trigger an ISR while the new Application is busy 
 * executing initialization code needed for any ISRs that have changed.
 * 
 * To detect if execution is occurring in the new Application due to a genuine 
 * reset or due to a partition swap event, check the NVMCON<SFTSWP> status bit. 
 * It is zero on real reset and one if a partition swap has taken place.
 *
 * XC16 v1.30+ supports a callback function before executing the CRT 
 * initializer to allow you to control if variable initializer code should 
 * execute. This callback should be declared as int _crt_start_mode(void); with 
 * the return value of 0 meaning to execute all initializer code as normal and 1 
 * meaning to skip normal initialization for Live Update style partition swaps. 
 * The later case skips most variable initialization and branches to main() 
 * sooner.
 *
 * A suggested implementation of this callback is:
 *     int __attribute__((optimize(1))) _crt_start_mode(void)
 *     {
 *         return _SFTSWP;
 *     }
 *
 * XC16 v1.30+ also supports a new 'priority(n)' function and variable 
 * attribute, where n is an integer from 1 to 65535 and allows code fragments + 
 * variables to execute in advance of ordinary CRT variable initalization 
 * completion and also when _crt_start_mode() returns 1.
 *
 * The CRT initializes the stack pointer, SPLIM, DSRPAG, initializes any 
 * variables attributed with 'priority(1)' and then calls any void functions 
 * with 'priority(1)' also assigned as an attribute. 'priority(2)' variables are 
 * then initialized followed by 'priority(2)' functions and so on until no more 
 * priority tagged variables or functions are left, whereupon:
 *   if(_crt_start_mode() == 1)
 *      - user_init() functions are called, if any
 *      - main() is executed
 *   else   // when _crt_start_mode() == 0 or _crt_start_mode() is undefined
 *      - unattributed (ordinary) variables are initialized
 *      - uart_init() functions are called, if any
 *      - main() is executed
 *    
 * Using the priority attributes allows you to selectively reenable any 
 * time-critical tasks, such as specific Interrupts, in a controlled order so as 
 * to avoid any unnecessary execution delay during a partition swap event as the 
 * bulk of non-special variables gets initialized or zeroed.
 *
 * See the XC16 documentation for more information on the _crt_start_mode() and 
 * and priority(n) features.
 *     
 * NOTE: The FICD<BTSWP> Config word bit must be turned ON (cleared to '0') or 
 * else this function will execute as described, but without actually swapping 
 * partitions. Use #pragma config BTSWP = ON or EZBL_SET_CONF(_FICD, _BTSWP_ON) 
 * to appropriately configure this.
 * 
 * @return This function never returns. Execution branches through the reset 
 *         vector, resulting in all normal SFRs retaining state, but the stack 
 *         is reinitialized and RAM variables may (or may not) also be 
 *         reinitialized.
 *
 *         This function's prototype has the __attribute__((noreturn)) option 
 *         applied so the compiler can optimize away subsequent code (like a 
 *         ulnk and return instruction) as dead code.
 */
    .pushsection    .text.EZBL_PartitionSwap, code
    .global         _EZBL_PartitionSwap
    .type           _EZBL_PartitionSwap, @function
    .extern         NVMKEY
    .extern         IEC0
    .extern         IPC0
_EZBL_PartitionSwap:
    ; Disable all interrupts by zeroing all IECx bits. These need to be restored 
    ; by the new partition's Application.
    mov     #IEC0, w0
    mov     #(IPC0-2), w1   ; Subtract 2 so after /= 2, we will have 1 total less for repeat parameter
    sub     w1, w0, w1  ; w1 = &IPC0 - 2 - &IEC0;
    lsr     w1, w1      ; w1 /= 2;
    repeat  w1
    clr     [w0++]

    ; Push a 0x000000 return address and 0x00 SR byte onto the stack so we can 
    ; do a single instruction word 'retfie' after 'bootswp' to reach the reset 
    ; vector on the other partition.
    clr     [w15++]
    clr     [w15++]

    ; Execute the partition swap and branch to 0x000000 on the new partition
    mov     #0x0055, w0
    mov     w0, NVMKEY  ; NVMKEY = 0x55
    sl      w0, #1, w0
    mov     w0, NVMKEY  ; NVMKEY = 0xAA
    bootswp             ; Do the bootswap while executing the next 24-bit instruction
    retfie              ; Branches to address 0x000000 on the alternate partition and simultaneously switches to IPL0 like a real Reset would do. Note: This opcode must be a single instruction wide and must be an instruction that modifies the PC for a new Flash fetch to take place since the very next instruction after this will be on the just-seleceted partition.
    
    .size   _EZBL_PartitionSwap, . - _EZBL_PartitionSwap
    .popsection
    