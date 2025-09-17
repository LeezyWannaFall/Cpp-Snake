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

void s21::Apple::Respawn(int x, int y) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distX(0, x - 1);
  std::uniform_int_distribution<int> distY(0, y - 1);

  // TODO: check if apple spawn in clear place
  AppleBody.first = distX(gen);
  AppleBody.second = distY(gen);
}

void s21::Apple::SetEaten() { Eaten = true; }