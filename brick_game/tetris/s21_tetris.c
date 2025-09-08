#include "s21_tetris.h"

GameInfo_t game;
GameState state = STATE_START;
Tetromino current;
Tetromino newTetromino = {0};
Tetromino queue[QUEUE_SIZE];
unsigned long lastFallMs = 0;
int fallDelay = 1000;

/**
 Getter for current tetromino
*/
Tetromino getCurrentTetromino(void) { return current; }

/**
 Getter for new tetromino
*/
Tetromino getNewTetromino(void) { return newTetromino; }

/**
 Figures
*/
static const int O_BLOCK[4][4] = {
    {0, 0, 0, 0}, 
    {0, 1, 1, 0}, 
    {0, 1, 1, 0}, 
    {0, 0, 0, 0}};

static const int I_BLOCK[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1}, 
    {0, 0, 0, 0}, 
    {0, 0, 0, 0}};

static const int T_BLOCK[4][4] = {
    {0, 0, 0, 0}, 
    {0, 1, 0, 0}, 
    {1, 1, 1, 0}, 
    {0, 0, 0, 0}};

static const int L_BLOCK[4][4] = {
    {0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};

static const int J_BLOCK[4][4] = {
    {0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};

static const int S_BLOCK[4][4] = {
    {0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};

static const int Z_BLOCK[4][4] = {
    {0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};

/**
 Load highscore to highscore.txt
*/
int loadHighScore() {
  int highscore = 0;
  FILE *f = fopen("highscore.txt", "r");
  if (f) {
    fscanf(f, "%d", &highscore);
    fclose(f);
  }
  return highscore;
}

/**
 Save highscore to highscore.txt
*/
void saveHighScore(int highscore) {
  FILE *f = fopen("highscore.txt", "w");
  if (f) {
    fprintf(f, "%d\n", highscore);
    fclose(f);
  }
}

/**
 Copy from Figures to Tetromino
*/
void copyBlock(const int src[4][4], int dest[4][4]) {
  for (int i = 0; i < TETROMINO_SIZE; ++i)
    for (int j = 0; j < TETROMINO_SIZE; ++j) dest[i][j] = src[i][j];
}

/**
 Spawn figures and add them to queue
*/
void spawnNextTetromino() {
  for (int i = 0; i < QUEUE_SIZE - 1; ++i) {
    queue[i] = queue[i + 1];
  }

  int RandomValue = rand() % 7;
  if (RandomValue == 0) {
    copyBlock(O_BLOCK, newTetromino.shape);
  } else if (RandomValue == 1) {
    copyBlock(I_BLOCK, newTetromino.shape);
  } else if (RandomValue == 2) {
    copyBlock(T_BLOCK, newTetromino.shape);
  } else if (RandomValue == 3) {
    copyBlock(L_BLOCK, newTetromino.shape);
  } else if (RandomValue == 4) {
    copyBlock(J_BLOCK, newTetromino.shape);
  } else if (RandomValue == 5) {
    copyBlock(S_BLOCK, newTetromino.shape);
  } else {
    copyBlock(Z_BLOCK, newTetromino.shape);
  }

  newTetromino.x = 12;
  newTetromino.y = 10;
  queue[QUEUE_SIZE - 1] = newTetromino;
  state = STATE_SPAWN;
}

/**
 Spawn current tetromino from queue
*/
void spawnCurrentTetromino() {
  current = queue[0];
  current.x = 3;  ///< @brief центр по горизонтали
  current.y = 0;  ///< @brief верх
  state = STATE_MOVE;
}

/**
 Initialize the game field and next tetromino field
*/
void initField(void) {
  game.field = malloc(FIELD_HEIGHT * sizeof(int *));
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    game.field[i] = calloc(FIELD_WIDTH, sizeof(int));
  }

  game.score = 0;
  game.level = 1;
  game.speed = fallDelay;
  game.pause = 0;
  game.high_score = loadHighScore();

  game.next = malloc(TETROMINO_SIZE * sizeof(int *));
  for (int i = 0; i < TETROMINO_SIZE; ++i) {
    game.next[i] = calloc(TETROMINO_SIZE, sizeof(int));
  }

  for (int i = 0; i < QUEUE_SIZE; ++i) {
    spawnNextTetromino();
  }
}

/**
 Try to move the current tetromino left or right
*/
void tryMove(int dx) {
  int newX = current.x + dx;
  for (int i = 0; i < TETROMINO_SIZE; ++i) {
    for (int j = 0; j < TETROMINO_SIZE; ++j) {
      if (current.shape[i][j]) {
        int x = newX + j;
        int y = current.y + i;

        if (x < 0 || x >= FIELD_WIDTH || y < 0 || y >= FIELD_HEIGHT) return;
        if (game.field[y][x]) return;
      }
    }
  }

  current.x = newX;
}

/**
 Try to move the current tetromino down
*/
void tryMoveDown(void) {
  int newY = current.y + 1;
  for (int i = 0; i < TETROMINO_SIZE; ++i) {
    for (int j = 0; j < TETROMINO_SIZE; ++j) {
      if (current.shape[i][j]) {
        int x = current.x + j;
        int y = newY + i;

        if (y >= FIELD_HEIGHT || game.field[y][x]) {
          state = STATE_CONNECT;
          return;
        }
      }
    }
  }

  current.y = newY;
}

/**
 Place the current tetromino on the field
*/
void placeCurrentToField(void) {
  for (int i = 0; i < TETROMINO_SIZE; ++i) {
    for (int j = 0; j < TETROMINO_SIZE; ++j) {
      if (current.shape[i][j]) {
        int x = current.x + j;
        int y = current.y + i;

        if (y >= 0 && y < FIELD_HEIGHT && x >= 0 && x < FIELD_WIDTH)
          game.field[y][x] = 1;
      }
    }
  }
}

/**
 Clear filled lines and shift the rest down
*/
int clearLine(void) {
  int clearedLines = 0;

  for (int i = FIELD_HEIGHT - 1; i >= 0; i--) {
    bool full = true;
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      if (game.field[i][j] == 0) {
        full = false;
        break;
      }
    }

    if (full) {
      clearedLines++;
      for (int y = i; y > 0; y--) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
          game.field[y][x] = game.field[y - 1][x];
        }
      }

      for (int x = 0; x < FIELD_WIDTH; x++) {
        game.field[0][x] = 0;
      }
      i++;
    }
  }
  return clearedLines;
}

/**
 Update the score based on cleared lines
*/
void updateScore(int clearedLines) {
  if (clearedLines == 1)
    game.score += 100;
  else if (clearedLines == 2)
    game.score += 300;
  else if (clearedLines == 3)
    game.score += 700;
  else if (clearedLines == 4)
    game.score += 1500;
  if (game.score > game.high_score) {
    game.high_score = game.score;
    saveHighScore(game.high_score);
  }

  if (game.score >= game.level * 600 && game.level < 10) {
    game.level += 1;
    fallDelay -= 90;
    game.speed -= 90;
  }
}

/**
 Free allocated memory for the game field
*/
void freeField(void) {
  if (!game.field) return;

  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    if (game.field[i]) {
      free(game.field[i]);
      game.field[i] = NULL;
    }
  }
  free(game.field);
  game.field = NULL;
}

/**
 Free allocated memory for next tetromino field
*/
void freeNext(void) {
  if (!game.next) return;

  for (int i = 0; i < TETROMINO_SIZE; ++i) {
    if (game.next[i]) {
      free(game.next[i]);
      game.next[i] = NULL;
    }
  }
  free(game.next);
  game.next = NULL;
}

/**
 Rotate the tetromino 90 degrees clockwise
*/
void rotateTetromino(Tetromino *src, Tetromino *dest) {
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      dest->shape[i][j] = src->shape[TETROMINO_SIZE - 1 - j][i];
    }
  }
}

/**
 Try to rotate the current tetromino
*/
void tryRotate(void) {
  Tetromino tmp = {0};
  tmp.x = current.x;
  tmp.y = current.y;
  rotateTetromino(&current, &tmp);

  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      if (tmp.shape[i][j]) {
        int newX = tmp.x + j;
        int newY = tmp.y + i;

        if (newX < 0 || newX >= FIELD_WIDTH || newY < 0 || newY >= FIELD_HEIGHT)
          return;

        if (game.field[newY][newX]) return;
      }
    }
  }

  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      current.shape[i][j] = tmp.shape[i][j];
    }
  }
}

/**
 Get the current time in milliseconds
*/
unsigned long currentTimeMs() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (unsigned long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/**
 Update the game state based on the current state
*/
GameInfo_t updateCurrentState(void) {
  if (state == STATE_PAUSE) {
    return game;
  }

  switch (state) {
    case STATE_START:
      initField();
      state = STATE_SPAWN_NEXT_FIGURE;
      break;
    case STATE_SPAWN_NEXT_FIGURE:
      spawnNextTetromino();
      state = STATE_SPAWN;
      break;
    case STATE_SPAWN:
      spawnCurrentTetromino();
      state = STATE_MOVE;
      break;
    case STATE_MOVE: {
      unsigned long now = currentTimeMs();
      if (now - lastFallMs >= (unsigned long)fallDelay) {
        tryMoveDown();
        lastFallMs = now;
      }
    } break;
    case STATE_CONNECT:
      placeCurrentToField();
      updateScore(clearLine());  ///< @brief clear filled lines and update score

      for (int j = 0; j < FIELD_WIDTH; ++j) {
        if (game.field[1][j]) {
          state = STATE_GAME_OVER;
          return game;
        }
      }

      state = STATE_SPAWN_NEXT_FIGURE;
      break;
    case STATE_GAME_OVER:
      freeField();
      freeNext();
      fallDelay = 1000;
      game.pause = -1;  ///< @brief game over state
      break;
    default:
      break;
  }

  return game;
}

/**
 Handle user input actions
*/
void userInput(UserAction_t action, bool hold) {
  if (state == STATE_MOVE) {
    if (action == Left) {
      tryMove(-1);  ///< @brief left
    } else if (action == Right) {
      tryMove(1);  ///< @brief right
    } else if (action == Down) {
      tryMoveDown();  ///< @brief down
    } else if (action == Rotate) {
      tryRotate();  ///< @brief rotate
    }
  }

  game.pause = (state == STATE_PAUSE) ? 1 : 0;

  if (action == Pause) {
    if (state == STATE_MOVE || state == STATE_SPAWN) {
      state = STATE_PAUSE;
    } else if (state == STATE_PAUSE) {
      state = STATE_MOVE;  ///< @brief continue game
    }
  }

  if (action == Restart && state == STATE_GAME_OVER) {
    freeField();
    freeNext();
    state = STATE_START;
  }
}
