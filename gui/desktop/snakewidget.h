#pragma once
#include <QPushButton>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/snake/controller/controller.h"

class SnakeWidget : public QWidget {
  Q_OBJECT
 public:
  explicit SnakeWidget(QWidget *parent = nullptr);
  ~SnakeWidget();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

 private slots:
  void updateGame();
  void restartGame();

 private:
  s21::Controller controller;
  QTimer *timer;
  int cellSize;
  QPushButton *restartButton;
  bool gameOver;
  void drawGameOverMenu(QPainter &painter);
};
