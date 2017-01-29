#include <Keypad.h>

extern "C" {
#include "garageDoorSM.h"
}

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

byte rowPins[ROWS] = {12,11,10,9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8,7,6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define DEBUG_LED 13
#define STATUS_LED 9

#define BACKLIGHT_DELAY 3000
static uint16_t backlightTimer = 0;

uint32_t timeLast = 0;
typedef struct _TASKRUNNER_timer {
  uint8_t ones;
  uint8_t tens;
  uint8_t hunds;
  uint8_t secs;
} TASKRUNNER_timer;
static TASKRUNNER_timer timer = {
  .ones = 0,
  .tens = 0,
  .hunds = 0,
  .secs = 0
};

static void FRAMEWORK_TASKRUNNER_run(void);

static inline void FRAMEWORK_TASKRUNNER_1ms(void);
static inline void FRAMEWORK_TASKRUNNER_10ms(void);
static inline void FRAMEWORK_TASKRUNNER_100ms(void);
static inline void FRAMEWORK_TASKRUNNER_1000ms(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.write("Welcome to the Garage Door Opener!\n\nThe Passward is 112233#\n\n");

  pinMode(DEBUG_LED, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  
  garageDoorInit();

}

static inline void FRAMEWORK_TASKRUNNER_1ms(void) {

  char key = keypad.getKey();
  if(key){
    Serial.print(key);
    Serial.print("\n");
    analogWrite(STATUS_LED,125);
    backlightTimer = 0;
  }
  if(backlightTimer++ >= BACKLIGHT_DELAY){
    analogWrite(STATUS_LED,0);
    backlightTimer = 0;
  }
  
  uint8_t state = garageDoorSM(key);
  if(state){
    Serial.print(garageDoorStateString[state]);
  }

  if (Serial.available()) {
    unsigned char temp = Serial.read();
    char tempstr[10];
    sprintf(tempstr, "rcvd: %x\n", temp);
    Serial.write(tempstr);
  }

}

static inline void FRAMEWORK_TASKRUNNER_10ms(void) {


}

static inline void FRAMEWORK_TASKRUNNER_100ms(void) {

}

static inline void FRAMEWORK_TASKRUNNER_1000ms(void) {
  //char mystr[15];
  //sprintf(mystr, "Hello: %d\n", timer.secs);
  //Serial.write(mystr);

  digitalWrite(DEBUG_LED, digitalRead(DEBUG_LED) ? 0 : 1);

}


void loop() {
  uint32_t timeNow = millis();
  if (timeNow - timeLast >= 1) {
    FRAMEWORK_TASKRUNNER_run();
  }
  timeLast = timeNow;
}

void FRAMEWORK_TASKRUNNER_run(void) {
  /*1ms tasks here*/
  timer.ones++;
  FRAMEWORK_TASKRUNNER_1ms();

  /*10ms tasks here*/
  if (timer.ones == 10) {
    timer.ones = 0;
    timer.tens++;
    FRAMEWORK_TASKRUNNER_10ms();

    /*100ms tasks here*/
    if (timer.tens == 10) {
      timer.tens = 0;
      timer.hunds++;
      FRAMEWORK_TASKRUNNER_100ms();

      /*1000ms tasks here*/
      if (timer.hunds == 10) {
        timer.hunds = 0;
        timer.secs++;
        FRAMEWORK_TASKRUNNER_1000ms();
      }
    }
  }
}




