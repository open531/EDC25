#include "jy62.h"

/**
 * @brief 构造函数
 *
 * @param serial 串口
 * @param baud 波特率
 */
JY62::JY62(HardwareSerial *serial, int baud) {
  _serial = serial;
  _baud = baud;
  _serial->begin(baud);
}

/**
 * @brief 读取串口数据
 *
 */
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

/**
 * @brief 设置波特率
 *
 * @param baud 波特率，可选择115200或9600
 */
void JY62::setBaud(int baud) {
  if (baud == 115200) {
    _serial->write(msgSetBaud115200, 3);
  } else if (baud == 9600) {
    _serial->write(msgSetBaud9600, 3);
  }
}

// @brief 设置为水平放置模式
void JY62::setHorizontal(void) { _serial->write(msgSetHorizontal, 3); }
// @brief 设置为垂直放置模式
void JY62::setVertical(void) { _serial->write(msgSetVertical, 3); }
// @brief 初始化z轴角度为0
void JY62::initAngle(void) { _serial->write(msgInitAngle, 3); }
// @brief 加速度计校准
void JY62::calibrate(void) { _serial->write(msgCalibrateAcce, 3); }
// @brief 设置休眠/唤醒
void JY62::sleepOrAwake(void) { _serial->write(msgSleepAndAwake, 3); }

// @brief 获取角速度
Velo JY62::getVelo(void) { return _velo; }
// @brief 获取x轴角速度
float JY62::getVeloX(void) { return _velo.x; }
// @brief 获取y轴角速度
float JY62::getVeloY(void) { return _velo.y; }
// @brief 获取z轴角速度
float JY62::getVeloZ(void) { return _velo.z; }
// @brief 获取加速度
Acce JY62::getAcce(void) { return _acce; }
// @brief 获取x轴加速度
float JY62::getAcceX(void) { return _acce.x; }
// @brief 获取y轴加速度
float JY62::getAcceY(void) { return _acce.y; }
// @brief 获取z轴加速度
float JY62::getAcceZ(void) { return _acce.z; }
// @brief 获取角度
Angl JY62::getAngl(void) { return _angl; }
// @brief 获取绕x轴旋转角度（横滚角）
float JY62::getRoll(void) { return _angl.roll; }
// @brief 获取绕y轴旋转角度（俯仰角）
float JY62::getPitch(void) { return _angl.pitch; }
// @brief 获取绕z轴旋转角度（偏航角）
float JY62::getYaw(void) { return _angl.yaw; }
// @brief 获取温度
float JY62::getTemp(void) { return _temp.temp; }

/**
 * @brief 打印数据
 *
 * @param type 1:加速度 2:角速度 3:角度 0:全部
 * @param serial 串口
 */
void JY62::printData(uint8_t type, HardwareSerial &serial) {
  serial.println("*****JY62 DATA BEGIN*****");
  serial.println("jy62 data:");
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
  serial.println("*****JY62 DATA END*****");
  serial.println();
}

/**
 * @brief 解码
 *
 */
void JY62::decode(void) {
  switch (message[1]) {
  case 0x51:
    decodeAcce();
    break;
  case 0x52:
    decodeVelo();
    break;
  case 0x53:
    decodeAngl();
    break;
  default:
    break;
  }
  decodeTemp();
}

/**
 * @brief 解码角速度
 *
 */
void JY62::decodeVelo(void) {
  _velo.x = (float)((int16_t)(message[3] << 8 | message[2])) / 32768.0 * 2000.0;
  _velo.y = (float)((int16_t)(message[5] << 8 | message[4])) / 32768.0 * 2000.0;
  _velo.z = (float)((int16_t)(message[7] << 8 | message[6])) / 32768.0 * 2000.0;
}

/**
 * @brief 解码加速度
 *
 */
void JY62::decodeAcce(void) {
  _acce.x = (float)((int16_t)(message[3] << 8 | message[2])) / 32768.0 * 16.0 *
            JY62_G;
  _acce.y = (float)((int16_t)(message[5] << 8 | message[4])) / 32768.0 * 16.0 *
            JY62_G;
  _acce.z = (float)((int16_t)(message[7] << 8 | message[6])) / 32768.0 * 16.0 *
            JY62_G;
}

/**
 * @brief 解码角度
 *
 */
void JY62::decodeAngl(void) {
  _angl.roll =
      (float)((int16_t)(message[3] << 8 | message[2])) / 32768.0 * 180.0;
  _angl.pitch =
      (float)((int16_t)(message[5] << 8 | message[4])) / 32768.0 * 180.0;
  _angl.yaw =
      (float)((int16_t)(message[7] << 8 | message[6])) / 32768.0 * 180.0;
}

/**
 * @brief 解码温度
 *
 */
void JY62::decodeTemp(void) {
  _temp.temp = (float)((int16_t)(message[9] << 8 | message[8])) / 340.0 + 36.53;
}

// @brief 获取串口
HardwareSerial *JY62::getSerial(void) { return _serial; }