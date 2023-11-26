#include "tb6612fng.h"

TB6612FNG::TB6612FNG(int pwma, int ain1, int ain2, int pwmb, int bin1, int bin2,
<<<<<<< HEAD
                     int stdby) {
=======
                     int stby) {
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4
  _pwma = pwma;
  _ain1 = ain1;
  _ain2 = ain2;
  _pwmb = pwmb;
  _bin1 = bin1;
  _bin2 = bin2;
<<<<<<< HEAD
  _stdby = stdby;
=======
  _stby = stby;
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4
}

void TB6612FNG::init(void) {
  pinMode(_pwma, OUTPUT);
  pinMode(_ain1, OUTPUT);
  pinMode(_ain2, OUTPUT);
  pinMode(_pwmb, OUTPUT);
  pinMode(_bin1, OUTPUT);
  pinMode(_bin2, OUTPUT);
<<<<<<< HEAD
  pinMode(_stdby, OUTPUT);
  digitalWrite(_stdby, HIGH);
}

void TB6612FNG::forward(int speed) {
=======
  pinMode(_stby, OUTPUT);
  digitalWrite(_stby, HIGH);
}

void TB6612FNG::forward(int speed) {
  digitalWrite(_stby, HIGH);
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4
  digitalWrite(_ain1, HIGH);
  digitalWrite(_ain2, LOW);
  analogWrite(_pwma, speed);
  digitalWrite(_bin1, HIGH);
  digitalWrite(_bin2, LOW);
  analogWrite(_pwmb, speed);
}

void TB6612FNG::backward(int speed) {
<<<<<<< HEAD
=======
  digitalWrite(_stby, HIGH);
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4
  digitalWrite(_ain1, LOW);
  digitalWrite(_ain2, HIGH);
  analogWrite(_pwma, speed);
  digitalWrite(_bin1, LOW);
  digitalWrite(_bin2, HIGH);
  analogWrite(_pwmb, speed);
}

void TB6612FNG::turnLeft(int speed) {
<<<<<<< HEAD
=======
  digitalWrite(_stby, HIGH);
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4
  digitalWrite(_ain1, HIGH);
  digitalWrite(_ain2, LOW);
  analogWrite(_pwma, speed);
  digitalWrite(_bin1, LOW);
  digitalWrite(_bin2, HIGH);
  analogWrite(_pwmb, speed);
}

void TB6612FNG::turnRight(int speed) {
<<<<<<< HEAD
=======
  digitalWrite(_stby, HIGH);
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4
  digitalWrite(_ain1, LOW);
  digitalWrite(_ain2, HIGH);
  analogWrite(_pwma, speed);
  digitalWrite(_bin1, HIGH);
  digitalWrite(_bin2, LOW);
  analogWrite(_pwmb, speed);
}

void TB6612FNG::stop(void) {
<<<<<<< HEAD
=======
  digitalWrite(_stby, LOW);
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4
  digitalWrite(_ain1, LOW);
  digitalWrite(_ain2, LOW);
  analogWrite(_pwma, 0);
  digitalWrite(_bin1, LOW);
  digitalWrite(_bin2, LOW);
  analogWrite(_pwmb, 0);
}

void TB6612FNG::setPwmA(int pwma) { _pwma = pwma; }
void TB6612FNG::setAin1(int ain1) { _ain1 = ain1; }
void TB6612FNG::setAin2(int ain2) { _ain2 = ain2; }
void TB6612FNG::setPwmB(int pwmb) { _pwmb = pwmb; }
void TB6612FNG::setBin1(int bin1) { _bin1 = bin1; }
void TB6612FNG::setBin2(int bin2) { _bin2 = bin2; }
<<<<<<< HEAD
=======
void TB6612FNG::setStby(int stby) { _stby = stby; }
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4

int TB6612FNG::getPwmA(void) { return _pwma; }
int TB6612FNG::getAin1(void) { return _ain1; }
int TB6612FNG::getAin2(void) { return _ain2; }
int TB6612FNG::getPwmB(void) { return _pwmb; }
int TB6612FNG::getBin1(void) { return _bin1; }
int TB6612FNG::getBin2(void) { return _bin2; }
<<<<<<< HEAD
=======
int TB6612FNG::getStby(void) { return _stby; }
>>>>>>> 4abe7484b916289e3985832f86ca5ab5c25f7de4
