/* 
 * File:   can_iso_tp.h
 * Author: kid group
 *
 * Created on April 19, 2022, 12:35 AM
 */

#ifndef CAN_ISO_TP_H
#define	CAN_ISO_TP_H

#include <stdint.h>

typedef enum {
    ISO_TP_NONE,
    ISO_TP_RESET,
    ISO_TP_SLEEP,
    ISO_TP_IO_CONTROL,
    ISO_TP_TESTER_PRESENT
} isoTP_command_E;

void isoTP_init(void);
uint8_t run_iso_tp_basic(void);
uint8_t isoTP_getCommand(void);
uint8_t * get_payload(void);
uint8_t get_payloadLength(void);
#endif	/* CAN_ISO_TP_H */

