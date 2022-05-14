/* 
 * File:   utils.h
 * Author: kid group
 *
 * Created on January 8, 2022, 1:34 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <stdint.h>
#include <stddef.h>

/**
 * Helper function to extract stuffed bits within a CAN data frame
 * @param payload: pointer to a CAN_payload_S type
 * @param offset: number of bits from start of 8-byte message
 * @param range: number of bit length of the signal
 * @param wordWidth: Width of 
 * @return the extracted signal
 */
uint16_t get_bits(size_t * payload, uint8_t offset, uint8_t range);


/**
 * 
 * @param payload
 * @param offset
 * @param range
 * @param value
 * @return 
 */
uint16_t set_bits(size_t * payload, uint8_t offset, uint8_t range, uint16_t value);


uint8_t crc8ccitt(uint8_t * data, uint8_t size);


#endif //UTILS_H