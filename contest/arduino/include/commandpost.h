#ifndef COMMANDPOST_H
#define COMMANDPOST_H

#include "jy62.h"
#include "player.h"
#include "tb6612fng.h"

class CommandPost {
public:
  CommandPost(Player *player, JY62 *jy62, TB6612FNG *tb6612fng);
  std::vector<Grid> AStar(Grid src, Grid dst);
  void turnLeft(double angle);
  void turnRight(double angle);

private:
  Player *player;
  JY62 *jy62;
  TB6612FNG *tb6612fng;
};

#endif // COMMANDPOST_H
