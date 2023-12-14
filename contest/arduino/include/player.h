#ifndef PLAYER_H
#define PLAYER_H

#include "canmvk210.h"
#include "jy62.h"
#include "pid.h"
#include "tb6612fng.h"
#include "zigbee.h"
#include <Arduino.h>
#include <queue>
#include <vector>

#define DEFAULT_SPEED 255

enum GameStage { READY, RUNNING, BATTLING, FINISHED };

enum Item {
  AGILITY_BOOST,
  HEALTH_BOOST,
  STRENGTH_BOOST,
  WOOL,
  POTION_OF_HEALING
};

struct Position {
  float_t x;
  float_t y;
};

struct Grid {
  int8_t x;
  int8_t y;
  Grid() {}
  Grid(int8_t x, int8_t y) : x(x), y(y) {}
  Grid(const Grid &grid) : x(grid.x), y(grid.y) {}
  Grid(const Position &position)
      : x((int8_t)position.x), y((int8_t)position.y) {}
  Grid &operator=(const Grid &grid) {
    x = grid.x;
    y = grid.y;
    return *this;
  }
  Grid &operator=(const Position &position) {
    x = (int8_t)position.x;
    y = (int8_t)position.y;
    return *this;
  }
  Grid operator+(const Grid &grid) const {
    return Grid(x + grid.x, y + grid.y);
  }
  Grid operator-(const Grid &grid) const {
    return Grid(x - grid.x, y - grid.y);
  }
  bool operator==(const Grid &grid) const { return x == grid.x && y == grid.y; }
  bool operator==(const Position &position) const {
    return x == (int8_t)position.x && y == (int8_t)position.y;
  }
  bool operator!=(const Grid &grid) const { return x != grid.x || y != grid.y; }
  bool operator!=(const Position &position) const {
    return x != (int8_t)position.x || y != (int8_t)position.y;
  }
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

enum PlayerState { IDLE, COLLECTING, ATTACKING, FLEEING };

class Player {
public:
  Player(); // 构造函数

  void updatePlayerInfo(void); // 更新玩家信息
  void updateMapInfo(void);    // 更新地图信息

  void printPlayerInfo(HardwareSerial &serial); // 打印玩家信息
  void printMapInfo(HardwareSerial &serial);    // 打印地图信息

  void attack(uint8_t chunk);     // 攻击
  void placeBlock(uint8_t chunk); // 放置方块
  void trade(Item item);          // 交易

  std::vector<Grid> AStar(Grid src, Grid dst); // A*寻路
  std::vector<Grid> BFS(Grid src, Grid dst);   // BFS寻路

  double calculateAngle(Position src, Position dst); // 计算角度
  double calculateAngle(Position src, Grid dst);     // 计算角度

  void turnLeft(double angle, int speed);  // 左转
  void turnRight(double angle, int speed); // 右转
  void faceTo(Grid dst, int speed);        // 面向目标
  void moveTo(Grid dst, int speed);        // 移动到目标

  PlayerInfo getPlayerInfo(void);      // 获取玩家信息
  MapInfo getMapInfo(void);            // 获取地图信息
  PlayerState getPlayerState(void);    // 获取玩家状态
  int32_t getLastUpdateTicks(void);    // 获取上次更新的tick数
  int8_t getDesiredEmeraldCount(void); // 获取期望的绿宝石数量

  void setPlayerInfo(PlayerInfo playerInfo);        // 设置玩家信息
  void setMapInfo(MapInfo mapInfo);                 // 设置地图信息
  void setPlayerState(PlayerState playerState);     // 设置玩家状态
  void setCanMVK210(CanMVK210 *canmvk210);          // 设置CanMV K210
  void setJY62(JY62 *jy62);                         // 设置IMU
  void setPID(PID *pid);                            // 设置PID
  void setTB6612FNG(TB6612FNG *tb6612fng);          // 设置电机驱动
  void setZigbee(Zigbee *zigbee);                   // 设置Zigbee
  void setLastUpdateTicks(int32_t lastUpdateTicks); // 设置上次更新的tick数
  void
  setDesiredEmeraldCount(int8_t desiredEmeraldCount); // 设置期望的绿宝石数量

private:
  PlayerInfo _playerInfo;      // 玩家信息
  MapInfo _mapInfo;            // 地图信息
  PlayerState _playerState;    // 玩家状态
  double_t _directionFix;      // 方向修正
  CanMVK210 *_canmvk210;       // CanMV K210
  JY62 *_jy62;                 // IMU
  PID *_pid;                   // PID
  TB6612FNG *_tb6612fng;       // 电机驱动
  Zigbee *_zigbee;             // Zigbee
  int32_t _lastUpdateTicks;    // 上次更新的tick数
  int8_t _desiredEmeraldCount; // 期望的绿宝石数量
};

#endif // PLAYER_H