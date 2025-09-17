#include "snake.h"

s21::Snake::Snake() {
  dir = Direction::RIGHT;
  alive = true;

  for (int i = 0; i < 4; i++) {
    snakeBody.push_back({5 - i, 10});
  }
}

void s21::Snake::Move(bool ateApple) {
  int x = getHead().first;
  int y = getHead().second;

  if (this->dir == Direction::RIGHT) x++;
  if (this->dir == Direction::LEFT) x--;
  if (this->dir == Direction::UP) y--;
  if (this->dir == Direction::DOWN) y++;

  snakeBody.insert(snakeBody.begin(), {x, y});

  if (!ateApple) {
    snakeBody.pop_back();
  }
}

void s21::Snake::SetDirection(Direction NewDir) {
  if (!((this->dir == Direction::RIGHT && NewDir == Direction::LEFT) ||
        (this->dir == Direction::LEFT && NewDir == Direction::RIGHT) ||
        (this->dir == Direction::UP && NewDir == Direction::DOWN) ||
        (this->dir == Direction::DOWN && NewDir == Direction::UP)))
    this->dir = NewDir;
}

bool s21::Snake::checkCollision(int width, int height) const {
  int x = getHead().first;
  int y = getHead().second;

  if (x < 0 || x >= width) return true;
  if (y < 0 || y >= height) return true;

  for (int i = 1; i < snakeBody.size(); i++) {
    if (x == snakeBody[i].first && y == snakeBody[i].second) return true;
  }
  return false;
}
