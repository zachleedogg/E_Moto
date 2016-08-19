#include <xc.h>
/*******************************************************************************
 * ZIP file of source code described in Digi-Key Electronics EE Wiki page 
 * "FLASH Self Write Microchip dsPIC33F, PIC24h".
 * This is NOT a ready to run program.
 ******************************************************************************/


/******************************************************************************/
// Default timeout limits in FLASH program memory
const unsigned int Parameters[3] __attribute__((space(psv), address(0x6FF0))) = {10, 20, 30};

// Timeout limits are global
unsigned int timeout1, timeout2, timeout3 = 0;
/******************************************************************************/


/*******************************************************************************
 *  initParameters()
 *
 *  Prototype: (void) initParameters(void)
 *  Passed arguments: none
 *  Return value: none
 *  Dependencies: none
 *
 *  Description:  Read the timeout parameters from the Program Space Visibility area and
 *   copy to RAM.*
 ******************************************************************************/
//void initParameters(void)
//{
//    int memptrtemp;
//    // User Managed PSV
//    memptrtemp = PSVPAG;    // Save the auto_psv page
//    PSVPAG = __builtin_psvpage(&iParameters);   // get address
//    CORCONbits.PSV = 1;                         // Set PSV enable
//    timeout1 = Parameters[0];              // Read parameters and assign to variables
//    timeout2 = Parameters[1];
//    timeout3 = Parameters[2];
//    PSVPAG = memptrtemp;    // Restore auto_psv page
//
//    return;
//}


/*******************************************************************************
 *  programParameters()
 *
 *  Prototype: int programParameters(unsigned int, unsigned int, unsigned int)
 *  Passed arguments: unsigned int param1, unsigned int param2, unsigned int param3
 *  Return value: none
 *  Dependencies: INTERRUPTS MUST be DISABLED prior to calling this routine.
 *
 *  Description:  Erase block of NVM and write data to FLASH memory
 ******************************************************************************/
void programParameters(unsigned int param1, unsigned int param2, unsigned int param3)
{
    unsigned int tbloffset;

    //Erase one page of FLASH memory
    NVMCON = 0x4042;   // Setup NVMCON for Block Erase operation (512 instructions)
    TBLPAG = __builtin_tblpage(&Parameters[0]);  // Load upper FLASH address
    tbloffset = __builtin_tbloffset(&Parameters[0]);  // Offset within page
    __builtin_tblwtl(tbloffset, 0xFFFF);  // Dummy write to select the page
    FLASHWrite();        // low level erase/write command sequence

    // write single word, param1, to FLASH
    // In this case the Page hasn't changed so don't have to load the table
    // page register again.
    tbloffset = __builtin_tbloffset(&Parameters[0]);  // offset within page
    __builtin_tblwtl(tbloffset, param1);  // load the write buffer
    NVMCON = 0x4003;     // Setup NVMCON for Single Word Write operation
    FLASHWrite();        // low level erase/write command sequence

    // write single word, param2, to FLASH
    tbloffset = __builtin_tbloffset(&Parameters[1]);  // offset within page
    __builtin_tblwtl(tbloffset, param2);  // load the write buffer
    NVMCON = 0x4003;     // Setup NVMCON for Single Word Write operation
    FLASHWrite();        // low level erase/write command sequence

    // write single word, param3, to FLASH
    tbloffset = __builtin_tbloffset(&Parameters[2]);  // offset within page
    __builtin_tblwtl(tbloffset, param3);  // load the write buffer
    NVMCON = 0x4003;     // Setup NVMCON for Single Word Write operation
    FLASHWrite();        // low level erase/write command sequence

    return;  
}



/*******************************************************************************
 *  FLASHWrite()
 *
 *  Prototype: (void) FLASHWrite(void)
 *  Passed arguments: none
 *  Return value: none
 *  Dependencies: Must have NVMCOM, TBLPAG, and table write buffer initialized
 *                prior to calling this routine.  NVMCOM must have the erase
 *                or write enable bits correctly set.
 *  Description:  Provides the low level FLASH erase/write command sequence
 *                required to erase a page of FLASH, or write a row of data in
 *                FLASH, or write a single word in FLASH.  It clears the
 *                NVMCOM WR bit before returning to calling function.
 ******************************************************************************/
void FLASHWrite()
{
    
    
    __builtin_disi(5);      // Disable interrupts for 5 cycles
    NVMKEY = 0x55;          // Write requisite KEY values for FLASH access
    NVMKEY = 0xAA;
    NVMCONbits.WR = 1;      // Activate the erase action
    __builtin_nop();        // Requires two nops for time delay
    __builtin_nop();
    NVMCONbits.WR = 0;      // Clear the write action bit
    
//    __builtin_disi(5);      // Disable interrupts for 5 cycles
//    __builtin_writeNVM();   // Unlock reg
    return;
}

