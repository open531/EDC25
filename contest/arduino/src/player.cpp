#include "player.h"

/**
 * @brief 构造函数
 *
 * @param zigbee Zigbee
 */
Player::Player(Zigbee *zigbee) {
  _canmvk210 = NULL;
  _zigbee = zigbee;
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
  _lastUpdateTicks = -1;
}

/**
 * @brief 构造函数
 *
 * @param canmvk210 CanMV K210
 * @param zigbee Zigbee
 */
Player::Player(CanMVK210 *canmvk210, Zigbee *zigbee) {
  _canmvk210 = canmvk210;
  _zigbee = zigbee;
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
 * @brief 打印玩家信息
 *
 * @param serial 串口
 */
void Player::printInfo(HardwareSerial &serial) {
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

// @brief 获取玩家信息
PlayerInfo Player::getPlayerInfo() { return _playerInfo; }

// @brief 获取地图信息
MapInfo Player::getMapInfo() { return _mapInfo; }
