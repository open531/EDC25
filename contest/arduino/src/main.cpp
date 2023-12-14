#include "jy62.h"
#include "pid.h"
#include "player.h"
#include "tb6612fng.h"
#include "zigbee.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <HardwareTimer.h>
#include <STM32FreeRTOS.h>
#include <stdint.h>

HardwareSerial SerialUART1(PB7, PB6);   // 和电脑通信串口
HardwareSerial SerialUART2(PA3, PA2);   // 和jy62通信串口
HardwareSerial SerialUART3(PB11, PB10); // 和zigbee通信串口

JY62 imu(&SerialUART2, 115200);
PID pid(PA6, PA7, 0.08, 0.07, 0.06);
TB6612FNG motor(PB3, PC12, PD2, PB4, PC10, PC11, PB5);
Zigbee zigbee(&SerialUART3, 115200);

Player player;

TaskHandle_t xIMUMessageRecordTask;
TaskHandle_t xIMUPrintDataTask;

long lastLoopTime = 0;

static void vIMUMessageRecordTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    if (SerialUART2.available()) {
      imu.messageRecord();
    }
  }
}

static void vIMUPrintDataTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    imu.printData(0, SerialUART1);
    vTaskDelay(configTICK_RATE_HZ);
  }
}

static void vZigbeeMessageRecordTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    zigbee.messageRecord();
    vTaskDelay(configTICK_RATE_HZ);
  }
}

static void vPlayerUpdateTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    player.updatePlayerInfo();
    vTaskDelay(configTICK_RATE_HZ);
  }
}

static void vPlayerPrintInfoTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    player.printPlayerInfo(SerialUART1);
    vTaskDelay(configTICK_RATE_HZ * 2);
  }
}

static void vPlayerChangeStatusTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    switch (player.getPlayerState()) {
    case IDLE:
      player.setPlayerState(COLLECTING);
      break;
    case COLLECTING:
      player.setPlayerState(ATTACKING);
      break;
    case ATTACKING:
      player.setPlayerState(FLEEING);
      break;
    case FLEEING:
      player.setPlayerState(IDLE);
      break;
    }
    vTaskDelay(configTICK_RATE_HZ / 2);
  }
}

void setup() {
  SerialUART1.begin(115200);
  imu.setBaud(115200);
  imu.setHorizontal();
  imu.initAngle();
  imu.calibrate();
  motor.init();
  motor.forward(DEFAULT_SPEED);
  player.setJY62(&imu);
  player.setPID(&pid);
  player.setTB6612FNG(&motor);
  player.setZigbee(&zigbee);
  xTaskCreate(vIMUMessageRecordTask, "IMUMessageRecordTask", 128, NULL,
              tskIDLE_PRIORITY, &xIMUMessageRecordTask);
  // xTaskCreate(vIMUPrintDataTask, "IMUPrintDataTask", 128, NULL,
  //             tskIDLE_PRIORITY, &xIMUPrintDataTask);
  xTaskCreate(vZigbeeMessageRecordTask, "ZigbeeMessageRecordTask", 256, NULL,
              tskIDLE_PRIORITY, NULL);
  xTaskCreate(vPlayerUpdateTask, "PlayerUpdateTask", 128, NULL,
              tskIDLE_PRIORITY, NULL);
  xTaskCreate(vPlayerPrintInfoTask, "PlayerPrintInfoTask", 128, NULL,
              tskIDLE_PRIORITY, NULL);

  vTaskStartScheduler();
}

void loop() {
  if (millis() - lastLoopTime > 1000) {
    SerialUART1.println("Hello, world!");
    lastLoopTime = millis();
  }
}
