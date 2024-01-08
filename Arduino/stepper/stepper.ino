#include <Stepper.h>

#define D5 5
#define D7 7
#define D6 6
#define D8 8

static int steps_per_rev = 2048;
static int rpm = 18;

Stepper motor(steps_per_rev, D5, D7, D6, D8);

void setup() {
  motor.setSpeed(rpm);
}

void loop() {
  motor.step(steps_per_rev);
  delay(1000);
  motor.step(-steps_per_rev);
  delay(1000);
}
