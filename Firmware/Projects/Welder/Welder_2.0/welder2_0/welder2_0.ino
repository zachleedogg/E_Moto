/*///////////////////////////////////////////////////////////////////////////////////////
////                              DEFINES                                            ////
///////////////////////////////////////////////////////////////////////////////////////*/
#define LED 13

#define WELD_BUTTON 12
#define MODE_BUTTON 10

#define RELAY 11


/*///////////////////////////////////////////////////////////////////////////////////////
////                              VARIABLES                                          ////
///////////////////////////////////////////////////////////////////////////////////////*/
typedef struct _button {
  int buttonName = 0;
  bool buttonStatus = 0;
  int buttonDebounce = 0;
  int buttonDebouceNumber = 100;
} button;

button weldButton;

button modeButton;

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


/*///////////////////////////////////////////////////////////////////////////////////////
////                              FUNCTIONS                                          ////
///////////////////////////////////////////////////////////////////////////////////////*/

static void FRAMEWORK_TASKRUNNER_run(void);

static inline void FRAMEWORK_TASKRUNNER_1ms(void);
static inline void FRAMEWORK_TASKRUNNER_10ms(void);
static inline void FRAMEWORK_TASKRUNNER_100ms(void);
static inline void FRAMEWORK_TASKRUNNER_1000ms(void);

void runButton(button *thisButton);


void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(WELD_BUTTON, INPUT_PULLUP);
  pinMode(MODE_BUTTON, INPUT_PULLUP);

  pinMode(RELAY, OUTPUT);

  weldButton.buttonName = WELD_BUTTON;
  modeButton.buttonName = MODE_BUTTON;

}

static inline void FRAMEWORK_TASKRUNNER_1ms(void) {

}

static inline void FRAMEWORK_TASKRUNNER_10ms(void) {

}

static inline void FRAMEWORK_TASKRUNNER_100ms(void) {

}

static inline void FRAMEWORK_TASKRUNNER_1000ms(void) {
digitalWrite(LED, digitalRead(LED) ? 0 : 1);
}

void loop() {
  uint32_t timeNow = millis();
  if (timeNow - timeLast >= 1) {
    FRAMEWORK_TASKRUNNER_run();
  }
  timeLast = timeNow;
}

void runButton(button *thisButton){
  bool state = digitalRead(thisButton->buttonName);
  if(state ==  HIGH){
    if(thisButton->buttonStatus == HIGH){
      
    }

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
