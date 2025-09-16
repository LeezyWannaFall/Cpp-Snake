#include "game.h"

s21::Game::Game() {
    Info.field[FIELD_WIDTH][FIELD_HEIGHT];
    Info.score = 0;
    Info.high_score = 0;
    Info.level = 0;
    State = STATE_START;
}

void s21::Game::ResetGame() {

}

void s21::Game::Update() {

}