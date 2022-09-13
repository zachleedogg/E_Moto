/******************************************************************************
 * Includes
 *******************************************************************************/
#include "j1772.h"
#include "IO.h"
#include "movingAverage.h"

/******************************************************************************
 * Constants
 *******************************************************************************/
#define DEBUG 0
#if DEBUG
#include <stdio.h>
#include "bolt_uart.h"

static uint8_t debugEnable = 1;
#define j1772Service_print(...) if(debugEnable){char tempArray[125]={};sprintf(tempArray,__VA_ARGS__);Uart1Write(tempArray);}
#else
#define j1772Service_print(...)
#endif
/******************************************************************************
 * Macros
 *******************************************************************************/

/*Proximty Range values in milliVolts*/
#define PROX_DISCONNECT_UPPER 2600
#define PROX_DISCONNECT_LOWER 2400

#define PROX_CONNECT_UPPER 1300
#define PROX_CONNECT_LOWER 1100

#define PROX_REQUEST_DISCONNECT_UPPER 1950
#define PROX_REQUEST_DISCONNECT_LOWER 1750

#define PROXIMITY_AVERAGE_WINDOW_SIZE 8

/******************************************************************************
 * Configuration
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/

/******************************************************************************
 * Variable Declarations
 *******************************************************************************/
static uint8_t j1772run = 1;

static prox_status_E proximity = J1772_SNA_PROX;

NEW_AVERAGE(proximityAverage, PROXIMITY_AVERAGE_WINDOW_SIZE);

NEW_LOW_PASS_FILTER(pilotFilter, 0.3);

static float pilotVoltagePeak = 0;
static float pilotVoltageFiltered = 0;
static uint8_t pilotDutyCycle = 0;
static uint8_t pilotEncodedCurrent = 0;
/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/


void j1772Control_Init(void) {
    pilotVoltagePeak = 0;
    pilotVoltageFiltered = 0;
    pilotDutyCycle = 0;
    pilotEncodedCurrent = 0;
    j1772run = 1;
}

void j1772Control_Run_cont(void) {
    if (j1772run) {
        float pilot_instantaneousVoltage = IO_GET_VOLTAGE_PILOT();
        pilotVoltageFiltered = takeLowPassFilter(pilotFilter, pilot_instantaneousVoltage);
        
        /*If the value is above 50mV, we assume that we are on a "HIGH" cycle of
          the PWM. This will represent the peak value of the signal.*/
        pilotVoltagePeak = (pilot_instantaneousVoltage > 50) ? pilot_instantaneousVoltage : 0;
        
        /*If peak voltage greater than 1V, then we can attempt to calculate 
          the duty cycle*/
        if (pilotVoltagePeak > 1){
            pilotDutyCycle = (uint16_t)(pilotVoltageFiltered * 100 / pilotVoltagePeak);
        }
        
        /*If the filtered voltage is less than 1V, assume the signal is low 
          and there is no duty cycle*/
        if (pilotVoltageFiltered < 1){
            pilotDutyCycle = 0;
        }
        
    }
}

void j1772Control_Run_100ms(void) {
    if (j1772run) {
        /*Take moving average of Proximity value*/
        uint16_t currentProxAve = takeMovingAverage(proximityAverage, IO_GET_VOLTAGE_PROXIMITY());

        j1772Service_print("PILOT Voltage: %d\nPROX Voltage: %d\n\n", pilotVoltagePeak, currentProxAve);


        /*Determine the state of the Proximity Detector*/
        switch (currentProxAve*1000) {
            case PROX_DISCONNECT_LOWER ... PROX_DISCONNECT_UPPER:
                proximity = J1772_DISCONNECTED;
                j1772Service_print("Disconnected\n")
                IO_SET_PILOT_EN(LOW);
                /*Do something*/
                break;
            case PROX_CONNECT_LOWER ... PROX_CONNECT_UPPER:
                proximity = J1772_CONNECTED;
                j1772Service_print("Connected\n")
                IO_SET_PILOT_EN(HIGH);
                break;
            case PROX_REQUEST_DISCONNECT_LOWER ... PROX_REQUEST_DISCONNECT_UPPER:
                proximity = J1772_REQUEST_DISCONNECT;
                j1772Service_print("Request Disconnect\n")
                IO_SET_PILOT_EN(LOW);
                break;
            default:
                proximity = J1772_SNA_PROX;
                break;
        }


        /*Run the Pilot Signal State Machine*/
        switch (pilotDutyCycle) {
            case 0 ... 9:
                pilotEncodedCurrent = 0;
                break;
            case 10 ... 84:
                pilotEncodedCurrent = pilotDutyCycle * 0.6;
                break;
            case 85 ... 100:
                pilotEncodedCurrent = (pilotDutyCycle - 64)*2.5;
                break;
            default:
                pilotEncodedCurrent = 0;
                break;
        }
    }
}

void j1772Control_Halt(void) {
    j1772run = 0;
    IO_SET_PILOT_EN(LOW);
}

prox_status_E j1772getProxState(void) {
    return proximity;
}

uint8_t j1772getPilotCurrent(void) {
    return pilotEncodedCurrent;
}