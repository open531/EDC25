#include "commandpost.h"

/**
 * @brief 构造函数
 *
 * @param player 玩家
 * @param jy62 IMU
 * @param tb6612fng 电机驱动
 */
CommandPost::CommandPost(Player *player, JY62 *jy62, TB6612FNG *tb6612fng) {
  this->player = player;
  this->jy62 = jy62;
  this->tb6612fng = tb6612fng;
}

/**
 * @brief A*算法
 *
 * @param src 出发点
 * @param dst 目的地
 * @return std::vector<Grid> 路径
 */
std::vector<Grid> CommandPost::AStar(Grid src, Grid dst) {
  std::vector<Grid> path;
  int8_t map[8][8];
  for (int8_t i = 0; i < 8; i++) {
    for (int8_t j = 0; j < 8; j++) {
      map[i][j] = player->getPlayerInfo().heightOfChunks[i * 8 + j];
    }
  }
  int8_t dx[8] = {0, 1, 0, -1, 1, 1, -1, -1};
  int8_t dy[8] = {1, 0, -1, 0, 1, -1, 1, -1};
  int8_t sx = src.x, sy = src.y, ex = dst.x, ey = dst.y;
  int8_t vis[8][8] = {0};
  int8_t dis[8][8];
  int8_t pre[8][8];
  for (int8_t i = 0; i < 8; i++) {
    for (int8_t j = 0; j < 8; j++) {
      dis[i][j] = 0xff;
      pre[i][j] = -1;
    }
  }
  dis[sx][sy] = 0;
  std::queue<Grid> q;
  q.push(src);
  while (!q.empty()) {
    Grid u = q.front();
    q.pop();
    vis[u.x][u.y] = 0;
    for (int8_t i = 0; i < 8; i++) {
      int8_t nx = u.x + dx[i], ny = u.y + dy[i];
      if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) {
        continue;
      }
      if (map[nx][ny] <= 0) {
        continue;
      }
      if (dis[nx][ny] > dis[u.x][u.y] + map[nx][ny]) {
        dis[nx][ny] = dis[u.x][u.y] + map[nx][ny];
        pre[nx][ny] = i;
        if (!vis[nx][ny]) {
          vis[nx][ny] = 1;
          q.push({nx, ny});
        }
      }
    }
  }
  int8_t x = ex, y = ey;
  while (x != sx || y != sy) {
    path.push_back({x, y});
    int8_t d = pre[x][y];
    x -= dx[d];
    y -= dy[d];
  }
  path.push_back({sx, sy});
  std::reverse(path.begin(), path.end());
  return path;
}

/**
 * @brief 左转
 *
 * @param angle 角度
 */
void CommandPost::turnLeft(double angle) {
  double yaw = jy62->getYaw();
  double targetYaw = yaw + angle;
  if (targetYaw > 180) {
    targetYaw -= 360;
  }
  while (abs(jy62->getYaw() - targetYaw) > 2) {
    tb6612fng->turnLeft(255);
  }
  tb6612fng->stop();
}

/**
 * @brief 右转
 *
 * @param angle 角度
 */
void CommandPost::turnRight(double angle) {
  double yaw = jy62->getYaw();
  double targetYaw = yaw - angle;
  if (targetYaw < -180) {
    targetYaw += 360;
  }
  while (abs(jy62->getYaw() - targetYaw) > 2) {
    tb6612fng->turnRight(255);
  }
  tb6612fng->stop();
}
