#include "jy62.h"

JY62::JY62(HardwareSerial *serial) {
  _serial = serial;
  _serial->readBytes(receive, JY62_MESSAGE_LENTH);
}

void JY62::messageRecord(void) {
  if (receive[0] == 0x55) {
    uint8_t sum = 0x00;
    for (int i = 0; i < JY62_MESSAGE_LENTH - 1; i++) {
      sum += receive[i];
    }
    if (sum == receive[JY62_MESSAGE_LENTH - 1]) {
      for (int i = 0; i < JY62_MESSAGE_LENTH; i++) {
        message[i] = receive[i];
      }
      decode();
    }
  }
  _serial->readBytes(receive, JY62_MESSAGE_LENTH);
}

void JY62::setBaud(int baud) {
  if (baud == 115200) {
    _serial->write(msgSetBaud115200, 3);
  } else if (baud == 9600) {
    _serial->write(msgSetBaud9600, 3);
  }
}

void JY62::setHorizontal(void) { _serial->write(msgSetHorizontal, 3); }
void JY62::setVertical(void) { _serial->write(msgSetVertical, 3); }
void JY62::initAngle(void) { _serial->write(msgInitAngle, 3); }
void JY62::calibrate(void) { _serial->write(msgCalibrateAcce, 3); }
void JY62::sleepOrAwake(void) { _serial->write(msgSleepAndAwake, 3); }

Velo JY62::getVelo(void) { return _velo; }
float JY62::getVeloX(void) { return _velo.x; }
float JY62::getVeloY(void) { return _velo.y; }
float JY62::getVeloZ(void) { return _velo.z; }
Acce JY62::getAcce(void) { return _acce; }
float JY62::getAcceX(void) { return _acce.x; }
float JY62::getAcceY(void) { return _acce.y; }
float JY62::getAcceZ(void) { return _acce.z; }
Angl JY62::getAngl(void) { return _angl; }
float JY62::getRoll(void) { return _angl.roll; }
float JY62::getPitch(void) { return _angl.pitch; }
float JY62::getYaw(void) { return _angl.yaw; }
float JY62::getTemp(void) { return _temp.temp; }

void JY62::decode(void) {
  switch (message[0]) {
  case 0x51:
    decodeAcce();
    break;
  case 0x52:
    decodeVelo();
    break;
  case 0x53:
    decodeAngl();
    break;
  }
  decodeTemp();
}

void JY62::decodeVelo(void) {
  _velo.x = (float)((int16_t)(message[3] << 8 | message[2])) / 32768.0 * 2000.0;
  _velo.y = (float)((int16_t)(message[5] << 8 | message[4])) / 32768.0 * 2000.0;
  _velo.z = (float)((int16_t)(message[7] << 8 | message[6])) / 32768.0 * 2000.0;
}

void JY62::decodeAcce(void) {
  _acce.x =
      (float)((int16_t)(message[3] << 8 | message[2])) / 32768.0 * 16.0 * g;
  _acce.y =
      (float)((int16_t)(message[5] << 8 | message[4])) / 32768.0 * 16.0 * g;
  _acce.z =
      (float)((int16_t)(message[7] << 8 | message[6])) / 32768.0 * 16.0 * g;
}

void JY62::decodeAngl(void) {
  _angl.roll =
      (float)((int16_t)(message[3] << 8 | message[2])) / 32768.0 * 180.0;
  _angl.pitch =
      (float)((int16_t)(message[5] << 8 | message[4])) / 32768.0 * 180.0;
  _angl.yaw =
      (float)((int16_t)(message[7] << 8 | message[6])) / 32768.0 * 180.0;
}

void JY62::decodeTemp(void) {
  _temp.temp = (float)((int16_t)(message[9] << 8 | message[8])) / 340.0 + 36.53;
}
