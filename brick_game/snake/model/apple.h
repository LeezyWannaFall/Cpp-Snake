#ifndef APPLE_H_
#define APPLE_H_

#include <random>
#include <utility>
#include <chrono>
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

namespace s21 {

using Position = std::pair<int, int>;

class Apple {
 private:
  Position AppleBody;
  bool Eaten;

 public:
  Apple();

  Position getPosition() const { return AppleBody; }
  int getX() { return AppleBody.first; }
  int getY() { return AppleBody.second; }
  bool IsEaten();
  void Respawn();
  void SetEaten();
};

}  // namespace s21

#endif  // APPLE_H_