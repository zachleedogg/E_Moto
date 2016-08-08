/* 
 * File:   emoto_flash_write.c
 * Author: kid group
 *
 * Created on August 6, 2016, 12:59 AM
 */

#include <xc.h>
#include <stdint.h>

/*
 * 
 */
uint8_t flashIsWriting(void) {
    return NVMCONbits.WR;
}

/**
 * eraseFlashPage erases a 1024 word block
 * @param thisPage must be a multiple of 1024
 * @return success or failure
 */
uint8_t eraseFlashPage(uint16_t* thisPage) {
    
    NVMADRU = 0x00;
    NVMADR = *thisPage;
    NVMKEY = 0x55; /*unlock*/
    NVMKEY = 0xAA; /*unlock*/
    NVMCONbits.WR = 1; /*Set write operation*/
    NVMCONbits.WREN = 1; /*enable read/write*/
    NVMCONbits.NVMOP = 3; /*Set page write*/
    asm("NOP"); /*wait*/
    asm("NOP"); /*wait*/
    
    /*wait some more*/
    while(NVMCONbits.WR == 1){}
    
    return 0;
    
    // Disable interrupts < priority 7 for next 5 instructions.
    // Assumes no level 7 peripheral interrupts
//    asm("DISI #06");
//    asm("MOV #0x55, W0");
//    asm("MOV W0, NVMKEY");
//    asm("MOV #0xAA, W0");
//    asm("MOV W0, NVMKEY ");
//    asm("BSET NVMCON, #15"); // Start the program/erase cycle
//    asm("NOP");
//    asm("NOP");
}

/**
 * writes two 16bit words to flashmem
 * @param thistable is the LSWord address of the two words
 * @param doubleWord pointer to two 16 bit words
 * @return 
 */
uint8_t writeTwoWords(uint16_t thisTable, uint16_t* doubleWord){
    
}