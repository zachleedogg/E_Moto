/*
 * File:   ltc_6802.c
 * Author: zachleedogg
 *
 * Created on May 10, 2022, 9:14 PM
 */


#include "ltc_6802.h"
#include "bolt_spi.h"
#include "IO.h"
#include "utils.h"

#define NUMBER_OF_LTC6802_STACKS 2

/*Register Size*/
#define CONFIG_REGISTER_SIZE 0x6
#define CELL_VOLTAGE_REGISTER_SIZE 0x12
#define FLAG_REGISTER_SIZE 0x3
#define TEMPERATURE_REGISTER_SIZE 0x5

/*Config*/
#define CDC_OFFSET 0x0
#define CDC_RANGE 0x03
#define CELL10_OFFSET 0x3
#define CELL10_RANGE 0x1
#define LVPL_OFFSET 0x4
#define LVPL_RANGE 0x1
#define GPIO1_OFFSET 0x5
#define GPIO1_RANGE 0x1
#define GPIO2_OFFSET 0x6
#define GPIO2_RANGE 0x1
#define WDT_OFFSET 0x7
#define WDT_RANGE 0x1
#define DCC_OFFSET 0x08
#define DCC_RANGE 0x0C
#define MC_OFFSET 0x14
#define MC_RANGE 0x0C
#define VUV_OFFSET 0x28
#define VUV_RANGE 0x08
#define VOV_OFFSET 0x30
#define VOV_RANGE 0x08


/*Addresses*/
#define WRCFG 0x01 //Write Configuration Register Group
#define RDCFG 0x02 //Read Configuration Register Group
#define RDCV 0x04 //Read Cell Voltage Register Group
#define RDFLG 0x06 //Read Flag Register Group
#define RDTMP 0x08 //Read Temperature Register Group
#define STCVAD 0x10 //Start Cell Voltage A/D Conversions and Poll Status
#define STCVAD_CELL(CELL) 0x1##CELL //Start single CELL converversion only
#define STOWAD 0x20 //Start Open-Wire A/D Conversions and Poll Status
#define STOWAD_CELL(CELL) 0x2##CELL //Start single CELL Open-Wire conversions only
#define STTMPAD 0x30 //Start Temperature A/D Conversions and Poll Status
#define STTMPAD_TEMP(TEMP) 0x3##TEMP //temp1, temp2, or internal temp
#define PLADC 0x40 //Poll A/D Converter Status
#define PLINT 0x50 //Poll Interrupt Status
#define STCVDC 0x60 //Start Cell Voltage A/D Conversions and Poll Status, with Discharge Permitted
#define STCVDC_CELL(CELL) 0x6##CELL //Start single CELL conversion with Discharge Permitted
#define STOWDC 0x70 //Start Open-Wire A/D Conversions and Poll Status, with Discharge Permitted
#define STOWDC_CELL(CELL) 0x7##CELL //Start Single Cell Open-Wire with Discharge Permitted

// Aligning by 8 (64bit) because even numbers of resgister sized guaruntee alignment to the last byte.
static uint8_t config_data[CONFIG_REGISTER_SIZE] __attribute__((aligned(8)));

static uint8_t flag_data[FLAG_REGISTER_SIZE*NUMBER_OF_LTC6802_STACKS] __attribute__((aligned(8)));

static uint8_t cellVoltage[CELL_VOLTAGE_REGISTER_SIZE*NUMBER_OF_LTC6802_STACKS] __attribute__((aligned(8)));

static uint8_t temperature[TEMPERATURE_REGISTER_SIZE*NUMBER_OF_LTC6802_STACKS] __attribute__((aligned(8)));
    
static uint8_t LTC_BroadcastPollCommand(uint8_t command);
static uint8_t LTC_BroadcastReadCommand(uint8_t command, uint8_t len, uint8_t * recieveData);
static void LTC_BroadcastWriteCommand(uint8_t command, uint8_t len, uint8_t * transmitData);
static uint8_t LTC_AddressReadCommand(uint8_t address, uint8_t command, uint8_t len, uint8_t * recieveData);

void LTC_set_CDC(uint8_t cdc){
    set_bits((size_t *)config_data, CDC_OFFSET, CDC_RANGE, cdc);
}

void LTC_set_CELL10(uint8_t cell10){
    set_bits((size_t *)config_data, CELL10_OFFSET, CELL10_RANGE, cell10);
}

void LTC_set_LVPL(uint8_t lvpl){
    set_bits((size_t *)config_data, LVPL_OFFSET, LVPL_RANGE, lvpl);
}

void LTC_set_GPIO1(uint8_t gpio1){
    set_bits((size_t *)config_data, GPIO1_OFFSET, GPIO1_RANGE, gpio1);
}

void LTC_set_GPIO2(uint8_t gpio2){
    set_bits((size_t *)config_data, GPIO2_OFFSET, GPIO2_RANGE, gpio2);
}

void LTC_set_WDT(uint8_t wdt){
    set_bits((size_t *)config_data, WDT_OFFSET, WDT_RANGE, wdt);
}

void LTC_set_cell_discharge(uint8_t cell){
    set_bits((size_t *)config_data, DCC_OFFSET, DCC_RANGE, 1<<(cell-1));
}

void LTC_set_MCI(uint8_t cell){
    set_bits((size_t *)config_data, MC_OFFSET, MC_RANGE, 1<<(cell-1));
}

void LTC_set_VUV(float underVoltage){
    uint8_t uv = (uint8_t)(underVoltage/(16*.0015));
    set_bits((size_t *)config_data, VUV_OFFSET, VUV_RANGE, uv);
}

void LTC_set_VOV(float overVoltage){
    uint8_t ov = (uint8_t)(overVoltage/(16*.0015));
    set_bits((size_t *)config_data, VOV_OFFSET, VOV_RANGE, ov);
}

void LTC_writeConfig(void){
    LTC_BroadcastWriteCommand(WRCFG, sizeof(config_data), config_data);
}

void LTC_start_all_cell_ADC(void){
    LTC_BroadcastWriteCommand(STCVDC, 0, 0);
}

uint8_t LTC_check_ADC_status(void){
    return LTC_BroadcastPollCommand(PLADC);
}

uint8_t LTC_read_all_cell_ADC(void){
    uint8_t returnVal = 0;
    uint8_t i = 0;
    for (i=0; i<NUMBER_OF_LTC6802_STACKS; i++){
        returnVal += LTC_AddressReadCommand(i, RDCV, sizeof(cellVoltage), cellVoltage);
    }
    return returnVal;
}

uint16_t LTC_get_cell_voltage(uint8_t cell){
    return 1.5*get_bits((size_t *)cellVoltage, (cell-1)*12, 12);
    
}


static uint8_t LTC_BroadcastPollCommand(uint8_t command){
    IO_SET_SPI_CS(LOW);
    spiTransfer(command);
    uint8_t value = spiTransfer(0xAA);
    IO_SET_SPI_CS(HIGH);
    return value;
}

static uint8_t LTC_BroadcastReadCommand(uint8_t command, uint8_t len, uint8_t * recieveData){
    IO_SET_SPI_CS(LOW);
    spiTransfer(command);
    uint8_t i =0;
    for(i=0; i<len; i++){
        recieveData[i] = spiTransfer(0xAA);
    }
    uint8_t pec = spiTransfer(0xAA);
    IO_SET_SPI_CS(HIGH);
    uint8_t calc = crc8ccitt(recieveData, len);
    if (pec == calc) {
        return 1;
    } else {
        return 0;
    }
}

static void LTC_BroadcastWriteCommand(uint8_t command, uint8_t len, uint8_t * transmitData){
    IO_SET_SPI_CS(LOW);
    spiTransfer(command);
    uint8_t i =0;
    for(i=0; i<len; i++){
        spiTransfer(transmitData[i]);
    }
    IO_SET_SPI_CS(HIGH);
}

static uint8_t LTC_AddressReadCommand(uint8_t address, uint8_t command, uint8_t len, uint8_t * recieveData){
    IO_SET_SPI_CS(LOW);
    spiTransfer(8 | address);
    spiTransfer(command);
    uint8_t i =0;
    for(i=0; i<len; i++){
        recieveData[i] = spiTransfer(0xAA);
    }
    uint8_t pec = spiTransfer(0xAA);
    IO_SET_SPI_CS(HIGH);
    uint8_t calc = crc8ccitt(recieveData, len);
    if (pec == calc) {
        return 1;
    } else {
        return 0;
    }
}
