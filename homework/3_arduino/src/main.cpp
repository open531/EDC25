#include <Arduino.h>
#include "pwm.h"

void setup()
{
  SerialUART1.begin(115200);
  pinMode(ENA, OUTPUT);
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  attachInterrupt(ENC_A, Code0, CHANGE);
  attachInterrupt(ENC_B, Code1, CHANGE);
  TimerTIM1.setPrescaleFactor(1);
  TimerTIM1.setOverflow(50000);
  TimerTIM1.attachInterrupt(TimerIsr);
  TimerTIM1.resume();
}

void loop()
{
}
