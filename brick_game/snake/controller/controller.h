#ifndef CONTROLLER_H_
#define CONTROLLER_H_

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
    UserAction_t Action;
 public:
    Controller(/* args */);
    ~Controller();

  void userInput(UserAction_t action, bool hold);
};

Controller::Controller(/* args */) {

}

Controller::~Controller() {

}


#endif // CONTROLLER_H_