/* 
 * File:   ping.c
 * Author: Zachary Levenberg
 *
 * Created on August 6, 2016, 9:27 AM
 */

#include "ic.h"

uint8_t ping_Init(ic_pin_number FR, ic_pin_number FL, ic_pin_number RR, ic_pin_number RL){
    ic_Init(FR);
    ic_Init(FL);
    ic_Init(RR);
    ic_Init(RL);
    ic_moduleMode(FR,IC_MODE_DISABLE);
    ic_moduleMode(FL,IC_MODE_DISABLE);
    ic_moduleMode(RR,IC_MODE_DISABLE);
    ic_moduleMode(RL,IC_MODE_DISABLE);
}
