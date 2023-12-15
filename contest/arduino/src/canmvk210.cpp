#include "canmvk210.h"

CanMVK210::CanMVK210(HardwareSerial *serial, int baud) {
  _serial = serial;
  _baud = baud;
}

void CanMVK210::messageRecord() {
  uint8_t head = 0;
  while (1) {
    if (_serial->available()) {
      head = _serial->read();
      if (head == 0x55 || head == 0x56) {
        break;
      }
    }
  }
  receive[0] = head;
  _serial->readBytes(receive + 1, 2);
  if (receive[1] != 0xAA) {
    return;
  }
  uint8_t len = receive[2];
  _serial->readBytes(receive + 3, len);
  uint8_t sum = 0;
  for (int i = 0; i < len + 3; i++) {
    sum += receive[i];
  }
  if (sum == receive[len + 3]) {
    memcpy(message, receive, len + 4);
  }
}

// @brief 获取串口
HardwareSerial *CanMVK210::getSerial(void) { return _serial; }