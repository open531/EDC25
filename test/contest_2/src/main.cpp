#include <Arduino.h>
#include <HardwareSerial.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

HardwareSerial MySerial1(PA10, PA9);
HardwareSerial MySerial2(PA3, PA2);

enum GameStage { READY, RUNNING, BATTLING, FINISHED };

struct Position {
  float_t x;
  float_t y;
};

struct PlayerInfo {
  GameStage gameStage;       // 游戏阶段
  int32_t elapsedTicks;      // 已经过的tick数
  int8_t heightOfChunks[64]; // 64个区块的高度
  bool hasBed;               // 是否有床
  bool hasBedOpponent;       // 对手是否有床
  Position position;         // 位置
  Position positionOpponent; // 对手位置
  int8_t agility;            // 敏捷度
  int8_t health;             // 生命值
  int8_t maxHealth;          // 最大生命值
  int8_t strength;           // 力量
  int8_t emeraldCount;       // 绿宝石数量
  int8_t woolCount;          // 羊毛数量
};

PlayerInfo playerInfo;

uint8_t buffer[100] = {0};

void randomPlayerInfo() {
  playerInfo.gameStage = (GameStage)(rand() % 4);
  playerInfo.elapsedTicks = rand();
  for (int i = 0; i < 64; i++) {
    playerInfo.heightOfChunks[i] = rand();
  }
  playerInfo.hasBed = rand() % 2;
  playerInfo.hasBedOpponent = rand() % 2;
  playerInfo.position.x = rand();
  playerInfo.position.y = rand();
  playerInfo.positionOpponent.x = rand();
  playerInfo.positionOpponent.y = rand();
  playerInfo.agility = rand();
  playerInfo.health = rand();
  playerInfo.maxHealth = rand();
  playerInfo.strength = rand();
  playerInfo.emeraldCount = rand();
  playerInfo.woolCount = rand();
}

void generateBuffer() {
  buffer[5] = playerInfo.gameStage;
  memcpy(buffer + 6, &playerInfo.elapsedTicks, sizeof(int32_t));
  memcpy(buffer + 10, playerInfo.heightOfChunks, sizeof(int8_t) * 64);
  buffer[74] = playerInfo.hasBed;
  buffer[75] = playerInfo.hasBedOpponent;
  memcpy(buffer + 76, &playerInfo.position, sizeof(Position));
  memcpy(buffer + 84, &playerInfo.positionOpponent, sizeof(Position));
  buffer[92] = playerInfo.agility;
  buffer[93] = playerInfo.health;
  buffer[94] = playerInfo.maxHealth;
  buffer[95] = playerInfo.strength;
  buffer[96] = playerInfo.emeraldCount;
  buffer[97] = playerInfo.woolCount;
  buffer[0] = 0x55;
  buffer[1] = 0xAA;
  int16_t N = 93;
  memcpy(buffer + 2, &N, sizeof(int16_t));
  uint8_t checksum = 0;
  for (int i = 5; i < 98; i++) {
    checksum ^= buffer[i];
  }
  buffer[4] = checksum;
}

void setup() {
  MySerial1.begin(115200);
  MySerial2.begin(115200);
}

void loop() {
  MySerial1.println("Hello");
  delay(50);
}