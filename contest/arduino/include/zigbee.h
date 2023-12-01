#ifndef ZIGBEE_H
#define ZIGBEE_H

#include <Arduino.h>
#include <HardwareSerial.h>

#define ZIGBEE_MSG_LEN 100

class Zigbee {
public:
  Zigbee(HardwareSerial *serial, int baud); // 构造函数
  void messageRecord(void);                 // 读取串口数据
  void send(uint8_t *msg, uint8_t len);     // 发送数据

  uint8_t receive[ZIGBEE_MSG_LEN]; // 实时记录收到的信息
  uint8_t message[ZIGBEE_MSG_LEN]; // 确认无误后用于解码的信息

private:
  HardwareSerial *_serial; // 串口
  int _baud;               // 波特率
};

#endif // ZIGBEE_H