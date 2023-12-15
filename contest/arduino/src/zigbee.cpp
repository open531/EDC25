#include "zigbee.h"

/**
 * @brief 构造函数
 *
 * @param serial 串口
 * @param baud 波特率
 */
Zigbee::Zigbee(HardwareSerial *serial, int baud) {
  _serial = serial;
  _baud = baud;
  _serial->begin(baud);
}

/**
 * @brief 读取串口数据
 *
 */
void Zigbee::messageRecord(void) {
  while (1) {
    if (_serial->available()) {
      if (_serial->read() == 0x55) {
        break;
      }
    }
  }
  receive[0] = 0x55;
  _serial->readBytes(receive + 1, 4);
  if (receive[1] != 0xAA) {
    return;
  }
  int16_t len = *(int16_t *)(receive + 2);
  _serial->readBytes(receive + 5, len);
  uint8_t sum = 0;
  for (int i = 5; i < len + 5; i++) {
    sum ^= receive[i];
  }
  if (sum == receive[4]) {
    memcpy(message, receive, len + 5);
  }
}

/**
 * @brief 发送数据
 *
 * @param msg 数据
 * @param len 数据长度
 */
void Zigbee::send(uint8_t *msg, uint8_t len) { _serial->write(msg, len); }

// @brief 获取串口
HardwareSerial *Zigbee::getSerial(void) { return _serial; }