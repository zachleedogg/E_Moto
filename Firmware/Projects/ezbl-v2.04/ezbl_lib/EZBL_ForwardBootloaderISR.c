/*******************************************************************************
  Easy Bootloader for PIC24 and dsPIC33 Library Source File

  Summary:
    EZBL_ForwardBootloaderISR variable definition (separately linkable)

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

#if defined(__XC16__)
/**
 * RAM bitfield controlling/indicating which interrupts that the Bootloader
 * implements should be passed to the Application's interrupt handler or trapped
 * by the Bootloader.
 *
 * Set individual bits to '1' to turn off a Bootloader's ISR and forward the
 * applicable interrupt to the Application's ISR. Setting a bit to '0' causes
 * the Application's ISR to be ignored with execution of the Bootloader's ISR
 * occurring instead. The reset default state of this variable is 0x00000000,
 * meaning all ISRs that the Bootloader implements will be active and all
 * non-bootloader-implemented interrupts will be sent to the Application's
 * ISR (or Application's _DefaultInterrupt() function when unimplemented in both
 * the Bootloader and Application).
 *
 * To disable all asynchronous Bootloader activity and allow the Application to
 * receive all interrupts, set EZBL_ForwardBootloaderISR to all '1's:
 * <code>
 *      EZBL_ForwardBootloaderISR = 0xFFFFFFFF;
 * </code>
 *
 * The exact mapping of bits in this RAM variable to their Interrupt Vector is
 * dynamically generated when the Bootloader project is compiled and depends on
 * the IRQ's natural interrupt order and whether the Bootloader uses the
 * interrupt. Bits are implemented starting from the LSbit of
 * EZBL_ForwardBootloaderISR and from the lowest Vector or IRQ Number, ascending
 * towards the MSb and highest Vector and IRQ numbers.
 *
 * Because it is possible to add or remove ISRs to your Bootloader as you work
 * on it, the flag bit ordering and positions within the
 * EZBL_ForwardBootloaderISR variable could change. Therefore, the Bootloader
 * build sequence exports some global symbols containing the bitmask needed to
 * set or clear individual interrupt flag bits in EZBL_ForwardBootloaderISR.
 *
 * Examples of some available symbol names (assuming the bootloader implements
 * equivalent ISRs) are:
 *      EZBL_FORWARD_MASK_T1
 *      EZBL_FORWARD_MASK_U2TX
 *      EZBL_FORWARD_MASK_U2RX
 *      EZBL_FORWARD_MASK_SI2C1
 *
 * The symbol names are always of the form EZBL_FORWARD_MASK_[ISR_NAME], where
 * [ISR_NAME] corresponds to an abbreviated form of the compiler recognized ISR
 * name. For example, the ISR corresponding to EZBL_FORWARD_MASK_U2TX is called
 * _U2TXInterrupt(). Mask symbol names are abbreviated from the ISR name by
 * removing the leading underscore and trailing "Interrupt" token.
 *
 * To gain access to these symbol values while writing your Bootloader or
 * Application code, you can use the EZBL_SYM32() or EZBL_WeakSYM32()
 * macros. EZBL_WeakSYM32() will return 0 if there is no forwarding mask bit
 * for the given symbol name (i.e. no such ISR in the Bootloader).
 *
 * To forward the U2RX Interrupt to the Application, you may write
 * your "begin forwarding" code as:
 *      <code>
 *      #include "ezbl_integration/ezbl.h"
 *      ...
 *      EZBL_ForwardBootloaderISR |= EZBL_SYM32(EZBL_FORWARD_MASK_U2RX);
 *      </code>
 *
 * To terminate forwarding and let the Bootloader handle the U2RX interrupt,
 * you would clear the same bit without affecting the others:
 *      <code>
 *      EZBL_ForwardBootloaderISR &= ~EZBL_SYM32(EZBL_FORWARD_MASK_U2RX);
 *      </code>
 */
volatile unsigned long __attribute__((near, keep)) EZBL_ForwardBootloaderISR;
extern volatile unsigned long __attribute__((near, keep, alias("EZBL_ForwardBootloaderISR"))) EZBL_IntSel;
#elif defined(__XC32__) || defined(__PIC32__)
#endif
