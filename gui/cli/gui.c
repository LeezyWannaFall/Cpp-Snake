#include <ncurses.h>
#include <stdlib.h>
#include <time.h>


#include "../../brick_game/tetris/s21_tetris.h"

void drawTetris(const GameInfo_t game, const Tetromino current,
              const Tetromino newTetromino);
int drawGameMenu();

int main() {
  srand(time(NULL));

  initscr();  // инициализация ncurses
  noecho();   // не показывать нажатия клавиш
  cbreak();   // не ждать Enter
  keypad(stdscr, TRUE);   // включить стрелки
  nodelay(stdscr, TRUE);  // getch() не блокирует
  curs_set(FALSE);        // скрыть курсор

  GameInfo_t game = updateCurrentState();  // START → SPAWN
  drawGameMenu();

  bool in_tetris = false;
  
  while (1) {
    Tetromino newTetromino = getNewTetromino();
    Tetromino current = getCurrentTetromino();
    int ch = getch();
    switch (ch) {
      case KEY_LEFT:
        userInput(Left, false);
        break;
      case KEY_RIGHT:
        userInput(Right, false);
        break;
      case KEY_DOWN:
        userInput(Down, false);
        break;
      case 'W':
      case 'w':
        userInput(Rotate, false);
        break;
      case 'Q':
      case 'q':
        freeField();
        freeNext();
        endwin();
        printf("Exiting...\n");
        return 0;
      case 'R':
      case 'r':
        userInput(Restart, false);
        break;
      case 'P':
      case 'p':
        userInput(Pause, false);
      case 'T':
      case 't':
        in_tetris = true;
        break;
      case 'S':
      case 's':
      in_tetris = false;
        // chose game: Snake
        break;
    }

    game = updateCurrentState();

    if (game.pause == -1) {
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
    }
    usleep(DELAY);
  }

  endwin();
  return 0;
}

int drawGameMenu() {
  clear();
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      mvprintw(FIELD_HEIGHT / 2, FIELD_WIDTH, "CHOOSE GAME\n      Tetris - Press T to play\n      Snake - Press S to play\n");
    }
  }
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
