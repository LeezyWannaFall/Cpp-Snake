#include "controller.h"

s21::Controller::Controller() : State(STATE_START), game() {}

void s21::Controller::userInput(UserAction_t action, bool hold) {
  if (game.getGameState() == STATE_MOVE) {
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
        if (game.getGameState() == STATE_MOVE) {
          game.setGameState(STATE_PAUSE);
        } else if (game.getGameState() == STATE_PAUSE) {
          game.setGameState(STATE_MOVE);
        }
        break;
      default: 
        break;
    }
  }
}