/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LTC6802__H
#define	LTC6802__H

#include "bolt_spi.h"

void LTC6802_set_CDC(uint8_t cdc);
void LTC6802_set_CELL10(uint8_t cell10);
void LTC6802_set_LVPL(uint8_t lvpl);
void LTC6802_set_GPIO1(uint8_t gpio1);
void LTC6802_set_GPIO2(uint8_t gpio2);
void LTC6802_set_WDT(uint8_t wdt);
void LTC6802_set_cell_discharge(uint8_t cell);
void LTC6802_set_MCI(uint8_t cell);
void LTC6802_set_VUV(float underVoltage);
void LTC6802_set_VOV(float overVoltage);

void LTC6802_writeConfig(void);
void LTC6802_start_all_cell_ADC(void);
uint8_t LTC6802_check_ADC_status(void);
uint8_t LTC6802_read_all_cell_ADC(void);
uint16_t LTC6802_get_cell_voltage(uint8_t cell);


#endif	/* XC_HEADER_TEMPLATE_H */

