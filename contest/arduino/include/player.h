#ifndef PLAYER_H
#define PLAYER_H

#include "canmvk210.h"
#include "zigbee.h"
#include <Arduino.h>
#include <queue>
#include <vector>

enum GameStage { READY, RUNNING, BATTLING, FINISHED };

struct Position {
  float_t x;
  float_t y;
};

struct Grid {
  int8_t x;
  int8_t y;
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

struct MapInfo {
  std::vector<Position> ironMine;    // 铁矿石
  std::vector<Position> goldMine;    // 金矿石
  std::vector<Position> diamondMine; // 钻石矿石
};

class Player {
public:
  Player(Zigbee *zigbee);                       // 构造函数
  Player(CanMVK210 *canmvk210, Zigbee *zigbee); // 构造函数

  void updatePlayerInfo(void); // 更新玩家信息
  void updateMapInfo(void);    // 更新地图信息

  void attack(uint8_t chunk);     // 攻击
  void placeBlock(uint8_t chunk); // 放置方块
  void trade(uint8_t item);       // 交易

  void printInfo(HardwareSerial &serial); // 打印玩家信息

  PlayerInfo getPlayerInfo(void); // 获取玩家信息
  MapInfo getMapInfo(void);       // 获取地图信息

private:
  PlayerInfo _playerInfo;   // 玩家信息
  MapInfo _mapInfo;         // 地图信息
  CanMVK210 *_canmvk210;    // CanMV K210
  Zigbee *_zigbee;          // Zigbee
  int32_t _lastUpdateTicks; // 上次更新的tick数
};

#endif // PLAYER_H