#include "qcustomplot.h"
#include <iostream>

#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QPixmap pix("srs/img/map.jpg");
  std::cout << pix.height();
  ui->visibility_map->setBackground(pix);
  ui->visibility_map->setBackgroundScaledMode(Qt::IgnoreAspectRatio);
}

MainWindow::~MainWindow() { delete ui; }
