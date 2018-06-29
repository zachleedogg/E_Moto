/*
 * Author: Howard Schlunder
 * Created: 29 March 2017
 *
 * ezbl_lib ButtonRead() implementation for decoding and saving button
 * pushes/releases/toggles from an existing ButtonPeek() implementation.
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

// Must be defined in top level (Application/Bootloader) project either as a
// function or indirectly via the EZBL_DefineButtonMap() macro for use with the
// ButtonPeek() function implemented in ezbl_lib.a. The ButtonPeek() API returns
// the current state of the buttons on the board.
unsigned int ButtonPeek(void);      


unsigned int ButtonsLastState = 0;  // Bitfield holding last sampled absolute button positions
unsigned int ButtonsToggled   = 0;  // Bitfiled of buttons that have changed state since last call to ButtonRead()
unsigned int ButtonsPushed    = 0;  // Bitfield of buttons that have changed from unpushed to pushed since the last call to ButtonRead()
unsigned int ButtonsReleased  = 0;  // Bitfield of buttons that have chagned from pushed to unpushed since the last call to ButtonRead()


/**
 * Reads the state of all buttons and updates the values of the ButtonsLastState
 * ButtonsToggled, ButtonsPushed and ButtonsReleased global bitmask variables.
 * The value returned is a bitmask indicating the present absolute button state
 * where '1' means pushed and '0' means released.
 *
 * For this ezbl_lib function to be used, the ButtonPeak() function must be
 * implemented in the Application to correctly read from the I/O pins and
 * discern pushed/released I/O logic polarity.
 *
 * Each '1' bit in the ButtonsToggled, ButtonsPushed, and ButtonsReleased
 * variables represents an event. For example if ButtonRead() indicates a button
 * that was previously released is now pushed ButtonsToggled and ButtonsPushed
 * will have a '1' stored in the corresponding button bitmask position.
 *
 * A '0' bit will be returned in the bitmask position of all variables 
 * corresponding to a given button if the ButtonRead() API is called while the 
 * user is still holding the button down relative to the last call to 
 * ButtonRead().
 *
 * Upon button release, ButtonsToggled and ButtonsReleased will have a '1'
 * stored in the corresponding button bitmask position.
 *
 * All '1's in the ButtonsPushed and ButtonsReleased fields will revert to '0's
 * when ButtonRead() is called again, making them edge sensitive events rather
 * than level sensitive. 
 * 
 * If you wish to re-obtain the button state without altering the edge events
 * (ex: to test a different bit position), reference the ButtonsLastState
 * variable.  ButtonsLastState is a copy of the last returned value for
 * ButtonRead(), so rather than representing events, indicates the most recent
 * absolute button positions.
 *
 * @return Bitmask representing the buttons that are currently pushed ('1' =
 *         pushed, '0' = released).
 *
 *         Button positions that do not exist are returned as '0' in the
 *         bitmask.
 *
 *         If you only want to read the current button state without altering
 *         the ButtonsLastState tracking + ButtonsToggled, ButtonsPushed and
 *         ButtonsReleased values, then call ButtonPeek() instead of this API.
 *
 *         If ButtonPeek() is not implemented, this function returns 0 and
 *         clears the ButtonsLastState, ButtonsToggled, ButtonsPushed and
 *         ButtonsReleased values.
 */
unsigned int __attribute__((weak)) ButtonRead(void)
{
    // Compute toggled, released, and pushed values
    ButtonsToggled    =  ButtonsLastState ^  ButtonPeek();
    ButtonsPushed     =  ButtonsToggled   & ~ButtonsLastState;
    ButtonsReleased   =  ButtonsToggled   &  ButtonsLastState;
    ButtonsLastState ^= ButtonsToggled;

    return ButtonsLastState;
}
