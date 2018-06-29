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


    ; Declare some global SFRs (latches) in Program Space
    
    ; Program space latches for holding table write data destined for Flash writing via NVMCON
    NVMDATA  = 0xFA0000
    NVMDATAL = NVMDATA
    NVMDATAH = NVMDATA+2
    _NVMDATA  = NVMDATA         ; C accessible names
    _NVMDATAL = NVMDATAL
    _NVMDATAH = NVMDATAH
    
    ; Declare Program Space 6-byte Table Latch like the ones at 0xFA0000 when writing to Flash, but at a lower address so they are EDS accessible
    SLVDATA  = 0x7FF800         
    SLVDATAL = SLVDATA
    SLVDATAH = SLVDATA+2
    _SLVDATA  = SLVDATA         ; C accessible names
    _SLVDATAL = SLVDATAL
    _SLVDATAH = SLVDATAH
    .global         NVMDATA
    .global         NVMDATAL
    .global         NVMDATAH
    .global         SLVDATA
    .global         SLVDATAL
    .global         SLVDATAH
    .global         _SLVDATA
    .global         _SLVDATAL
    .global         _SLVDATAH
    
/**
 * unsigned int EZBL_SlaveLoad(int cpuID, int targetPartition, unsigned long localSlvImageAddr);
 *
 * Copys the specified program data from Master RAM into the specified slave 
 * CPU's Program RAM (PRAM) address space.
 *
 * NOTE: The specified slave CPU core must be held in reset while this function
 *       executes. If reset has been released, use the EZBL_SlaveReset() 
 *       function to reassert slave reset.
 *
 * @param cpuID Reserved for future use. Set to 1 for devices with only one 
 *              slave CPU.
 *
 * @param srcData Pointer into master CPU RAM to read the source data from.
 *
 * @param byteLen Number of bytes to copy to the destination slave PRAM 
 *                location. Due to ECC over each 48-bit PRAM word length, 
 *                byteLen should be an integer multiple of 6 bytes. 
 *                Non-multiples are allowed, but will result in 0xFF filler 
 *                bytes being added to the beginning or end of the 48-bit PRAM 
 *                word in order to comit the given bytes into the PRAM. These 
 *                filler bytes can overwrite already existing data (which cannot 
 *                be read beforehand), so ensure necessary alignment and padding 
 *                is done outside this function if this behavior must be 
 *                avoided.
 *
 * @return Number of bytes written to the slave Program-RAM.
 */
    .pushsection    .text.EZBL_SlaveLoad, code
    .global         _EZBL_SlaveLoad
    .type           _EZBL_SlaveLoad, @function
    .extern         DSRPAG
    .extern         DSWPAG
_EZBL_SlaveLoad:
    push        DSRPAG
    push        DSWPAG

    ; w0    = cpuID - CPU ID
    ; w1    = targetPartition - Run-time Target Partition select: 0 = leave bit 22 unchanged, 1 = mask Bit 22 to '0' for Partition 1, 2 = mask bit 22 to '1' for Partition 2
    ; w2:w3 = localSlvImageAddr - Source Program address of slave image (as linear, long address, not EDS ptr)

    ; Convert long source program address to EDS address
    btsts.c 	w2, #15
    rlc     	w3, w3
    bset    	w3, #9
    mov     	w3, DSRPAG
    mov         w1, w4          ; Save a copy of the run-time target partition select

    add         w2, #4, w3      ; Save initial source address + 0x4 to work around missing termination problem in early slave images
    bset        w3, #15
    
next_record:
    mov.d   	[w2++], w0      ; w0 = dest[15:0], w1 = instruction word count
    dec         w1, w1
    bra         NC, done
    cp0         w0              ; Termination problem workaround
    bra         NZ, 1f
    cp          w2, w3
    bra         NZ, done
1:  mov.w   	[w2++], w6      ; w6[15:7] = dest page for DSWPAG when shifted; w6[6:0] = record type (0x1F for Slave PRAM image)
    lsr.w   	w6, #0x7, w5
    
    btss        w4, #0  ; Partition == 1 ?
    bra         1f  
    bclr        w5, #7  ; Clear target address bit (22-1-16) so we force 0x000000 - 0x3FFFFF range for Partition 1
    btsc        SR, #SR_Z
    bclr        w0, #15
    
1:  btss        w4, #1  ; Partition == 2 ?
    bra         1f
    bset        w5, #7  ; Set target address bit (22-1-16) so we force 0x400000 - 0x7FFFFF range for Partition 2
    bset        w0, #15 ; Make DSWPAG get used
    
1:  mov.w   	w5, DSWPAG
    and.w   	#0x7F, w6
    cp.w    	w6, #0x1F
    bra     	NZ, done

copy_slave:
    repeat      w1
    ldslv       [w2++], [w0++], #0x1
    bra         next_record
    
done:
    pop         DSWPAG
    pop         DSRPAG
    return
    
    .size   _EZBL_SlaveLoad, . - _EZBL_SlaveLoad
    .popsection





/**
 * unsigned int EZBL_SlaveLoadFromRAM(int cpuID, unsigned long slvTargetAddress, void *srcData, unsigned int dataLen);
 *
 * Copys the specified program data from Master RAM into the specified slave 
 * CPU's Program RAM (PRAM) address space.
 *
 * NOTE: The specified slave CPU core must be held in reset while this function
 *       executes. If reset has been released, use the EZBL_SlaveReset() 
 *       function to reassert slave reset.
 *
 * @param cpuID Reserved for future use. Set to 1 for devices with only one 
 *              slave CPU.
 *
 * @param slvTargetAddress Slave PRAM address to being writing to. This address 
                  must be double word aligned (0x4 start boundary).
 * @param *srcData Pointer into master CPU RAM to read the source data from.
 * @param dataLen Number of bytes to copy to the destination slave PRAM 
 *                location. Due to ECC over each 48-bit PRAM word length, 
 *                dataLen should be an integer multiple of 6 bytes. 
 *                Non-multiples are allowed, but will result in 0xFF filler 
 *                bytes being added to the beginning or end of the 48-bit PRAM 
 *                word in order to comit the given bytes into the PRAM. These 
 *                filler bytes can overwrite already existing data (which cannot 
 *                be read beforehand), so ensure necessary alignment and padding 
 *                is done outside this function if this behavior must be 
 *                avoided.
 *
 * @return void
 */
    ;unsigned int EZBL_SlaveLoadFromRAM(int cpuID, unsigned long slvTargetAddress, void *srcData, unsigned int dataLen);
    .pushsection    .text.EZBL_SlaveLoadFromRAM, code
    .global         _EZBL_SlaveLoadFromRAM
    .type           _EZBL_SlaveLoadFromRAM, @function
    .extern         TBLPAG
    .extern         DSWPAG
    .extern         DSRPAG
_EZBL_SlaveLoadFromRAM:
    push        TBLPAG
    push        DSWPAG
    push        DSRPAG

    ; w0    = CPU ID
    ; w1    = *srcData pointer
    ; w2:w3 = slvTargetAddress
    ; w4    = dataLen
    movpag      #0x200 | (SLVDATA>>15), DSRPAG
    movpag      #SLVDATA>>16, TBLPAG
    btst.c      w2, #15
    rlc         w3, w3
    mov         w3, DSWPAG
    btss        SR, #SR_Z
    bset        w2, #15
    bra         2f
    
1:  tblwtl.b    [w1++], [w5++]
    tblwtl.b    [w1++], [w5--]
    tblwth.b    [w1++], [w5++]
    tblwtl.b    [w1++], [++w5]
    tblwtl.b    [w1++], [++w5]
    tblwth.b    [w1++], [--w5]
    mov         #SLVDATA & 0xFFFF, w5
    
    ldslv       [w5++], [w2++], #1
    ldslv       [w5++], [w2++], #1
2:  mov         #SLVDATA & 0xFFFF, w5
    sub         w4, #6, w4
    bra         GE, 1b
    
    pop         DSRPAG
    pop         DSWPAG
    pop         TBLPAG
    return
    
    .size   _EZBL_SlaveLoadFromRAM, . - _EZBL_SlaveLoadFromRAM
    .popsection

