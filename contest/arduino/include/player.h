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

#define DEFAULT_SPEED 63
#define SAFE_HOME_HEIGHT 2

enum GameStage { READY, RUNNING, BATTLING, FINISHED };

enum Item {
  AGILITY_BOOST,
  HEALTH_BOOST,
  STRENGTH_BOOST,
  WOOL,
  POTION_OF_HEALING
};

const int8_t itemCost[5] = {32, 32, 64, 2, 4};

struct Position {
  float_t x;
  float_t y;
  Position() {}
  Position(float_t x, float_t y) : x(x), y(y) {}
  Position(const Position &position) : x(position.x), y(position.y) {}
  Position &operator=(const Position &position) {
    x = position.x + 0.5;
    y = position.y + 0.5;
    return *this;
  }
  Position operator+(const Position &position) const {
    return Position(x + position.x, y + position.y);
  }
  Position operator-(const Position &position) const {
    return Position(x - position.x, y - position.y);
  }
  bool operator==(const Position &position) const {
    return x == position.x && y == position.y;
  }
  bool operator!=(const Position &position) const {
    return x != position.x || y != position.y;
  }
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
  std::vector<Grid> ironMine;    // 铁矿石
  std::vector<Grid> goldMine;    // 金矿石
  std::vector<Grid> diamondMine; // 钻石矿石
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
  void attack(Grid chunk);        // 攻击
  void placeBlock(uint8_t chunk); // 放置方块
  void placeBlock(Grid chunk);    // 放置方块
  void trade(Item item);          // 交易
  void
  updateStrategy(); // 升级属性//根据目前的属性和持有的绿宝石数量判断最优的升级策略

  Grid findMineral(); // 根据过去采矿的记录、矿物价值和距离判断要开采的矿物

  std::vector<Grid> BFS(Grid src, Grid dst);     // BFS寻路
  std::vector<Grid> BFSPlus(Grid src, Grid dst); // BFS+寻路

  double calculateAngle(Position src, Position dst);    // 计算角度
  double calculateAngle(Position src, Grid dst);        // 计算角度
  int8_t calculateDistance(Position src, Position dst); // 计算距离
  int8_t calculateDistance(Position src, Grid dst);     // 计算距离
  boolean isNear(Position src, Grid dst);               // 是否相邻
  boolean isHome();                                     // 是否在家

  void turnLeft(double angle, int speed);   // 左转
  void turnRight(double angle, int speed);  // 右转
  void faceTo(Grid dst, int speed);         // 面向目标
  bool moveToNextGrid(Grid dst, int speed); // 移动到下一个格子
  bool moveTo(Grid dst, int speed);         // 移动到目标

  void reborn(void); // 重生

  PlayerInfo getPlayerInfo(void); // 获取玩家信息
  MapInfo getMapInfo(void);       // 获取地图信息
  std::vector<int32_t>
  getLastVisitedGoldMine(void); // 获取上次访问金矿石的tick数
  std::vector<int32_t>
  getLastVisitedDiamondMine(void);  // 获取上次访问钻石矿石的tick数
  PlayerState getPlayerState(void); // 获取玩家状态
  double_t getDirectionFix(void);   // 获取方向修正
  Grid getHome(void);               // 获取家的位置
  CanMVK210 *getCanMVK210(void);    // 获取CanMV K210
  JY62 *getJY62(void);              // 获取IMU
  PID *getPID(void);                // 获取PID
  TB6612FNG *getTB6612FNG(void);    // 获取电机驱动
  Zigbee *getZigbee(void);          // 获取Zigbee
  int32_t getLastUpdateTicks(void); // 获取上次更新的tick数
  int32_t getLastAttackTicks(void); // 获取上次攻击的tick数
  int8_t getDesiredEmeraldCount(void); // 获取期望的绿宝石数量
  int8_t getLeastEmeraldCount(void);   // 获取至少保留的绿宝石数量
  double_t getAttackCooldown(void);    // 获取攻击冷却
  int8_t getHomeHeight(void);          // 获取家的高度
  int8_t getSafeHomeHeight(void);      // 获取安全的家的高度

  void setPlayerInfo(PlayerInfo playerInfo);        // 设置玩家信息
  void setMapInfo(MapInfo mapInfo);                 // 设置地图信息
  void setPlayerState(PlayerState playerState);     // 设置玩家状态
  void setDirectionFix(double_t directionFix);      // 设置方向修正
  void setHome(Grid home);                          // 设置家的位置
  void setCanMVK210(CanMVK210 *canmvk210);          // 设置CanMV K210
  void setJY62(JY62 *jy62);                         // 设置IMU
  void setPID(PID *pid);                            // 设置PID
  void setTB6612FNG(TB6612FNG *tb6612fng);          // 设置电机驱动
  void setZigbee(Zigbee *zigbee);                   // 设置Zigbee
  void setLastUpdateTicks(int32_t lastUpdateTicks); // 设置上次更新的tick数
  void
  setDesiredEmeraldCount(int8_t desiredEmeraldCount); // 设置期望的绿宝石数量
  void setSafeHomeHeight(int8_t safeHomeHeight); // 设置家的高度

private:
  PlayerInfo _playerInfo;                    // 玩家信息
  MapInfo _mapInfo;                          // 地图信息
  std::vector<int32_t> _lastVisitedGoldMine; // 上次访问金矿石的tick数
  std::vector<int32_t> _lastVisitedDiamondMine; // 上次访问钻石矿石的tick数
  PlayerState _playerState;                     // 玩家状态
  double_t _directionFix;                       // 方向修正
  Grid _home;                                   // 家的位置
  CanMVK210 *_canmvk210;                        // CanMV K210
  JY62 *_jy62;                                  // IMU
  PID *_pid;                                    // PID
  TB6612FNG *_tb6612fng;                        // 电机驱动
  Zigbee *_zigbee;                              // Zigbee
  int32_t _lastUpdateTicks;                     // 上次更新的tick数
  int32_t _lastAttackTicks;                     // 上次攻击的tick数
  int8_t _desiredEmeraldCount;                  // 期望的绿宝石数量
  int8_t _leastEmeraldCount; // 至少保留的绿宝石数量
  double_t _attackCooldown;  // 攻击冷却
  int8_t _homeHeight;        // 家的高度
  int8_t _safeHomeHeight;    // 安全的家的高度
};

#endif // PLAYER_H