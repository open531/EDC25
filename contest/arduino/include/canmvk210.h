#ifndef CANMVK210_H
#define CANMVK210_H

#include <Arduino.h>

class CanMVK210 {
public:
  CanMVK210(HardwareSerial *serial, int baud); // 构造函数
private:
  HardwareSerial *_serial; // 串口
  int _baud;               // 波特率
};

#endif // CANMVK210_H
