#ifndef GAME_H_
#define GAME_H_

#define LEN 4
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#include <utility>
#include <vector>
#include <random>
#include "snake.h"
#include "apple.h"
#include "../controller/controller.h"

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

  void userInput(UserAction_t action, bool hold);
  GameInfo_t updateCurrentState();
  void ResetGame();
  void Update();
  void handleInput(UserAction_t);
  GameInfo_t getInfo() const;

 private:
  Snake snake;
  Apple apple;
  GameInfo_t Info;
  GameState State;
  UserAction_t Action;
};

} // namespace s21

#endif  // GAME_H_