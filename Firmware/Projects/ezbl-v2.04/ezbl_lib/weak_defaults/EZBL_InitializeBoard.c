/*******************************************************************************
  Easy Bootloader for PIC24/dsPIC and PIC32MM (ezbl_lib)

  Summary:
    Implements the default InitializeBoard() weak function in ezbl_lib. This
    function is never included unless your Bootloader or Application calls
    InitializeBoard() without a local implementation present in any of your
    source files.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Copyright (C) 2017 Microchip Technology Inc.

  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
  derivatives created by any person or entity by or on your behalf, exclusively
  with Microchip's products.  Microchip and its licensors retain all ownership
  and intellectual property rights in the accompanying software and in all
  derivatives here to.

  This software and any accompanying information is for suggestion only.  It
  does not modify Microchip's standard warranty for its products.  You agree
  that you are solely responsible for testing the software and determining its
  suitability.  Microchip has no obligation to modify, test, certify, or
  support the software.

  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
  TERMS.
*******************************************************************************/
// DOM-IGNORE-END


// dsPIC33C PLL configuration structures
typedef struct
{
    unsigned PLLPRE:6;
    unsigned :2;
    unsigned FRCDIV:3;
    unsigned DOZEN:1;
    unsigned DOZE:3;
    unsigned ROI:1;
} CLKDIVBITS;
typedef struct
{
    unsigned POST2DIV:3;
    unsigned :1;
    unsigned POST1DIV:3;
    unsigned :1;
    unsigned VCODIV:2;
    unsigned :6;
} PLLDIVBITS;


extern volatile unsigned int PLLFBD         __attribute__((near, weak));
extern volatile unsigned int OSCCON         __attribute__((near, weak));
extern volatile unsigned int CLKDIV         __attribute__((near));
extern volatile CLKDIVBITS   CLKDIVbits     __attribute__((near, weak));
extern volatile unsigned int PLLDIV         __attribute__((near, weak));    // dsPIC33C only
extern volatile PLLDIVBITS   PLLDIVbits     __attribute__((near, weak));    // dsPIC33C only
extern volatile unsigned int OSCDIV         __attribute__((near, weak));    // PIC24FJ256GA705 and PIC24FJ1024GA610/GB610 families


#define _OSCCON_OSWEN_MASK                  0x0001
#define _PLLDIV_POST2DIV_POSITION           0x0000
#define _PLLDIV_POST1DIV_POSITION           0x0004
#define _PLLDIV_VCODIV_POSITION             0x0008



/**
 * Dummy weak function for allowing compilation on devices which we don't have
 * initialization code written for. Since this is weak, any ordinary
 * EZBL_InitializeBoard() function which is implemented will take precedence and
 * this weak dummy will get discarded automatically/not sucked in from the
 * archive.
 *
 * @return Assumed system clock frequency. ex: 16000000, 40000000, 60000000 or
 *         80000000. This may or may not be accurate at all if run-time clock
 *         switching is disabled or the OSCCON/PLL configuration settings can't
 *         be generically computed.
 * 
 *         For PIC24H/dsPIC33F devices, the code will attempt to configure the
 *         PLL for 40 MIPS from the 7.37MHz FRC.
 *
 *         For PIC24E/dsPIC33E devices, 60 MIPS will be attempted via the
 *         7.37MHz FRC + PLL.
 *
 *         For dsPIC33C devices, 80 MIPS will be attempted with a 1.2GHz VCO
 *         output via the 8.0MHz FRC + PLL.
 * 
 *         _FOSC Config word needs to have FCKSM_CSECME or FCKSM_CSECMD for
 *         clock switching to work.
 */
unsigned long __attribute__((weak)) EZBL_InitializeBoard(void)
{
    volatile unsigned int i;
    unsigned long predictedFcy;

    // Switch to FRC clock (no PLL), in case if the PLL is currently in use.
    // We are not allowed to change the PLL prescalar, postscalar or feedback
    // divider while it is running.
    __builtin_write_OSCCONH(0x00);                          // 000 = Fast RC Oscillator (FRC)
    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);

    __builtin_clrwdt();
    i = 0x0FFF;
    while(OSCCON & _OSCCON_OSWEN_MASK)  // Wait for clock switch to complete by polling OSWEN
    {
        // Abort waiting if we don't succeed within a few hundred microseconds
        // (~351us at 70 MIPS) or few milliseconds (~13.3ms @ 1.84 MIPS). The
        // FRC should start up and clock switch in less than 20us, so a failure
        // to switch probably means OSCCON<CLKLOCK> is set, run-time clock
        // switching hasn't been enabled in FOSC, or you are operating in debug
        // mode and need to restart your MPLAB X IDE/debugger).
        if(--i == 0u)
            break;      
    }

    if(&PLLFBD) // dsPIC33F/dsPIC33E/PIC24H/PIC24E with run-time configurable PLL feedback divider
    {
#if defined(EZBL_LIB16EP)   // -mcpu=generic-16bit-ep processor target (PIC24E/dsPIC33E devices only)
        #define FCY             60000000ul  // Desired frequency

        // Configure PLL for Fosc = 120MHz/Fcy = 60MIPS using 7.37 MHz internal FRC oscillator
        CLKDIV = 0xB000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 2:1
        PLLFBD = (FCY * 2u * 2u * 2u + 7370000u / 2u) / 7370000u - 2u; // 63 @ 60 MIPS (7.37 MHz input clock from FRC)
        predictedFcy = (((7370000u/8u) * (((FCY*2ul) * 8u*2u + 7370000u/2u) / 7370000u) / 2u) / 2u);     //  (Fin/PLLPRE) * (PLLFBD rounded with with 2 added back) / PLLPOST / (2 FOSC/FCY)

#elif defined(EZBL_LIB16CH) // -mcpu=generic-16dsp-ch processor target (dsPIC33C devices only)
        #define PLL_IN_CLK             8000000ul    // Define PLL input: 8.0 MHz from FRC
        #define FVCO                1200000000ul    // Choose raw VCO output: 1.2 GHz for High Resolution PWM and DAC. Can affect precision achieving target FCY.
        #define FCY                   80000000ul    // Choose CPU speed: 80 MIPS clock (160 MHz) for CPU execution. Change this and PLL initialization will compute needed scalars to achieive this output clock.

        // Macros to auto-compute optimal PLL dividers given the PLL_IN_CLK, chosen CPU
        // FCY, and target FVCO settings above. You should change those
        // named values and not have to do anything to the NET_POST_DIV and BEST_PLLDIV
        // macros.
        #define NET_POST_DIV    (4u*(FVCO/(4.0*FCY) >= 49 ? 7u*7u : \
                                    (FVCO/(4.0*FCY) >= 42 ? 7u*6u : \
                                    (FVCO/(4.0*FCY) >= 36 ? 6u*6u : \
                                    (FVCO/(4.0*FCY) >= 35 ? 7u*5u : \
                                    (FVCO/(4.0*FCY) >= 30 ? 6u*5u : \
                                    (FVCO/(4.0*FCY) >= 28 ? 7u*4u : \
                                    (FVCO/(4.0*FCY) >= 10 ? 5u*5u : \
                                    (FVCO/(4.0*FCY) >= 21 ? 7u*3u : \
                                    (FVCO/(4.0*FCY) >= 20 ? 5u*4u : \
                                    (FVCO/(4.0*FCY) >= 18 ? 6u*3u : \
                                    (FVCO/(4.0*FCY) >= 16 ? 4u*4u : \
                                    (FVCO/(4.0*FCY) >= 14 ? 7u*2u : \
                                    (FVCO/(4.0*FCY) >= 12 ? 6u*2u : \
                                    (FVCO/(4.0*FCY) >= 10 ? 5u*2u : \
                                    (FVCO/(4.0*FCY) >=  9 ? 3u*3u : \
                                    (FVCO/(4.0*FCY) >=  8 ? 4u*2u : \
                                    (FVCO/(4.0*FCY) >=  7 ? 7u*1u : \
                                    (FVCO/(4.0*FCY) >=  6 ? 6u*1u : \
                                    (FVCO/(4.0*FCY) >=  5 ? 5u*1u : \
                                    (FVCO/(4.0*FCY) >=  4 ? 4u*1u : \
                                    (FVCO/(4.0*FCY) >=  3 ? 3u*1u : \
                                    (FVCO/(4.0*FCY) >=  2 ? 2u*1u : \
                                                            1u*1u)))))))))))))))))))))))

        #define BEST_PLLDIV (FVCO/(4.0*FCY) >= 49 ? (7<<_PLLDIV_POST1DIV_POSITION) | (7<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 42 ? (7<<_PLLDIV_POST1DIV_POSITION) | (6<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 36 ? (6<<_PLLDIV_POST1DIV_POSITION) | (6<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 35 ? (7<<_PLLDIV_POST1DIV_POSITION) | (5<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 30 ? (6<<_PLLDIV_POST1DIV_POSITION) | (5<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 28 ? (7<<_PLLDIV_POST1DIV_POSITION) | (4<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 25 ? (5<<_PLLDIV_POST1DIV_POSITION) | (5<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 21 ? (7<<_PLLDIV_POST1DIV_POSITION) | (3<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 20 ? (5<<_PLLDIV_POST1DIV_POSITION) | (4<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 18 ? (6<<_PLLDIV_POST1DIV_POSITION) | (3<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 16 ? (4<<_PLLDIV_POST1DIV_POSITION) | (4<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 14 ? (7<<_PLLDIV_POST1DIV_POSITION) | (2<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 12 ? (6<<_PLLDIV_POST1DIV_POSITION) | (2<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >= 10 ? (5<<_PLLDIV_POST1DIV_POSITION) | (2<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >=  9 ? (3<<_PLLDIV_POST1DIV_POSITION) | (3<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >=  8 ? (4<<_PLLDIV_POST1DIV_POSITION) | (2<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >=  7 ? (7<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >=  6 ? (6<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >=  5 ? (5<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >=  4 ? (4<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >=  3 ? (3<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                            (FVCO/(4.0*FCY) >=  2 ? (2<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION) :  \
                                                    (1<<_PLLDIV_POST1DIV_POSITION) | (1<<_PLLDIV_POST2DIV_POSITION)))))))))))))))))))))))

        // Configure PLL for Fosc = 1200MHz/Fcy = 80MIPS using 8.0 MHz internal FRC oscillator
        CLKDIV = 0xB000; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1
        CLKDIVbits.PLLPRE = 1u + ((PLL_IN_CLK)/64000000ul); // 1:1 PLL Input clock divider (use raw 8.0 MHz FRC clock); PLL Input legal range is 8-64 MHz; PLL Feedback Input must be between 8 MHz and FVCO/16 MHz; VCO Output limit is 400MHz to 1600MHz
        PLLDIV = BEST_PLLDIV;
        PLLFBD = NET_POST_DIV*FCY/((unsigned long)((PLL_IN_CLK)/((unsigned long)(1u + (PLL_IN_CLK/64000000ul)))));
        predictedFcy = FCY;

#elif defined(EZBL_LIB16)     // -mcpu=generic-16bit processor target (dsPIC30F, PIC24H or dsPIC33F)
        #define FCY             40000000ul  // Desired frequency

        // Configure PLL for Fosc = 80MHz/Fcy = 40MIPS using 7.37 MHz internal FRC oscillator
        CLKDIV = 0xB006; // ROI = 1, DOZE = 8:1, FRCDIV = 1:1, PLLPOST = 2:1, PLLPRE = 8:1
        PLLFBD = ((FCY*2u) * 8u*2u + 7370000u/2u) / 7370000u - 2u; // 345 @ 40 MIPS (7.37 MHz input clock from FRC, VCO @ 319.67 MHz)
        predictedFcy = (((7370000u/8u) * (((FCY*2ul) * 8u*2u + 7370000u/2u) / 7370000u) / 2u) / 2u);    // (Fin/PLLPRE) * (PLLFBD rounded with with 2 added back) / PLLPOST / (2 FOSC/FCY)

#else   // Unknown device - should never happen for dsPIC30F/dsPIC33F/dsPIC33E/dsPIC33C/PIC24H/PIC24E devices
        predictedFcy = (7370000u/2u);   // Unknown device - assume we are running from the FRC with 1:1 scalar
#endif
    }
    else    // PIC24F device with fixed PLL controlled by flash Config words
    {
        CLKDIV = 0xB000;    // ROI = 1, DOZE = 8:1, FRCDIV = 1:1
#if defined(EZBL_LIB16DA)   // -mcpu=generic-16bit-da processor target (PIC24F with EDS (DSRPAG/DSWPAG registers), not PSV)
        if(&OSCDIV) // PIC24FJ1024GA610/GB610 and PIC24FJ256GA705 families (generic-16bit-da devices) have OSCDIV register instead of FRCDIV<2:0> in CLKDIV
        {
            // Configure PLL for Fosc = 32MHz/Fcy = 16MIPS using 8 MHz internal FRC oscillator
            CLKDIV = 0xB120;    // ROI = 1, DOZE = 8:1, RCDIV<2:0> = Fast RC Oscillator (FRC) with PLL module (FRCPLL), CPDIV<1:0> = 32MHz (96MHz PLL post divider div by 1 for 32MHz CPU clock); PLLEN = 1 (PLL stays active for USB when CPU not being clocked from PLL)
            OSCDIV = 0x0000;    // 1:1 divide on FRC, XT or EC oscillator (2:1 is the default)
        }
#endif
        predictedFcy = 16000000u;
    }

    __builtin_write_OSCCONH(0x01);                          // NOSC = 001 = Fast RC Oscillator (FRC) with Divide-by-N and PLL (FRCPLL)
    __builtin_write_OSCCONL(OSCCON | _OSCCON_OSWEN_MASK);   // Initiate Clock Switch to use the FRC Oscillator + PLL (NOSC = 0b001)

    __builtin_clrwdt();
    i = 0x1FFF;
    while(OSCCON & _OSCCON_OSWEN_MASK)  // Wait for clock switch to complete by polling OSWEN
    {
        // Abort waiting if we don't succeed within a few milliseconds (~13.3ms
        // at 3.69 MIPS). It could take up to 3.1ms for a PLL to lock and
        // complete a switch, so not successfully switching may mean we are
        // running from the FRC.
        if(--i == 0u)
            return 7370000u/2u;
    }

    return predictedFcy;
}
