/* 
 * File:   reset_control.h
 * Author: Zachary Levenberg
 * Comments: zzzzzzzzzzzzzzzzz
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef RESET_CONTROL_H
#define	RESET_CONTROL_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    RESET_NONE           = 0x0000,
    RESET_POR            = _RCON_POR_MASK,
    RESET_BOR            = _RCON_BOR_MASK,
    RESET_WDTO           = _RCON_WDTO_MASK,
    RESET_SWR            = _RCON_SWR_MASK,
    RESET_EXTR           = _RCON_EXTR_MASK,
    RESET_TRAPR          = _RCON_TRAPR_MASK,
    RESET_IOPUWR         = _RCON_IOPUWR_MASK
} ResetReason;

ResetReason GetAndClearResetReasons(void);

#endif	/* RESET_CONTROL_H */

