#ifndef TB6612FNG_H
#define TB6612FNG_H

#include <Arduino.h>

class TB6612FNG {
public:
  TB6612FNG(int pwma, int ain1, int ain2, int pwmb, int bin1, int bin2,
            int stdby);
  void init(void);

  void forward(int speed);
  void backward(int speed);
  void turnLeft(int speed);
  void turnRight(int speed);
  void stop(void);

  void setPwmA(int pwma);
  void setAin1(int ain1);
  void setAin2(int ain2);
  void setPwmB(int pwmb);
  void setBin1(int bin1);
  void setBin2(int bin2);

  int getPwmA(void);
  int getAin1(void);
  int getAin2(void);
  int getPwmB(void);
  int getBin1(void);
  int getBin2(void);

private:
  int _pwma;
  int _ain1;
  int _ain2;
  int _pwmb;
  int _bin1;
  int _bin2;
  int _stdby;
};

#endif // TB6612FNG_H
