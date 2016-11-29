/* 
 * File:   bolt_i2c.h
 * Author: kid group
 *
 * Created on March 14, 2016, 11:57 PM
 */
#include <xc.h>
#include <stdint.h>

#ifndef BOLT_I2C_H
#define	BOLT_I2C_H

#define BAUD_100K 10.0
#define BAUD_400K 2.5
#define BAUD_1M 1.0

#define READ 0x01
#define WRITE 0x00

#define ADDRESS_SPACE_8_BIT 0X00
#define ADDRESS_SPACE_16_BIT 0x01


/**
 * Initialize i2c module number 1, must use alt i2c outputs in config registers
 * @param baudRate can be 1M, 400k, or 100k
 * @return success or failure
 */
uint8_t i2c1_Init(float baudRate); 
uint8_t i2c2_Init(float baudRate);

/**
 * Set the device ID number for the next device to interface with
 * @param deviceID is the unique ID in the device datasheet
 * @return success or failure
 */
uint8_t i2c_SetDeviceID(uint8_t deviceID);

/**
 * Sets the address to read or write to in the device specified by Device ID
 * @param address: the 8 or 16 bit address
 * @param addressSpace: 8 or 16 bit specifier
 * @return 
 */
uint8_t i2c_SetAddress(uint16_t address, uint8_t addressSpace);

/**
 * Writes to the device.
 * @param message: String data to write into TX buffer to device
 * @param numberOfBytes: length of data string
 * @return success or failure
 */
uint8_t i2c1_Write(char *message, uint8_t numberOfBytes);
uint8_t i2c2_Write(char *message, uint8_t numberOfBytes);

/**
 * Reads from device
 * @param message: Pointer to return string data
 * @param numberOfBytes: length of data to read
 * @return success or failure
 */
uint8_t i2c1_Read(char *message, uint8_t numberOfBytes);
uint8_t i2c2_Read(char *message, uint8_t numberOfBytes);

/**
 * When true, the data last read using i2cRead() is ready for use.
 * @return True or False
 */
uint8_t i2c1_ReadReady(void);
uint8_t i2c_ReadReady(void);


#endif	/* BOLT_I2C_H */

