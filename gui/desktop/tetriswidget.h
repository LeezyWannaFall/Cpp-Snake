#pragma once
#include <QWidget>
#include <QTimer>
#include <QPushButton>
#include "../../brick_game/tetris/s21_tetris.h"

class TetrisWidget : public QWidget {
    Q_OBJECT
public:
    explicit TetrisWidget(QWidget *parent = nullptr);
    ~TetrisWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateGame();
    void restartGame();

private:
    QTimer *timer;
    int cellSize;
    QPushButton *restartButton;
    bool gameOver;
    void drawGameOverMenu(QPainter &painter);
    void drawNextBlock(QPainter &painter, int x, int y);
};
