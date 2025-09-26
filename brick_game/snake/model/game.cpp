#include "game.h"

s21::Game::Game() {
  Info.field = new int *[FIELD_HEIGHT];
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

void s21::Game::saveHighScore(int high_score) {
  FILE *f = fopen("highscore_snake.txt", "w");
  if (f) {
    fprintf(f, "%d\n", high_score);
    fclose(f);
  }
}

int s21::Game::loadHighScore() {
  int highscore = 0;
  FILE *f = fopen("highscore_snake.txt", "r");
  if (f) {
    fscanf(f, "%d", &highscore);
    fclose(f);
  }
  return highscore;
}

void s21::Game::ResetGame() {
  Info.score = 0;
  Info.level = 1;
  Info.speed = 0;
  Info.pause = 0;
  Info.high_score = loadHighScore();

  delayMs = 500;
  lastUpdate = std::chrono::steady_clock::now();
  snake = Snake();
  apple = Apple();
}

s21::GameInfo_t s21::Game::updateCurrentState() {
  if (State == STATE_PAUSE) {
    return Info;
  }

  auto now = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate)
          .count();

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
          apple.Respawn(snake.getBody());
          Info.score += 1;
          if (Info.score > Info.high_score) {
            Info.high_score = Info.score;
            saveHighScore(Info.high_score);
          }
        }

        if (!snake.isAlive()) {
          State = STATE_GAME_OVER;
        }

        if ((Info.score >= 5 * Info.level) && Info.level < 10) {
          Info.level += 1;
          delayMs = std::max(100, delayMs - 50);  // Уменьшаем задержку, но не менее 100 мс
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
    Info.field[part.second][part.first] = 1;  // 1 = змейка
  }

  // ставим яблоко
  Info.field[apple.getY()][apple.getX()] = 2;  // 2 = яблоко

  return Info;
}
