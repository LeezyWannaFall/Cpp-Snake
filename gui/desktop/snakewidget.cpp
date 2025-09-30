#include "snakewidget.h"
#include <QFont>
#include <QKeyEvent>
#include <QPainter>

SnakeWidget::SnakeWidget(QWidget *parent)
    : QWidget(parent),
      timer(new QTimer(this)),
      cellSize(24),
      restartButton(nullptr),
      gameOver(false) {
  setFixedSize(FIELD_WIDTH * cellSize + 140, FIELD_HEIGHT * cellSize);
  setFocusPolicy(Qt::StrongFocus);
  setFocus();
  controller.userInput(s21::Restart, false);
  timer->setInterval(100);
  connect(timer, &QTimer::timeout, this, &SnakeWidget::updateGame);
  timer->start();
}

SnakeWidget::~SnakeWidget() {
  if (restartButton) delete restartButton;
}

void SnakeWidget::updateGame() {
  controller.getGame().updateCurrentState();
  gameOver = (controller.getGame().getGameState() == s21::STATE_GAME_OVER);
  update();
}

void SnakeWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  auto info = controller.getGame().getInfo();

  if (gameOver) {
    drawGameOverMenu(painter);
    return;
  }

  // поле
  painter.setBrush(Qt::black);
  painter.drawRect(0, 0, FIELD_WIDTH * cellSize, FIELD_HEIGHT * cellSize);

  // змейка
  painter.setBrush(Qt::green);
  for (auto &part : controller.getGame().getSnake().getBody()) {
    painter.drawRect(part.first * cellSize, part.second * cellSize, cellSize,
                     cellSize);
  }

  // яблоко
  painter.setBrush(Qt::red);
  auto apple = controller.getGame().getApple().getPosition();
  painter.drawEllipse(apple.first * cellSize, apple.second * cellSize, cellSize,
                      cellSize);

  // инфо справа
  painter.setPen(Qt::black);
  QFont font = painter.font();
  font.setPointSize(12);
  painter.setFont(font);
  int infoX = FIELD_WIDTH * cellSize + 20;
  painter.drawText(infoX, 30, QString("Score: %1").arg(info.score));
  painter.drawText(infoX, 60, QString("Highscore: %1").arg(info.high_score));
  painter.drawText(infoX, 90, QString("Level: %1").arg(info.level));
}

void SnakeWidget::drawGameOverMenu(QPainter &painter) {
  painter.setBrush(Qt::black);
  painter.drawRect(0, 0, width(), height());
  painter.setPen(Qt::red);
  QFont font = painter.font();
  font.setPointSize(20);
  font.setBold(true);
  painter.setFont(font);
  painter.drawText(rect(), Qt::AlignCenter, "GAME OVER");

  font.setPointSize(14);
  font.setBold(false);
  painter.setFont(font);
  auto info = controller.getGame().getInfo();
  painter.setPen(Qt::white);
  painter.drawText(width() / 2 - 80, height() / 2 + 40,
                   QString("Score: %1").arg(info.score));
  painter.drawText(width() / 2 - 80, height() / 2 + 70,
                   QString("Highscore: %1").arg(info.high_score));

  // Кнопка рестарта (только она)
  if (!restartButton) {
    restartButton = new QPushButton("Restart", this);
    restartButton->setGeometry(width() / 2 - 60, height() / 2 + 80, 120, 40);
    connect(restartButton, &QPushButton::clicked, this,
            &SnakeWidget::restartGame);
    restartButton->show();
  }
}

void SnakeWidget::restartGame() {
  controller.getGame().setGameState(s21::STATE_START);
  controller.getGame().updateCurrentState();
  gameOver = false;
  if (restartButton) {
    restartButton->hide();
    delete restartButton;
    restartButton = nullptr;
  }
}


void SnakeWidget::resetGameState() {
  controller.getGame().setGameState(s21::STATE_START);
  controller.getGame().updateCurrentState();
  gameOver = false;
  setFocus();
}

void SnakeWidget::keyPressEvent(QKeyEvent *event) {
  if (gameOver) return;
  switch (event->key()) {
    case Qt::Key_Left:
      controller.userInput(s21::Left, false);
      break;
    case Qt::Key_Right:
      controller.userInput(s21::Right, false);
      break;
    case Qt::Key_Up:
      controller.userInput(s21::Up, false);
      break;
    case Qt::Key_Down:
      controller.userInput(s21::Down, false);
      break;
    case Qt::Key_P:
      controller.userInput(s21::Pause, false);
      break;
    // case Qt::Key_Q:
    //   controller.userInput(s21::Terminate, false);
    //   break;
  }
}

void SnakeWidget::keyReleaseEvent(QKeyEvent *event) {
  // Можно добавить обработку ускорения по пробелу, если реализовано в
  // контроллере
}
