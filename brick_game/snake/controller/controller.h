#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "../model/game.h"
#include "../common/user_action.h"

namespace s21 {

class Game;

class Controller {
 private:
  Game* game;

 public:
  Controller();
  ~Controller();
  void userInput(UserAction_t action, bool hold);
  Game& getGame() { return *game; }
};

}  // namespace s21

#endif  // CONTROLLER_H_