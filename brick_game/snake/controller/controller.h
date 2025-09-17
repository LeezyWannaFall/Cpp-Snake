#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "../model/game.h"

namespace s21 {

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

class Controller {
 private:
  // UserAction_t action;
  GameState State;
  Game game;

 public:
  Controller(/* args */);

  void userInput(UserAction_t action, bool hold);
  GameInfo_t updateCurrentState();
};

Controller::Controller(/* args */) {}

Controller::~Controller() {}

}  // namespace s21

#endif  // CONTROLLER_H_