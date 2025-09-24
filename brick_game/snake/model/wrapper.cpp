#include "wrapper.h"

#include "game.h"

using namespace s21;

GameHandle game_create() { return new Game(); }

void game_destroy(GameHandle g) { delete static_cast<Game*>(g); }

void game_update(GameHandle g) { static_cast<Game*>(g)->updateCurrentState(); }

void game_set_direction(GameHandle g, int dir) {
  static_cast<Game*>(g)->SetDirection(static_cast<Direction>(dir));
}

int game_get_score(GameHandle g) {
  return static_cast<Game*>(g)->getInfo().score;
}

int game_get_pause(GameHandle g) {
  return static_cast<Game*>(g)->getInfo().pause;
}

int game_get_snake_length(GameHandle g) {
  return static_cast<Game*>(g)->getSnake().getBody().size();
}

int game_get_snake_body(GameHandle g, int* buffer, int bufSize) {
  auto body = static_cast<Game*>(g)->getSnake().getBody();
  int count = std::min((int)body.size(), bufSize / 2);  // x,y по 2 числа
  for (int i = 0; i < count; i++) {
    buffer[i * 2] = body[i].first;
    buffer[i * 2 + 1] = body[i].second;
  }
  return count;
}

void game_get_apple(GameHandle g, int* x, int* y) {
  auto a = static_cast<Game*>(g)->getApple().getPosition();
  *x = a.first;
  *y = a.second;
}
