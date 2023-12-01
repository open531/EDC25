#include "player.h"

Player::Player(Zigbee *zigbee) {
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

void Player::update() {
  _playerInfo.gameStage = (GameStage)_zigbee->message[0];
  _playerInfo.elapsedTicks = *((int32_t *)(_zigbee->message + 1));
  memcpy(_playerInfo.heightOfChunks, _zigbee->message + 5, 64);
  _playerInfo.hasBed = (boolean)_zigbee->message[69];
  _playerInfo.hasBedOpponent = (boolean)_zigbee->message[70];
  _playerInfo.position.x = *((float_t *)(_zigbee->message + 71));
  _playerInfo.position.y = *((float_t *)(_zigbee->message + 75));
  _playerInfo.positionOpponent.x = *((float_t *)(_zigbee->message + 79));
  _playerInfo.positionOpponent.y = *((float_t *)(_zigbee->message + 83));
  _playerInfo.agility = _zigbee->message[87];
  _playerInfo.health = _zigbee->message[88];
  _playerInfo.maxHealth = _zigbee->message[89];
  _playerInfo.strength = _zigbee->message[90];
  _playerInfo.emeraldCount = _zigbee->message[91];
  _playerInfo.woolCount = _zigbee->message[92];
}

void Player::attack(uint8_t chunk) {
  uint8_t msg[8] = {0x55, 0xAA, 0x00, 0x00, 0x02, chunk, 0x00, chunk};
  _zigbee->send(msg, 8);
}

void Player::placeBlock(uint8_t chunk) {
  uint8_t msg[8] = {0x55, 0xAA, 0x00, 0x00, 0x02, (uint8_t)(0x01 ^ chunk),
                    0x01, chunk};
  _zigbee->send(msg, 8);
}

void Player::trade(uint8_t item) {
  uint8_t msg[8] = {0x55, 0xAA, 0x00, 0x00, 0x02, (uint8_t)(0x02 ^ item),
                    0x02, item};
  _zigbee->send(msg, 8);
}

PlayerInfo Player::getPlayerInfo() { return _playerInfo; }
