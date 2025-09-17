#ifndef GAME_H_
#define GAME_H_

#define LEN 4
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#include <random>
#include <utility>
#include <vector>

#include "../controller/controller.h"
#include "apple.h"
#include "snake.h"

namespace s21 {

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

class Game {
 public:
  Game();
  ~Game();

  void SetDirection(Direction NewDir);  // Snake method for class Game
  void ResetGame();
  void UpdateTimer();
  void handleInput(UserAction_t);
  void PauseGame();
  GameInfo_t updateCurrentState();

  // GameInfo_t getInfo() const { return Info; }

 private:
  Snake snake;
  Apple apple;
  GameInfo_t Info;
  GameState State;
  UserAction_t Action;
};

}  // namespace s21

#endif  // GAME_H_