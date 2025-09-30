#include <check.h>
#include <stdlib.h>
#include <time.h>

#include "../brick_game/tetris/s21_tetris.h"

// Helper function to compare two 4x4 matrices
static int matricesEqual(int a[4][4], int b[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (a[i][j] != b[i][j]) return 0;
    }
  }
  return 1;
}

// Test for copyBlock
START_TEST(test_copyBlock) {
  int src[4][4] = {
      {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
  int dest[4][4] = {0};
  copyBlock(src, dest);
  ck_assert_int_eq(matricesEqual(src, dest), 1);
}
END_TEST

// Test for spawnNextTetromino (mock rand to control output)
START_TEST(test_spawnNextTetromino) {
  srand(0);  // Fixed seed for reproducibility
  spawnNextTetromino();
  Tetromino nt = getNewTetromino();
  // Depending on rand % 7 == 0, it should be O_BLOCK
  int expected[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  ck_assert_int_eq(matricesEqual(nt.shape, expected), 0);
  ck_assert_int_eq(nt.x, 12);
  ck_assert_int_eq(nt.y, 10);
}
END_TEST

// Test for spawnCurrentTetromino
START_TEST(test_spawnCurrentTetromino) {
  // Assume queue[0] is set
  int shape[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  copyBlock(shape, queue[0].shape);
  queue[0].x = 0;
  queue[0].y = 0;
  spawnCurrentTetromino();
  Tetromino ct = getCurrentTetromino();
  ck_assert_int_eq(matricesEqual(ct.shape, shape), 1);
  ck_assert_int_eq(ct.x, 3);
  ck_assert_int_eq(ct.y, 0);
}
END_TEST

// Test for initField
START_TEST(test_initField) {
  initField();
  ck_assert_ptr_nonnull(game.field);
  ck_assert_ptr_nonnull(game.next);
  ck_assert_int_eq(game.score, 0);
  ck_assert_int_eq(game.level, 1);
  ck_assert_int_eq(game.speed, 1000);
  ck_assert_int_ge(game.high_score, 0);  // Loaded value
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      ck_assert_int_eq(game.field[i][j], 0);
    }
  }
  freeField();
  freeNext();
}
END_TEST

// Test for tryMove (left/right)
START_TEST(test_tryMove) {
  initField();
  current.x = 3;
  current.y = 0;
  int shape[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  copyBlock(shape, current.shape);
  tryMove(-1);  // Left
  ck_assert_int_eq(current.x, 2);
  tryMove(1);  // Right back
  ck_assert_int_eq(current.x, 3);
  // Collision with field
  game.field[1][3] = 1;
  tryMove(1);
  ck_assert_int_eq(current.x, 4);  // No move if collision
  freeField();
  freeNext();
}
END_TEST

// Test for tryMoveDown
START_TEST(test_tryMoveDown) {
  initField();
  current.x = 3;
  current.y = 0;
  int shape[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  copyBlock(shape, current.shape);
  tryMoveDown();
  ck_assert_int_eq(current.y, 1);
  // Bottom edge
  current.y = FIELD_HEIGHT - 3;
  tryMoveDown();
  ck_assert_int_eq(state, STATE_CONNECT);  // Should trigger connect
  freeField();
  freeNext();
}
END_TEST

// Test for placeCurrentToField
START_TEST(test_placeCurrentToField) {
  initField();
  current.x = 3;
  current.y = 5;
  int shape[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  copyBlock(shape, current.shape);
  placeCurrentToField();
  ck_assert_int_eq(game.field[6][4], 1);
  ck_assert_int_eq(game.field[6][5], 1);
  ck_assert_int_eq(game.field[7][4], 1);
  ck_assert_int_eq(game.field[7][5], 1);
  freeField();
  freeNext();
}
END_TEST

// Test for clearLine
START_TEST(test_clearLine) {
  initField();
  // Fill a line
  for (int j = 0; j < FIELD_WIDTH; j++) {
    game.field[19][j] = 1;
  }
  int cleared = clearLine();
  ck_assert_int_eq(cleared, 1);
  for (int j = 0; j < FIELD_WIDTH; j++) {
    ck_assert_int_eq(game.field[19][j], 0);
  }
  // Multiple lines
  for (int i = 18; i <= 19; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game.field[i][j] = 1;
    }
  }
  cleared = clearLine();
  ck_assert_int_eq(cleared, 2);
  freeField();
  freeNext();
}
END_TEST

// Test for updateScore
START_TEST(test_updateScore) {
  game.score = 0;
  game.level = 1;
  game.high_score = 0;
  fallDelay = 1000;
  updateScore(1);
  ck_assert_int_eq(game.score, 100);
  updateScore(2);
  ck_assert_int_eq(game.score, 400);
  updateScore(3);
  ck_assert_int_eq(game.score, 1100);
  updateScore(4);
  ck_assert_int_eq(game.score, 2600);
  // Level up
  game.score = 500;
  game.level = 1;
  fallDelay = 1000;
  updateScore(1);  // 600 -> level 2
  ck_assert_int_eq(game.level, 2);
  ck_assert_int_eq(fallDelay, 910);
  // High score update
  ck_assert_int_eq(game.high_score, 2600);
}
END_TEST

// Test for freeField and freeNext
START_TEST(test_freeField) {
  initField();
  freeField();
  ck_assert_ptr_null(game.field);
  freeNext();
  ck_assert_ptr_null(game.next);
}
END_TEST

// Test for rotateTetromino
START_TEST(test_rotateTetromino) {
  Tetromino src, dest;
  int shape[4][4] = {
      {0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};  // T_BLOCK
  copyBlock(shape, src.shape);
  rotateTetromino(&src, &dest);
  int expected[4][4] = {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}};
  ck_assert_int_eq(matricesEqual(dest.shape, expected), 1);
}
END_TEST

// Test for tryRotate
START_TEST(test_tryRotate) {
  initField();
  current.x = 3;
  current.y = 0;
  int shape[4][4] = {
      {0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};  // T_BLOCK
  copyBlock(shape, current.shape);
  tryRotate();
  int expected[4][4] = {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}};
  ck_assert_int_eq(matricesEqual(current.shape, expected), 1);
  // Collision test
  game.field[1][4] = 1;  // Block rotation
  tryRotate();
  ck_assert_int_eq(matricesEqual(current.shape, expected), 1);  // No change
  freeField();
  freeNext();
}
END_TEST

// Test for updateCurrentState (basic flow)
START_TEST(test_updateCurrentState) {
  state = STATE_START;
  updateCurrentState();
  ck_assert_int_eq(state, STATE_SPAWN_NEXT_FIGURE);
  updateCurrentState();
  ck_assert_int_eq(state, STATE_SPAWN);
  updateCurrentState();
  ck_assert_int_eq(state, STATE_MOVE);
  // Simulate time for down move
  lastFallMs = currentTimeMs() - 1000;
  updateCurrentState();
  ck_assert_int_ge(current.y, 0);
  freeField();
  freeNext();
}
END_TEST

// Test for userInput
START_TEST(test_userInput) {
  state = STATE_MOVE;
  userInput(Left, false);
  ck_assert_int_eq(current.x, -1);  // Assuming initial x=3
  userInput(Pause, false);
  ck_assert_int_eq(state, STATE_PAUSE);
  userInput(Pause, false);
  ck_assert_int_eq(state, STATE_MOVE);
  state = STATE_GAME_OVER;
  userInput(Restart, false);
  ck_assert_int_eq(state, STATE_START);
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s = suite_create("Tetris");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_copyBlock);
  tcase_add_test(tc, test_spawnNextTetromino);
  tcase_add_test(tc, test_spawnCurrentTetromino);
  tcase_add_test(tc, test_initField);
  tcase_add_test(tc, test_tryMove);
  tcase_add_test(tc, test_tryMoveDown);
  tcase_add_test(tc, test_placeCurrentToField);
  tcase_add_test(tc, test_clearLine);
  tcase_add_test(tc, test_updateScore);
  tcase_add_test(tc, test_freeField);
  tcase_add_test(tc, test_rotateTetromino);
  tcase_add_test(tc, test_tryRotate);
  tcase_add_test(tc, test_updateCurrentState);
  tcase_add_test(tc, test_userInput);

  suite_add_tcase(s, tc);
  return s;
}

int main(void) {
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int nf = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}