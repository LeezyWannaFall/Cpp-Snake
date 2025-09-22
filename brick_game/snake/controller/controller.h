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
  Game game;

 public:
  Controller();
  void userInput(UserAction_t action, bool hold);
  Game& getGame() { return game; }
};

}  // namespace s21

#endif  // CONTROLLER_H_