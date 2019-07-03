

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

uint8_t ledArray[8][2] = {{LED_8,0},{LED_7,0},{LED_6,0},{LED_5,0},{LED_4,0},{LED_3,0},{LED_2,0},{LED_1,0}};
uint8_t weldTimeAdjust = 1;

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

/*State machine variables*/
typedef enum {
  STANDBY_1,
  CLEAN,
  CLEANING,
  STOP_CLEAN,
  FINISH_CLEAN,
  STANDBY_2,
  WELD,
  WELDING,
  STOP_WELD,
} welderStates_E;
welderStates_E welderState = STANDBY_1;

/*Timer Variables*/
uint32_t weldTimer = 0;
bool weldTimerRunning = false;
bool weldTimerStatus = false;

/*Weld Variables*/
const uint32_t cleanPeriod = 50;
uint32_t weldPeriod = 50;
uint32_t weldOnce = 1;

/*Button debounce variables*/
typedef struct button{
  uint8_t pin;
  uint32_t debouncePeriod;
  uint32_t debounceCount;
  bool buttonState;
  uint32_t buttonCount;
  bool buttonRead;
} button;

button weldButton = {WELD_BUTTON,250,0,false,0,false};

button timeButton = {TIMER_ADJUST, 250,0,false,0,false};

bool buzzer = false;


unsigned  secondTimer = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.print("hello, welcome to the welder\n\n");

  pinMode(ZERO_CROSS, INPUT_PULLUP);
  pinMode(WELD_BUTTON, INPUT_PULLUP);
  pinMode(TIMER_ADJUST, INPUT_PULLUP);
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
  setLED(1);
  secondTimer = millis();
}

void loop() {
  //run button checkers
  runButton(&weldButton);
  runButton(&timeButton);

  //Detect Zero Crossings
  runZeroCrossing();

  //Run Timer
  runTimer();

  //Run the Welder State Machine
  welderRun();

  //Run Buzzer
  runBuzzer();
  
  /*Print zeroCross statement every Second*/
  runDebugPrint();
}

/********************************************************************
 * ***************************WELDER STATE MACHINE*******************
 * *****************************************************************/
void welderRun() {
  switch (welderState) {
    case STANDBY_1:
      if (getZeroCross() && getButtonState(&weldButton)) {
        Serial.print("Preparing to clean.......");
        startTimer(averageTime/2); //delay timer to start on a Peak
        setBuzzer(true); //quick buzz to acknowledge start
        welderState = CLEAN;
      }
      if(getButtonState(&timeButton)){
        weldTimeAdjust += 1;
        if(weldTimeAdjust >=9){
          weldTimeAdjust = 1;
        }
        setLED(weldTimeAdjust);
        
      }
      break;
    case CLEAN:
      if (getTimeUp()) { //Peak is 1/4 cycle after zero crossing
        setBuzzer(false);
        digitalWrite(STATUS_LED, HIGH);
        digitalWrite(WELDER_GATE, HIGH);
        Serial.print("CLEANING.....");
        startTimer(cleanPeriod * 1000); //convert to uS
        welderState = CLEANING;
      }
      break;
    case CLEANING:
      if(getTimeUp()){
        welderState = STOP_CLEAN;
      }
      break;
    case STOP_CLEAN:
      if (getZeroCross()) {
        Serial.print("Done.\n\n");
        digitalWrite(STATUS_LED, LOW);
        digitalWrite(WELDER_GATE, LOW);
        welderState = STANDBY_2;
        startTimer(1000000); //convert to uS
      }
      break;


    case STANDBY_2:
      if (getTimeUp()) {
        Serial.print("Preparing to weld.......");
        setBuzzer(true); //quick buzz to acknowledge start
        delay(500);
        setBuzzer(false);
        startTimer(averageTime/2); //delay timer to start on a Peak
        welderState = WELD;
      }
      break;
    case WELD:
      if (getTimeUp()) { //Peak is 1/4 cycle after zero crossing
        digitalWrite(STATUS_LED, HIGH);
        digitalWrite(WELDER_GATE, HIGH);
        Serial.print("WELDING.....");
        startTimer(weldPeriod*weldTimeAdjust*1000); //convert to uS
        welderState = WELDING;
      }
      break;
    case WELDING:
      if(getTimeUp()){
        welderState = STOP_WELD;
      }
      break;
    case STOP_WELD:
      if (getZeroCross()) {
        Serial.print("Done.\n\n");
        digitalWrite(STATUS_LED, LOW);
        digitalWrite(WELDER_GATE, LOW);
        welderState = STANDBY_1;
        startTimer(1000000); //convert to uS
      }
      break;
    default:
      break;
  }
}

/********************************************************************
 * ***************************ZERO CROSSING STUFF********************
 * *****************************************************************/
void runZeroCrossing(void){
  if (digitalRead(ZERO_CROSS)) {
    if (zeroCross == false) {//only detect on pin state transition
      zeroCross = true;
      movingAverage(); //find average cycle length
    }
  } else {
    zeroCross = false;
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

bool getZeroCross(void){
  return zeroCross;
}


/********************************************************************
 * ***************************BUTTON STUFF***************************
 * *****************************************************************/
void runButton(button * thisButton) {
  uint8_t value = digitalRead(thisButton->pin);
  if (thisButton->buttonState == true) { //if button state is "pressed"
    if (value == 1) { // and the button is being released
      if (thisButton->debounceCount++ >= thisButton->debouncePeriod) { //debounce and change state back to "not pressed"
        thisButton->buttonState = false;
        thisButton->buttonRead = false;
        thisButton->debounceCount = 0;
      }
    } else {
      thisButton->debounceCount = 0;
    }
  } else { //if state is "not pressed"
    if (value == 0) { // and the button is being pressed
      if (thisButton->debounceCount++ >= thisButton->debouncePeriod) { //debounce and change state to "pressed"
        thisButton->buttonState = true;
        thisButton->buttonRead = false;
        thisButton->debounceCount = 0;
      }
    } else {
      thisButton->debounceCount = 0;
    }
  }
}

bool getButtonState(button * thisButton){
  bool returnVal = false;
  if(thisButton->buttonRead == false && thisButton->buttonState == true){
    thisButton->buttonRead = true;
    returnVal = true;
  }
  return returnVal;
}


/********************************************************************
 * ***************************DEBUG STUFF****************************
 * *****************************************************************/
void runDebugPrint(void){
    int temp = millis();
  if (temp - secondTimer >= 1000) {
    secondTimer = temp;
    float Hz = 1.0/(float)(2*averageTime/1000000.0);
    Serial.print("Mains frquency is ");
    Serial.print(Hz);
    Serial.print("Hz\nAverage zero crossing period is ");
    Serial.print(averageTime);
    Serial.print(" microSeconds.\n");
    Serial.print("Weld time selected: ");
    Serial.print(weldPeriod*weldTimeAdjust);
    Serial.print("\nWeld Button Status: ");
    Serial.print(weldButton.buttonState);
    Serial.print("\nTime Button Status: ");
    Serial.print(timeButton.buttonState);
    
    Serial.print("\n\n");
  }
}

/********************************************************************
 * ***************************TIMER STUFF****************************
 * *****************************************************************/

void startTimer(uint32_t duration){
  weldTimerStatus = false;
  weldTimerRunning = true;
  weldTimer = micros() + duration; 

}

void runTimer(void){
  if(weldTimerRunning){
    if(micros() >= weldTimer){
      weldTimerStatus = true;
      weldTimerRunning = false;
    }
  }
}

bool getTimeUp(void){
  bool returnVal = false;
  if(weldTimerStatus == true){
    weldTimerStatus = false;
    returnVal = true;
  }
  return returnVal;
}


/********************************************************************
 * ***************************BUZZER STUFF***************************
 * *****************************************************************/
 void runBuzzer(void){
  if(buzzer == true){
    digitalWrite(BUZZER, !digitalRead(BUZZER));
  } else {
    digitalWrite(BUZZER,0);
  }
 }

 void setBuzzer(bool enable){
  buzzer = enable;
 }

 /********************************************************************
 * ***************************LED STUFF*******************************
 * *****************************************************************/
void setLED(uint8_t thisLED){
  int i = 0;
  for(i=0;i<8;i++){
    if(i == thisLED-1){
      digitalWrite(ledArray[i][0],ledArray[i][0]);
    } else{
      digitalWrite(ledArray[i][0],ledArray[i][1]);
    }
  }
}
