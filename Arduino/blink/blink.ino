#define D2 2
#define LED 13
#define EPSILON 250

volatile static bool led_state = LOW;
volatile unsigned long prev = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(D2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D2), IRSwapLEDState, FALLING);

  digitalWrite(LED, led_state);
}

void loop() {
  digitalWrite(LED, led_state);
  // digitalWrite(LED, digitalRead(D2));
}

void swapLEDState() {
  unsigned long now = millis();
  
  if (now-prev > EPSILON) {
    led_state = !led_state;
    prev = now;
  }
}

void IRSwapLEDState() {
  led_state = !led_state;
}