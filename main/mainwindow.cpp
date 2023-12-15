#include "mainwindow.h"

#include <cmath>
#include <iostream>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->on_visibility_map_add_RLS_clicked(); // add defualt 1 RLS
    this->visibility_map_plot_default_map();
}

void MainWindow::resizeEvent(QResizeEvent *) {
    QCPAxis *x, *y;
    x = ui->visibility_map->axisRect()->axis(QCPAxis::atBottom);
    y = ui->visibility_map->axisRect()->axis(QCPAxis::atLeft);
    y->setScaleRatio(x, 1.0);
    ui->visibility_map->replot();
}

MainWindow::~MainWindow()
{
    if (!visibility_map_rls_contour_curves.empty())
        for (auto rls : visibility_map_rls_contour_curves)
            for (auto contour : rls)
                ui->visibility_map->removePlottable(contour);
    delete ui;
}
