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

CanMVK210 camera(&SerialUART1, 115200);
JY62 imu(&SerialUART2, 115200);
TB6612FNG motor(PB3, PC12, PD2, PB4, PC10, PC11, PB5);
Zigbee zigbee(&SerialUART3, 115200);

Player player;

TaskHandle_t xIMUMessageRecordTask;
TaskHandle_t xIMUPrintDataTask;

long lastLoopTime = 0;
boolean setHome = false;

boolean runStatus = false;

static void vCameraMessageRecordTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    if (camera.getSerial()->available()) {
      camera.messageRecord();
    }
  }
}

static void vIMUMessageRecordTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    if (imu.getSerial()->available()) {
      imu.messageRecord();
    }
  }
}

// static void vIMUPrintDataTask(void *pvParameters) {
//   UNUSED(pvParameters);
//   while (1) {
//     imu.printData(0, SerialUART1);
//     vTaskDelay(configTICK_RATE_HZ);
//   }
// }

static void vZigbeeMessageRecordTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    if (zigbee.getSerial()->available()) {
      zigbee.messageRecord();
    }
  }
}

static void vPlayerUpdatePlayerInfoTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    player.updatePlayerInfo();
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}

// static void vPlayerPrintPlayerInfoTask(void *pvParameters) {
//   UNUSED(pvParameters);
//   while (1) {
//     player.printPlayerInfo(SerialUART1);
//     vTaskDelay(configTICK_RATE_HZ * 2);
//   }
// }

static void vPlayerUpdateMapInfoTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    player.updateMapInfo();
    vTaskDelay(configTICK_RATE_HZ);
  }
}

// static void vPlayerPrintMapInfoTask(void *pvParameters) {
//   UNUSED(pvParameters);
//   while (1) {
//     player.printMapInfo(SerialUART1);
//     vTaskDelay(configTICK_RATE_HZ * 2);
//   }
// }

static void vPlayerUpdateMineInfoTask(void *pvParameters) {
  UNUSED(pvParameters);
  while (1) {
    for (int i = 0; i < player.getMapInfo().goldMine.size(); i++) {
      if (player.getMapInfo().goldMine[i] == player.getPlayerInfo().position ||
          player.getMapInfo().goldMine[i] ==
              player.getPlayerInfo().positionOpponent) {
        player.getLastVisitedGoldMine()[i] =
            player.getPlayerInfo().elapsedTicks;
      }
    }
    for (int i = 0; i < player.getMapInfo().diamondMine.size(); i++) {
      if (player.getMapInfo().diamondMine[i] ==
              player.getPlayerInfo().position ||
          player.getMapInfo().diamondMine[i] ==
              player.getPlayerInfo().positionOpponent) {
        player.getLastVisitedDiamondMine()[i] =
            player.getPlayerInfo().elapsedTicks;
      }
    }
  }
}

// static void vPlayerChangeStatusTask(void *pvParameters) {
//   UNUSED(pvParameters);
//   while (1) {
//     if (player.getPlayerInfo().gameStage == RUNNING ||
//         player.getPlayerInfo().gameStage == BATTLING) {
//       switch (player.getPlayerState()) {
//       case IDLE:
//         if (player.getPlayerInfo().health < 5 ||
//             player.getHomeHeight() < SAFE_HOME_HEIGHT) {
//           player.setPlayerState(FLEEING);
//         } else if (player.calculateDistance(
//                        player.getPlayerInfo().position,
//                        player.getPlayerInfo().positionOpponent) < 8) {
//           player.setPlayerState(ATTACKING);
//         } else if (player.getPlayerInfo().emeraldCount < 5) {
//           player.setPlayerState(COLLECTING);
//         }
//         break;
//       case COLLECTING:
//         if (player.getPlayerInfo().emeraldCount >=
//             player.getDesiredEmeraldCount()) {
//           player.setPlayerState(IDLE);
//         }
//         break;
//       case ATTACKING:
//         if (player.getPlayerInfo().health < 5) {
//           player.setPlayerState(FLEEING);
//         } else if (player.calculateDistance(
//                        player.getPlayerInfo().position,
//                        player.getPlayerInfo().positionOpponent) >= 8) {
//           player.setPlayerState(IDLE);
//         } else if (player.getPlayerInfo().elapsedTicks -
//                        player.getLastAttackTicks() <
//                    (player.getAttackCooldown() - 0.5) * 1000) {
//           player.setPlayerState(FLEEING);
//         }
//         break;
//       case FLEEING:
//         if ((int8_t)player.getPlayerInfo().position.x == player.getHome().x
//         &&
//             (int8_t)player.getPlayerInfo().position.y == player.getHome().y)
//             {
//           player.setPlayerState(IDLE);
//         }
//         break;
//       }
//     }
//     vTaskDelay(configTICK_RATE_HZ / 2);
//   }
// }

void setup() {
  SerialUART1.begin(115200);
  imu.setBaud(115200);
  imu.setHorizontal();
  imu.initAngle();
  imu.calibrate();
  motor.init();
  player.setCanMVK210(&camera);
  player.setJY62(&imu);
  player.setTB6612FNG(&motor);
  player.setZigbee(&zigbee);
  xTaskCreate(vCameraMessageRecordTask, "CameraMessageRecordTask", 128, NULL,
              tskIDLE_PRIORITY, NULL);
  xTaskCreate(vIMUMessageRecordTask, "IMUMessageRecordTask", 128, NULL,
              tskIDLE_PRIORITY, &xIMUMessageRecordTask);
  // xTaskCreate(vIMUPrintDataTask, "IMUPrintDataTask", 128, NULL,
  //             tskIDLE_PRIORITY, &xIMUPrintDataTask);
  xTaskCreate(vZigbeeMessageRecordTask, "ZigbeeMessageRecordTask", 256, NULL,
              tskIDLE_PRIORITY, NULL);
  xTaskCreate(vPlayerUpdatePlayerInfoTask, "PlayerUpdatePlayerInfoTask", 128,
              NULL, tskIDLE_PRIORITY, NULL);
  // xTaskCreate(vPlayerPrintPlayerInfoTask, "PlayerPrintPlayerInfoTask", 128,
  //             NULL, tskIDLE_PRIORITY, NULL);
  xTaskCreate(vPlayerUpdateMapInfoTask, "PlayerUpdateMapInfoTask", 128, NULL,
              tskIDLE_PRIORITY, NULL);
  // xTaskCreate(vPlayerPrintMapInfoTask, "PlayerPrintMapInfoTask", 128, NULL,
  //             tskIDLE_PRIORITY, NULL);
  xTaskCreate(vPlayerUpdateMineInfoTask, "PlayerUpdateMineInfoTask", 128, NULL,
              tskIDLE_PRIORITY, NULL);
  vTaskStartScheduler();
}

void loop() {
  if ((player.getPlayerInfo().gameStage == RUNNING ||
       player.getPlayerInfo().gameStage == BATTLING) &&
      player.getPlayerInfo().health == 0) {
    player.reborn();
  } // 若游戏进行阶段死亡则复活
  if ((player.getPlayerInfo().gameStage == RUNNING ||
       player.getPlayerInfo().gameStage == BATTLING) &&
      !setHome) {
    player.setHome(player.getPlayerInfo().position);
    setHome = true;
  } // 初始化床的位置
  if (player.isNear(player.getPlayerInfo().position,
                    player.getPlayerInfo().positionOpponent)) {
    player.attack(player.getPlayerInfo().positionOpponent);
  } // 若在攻击范围内则攻击
  if (player.isNear(player.getPlayerInfo().position, player.getHome()) &&
      player.getHomeHeight() < player.getSafeHomeHeight()) {
    player.placeBlock(player.getHome());
  } // 若家的高度低于设定的安全高度则回家补充羊毛
  if ((player.getPlayerInfo().gameStage == RUNNING ||
       player.getPlayerInfo().gameStage == BATTLING) &&
      player.isHome()) {
    player.updateStrategy();
  } // 升级属性

  if (player.getHomeHeight() < player.getSafeHomeHeight() &&
      !player.isNear(player.getPlayerInfo().position, player.getHome())) {
    player.moveTo(player.getHome(), DEFAULT_SPEED);
  } else if ((player.getPlayerInfo().gameStage == RUNNING ||
              player.getPlayerInfo().gameStage == BATTLING) &&
             player.getPlayerInfo().emeraldCount >
                 player.getDesiredEmeraldCount()) {
    player.moveTo(player.getHome(), DEFAULT_SPEED);
  } else if (player.getPlayerInfo().gameStage == RUNNING ||
             player.getPlayerInfo().gameStage == BATTLING) {
    player.moveTo(player.findMineral(), DEFAULT_SPEED);
  }

  vTaskDelay(50);
}
