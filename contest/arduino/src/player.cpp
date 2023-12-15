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
  _playerState = IDLE;
  _directionFix = 0;
  _home.x = 0;
  _home.y = 0;
  _lastUpdateTicks = -1;
  _lastAttackTicks = -1;
  _desiredEmeraldCount = 100;
  _leastEmeraldCount = 8;
  _attackCooldown = 8.5;
  _homeHeight = 0;
  _safeHomeHeight = 2;
  _lastVisitedDiamondMine.clear();
  _lastVisitedGoldMine.clear();
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
    _attackCooldown = max(8.5 - 0.25 * _playerInfo.agility, 0.5);
    _homeHeight = _playerInfo.heightOfChunks[_home.x * 8 + _home.y];
  }
}

void Player::updateMapInfo() {
  if (_canmvk210 != NULL &&
      (_canmvk210->message[0] == 0x55 || _canmvk210->message[0] == 0x56) &&
      _canmvk210->message[1] == 0xAA) {
    uint8_t type = _canmvk210->message[0];
    uint8_t len = _canmvk210->message[2];
    uint8_t *data = _canmvk210->message + 3;
    if (type == 0x55) {
      _mapInfo.goldMine.clear();
      for (int i = 0; i < len / 2; i++) {
        _mapInfo.goldMine.push_back(Grid(data[i * 2], data[i * 2 + 1]));
        _lastVisitedGoldMine.push_back(-1);
      }
    } else if (type == 0x56) {
      _mapInfo.diamondMine.clear();
      for (int i = 0; i < len / 2; i++) {
        _mapInfo.diamondMine.push_back(Grid(data[i * 2], data[i * 2 + 1]));
        _lastVisitedDiamondMine.push_back(-1);
      }
    }
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
    _lastAttackTicks = _playerInfo.elapsedTicks;
  }
}

/**
 * @brief 攻击
 *
 * @param chunk 区块
 */
void Player::attack(Grid chunk) {
  if (_zigbee != NULL) {
    uint8_t msg[7] = {0x55,
                      0xAA,
                      0x02,
                      0x00,
                      (uint8_t)(0x00 ^ (uint8_t)(chunk.x * 8 + chunk.y)),
                      0x00,
                      (uint8_t)(chunk.x * 8 + chunk.y)};
    _zigbee->send(msg, 8);
    _lastAttackTicks = _playerInfo.elapsedTicks;
  }
}

/**
 * @brief 放置方块
 *
 * @param chunk 区块
 */
void Player::placeBlock(uint8_t chunk) {
  if (_zigbee != NULL) {
    uint8_t msg[7] = {0x55, 0xAA, 0x02, 0x00, (uint8_t)(0x01 ^ chunk),
                      0x01, chunk};
    _zigbee->send(msg, 8);
  }
}

/**
 * @brief 放置方块
 *
 * @param chunk 区块
 */
void Player::placeBlock(Grid chunk) {
  if (_zigbee != NULL) {
    uint8_t msg[7] = {0x55,
                      0xAA,
                      0x02,
                      0x00,
                      (uint8_t)(0x01 ^ (uint8_t)(chunk.x * 8 + chunk.y)),
                      0x01,
                      (uint8_t)(chunk.x * 8 + chunk.y)};
    _zigbee->send(msg, 8);
  }
}

/**
 * @brief 交易
 *
 * @param item 物品
 */
void Player::trade(Item item) {
  if (_zigbee != NULL) {
    uint8_t msg[8] = {0x55, 0xAA, 0x00,
                      0x00, 0x02, (uint8_t)(0x02 ^ (uint8_t)item),
                      0x02, item};
    _zigbee->send(msg, 8);
  }
}

void Player::updateStrategy() {
  do {
    if (getPlayerInfo().maxHealth / 6 > getPlayerInfo().strength &&
        getPlayerInfo().emeraldCount >= 64) {
      trade(STRENGTH_BOOST);
    } else if (getPlayerInfo().emeraldCount >= 32) {
      trade(HEALTH_BOOST);
    }
  } while (getPlayerInfo().emeraldCount >= getLeastEmeraldCount());
}

void Player::shop() { // 应该采用什么样的加点策略呢？
  // 优先把血条补满吗？但是可能导致手上剩下31个绿宝石没有用
  // 优先升级属性吗？但是可能导致血条只剩五格血而暴毙
  int8_t best_boost = 0, Potion = 0;
  int8_t money = getPlayerInfo().emeraldCount;
  int8_t minus = getPlayerInfo().maxHealth - getPlayerInfo().health;
  for (int8_t i = 0; 32 * i <= money; i++) {
    int8_t boost = 3 * i;
    int8_t PotionCount = money / 4 - i * 8;
    if (PotionCount > minus)
      boost += minus;
    else
      boost += PotionCount;
    if (boost >= best_boost) {
      best_boost = boost;
      Potion = PotionCount;
    }
  }
  for (int8_t i = 0; i < (money - 4 * Potion) / 32; i++)
    trade(HEALTH_BOOST);
  while (getPlayerInfo().emeraldCount >= 4)
    trade(POTION_OF_HEALING);
}

Grid Player::findMineral() {
  Grid mineral;
  double score = -1;
  for (int i = 0; i < _mapInfo.goldMine.size(); i++) {
    if ((_playerInfo.elapsedTicks - _lastVisitedGoldMine[i]) /
            calculateDistance(_playerInfo.position, _mapInfo.goldMine[i]) >=
        score) {
      mineral = _mapInfo.goldMine[i];
      score = (_playerInfo.elapsedTicks - _lastVisitedGoldMine[i]) /
              calculateDistance(_playerInfo.position, _mapInfo.goldMine[i]);
    }
  }
  for (int i = 0; i < _mapInfo.diamondMine.size(); i++) {
    if ((_playerInfo.elapsedTicks - _lastVisitedDiamondMine[i]) * 4 /
            calculateDistance(_playerInfo.position, _mapInfo.diamondMine[i]) >=
        score) {
      mineral = _mapInfo.diamondMine[i];
      score = (_playerInfo.elapsedTicks - _lastVisitedDiamondMine[i]) * 4 /
              calculateDistance(_playerInfo.position, _mapInfo.diamondMine[i]);
    }
  }
  return mineral;
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
  int8_t pre[8][8];
  memset(pre, -1, sizeof(pre));
  pre[src.x][src.y] = 0;
  std::queue<Grid> q;
  q.push(src);
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
 * @brief BFS算法
 *
 * @param src 起点
 * @param dst 终点
 * @return std::vector<Grid> 路径
 */
std::vector<Grid> Player::BFSPlus(Grid src, Grid dst) {
  std::vector<Grid> path;
  int8_t map[8][8];
  for (int i = 0; i < 8; i++) {
    memcpy(map[i], _playerInfo.heightOfChunks + i * 8, 8);
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 4; j++) {
      map[i][j] += 1;
    }
  }
  int8_t pre[8][8];
  memset(pre, -1, sizeof(pre));
  pre[src.x][src.y] = 0;
  std::queue<Grid> q;
  q.push(src);
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
 * @brief 计算角度
 *
 * @param src 起点
 * @param dst 终点
 * @return double 角度
 */
double Player::calculateAngle(Position src, Position dst) {
  double angle = atan2(dst.y - src.y, dst.x - src.x) * 180 / PI;
  if (angle > 180) {
    angle -= 360;
  } else if (angle < -180) {
    angle += 360;
  }
  return angle;
}

/**
 * @brief 计算角度
 *
 * @param src 起点
 * @param dst 终点
 * @return double 角度
 */
double Player::calculateAngle(Position src, Grid dst) {
  double angle = atan2(dst.y + 0.5 - src.y, dst.x + 0.5 - src.x) * 180 / PI;
  if (angle > 180) {
    angle -= 360;
  } else if (angle < -180) {
    angle += 360;
  }
  return angle;
}

/**
 * @brief 计算距离
 *
 * @param src 起点
 * @param dst 终点
 * @return int8_t 距离
 */
int8_t Player::calculateDistance(Position src, Position dst) {
  return (int8_t)(abs(dst.x - src.x) + abs(dst.y - src.y));
}

/**
 * @brief 计算距离
 *
 * @param src 起点
 * @param dst 终点
 * @return int8_t 距离
 */
int8_t Player::calculateDistance(Position src, Grid dst) {
  return (int8_t)(abs(dst.x + 0.5 - src.x) + abs(dst.y + 0.5 - src.y));
}

boolean Player::isNear(Position src, Grid dst) {
  return abs(dst.x + 0.5 - src.x) < 1.5 && abs(dst.y + 0.5 - src.y) < 1.5;
}

boolean Player::isHome() {
  return (int8_t)getPlayerInfo().position.x == getHome().x &&
         (int8_t)getPlayerInfo().position.y == getHome().y;
}

/**
 * @brief 左转
 *
 * @param angle 角度
 * @param speed 速度
 */
void Player::turnLeft(double angle, int speed = DEFAULT_SPEED) {
  if (_jy62 != NULL && _tb6612fng != NULL) {
    double angleCurrent = _jy62->getAngl().yaw;
    double angleTarget = angleCurrent + angle;
    if (angleTarget > 180) {
      angleTarget -= 360;
    } else if (angleTarget < -180) {
      angleTarget += 360;
    }
    if (abs(angleCurrent - angleTarget) > 1) {
      angleCurrent = _jy62->getAngl().yaw;
      _tb6612fng->turnLeft(speed);
    } else {
      _tb6612fng->turnRight(speed);
      _tb6612fng->stop();
    }
  }
}

/**
 * @brief 右转
 *
 * @param angle 角度
 * @param speed 速度
 */
void Player::turnRight(double angle, int speed = DEFAULT_SPEED) {
  if (_jy62 != NULL && _tb6612fng != NULL) {
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
    _tb6612fng->turnLeft(speed);
    _tb6612fng->stop();
  }
}

void Player::faceTo(Grid dst, int speed = DEFAULT_SPEED) {
  if (_jy62 != NULL && _tb6612fng != NULL) {
    double angleCurrent = _jy62->getAngl().yaw;
    double angleTarget = calculateAngle(_playerInfo.position, dst);
    if (angleTarget > 180) {
      angleTarget -= 360;
    } else if (angleTarget < -180) {
      angleTarget += 360;
    }
    if (angleTarget - angleCurrent > 0) {
      if (angleTarget - angleCurrent > 180) {
        angleTarget -= 360;
      }
      while (abs(angleCurrent - angleTarget) > 1) {
        angleCurrent = _jy62->getAngl().yaw;
        _tb6612fng->turnLeft(speed);
      }
      _tb6612fng->turnRight(speed);
      _tb6612fng->stop();
    } else {
      if (angleTarget - angleCurrent < -180) {
        angleTarget += 360;
      }
      while (abs(angleCurrent - angleTarget) > 1) {
        angleCurrent = _jy62->getAngl().yaw;
        _tb6612fng->turnRight(speed);
      }
      _tb6612fng->turnLeft(speed);
      _tb6612fng->stop();
    }
  }
}

/**
 * @brief 移动到下一个格子
 *
 * @param dst 目的地
 * @param speed 速度
 */
bool Player::moveToNextGrid(Grid dst, int speed = DEFAULT_SPEED) {
  if (_jy62 != NULL && _tb6612fng != NULL) {
    faceTo(dst, speed);
    if (_playerInfo.heightOfChunks[dst.x * 8 + dst.y] == 0) {
      if (_playerInfo.woolCount > 0) {
        placeBlock(dst);
      } else {
        _tb6612fng->stop();
        return false;
      }
    }
    while (calculateDistance(_playerInfo.position, dst) > 0.2) {
      _tb6612fng->forward(speed);
    }
    _tb6612fng->backward(speed);
    _tb6612fng->stop();
    return true;
  }
}

/**
 * @brief 移动到目标
 *
 * @param dst 目的地
 * @param speed 速度
 */
bool Player::moveTo(Grid dst, int speed = DEFAULT_SPEED) {
  if (_jy62 != NULL && _tb6612fng != NULL) {
    std::vector<Grid> path = BFSPlus(_playerInfo.position, dst);
    for (int i = 0; i < path.size(); i++) {
      if (i + 1 < path.size() &&
          calculateDistance(_playerInfo.position, path[i + 1]) < 1.5) {
        return moveToNextGrid(path[i + 1], speed);
      } else {
        return moveToNextGrid(path[i], speed);
      }
    }
  }
}

/**
 * @brief 重生
 *
 */
void Player::reborn() {
  if (_jy62 != NULL && _tb6612fng != NULL) {
    faceTo(_home, DEFAULT_SPEED);
    while (calculateDistance(_playerInfo.position, _home) > 0.5) {
      _tb6612fng->forward(DEFAULT_SPEED);
    }
    _tb6612fng->stop();
  }
  while (!_playerInfo.health) {
    _tb6612fng->stop();
  }
}

// @brief 获取玩家信息
PlayerInfo Player::getPlayerInfo() { return _playerInfo; }
// @brief 获取地图信息
MapInfo Player::getMapInfo() { return _mapInfo; }
// @brief 获取上次访问金矿石的tick数
std::vector<int32_t> Player::getLastVisitedGoldMine() {
  return _lastVisitedGoldMine;
}
// @brief 获取上次访问钻石矿石的tick数
std::vector<int32_t> Player::getLastVisitedDiamondMine() {
  return _lastVisitedDiamondMine;
}
// @brief 获取玩家状态
PlayerState Player::getPlayerState() { return _playerState; }
// @brief 获取方向修正
double_t Player::getDirectionFix() { return _directionFix; }
// @brief 获取家的位置
Grid Player::getHome() { return _home; }
// @brief 获取CanMV K210
CanMVK210 *Player::getCanMVK210() { return _canmvk210; }
// @brief 获取IMU
JY62 *Player::getJY62() { return _jy62; }
// @brief 获取PID
PID *Player::getPID() { return _pid; }
// @brief 获取电机驱动
TB6612FNG *Player::getTB6612FNG() { return _tb6612fng; }
// @brief 获取Zigbee
Zigbee *Player::getZigbee() { return _zigbee; }
// @brief 获取上次更新的tick数
int32_t Player::getLastUpdateTicks() { return _lastUpdateTicks; }
// @brief 获取上次攻击的tick数
int32_t Player::getLastAttackTicks() { return _lastAttackTicks; }
// @brief 获取期望的绿宝石数量
int8_t Player::getDesiredEmeraldCount() { return _desiredEmeraldCount; }
// @brief 获取至少保留的绿宝石数量
int8_t Player::getLeastEmeraldCount() {
  return _leastEmeraldCount + getPlayerInfo().elapsedTicks * 3 / 2000;
}
// @brief 获取攻击冷却
double_t Player::getAttackCooldown() { return _attackCooldown; }
// @brief 获取家的高度
int8_t Player::getHomeHeight() { return _homeHeight; }
// @brief 获取安全的家的高度
int8_t Player::getSafeHomeHeight() {
  return _safeHomeHeight + getPlayerInfo().elapsedTicks / 4000;
}

// @brief 设置玩家信息
void Player::setPlayerInfo(PlayerInfo playerInfo) { _playerInfo = playerInfo; }
// @brief 设置地图信息
void Player::setMapInfo(MapInfo mapInfo) { _mapInfo = mapInfo; }
// @brief 设置玩家状态
void Player::setPlayerState(PlayerState playerState) {
  _playerState = playerState;
}
// @brief 设置方向修正
void Player::setDirectionFix(double_t directionFix) {
  _directionFix = directionFix;
}
// @brief 设置家的位置
void Player::setHome(Grid home) { _home = home; }
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
// @brief 设置期望的绿宝石数量
void Player::setDesiredEmeraldCount(int8_t desiredEmeraldCount) {
  _desiredEmeraldCount = desiredEmeraldCount;
}
// @brief 设置家的高度
void Player::setSafeHomeHeight(int8_t safeHomeHeight) {
  _safeHomeHeight = safeHomeHeight;
}
