/**
 * EZBL Bootloader "main" file for initializing communications hardware,
 * checking app-installed flags, receiving new application firmware, and
 * dispatching execution to the Application's reset/main() routines.
 *
 * Interrupt Service Routines implemented in this project are:
 *  - One 16-bit Timer or CCT Interrupt, selected in the hardware initializer file (ISR implementation defined in ezbl_lib.a, see ezbl_lib -> weak_defaults/_TxInterrupt.s (or _CCTxInterrupt.s))
 *  - UART 2 RX (defined in ezbl_lib.a, see ezbl_lib -> weak_defaults/uart2_fifo.c; can be a different UART instance, see UART_Reset() in hardware initializer)
 *  - UART 2 TX (defined in ezbl_lib.a, see ezbl_lib -> weak_defaults/uart2_fifo.c; can be a different UART instance, see UART_Reset() in hardware initializer)
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
#include <stdlib.h>
#include <libpic30.h>
#include <string.h>
#include <stdarg.h>
#include "ezbl_integration/ezbl.h"


// EZBL ezbl_lib.a link-time options:
//EZBL_SetSYM(EZBL_NO_APP_DOWNGRADE, 1);    // Uncomment to disallow upload of App image with a version/build number that is < existing App version (assuming a valid App exists). Note: This can be circumvented if someone starts uploading a recent App image, they interrupt power or communications, then upload older firmware. With nowhere to store an offered Application but in an erased flash, the act of starting a valid upload results in prior knowledge of which older version(s) are disallowed.
//EZBL_SetSYM(EZBL_FLOW_THRESHOLD, 48);     // Optional water mark to adjust when outbound flow control advertisements are generated. Small values consume more TX bandwidth by generating more messages. See EZBL_Install2Flash() API documentation for additional information.
//EZBL_SetSYM(UART2_TX_ISR_PRIORITY, 1);    // Optionally change the U2TX Interrupt priority. The default is IPL 1 (lowest possible priority).
//EZBL_SetSYM(UART2_RX_ISR_PRIORITY, 2);    // Optionally change the U2RX Interrupt priority. The default is IPL 2 (low priority).


// General Bootloader timeout governing reset idle time to initial Application
// launch and bootload termination when communications are broken.
#define BOOTLOADER_TIMEOUT          (NOW_sec * 1u)


// Uncomment for superior performance (with baud rates up to 761kbps). Buffer
// default size is 96 bytes in ezbl_lib, which conserves static RAM, but reduces 
// sustained throughput. Any size >= 64 bytes can be used for bootloading.
//unsigned char UART2_RxFifoBuffer[1024] __attribute__((section(".bss.UART2_RxFifoBuffer")));


// Bootloader global variables and flags
NOW_TASK            EZBL_bootloaderTask __attribute__((persistent));    // Task callback pointers/timers to background execute the EZBL_BootloaderTask() function
EZBL_INSTALL_CTX    EZBL_bootCtx        __attribute__((persistent));    // State tracking structure, including timeout detection for active bootload operations
int (*EZBL_AppPreInstall)(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer) = 0;    // Application's EZBL_PreInstall() callback function pointer to permit current App rejection of offered firmware or clean App shutdown



int main(void)
{
    unsigned long appLaunchTimer;   // Timestamp tracking when we last saw COM/Bootloader activity
    unsigned long ledTimer;         // Timestamp tracking when we last toggled an LED
    unsigned long now;              // Cached return value from NOW_32()


    // Initialize Bootloader global variables and flags. Explicit zeroing is
    // necessary if this Bootloader project is built with "--no-data-init"
    // xc16-ld Linker option used (i.e. "Initialize data sections" unchecked to
    // save code space). For default (initialization enabled) state, this code
    // could be removed.
    EZBL_ForwardBootloaderISR = 0x00000000;
    EZBL_appIsRunning = 0;
    EZBL_NVMKey = 0x0000;
    EZBL_COM_RX = 0;
    EZBL_AppPreInstall = 0;


    // Set processor clock, configure LED/Button I/O pin states, PPS
    // assignment, initialize NOW timer peripheral, UART/I2C communications, etc.
    InitializeBoard();

    // Launch Bootloader communications listener and command processor periodic
    // task (background calls EZBL_BootloaderTask() from IPL0/main() context
    // every ~32ms). EZBL_Install2Flash() API start requirements require that we
    // first initialize EZBL_bootCtx state.
    EZBL_bootCtx.state = SM_SYNC_INIT;
    EZBL_bootCtx.timeout = BOOTLOADER_TIMEOUT;
    NOW_CreateRepeatingTask(&EZBL_bootloaderTask, EZBL_BootloaderTask, 32u*NOW_ms);


    now = NOW_32();
    appLaunchTimer = now;
    ledTimer = now;


    // Main Bootloader LED blink and App dispatch timeout loop
    while(1)
    {
        ClrWdt();

        // Every 62.5ms toggle a heartbeat LED (8 Hz blink rate) indicating this Bootloader is executing
        now = NOW_32();
        if(now - ledTimer > NOW_sec/16u)
        {
            ledTimer += NOW_sec/16u;
            LEDToggle(0x01);            // LEDToggle() needs to be defined in this Bootloader project, or default implementation in ezbl_lib will be used. Default implementation accesses LATA/PORTA.
        }

        // Check if it is time to jump into the application (default is 1 second
        // of nothing being received, as defined by BOOTLOADER_TIMEOUT.
        if(EZBL_COMBootIF->activity.any)
        {   // Activity happened, so reset launch timer to full interval remaining
            EZBL_COMBootIF->activity.any = 0x0000;
            appLaunchTimer = now;
        }
        if(now - appLaunchTimer > EZBL_bootCtx.timeout)
        {
            // If auto-baud is used, automatically reset the UART back to 
            // auto-baud mode while idle for longer than the timeout.
            if(EZBL_COMBaud <= 0)
            {
                EZBL_FIFOSetBaud(EZBL_COMBootIF, EZBL_COMBaud); // This will set EZBL_COMBootIF->activity.other, so appLaunchTimer will also be reset on next loop iteration
            }
            if(EZBL_IsAppPresent())
            {
                LEDOff(0xFF);           // Executes LEDToggle(0x00FF & LEDToggle(0)); to turn off all LEDs we were blinking
                
                // Optionally turn off all Bootloader ISRs and forward the
                // interrupts to the Application so we become a passive classic
                // bootloader.
                // NOTE: You are giving up useful timing and communications APIs
                // if you do this. Also, the automatic bootloader wakeup upon
                // .bl2 file presentation won't work. To minimize flash, the
                // App can just reuse the bootloader APIs as is, keeping the
                // NOW Timer interrupt and communications ISRs active in the
                // background (which have minimal run-time execution cost).
                //EZBL_RAMSet((void*)&IEC0, 0x00, (unsigned int)&IPC0 - (unsigned int)&IEC0);   // Clear every bit in all IECx Interrupt Enable registers
                //EZBL_ForwardBootloaderISR = 0xFFFFFFFF;                                       // Forward all Interrupts to the Application
                //NOW_EndAllTasks();                                                            // Terminates the EZBL_BootloaderTask() from executing via background NOW ISR and when a NOW_32() or NOW_64() call is made which indirectly needs the timer ISR to execute and perform a carry propagation.

                EZBL_StartAppIfPresent();   // Sets EZBL_appIsRunning = 0xFFFF and temporarily disables IPL6 and lower interrupts before launching Application
            }
        }

        // Not much going on in this loop. Let's save power. The NOW Timer ISR
        // will wake us up, as will COM RX events.
        Idle();
    }
}


/**
 * Periodically called EZBL bootloader task callback function. This function
 * executes at IPL 0 (main() context), essentially acting like a virtual
 * software ISR created by the NOW_CreateRepeatingTask() call.
 *
 * NOTE: Unless you disable the NOW interrupt and forward it to the Application,
 * this function will continue to execute in the background even while the
 * Application is running. This allows the Bootloader to wake up and process an
 * offered .bl2 image file at any time. This task can be manually ended by 
 * calling EZBL_EndTask(&EZBL_bootCtx) from this Bootloader or the Application
 * project.
 *
 * @return 0 - value not used.
 */
int EZBL_BootloaderTask(void)
{
    // Do bootloader processing when the communications RX ISR has detected the
    // BOOTLOADER_WAKE_KEY sequence (default "MCUPHCME") has been received on
    // a FIFO'd communications interface and set the EZBL_COM_RX (and
    // EZBL_COM_TX) global pointers to a non-null value.
    if(EZBL_COM_RX)
    {
        // Run the EZBL state machine that reads from the communications port via
        // EZBL_COM_RX, responds via EZBL_COM_TX, handles flow control, erases flash
        // (only when valid firmware offering presented and EZBL_PreInstall()
        // callback returns 1), programs flash, validates with CRC and read-back,
        // then programs a App-valid flag to allow new Application execution. For
        // the first call, the EZBL_INSTALL_CTX.state, and timeout fields must be
        // pre-configured (done during main() initialization):
        //     EZBL_bootCtx.state = SM_SYNC_INIT;
        //     EZBL_bootCtx.timeout = BOOTLOADER_TIMEOUT;
        EZBL_Install2Flash(&EZBL_bootCtx);

        // When Bootloader is communicating, increase task frequency for lower
        // communications latency and better throughput. EZBL_Install2Flash() sets
        // EZBL_COM_RX to null when a rejection, timeout, error or successful
        // bootload completion occurs.
        NOW_SetNextTaskTime(&EZBL_bootloaderTask, NOW_ms);
    }

    if(EZBL_COMBaud <= 0)
    {
        if(EZBL_COMBootIF->activity.framingError)           // On framing errors or Break characters, revert back to auto-baud, even if the Application has already been launched
            EZBL_FIFOSetBaud(EZBL_COMBootIF, EZBL_COMBaud); // This will set EZBL_COMBootIF->activity.other = 1 (and clear activity.framingError), so appLaunchTimer will also be reset on next bootloader main() while(1) loop iteration.
    }

    return 0;
}


// Callback function executed by EZBL_Install2Flash() just before the existing
// Application (if any) is erased, permitting last minute rejection of a
// firmware update or an opportunity to cleanly shut down any running operations
// (especially if they might interfere with successful Bootloading). See
// callback documentation for EZBL_PreInstall() in ezbl.h for more information.
int EZBL_PreInstall(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer)
{
    if(EZBL_AppPreInstall)
        return (*EZBL_AppPreInstall)(rxFromRemote, txToRemote, fileSize, appIDVer);

    return 1; // Accepts update offering if no App installed or running right now
}


// These trap handlers are not necessary, but if you ever find your Bootloader
// going out to lunch (and probably crashing a debug session with
// PICkit/ICD/REAL ICE), then be sure to enable these. When absent from the
// bootloader project, EZBL remaps all of the trap vectors to the Application's
// Interrupt Goto Table, which will not help you debug Bootloader problems (and
// confuses the debuggers because there is usually nothing in App flash space at
// the time, resulting in NOPR execution until the end of flash is hit -- which
// triggers another unreachable trap.)
#if defined(__DEBUG)
void __attribute__((interrupt, auto_psv)) _AddressError(void) // Consider EZBL_KeepSYM(_AddressError); for a more powerful trap handler
{
    EZBL_printf("\n\nAddress Error Trap!");
    EZBL_FIFOFlush(EZBL_STDOUT, NOW_sec);
    __builtin_software_breakpoint();

    INTCON1 &= 0x8700;
}
void __attribute__((interrupt, auto_psv)) _StackError(void) // Consider EZBL_KeepSYM(_StackError); for a more powerful trap handler
{
    EZBL_printf("\n\nStack Error Trap!");
    EZBL_FIFOFlush(EZBL_STDOUT, NOW_sec);
    __builtin_software_breakpoint();

    INTCON1 &= 0x8700;
}
void __attribute__((interrupt, auto_psv)) _MathError(void) // Consider EZBL_KeepSYM(_MathError); for a more powerful trap handler
{
    EZBL_printf("\n\nMath Error Trap!");
    EZBL_FIFOFlush(EZBL_STDOUT, NOW_sec);
    __builtin_software_breakpoint();

    INTCON1 &= 0x8700;
}
#endif
