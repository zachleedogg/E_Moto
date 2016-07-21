/*******************************************************************************************
 Module:
    bolt_init.c
 Purpose:
    To facilitate easy configuration of the PIC33 Micro for use in projects
 author: Zach Levenberg
 
 History
 ***********
 6/13/16: initial write; brdgordo
 *********************************************************************************************/

#include "bolt_init.h"

/*Communicate on PGED1 and PGED1, JTAG is OFF*/

_FICD(ICS_PGD1 & JTAGEN_OFF);

// FPOR
/* ALTI2C1 = OFF        Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)*/
/* ALTI2C2 = OFF        Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)*/
/* WDTWIN = WIN25       Watchdog Window Select bits (WDT Window is 25% of WDT period)*/
_FPOR(ALTI2C1_OFF & ALTI2C2_OFF & WDTWIN_WIN25)

// FWDT
/* WDTPOST = PS32768    Watchdog Timer Postscaler bits (1:32,768)*/
/* WDTPRE = PR128       Watchdog Timer Prescaler bit (1:128)*/
/* PLLKEN = OFF         PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)*/
/* WINDIS = OFF         Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)*/
/* FWDTEN = OFF         Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)*/
_FWDT(WDTPOST_PS32768 & WDTPRE_PR128 & PLLKEN_OFF & WINDIS_OFF & FWDTEN_OFF);

// FOSC
/*POSCMD = HS         Primary Oscillator Mode Select bits (HS Crystal Oscillator Mode)*/
/*OSCIOFNC = ON       OSC2 Pin Function bit (OSC2 is IO)*/
/*IOL1WAY = ON       Peripheral pin select configuration (Allow multiple reconfigurations)*/
/*FCKSM = CSECMD      Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)*/
//_FOSC(POSCMD_HS & FCKSM_CSECMD & OSCIOFNC_OFF & IOL1WAY_ON);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT & IOL1WAY_ON);

/* FOSCSEL */
/*FNOSC = FRCDIVN          Oscillator Source Selection (Internal Fast RC (FRC) divide by N*/
/*IESO = ON           (Start up with internal oscillator source)*/
/*PWMLOCK = OFF        no unlock for PWM module*/
/*select Internal Fast RC clock at Power-On Reset*/
#if defined(__dsPIC33EP256MC502__) || defined(__dsPIC33EP256MC504__)
_FOSCSEL(FNOSC_FRCDIVN & IESO_ON & PWMLOCK_OFF);
#endif

#if defined (__dsPIC33EP32GP502__)
_FOSCSEL(FNOSC_FRCDIVN & IESO_OFF);
#endif

/* FGS */
/*GWRP = OFF           General Segment Write-Protect bit (General Segment may be written)*/
/*GCP = OFF            General Segment Code-Protect bit (General Segment Code protect is Disabled)*/
_FGS(GWRP_OFF & GCP_OFF);





/***************************************************************************************
 Micro Functions
 ***************************************************************************************/
#define INTERNAL_OSC 7394800
#define EXTERNAL_OSC 8000000

static uint32_t freq;

/*
 Function: Micro_Init()
 Params: none
 Return: void
 Description: Initializes PIC33 for operation. Disables all analog pins and sets all io pins
    to output low
 author: brdgordo - 1/22/16
 */

void Micro_Init(void) {
    /* Configure all AD pins as digital */
    ANSELA = ANSELB = 0x0000;
#ifdef ANSELC
    ANSELC = 0x0000;
#endif

    /* Set the PWM's off */
#if defined(__dsPIC33EP256MC502__) || defined(__dsPIC33EP256MC504__)
    IOCON1bits.PENH = 0;
    IOCON1bits.PENL = 0;
    IOCON2bits.PENH = 0;
    IOCON2bits.PENL = 0;
    IOCON3bits.PENH = 0;
    IOCON3bits.PENL = 0;
#endif
}

uint16_t clockInit(uint32_t clockSpeed, uint8_t source) {

    static uint32_t OSC;
    if (source == INTERNAL) {
        OSC = INTERNAL_OSC;
    } else if (source == EXTERNAL) {
        OSC = EXTERNAL_OSC;
    }

    /*if greater than 8MHZ, initiate PhaseLockedLoop*/
    if (clockSpeed > FREQ_8MHZ) {
        uint8_t pllPre = 0;
        uint8_t pllPost = 0;
        uint8_t pllMult = 0;

        /*Set PLL postScaler PLL_POST*/
        while ((((uint32_t) ((pllPost + 1)*2)) * clockSpeed) < 120000000) {
            pllPost++;
            if (pllPost == 2) {/*this rule must be followed per data sheet*/
                pllPost++;
            }
        }

        /*Set PLL preScaler PLL_PRE*/
        while ((OSC / ((uint32_t) (pllPre + 2))) >= 8000000) {
            pllPre++;
        }

        /*Set PLL multiplier PLL_MULT*/
        pllMult = (clockSpeed * ((uint32_t) ((pllPost + 1)*2)) / (OSC / ((uint32_t) (pllPre + 2))) - 2);
        if (pllMult > 511) {
            return 1; /*Failure*/
        }

        /*Set the global freq*/
        freq = (OSC * (pllMult + 2) / ((pllPre + 2)*((pllPost + 1)*2)));


        /* Configure PLL prescaler such that 0.8MHz < INTERNAL_CLOCK/(PLLPRE+2) < 8MHz*/
        CLKDIVbits.PLLPRE = pllPre; // N1=2, input divided by 3
        /*PRESCALED_CLOCK = 2456667*/

        /*Configure PLL postscaler such that 120MHz < Fin*FBD/(PLLPRE) < 340MHz*/
        CLKDIVbits.PLLPOST = pllPost; /* N2=2, output of PLL divided by 2*/
        /*POSTSCALED_CLOCK = 160000000*/

        /*Configure PLL divisor to obtain final frequency:
         * FREQ = PRESCALED_CLOCK*(PLLFBD+2)/((PLLPOST+2)*(PLLPRE+2)) */
        PLLFBD = pllMult; // M=30, PLL multiplier by 13


        /* Initiate Clock Switch to PRI oscillator with PLL (NOSC=0b011)*/
        if (source == INTERNAL) {
            __builtin_write_OSCCONH(0b001); /*internalFRC w PLL request*/
        } else if (source == EXTERNAL) {
            __builtin_write_OSCCONH(0b011); /*PrimaryOSC w PLL request*/
        }
        __builtin_write_OSCCONL(OSCCON | 0x01); /*submit request*/

        /* Wait for Clock switch to occur*/
        if (source == INTERNAL) {
            while (OSCCONbits.COSC != 0b001); /*internalFRC w PLL locked*/
        } else if (source == EXTERNAL) {
            while (OSCCONbits.COSC != 0b011); /*PrimaryOSC w PLL locked*/
        }
        /* Wait for PLL to lock*/
        while (OSCCONbits.LOCK != 1);
    } else if (clockSpeed == FREQ_8MHZ) {
        /* Initiate Clock Switch to PRI oscillator*/
        if (source == INTERNAL) {
            /*Do nothing cause we already at 8MHz*/
        } else if (source == EXTERNAL) {
            __builtin_write_OSCCONH(0b010); /*PrimaryOSC request*/
        }
        __builtin_write_OSCCONL(OSCCON | 0x01); /*submit request*/

        /* Wait for Clock switch to occur*/
        if (source == INTERNAL) {
            /*Do nothing cause we already at 8MHz*/
        } else if (source == EXTERNAL) {
            while (OSCCONbits.COSC != 0b010);
        }
        freq = OSC;
    } else if (clockSpeed == FREQ_32KHZ) {
        __builtin_write_OSCCONH(0b101); /*LowPowerRC request*/
        __builtin_write_OSCCONL(OSCCON | 0x01); /*submit request*/
        /* Wait for Clock switch to occur*/
        while (OSCCONbits.COSC != 0b101);
        freq = OSC;
    } else if (clockSpeed < FREQ_8MHZ) {

        /*Set the prescaler, CANNOT USE EXRTERNAL CLOCK*/
        uint8_t div = 0;
        for (div = 1; div < 8; div++) {
            if ((FREQ_8MHZ >> div) == clockSpeed) {
                CLKDIVbits.FRCDIV = div;
                freq = OSC / (1 << div);
                break;
            }
        }
    }
    return 1;
}

uint32_t clockFreq() {
    return freq;
}

/*
 Function: T1_Interrupt_Init()
 Param: period: in milliseconds
        priority: the priority of the interrupt, 1-7
 Return: void
 Description: Initializes timer 1 interrupt
 author: brdgordo - 1/28/16
 */
uint8_t T1_Interrupt_Init(uint16_t period, uint8_t priority) {
    uint32_t currentSpeed = clockFreq() / 2;
    uint8_t scalerIsSelected = 0;
    uint8_t scalerValue = 0;
    while (!scalerIsSelected) {
        uint32_t tempSpeed = (((uint32_t) period)*(currentSpeed / (uint32_t) 1000));
        if (tempSpeed > 0x0000FFFF) {
            scalerValue++;
            switch (scalerValue) {
                case 1:
                case 2:
                    currentSpeed = currentSpeed / 8;
                    break;
                case 3:
                    currentSpeed = currentSpeed / 4;
                    break;
                default:
                    return 1;
            }
        } else {
            scalerIsSelected = 1;
        }
    }
    float tempPeriod = ((float) period)*((float) currentSpeed / 1000.0);
    /* Initialize timer */
    T1CON = 0; /* Clear timer config register */
    T1CONbits.TCKPS = scalerValue; /* prescaler set */
    PR1 = (uint16_t) (tempPeriod); /*set timer period */

    /* Enable level 1-7 interrupts */
    /* No restoring of previous CPU IPL state performed here */
    INTCON2bits.GIE = 1;

    /* set timer 1 interrupt priority */
    _T1IP = priority;

    /* reset timer 1 interrupt */
    _T1IF = 0;

    /* Enable timer 1 interrupt */
    _T1IE = 1;

    /* Turn timer 1 on */
    T1CONbits.TON = 1;
    return 0;
}

/*
 Function: T1_Interrupt_Disable()
 Params: none
 Return: void
 Description: Disables timer 1 interrupt
 author: brdgordo - 3/1/16
 */
void T1_Interrupt_Disable(void) {
    _T1IE = 0;
    T1CONbits.TON = 0;
}

/*
 Function: T2_Interrupt_Init()
 Param: period: in milliseconds
        priority: the priority of the interrupt, 1-7
 Return: void
 Description: Initializes timer 2 interrupt
 author: brdgordo - 1/28/16
 */
uint8_t T2_Interrupt_Init(uint16_t period, uint8_t priority) {
    uint32_t currentSpeed = clockFreq() / 2;
    uint8_t scalerIsSelected = 0;
    uint8_t scalerValue = 0;
    while (!scalerIsSelected) {
        uint32_t tempSpeed = (((uint32_t) period)*(currentSpeed / 1000));
        if (tempSpeed > 0x0000FFFF) {
            scalerValue++;
            switch (scalerValue) {
                case 1:
                case 2:
                    currentSpeed = currentSpeed / 8;
                    break;
                case 3:
                    currentSpeed = currentSpeed / 4;
                    break;
                default:
                    return 1;
            }
        } else {
            scalerIsSelected = 1;
        }
    }
    float tempPeriod = ((float) period)*((float) currentSpeed / 1000.0);
    /* Initialize timer */
    T2CON = 0; /* Clear timer config register */
    T2CONbits.TCKPS = scalerValue; /* prescaler set */
    PR2 = (uint16_t) tempPeriod; /*set timer period */

    /* Enable level 1-7 interrupts */
    /* No restoring of previous CPU IPL state performed here */
    INTCON2bits.GIE = 1;

    /* set timer 2 interrupt priority */
    _T2IP = priority;

    /* reset timer 2 interrupt */
    _T2IF = 0;

    /* Enable timer 2 interrupt */
    _T2IE = 1;

    /* Turn timer 2 on */
    T2CONbits.TON = 1;
    return 0;
}

/*
 Function: T2_Interrupt_Disable()
 Params: none
 Return: void
 Description: Disables timer 2 interrupt
 author: brdgordo - 3/1/16
 */
void T2_Interrupt_Disable(void) {
    _T2IE = 0;
    T2CONbits.TON = 0;
}

/*
 Function: T3_Interrupt_Init()
 Param: period: in MICROSECONDS
        priority: the priority of the interrupt, 1-7
 Return: void
 Description: Initializes timer 3 interrupt
 author: brdgordo - 1/28/16
 */
uint8_t T3_Interrupt_Init(uint16_t period, uint8_t priority) {
    uint32_t currentSpeed = clockFreq() / 2;
    uint8_t scalerIsSelected = 0;
    uint8_t scalerValue = 0;
    while (!scalerIsSelected) {
        uint32_t tempSpeed = (((uint32_t) period)*(currentSpeed / 1000000));
        if (tempSpeed > 0x0000FFFF) {
            scalerValue++;
            switch (scalerValue) {
                case 1:
                case 2:
                    currentSpeed = currentSpeed / 8;
                    break;
                case 3:
                    currentSpeed = currentSpeed / 4;
                    break;
                default:
                    return 1;
            }
        } else {
            scalerIsSelected = 1;
        }
    }
    float tempPeriod = ((float) period)*((float) currentSpeed / 1000000.0);
    /* Initialize timer */
    T3CON = 0; /* Clear timer config register */
    T3CONbits.TCKPS = scalerValue; /* prescaler set */
    PR3 = (uint16_t) tempPeriod; /*set timer period */

    /* Enable level 1-7 interrupts */
    /* No restoring of previous CPU IPL state performed here */
    INTCON2bits.GIE = 1;

    /* set timer 3 interrupt priority */
    _T3IP = priority;

    /* reset timer 3 interrupt */
    _T3IF = 0;

    /* Enable timer 3 interrupt */
    _T3IE = 1;

    /* Turn timer 3 on */
    T3CONbits.TON = 1;
    return 0;
}

/*
 Function: T3_Interrupt_Disable()
 Params: none
 Return: void
 Description: Disables timer 3 interrupt
 author: brdgordo - 3/1/16
 */
void T3_Interrupt_Disable(void) {
    _T3IE = 0;
    T3CONbits.TON = 0;
}

uint8_t T4_Interrupt_Init(uint16_t period, uint8_t priority) {
    uint32_t currentSpeed = clockFreq() / 2;
    uint8_t scalerIsSelected = 0;
    uint8_t scalerValue = 0;
    while (!scalerIsSelected) {
        uint32_t tempSpeed = (((uint32_t) period)*(currentSpeed / 1000));
        if (tempSpeed > 0x0000FFFF) {
            scalerValue++;
            switch (scalerValue) {
                case 1:
                case 2:
                    currentSpeed = currentSpeed / 8;
                    break;
                case 3:
                    currentSpeed = currentSpeed / 4;
                    break;
                default:
                    return 1;
            }
        } else {
            scalerIsSelected = 1;
        }
    }
    float tempPeriod = ((float) period)*((float) currentSpeed / 1000.0);
    /* Initialize timer */
    T4CON = 0; /* Clear timer config register */
    T4CONbits.TCKPS = scalerValue; /* prescaler set */
    PR4 = (uint16_t) tempPeriod; /*set timer period */

    /* Enable level 1-7 interrupts */
    /* No restoring of previous CPU IPL state performed here */
    INTCON2bits.GIE = 1;

    /* set timer 4 interrupt priority */
    _T4IP = priority;

    /* reset timer 4 interrupt */
    _T4IF = 0;

    /* Enable timer 4 interrupt */
    _T4IE = 1;

    /* Turn timer 4 on */
    T4CONbits.TON = 1;
    return 0;
}

/*
 Function: T4_Interrupt_Disable()
 Params: none
 Return: void
 Description: Disables timer 4 interrupt
 author: brdgordo - 3/1/16
 */
void T4_Interrupt_Disable(void) {
    _T4IE = 0;
    T4CONbits.TON = 0;
}