#define DIRECTION 12
#define PWM 3
#define BRAKE 9

#define BALL_TOP 1 << PCINT16
#define ELEVATOR_BOTTOM 1 << PCINT17
#define ELEVATOR_TOP 1 << PCINT18
#define BALL_BOTTOM 1 << PCINT20

#define DOWN HIGH
#define UP LOW

volatile int brake_state = HIGH;
volatile int direction = UP;

void setup() {
  pinMode(DIRECTION, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(BRAKE, OUTPUT);

  pinMode(PCINT16, INPUT_PULLUP);
  pinMode(PCINT17, INPUT_PULLUP);
  pinMode(PCINT18, INPUT_PULLUP);
  pinMode(PCINT20, INPUT_PULLUP);

  while (digitalRead(PCINT17) == HIGH) {
    digitalWrite(PWM, HIGH);
    digitalWrite(BRAKE, LOW);
    digitalWrite(DIRECTION, DOWN);
  }

  PCICR |= (1 << PCIE2);
  PCMSK2 |= BALL_TOP | BALL_BOTTOM | ELEVATOR_TOP | ELEVATOR_BOTTOM;
}

void loop() {
  digitalWrite(PWM, HIGH);

  digitalWrite(BRAKE, brake_state);
  digitalWrite(DIRECTION, direction);
}

ISR (PCINT2_vect) {
  static bool top_elevator_previous = HIGH;
  static bool bottom_elevator_previous = LOW;
  
  static char pind_previous = 0x0;

  char pin_changed = PIND ^ pind_previous;
  pind_previous = PIND;

  switch (pin_changed) {
    case (BALL_TOP):
      brake_state = LOW;
      direction = DOWN;
      break;

    case BALL_BOTTOM:
      brake_state = LOW;
      direction = UP;
      break;

    case ELEVATOR_TOP:
      if (top_elevator_previous == HIGH)
        brake_state = HIGH;

      top_elevator_previous = !top_elevator_previous;
      break;

    case ELEVATOR_BOTTOM:
      if (bottom_elevator_previous == HIGH)
        brake_state = HIGH;

      bottom_elevator_previous = !bottom_elevator_previous;
      break;

    default:
      //error! an unexpected pin changed
      break;
  }
}