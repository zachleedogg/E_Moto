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
 * Wait for NVMCON<WR> to be clear, disables all interrupts, computes the proper 
 * NVMKEY, copies the computed values into NVMKEY, sets the NVMCON<WR> bit,
 * restores interrupts (if blockUntilDone == 0), polls for completion of the 
 * erase/write operation (if blockUntilDone is non-zero), and then restores 
 * interrupts (if not done already). For the unlock sequence to work correctly, 
 * set:
 *     unlockKeyMultiplier = 0x03E5
 * and:
 *     EZBL_NVMKey = 0x03DF
 *
 * @param nvmconValue Value to write to NVMCON with bit 15 (NVMCON<WR> NOT yet 
 *                    set). This function will issue a second write to set 
 *                    NVMCON<WR> internally after performing correct key 
 *                    sequencing. Bit 14, corresponding to NVMCON<WREN> should 
 *                    be set if you intend to start an erase/write sequence.
 * @param unlockKeyMultiplier Set to value 0x03E5. All other values will result 
 *                            in NVMCON<WR> not getting set and therefore 
 *                            trigger no erase/write operation.
 * @param blockUntilDone <p>Flag indicating if this function should not return
 *                       until the NVMCON write is done and NVMCON<WR> self 
 *                       clears by hardware. Should be 0x0001 (or any value with 
 *                       bit 0 set) to force blocking on certain devices which 
 *                       have an errata on the CPU not stalling correctly (ex: 
 *                       dsPIC33EPXXX(GP/MC/MU)806/810/814 and 
 *                       PIC24EPXXX(GP/GU)806/810/814 rev B1 devices). 0x0000 
 *                       indicates the erase/write should be started and then
 *                       return asynchronously without waiting for hardware to
 *                       clear it.</p>
 *                       
 *                       <p>NOTE: this flag is only a software polling control 
 *                       mechanism. If the underlying NVM hardware requires the 
 *                       CPU to stall for the duration of the erase/write, then
 *                       this parameter will have no effect and the CPU will 
 *                       block regardless.
 *
 * @return NVM erase/write is started (and optionally completed), interrupts are 
 *         restored as appropraite, and EZBL_NVMKey is cleared to zero. If the 
 *         CPU hardware stalls, <i>blockUntilDone</i> is set to '1', or the 
 *         erase/write otherwise completes before the return code is reached, 
 *         then the Write Enable bit, NVMCON<WREN>, is cleared before return.
 */
 ; void EZBL_WriteNVMCON(unsigned int nvmconValue, unsigned int unlockKeyMultiplier, unsigned int blockUntilDone);
 
    .pushsection    .text.EZBL_WriteNVMCON, code
    .global         _EZBL_WriteNVMCON
    .type           _EZBL_WriteNVMCON, @function
    .extern         _EZBL_NVMKey
    .extern         NVMCON
    .extern         NVMKEY
    .extern         SR
    .extern         _EZBL_DisableInterrupts
    .extern         _EZBL_RestoreInterrupts
    reset       ; Guard against possible function entry from Program Counter accidentally falling into executing this function (ex: operating outside Vdd/Clock speed allowable region and device is mis-executing occasional instructions)
_EZBL_WriteNVMCON:
1:  btsc    NVMCON, #15             ; Poll until NVMCON<WR> bit is clear if a prior asynchronous erase/write is ongoing
    bra     1b
    
    ; Set specified NVMCON value (must have WR clear, WREN set, and proper opcode set)
    mov     w0, NVMCON

    ; Write the KEY sequence        ; w1 = 0x3E5
    mov     _EZBL_NVMKey, w7        ; w7 = 0x3DF
    clr     _EZBL_NVMKey
    mul.uu  w7, w1, w6              ; w6:w7 = 0x3E5*0x3DF = 997*991 = 0xF137B = 988,027 (these are two prime numbers, so less likely to accidentally yield the correct unlock key)
    mov     #0x1326, w1             ; w1 = 0x1326
    add     #0x9B, w7               ; w7 = 0x009B + 0x000F = 0x00AA
    sub     w6, w1, w1              ; w1 = 0x137B - 0x1326 = 0x0055
    call    _EZBL_DisableInterrupts ; Disable all interrupts by switching to IPL8; Only clobbers w0
    mov     w1, NVMKEY
    clr     w1                      ; Kill the first 0x0055 key now that we've used it
    mov     w7, NVMKEY
    bset    NVMCON, #15             ; Start the programming sequence
    nop                             ; Two NOPs required after setting NVMCON WR bit
    nop

    ; Enable interrupts ASAP if we aren't asked to block
    btss    w2, #0
    call    _EZBL_RestoreInterrupts ; Only clobbers w0

    clr     w7                      ; Discard unlock key data

    ; Wait for operation to complete if requested
    btst    w2, #0
1:  btsc    NVMCON, #15
    bra     NZ, 1b

    ; Clear NVMCON<WREN> (Write Enable) for safety. This won't work for
    ; asynchronous writes, but also shouldn't do any harm for such cases.
    bclr    NVMCON, #14

    ; Restore interrupts now if we didn't already do so earlier; then return
    btsc    w2, #0
    call    _EZBL_RestoreInterrupts ; Only clobbers w0
    return
    .size   _EZBL_WriteNVMCON, . - _EZBL_WriteNVMCON + 2
    .popsection
    