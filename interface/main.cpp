#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  /* QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar); */
  MainWindow w;
  w.resize(400,400);
  w.show();
  return a.exec();
}
