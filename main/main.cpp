#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(":icon/icon.ico"));
  MainWindow w;
  w.show();
  return a.exec();
}
