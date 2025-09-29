#include <QApplication>

#include "snakewidget.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  SnakeWidget window;
  window.show();

  return app.exec();
}
