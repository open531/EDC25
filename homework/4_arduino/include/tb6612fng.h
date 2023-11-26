#ifndef TB6612FNG_H
#define TB6612FNG_H

#include <Arduino.h>

class TB6612FNG {
public:
  TB6612FNG(int pwma, int ain1, int ain2, int pwmb, int bin1, int bin2,
<<<<<<< HEAD
            int stdby);
=======
            int stby);
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4
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
<<<<<<< HEAD
=======
  void setStby(int stby);
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4

  int getPwmA(void);
  int getAin1(void);
  int getAin2(void);
  int getPwmB(void);
  int getBin1(void);
  int getBin2(void);
<<<<<<< HEAD
=======
  int getStby(void);
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4

private:
  int _pwma;
  int _ain1;
  int _ain2;
  int _pwmb;
  int _bin1;
  int _bin2;
<<<<<<< HEAD
  int _stdby;
=======
  int _stby;
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4
};

#endif // TB6612FNG_H
