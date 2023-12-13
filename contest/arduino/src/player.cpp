#include "player.h"

/**
 * @brief 构造函数
 *
 */
Player::Player() {
  _canmvk210 = NULL;
  _jy62 = NULL;
  _pid = NULL;
  _tb6612fng = NULL;
  _zigbee = NULL;
  _playerInfo.gameStage = READY;
  _playerInfo.elapsedTicks = 0;
  _playerInfo.hasBed = false;
  _playerInfo.hasBedOpponent = false;
  _playerInfo.position.x = 0;
  _playerInfo.position.y = 0;
  _playerInfo.positionOpponent.x = 0;
  _playerInfo.positionOpponent.y = 0;
  _playerInfo.agility = 0;
  _playerInfo.health = 0;
  _playerInfo.maxHealth = 0;
  _playerInfo.strength = 0;
  _playerInfo.emeraldCount = 0;
  _playerInfo.woolCount = 0;
  _mapInfo.ironMine.clear();
  _mapInfo.goldMine.clear();
  _mapInfo.diamondMine.clear();
  _directionFix = 0;
  _lastUpdateTicks = -1;
}

/**
 * @brief 更新玩家信息
 *
 */
void Player::updatePlayerInfo() {
  if (_zigbee != NULL && _zigbee->message[0] == 0x55 &&
      _zigbee->message[1] == 0xAA) {
    _playerInfo.gameStage = (GameStage)_zigbee->message[0 + 5];
    _playerInfo.elapsedTicks = *((int32_t *)(_zigbee->message + 1 + 5));
    memcpy(_playerInfo.heightOfChunks, _zigbee->message + 5 + 5, 64);
    _playerInfo.hasBed = (bool)_zigbee->message[69 + 5];
    _playerInfo.hasBedOpponent = (bool)_zigbee->message[70 + 5];
    _playerInfo.position.x = *((float_t *)(_zigbee->message + 71 + 5));
    _playerInfo.position.y = *((float_t *)(_zigbee->message + 75 + 5));
    _playerInfo.positionOpponent.x = *((float_t *)(_zigbee->message + 79 + 5));
    _playerInfo.positionOpponent.y = *((float_t *)(_zigbee->message + 83 + 5));
    _playerInfo.agility = _zigbee->message[87 + 5];
    _playerInfo.health = _zigbee->message[88 + 5];
    _playerInfo.maxHealth = _zigbee->message[89 + 5];
    _playerInfo.strength = _zigbee->message[90 + 5];
    _playerInfo.emeraldCount = _zigbee->message[91 + 5];
    _playerInfo.woolCount = _zigbee->message[92 + 5];
  }
}

void Player::updateMapInfo() {
  if (_canmvk210 != NULL) {
    // TODO
  }
}

/**
 * @brief 打印玩家信息
 *
 * @param serial 串口
 */
void Player::printPlayerInfo(HardwareSerial &serial) {
  serial.println("*****PLAYER INFO BEGIN*****");
  serial.println("player info:");
  serial.println("gameStage:");
  serial.println(_playerInfo.gameStage);
  serial.println("elapsedTicks:");
  serial.println(_playerInfo.elapsedTicks);
  serial.println("hasBed:");
  serial.println("heightOfChunks:");
  for (int i = 0; i < 64; i++) {
    serial.print(_playerInfo.heightOfChunks[i]);
    serial.print(" ");
  }
  serial.println();
  serial.println(_playerInfo.hasBed);
  serial.println("hasBedOpponent:");
  serial.println(_playerInfo.hasBedOpponent);
  serial.println("position:");
  serial.println(_playerInfo.position.x);
  serial.println(_playerInfo.position.y);
  serial.println("positionOpponent:");
  serial.println(_playerInfo.positionOpponent.x);
  serial.println(_playerInfo.positionOpponent.y);
  serial.println("agility:");
  serial.println(_playerInfo.agility);
  serial.println("health:");
  serial.println(_playerInfo.health);
  serial.println("maxHealth:");
  serial.println(_playerInfo.maxHealth);
  serial.println("strength:");
  serial.println(_playerInfo.strength);
  serial.println("emeraldCount:");
  serial.println(_playerInfo.emeraldCount);
  serial.println("woolCount:");
  serial.println(_playerInfo.woolCount);
  serial.println("*****PLAYER INFO END*****");
}

/**
 * @brief 打印地图信息
 *
 * @param serial 串口
 */
void Player::printMapInfo(HardwareSerial &serial) {
  serial.println("*****MAP INFO BEGIN*****");
  serial.println("ironMine:");
  for (int i = 0; i < _mapInfo.ironMine.size(); i++) {
    serial.print(_mapInfo.ironMine[i].x);
    serial.print(" ");
    serial.print(_mapInfo.ironMine[i].y);
    serial.print(" ");
  }
  serial.println();
  serial.println("goldMine:");
  for (int i = 0; i < _mapInfo.goldMine.size(); i++) {
    serial.print(_mapInfo.goldMine[i].x);
    serial.print(" ");
    serial.print(_mapInfo.goldMine[i].y);
    serial.print(" ");
  }
  serial.println();
  serial.println("diamondMine:");
  for (int i = 0; i < _mapInfo.diamondMine.size(); i++) {
    serial.print(_mapInfo.diamondMine[i].x);
    serial.print(" ");
    serial.print(_mapInfo.diamondMine[i].y);
    serial.print(" ");
  }
  serial.println();
  serial.println("*****MAP INFO END*****");
}

/**
 * @brief 攻击
 *
 * @param chunk 区块
 */
void Player::attack(uint8_t chunk) {
  if (_zigbee != NULL) {
    uint8_t msg[8] = {0x55, 0xAA, 0x00, 0x00, 0x02, chunk, 0x00, chunk};
    _zigbee->send(msg, 8);
  }
}

/**
 * @brief 放置方块
 *
 * @param chunk 区块
 */
void Player::placeBlock(uint8_t chunk) {
  if (_zigbee != NULL) {
    uint8_t msg[8] = {0x55, 0xAA, 0x00, 0x00, 0x02, (uint8_t)(0x01 ^ chunk),
                      0x01, chunk};
    _zigbee->send(msg, 8);
  }
}

/**
 * @brief 交易
 *
 * @param item 物品
 */
void Player::trade(uint8_t item) {
  if (_zigbee != NULL) {
    uint8_t msg[8] = {0x55, 0xAA, 0x00, 0x00, 0x02, (uint8_t)(0x02 ^ item),
                      0x02, item};
    _zigbee->send(msg, 8);
  }
}

/**
 * @brief BFS算法
 *
 * @param src 起点
 * @param dst 终点
 * @return std::vector<Grid> 路径
 */
std::vector<Grid> Player::BFS(Grid src, Grid dst) {
  std::vector<Grid> path;
  int8_t map[8][8];
  for (int i = 0; i < 8; i++) {
    memcpy(map[i], _playerInfo.heightOfChunks + i * 8, 8);
  }
  std::queue<Grid> q;
  q.push(src);
  int8_t pre[8][8];
  memset(pre, -1, sizeof(pre));
  pre[src.x][src.y] = 0;
  Grid dir[4] = {Grid(0, 1), Grid(0, -1), Grid(1, 0), Grid(-1, 0)};
  while (!q.empty()) {
    Grid cur = q.front();
    q.pop();
    if (cur == dst) {
      break;
    }
    for (int i = 0; i < 4; i++) {
      Grid next = cur + dir[i];
      if (next.x < 0 || next.x >= 8 || next.y < 0 || next.y >= 8) {
        continue;
      }
      if (pre[next.x][next.y] != -1) {
        continue;
      }
      if (map[next.x][next.y] == 0) {
        continue;
      }
      pre[next.x][next.y] = i;
      q.push(next);
    }
  }
  if (pre[dst.x][dst.y] != -1) {
    Grid cur = dst;
    while (cur != src) {
      path.push_back(cur);
      cur = cur - dir[pre[cur.x][cur.y]];
    }
    path.push_back(src);
    std::reverse(path.begin(), path.end());
  }
  return path;
}

/**
 * @brief 左转
 *
 * @param angle
 * @param speed
 */
void Player::turnLeft(double angle, int speed) {
  if (_tb6612fng != NULL) {
    double angleCurrent = _jy62->getAngl().yaw;
    double angleTarget = angleCurrent + angle;
    if (angleTarget > 180) {
      angleTarget -= 360;
    } else if (angleTarget < -180) {
      angleTarget += 360;
    }
    while (abs(angleCurrent - angleTarget) > 1) {
      angleCurrent = _jy62->getAngl().yaw;
      _tb6612fng->turnLeft(speed);
    }
    _tb6612fng->stop();
  }
}

/**
 * @brief 右转
 *
 * @param angle
 * @param speed
 */
void Player::turnRight(double angle, int speed) {
  if (_tb6612fng != NULL) {
    double angleCurrent = _jy62->getAngl().yaw;
    double angleTarget = angleCurrent - angle;
    if (angleTarget > 180) {
      angleTarget -= 360;
    } else if (angleTarget < -180) {
      angleTarget += 360;
    }
    while (abs(angleCurrent - angleTarget) > 1) {
      angleCurrent = _jy62->getAngl().yaw;
      _tb6612fng->turnRight(speed);
    }
    _tb6612fng->stop();
  }
}

// @brief 获取玩家信息
PlayerInfo Player::getPlayerInfo() { return _playerInfo; }

// @brief 获取地图信息
MapInfo Player::getMapInfo() { return _mapInfo; }

// @brief 设置玩家信息
void Player::setPlayerInfo(PlayerInfo playerInfo) { _playerInfo = playerInfo; }

// @brief 设置地图信息
void Player::setMapInfo(MapInfo mapInfo) { _mapInfo = mapInfo; }

// @brief 设置CanMV K210
void Player::setCanMVK210(CanMVK210 *canmvk210) { _canmvk210 = canmvk210; }

// @brief 设置IMU
void Player::setJY62(JY62 *jy62) { _jy62 = jy62; }

// @brief 设置PID
void Player::setPID(PID *pid) { _pid = pid; }

// @brief 设置电机驱动
void Player::setTB6612FNG(TB6612FNG *tb6612fng) { _tb6612fng = tb6612fng; }

// @brief 设置Zigbee
void Player::setZigbee(Zigbee *zigbee) { _zigbee = zigbee; }

// @brief 设置上次更新的tick数
void Player::setLastUpdateTicks(int32_t lastUpdateTicks) {
  _lastUpdateTicks = lastUpdateTicks;
}