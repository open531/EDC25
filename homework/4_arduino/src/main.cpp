#include "jy62.h"
#include "pid.h"
#include "tb6612fng.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <HardwareTimer.h>

// HardwareSerial SerialUART1(PB7, PB6); // 和电脑通信串口
// HardwareSerial SerialUART2(PA3, PA2); // 和jy62通信串口

TB6612FNG motor(PB3, PC12, PD2, PB4, PC10, PC11, PB5);
// JY62 imu(&SerialUART2, 115200);
// PID pid(9, 10, 0.1, 0.1, 0.1);

void setup() {
  motor.init();
  motor.turnLeft(128);
}

void loop() {}
