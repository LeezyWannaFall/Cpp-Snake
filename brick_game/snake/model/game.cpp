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
  Info.high_score = 0;
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
        bool ateApple = false;
        snake.Move(ateApple);

        if (apple.IsEaten()) {
          apple.Respawn();
        }

        if (snake.checkCollision(FIELD_WIDTH, FIELD_HEIGHT)) {
          State = STATE_GAME_OVER;
        }
        
        lastUpdate = now;
      }
      break;
    case STATE_GAME_OVER:
      Info.pause = -1;
      break;
    default:
      break;
  }
  return Info;
}
