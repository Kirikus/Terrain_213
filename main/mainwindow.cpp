#include "mainwindow.h"

#include <cmath>
#include <iostream>

#include "./ui_mainwindow.h"
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QPixmap pix(":map/map.jpg");
  //  std::cout << pix.height();
  ui->visibility_map->setBackground(pix);
  ui->visibility_map->setBackgroundScaledMode(Qt::IgnoreAspectRatio);

}

MainWindow::~MainWindow() { delete ui; }
