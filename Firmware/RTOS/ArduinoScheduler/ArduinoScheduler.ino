uint32_t timeLast = 0;

typedef struct _TASKRUNNER_timer {
    uint8_t ones;
    uint8_t tens;
    uint8_t hunds;
    uint8_t secs;
} TASKRUNNER_timer;
static TASKRUNNER_timer timer = {};

static inline void FRAMEWORK_TASKRUNNER_1ms(void);
static inline void FRAMEWORK_TASKRUNNER_10ms(void);
static inline void FRAMEWORK_TASKRUNNER_100ms(void);
static inline void FRAMEWORK_TASKRUNNER_1000ms(void);

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
Serial.write("Welcome to the UART debug interface.\nSend any string and it will be echo'd back.\n");
}

void loop() {
  uint32_t timeNow = millis();
  if(timeNow-timeLast>=1){
    FRAMEWORK_TASKRUNNER_run();
  }
  timeLast = timeNow; 
}


static inline void FRAMEWORK_TASKRUNNER_1ms(void) {

  if(Serial.available()){
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
  sprintf(mystr,"Hello: %d\n", timer.secs);
  Serial.write(mystr);

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
