#include <Arduino.h>

void setup()
{
  pinMode(PD2, OUTPUT);
}

void loop()
{
  digitalWrite(PD2, HIGH);
  delay(500);
  digitalWrite(PD2, LOW);
  delay(500);
}
