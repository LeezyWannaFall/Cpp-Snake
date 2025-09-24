#include "game.h"

s21::Game::Game() {
  Info.field = new int*[FIELD_HEIGHT];
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    Info.field[i] = new int[FIELD_WIDTH]();
  }
}

s21::Game::~Game() {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    delete[] Info.field[i];
  }
  delete[] Info.field;
}

void s21::Game::SetDirection(Direction NewDir) { snake.SetDirection(NewDir); }

void s21::Game::ResetGame() {
  Info.score = 0;
  Info.level = 0;
  Info.speed = 0;
  Info.pause = 0;

  delayMs = 500;
  lastUpdate = std::chrono::steady_clock::now();
}

s21::GameInfo_t s21::Game::updateCurrentState() {
  if (State == STATE_PAUSE) {
    return Info;
  }

  auto now = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate).count();

  switch (State) {
    case STATE_START:
      ResetGame();
      State = STATE_MOVE;
      break;
    case STATE_MOVE:
      if (elapsed >= delayMs) {
        snake.Move();
        lastUpdate = now;

        if (snake.getHead() == apple.getPosition()) {
          snake.Grow();
          apple.Respawn();
          Info.score += 10;
        }

        if (!snake.isAlive()) {
          State = STATE_GAME_OVER;
        }

      }
      break;
    case STATE_PAUSE:
      Info.pause = 1;
      break;
    case STATE_GAME_OVER:
      Info.pause = -1;
      break;
    default:
      break;
  }
  
  // чистим поле
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      Info.field[y][x] = 0;
    }
  }

  // ставим змейку
  for (auto &part : snake.getBody()) {
    Info.field[part.second][part.first] = 1; // 1 = змейка
  }

  // ставим яблоко
  Info.field[apple.getY()][apple.getX()] = 2; // 2 = яблоко

  return Info;
}
