#ifndef JY62_H
#define JY62_H

#include <Arduino.h>
#include <HardwareSerial.h>

#define JY62_MESSAGE_LENTH 11
#define g 9.8 // 重力加速度

struct Acce { // 加速度
  float x;
  float y;
  float z;
};

struct Velo { // 角速度
  float x;
  float y;
  float z;
};

struct Angl { // 角度
  float roll;
  float pitch;
  float yaw;
};

struct Temp { // 温度
  float temp;
};

const uint8_t msgInitAngle[3] = {0xFF, 0xAA, 0x52};     // 角度初始化
const uint8_t msgCalibrateAcce[3] = {0xFF, 0xAA, 0x67}; // 加速度计校准
const uint8_t msgSetBaud115200[3] = {0xFF, 0xAA,
                                     0x63}; // 波特率115200，回传速率100Hz
const uint8_t msgSetBaud9600[3] = {0xFF, 0xAA,
                                   0x64}; // 波特率9600，回传速率20Hz
const uint8_t msgSetHorizontal[3] = {0xFF, 0xAA, 0x65}; // 水平安装
const uint8_t msgSetVertical[3] = {0xFF, 0xAA, 0x66};   // 垂直安装
const uint8_t msgSleepAndAwake[3] = {0xFF, 0xAA, 0x60}; // 休眠

class JY62 {
public:
  JY62(HardwareSerial *serial); // 构造函数
  void messageRecord(void);     // 读取串口数据

  void setBaud(int baud);   // 设置波特率，可选择115200或9600
  void setHorizontal(void); // 设置为水平放置模式
  void setVertical(void);   // 设置为垂直放置模式
  void initAngle(void);     // 初始化z轴角度为0
  void calibrate(void);     // 校准加速度计
  void sleepOrAwake(void);  // 设置休眠/唤醒

  Velo getVelo(void);   // 获取角速度
  float getVeloX(void); // 获取x轴角速度
  float getVeloY(void); // 获取y轴角速度
  float getVeloZ(void); // 获取z轴角速度
  Acce getAcce(void);   // 获取加速度
  float getAcceX(void); // 获取x轴加速度
  float getAcceY(void); // 获取y轴加速度
  float getAcceZ(void); // 获取z轴加速度
  Angl getAngl(void);   // 获取角度
  float getRoll(void);  // 获取绕x轴旋转角度（横滚角）
  float getPitch(void); // 获取绕y轴旋转角度（俯仰角）
  float getYaw(void);   // 获取绕z轴旋转角度（偏航角）
  float getTemp(void);  // 获取温度

  void decode(void); // 解码

protected:
  uint8_t receive[JY62_MESSAGE_LENTH]; // 实时记录收到的信息
  uint8_t message[JY62_MESSAGE_LENTH]; // 确认无误后用于解码的信息

private:
  HardwareSerial *_serial; // 串口
  Velo _velo;              // 角速度
  Acce _acce;              // 加速度
  Angl _angl;              // 角度
  Temp _temp;              // 温度

  void decodeVelo(void); // 解码角速度
  void decodeAcce(void); // 解码加速度
  void decodeAngl(void); // 解码角度
  void decodeTemp(void); // 解码温度
};

#endif // JY62_H
