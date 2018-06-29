;/*******************************************************************************
;  Easy Bootloader Library API Source File
;
;  Description:
;    EZBL_ReadPacked()/EZBL_ReadPackedFlash() implementation for reading all 3
;    bytes of each flash location into a byte array or structure in RAM.
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

    .ifdef  ezbl_lib16ch    ; __GENERIC-16DSP-CH target, but can't test this symbol since it has dashes in it
    .equ    DIV_CYCLES, #5
    .else
    .equ    DIV_CYCLES, #17
    .endif


/**
 * void EZBL_ReadPackedFlash(void *dest, unsigned long baseAddress, unsigned int byteOffset, unsigned int byteCount);
 *
 * Copies Flash/program memory region contents into RAM.
 *
 * Besides ordinary program memory locations, this function can be used to read
 * from Auxiliary Flash, Configuration fuse bytes, and other debug or
 * programming executive memory spaces. On applicable devices programmed for
 * multiple Flash partitions, this function can also be used to read from the
 * inactive Flash partition (starting at address 0x400000), but only when the
 * inactive Flash partition is idle (i.e. not undergoing an erase or programming
 * operation). If the inactive partition is busy with an erase or programming
 * operation, the return results will be indeterminate.
 *
 * Care should be taken to ensure that the target address is legal for the
 * device. Attempting to read from unimplemented memory locations will, in most
 * cases, result in an Address Error Trap rather than returning 0x000000 data.
 * Similarly, on devices that implement segmented Flash Code Protection
 * security, attempting to read from a Flash region that is dissallowed will
 * trigger a device reset.
 *
 * This function must temporary change the TBLPAG register contents in order to
 * issue the read. Ensure no interrupts are implemented which require TBLPAG to
 * be always valid. Alternatively, disable interrupts before calling this
 * function. The TBLPAG contents are restored upon return.
 *
 * @param *dest RAM pointer to write the read program memory data to. This
 *              pointer has no alignment requirements (i.e. can be odd). This
 *              parameter can be null only if byteCount is zero.
 *
 * @param baseAddress Unsigned 32-bit (24-bit) program memory address partially
 *                    specifying where to read from. This address must be even
 *                    (instruction aligned address). The byteOffset parameter is
 *                    added to this address (with proper 3 byte/2 address
 *                    translation) to generate the actual starting read
 *                    location.
 *
 * @param byteOffset A count of how many bytes to add to baseAddress before
 *                   beginning reading. Proper 3-byte/2-address conversion is
 *                   done. For example, if you want to read bits 8-23 of an
 *                   instruction word at address 0x001234 (throwing the first
 *                   8-bits of instruction word data away), you could specify
 *                   baseAddress = 0x001234 and byteOffset = 1.
 *                   Alternatively, baseAddress = 0x001230 and byteOffset = 7
 *                   would address the same byte (byteOffsets 0, 1, and 2 would
 *                   be the 24-bit data bits from address 0x001230; offsets 3,
 *                   4, and 5 would be data from 0x001232, and offset 6 would be
 *                   the least significant 8-bits of the data at address
 *                   0x001234.
 *
 * @param byteCount  Count of many bytes to read from Flash starting at the
 *                   specified address and byte offset. The *dest RAM must have
 *                   this much freespace allocated.
 *
 * @return  Program memory data read is written to the *dest pointer. The data
 *          is returned in naturally packed form (i.e. no "phantom" 0x00 bytes
 *          included).
 */
    .pushsection    .text.EZBL_ReadPackedFlash, code
    .global         _EZBL_ReadPackedFlash
    .type           _EZBL_ReadPackedFlash, @function
_EZBL_ReadPackedFlash:
    ; w0 = void *dest
    ; w1 = unsigned int byteOffset      ****NOTE ALIGNMENT REORDERING****
    ; w3:w2 = unsigned long baseAddress ****NOTE ALIGNMENT REORDERING****
    ; w4 = unsigned int byteCount
    cp0         w4          ; Check to ensure we have at least 1 byteCount to copy
    bra         Z, 0f

    mov         w0, w5      ; Move *dest out of divide register output (w1:w0)
    mul.uu      w1, #2, w0  ; Multiply byteOffset by 2 and align in w1:w0
    mov         #3, w6      ; Divide byteOffset by 3
    repeat      #DIV_CYCLES ;   w0 = program address offset
    div.ud      w0, w6      ;   w1 = 0 means tblrdl-low byte, 2 means tblrdl-high byte, 1 means tblrdh-low byte-with address also being odd
    add         w2, w0, w2  ; Add computed address offset to baseAddress
    addc        #0, w3

    mov         TBLPAG, w7  ; Save TBLPAG
    mov         w3, TBLPAG  ; Set TBLPAG for loop start

    ; Critical for Dual Partition devices, make sure there is no presently 
    ; active Erase/Write operation ongoing before we issue the first table read
0:  btsc        NVMCON, #15
    bra         0b
    
    ; Check tblrdh-low byte condition (remainder = 1; address is odd)
    sub         w4, #1, w4  ; Set loop counter to account for the byte we are going to read before reaching the subtract 3 check stage
    btsc        w2, #0      ; See if address is odd (need tblrdh-low byte, with predecrement)
    bra         1f          ; Yes - start with tblrdh-low byte

    ; tblrdl-low starts by preincrementing, so we need to start by predecrementing the source pointer
    dec         w2, w2

    ; Check tblrdl-high byte condition    (remainder = 2; address is even)
    add         w4, #1, w4      ; Restore loop counter since we are going to do our own loop end check and count reduction (if anything at all)
    btss        w1, #1          ; See if remainder is == 2 (bit 1 is set)
    bra         TestCompletion  ; No - start with tblrdl-low byte


    ; Yes - start with tblrdl-high byte.
    ; Must do seperately in case if the read count was only 1
    inc2        w2, w2          ; Increment to the odd byte of the address. This is a +2 since we predecremented by 1 earlier for the tblrdl-low check case.
    tblrdl.b    [w2], [w5++]
    dec2        w4, w4
    bra         NC, ReadingDone
    bra         1f
    
ReadLoop:
    tblrdl.b    [++w2], [w5++]
    tblrdl.b    [++w2], [w5++]
1:  tblrdh.b    [--w2], [w5++]
    inc         w2, w2
    btsc        SR, #SR_Z
    inc         TBLPAG
TestCompletion:
    sub         w4, #3, w4
    bra         GEU, ReadLoop


    ; Read residual (aligned), if any
    add         w4, #2, w4      ; Make earlier subtract 3 a subtract 1 (possible results are -1 meaning no more reads; 0 meaning do one more read; or 1 meaning do two more reads)
    bra         N, ReadingDone
    repeat      w4
    tblrdl.b    [++w2], [w5++]

ReadingDone:
    mov         w7, TBLPAG      ; Restore TBLPAG
0:  return
    
    .size       _EZBL_ReadPackedFlash, . - _EZBL_ReadPackedFlash
    .popsection
