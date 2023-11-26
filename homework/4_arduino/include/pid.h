#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID {
public:
  PID(int enca, int encb, float kp, float ki, float kd);
  void init(void);
  int compute(float target, float current);
<<<<<<< HEAD
  int getEnca();
  int getEncb();
  int64_t getCount();
  void resetCount();
  friend void countA(PID &);
  friend void countB(PID &);
=======
  void countA(void);
  void countB(void);
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4

private:
  float _kp;
  float _ki;
  float _kd;
  float _serr;
  float _err;
  float _derr;
<<<<<<< HEAD
  int64_t _count;
=======
  int _count;
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4
  int _enca;
  int _encb;
};

#endif // PID_H