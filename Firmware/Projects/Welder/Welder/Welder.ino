#define DEBUG 1
#define DEBUG_PRINT(x) Serial.print(x)

/*PINS*/
#define TIMER_ADJUST A5
#define ZERO_CROSS 2
#define STATUS_LED 4
#define WELDER_GATE 3
#define WELD_BUTTON 6
#define BUZZER 8
#define LED_1 9
#define LED_2 10
#define LED_3 11
#define LED_4 12
#define LED_5 13
#define LED_6 16 //A2
#define LED_7 17 //A3
#define LED_8 18 //A4
#define NUMBER_LEDS 8

uint8_t ledArray[8][2] = {{LED_1,0},{LED_2,0},{LED_3,0},{LED_4,0},{LED_5,0},{LED_6,0},{LED_7,0},{LED_8,0}};


/*Zero Crossing Variables*/
uint32_t currentTime = 0;
uint32_t lastTime = 0;
uint32_t lastPeriod = 0;
#define AVERAGE_WINDOW 20
uint32_t averageTime = 0;
uint32_t aveArr[AVERAGE_WINDOW] = {};
uint32_t avePtr = 0;
uint32_t sum = 0;
bool zeroCross = 0;
uint32_t count = 0;

/*State machine variables*/
typedef enum {
  STANDBY_1,
  CLEAN,
  STOP_CLEAN,
  FINISH_CLEAN,
  STANDBY_2,
  WELD
} welderStates_E;
welderStates_E welderState = STANDBY_1;
uint32_t weldStartDelay = 0;
uint32_t weldTimer = 0;
uint32_t weldPeriod = 0;
uint32_t weldOnce = 1;

/*Button debounce variables*/
#define DEBOUNCE_PERIOD 250
uint32_t debounceCount = 0;
bool buttonPressed = false;
bool readyToWeld = false;
uint32_t buttonCount = 0;

unsigned  secondTimer = 0;

void setup() {
  // put your setup code here, to run once:
#if DEBUG
  Serial.begin(115200);
  DEBUG_PRINT("hello, welcome to the welder\n\n");
#endif
  pinMode(ZERO_CROSS, INPUT_PULLUP);
  pinMode(WELD_BUTTON, INPUT_PULLUP);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(WELDER_GATE, OUTPUT);
  digitalWrite(STATUS_LED, LOW);
  digitalWrite(BUZZER, LOW);
  digitalWrite(WELDER_GATE, LOW);
  for(int i=0;i<NUMBER_LEDS;i++){
    pinMode(ledArray[i][0],OUTPUT);
    digitalWrite(ledArray[i][0], HIGH);
    delay(100);//DO NOT REMOVE this delay prevents weld at startup.
    digitalWrite(ledArray[i][0], LOW);
  }
  secondTimer = millis();
}

void loop() {
  //Detect Zero Crossings
  if (digitalRead(ZERO_CROSS)) {
    if (zeroCross == false) {//debounce the zero detector
      zeroCross = true;
      count++; //count cycles
      movingAverage(); //find average cycle length
    }
  } else {
    zeroCross = false;
  }

  //Run the Welder State Machine
  welderRun();

  /*Print zeroCross statement every Second*/
  int temp = millis();
  if (temp - secondTimer >= 1000) {
    secondTimer = temp;
    float Hz = (float)count / 2.0;
    count = 0;
    DEBUG_PRINT("Mains frquency is ");
    DEBUG_PRINT(Hz);
    DEBUG_PRINT("Hz\nAverage zero crossing period is ");
    DEBUG_PRINT(averageTime);
    DEBUG_PRINT(" microSeconds.\n");
    DEBUG_PRINT("Weld time selected: ");
    DEBUG_PRINT(weldPeriod);
    DEBUG_PRINT("\n\n");
  }
}

void welderRun() {
  switch (welderState) {
    case STANDBY_1:
      getButtonPress();
      getWeldPeriod();
      if (zeroCross && readyToWeld) {
        DEBUG_PRINT("Preparing to weld.......");
        readyToWeld = false;
        weldStartDelay = micros() + averageTime / 2; //delay timer to start on a Peak
        welderState = CLEAN;
      }
      break;
    case CLEAN:
      if (micros() >= weldStartDelay) { //When peak is detected, start the timer
        DEBUG_PRINT("WELDING.....");
        digitalWrite(STATUS_LED, HIGH);
        digitalWrite(BUZZER, HIGH);
        digitalWrite(WELDER_GATE, HIGH);
        weldTimer = micros() + weldPeriod * 1000;
        welderState = STOP_CLEAN;
      }
      break;
    case STOP_CLEAN:
      if (micros() >= weldTimer) { //When weld Time has expired, prepare to turn off on a ZeroCrossing
        if (zeroCross) {
          weldTimer = micros() + averageTime - 200;
          welderState = FINISH_CLEAN;
        }
      }
      break;
    case FINISH_CLEAN:
      if (micros() >= weldTimer) {
        DEBUG_PRINT("Done.\n\n");
        digitalWrite(STATUS_LED, LOW);
        digitalWrite(BUZZER, LOW);
        digitalWrite(WELDER_GATE, LOW);
        welderState = STANDBY_1;
      }
      break;
    case STANDBY_2:
      break;
    case WELD:
      break;
    default:
      break;
  }
}

void movingAverage(void) {
  lastTime = currentTime;
  currentTime = micros();
  lastPeriod = currentTime - lastTime;
  sum -= aveArr[avePtr];
  sum += lastPeriod;
  aveArr[avePtr++] = lastPeriod;
  if (avePtr == AVERAGE_WINDOW) {
    avePtr = 0;
  }
  averageTime = sum / AVERAGE_WINDOW;
}

void getWeldPeriod(void) {
  uint32_t potVal = analogRead(TIMER_ADJUST);
  weldPeriod = map(potVal, 0, 1023, 50, 1000);
}

void getButtonPress(void) {
  uint32_t value = digitalRead(WELD_BUTTON);
  if (buttonPressed == true) {
    if (value == 1) { //if button is being pressed
      if (++debounceCount == DEBOUNCE_PERIOD) {
        buttonPressed = false;
        debounceCount = 0;
      }
    } else {
      debounceCount = 0;
    }
  } else {
    if (value == 0) {
      if (++debounceCount == DEBOUNCE_PERIOD) {
        buttonPressed = true;
        buttonCount++;
        readyToWeld = true;
        debounceCount = 0;
      }
    } else {
      debounceCount = 0;
    }
  }
}

