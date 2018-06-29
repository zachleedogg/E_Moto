/*******************************************************************************
  Easy Bootloader for PIC24 and dsPIC33 Library API Source File

  Summary:
    Writes a selectable group of system SFRs with reset default values to
    emulate device reset. Useful in hiding Bootloader execution state before
    launching an Application, allowing the Application to start in a predictable
    environment.

 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Copyright (C) 2017 Microchip Technology Inc.

  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
  derivatives created by any person or entity by or on your behalf, exclusively
  with Microchip's products.  Microchip and its licensors retain all ownership
  and intellectual property rights in the accompanying software and in all
  derivatives here to.

  This software and any accompanying information is for suggestion only.  It
  does not modify Microchip's standard warranty for its products.  You agree
  that you are solely responsible for testing the software and determining its
  suitability.  Microchip has no obligation to modify, test, certify, or
  support the software.

  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
  TERMS.
*******************************************************************************/
// DOM-IGNORE-END

extern volatile unsigned int IEC0;
extern volatile unsigned int IFS0;
extern volatile unsigned int IPC0;
extern volatile unsigned int RPOR0;
extern volatile unsigned int RPINR0;
extern volatile unsigned int PMD1;
extern volatile unsigned int TRISA;
extern volatile unsigned int TRISG;
extern volatile unsigned int INTCON1;
extern volatile unsigned int NVMCON;
extern volatile unsigned long EZBL_ForwardBootloaderISR;


void EZBL_RAMSet(void *dest, int value, unsigned int length);
#if defined(__XC16__)
#define EZBL_RAMSet(dest, value, length)  /* macro for inline compiler memset() when length is constant */    \
    (__builtin_constant_p(length) ? __builtin_memset(dest, value, length) : EZBL_RAMSet(dest, value, length))
#endif


/**
 * Resets various system SFRs back to their hardware reset state. This is often
 * useful to passivate an EZBL bootloader to make it look as transparent as
 * possible to an Application project that desires full control of the system.
 *
 * @param itemsToReset 
 *      Collection of bit flag constants specifying what should be reset. A
 *      bit-wise OR'd combination of the following values are supported:
 *      <ul>
 *          <li>EZBL_RST_INT_EN - All Interrupt Enable (IECx) registers cleared
 *                                to 0x0000</li>
 *          <li>EZBL_RST_PORTIO - All TRISx/ANSELx registers are set to 0xFFFF
 *                                and all ODCx/CNENx/CNPUx/CNPDx registers
 *                                cleared to 0x0000 (unimplemented bits will
 *                                read-back as '0's)</li>
 *          <li>EZBL_RST_PPS - All Peripheral Pin Select RPORx/RPINRx registers
 *                             are cleared to 0x0000 to disconnect all inputs
 *                             and outputs from RPx/RPIx pins.</li>
 *          <li>EZBL_RST_PMD - All PMDx registers are set to 0xFFFF and then
 *                             cleared back to 0x000. This will turn off all
 *                             peripherals and set their SFRs back to hardware
 *                             reset states.</li>
 *          <li>EZBL_RST_INT - All Interrupt Enable (IECx) registers cleared to
 *                             0x0000, Interrupt Flag (IFSx) registers cleared
 *                             to 0x0000, and Interrupt Priority (IPCx)
 *                             registers rewritten with 0x4444 to restore IPL4
 *                             priorities.</li>
 *          <li>EZBL_RST_INT_FORWARDING - EZBL_ForwardBootloaderISR is written
 *                                        with 0xFFFFFFFF to relinquish all
 *                                        Interrupt processing to an Application
 *                                        instead of the Bootloader.</li>
 *      </ul>
 */

typedef struct
{
    unsigned int intEn:1;           // All IECx Interrupt Enable registers written to 0x0000
    unsigned int portIO:1;          // All TRISx/ANSELx Port I/O registers set to 0xFFFF and all ODCx/CNENx/CNPUx/CNPDx registers cleared to 0x0000
    unsigned int PPS:1;             // All RPORx/RPINRx Peripheral Pin Select registers cleared to 0x0000
    unsigned int peripherals:1;     // All PMDx Peripheral Module Disable bits set and then cleared to turn off and reset all peripheral SFRs
    unsigned int interrupts:1;      // All IECx Interrupt Enable + IFSx Interrupt Flag registers written with 0x0000, then all IPCx Interrupt Priority registers written to 0x4444
    unsigned int ezblIntForwarding:1;   // EZBL_ForwardBootloaderISR written to 0xFFFFFFFF to pass all Interrupts to the Application without any Bootloader handlers executing
    unsigned int :10;
} EZBL_SFR_RESET_OPTIONS;

void EZBL_SFRReset(EZBL_SFR_RESET_OPTIONS itemsToReset)
{
    unsigned int sfrAddr;

    if(itemsToReset.intEn || itemsToReset.interrupts)
        EZBL_RAMSet((void*)&IEC0, 0x00, ((unsigned int)&IPC0) - ((unsigned int)&IEC0));     // Clear all IECx registers to disable all Interrupts
    if(itemsToReset.portIO)
    {
        for(sfrAddr = (unsigned int)&TRISA; sfrAddr <= (unsigned int)&TRISG; sfrAddr += 0x10)
        {
            *((volatile unsigned int*)(sfrAddr+0x0)) = 0xFFFF;                              // Write 0xFFFF to all TRISx registers
            *((volatile unsigned int*)(sfrAddr+0xE)) = 0xFFFF;                              // Write 0xFFFF to all ANSELx registers
            EZBL_RAMSet((void*)(sfrAddr+0x4), 0x00, 0xA);                                   // Write 0x0000 to all LATx, ODCx, CNENx, CNPUx, and CNPDx registers
        }
    }
    if(itemsToReset.PPS)
        EZBL_RAMSet((void*)&RPOR0, 0x00, ((unsigned int)&NVMCON) - ((unsigned int)&RPOR0)); // Deassign all PPS outputs and inputs
    if(itemsToReset.peripherals)
    {
        EZBL_RAMSet((void*)&PMD1, 0xFF, ((unsigned int)&IFS0) - ((unsigned int)&PMD1));     // Set all Peripheral Module Disable bits to make peripheral SFRs reset
        EZBL_RAMSet((void*)&PMD1, 0x00, ((unsigned int)&IFS0) - ((unsigned int)&PMD1));     // Restore peripheral existence
    }
    if(itemsToReset.interrupts)
    {
        EZBL_RAMSet((void*)&IPC0, 0x44, ((unsigned int)&INTCON1) - ((unsigned int)&IPC0));  // Reset all IPCx registers to 0x4444 to restore default ISR priority to IPL 4
        EZBL_RAMSet((void*)&IFS0, 0x00, ((unsigned int)&IEC0) - ((unsigned int)&IFS0));     // Clear all IFSx registers to clear all Interrupt flags
    }

    if(itemsToReset.ezblIntForwarding)
        EZBL_ForwardBootloaderISR = 0xFFFFFFFF;                                         // Forward any Interrupts with ISRs implemented in this Bootloader to the Application instead
}
