#include "jy62.h"
#include "pid.h"
#include "tb6612fng.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <HardwareTimer.h>

HardwareSerial SerialUART1(PB7, PB6); // 和电脑通信串口
HardwareSerial SerialUART2(PA3, PA2); // 和jy62通信串口

// TB6612FNG motor(3, 4, 5, 6, 7, 8);
JY62 imu(&SerialUART2, 115200);
// PID pid(9, 10, 0.1, 0.1, 0.1);

long long lastUpdateTime = 0;

void setup() {
  SerialUART1.begin(115200);
  imu.setBaud(115200);
  imu.setHorizontal();
  imu.initAngle();
  imu.calibrate();
}

// JY62类测试loop
void loop() {
  if (SerialUART2.available()) {
    imu.messageRecord();
    if (millis() - lastUpdateTime > 1000) {
      imu.printData(0, SerialUART1);
      lastUpdateTime = millis();
    }
  }
}
