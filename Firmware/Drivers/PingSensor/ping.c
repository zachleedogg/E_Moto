/* 
 * File:   ping.c
 * Author: Zachary Levenberg
 *
 * Created on August 6, 2016, 9:27 AM
 */

#include <xc.h>

#include "ping.h"

#include "bolt_init.h"
#include "bolt_uart.h"

static uint32_t secondsPerTick;

static PINS_pin_S r_trig;
static PINS_pin_S l_trig;

static volatile ic_status IC1_status = DONE;
static volatile ic_status IC3_status = DONE;

static volatile uint8_t distance = 0;

static volatile uint8_t distanceL = 0;

uint8_t ping_Init(ic_pin_number rightEchoPin, PINS_pin_S R_TRIGGER, ic_pin_number leftEchoPin, PINS_pin_S leftTriggerPin) {
    secondsPerTick = clockFreq() / 2000000;

    _IC1R = rightEchoPin;
    r_trig = R_TRIGGER;

    PINS_write(r_trig.port, r_trig.pin, LOW);
    PINS_direction(r_trig.port, r_trig.pin, OUTPUT);
    uint16_t temp;

    while (IC1CON1bits.ICBNE) { //clear buffer
        temp = IC1BUF;
    }
    while (IC2CON1bits.ICBNE) { //clear buffer
        temp = IC2BUF;
    }

    _IC3R = leftEchoPin;
    l_trig = leftTriggerPin;

    PINS_write(l_trig.port, l_trig.pin, LOW);
    PINS_direction(l_trig.port, l_trig.pin, OUTPUT);

    while (IC3CON1bits.ICBNE) { //clear buffer
        temp = IC3BUF;
    }
    while (IC4CON1bits.ICBNE) { //clear buffer
        temp = IC4BUF;
    }


    IC2CON2bits.TRIGSTAT = 0; // IC1TMR has not been triggered and is being held clear
    IC2CON1bits.ICSIDL = 0; // Input capture will continue to operate in CPU idle mode
    IC2CON1bits.ICTSEL = 0b111; // Peripheral (FP) is the clock source for the IC1 module
    IC2CON1bits.ICI = 0; // Interrupt on every capture event
    IC2CON2bits.IC32 = 1; // Cascade module operation is disabled
    IC2CON2bits.ICTRIG = 1; // Input source used to synchronize the input capture timer of  
    IC2CON2bits.SYNCSEL = 0; // No Sync or Trigger source for the IC1 module


    IC1CON2bits.TRIGSTAT = 0; // IC1TMR has not been triggered and is being held clear
    IC1CON1bits.ICSIDL = 0; // Input capture will continue to operate in CPU idle mode
    IC1CON1bits.ICTSEL = 0b111; // Peripheral (FP) is the clock source for the IC1 module
    IC1CON1bits.ICI = 0; // Interrupt on every capture event
    IC1CON2bits.IC32 = 1; // Cascade module operation is disabled
    IC1CON2bits.ICTRIG = 1; // Input source used to synchronize the input capture timer of  
    IC1CON2bits.SYNCSEL = 0; // No Sync or Trigger source for the IC1 module

    IEC0bits.IC1IE = 1; // Enable IC1 interrupts
    IFS0bits.IC1IF = 0; // Clear Flag
    IPC0bits.IC1IP = 1; // Set module interrupt priority as 1

    IC2CON1bits.ICM = 0b001; // Enable Capture Mode on every edge
    IC1CON1bits.ICM = 0b001; // Enable Capture Mode on every edge
    
    
    

    IC4CON2bits.TRIGSTAT = 0; // IC1TMR has not been triggered and is being held clear
    IC4CON1bits.ICSIDL = 0; // Input capture will continue to operate in CPU idle mode
    IC4CON1bits.ICTSEL = 0b111; // Peripheral (FP) is the clock source for the IC1 module
    IC4CON1bits.ICI = 0; // Interrupt on every capture event
    IC4CON2bits.IC32 = 1; // Cascade module operation is disabled
    IC4CON2bits.ICTRIG = 1; // Input source used to synchronize the input capture timer of  
    IC4CON2bits.SYNCSEL = 0; // No Sync or Trigger source for the IC1 module


    IC3CON2bits.TRIGSTAT = 0; // IC1TMR has not been triggered and is being held clear
    IC3CON1bits.ICSIDL = 0; // Input capture will continue to operate in CPU idle mode
    IC3CON1bits.ICTSEL = 0b111; // Peripheral (FP) is the clock source for the IC1 module
    IC3CON1bits.ICI = 0; // Interrupt on every capture event
    IC3CON2bits.IC32 = 1; // Cascade module operation is disabled
    IC3CON2bits.ICTRIG = 1; // Input source used to synchronize the input capture timer of  
    IC3CON2bits.SYNCSEL = 0; // No Sync or Trigger source for the IC1 module

    IEC2bits.IC3IE = 1; // Enable IC1 interrupts
    IFS2bits.IC3IF = 0; // Clear Flag
    IPC9bits.IC3IP = 1; // Set module interrupt priority as 1

    IC4CON1bits.ICM = 0b001; // Enable Capture Mode on every edge
    IC3CON1bits.ICM = 0b001; // Enable Capture Mode on every edge

    return 0;
}

uint8_t ping_Run(void) {
    if (IC1_status == DONE) {
        //count2 = 0;
        //Uart1Write("Set Pin HIGH\n");
        /*Make pin go Hi*/
        PINS_write(r_trig.port, r_trig.pin, HIGH);
        IC1_status = CHARGING;

    } else if (IC1_status == CHARGING) {
        //Uart1Write("Pull Pin LOW\n");
        /*Pull Pin Low*/
        if (IC1_status == CHARGING) {
            PINS_write(r_trig.port, r_trig.pin, LOW);
            IC1_status = RESLEASED;
        }
    }

    if (IC3_status == DONE) {
        //count2 = 0;
        //Uart1Write("Set Pin HIGH\n");
        /*Make pin go Hi*/
        PINS_write(l_trig.port, l_trig.pin, HIGH);
        IC3_status = CHARGING;

    } else if (IC3_status == CHARGING) {
        //Uart1Write("Pull Pin LOW\n");
        /*Pull Pin Low*/
        if (IC3_status == CHARGING) {
            PINS_write(l_trig.port, l_trig.pin, LOW);
            IC3_status = RESLEASED;
        }
    }

    return distance;
}

uint8_t PING_getDistanceRight(uint8_t leftOrRight) {
    if (leftOrRight == 1) {
        return distance;
    }
    
    if(leftOrRight == 0){
        return distanceL;
    }
}

void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void) {

    static uint8_t ic_ignoreNextVal = 0;
    /*clear the interrupt*/
    _IC1IF = 0;

    uint16_t temp1 = 0;
    uint16_t temp2 = 0;

    switch (IC1_status) {
        case RESLEASED:
            IC1CON2bits.TRIGSTAT = 1; /*Release the ICx Timer from reset*/
            IC2CON2bits.TRIGSTAT = 1;

            uint16_t temp;
            temp = IC1BUF;
            temp = IC2BUF;

            IC1_status = RUNNING;
            break;

        case RUNNING:

            temp1 = IC1BUF;
            temp2 = IC2BUF;

            uint32_t time = ((uint32_t) temp1 | ((uint32_t) temp2 << 16));
            uint32_t dist = time;
            dist = dist / secondsPerTick;
            dist = dist / 148;
            if (dist > 150) {
                ic_ignoreNextVal = 1;
                //Uart1Write("INF\n");
                dist = 0xFFFFFFFF;
            } else if (ic_ignoreNextVal == 1) {
                ic_ignoreNextVal = 0;
                //Uart1Write("INF\n");
                dist = 0xFFFFFFFF;
            } else {
                //char string[50];
                //sprintf(string, "dist:%lu\n", dist);
                //Uart1Write(string);
            }
            distance = (uint8_t) dist;
            IC1_status = DONE;
            IC1CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
            IC2CON1bits.ICM = 0b000; /*Reset ICx+1 Timer*/
            IC2CON1bits.ICM = 0b001;
            break;
        default:
            while (IC1CON1bits.ICBNE || IC2CON1bits.ICBNE) {
                temp1 = IC1BUF;
                temp1 = IC2BUF;
            }
            break;
    }
}

void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void) {

    static uint8_t ic_ignoreNextVal = 0;
    /*clear the interrupt*/
    _IC3IF = 0;

    uint16_t temp1 = 0;
    uint16_t temp2 = 0;

    switch (IC3_status) {
        case RESLEASED:
            IC3CON2bits.TRIGSTAT = 1; /*Release the ICx Timer from reset*/
            IC4CON2bits.TRIGSTAT = 1;

            uint16_t temp;
            temp = IC3BUF;
            temp = IC4BUF;

            IC3_status = RUNNING;
            break;

        case RUNNING:

            temp1 = IC3BUF;
            temp2 = IC4BUF;

            uint32_t time = ((uint32_t) temp1 | ((uint32_t) temp2 << 16));
            uint32_t dist = time;
            dist = dist / secondsPerTick;
            dist = dist / 148;
            if (dist > 150) {
                ic_ignoreNextVal = 1;
                //Uart1Write("INF\n");
                dist = 0xFFFFFFFF;
            } else if (ic_ignoreNextVal == 1) {
                ic_ignoreNextVal = 0;
                //Uart1Write("INF\n");
                dist = 0xFFFFFFFF;
            } else {
                //char string[50];
                //sprintf(string, "dist:%lu\n", dist);
                //Uart1Write(string);
            }
            distanceL = (uint8_t) dist;
            IC3_status = DONE;
            IC3CON2bits.TRIGSTAT = 0; /*Hold the ICx Timer in reset*/
            IC4CON1bits.ICM = 0b000; /*Reset ICx+1 Timer*/
            IC4CON1bits.ICM = 0b001;
            break;
        default:
            while (IC3CON1bits.ICBNE || IC4CON1bits.ICBNE) {
                temp1 = IC3BUF;
                temp1 = IC4BUF;
            }
            break;
    }
}