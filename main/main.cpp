#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(":icon/icon.ico"));
  a.setApplicationName(QString("Модель рельефа и многолучевое распределение"));
  MainWindow w;
  w.setWindowTitle(QString("Модель рельефа и многолучевое распределение"));
  w.show();
  return a.exec();
}
