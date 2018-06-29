#include <xc.h>
#include "ezbl_integration/ezbl.h"

EZBL_AllocFlashHole(emuEEData, 3072, 0x800, -1);
struct
{
    unsigned int ledsToBlink;
} eeVars;
NOW_TASK ledToggleTask;

int toggleLED(void)
{
    return LEDToggle(eeVars.ledsToBlink);
}

int main(void)
{
    EZBL_ReadROMObj(&eeVars, EZBL_FlashHoleAddr(emuEEData));
    if(eeVars.ledsToBlink == 0xFFFFu)
    {   // On erased power up, define initial state
        eeVars.ledsToBlink = 0x0003u;
    }
    NOW_CreateRepeatingTask(&ledToggleTask, toggleLED, 500*NOW_ms);
    
    while(1)
    {
        if(!_RD13)
        {
            eeVars.ledsToBlink = (eeVars.ledsToBlink + 0x1) & 0x00FF;
            LEDSet(eeVars.ledsToBlink);
            EZBL_WriteROMObj(EZBL_FlashHoleAddr(emuEEData), &eeVars);
            while(!_RD13);
        }
        Idle();
    }
}
