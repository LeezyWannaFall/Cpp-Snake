#ifndef SNAKE_H_
#define SNAKE_H_

#include "game.h"

namespace s21 {

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Snake {
 public:
  Snake();

  void Move(bool ateApple);
  void SetDirection(Direction dir);
  bool checkCollision(int width, int height) const;

  std::vector<std::pair<int, int>> getBody() const { return snakeBody; }
  std::pair<int, int> getHead() const { return snakeBody.front(); }

 private:
  std::vector<std::pair<int, int>> snakeBody;
  Direction dir;
  bool alive;
};

}  // namespace s21

#endif  // SNAKE_H_