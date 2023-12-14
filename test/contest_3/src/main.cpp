#include <Arduino.h>

#define PIN_TEST PB5

void setup() {
  pinMode(PIN_TEST, OUTPUT);
  digitalWrite(PIN_TEST, HIGH);
}

void loop() {}