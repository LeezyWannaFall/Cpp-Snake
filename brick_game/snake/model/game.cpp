#include "game.h"

s21::Game::Game() {
    Info.field = new int* [FIELD_HEIGHT];
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        Info.field[i] = new int[FIELD_WIDTH]();
    }

    Info.score = 0;
    Info.high_score = 0;
    Info.level = 0;
    State = STATE_START;
}

void s21::Game::ResetGame() {

}

void s21::Game::Update() {

}