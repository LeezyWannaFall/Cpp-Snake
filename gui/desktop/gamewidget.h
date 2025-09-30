#pragma once
#include <QWidget>
#include <QTimer>
#include <QPushButton>
#include <QStackedLayout>
#include "tetriswidget.h"
#include "snakewidget.h"

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);

private slots:
    void showTetris();
    void showSnake();
    void showMenu();

private:
    enum Mode { MENU, TETRIS, SNAKE } mode;
    QPushButton *tetrisButton;
    QPushButton *snakeButton;
    TetrisWidget *tetrisWidget;
    SnakeWidget *snakeWidget;
    QStackedLayout *stackedLayout;
    QWidget *menuWidget;
};
