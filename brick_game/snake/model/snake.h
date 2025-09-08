#ifndef SNAKE_H_
#define SNAKE_H_
#define LEN 

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

namespace s21 {
class snake {
 private:
  typedef struct snakeMatrix {
    int snake[LEN];
  };
 public:
  snake();
  ~snake();
  
}

snake::snake() {

}

snake::~snake() {

}

} // namespace s21


#endif // SNAKE_H_