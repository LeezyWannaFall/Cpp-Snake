#ifndef SNAKE_H_
#define SNAKE_H_

#include "../common/direction.h"

#include <vector>

namespace s21 {

class Snake {
 public:
  Snake();

  void Move();                 // просто двигается
  void Grow();                 // увеличивается на 1 сегмент
  void SetDirection(Direction dir);
  bool checkCollision(int width, int height, int x, int y) const;
  bool isAlive() const { return alive; }
  std::vector<std::pair<int, int>> getBody() const { return snakeBody; }
  std::pair<int, int> getHead() const { return snakeBody.front(); }
  

 private:
  std::vector<std::pair<int, int>> snakeBody;
  Direction dir;
  bool alive;
};


}  // namespace s21

#endif  // SNAKE_H_