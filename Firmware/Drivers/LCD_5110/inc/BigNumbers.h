/*
 * File:   BigNumbers.h
 * Author: garrettd
 *
 * Created on February 16, 2016, 3:53 PM
 */

#ifndef BIGNUMBERS_H
#define	BIGNUMBERS_H

#include <stdint.h>

//Writes big numbers
void BigZeroNumber(uint8_t xCursorNum, uint8_t yCursorNum);
void BigOneNumber(uint8_t xCursorNum, uint8_t yCursorNum);
void BigTwoNumber(uint8_t xCursorNum, uint8_t yCursorNum);
void BigThreeNumber(uint8_t xCursorNum, uint8_t yCursorNum);
void BigFourNumber(uint8_t xCursorNum, uint8_t yCursorNum);
void BigFiveNumber(uint8_t xCursorNum, uint8_t yCursorNum);
void BigSixNumber(uint8_t xCursorNum, uint8_t yCursorNum);
void BigSevenNumber(uint8_t xCursorNum, uint8_t yCursorNum);
void BigEightNumber(uint8_t xCursorNum, uint8_t yCursorNum);
void BigNineNumber(uint8_t xCursorNum, uint8_t yCursorNum);
void BigBlankNumber(uint8_t xCursorNum, uint8_t yCursorNum);

//Converts a uint8_t to a decimal and outputs on screen
void LCDBigConversion(double putNumHere, uint8_t TheXCursor, uint8_t TheYCursor);

#endif	/* BIGNUMBERS_H */

