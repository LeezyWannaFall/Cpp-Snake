#include "../brick_game/snake/model/apple.h"
#include "../brick_game/snake/model/game.h"
#include "../brick_game/snake/model/snake.h"
#include "gtest/gtest.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// APPLE TESTS

TEST(AppleTest, PlaceApple) {
  s21::Apple apple;
  int** field = new int*[20];
  for (int i = 0; i < 20; ++i) {
    field[i] = new int[20];
    for (int j = 0; j < 20; ++j) {
      field[i][j] = 0;
    }
  }

  apple.getPosition();
  int x = apple.getX();
  int y = apple.getY();
  EXPECT_GE(x, 0);
  EXPECT_LT(x, 10);
  EXPECT_GE(y, 0);
  EXPECT_LT(y, 20);

  for (int i = 0; i < 20; ++i) {
    delete[] field[i];
  }
  delete[] field;
}

// SNAKE TESTS

TEST(SnakeTest, Move) {
  s21::Snake snake;
  auto initial_head = snake.getBody().front();
  snake.Move();
  auto new_head = snake.getBody().front();
  EXPECT_EQ(new_head.first, initial_head.first + 1);
  EXPECT_EQ(new_head.second, initial_head.second);  // Moving down
}

TEST(SnakeTest, ChangeDirection) {
  s21::Snake snake;
  snake.SetDirection(s21::Direction::LEFT);
  snake.Move();
  auto head = snake.getBody().front();
  EXPECT_EQ(head.first, 6);  // Moved left
  EXPECT_EQ(head.second, 10);
}

TEST(SnakeTest, Grow) {
  s21::Snake snake;
  int initial_size = snake.getBody().size();
  snake.Grow();
  EXPECT_EQ(snake.getBody().size(), initial_size + 1);
}

// GAME TESTS

TEST(GameTest, SaveHighScore) {
  s21::Game game;
  game.saveHighScore(100);
  int loaded_score = game.loadHighScore();
  EXPECT_EQ(loaded_score, 100);
}

TEST(GameTest, LoadHighScore) {
  s21::Game game;
  game.saveHighScore(200);
  int loaded_score = game.loadHighScore();
  EXPECT_EQ(loaded_score, 200);
}

TEST(GameTest, DirectionChange) {
  s21::Game game;
  game.SetDirection(s21::Direction::UP);
  auto snake = game.getSnake();
  EXPECT_EQ(snake.getBody().front().first, 5);
  EXPECT_EQ(snake.getBody().front().second, 10);
  game.SetDirection(s21::Direction::LEFT);
  snake = game.getSnake();
  EXPECT_EQ(snake.getBody().front().first, 5);
  EXPECT_EQ(snake.getBody().front().second, 10);
}

TEST(GameTest, ResetGame) {
  s21::Game game;
  game.SetDirection(s21::Direction::UP);
  game.ResetGame();
  EXPECT_EQ(game.getInfo().score, 0);
  EXPECT_EQ(game.getInfo().level, 1);
}

TEST(GameTest, UpdateState) {
  s21::Game game;
  game.setGameState(s21::STATE_START);
  game.updateCurrentState();
  EXPECT_EQ(game.getGameState(), s21::STATE_SPAWN + 1);
  game.updateCurrentState();
  EXPECT_EQ(game.getGameState(), s21::STATE_MOVE);
}

TEST(GameTest, HighScore) {
  s21::Game game;
  game.saveHighScore(150);
  int loaded_score = game.loadHighScore();
  EXPECT_EQ(loaded_score, 150);
}
