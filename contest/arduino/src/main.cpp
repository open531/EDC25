#include "jy62.h"
#include "pid.h"
#include "tb6612fng.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <HardwareTimer.h>
#include <STM32FreeRTOS.h>

HardwareSerial SerialUART1(PB7, PB6); // 和电脑通信串口
HardwareSerial SerialUART2(PA3, PA2); // 和jy62通信串口

// TB6612FNG motor(3, 4, 5, 6, 7, 8);
JY62 imu(&SerialUART2, 115200);
// PID pid(9, 10, 0.1, 0.1, 0.1);

TaskHandle_t xIMUMessageRecordTask;
TaskHandle_t xIMUPrintDataTask;

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

void setup() {
  SerialUART1.begin(115200);
  imu.setBaud(115200);
  imu.setHorizontal();
  imu.initAngle();
  imu.calibrate();
  xTaskCreate(vIMUMessageRecordTask, "IMUMessageRecordTask", 128, NULL,
              tskIDLE_PRIORITY + 2, &xIMUMessageRecordTask);
  xTaskCreate(vIMUPrintDataTask, "IMUPrintDataTask", 128, NULL,
              tskIDLE_PRIORITY + 2, &xIMUPrintDataTask);
  vTaskStartScheduler();
}

void loop() {}
