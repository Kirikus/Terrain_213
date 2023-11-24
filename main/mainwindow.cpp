#include "mainwindow.h"
#include "rls.h"

#include <cmath>
#include <iostream>
#include <string.h>

#include "./ui_mainwindow.h"
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QPixmap pix(":map/map.jpg");
    ui->visibility_map->setBackground(pix);
    ui->visibility_map->setBackgroundScaledMode(Qt::IgnoreAspectRatio);

    ui->visibility_map->addGraph();
    ui->visibility_map->xAxis->setRange(-2, 2);
    ui->visibility_map->yAxis->setRange(-2, 2);

    const int pointCount = 1000;

    QVector<QCPCurveData> data_cirle_inside(pointCount);
    for(int i=0; i < pointCount; ++i)
    {
        double phi = i/(double)(pointCount-1)* 2*M_PI;
        data_cirle_inside[i] = QCPCurveData(i, qCos(phi), qSin(phi));
    }

    QVector<QCPCurveData> data_cirle_outside(pointCount);
    for(int i=0; i < pointCount; ++i)
    {
        double phi = i/(double)(pointCount-1)* 2*M_PI;
        data_cirle_outside[i] = QCPCurveData(i, 0.7*qCos(phi), 0.7*qSin(phi));
    }

    QVector<QCPCurveData> data_polygon(5);
    data_polygon[0] = QCPCurveData(0, 0.5, -0.5);
    data_polygon[1] = QCPCurveData(1, 0.5, 0.5);
    data_polygon[2] = QCPCurveData(2, -0.5, 0.5);
    data_polygon[3] = QCPCurveData(3, -0.5, -0.5);
    data_polygon[4] = QCPCurveData(4, 0.5, -0.5);

    QCPCurve *circle_inside = new QCPCurve(ui->visibility_map->xAxis, ui->visibility_map->yAxis);
    circle_inside->data()->set(data_cirle_inside, true);

    QCPCurve *circle_outside = new QCPCurve(ui->visibility_map->xAxis, ui->visibility_map->yAxis);
    circle_outside->data()->set(data_cirle_outside, true);

    QCPCurve *polygon = new QCPCurve(ui->visibility_map->xAxis, ui->visibility_map->yAxis);
    polygon->data()->set(data_polygon, true);

    circle_inside->setPen(QPen(Qt::blue));
    circle_inside->setBrush(QBrush(QColor(0, 0, 255, 20)));

    circle_outside->setPen(QPen(Qt::red));
    circle_outside->setBrush(QBrush(QColor(255, 0, 0, 40)));

    polygon->setPen(QPen(Qt::green));
    polygon->setBrush(QBrush(QColor(0, 255, 0, 40)));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_apply_reset_button_clicked(QAbstractButton *button)

void MainWindow::on_addRLS_clicked()
{
    RLS *NewRls = new RLS;
    std::string name = "РЛС " + std::to_string(++(this->_rls_index));
    ui->RLS_widgets->addTab(NewRls, QString::fromStdString(name));
}


{
    ui->RLS_data_1->reset_data();
    ui->RLS_data_2->reset_data();
    ui->RLS_data_3->reset_data();
    ui->RLS_data_4->reset_data();
    ui->RLS_data_5->reset_data();
    ui->RLS_data_6->reset_data();
}

