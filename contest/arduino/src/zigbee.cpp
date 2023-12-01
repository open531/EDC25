#include "zigbee.h"

Zigbee::Zigbee(HardwareSerial *serial, int baud) {
  _serial = serial;
  _baud = baud;
  _serial->begin(baud);
}

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
    for (int i = 0; i < len + 5; i++) {
      message[i] = receive[i];
    }
  }
}

void Zigbee::send(uint8_t *msg, uint8_t len) { _serial->write(msg, len); }