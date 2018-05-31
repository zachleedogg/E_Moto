/******************************************************************************
 * Includes
 *******************************************************************************/
#include "Efuse.h"
#include "pinSetup.h"
#include "bolt_ADC.h"


/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/

/******************************************************************************
 * Configuration
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/

/******************************************************************************
 * Variable Declarations
 *******************************************************************************/

static uint8_t FAN_fault = 0;
static uint8_t PUMP_fault = 0;

static uint8_t TAILLIGHT_fault = 0;
static uint8_t BRAKELIGHT_fault = 0;

static uint8_t LOWBEAM_fault = 0;
static uint8_t HIGHBEAM_fault = 0;

static uint8_t HORN_fault = 0;

static uint8_t AUX_PORT_fault = 0;

static uint8_t HEATED_GRIPS_fault = 0;
static uint8_t HEATED_SEAT_fault = 0;

static uint8_t CHARGE_CONTROLLER_fault = 0;
static uint8_t MOTOR_CONTROLLER_fault = 0;

static uint8_t BMS_CONTROLLER_fault = 0;
static uint8_t SPARE_1_CONTROLLER_fault = 0;

static uint8_t BATT_fault = 0;

static uint8_t DCDC_fault = 0;

static uint8_t IC_CONTROLLER_fault = 0;


/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

uint16_t EFUSE_GET_CURRENT_FAN();
uint16_t EFUSE_GET_CURRENT_PUMP();

uint16_t EFUSE_GET_CURRENT_TAILLIGHT();
uint16_t EFUSE_GET_CURRENT_BRAKELIGHT();

uint16_t EFUSE_GET_CURRENT_LOWBEAM();
uint16_t EFUSE_GET_CURRENT_HIGHBEAM();

uint16_t EFUSE_GET_CURRENT_HORN();

uint16_t EFUSE_GET_CURRENT_AUX_PORT();

uint16_t EFUSE_GET_CURRENT_HEATED_GRIPS();
uint16_t EFUSE_GET_CURRENT_HEATED_SEAT();

uint16_t EFUSE_GET_CURRENT_CHARGE_CONTROLLER();
uint16_t EFUSE_GET_CURRENT_MOTOR_CONTROLLER();

uint16_t EFUSE_GET_CURRENT_BMS_CONTROLLER();
uint16_t EFUSE_GET_CURRENT_SPARE_1_CONTROLLER();

uint16_t EFUSE_GET_CURRENT_BATT();

uint16_t EFUSE_GET_CURRENT_DCDC();

uint16_t EFUSE_GET_CURRENT_IC_CONTROLLER();

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void EFUSE_Init(void){
    SET_DIAG_SELECT_EN(LOW);
    SET_DIAG_EN(HIGH);
}

void EFUSE_Run(void) {
    uint8_t diagSelect = GET_DIAG_SELECT_EN();
    uint16_t value = 0;

    if (diagSelect == 0) {

        value = EFUSE_GET_CURRENT_PUMP();
        if (value >= 1000) {
            PUMP_fault = 1;
            SET_PUMP_1_EN(LOW);
        }

        value = EFUSE_GET_CURRENT_BRAKELIGHT();
        if (value >= 1000) {
            BRAKELIGHT_fault = 1;
            SET_BRAKE_LIGHT_EN(LOW);
        }

        value = EFUSE_GET_CURRENT_HIGHBEAM();
        if (value >= 1000) {
            HIGHBEAM_fault = 1;
            SET_HEADLIGHT_HI_EN(LOW);
        }

        value = EFUSE_GET_CURRENT_HEATED_SEAT();
        if (value >= 1000) {
            HEATED_SEAT_fault = 1;
            SET_HEATED_SEAT_EN(LOW);
        }

        value = EFUSE_GET_CURRENT_CHARGE_CONTROLLER();
        if (value >= 1000) {
            CHARGE_CONTROLLER_fault = 1;
            SET_CHARGE_CONTROLLER_EN(LOW);
        }

        value = EFUSE_GET_CURRENT_SPARE_1_CONTROLLER();
        if (value >= 1000) {
            SPARE_1_CONTROLLER_fault = 1;
            SET_J1772_CONTROLLER_EN(LOW);
        }

    } else {
        value = EFUSE_GET_CURRENT_FAN();
        if (value >= 1000) {
            FAN_fault = 1;
            SET_FAN_1_EN(LOW);
        }

        value = EFUSE_GET_CURRENT_TAILLIGHT();
        if (value >= 1000) {
            TAILLIGHT_fault = 1;
            SET_TAILLIGHT_EN(LOW);
        }

        value = EFUSE_GET_CURRENT_LOWBEAM();
        if (value >= 1000) {
            LOWBEAM_fault = 1;
            SET_HEADLIGHT_LO_EN(LOW);
        }

        value = EFUSE_GET_CURRENT_HEATED_GRIPS();
        if (value >= 1000) {
            HEATED_GRIPS_fault = 1;
            SET_HEATED_GRIPS_EN(LOW);
        }

        value = EFUSE_GET_CURRENT_MOTOR_CONTROLLER();
        if (value >= 1000) {
            MOTOR_CONTROLLER_fault = 1;
            SET_MOTOR_CONTROLLER_EN(LOW);
        }

        value = EFUSE_GET_CURRENT_BMS_CONTROLLER();
        if (value >= 1000) {
            BMS_CONTROLLER_fault = 1;
            SET_BMS_CONTROLLER_EN(LOW);
        }
    }

    value = EFUSE_GET_CURRENT_HORN();
    if (value >= 1000) {
        HORN_fault = 1;
        SET_HORN_EN(LOW);
    }

    value = EFUSE_GET_CURRENT_AUX_PORT();
    if (value >= 1000) {
        AUX_PORT_fault = 1;
        SET_AUX_PORT_EN(LOW);
    }
    
    BATT_fault = GET_BATT_FAULT();
    
    DCDC_fault = GET_DCDC_FAULT();
    
    BATT_fault = GET_BATT_FAULT();
    
}

void EFUSE_Halt(void){
    SET_DIAG_SELECT_EN(LOW);
    SET_DIAG_EN(LOW);
}
    
    
uint16_t EFUSE_GET_CURRENT_FAN() {
    return ADC_GetValue(FAN_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_PUMP() {
    return ADC_GetValue(FAN_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_TAILLIGHT() {
    return ADC_GetValue(TAILLIGHT_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_BRAKELIGHT() {
    return ADC_GetValue(TAILLIGHT_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_LOWBEAM() {
    return ADC_GetValue(HEADLIGHT_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_HIGHBEAM() {
    return ADC_GetValue(HEADLIGHT_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_HORN() {
    return ADC_GetValue(HORN_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_AUX_PORT() {
    return ADC_GetValue(AUX_PORT_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_HEATED_GRIPS() {
    return ADC_GetValue(HEATER_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_HEATED_SEAT() {
    return ADC_GetValue(HEADLIGHT_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_CHARGE_CONTROLLER() {
    return ADC_GetValue(ECU_2_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_MOTOR_CONTROLLER() {
    return ADC_GetValue(ECU_2_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_BMS_CONTROLLER() {
    return ADC_GetValue(ECU_1_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_SPARE_1_CONTROLLER() {
    return ADC_GetValue(ECU_1_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_BATT() {
    return ADC_GetValue(BATT_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_DCDC() {
    return ADC_GetValue(DCDC_ISENSE_AI);
}

uint16_t EFUSE_GET_CURRENT_IC_CONTROLLER() {
    return ADC_GetValue(IC_CONTROLLER_ISENSE_AI);
}

uint8_t EFUSE_GET_CURRENT_FAN_FAULT(void){
    return FAN_fault;
}

uint8_t EFUSE_GET_CURRENT_PUMP_FAULT(void){
    return PUMP_fault;
}

uint8_t EFUSE_GET_CURRENT_TAILLIGHT_FAULT(void){
    return TAILLIGHT_fault;
}

uint8_t EFUSE_GET_CURRENT_BRAKELIGHT_FAULT(void){
    return BRAKELIGHT_fault;
}

uint8_t EFUSE_GET_CURRENT_LOWBEAM_FAULT(void){
    return LOWBEAM_fault;
}

uint8_t EFUSE_GET_CURRENT_HIGHBEAM_FAULT(void){
    return HIGHBEAM_fault;
}

uint8_t EFUSE_GET_CURRENT_HORN_FAULT(void){
    return HORN_fault;
}

uint8_t EFUSE_GET_CURRENT_AUX_PORT_FAULT(void){
    return AUX_PORT_fault;
}

uint8_t EFUSE_GET_CURRENT_HEATED_GRIPS_FAULT(void){
    return HEATED_GRIPS_fault;
}

uint8_t EFUSE_GET_CURRENT_HEATED_SEAT_FAULT(void){
    return HEATED_SEAT_fault;
}

uint8_t EFUSE_GET_CURRENT_CHARGE_CONTROLLER_FAULT(void){
    return CHARGE_CONTROLLER_fault;
}

uint8_t EFUSE_GET_CURRENT_MOTOR_CONTROLLER_FAULT(void){
    return MOTOR_CONTROLLER_fault;
}

uint8_t EFUSE_GET_CURRENT_BMS_CONTROLLER_FAULT(void){
    return BMS_CONTROLLER_fault;
}

uint8_t EFUSE_GET_CURRENT_SPARE_1_CONTROLLER_FAULT(void){
    return SPARE_1_CONTROLLER_fault;
}


/*** End of File **************************************************************/


