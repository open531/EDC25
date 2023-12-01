#include "jy62.h"

JY62::JY62(HardwareSerial *serial, int baud) {
  _serial = serial;
  _baud = baud;
  _serial->begin(baud);
}

void JY62::messageRecord(void) {
  while (1) {
    if (_serial->available()) {
      if (_serial->read() == 0x55)
        break;
    }
  }
  receive[0] = 0x55;
  _serial->readBytes(receive + 1, JY62_MSG_LEN - 1);
  uint8_t sum = 0x55;
  for (int i = 1; i < JY62_MSG_LEN - 1; i++) {
    sum += receive[i];
  }
  if (sum == receive[JY62_MSG_LEN - 1]) {
    for (int i = 0; i < JY62_MSG_LEN; i++) {
      message[i] = receive[i];
    }
    decode();
  }
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
bool JY62::isFreshValid(void) { return _freshValid; }

// 0表示打印所有数据；1表示打印加速度；2表示打印角速度；3表示打印角度
void JY62::printData(uint8_t type, HardwareSerial &serial) {
  serial.println("*****************");
  serial.println("jy62 data:");
  serial.println("IsFreshValid:");
  serial.println(_freshValid);
  if (type == 1 || !type) {
    serial.print("Accelerate_x:");
    serial.println(_acce.x);
    serial.print("Accelerate_y:");
    serial.println(_acce.y);
    serial.print("Accelerate_z:");
    serial.println(_acce.z);
  }
  if (type == 2 || !type) {
    serial.print("Velocity_x:");
    serial.println(_velo.x);
    serial.print("Velocity_y:");
    serial.println(_velo.y);
    serial.print("Velocity_z:");
    serial.println(_velo.z);
  }
  if (type == 3 || !type) {
    serial.print("Angle_roll:");
    serial.println(_angl.roll);
    serial.print("Angle_pitch:");
    serial.println(_angl.pitch);
    serial.print("Angle_yaw:");
    serial.println(_angl.yaw);
  }
  serial.println("*****************");
}

void JY62::decode(void) {
  switch (message[1]) {
  case 0x51:
    decodeAcce();
    _freshValid = true;
    break;
  case 0x52:
    decodeVelo();
    _freshValid = true;
    break;
  case 0x53:
    decodeAngl();
    _freshValid = true;
    break;
  default:
    _freshValid = false;
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
