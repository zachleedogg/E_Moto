#define LED_1 A1
#define LED_2 A2

#define SW_1 A3

#define HEATER 9

typedef enum{
  OFF,
  WARM,
  HOT,
  NUMBER_OF_STATES
} heaterState_E;

static int heaterState = OFF;

static int tog;

void setup() {
  // put your setup code here, to run once:
  heaterState = OFF;
  Serial.begin(9600);
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(HEATER, OUTPUT);
  pinMode(SW_1, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(switchIsPressed()){
    runMachine();
  }
  
  if(heaterState == WARM){
    if(millis()%500 == 0){
      if(tog){
        tog = 0;
      } else {
        tog = 1;
      }
      digitalWrite(HEATER, tog);
    }
  }
  
}

void runMachine(void){
  if(++heaterState == NUMBER_OF_STATES){
    heaterState = OFF;
  }
  switch(heaterState){
    case OFF:
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
      digitalWrite(HEATER, LOW);
      Serial.print("OFF\n");
      break;
    case WARM:
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, LOW);
      analogWrite(HEATER, 150);
      Serial.print("WARM\n");
      break;
    case HOT:
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, HIGH);
      digitalWrite(HEATER, HIGH);
      Serial.print("HOT\n");
      break;
    default:
      break;
  }
}
        
bool switchIsPressed(void){
  static int state = 0;
  static int debouncer = 0;
  bool returnVal = false;
  switch(state){
    case 0:
      if(digitalRead(SW_1) == 0){
        debouncer++;
      } else {
        debouncer = 0;
      }
      if(debouncer == 100){
        debouncer = 0;
        state = 1;
        returnVal = true;
      }
      break;
    case 1:
      if(digitalRead(SW_1)){
        debouncer++;
      } else {
        debouncer = 0;
      }
      if(debouncer == 100){
        debouncer = 0;
        state = 0;
      }
      break;
      default:
      break;
  }
  return returnVal;
}
