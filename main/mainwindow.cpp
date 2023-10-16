#include "qcustomplot.h"

#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QPixmap pix("srs/img/map.jpg");
  ui->visibility_map->setBackground(pix);
  ui->visibility_map->setBackgroundScaledMode(Qt::IgnoreAspectRatio);
}

MainWindow::~MainWindow() { delete ui; }
