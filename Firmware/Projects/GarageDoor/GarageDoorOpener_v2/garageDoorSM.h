#ifndef GARAGE_DOOR_SM_H
#define GARAGE_DOOR_SM_H

#include "Arduino.h"

static char garageDoorStateString[5][15] = {
  "init\n",
  "closing\n",
  "closed\n",
  "opening\n",
  "open\n",
};


void garageDoorInit(void);

uint8_t garageDoorSM(uint8_t buttonPressed);

#endif
