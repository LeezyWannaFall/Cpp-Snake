#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "../../brick_game/snake/controller/wrapper.h"
#include "../../brick_game/tetris/s21_tetris.h"

void drawTetris(const GameInfo_t game, const Tetromino current,
                const Tetromino newTetromino);
void drawGameMenu();
void drawSnake(GameHandle snake_game);

typedef enum { MENU, GAME_TETRIS, GAME_SNAKE } GameMode;

int main() {
  srand(time(NULL));

  initscr();  // инициализация ncurses
  noecho();   // не показывать нажатия клавиш
  cbreak();   // не ждать Enter
  keypad(stdscr, TRUE);   // включить стрелки
  nodelay(stdscr, TRUE);  // getch() не блокирует
  curs_set(FALSE);        // скрыть курсор

  GameInfo_t game = updateCurrentState();  // START → SPAWN
  GameMode mode = MENU;
  GameHandle snake_game = NULL;

  bool in_tetris = false;

  while (1) {
    Tetromino newTetromino = getNewTetromino();
    Tetromino current = getCurrentTetromino();
    int ch = getch();
    switch (ch) {
      case KEY_LEFT:
        if (mode == GAME_TETRIS) {
            userInput(Left, false);
        } else if (mode == GAME_SNAKE && snake_game) {
            game_set_direction(snake_game, 2); // 2 = LEFT
        }
        break;
      case KEY_RIGHT:
        if (mode == GAME_TETRIS) {
            userInput(Right, false);
        } else if (mode == GAME_SNAKE && snake_game) {
            game_set_direction(snake_game, 3); // 3 = RIGHT
        }
        break;
      case KEY_DOWN:
        if (mode == GAME_TETRIS) {
            userInput(Down, false);
        } else if (mode == GAME_SNAKE && snake_game) {
            game_set_direction(snake_game, 1); // 1 = DOWN
        }
        break;
      case KEY_UP:
        if (mode == GAME_TETRIS) {
            userInput(Rotate, false);
        } else if (mode == GAME_SNAKE && snake_game) {
            game_set_direction(snake_game, 0); // 0 = UP
        }
        break;
      case 'Q':
      case 'q':
        if (mode == GAME_TETRIS) {
          freeField();
          freeNext();
          endwin();
          printf("Exiting...\n");
          return 0;
        } else {
          endwin();
          printf("Exiting...\n");
          return 0;
        }
        break;
      case 'R':
      case 'r':
        if (mode == GAME_SNAKE) {
          game_restart(snake_game);
        } else if (mode == GAME_TETRIS) {
          userInput(Restart, false); // старая логика для тетриса
        }
        break;
      case 'P':
      case 'p':
        if (mode == GAME_TETRIS)
          userInput(Pause, false);
        else if (mode == GAME_SNAKE && snake_game)
          game_toggle_pause(snake_game); // 4 = STATE_PAUSE
        break;
      case 'T':
      case 't':
        mode = GAME_TETRIS;
        in_tetris = true;
        break;
      case 'S':
      case 's':
        mode = GAME_SNAKE;
        in_tetris = false;
        if (!snake_game) snake_game = game_create();
        break;
    }

    if (mode == MENU) {
      drawGameMenu();
      usleep(100000);  // 0.1 секунды паузы
      continue;        // не делаем больше ничего
    }

    if (mode == GAME_TETRIS && !in_tetris) {
      // переключаемся в тетрис
      in_tetris = true;
      freeField();
      freeNext();
      game = updateCurrentState();  // START → SPAWN
    } else if (mode == GAME_SNAKE && in_tetris) {
      // переключаемся в змейку
      in_tetris = false;
      game_restart(snake_game); // START → SPAWN
    }

    if (in_tetris) {
      game = updateCurrentState();
    } else if (snake_game) {
      game_update(snake_game);
      // Получаем поле и инфу для отрисовки
      game.score = game_get_score(snake_game);
      // ... (можно добавить high_score, level и т.д. если нужно)
      // Для drawSnake(game) достаточно game.field, который нужно получить из
      // snake_game Но в вашем wrapper нет функции для получения field напрямую,
      // поэтому используйте game_get_snake_body и game_get_apple для отрисовки
      // змейки и яблока
    }

    if (game.pause == -1 || (snake_game && game_get_pause(snake_game) == -1)) {
      clear();
      mvprintw(FIELD_HEIGHT / 2, FIELD_WIDTH,
               "GAME OVER\n      Your score: %d\n      Your highscore: %d\n    "
               "  Press 'q' to exit.\n      Press 'r' to restart.",
               game.score, game.high_score);
      refresh();
      usleep(500000);  // полсекунды паузы
      continue;        // не делаем больше ничего
    }

    if (in_tetris) {
      clear();
      drawTetris(game, current, newTetromino);
      refresh();
    } else if (snake_game) {
      drawSnake(snake_game);
    }

    usleep(DELAY);
  }

  endwin();
  return 0;
}

void drawGameMenu() {
  clear();
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      mvprintw(FIELD_HEIGHT / 2, FIELD_WIDTH,
               "CHOOSE GAME\n      Tetris - Press T to play\n      Snake - "
               "Press S to play\n");
    }
  }
}

void drawSnake(GameHandle snake_game) {
  clear();

  int snake_buf[FIELD_WIDTH * FIELD_HEIGHT * 2];
  int snake_len = game_get_snake_body(snake_game, snake_buf,
                                      sizeof(snake_buf) / sizeof(int));
  int apple_x, apple_y;
  game_get_apple(snake_game, &apple_x, &apple_y);

  // рисуем поле
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      bool is_snake = false;
      for (int i = 0; i < snake_len; i++) {
        if (snake_buf[i * 2] == x && snake_buf[i * 2 + 1] == y) {
          is_snake = true;
          break;
        }
      }
      if (is_snake) {
        mvprintw(y, x * 2, "[]");  // тело змейки
      } else if (apple_x == x && apple_y == y) {
        mvprintw(y, x * 2, " @");  // яблоко
      } else {
        mvprintw(y, x * 2, " .");  // пусто
      }
    }
  }

  // справа пишем инфу
  int info_x = FIELD_WIDTH * 2 + 4;
  mvprintw(1, info_x, "=== SNAKE ===");
  mvprintw(2, info_x, "Score:      %d", game_get_score(snake_game));
  mvprintw(3, info_x, "High Score: %d", game_get_high_score(snake_game));
  mvprintw(4, info_x, "Level:      %d", game_get_level(snake_game));

  // if (game_get_state(snake_game) == STATE_PAUSE) mvprintw(6, info_x, "=== PAUSED ===");

  refresh();
}

void drawTetris(const GameInfo_t game, const Tetromino current,
                const Tetromino newTetromino) {
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      if (game.field[y][x])
        mvprintw(y, x * 2, "[]");
      else
        mvprintw(y, x * 2, " .");
    }
  }

  // Информация справа
  int info_x = FIELD_WIDTH * 2 + 4;  // правее от поля начиная с 20+4 клетки
  int info_y = 2;

  // Рисуем next фигуру справа
  mvprintw(info_y + 7, info_x, "Next:");
  mvprintw(info_y - 1, info_x, "=== TETRIS ===");
  mvprintw(info_y + 0, info_x, "Score:      %d", game.score);
  mvprintw(info_y + 1, info_x, "High Score: %d", game.high_score);
  mvprintw(info_y + 2, info_x, "Level:      %d", game.level);
  mvprintw(info_y + 3, info_x, "Speed:      %d", game.speed);

  if (game.pause) mvprintw(info_y + 5, info_x, "=== PAUSED ===");

  // текущая фигура
  for (int i = 0; i < TETROMINO_SIZE; ++i) {
    for (int j = 0; j < TETROMINO_SIZE; ++j) {
      if (current.shape[i][j]) {
        int y = current.y + i;
        int x = current.x + j;
        if (y >= 0 && y < FIELD_HEIGHT && x >= 0 && x < FIELD_WIDTH)
          mvprintw(y, x * 2, "[]");
      }
    }
  }

  // следуящая фигура
  for (int i = 0; i < TETROMINO_SIZE; ++i) {
    for (int j = 0; j < TETROMINO_SIZE; ++j) {
      if (newTetromino.shape[i][j]) {
        int y = newTetromino.y + i;
        int x = newTetromino.x + j;
        mvprintw(y, x * 2, "[]");
      }
    }
  }
}
