/*
 * File:   LTC6802_.c
 * Author: zachleedogg
 *
 * Created on May 10, 2022, 9:14 PM
 */


#include "ltc_6802.h"
#include "bolt_spi.h"
#include "IO.h"
#include "utils.h"

#define NUMBER_OF_LTC6802_STACKS 2
#define NUMBER_OF_BANKS 24

/*Register Size*/
#define CONFIG_REGISTER_SIZE 0x6*NUMBER_OF_LTC6802_STACKS
#define CELL_VOLTAGE_REGISTER_SIZE 0x12*NUMBER_OF_LTC6802_STACKS
#define FLAG_REGISTER_SIZE 0x3*NUMBER_OF_LTC6802_STACKS
#define TEMPERATURE_REGISTER_SIZE 0x5*NUMBER_OF_LTC6802_STACKS

#define NEXT_POWER(x) 1<<((sizeof(x)*8)-__builtin_clz(x-1))

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
#define VUV_OFFSET 0x20
#define VUV_RANGE 0x08
#define VOV_OFFSET 0x28
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

// Aligning so we can reach insto structures larger than 64 bit and perform bitwise operations
static uint8_t config_data[CONFIG_REGISTER_SIZE] __attribute__((aligned(NEXT_POWER(CONFIG_REGISTER_SIZE))));
static uint8_t flag_data[FLAG_REGISTER_SIZE] __attribute__((aligned(NEXT_POWER(FLAG_REGISTER_SIZE))));
static uint8_t cellVoltage[CELL_VOLTAGE_REGISTER_SIZE] __attribute__((aligned(NEXT_POWER(CELL_VOLTAGE_REGISTER_SIZE))));
static uint8_t temperature[TEMPERATURE_REGISTER_SIZE] __attribute__((aligned(NEXT_POWER(TEMPERATURE_REGISTER_SIZE))));
    
static uint8_t LTC6802_BroadcastPollCommand(uint8_t command);
static uint8_t LTC6802_BroadcastReadCommand(uint8_t command, uint8_t len, uint8_t * recieveData);
static void LTC6802_BroadcastWriteCommand(uint8_t command, uint8_t len, uint8_t * transmitData);


void LTC6802_init(void){
    spi1Init();
}

void LTC6802_set_CDC(uint8_t cdc){
    uint8_t i = 0;
    for (i=0; i<NUMBER_OF_LTC6802_STACKS; i++){
        set_bits((size_t *)&config_data[i*(CONFIG_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS)], CDC_OFFSET, CDC_RANGE, cdc);
    }
}

void LTC6802_set_CELL10(uint8_t cell10){
    uint8_t i = 0;
    for (i=0; i<NUMBER_OF_LTC6802_STACKS; i++){
        set_bits((size_t *)&config_data[i*(CONFIG_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS)], CELL10_OFFSET, CELL10_RANGE, cell10);
    }
}

void LTC6802_set_LVPL(uint8_t lvpl){
    uint8_t i = 0;
    for (i=0; i<NUMBER_OF_LTC6802_STACKS; i++){
        set_bits((size_t *)&config_data[i*(CONFIG_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS)], LVPL_OFFSET, LVPL_RANGE, lvpl);
    }
}

void LTC6802_set_GPIO1(uint8_t address, uint8_t gpio1){
    set_bits((size_t *)&config_data[address*(CONFIG_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS)], GPIO1_OFFSET, GPIO1_RANGE, gpio1);
}

void LTC6802_set_GPIO2(uint8_t address, uint8_t gpio2){
    set_bits((size_t *)&config_data[address*(CONFIG_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS)], GPIO2_OFFSET, GPIO2_RANGE, gpio2);
}

void LTC6802_set_cell_discharge(uint8_t cell, uint8_t value){
    uint8_t address = cell/12;
    set_bits((size_t *)&config_data[address*(CONFIG_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS)], DCC_OFFSET+(cell-1), 1, value);
}

void LTC6802_stop_all_cell_dischage(){
    uint8_t i = 0;
    for (i=0; i<NUMBER_OF_LTC6802_STACKS; i++){
        set_bits((size_t *)&config_data[i*(CONFIG_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS)], DCC_OFFSET, DCC_RANGE, 0x0000);
    }
}

void LTC6802_set_MCI(uint8_t cell){
    uint8_t address = cell/12;
    set_bits((size_t *)&config_data[address*(CONFIG_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS)], MC_OFFSET, MC_RANGE, 1<<(cell-1));
}

void LTC6802_set_VUV(float underVoltage){
    uint8_t uv = (uint8_t)(underVoltage/(16*.0015));
    uint8_t i = 0;
    for (i=0; i<NUMBER_OF_LTC6802_STACKS; i++){
        set_bits((size_t *)&config_data[i*(CONFIG_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS)], VUV_OFFSET, VUV_RANGE, uv);
    }
}

void LTC6802_set_VOV(float overVoltage){
    uint8_t ov = (uint8_t)(overVoltage/(16*.0015));
    uint8_t i = 0;
    for (i=0; i<NUMBER_OF_LTC6802_STACKS; i++){
        set_bits((size_t *)&config_data[i*(CONFIG_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS)], VOV_OFFSET, VOV_RANGE, ov);
    }
}

void LTC6802_writeConfig(void){
    uint8_t i = 0;
    for (i=0; i<NUMBER_OF_LTC6802_STACKS; i++){
        LTC6802_AddressWriteCommand(i, WRCFG, sizeof(config_data)/NUMBER_OF_LTC6802_STACKS, &config_data[i*(CONFIG_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS)]);
    }
}

void LTC6802_start_all_cell_ADC(void){
    LTC6802_BroadcastWriteCommand(STCVDC, 0, 0);
}

uint8_t LTC6802_check_ADC_status(void){
    return 0xFF & LTC6802_BroadcastPollCommand(PLADC);
}

uint8_t LTC6802_read_all_cell_ADC(void){
    uint8_t returnVal = 0;
    uint8_t i = 0;
    uint16_t range = CELL_VOLTAGE_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS;
    for (i=0; i<NUMBER_OF_LTC6802_STACKS; i++){
        returnVal += LTC6802_AddressReadCommand(i, RDCV, range, &cellVoltage[range*i]);
    }
    return returnVal;
}

void LTC6802_start_all_temp_ADC(void){
    LTC6802_BroadcastWriteCommand(STTMPAD, 0, 0);
}

uint8_t LTC6802_read_all_temp_ADC(void){
    uint8_t returnVal = 0;
    uint8_t i = 0;
    uint16_t range = TEMPERATURE_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS;
    for (i=0; i<NUMBER_OF_LTC6802_STACKS; i++){
        returnVal += LTC6802_AddressReadCommand(i, RDTMP, range, &temperature[range*i]);
    }
    return returnVal;
}

float LTC6802_get_cell_voltage(uint8_t cell){
    float val = 0;
    val = 0.0015*(float)(get_bits((size_t *)cellVoltage, (cell-1)*12, 12));
    return val;
}

float LTC6802_get_temp_voltage(uint8_t temp){
    temp = temp-1;
    uint8_t address = temp/3;
    temp = temp%3;
    return 0.0015*(float)(get_bits((size_t *)temperature, address*8*TEMPERATURE_REGISTER_SIZE/NUMBER_OF_LTC6802_STACKS + (temp)*12, 12));
}

static uint8_t LTC6802_BroadcastPollCommand(uint8_t command){
    IO_SET_SPI_CS(LOW);
    spiTransfer(command);
    uint8_t value = 1;//spiTransfer(0xAA);
    IO_SET_SPI_CS(HIGH);
    return value;
}

static uint8_t LTC6802_BroadcastReadCommand(uint8_t command, uint8_t len, uint8_t * recieveData){
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

static void LTC6802_BroadcastWriteCommand(uint8_t command, uint8_t len, uint8_t * transmitData){
    IO_SET_SPI_CS(LOW);
    spiTransfer(command);
    uint8_t i =0;
    for(i=0; i<len; i++){
        spiTransfer(transmitData[i]);
    }
    IO_SET_SPI_CS(HIGH);
}

void LTC6802_AddressWriteCommand(uint8_t address, uint8_t command, uint8_t len, uint8_t * transmitData){
    IO_SET_SPI_CS(LOW);
    spiTransfer(0x80 | address);
    spiTransfer(command);
    uint8_t i =0;
    for(i=0; i<len; i++){
        spiTransfer(transmitData[i]);
    }
    IO_SET_SPI_CS(HIGH);
}

uint8_t LTC6802_AddressReadCommand(uint8_t address, uint8_t command, uint8_t len, uint8_t * recieveData){
    IO_SET_SPI_CS(LOW);
    uint8_t dummy = 0x3;
    dummy = spiTransfer(0x80 | address);
    dummy = spiTransfer(command);
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
