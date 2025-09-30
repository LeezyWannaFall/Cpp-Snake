
#include "tetriswidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QFont>
#include <QLabel>

extern "C" {
#include "../../brick_game/tetris/s21_tetris.h"
}

TetrisWidget::TetrisWidget(QWidget *parent)
    : QWidget(parent), timer(new QTimer(this)), cellSize(24), gameOver(false) {
    setFocusPolicy(Qt::StrongFocus);
    setFixedSize(FIELD_WIDTH * cellSize + 140, FIELD_HEIGHT * cellSize);
    initField();
    connect(timer, &QTimer::timeout, this, &TetrisWidget::updateGame);
    timer->start(120);
    restartButton = new QPushButton("Restart", this);
    restartButton->setGeometry(FIELD_WIDTH * cellSize + 20, 420, 100, 32);
    connect(restartButton, &QPushButton::clicked, this, &TetrisWidget::restartGame);
}

TetrisWidget::~TetrisWidget() {
    freeField();
    freeNext();
}

void TetrisWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw field
    for (int y = 0; y < FIELD_HEIGHT; ++y) {
        for (int x = 0; x < FIELD_WIDTH; ++x) {
            int val = game.field[y][x];
            if (val) {
                painter.setBrush(QColor::fromHsv((val * 40) % 360, 255, 200));
                painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
            } else {
                painter.setBrush(Qt::black);
                painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
            }
        }
    }

    // Draw current tetromino
    Tetromino cur = getCurrentTetromino();
    for (int i = 0; i < TETROMINO_SIZE; ++i) {
        for (int j = 0; j < TETROMINO_SIZE; ++j) {
            if (cur.shape[i][j]) {
                int bx = cur.x + j;
                int by = cur.y + i;
                if (by >= 0 && bx >= 0 && bx < FIELD_WIDTH && by < FIELD_HEIGHT) {
                    painter.setBrush(QColor::fromHsv(200, 255, 255));
                    painter.drawRect(bx * cellSize, by * cellSize, cellSize, cellSize);
                }
            }
        }
    }

    // Draw info
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(FIELD_WIDTH * cellSize + 20, 30, QString("Score: %1").arg(game.score));
    painter.drawText(FIELD_WIDTH * cellSize + 20, 60, QString("Level: %1").arg(game.level));
    painter.drawText(FIELD_WIDTH * cellSize + 20, 90, QString("Highscore: %1").arg(game.high_score));
    painter.drawText(FIELD_WIDTH * cellSize + 20, 130, "Next:");
    drawNextBlock(painter, FIELD_WIDTH * cellSize + 20, 150);

    if (state == STATE_GAME_OVER) {
        drawGameOverMenu(painter);
    }
}

void TetrisWidget::drawNextBlock(QPainter &painter, int x, int y) {
    Tetromino next = getNewTetromino();
    painter.setBrush(QColor::fromHsv(120, 255, 255));
    for (int i = 0; i < TETROMINO_SIZE; ++i) {
        for (int j = 0; j < TETROMINO_SIZE; ++j) {
            if (next.shape[i][j]) {
                painter.drawRect(x + j * cellSize, y + i * cellSize, cellSize, cellSize);
            }
        }
    }
}

void TetrisWidget::drawGameOverMenu(QPainter &painter) {
    painter.setPen(Qt::red);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, "Game Over\nPress Restart");
}

void TetrisWidget::updateGame() {
    if (state != STATE_GAME_OVER) {
        updateCurrentState();
        update();
    }
}

void TetrisWidget::restartGame() {
    freeField();
    freeNext();
    initField();
    state = STATE_START;
    gameOver = false;
    timer->start(120);
    update();
}

void TetrisWidget::keyPressEvent(QKeyEvent *event) {
    if (state == STATE_GAME_OVER) return;
    switch (event->key()) {
    case Qt::Key_Left:
        userInput(Left, false);
        break;
    case Qt::Key_Right:
        userInput(Right, false);
        break;
    case Qt::Key_Up:
        userInput(Rotate, false);
        break;
    case Qt::Key_Down:
        userInput(Down, false);
        break;
    case Qt::Key_Space:
        // Быстрое падение (несколько раз вниз)
        for (int i = 0; i < FIELD_HEIGHT; ++i) userInput(Down, false);
        break;
    case Qt::Key_P:
        userInput(Pause, false);
        break;
    }
    update();
}

void TetrisWidget::keyReleaseEvent(QKeyEvent *event) {
    QWidget::keyReleaseEvent(event);
}
