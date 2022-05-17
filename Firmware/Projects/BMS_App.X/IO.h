/* 
 * File:   IO.h
 * Author: kid group
 *
 * Created on March 10, 2018, 10:04 PM
 */

#ifndef IO_H
#define	IO_H

#include "stdint.h"
#include "pins.h"


/**
 * IO_SET_xxxx will set a digital output to the desired state
 * @param state: HIGH, LOW or TOGGLE (or duty cycle from 0-100)
 */
void IO_SET_DEBUG_LED_EN(uint8_t state);
void IO_SET_SW_EN(uint8_t state);
void IO_SET_DCDC_EN(uint8_t state);
void IO_SET_EV_CHARGER_EN(uint8_t state);
void IO_SET_DISCHARGE_EN(uint8_t state);
void IO_SET_PRE_CHARGE_EN(uint8_t state);
void IO_SET_MUX_A(uint8_t state);
void IO_SET_MUX_B(uint8_t state);
void IO_SET_MUX_C(uint8_t state);
void IO_SET_SPI_CS(uint8_t state);

void IO_SET_CHARGEPUMP_PWM(uint8_t duty);
void IO_SET_CONTACTOR_1_PWM(uint8_t duty);
void IO_SET_CONTACTOR_2_PWM(uint8_t duty);


/**
 * IO_GET_xxxx will return the state of a digital input or output
 * @return: HIGH or LOW
 */
uint8_t IO_GET_DEBUG_LED_EN(void);
uint8_t IO_GET_SW_EN(void);
uint8_t IO_GET_DCDC_EN(void);
uint8_t IO_GET_EV_CHARGER_EN(void);
uint8_t IO_GET_DISCHARGE_EN(void);
uint8_t IO_GET_PRE_CHARGE_EN(void);
uint8_t IO_GET_MUX_A(void);
uint8_t IO_GET_MUX_B(void);
uint8_t IO_GET_MUX_C(void);
uint8_t IO_GET_V12_POWER_STATUS(void);

/**
 * IO_IO_GET_xxx_FAULT returns the Fault Status of a given output. If the fault
 * bit is set, the output has been automatically turned off, and will not reset 
 * until the unit is power cycled or reset.
 * @return HIGH = Faulted, Low = Not Faulted
 */

uint8_t IO_GET_V5_SW_FAULT(void);
uint8_t IO_GET_DCDC_FAULT(void);
uint8_t IO_GET_EV_CHARGER_FAULT(void);

/**
 * IO_GET_xxx_VOLTAGE will return the converted voltage (including divider) in milliVolts
 * @return Voltage in milliVolts
 */
uint32_t IO_GET_ISOLATION_VOLTAGE(void);
uint32_t IO_GET_HV_BUS_VOLTAGE(void);
uint32_t IO_GET_EV_CHARGER_VOLTAGE(void);
uint32_t IO_GET_DCDC_OUTPUT_VOLTAGE(void);
uint32_t IO_GET_MUX_1_VOLTAGE(void);
uint32_t IO_GET_MUX_2_VOLTAGE(void);
uint32_t IO_GET_MUX_3_VOLTAGE(void);

/**
 * IO_GET_xxx_CURRENT will return the converted current (including divider) in milliAmps
 * @return Current in milliAmps
 */
uint32_t IO_GET_DCDC_CURRENT(void);
uint32_t IO_GET_EV_CHARGER_CURRENT(void);
uint32_t IO_GET_SHUNT_HIGH_CURRENT(void);
uint32_t IO_GET_SHUNT_LOW_CURRENT(void);
uint32_t IO_GET_TRANSDUCER_CURRENT(void);





#endif	/* IO_H */

