/* 
 * File:   main.c
 * Author: kid group
 *
 * Created on February 24, 2018, 5:32 PM
 */


#include <xc.h>

#include "bolt_clock.h"
#include "tsk.h"

/*
 ** _FGS( OPT1_ON & OPT2_OFF & OPT3_PLL )
 **
 **   General Segment Write-Protect bit:
 **     GWRP_ON              General Segment is write protected
 **     GWRP_OFF             General Segment may be written
 **
 **   General Segment Code-Protect bit:
 **     GSS_ON               Standard Security Code protection Enabled
 **     GSS_OFF              General Segment Code protect is disabled
 **
 **   General Segment Key bits:
 **     GSSK_OFF             General Segment Write Protection and Code Protection is Disabled
 **     GSSK_ON              General Segment Write Protection or Code Protection is Enabled
 **
 */
#ifdef GSS_OFF
_FGS(GWRP_OFF & GSS_OFF & GSSK_OFF)
#else
_FGS(GWRP_OFF)
#endif

/*
 ** _FOSCSEL( OPT1_ON & OPT2_OFF & OPT3_PLL )
 **
 **   Initial Oscillator Source Selection bits:
 **     FNOSC_FRC            Internal Fast RC (FRC)
 **     FNOSC_FRCPLL         Internal Fast RC with PLL (FRCPLL)
 **     FNOSC_PRI            Primary Oscillator (XT, HS, EC)
 **     FNOSC_PRIPLL         Primary Oscillator (XT, HS, EC) with PLL
 **     FNOSC_SOSC           Secondary Oscillator (SOSC)
 **     FNOSC_LPRC           Low-Power RC Oscillator (LPRC)
 **     FNOSC_FRCDIV16       Internal Fast RC (FRC) Oscillator with divide-by-16
 **     FNOSC_FRCDIVN        Internal Fast RC (FRC) Oscillator with postscaler
 **
 **   Two-speed Oscillator Start-up Enable bit:
 **     IESO_OFF             Start up with user-selected oscillator source
 **     IESO_ON              Start up device with FRC, then switch to user-selected oscillator source
 **
 */
_FOSCSEL(FNOSC_FRC & IESO_ON)


/*
 ** _FOSC( OPT1_ON & OPT2_OFF & OPT3_PLL )
 **
 **   Primary Oscillator Mode Select bits:
 **     POSCMD_EC            EC (External Clock) Mode
 **     POSCMD_XT            XT Crystal Oscillator Mode
 **     POSCMD_HS            HS Crystal Oscillator Mode
 **     POSCMD_NONE          Primary Oscillator disabled
 **
 **   OSC2 Pin Function bit:
 **     OSCIOFNC_ON          OSC2 is general purpose digital I/O pin
 **     OSCIOFNC_OFF         OSC2 is clock output
 **
 **   Peripheral pin select configuration:
 **     IOL1WAY_OFF          Allow multiple reconfigurations
 **     IOL1WAY_ON           Allow only one reconfiguration
 **
 **   Clock Switching Mode bits:
 **     FCKSM_CSECME         Both Clock switching and Fail-safe Clock Monitor are enabled
 **     FCKSM_CSECMD         Clock switching is enabled,Fail-safe Clock Monitor is disabled
 **     FCKSM_CSDCMD         Both Clock switching and Fail-safe Clock Monitor are disabled
 **
 */
_FOSC(POSCMD_XT & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSECME)


/*
 ** _FWDT( OPT1_ON & OPT2_OFF & OPT3_PLL )
 **
 **   Watchdog Timer Postscaler bits:
 **     WDTPOST_PS1          1:1
 **     WDTPOST_PS2          1:2
 **     WDTPOST_PS4          1:4
 **     WDTPOST_PS8          1:8
 **     WDTPOST_PS16         1:16
 **     WDTPOST_PS32         1:32
 **     WDTPOST_PS64         1:64
 **     WDTPOST_PS128        1:128
 **     WDTPOST_PS256        1:256
 **     WDTPOST_PS512        1:512
 **     WDTPOST_PS1024       1:1,024
 **     WDTPOST_PS2048       1:2,048
 **     WDTPOST_PS4096       1:4,096
 **     WDTPOST_PS8192       1:8,192
 **     WDTPOST_PS16384      1:16,384
 **     WDTPOST_PS32768      1:32,768
 **
 **   Watchdog Timer Prescaler bit:
 **     WDTPRE_PR32          1:32
 **     WDTPRE_PR128         1:128
 **
 **   PLL Lock Wait Enable bit:
 **     PLLKEN_OFF           Clock switch will not wait for the PLL lock signal.
 **     PLLKEN_ON            Clock switch to PLL source will wait until the PLL lock signal is valid.
 **
 **   Watchdog Timer Window Enable bit:
 **     WINDIS_ON            Watchdog Timer in Window mode
 **     WINDIS_OFF           Watchdog Timer in Non-Window mode
 **
 **   Watchdog Timer Enable bit:
 **     FWDTEN_OFF           Watchdog timer enabled/disabled by user software
 **     FWDTEN_ON            Watchdog timer always enabled
 **
 */
_FWDT(WDTPOST_PS1 & WDTPRE_PR32 & PLLKEN_ON & WINDIS_OFF & FWDTEN_OFF)


/*
 ** Only one invocation of FPOR should appear in a project,
 ** at the top of a C source file (outside of any function).
 **
 ** The following constants can be used to set FPOR.
 ** Multiple options may be combined, as shown:
 **
 ** _FPOR( OPT1_ON & OPT2_OFF & OPT3_PLL )
 **
 **   Power-on Reset Timer Value Select bits:
 **     FPWRT_PWR1           Disabled
 **     FPWRT_PWR2           2ms
 **     FPWRT_PWR4           4ms
 **     FPWRT_PWR8           8ms
 **     FPWRT_PWR16          16ms
 **     FPWRT_PWR32          32ms
 **     FPWRT_PWR64          64ms
 **     FPWRT_PWR128         128ms
 **
 **   Brown-out Reset (BOR) Detection Enable bit:
 **     BOREN_OFF            BOR is disabled
 **     BOREN_ON             BOR is enabled
 **
 **   Alternate I2C pins for I2C1:
 **     ALTI2C1_ON           ASDA1/ASCK1 pins are selected as the I/O pins for I2C1
 **     ALTI2C1_OFF          SDA1/SCK1 pins are selected as the I/O pins for I2C1
 **
 **   Alternate I2C pins for I2C2:
 **     ALTI2C2_ON           ASDA2/ASCK2 pins are selected as the I/O pins for I2C2
 **     ALTI2C2_OFF          SDA2/SCK2 pins are selected as the I/O pins for I2C2
 **
 */
#ifdef FPWRT_PWR1
_FPOR(FPWRT_PWR1 & BOREN_ON & ALTI2C1_OFF & ALTI2C2_OFF)
#else
_FPOR(ALTI2C1_OFF & ALTI2C2_OFF)
#endif


/*
 ** _FICD( OPT1_ON & OPT2_OFF & OPT3_PLL )
 **
 **   ICD Communication Channel Select bits:
 **     ICS_NONE             Reserved, do not use
 **     ICS_PGD3             Communicate on PGEC3 and PGED3
 **     ICS_PGD2             Communicate on PGEC2 and PGED2
 **     ICS_PGD1             Communicate on PGEC1 and PGED1
 **
 **   Reset Target Vector Select bit:
 **     RSTPRI_AF            Device will obtain reset instruction from Aux flash
 **     RSTPRI_PF            Device will obtain reset instruction from Primary flash
 **
 **   JTAG Enable bit:
 **     JTAGEN_OFF           JTAG is disabled
 **     JTAGEN_ON            JTAG is enabled
 **
 */
#ifdef RSTPRI_PF
_FICD(ICS_PGD1 & RSTPRI_PF & JTAGEN_OFF)
#else
_FICD(ICS_PGD1 & JTAGEN_OFF)
#endif

/*
 ** _FAS( OPT1_ON & OPT2_OFF & OPT3_PLL )
 **
 **   Auxiliary Segment Write-protect bit:
 **     AWRP_ON              Aux Flash is write protected
 **     AWRP_OFF             Aux Flash may be written
 **
 **   Auxiliary Segment Code-protect bit:
 **     APL_ON               Aux Flash Code protect is enabled
 **     APL_OFF              Aux Flash Code protect is disabled
 **
 **   Auxiliary Segment Key bits:
 **     APLK_OFF             Aux Flash Write Protection and Code Protection is Disabled
 **     APLK_ON              Aux Flash Write Protection or Code Protection is Enabled
 **
 */
#ifdef AWRP_ON
_FAS(AWRP_ON & APL_ON & APLK_ON)
#endif

/*
 * 
 */
int main(void) {

    clockInit(FREQ_120MHZ, INTERNAL);

    Tsk_Run(clockFreq());

    while (1) {
    }

    return (0);
}

