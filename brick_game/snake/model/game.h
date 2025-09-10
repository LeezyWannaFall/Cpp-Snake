#ifndef GAME_H_
#define GAME_H_

#define LEN 4
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#include <utility>
#include <vector>

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum {
  STATE_START,
  STATE_SPAWN,
  STATE_MOVE,
  STATE_SHIFT,
  STATE_CONNECT,
  STATE_PAUSE,
  STATE_GAME_OVER
} GameState;

void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();

#endif  // GAME_H_