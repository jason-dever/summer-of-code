#define direction 12
#define pwm 3
#define brake 9

void setup() {
  pinMode(direction, OUTPUT);
  pinMode(pwm, OUTPUT);
  pinMode(brake, OUTPUT);
}

void loop() {
  analogWrite(pwm, 255);
  digitalWrite(brake, LOW);
  
  digitalWrite(direction, HIGH);
  // delay(3000);
  // digitalWrite(direction, LOW);
  // delay(3000);

  // digitalWrite(brake, HIGH);
  // delay(1000);
}
