#include "snake.h"

s21::Snake::Snake(int startX, int startY) {
  dir = Direction::RIGHT;
  alive = true;

  for (int i = 0; i < 4; i++) {
    snakeBody.push_back({startX - i, startY});
  }
}

void s21::Snake::move(bool ateApple) {
  auto head = snakeBody.front();
  int x = head.first;
  int y = head.second;

  if (dir == Direction::RIGHT) x++;
  if (dir == Direction::LEFT) x--;
  if (dir == Direction::UP) y--;
  if (dir == Direction::DOWN) y++;

  snakeBody.insert(snakeBody.begin(), {x, y});

  if (!ateApple) {
    snakeBody.pop_back();
  }
}

void s21::Snake::setDirection(Direction NewDir) {
    if (!((this->dir == Direction::RIGHT && NewDir == Direction::LEFT) ||
    (this->dir == Direction::LEFT && NewDir == Direction::RIGHT) ||
    (this->dir == Direction::UP && NewDir == Direction::DOWN) ||
    (this->dir == Direction::DOWN && NewDir == Direction::UP))) {
        this->dir = NewDir;
    }
}

bool s21::Snake::checkCollision(int width, int height) const {}
