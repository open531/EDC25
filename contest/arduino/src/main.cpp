#include "jy62.h"
#include "pid.h"
#include "tb6612fng.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <HardwareTimer.h>

HardwareSerial SerialUART1(PA10, PA9);

TB6612FNG motor(3, 4, 5, 6, 7, 8);
JY62 imu(&SerialUART1);
PID pid(9, 10, 0.1, 0.1, 0.1);

void setup() {}

void loop() {}
