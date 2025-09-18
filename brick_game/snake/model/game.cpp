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
  Info.pause = 0;
}

void s21::Game::UpdateTimer() {
    
}

void s21::Game::handleInput(UserAction_t) {}

void s21::Game::PauseGame() {}

s21::GameInfo_t s21::Game::updateCurrentState() {
  if (State == STATE_PAUSE) {
    return Info;
  }

  switch (State) {
    case STATE_START:
      Game::ResetGame();
      State = STATE_SPAWN;
      break;
    case STATE_MOVE:
      bool ateApple = false;
      snake.Move(ateApple);
      if (snake.checkCollision(FIELD_WIDTH, FIELD_HEIGHT)) {
        State = STATE_GAME_OVER;
      }
      break;
    case STATE_GAME_OVER:
      break;
    default:
      break;
  }
  return Info;
}
