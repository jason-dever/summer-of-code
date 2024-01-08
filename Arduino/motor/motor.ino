#define D5 5
#define DIRECTION_A 4
#define DIRECTION_B 3


void setup() {
  pinMode(D5, OUTPUT);
  pinMode(DIRECTION_A, OUTPUT);
  pinMode(DIRECTION_B, OUTPUT);
}

void loop() {
  // digitalWrite(D5, HIGH);
  analogWrite(D5, 255);

  digitalWrite(DIRECTION_A, HIGH);
  digitalWrite(DIRECTION_B, LOW);

  // delay(2000);

  // digitalWrite(DIRECTION_A, LOW);
  // digitalWrite(DIRECTION_B, HIGH);

  // delay(2000);
}
