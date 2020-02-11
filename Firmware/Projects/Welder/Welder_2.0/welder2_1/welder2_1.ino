/*///////////////////////////////////////////////////////////////////////////////////////
////                              DEFINES                                            ////
///////////////////////////////////////////////////////////////////////////////////////*/
#define LED 13

#define WELD_BUTTON 12
#define MODE_BUTTON 10

#define RELAY 11

int weldTime = 20;

long lastTime = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(WELD_BUTTON, INPUT_PULLUP);
  pinMode(MODE_BUTTON, INPUT_PULLUP);

  delay(100);

  digitalWrite(RELAY, LOW);
  pinMode(RELAY, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(WELD_BUTTON) == LOW){
    digitalWrite(RELAY, HIGH);
    delay(weldTime);
    digitalWrite(RELAY, LOW);
    delay(1000);
  }

  if(digitalRead(MODE_BUTTON) == LOW){
    weldTime += 20;
    if(weldTime >= 100){
      weldTime = 20;
    }
    Serial.print("mode: ");
    Serial.println(weldTime);
    delay(250);
  }

  long timeNow = millis();
  if(timeNow-lastTime > 2*weldTime){
    Serial.print(timeNow);
    Serial.print("    ");
    Serial.println(lastTime);
    digitalWrite(LED, digitalRead(LED) ? 0 : 1);
    lastTime = timeNow;
  }
}
