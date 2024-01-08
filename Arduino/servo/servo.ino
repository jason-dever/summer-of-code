#include <Servo.h>

#define D5 5

Servo servo;

void setup() {
  servo.attach(D5);
}

void loop() {
  servo.write(90);
  delay(500);
  servo.write(180);
  delay(500);
  servo.write(0);
  delay(500);
}