/*
 * File:   BigNumbers.c
 * Author: garrettd
 *
 * Created on February 16, 2016, 3:54 PM
 */


#include <xc.h>
#include "BigNumbers.h"
#include "LCD_SPI.h"
#include "bolt_uart.h"

static const uint8_t BigZeroTop[] = {0xFE, 0xFC, 0xF9, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0xF9, 0xFC, 0xFE};
static const uint8_t BigZeroMid[] = {0xFB, 0xF1, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF1, 0xFB};
static const uint8_t BigZeroBtm[] = {0x0F, 0x07, 0x13, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x18, 0x13, 0x07, 0x0F};

void BigZeroNumber(uint8_t xCursorNum, uint8_t yCursorNum){//May need to change parameters to put zero anywhere
    //Writes a big zero on LCD Screen
    LCDCursor(xCursorNum, yCursorNum);//row 0
    LCDWriteData((uint8_t*)BigZeroTop, 13);
    LCDCursor(xCursorNum, yCursorNum+1);//row 1
    LCDWriteData((uint8_t*)BigZeroMid, 13);
    LCDCursor(xCursorNum, yCursorNum+2);//row 2
    LCDWriteData((uint8_t*)BigZeroBtm, 13);

}

static const uint8_t BigOneTop[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFC, 0xFE};
static const uint8_t BigOneMid[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF1, 0xFB};
static const uint8_t BigOneBtm[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F};

void BigOneNumber(uint8_t xCursorNum, uint8_t yCursorNum){
    //Writes a big one on the LCD screen
    LCDCursor(xCursorNum, yCursorNum);//row 0
    LCDWriteData((uint8_t*)BigOneTop, 13);
    LCDCursor(xCursorNum, yCursorNum+1);//row 1
    LCDWriteData((uint8_t*)BigOneMid, 13);
    LCDCursor(xCursorNum, yCursorNum+2);//row 2
    LCDWriteData((uint8_t*)BigOneBtm, 13);
}

static const uint8_t BigTwoTop[] = {0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0xF9, 0xFC, 0xFE};
static const uint8_t BigTwoMid[] = {0xF8, 0xF0, 0xE4, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x04, 0x00, 0x01, 0x03};
static const uint8_t BigTwoBtm[] = {0x0F, 0x07, 0x13, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x18, 0x10, 0x00, 0x00};

void BigTwoNumber(uint8_t xCursorNum, uint8_t yCursorNum){
    //Writes a big two on the LCD screen
    LCDCursor(xCursorNum, yCursorNum);//row 0
    LCDWriteData((uint8_t*)BigTwoTop, 13);
    LCDCursor(xCursorNum, yCursorNum+1);//row 1
    LCDWriteData((uint8_t*)BigTwoMid, 13);
    LCDCursor(xCursorNum, yCursorNum+2);//row 2
    LCDWriteData((uint8_t*)BigTwoBtm, 13);

}

static const uint8_t BigThreeTop[] = {0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0xF9, 0xFC, 0xFE};
static const uint8_t BigThreeMid[] = {0x00, 0x00, 0x04, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xE4, 0xF1, 0xFB};
static const uint8_t BigThreeBtm[] = {0x00, 0x00, 0x10, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x18, 0x13, 0x07, 0x0F};

void BigThreeNumber(uint8_t xCursorNum, uint8_t yCursorNum){
    //Writes a big three on the LCD screen
    LCDCursor(xCursorNum, yCursorNum);//row 0
    LCDWriteData((uint8_t*)BigThreeTop, 13);
    LCDCursor(xCursorNum, yCursorNum+1);//row 1
    LCDWriteData((uint8_t*)BigThreeMid, 13);
    LCDCursor(xCursorNum, yCursorNum+2);//row 2
    LCDWriteData((uint8_t*)BigThreeBtm, 13);

}

static const uint8_t BigFourTop[] = {0xFE, 0xFC, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFC, 0xFE};
static const uint8_t BigFourMid[] = {0x03, 0x01, 0x04, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xE4, 0xF1, 0xFB};
static const uint8_t BigFourBtm[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F};

void BigFourNumber(uint8_t xCursorNum, uint8_t yCursorNum){
    //Writes a big four on the LCD screen
    LCDCursor(xCursorNum, yCursorNum);//row 0
    LCDWriteData((uint8_t*)BigFourTop, 13);
    LCDCursor(xCursorNum, yCursorNum+1);//row 1
    LCDWriteData((uint8_t*)BigFourMid, 13);
    LCDCursor(xCursorNum, yCursorNum+2);//row 2
    LCDWriteData((uint8_t*)BigFourBtm, 13);

}

static const uint8_t BigFiveTop[] = {0xFE, 0xFC, 0xF9, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00};
static const uint8_t BigFiveMid[] = {0x03, 0x01, 0x04, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xE4, 0xF0, 0xF8};
static const uint8_t BigFiveBtm[] = {0x00, 0x00, 0x10, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x18, 0x13, 0x07, 0x0F};

void BigFiveNumber(uint8_t xCursorNum, uint8_t yCursorNum){
    //Writes a big five on the LCD screen
    LCDCursor(xCursorNum, yCursorNum);//row 0
    LCDWriteData((uint8_t*)BigFiveTop, 13);
    LCDCursor(xCursorNum, yCursorNum+1);//row 1
    LCDWriteData((uint8_t*)BigFiveMid, 13);
    LCDCursor(xCursorNum, yCursorNum+2);//row 2
    LCDWriteData((uint8_t*)BigFiveBtm, 13);

}

static const uint8_t BigSixTop[] = {0xFE, 0xFC, 0xF9, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00};
static const uint8_t BigSixMid[] = {0xFB, 0xF1, 0xE4, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xE4, 0xF0, 0xF8};
static const uint8_t BigSixBtm[] = {0x0F, 0x07, 0x13, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x18, 0x13, 0x07, 0x0F};

void BigSixNumber(uint8_t xCursorNum, uint8_t yCursorNum){
    //Writes a big six on the LCD screen
    LCDCursor(xCursorNum, yCursorNum);//row 0
    LCDWriteData((uint8_t*)BigSixTop, 13);
    LCDCursor(xCursorNum, yCursorNum+1);//row 1
    LCDWriteData((uint8_t*)BigSixMid, 13);
    LCDCursor(xCursorNum, yCursorNum+2);//row 2
    LCDWriteData((uint8_t*)BigSixBtm, 13);
}

static const uint8_t BigSevenTop[] = {0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0xF9, 0xFC, 0xFE};
static const uint8_t BigSevenMid[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF1, 0xFB};
static const uint8_t BigSevenBtm[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F};

void BigSevenNumber(uint8_t xCursorNum, uint8_t yCursorNum){
    //Writes a big seven on the LCD screen
    LCDCursor(xCursorNum, yCursorNum);//row 0
    LCDWriteData((uint8_t*)BigSevenTop, 13);
    LCDCursor(xCursorNum, yCursorNum+1);//row 1
    LCDWriteData((uint8_t*)BigSevenMid, 13);
    LCDCursor(xCursorNum, yCursorNum+2);//row 2
    LCDWriteData((uint8_t*)BigSevenBtm, 13);
}

static const uint8_t BigEightTop[] = {0xFE, 0xFC, 0xF9, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0xF9, 0xFC, 0xFE};
static const uint8_t BigEightMid[] = {0xFB, 0xF1, 0xE4, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xE4, 0xF1, 0xFB};
static const uint8_t BigEightBtm[] = {0x0F, 0x07, 0x13, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x18, 0x13, 0x07, 0x0F};

void BigEightNumber(uint8_t xCursorNum, uint8_t yCursorNum){
    //Writes a big eight on the LCD screen
    LCDCursor(xCursorNum, yCursorNum);//row 0
    LCDWriteData((uint8_t*)BigEightTop, 13);
    LCDCursor(xCursorNum, yCursorNum+1);//row 1
    LCDWriteData((uint8_t*)BigEightMid, 13);
    LCDCursor(xCursorNum, yCursorNum+2);//row 2
    LCDWriteData((uint8_t*)BigEightBtm, 13);

}

static const uint8_t BigNineTop[] = {0xFE, 0xFC, 0xF9, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0xF9, 0xFC, 0xFE};
static const uint8_t BigNineMid[] = {0x03, 0x01, 0x04, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0xE4, 0xF1, 0xFB};
static const uint8_t BigNineBtm[] = {0x00, 0x00, 0x10, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x18, 0x13, 0x07, 0x0F};

void BigNineNumber(uint8_t xCursorNum, uint8_t yCursorNum){
    //Writes a big nine on the LCD screen
    LCDCursor(xCursorNum, yCursorNum);//row 0
    LCDWriteData((uint8_t*)BigNineTop, 13);
    LCDCursor(xCursorNum, yCursorNum+1);//row 1
    LCDWriteData((uint8_t*)BigNineMid, 13);
    LCDCursor(xCursorNum, yCursorNum+2);//row 2
    LCDWriteData((uint8_t*)BigNineBtm, 13);
}

static const uint8_t BigBlankTop[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t BigBlankMid[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t BigBlankBtm[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void BigBlankNumber(uint8_t xCursorNum, uint8_t yCursorNum){
    //This writes blank space in place of a number
    LCDCursor(xCursorNum, yCursorNum);//row 0
    LCDWriteData((uint8_t*)BigBlankTop, 13);
    LCDCursor(xCursorNum, yCursorNum+1);//row 1
    LCDWriteData((uint8_t*)BigBlankMid, 13);
    LCDCursor(xCursorNum, yCursorNum+2);//row 2
    LCDWriteData((uint8_t*)BigBlankBtm, 13);
}

//Converts a uint8_t to a decimal and outputs on screen
void LCDBigConversion(double putNumHere, uint8_t TheXCursor, uint8_t TheYCursor){
    //incrementors for the for loop
    int tensPlace,onesPlace,decimalPlace;
    
    int temp = (int)(putNumHere*10);
    
    decimalPlace = temp%10;// & 0xF;
    onesPlace = (temp/10)%10;// & 0xF;
    tensPlace = (temp/100)%10;// & 0xF;
    
    if(tensPlace == 0){
        BigBlankNumber(TheXCursor, TheYCursor);
        //BigNineNumber(TheXCursor, TheYCursor);
    } else if (tensPlace == 1){
        BigOneNumber(TheXCursor, TheYCursor);
    } else if (tensPlace == 2){
        BigTwoNumber(TheXCursor, TheYCursor);
    } else if (tensPlace == 3){
        BigThreeNumber(TheXCursor, TheYCursor);
    } else if (tensPlace == 4){
        BigFourNumber(TheXCursor, TheYCursor);
    } else if (tensPlace == 5){
        BigFiveNumber(TheXCursor, TheYCursor);
    } else if (tensPlace == 6){
        BigSixNumber(TheXCursor, TheYCursor);
    } else if (tensPlace == 7){
        BigSevenNumber(TheXCursor, TheYCursor);
    } else if (tensPlace == 8){
        BigEightNumber(TheXCursor, TheYCursor);
    } else {    //it is == to 9
        BigNineNumber(TheXCursor, TheYCursor);
        //BigBlankNumber(TheXCursor, TheYCursor);
    }

    if(onesPlace == 0){
        BigZeroNumber(TheXCursor+16, TheYCursor);
    } else if(onesPlace == 1){
        BigOneNumber(TheXCursor+16, TheYCursor);
    } else if(onesPlace == 2){
        BigTwoNumber(TheXCursor+16, TheYCursor);
    } else if(onesPlace == 3){
        BigThreeNumber(TheXCursor+16, TheYCursor);
    } else if(onesPlace == 4){
        BigFourNumber(TheXCursor+16, TheYCursor);
    } else if(onesPlace == 5){
        BigFiveNumber(TheXCursor+16, TheYCursor);
    } else if(onesPlace == 6){
        BigSixNumber(TheXCursor+16, TheYCursor);
    } else if(onesPlace == 7){
        BigSevenNumber(TheXCursor+16, TheYCursor);
    } else if(onesPlace == 8){
        BigEightNumber(TheXCursor+16, TheYCursor);
    } else {//it is == 9
        BigNineNumber(TheXCursor+16, TheYCursor);
    }
    
    if(decimalPlace == 0){
        BigZeroNumber(TheXCursor+36, TheYCursor);
    } else if(decimalPlace == 1){
        BigOneNumber(TheXCursor+36, TheYCursor);
    } else if(decimalPlace == 2){
        BigTwoNumber(TheXCursor+36, TheYCursor);
    } else if(decimalPlace == 3){
        BigThreeNumber(TheXCursor+36, TheYCursor);
    } else if(decimalPlace == 4){
        BigFourNumber(TheXCursor+36, TheYCursor);
    } else if(decimalPlace == 5){
        BigFiveNumber(TheXCursor+36, TheYCursor);
    } else if(decimalPlace == 6){
        BigSixNumber(TheXCursor+36, TheYCursor);
    } else if(decimalPlace == 7){
        BigSevenNumber(TheXCursor+36, TheYCursor);
    } else if(decimalPlace == 8){
        BigEightNumber(TheXCursor+36, TheYCursor);
    } else {//it is == 9
        BigNineNumber(TheXCursor+36, TheYCursor);
    }
}