#include "pid.h"

PID::PID(int enca, int encb, float kp, float ki, float kd) {
  _kp = kp;
  _ki = ki;
  _kd = kd;
  _serr = 0;
  _err = 0;
  _derr = 0;
  _count = 0;
  _enca = enca;
  _encb = encb;
}

void PID::init() {
  pinMode(_enca, INPUT);
  pinMode(_encb, INPUT);
}

int PID::compute(float target, float current) {
  _derr = target - current - _err;
  _err = target - current;
  _serr += _err;
  int output = _kp * _err + _ki * _serr + _kd * _derr;
  if (output > 255) {
    output = 255;
  } else if (output < -255) {
    output = -255;
  }
  return output;
}

void PID::countA() {
  if (digitalRead(_enca) == digitalRead(_encb)) {
    _count += 1;
  } else {
    _count -= 1;
  }
}

void PID::countB() {
  if (digitalRead(_enca) == digitalRead(_encb)) {
    _count -= 1;
  } else {
    _count += 1;
  }
}
