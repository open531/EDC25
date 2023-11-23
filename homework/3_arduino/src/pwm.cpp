#include "pwm.h"

HardwareSerial SerialUART1(PB7, PB6);
HardwareTimer TimerTIM1(TIM1);

int count = 0;
float serr = 0, err = 0, derr = 0, dderr = 0;
float Kp = 0.08, Ki = 0.07, Kd = 0.06;
float rpm;
float goal = 9.23;
int pwm;

int PID(float goal, float now) {
  dderr = goal - now - err - derr;
  derr = goal - now - err;
  err = goal - now;
  serr += err;
  float PWM = Ki * (serr) + Kp * (err) + Kd * (derr);
  return PWM;
}

void Code0() {
  if (digitalRead(ENC_A) == LOW) {
    if (digitalRead(ENC_B) == LOW) {
      count += 1;
    }
    if (digitalRead(ENC_B) == HIGH) {
      count -= 1;
    }
  } else {
    if (digitalRead(ENC_B) == LOW) {
      count -= 1;
    }
    if (digitalRead(ENC_B) == HIGH) {
      count += 1;
    }
  }
}

void Code1() {
  if (digitalRead(ENC_B) == LOW) {
    if (digitalRead(ENC_A) == LOW) {
      count -= 1;
    }
    if (digitalRead(ENC_A) == HIGH) {
      count += 1;
    }
  } else {
    if (digitalRead(ENC_A) == LOW) {
      count += 1;
    }
    if (digitalRead(ENC_A) == HIGH) {
      count -= 1;
    }
  }
}

void TimerIsr() {
  rpm = count / 0.05 * 60 / 13 / 20;
  count = 0;
  pwm = PID(goal, rpm);
  if (abs(pwm) > 255) {
    if (pwm < 0) {
      pwm = -255;
    } else {
      pwm = 255;
    }
  }
  analogWrite(ENA, pwm);
  SerialUART1.print("rpm=");
  SerialUART1.println(rpm);
}
