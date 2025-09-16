#ifndef APPLE_H_
#define APPLE_H_

#include "game.h"

namespace s21 {

class Apple {
 private:
  std::pair<int, int> AppleBody;
  bool Eaten;
 public:
  Apple(int x, int y);

  int getX() { return AppleBody.first; }
  int getY() { return AppleBody.second; }
  bool IsEaten();
  void Respawn(int x, int y);
  void SetEaten();
};


} // namespace s21

#endif // APPLE_H_