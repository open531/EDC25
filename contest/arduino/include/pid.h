#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID {
public:
  PID(int enca, int encb, float kp, float ki, float kd);
  void init(void);
  int compute(float target, float current);
  void countA(void);
  void countB(void);

private:
  float _kp;
  float _ki;
  float _kd;
  float _serr;
  float _err;
  float _derr;
  int _count;
  int _enca;
  int _encb;
};

#endif // PID_H