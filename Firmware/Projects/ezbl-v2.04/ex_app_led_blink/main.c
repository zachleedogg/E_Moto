/**
 * File:   main.c
 *
 * Example LED blinking application to demonstrate successful building of an 
 * Application containing an EZBL Bootloader and also how Bootloader functions
 * can be called directly from an Application project (with appropriate headers
 * for function prototypes and extern variable declarations).
 *
 */

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

#include <xc.h>
#include "ezbl_integration/ezbl.h"


// Optional App defined Bootloader callback to reject or accept w/safe App shutdown in response to a background external firmware update offer
int EZBL_BootloadRequest(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer);


//__prog__ unsigned int __attribute__((space(prog), keep, unused)) dummy[1024];     // Dummy items in program memory for bootloader testing purposes. This declares 3072 bytes/0x800 program addresses, 1024 instruction words worth of Flash data.
//__prog__ unsigned int __attribute__((space(prog), keep, unused)) dummy2[10000];   // Dummy items in program memory for bootloader testing purposes. This declares 30,000 bytes/0x2710 program addresses, 10,000 instruction words worth of Flash data.
//__prog__ char         __attribute__((space(prog), keep, unused)) dummy3[0x800];   // Dummy items in program memory for bootloader testing purposes. This declares 3,072 bytes/0x800 program addresses, 1024 instruction words worth of Flash data.
//__prog__ char         __attribute__((space(prog), keep, unused)) dummy4[0x4000];  // Dummy items in program memory for bootloader testing purposes. This declares 24,576 bytes/0x4000 program addresses, 8192 instruction words worth of Flash data.



int main(void)
{
    unsigned long ledTimer;

    // Connect Bootloader's EZBL_AppPreInstall function pointer to our
    // EZBL_BootloadRequest function. If you delete this line, the Bootloader
    // will decide how to handle background Bootload requests (default: kill App
    // mid execution, accept firmware update and erase this App).
    EZBL_AppPreInstall = EZBL_BootloadRequest;

//    // Optionally take over the hardware communications interrupts if we want
//    // to disable Bootloader implemented software FIFOing ISRs for them. Be sure
//    // and implement a matching _[UxRX/UxTX/SI2Cx]Interrupt() ISR function or
//    // set _U2RXIE/_U2TXIE/_SI2C1IE/etc. = 0; to disable the interrupt before
//    // forwarding it to the App as the effect is immediate. Taking Bootloader
//    // communications interrupts will disable the Bootloader background ability
//    // to start a firmware update in response to externally initiated firmware
//    // update offerings.
//    EZBL_ForwardIntToApp(U2RX);     // UART 2 RX
//    EZBL_ForwardIntToApp(U2TX);     // UART 2 TX
//    EZBL_ForwardIntToApp(SI2C1);    // Slave I2C 1 Interrupt

//    // Optionally take the NOW Timer/CCT interrupt from the Bootloader. If you
//    // do this, all NOW_TASKs will stop (Bootloader won't be able to do anything
//    // in the background) and it will then be unsafe to call NOW/timing or
//    // communications related functions in the Bootloader or ezbl_lib.a. Ex:
//    // NOW_64()/NOW_32(), EZBL_FIFOWrite()/EZBL_FIFORead()/EZBL_FIFOPeek()
//    // against EZBL_FIFOs with onRead/onWrite callback functions implementing
//    // blocking timeouts, EZBL_printf(), etc. It is recommended that you also
//    // take control of the communications interrupts if you take the timer.
//    EZBL_ForwardIntToApp(T1);


//    // Optionally let the world know that we are not a brick! Whoohoo!
//    EZBL_FIFOSetBaud(EZBL_STDOUT, -230400);
//    EZBL_printf("\x0F\n\n\nApp v%u.%02u.%04lu starting\n", EZBL_appIDVer.appIDVerMajor, EZBL_appIDVer.appIDVerMinor, EZBL_appIDVer.appIDVerBuild); // \x0F is a SI "Shift In" control code so terminal emulators go back to the correct character set if they got changed while we were being programmed or reset.


    ledTimer = NOW_32();
    while(1)
    {
        // Every 500ms toggle an LED
        if(NOW_32() - ledTimer > NOW_sec/2u)
        {
            ledTimer += NOW_sec/2u;
            LEDToggle(0x01);
        }
        Idle();
    }
}


/**
 * Optional callback function invoked when the EZBL Bootloader wants to
 * terminate and erase this Application. Use this function to safely shut down
 * any ongoing hardware operations or reject the offered firmware image and
 * prevent this application from being erased.
 *
 * If last minute notification or rejection opportunity is not needed or
 * applicable for the bootloader in use, this function can be removed and the
 * EZBL_AppPreInstall pointer should be left unreferenced.
 *
 * @param *rxFromRemote Pointer to the RX EZBL_FIFO applicable to this offering.
 *
 *                      If the offering medium does not have an RX EZBL_FIFO
 *                      associated with it (Ex: FILEIO/USB thumb drive or local
 *                      memory type Bootloaders), then this pointer is null.
 *
 * @param *txToRemote Pointer to the TX EZBL_FIFO applicable to this offering.
 *
 *                    If the offering medium does not have a TX EZBL_FIFO
 *                    associated with it (Ex: FILEIO/USB thumb drive or local
 *                    memory type Bootloaders), then this pointer is null.
 *
 * @param fileSize Total file size of the .bl2 file being offered, including
 *                 the SYNC bytes and .bl2 header.
 *
 * @param *appIDVer Pointer to the EZBL_APPID_VER data contained in the offered
 *                  .bl2 file header.
 *
 *                  By the time this callback is called, the BOOTID_HASH has
 *                  already been confirmed as a match for the Bootloader.
 *                  Additionally, if the EZBL_NO_APP_DOWNGRADE feature is
 *                  enabled, the offered APPID_VER value has already been
 *                  confirmed acceptable before this callback occurs.
 *
 * @return Return 0 to reject the offered firmware image or 1 to accept the
 *         image and proceed with erase/programming steps.
 *
 *         NOTE: When this callback is executed, the remote node (if applicable)
 *         is left waiting and will timeout if a response takes too long to
 *         arrive. To ensure a communications timeout does not occur, limit code
 *         in this callback to less than the remote node's communications
 *         timeout (typically around 1 second, but configurable and not
 *         applicable for local memory bootloaders).
 */
int EZBL_BootloadRequest(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer)
{
    // Implement any Application shutdown code here. The Bootloader is about to
    // disable all interrupts and start erasing the flash.

    return 1;   // 1 = Allow erase/programming, this App dies. 0 = Reject the firmware update, keeping this App running unharmed.
}


// Optional trap handler.
//
// For a trap handler that dumps useful debugging data to stdout (such as the
// exception return address, stack and frame pointers, stack contents,
// w-registers, RAM contents, etc.), delete this _DefaultInterrupt() ISR and
// instead use (at file-level scope):
//     EZBL_KeepSYM(EZBL_TrapHandler);
// Ensure that UART_Reset() is called before any trap occurs to initialize the
// EZBL_STDOUT pointer and that interrupts for the FIFO stay forwarded to the
// correct ISR handler (communications ISRs are polled when masked by IPL8+ to
// still TX progression.
void __attribute__((interrupt, no_auto_psv)) _DefaultInterrupt(void)
{
    // If this interrupt is unexpected, you should check all of the INTCON1 bits
    // to see if a trap is responsible.
    __builtin_software_breakpoint();

    // Clear trap(s), if any, so we can return to see where the PC was when this
    // interrupt was triggered
    INTCON1 &= 0x8700;
}

