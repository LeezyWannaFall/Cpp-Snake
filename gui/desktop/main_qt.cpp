#include <QApplication>
#include "gamewidget.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  GameWidget window;
  window.show();
  return app.exec();
}

