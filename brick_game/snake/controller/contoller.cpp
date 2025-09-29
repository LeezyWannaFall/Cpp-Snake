#include "controller.h"

s21::Controller::Controller() : game(new Game()) {}

s21::Controller::~Controller() { delete game; }

void s21::Controller::userInput(UserAction_t action, bool hold) {
  switch (action) {
    case Left:
      if (game->getGameState() == STATE_MOVE)
        game->SetDirection(Direction::LEFT);
      break;
    case Right:
      if (game->getGameState() == STATE_MOVE)
        game->SetDirection(Direction::RIGHT);
      break;
    case Up:
      if (game->getGameState() == STATE_MOVE)
        game->SetDirection(Direction::UP);
      break;
    case Down:
      if (game->getGameState() == STATE_MOVE)
        game->SetDirection(Direction::DOWN);
      break;
    case Pause:
      if (game->getGameState() == STATE_MOVE) {
        game->setGameState(STATE_PAUSE);
      } else if (game->getGameState() == STATE_PAUSE) {
        game->setGameState(STATE_MOVE);
      }
      break;
    case Terminate:
      game->setGameState(STATE_GAME_OVER);
      break;
    case Restart:
      game->ResetGame();
      game->setGameState(STATE_START);
      game->updateCurrentState();
      break;
    default:
      break;
  }
}