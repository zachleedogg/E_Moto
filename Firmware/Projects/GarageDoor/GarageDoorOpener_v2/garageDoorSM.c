
#include "garageDoorSM.h"
#include <Arduino.h>


#define ANALOG_TO_DIG(x) (14+x)

#define DOOR_SW 2
#define DOOR_SW_V2 3
#define REED_1 4
#define REED_2 5



//Timers
#define MINS_TO_MILLIS(x) (x*60000)
#define DOOR_AUTOCLOSE_DELAY MINS_TO_MILLIS(15) //15 minutes
static uint32_t doorAutoCloseTimer = 0;

#define DOOW_SW_HOLD_DELAY 500
static uint16_t doorSwitchHoldTimer = DOOW_SW_HOLD_DELAY;

/*Password Stuff*/
static const uint8_t passcode[] = {'4','1','5','3'};
#define PASSCODE_SIZE sizeof(passcode)
static uint8_t code[PASSCODE_SIZE + 1] = {};
static uint8_t codeIndex = 0;
typedef enum {
  NOTHING,
  PASSED,
  FAILED,
} passwordOutcomes_E;

typedef enum {
  NOT_POSSIBLE,
  DOOR_DOWN,
  NOT_POSSIBLE_2,
  TRANSITIONING,
} doorPosition_E;
static doorPosition_E currentReedSwitchPosition = 0;
static uint16_t currentReedSwitchDebounce = 0;
#define REED_SW_DEBOUNCE_TIME 1000

//States
typedef enum {
  INIT,
  CLOSED,
  OPENING,
  NUMBER_OF_STATES,
} garageDoorStatus_E;
static garageDoorStatus_E garageDoorStatus;

//Helper Functions
static doorPosition_E checkReeds(void);
static doorPosition_E getDoorStatus(void);
static passwordOutcomes_E passwordHandler(uint8_t value);

void garageDoorInit(void) {

  pinMode(DOOR_SW, OUTPUT);
  digitalWrite(DOOR_SW, HIGH);

  pinMode(DOOR_SW_V2, INPUT);
  digitalWrite(DOOR_SW_V2, LOW);

  pinMode(REED_1, INPUT_PULLUP);
  pinMode(REED_2, INPUT_PULLUP);
  currentReedSwitchPosition = checkReeds();
}

uint8_t garageDoorSM(uint8_t buttonPressed) {

  //Check Reed Switch positions to dertimine if door is open or closed.
  doorPosition_E doorPosition = getDoorStatus();

  //Check key input for correct Password
  passwordOutcomes_E thisPW = passwordHandler(buttonPressed);

  //Check Door SW timer
  if(doorSwitchHoldTimer++ == DOOW_SW_HOLD_DELAY){
    doorSwitchHoldTimer = 0;
    digitalWrite(DOOR_SW, HIGH);
    pinMode(DOOR_SW_V2, INPUT);
  }

  //Run the state machine
  garageDoorStatus_E garageDoorStatusLast = garageDoorStatus;
  switch (garageDoorStatus) {
    case INIT:
      switch (doorPosition) {
        case TRANSITIONING:
          garageDoorStatus = OPENING;
          break;
        case DOOR_DOWN:
          garageDoorStatus = CLOSED;
          break;
        default:
          garageDoorStatus = OPENING;
          break;
      }
      break;

    case CLOSED:
      if (thisPW == PASSED) {
        digitalWrite(DOOR_SW, LOW);
        pinMode(DOOR_SW_V2, OUTPUT);
        digitalWrite(DOOR_SW_V2, LOW);
        doorSwitchHoldTimer = 0;
      }
      switch (doorPosition) {
        case TRANSITIONING:
          garageDoorStatus = OPENING;
          break;
        case DOOR_DOWN:
          garageDoorStatus = CLOSED;
          break;
        default:
          garageDoorStatus = OPENING;
          break;
      }
      break;

    case OPENING:
      if (buttonPressed == '#') {
        digitalWrite(DOOR_SW, LOW);
        pinMode(DOOR_SW_V2, OUTPUT);
        digitalWrite(DOOR_SW_V2, LOW);
        doorSwitchHoldTimer = 0;
      }
      switch (doorPosition) {
        case TRANSITIONING:
          garageDoorStatus = OPENING;
          break;
        case DOOR_DOWN:
          garageDoorStatus = CLOSED;
          break;
        default:
          garageDoorStatus = OPENING;
          break;
      }
      break;

    default:
      break;
  }

  if (garageDoorStatusLast != garageDoorStatus) {
    return garageDoorStatus;
  } else {
    return INIT;
  }
}


static doorPosition_E checkReeds(void) {
  return (digitalRead(REED_1) | digitalRead(REED_2) << 1);
}

static doorPosition_E getDoorStatus(void) {
  doorPosition_E temp = checkReeds();
  if (temp != currentReedSwitchPosition) {
    if (currentReedSwitchDebounce++ >= REED_SW_DEBOUNCE_TIME) {
      currentReedSwitchDebounce = 0;
      currentReedSwitchPosition = temp;
    }
  } else {
    currentReedSwitchDebounce = 0;
  }
  return currentReedSwitchPosition;
}


static passwordOutcomes_E passwordHandler(uint8_t value) {
  uint8_t returnVal = NOTHING;

  switch (value) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      code[codeIndex++] = value;
      if (codeIndex >= PASSCODE_SIZE) {
        codeIndex--;
      }
      break;

    case '*':
      for (uint8_t i = 0; i < PASSCODE_SIZE; i++) {
        code[i] = codeIndex = 0; /*make string empty*/
      }
      returnVal = FAILED;
      break;

    case '#':
      codeIndex = 0;
      uint16_t i = 0;
      for (i = 0; i < PASSCODE_SIZE; i++) {
        if (code[i] != passcode[i]) {
          returnVal = FAILED;
          break;
        }
        code[i] = 0;
      }
      if (i == PASSCODE_SIZE) {
        returnVal = PASSED;
      }
      break;

    default:
      break;
  }

  return returnVal;
}
