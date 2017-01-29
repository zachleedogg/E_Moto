#define SET(p,n) ((p) |= (1 << (n)))
#define CLR(p,n) ((p) &= ~((1) << (n)))
#define ANALOG_TO_DIG(x) (14+x)
/*Switch Stuff*/
#define DEBUG_LED 13
#define STATUS_LED 8
#define DOOR_SW ANALOG_TO_DIG(5)

#define DOOR_SW_DELAY 1000
uint16_t doorSwitchDelayCounter = 1001;

#define DOOR_SW_ERROR 2000
#define DOOR_SW_ERROR_TIME 175
uint16_t doorSwitchErrorCounter = 3001;

#define SW_0 7
#define SW_1 2
#define SW_2 3
#define SW_3 4
#define SW_4 5
#define SW_5 ANALOG_TO_DIG(1)
#define SW_6 9
#define SW_7 10
#define SW_8 11
#define SW_9 12
#define SW_STAR ANALOG_TO_DIG(0)
#define SW_POUND 6
static uint16_t switchArray[] = {
  SW_0,
  SW_1,
  SW_2,
  SW_3,
  SW_4,
  SW_5,
  SW_6,
  SW_7,
  SW_8,
  SW_9,
  SW_STAR,
  SW_POUND,
};
const uint16_t numberOfSwitches = sizeof(switchArray) / 2;
static uint16_t switches = 0;

/*Password Stuff*/
static const uint8_t passcode[] = {1, 1, 2, 2, 3, 3};
#define PASSCODE_SIZE sizeof(passcode)
static uint8_t code[PASSCODE_SIZE + 1] = {};
static uint8_t codeIndex = 0;
#define NO_CHAR_INPUT 0xFF
#define CHAR_INPUT 0
#define FAILED 1
#define PASSED 2

uint32_t timeLast = 0;

typedef struct _TASKRUNNER_timer {
  uint8_t ones;
  uint8_t tens;
  uint8_t hunds;
  uint8_t secs;
} TASKRUNNER_timer;
static TASKRUNNER_timer timer = {};

static void FRAMEWORK_TASKRUNNER_run(void);

static inline void FRAMEWORK_TASKRUNNER_1ms(void);
static inline void FRAMEWORK_TASKRUNNER_10ms(void);
static inline void FRAMEWORK_TASKRUNNER_100ms(void);
static inline void FRAMEWORK_TASKRUNNER_1000ms(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.write("Welcome to the UART debug interface.\nSend any string and it will be echo'd back.\n");

  pinMode(DEBUG_LED, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(DOOR_SW, OUTPUT);
  digitalWrite(DOOR_SW, HIGH);

  for (int i = 0; i < numberOfSwitches; i++) {
    pinMode(switchArray[i], INPUT_PULLUP);
  }


}

void loop() {
  uint32_t timeNow = millis();
  if (timeNow - timeLast >= 1) {
    FRAMEWORK_TASKRUNNER_run();
  }
  timeLast = timeNow;
}


static inline void FRAMEWORK_TASKRUNNER_1ms(void) {

  //Check each button
  for (int i = 0; i < numberOfSwitches; i++) {
    if (digitalRead(switchArray[i]) == 0) {
      SET(switches, i);
    } else {
      CLR(switches, i);
    }
  }

  //Run the Password Handler
  uint8_t pw = passwordHandler();
  if (pw == PASSED) {
    Serial.write("PASSED!\n");
    digitalWrite(DOOR_SW, LOW);
    doorSwitchDelayCounter = 0;
  } else if (pw == FAILED) {
    Serial.write("Failed...\n");
    doorSwitchErrorCounter = 0;
  } else {
    //nothing
  }

  //Run the Door Switch Delay
  if (doorSwitchDelayCounter < DOOR_SW_DELAY) {
    if (++doorSwitchDelayCounter == DOOR_SW_DELAY) {
      digitalWrite(DOOR_SW, HIGH);
    }
  }

  if (doorSwitchErrorCounter < DOOR_SW_ERROR) {
    if ((++doorSwitchErrorCounter % DOOR_SW_ERROR_TIME) == 0) {
      digitalWrite(STATUS_LED, digitalRead(STATUS_LED) ? 0 : 1);
    }
    if (doorSwitchErrorCounter == DOOR_SW_ERROR) {
      digitalWrite(STATUS_LED, 0);
    }
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
  char mystr[15];
  sprintf(mystr, "Hello: %d\n", timer.secs);
  Serial.write(mystr);

  Serial.write("Switches: ");
  digitalWrite(DEBUG_LED, digitalRead(DEBUG_LED) ? 0 : 1);
  for (int i = 0; i < numberOfSwitches; i++) {
    Serial.write(digitalRead(switchArray[i]) + 48);
  }
  Serial.write("\n");
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

static uint8_t buttonHandler(void) {
  uint8_t returnVal = NO_CHAR_INPUT;
  static uint16_t lastSwitch;
  if (lastSwitch != switches) {
    //Turn on Status LED during any button Press.
    if (switches > 0) {
      digitalWrite(STATUS_LED, HIGH);
    } else {
      digitalWrite(STATUS_LED, LOW);
    }
    for (int i = 0; i < numberOfSwitches; i++) {
      if (((switches >> i) & 1) != ((lastSwitch >> i) & 1)) {
        if (((switches >> i) & 1) == 1) {
          returnVal = i;
        }
      }
    }
  }
  lastSwitch = switches;
  return returnVal;
}

static uint8_t passwordHandler(void) {
  uint16_t temp = buttonHandler();
  uint8_t returnVal = NO_CHAR_INPUT;
  uint8_t enter = 0;

  switch (temp) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      Serial.print(temp);
      returnVal = CHAR_INPUT;
      code[codeIndex++] = temp;
      if(codeIndex>=PASSCODE_SIZE){
        codeIndex--;
      }
      break;
    case 10://STAR
      code[0] = codeIndex = 0; /*make string empty*/
      returnVal = FAILED;
      break;
    case 11://POUND
      enter = 1;
      break;
    default://No input
      break;
  }


  if (enter) {
    returnVal = PASSED;
    codeIndex = 0;
    uint8_t i = 0;
    for (i = 0; i < PASSCODE_SIZE; i++) {
      Serial.print(code[i]);
      if (code[i] != passcode[i]) {
        returnVal = FAILED;
        break;
      }
      code[i] = 0;
    }
  }

  return returnVal;
}
