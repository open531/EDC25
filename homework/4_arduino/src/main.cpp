#include "jy62.h"
#include "pid.h"
#include "tb6612fng.h"
#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial SerialUART1(PB7, PB6); // 和电脑通信串口
HardwareSerial SerialUART2(PA3, PA2); // 和jy62通信串口
TB6612FNG motor(PB3, PC12, PD2, PB4, PC10, PC11, PB5);
PID pid(PA6, PA7, 0.08, 0.07, 0.06);
JY62 imu(&SerialUART2, 115200);
HardwareTimer TimerTIM1(TIM1);

uint64_t lastUpdateTime = 0;
float_t angle = 0;

void CountA(void) { countA(pid); }
void CountB(void) { countB(pid); }

void setup() {
  SerialUART1.begin(115200);
  imu.setBaud(115200);
  imu.setHorizontal();
  imu.initAngle();
  imu.calibrate();
  pid.init();
  motor.init();
  attachInterrupt(digitalPinToInterrupt(pid.getEnca()), CountA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pid.getEncb()), CountB, CHANGE);
  motor.forward(64);
  angle = imu.getYaw();
}

void loop() {
  if (SerialUART2.available()) {
    imu.messageRecord();
    if (millis() - lastUpdateTime > 1000) {
      imu.printData(0, SerialUART1);
      lastUpdateTime = millis();
      SerialUART1.println(pid.getCount());
    }
  }
  if (pid.getCount() >= 5000) {
    motor.turnLeft(64);
    while (!(
        (abs(imu.getYaw() - angle) > 89 && abs(imu.getYaw() - angle) < 91) ||
        (abs(imu.getYaw() - angle) > 269 && abs(imu.getYaw() - angle) < 271))) {
      if (SerialUART2.available()) {
        imu.messageRecord();
        if (millis() - lastUpdateTime > 1000) {
          imu.printData(0, SerialUART1);
          lastUpdateTime = millis();
          SerialUART1.println(pid.getCount());
          SerialUART1.println(abs(imu.getYaw() - angle));
        }
      }
    }
    motor.forward(64);
    pid.resetCount();
    angle = imu.getYaw();
  }
}
