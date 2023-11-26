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

void countA(PID &pid) {
  if (digitalRead(pid._enca) == digitalRead(pid._encb)) {
    pid._count += 1;
  } else {
    pid._count -= 1;
  }
}

void countB(PID &pid) {
  if (digitalRead(pid._enca) == digitalRead(pid._encb)) {
    pid._count -= 1;
  } else {
    pid._count += 1;
  }
}

int PID::getEnca() { return _enca; }
int PID::getEncb() { return _encb; }
int64_t PID::getCount() { return _count; }
void PID::resetCount() { _count = 0; }