/*
 * File:   bolt_reset_control.c
 * Author: Zach Levenberg
 *
 * Created on February 10, 2016, 10:23 PM
 */

#include "bolt_reset_control.h"

// Static to keep internal state private to this file
static ResetReason resetReason = RESET_NONE;
static bool resetCaptured = false;

ResetReason GetAndClearResetReasons(void) {
    if (!resetCaptured) {
        uint16_t rcon = RCON;
        RCON = 0;
        resetCaptured = true;

        resetReason = RESET_NONE;

        // Accumulate multiple flags using bitwise OR
        if (rcon & RESET_POR)    resetReason |= RESET_POR;
        if (rcon & RESET_BOR)    resetReason |= RESET_BOR;
        if (rcon & RESET_WDTO)   resetReason |= RESET_WDTO;
        if (rcon & RESET_SWR)    resetReason |= RESET_SWR;
        if (rcon & RESET_EXTR)   resetReason |= RESET_EXTR;
        if (rcon & RESET_TRAPR)  resetReason |= RESET_TRAPR;
        if (rcon & RESET_IOPUWR) resetReason |= RESET_IOPUWR;
    }

    return resetReason;
}