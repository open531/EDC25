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

enum GameStage { READY, RUNNING, BATTLING, FINISHED };

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
  bool operator!=(const Grid &grid) const { return x != grid.x || y != grid.y; }
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
  Player(); // 构造函数

  void updatePlayerInfo(void); // 更新玩家信息
  void updateMapInfo(void);    // 更新地图信息

  void printPlayerInfo(HardwareSerial &serial); // 打印玩家信息
  void printMapInfo(HardwareSerial &serial);    // 打印地图信息

  void attack(uint8_t chunk);     // 攻击
  void placeBlock(uint8_t chunk); // 放置方块
  void trade(uint8_t item);       // 交易

  std::vector<Grid> AStar(Grid src, Grid dst);
  std::vector<Grid> BFS(Grid src, Grid dst);
  void turnLeft(double angle, int speed);
  void turnRight(double angle, int speed);

  PlayerInfo getPlayerInfo(void); // 获取玩家信息
  MapInfo getMapInfo(void);       // 获取地图信息

  void setPlayerInfo(PlayerInfo playerInfo);        // 设置玩家信息
  void setMapInfo(MapInfo mapInfo);                 // 设置地图信息
  void setCanMVK210(CanMVK210 *canmvk210);          // 设置CanMV K210
  void setJY62(JY62 *jy62);                         // 设置IMU
  void setPID(PID *pid);                            // 设置PID
  void setTB6612FNG(TB6612FNG *tb6612fng);          // 设置电机驱动
  void setZigbee(Zigbee *zigbee);                   // 设置Zigbee
  void setLastUpdateTicks(int32_t lastUpdateTicks); // 设置上次更新的tick数

private:
  PlayerInfo _playerInfo;   // 玩家信息
  MapInfo _mapInfo;         // 地图信息
  double_t _directionFix;   // 方向修正
  CanMVK210 *_canmvk210;    // CanMV K210
  JY62 *_jy62;              // IMU
  PID *_pid;                // PID
  TB6612FNG *_tb6612fng;    // 电机驱动
  Zigbee *_zigbee;          // Zigbee
  int32_t _lastUpdateTicks; // 上次更新的tick数
};

#endif // PLAYER_H