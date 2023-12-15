#ifndef CANMVK210_H
#define CANMVK210_H

#include <Arduino.h>

#define CANMVK210_MSG_LEN 50

class CanMVK210 {
public:
  CanMVK210(HardwareSerial *serial, int baud); // 构造函数
  void messageRecord(void);                    // 读取串口数据

  uint8_t receive[CANMVK210_MSG_LEN]; // 实时记录收到的信息
  uint8_t message[CANMVK210_MSG_LEN]; // 确认无误后用于解码的信息

  HardwareSerial *getSerial(void); // 获取串口

private:
  HardwareSerial *_serial; // 串口
  int _baud;               // 波特率
};

#endif // CANMVK210_H
