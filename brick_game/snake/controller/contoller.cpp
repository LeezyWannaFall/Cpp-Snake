#include "controller.h"

s21::Controller::Controller() : State(STATE_START), game() {}

void s21::Controller::userInput(UserAction_t action, bool hold) {
  if (State == STATE_MOVE) {
    bool ateApple = false;
    
    switch (action) {
      case Left:
        game.SetDirection(Direction::LEFT);
        break;
      case Right:
        game.SetDirection(Direction::RIGHT);
        break;
      case Up:
        game.SetDirection(Direction::UP);
        break;
      case Down:
        game.SetDirection(Direction::DOWN);
        break;
      case Pause:
        State = STATE_PAUSE;
        if (State == STATE_PAUSE) {
          State = STATE_MOVE;
        }
        break;
      default: 
        break;
    }
  }
}