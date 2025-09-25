#include "apple.h"

s21::Apple::Apple() {
  Eaten = false;
  AppleBody.first = 7;
  AppleBody.second = 10;
}

bool s21::Apple::IsEaten() {
  if (Eaten) return true;
  return false;
}

void s21::Apple::Respawn(const std::vector<std::pair<int, int>>& snakeBody) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distX(0, FIELD_WIDTH - 1);
  std::uniform_int_distribution<int> distY(0, FIELD_HEIGHT - 1);

  while (true) {
    int x = distX(gen);
    int y = distY(gen);
    bool conflict = false;
    for (const auto& part : snakeBody) {
      if (part.first == x && part.second == y) {
        conflict = true;
        break;
      }
    }
    if (!conflict) {
      AppleBody.first = x;
      AppleBody.second = y;
      break;
    }
  }
  Eaten = false;
}

void s21::Apple::SetEaten() { Eaten = true; }