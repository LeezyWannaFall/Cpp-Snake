#include "gamewidget.h"
#include "snakewidget.h"
#include "tetriswidget.h"
#include <QStackedLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), stackedLayout(new QStackedLayout(this)), menuWidget(new QWidget(this)), snakeWidget(new SnakeWidget(this)), tetrisWidget(new TetrisWidget(this)) {
    // Menu UI
    QVBoxLayout *menuLayout = new QVBoxLayout(menuWidget);
    QLabel *title = new QLabel("<h1>Brick Game</h1>", menuWidget);
    title->setAlignment(Qt::AlignCenter);
    QPushButton *snakeBtn = new QPushButton("Play Snake", menuWidget);
    QPushButton *tetrisBtn = new QPushButton("Play Tetris", menuWidget);
    menuLayout->addWidget(title);
    menuLayout->addWidget(snakeBtn);
    menuLayout->addWidget(tetrisBtn);
    menuLayout->addStretch();
    connect(snakeBtn, &QPushButton::clicked, this, &GameWidget::showSnake);
    connect(tetrisBtn, &QPushButton::clicked, this, &GameWidget::showTetris);

    // Add widgets to stacked layout
    stackedLayout->addWidget(menuWidget);      // index 0
    stackedLayout->addWidget(snakeWidget);     // index 1
    stackedLayout->addWidget(tetrisWidget);    // index 2
    setLayout(stackedLayout);

    // Back to menu buttons
    QPushButton *backBtnSnake = new QPushButton("Menu", snakeWidget);
    backBtnSnake->setGeometry(320, 10, 60, 32);
    connect(backBtnSnake, &QPushButton::clicked, this, &GameWidget::showMenu);
    QPushButton *backBtnTetris = new QPushButton("Menu", tetrisWidget);
    backBtnTetris->setGeometry(320, 10, 60, 32);
    connect(backBtnTetris, &QPushButton::clicked, this, &GameWidget::showMenu);
}

void GameWidget::showMenu() {
    stackedLayout->setCurrentIndex(0);
}

void GameWidget::showSnake() {
    // Сбросить состояние игры змейки при выборе из меню
    snakeWidget->resetGameState();
    stackedLayout->setCurrentIndex(1);
}

void GameWidget::showTetris() {
    stackedLayout->setCurrentIndex(2);
    tetrisWidget->setFocus();
}
