#include "snake.h"

s21::Snake::Snake() {
  dir = Direction::RIGHT;
  alive = true;

  for (int i = 0; i < 4; i++) {
    snakeBody.push_back({5 - i, 10});
  }
}

void s21::Snake::Move() {
  int x = getHead().first;
  int y = getHead().second;

  if (dir == Direction::RIGHT) x++;
  if (dir == Direction::LEFT) x--;
  if (dir == Direction::UP) y--;
  if (dir == Direction::DOWN) y++;

  if (checkCollision(10, 20, x, y)) {
    alive = false;
    return;
  } else {
    snakeBody.insert(snakeBody.begin(), {x, y});
    snakeBody.pop_back();
  }
}

void s21::Snake::Grow() {
  snakeBody.push_back(snakeBody.back());
}


void s21::Snake::SetDirection(Direction NewDir) {
  if ((dir == Direction::UP && NewDir == Direction::DOWN) ||
      (dir == Direction::DOWN && NewDir == Direction::UP) ||
      (dir == Direction::LEFT && NewDir == Direction::RIGHT) ||
      (dir == Direction::RIGHT && NewDir == Direction::LEFT)) {
    return;
  }
  dir = NewDir;
}

bool s21::Snake::checkCollision(int width, int height, int x, int y) const {
  if (x < 0 || x >= width) return true;
  if (y < 0 || y >= height) return true;

  for (int i = 1; i < snakeBody.size(); i++) {
    if (x == snakeBody[i].first && y == snakeBody[i].second) return true;
  }
  return false;
}
