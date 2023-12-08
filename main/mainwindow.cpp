#include "mainwindow.h"

#include <cmath>
#include <iostream>

#include "./ui_mainwindow.h"
#include "../lib/screenangle.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->on_add_RLS_clicked(); // add defualt 1 RLS
    this->_plot_image();
//    this->_plot_angle_map();
    ui->visibility_map->yAxis->setScaleRatio(ui->visibility_map->xAxis, 1.0);
    ui->visibility_map->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                        QCP::iSelectPlottables);
}

void MainWindow::resizeEvent(QResizeEvent *) {
    QCPAxis *x, *y;
    x = ui->visibility_map->axisRect()->axis(QCPAxis::atBottom);
    y = ui->visibility_map->axisRect()->axis(QCPAxis::atLeft);
    y->setScaleRatio(x, 1.0);
    ui->visibility_map->replot();
}

void MainWindow::_plot_image()
{
    QPixmap pix(":map/map.PNG");
    QCPItemPixmap *MyImage = new QCPItemPixmap(ui->visibility_map);
    MyImage->setPixmap(pix);
    MyImage->topLeft->setType(QCPItemPosition::ptPlotCoords);
    MyImage->topLeft->setCoords(-3. * pix.width()/pix.height(), 3);
    MyImage->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    MyImage->bottomRight->setCoords(3. * pix.width()/pix.height(), -3);
    MyImage->setScaled(true, Qt::AspectRatioMode::IgnoreAspectRatio);
}

void MainWindow::_plot_angle_map(std::vector<std::vector<PointScreenAngle>> contours)
{
    ui->visibility_map->addGraph();
    ui->visibility_map->xAxis->setRange(-50, 50);
    ui->visibility_map->yAxis->setRange(-50, 50);

    for (auto contour : contours)
    {
        const size_t point_count = contour.size();
        QVector<QCPCurveData> data(point_count);
        for (size_t i = 0; i < point_count; i++)
        {
            double x = contour[i].point2d.get_x();
            double y = contour[i].point2d.get_y();
            data[i] = QCPCurveData(i, x, y);
        }

        QCPCurve *contour_curve = new QCPCurve(ui->visibility_map->xAxis, ui->visibility_map->yAxis);
        contour_curve->data()->set(data, true);
        contour_curve->setPen(QPen(Qt::blue));
        contour_curve->setBrush(QBrush(QColor(0, 0, 255, 40)));
    }
}

MainWindow::~MainWindow() { delete ui; }


void MainWindow::on_add_RLS_clicked()
{
    RLS *NewRls = new RLS;
    std::string name = "РЛС " + std::to_string(++(this->_rls_index));
    ui->RLS_widgets->addTab(NewRls, QString::fromStdString(name));
}


void MainWindow::on_RLS_widgets_tabCloseRequested(int index)
{
    ui->RLS_widgets->removeTab(index);
}


void MainWindow::on_apply_button_clicked(QAbstractButton *button)
{
    for (int i = 0; i < ui->RLS_widgets->count(); ++i)
    {
        RLS* rls = (RLS*)ui->RLS_widgets->widget(0);
        RLS::Data data = rls->get_all_data();
        std::vector<std::vector<PointScreenAngle>> contour_points = _screen_angle_search(data);
        _plot_angle_map(contour_points);
    }
}

std::vector<std::vector<PointScreenAngle>> MainWindow::_screen_angle_search(RLS::Data data)
{
    double angle_iter = 1000;  // count of iteration in angle loop
    double R_iter = 100;  // count of iteration in radius loop

    PointCartesian rls_position = PointCartesian();  // rls in (0, 0, 0) by default
    PointSpheric current_point = PointSpheric(rls_position, PointCartesian());

    size_t count_of_contours = 3;
    std::vector<std::vector<PointScreenAngle>> contours(count_of_contours);  // points of contours to draw visibility angle map
    std::vector<double> angles = {(M_PI / 2) / 3, (M_PI / 2) / 3 * 2, M_PI / 2};  // angles for the i contour (<= angles[i])
    std::vector<PointScreenAngle> max_points(count_of_contours); // current max points (with max screening angle) for the i contour

    for (double azimuth = 0; azimuth < 2 * M_PI; azimuth += 2 * M_PI / angle_iter)
    {
        current_point.change_azimuth(azimuth);

        for (double R = 0; R <= data.radius; R += R_iter)
        {
            current_point.change_r(R);
            double screening_angle = FindScreeningAngle(current_point, R);

            for (size_t i; i < angles.size(); ++i)  // find contour which fits this screening angle
                if (screening_angle > angles[i])
                {
                    size_t index = i - 1;  // index of contour
                    Point2d point2d = Point2d(current_point.get_x(), current_point.get_y());
                    max_points[index] = PointScreenAngle(point2d, screening_angle);
                }
        }

        for (size_t i; i < max_points.size(); ++i)
            if (!max_points[i].empty())
                contours[i].push_back(max_points[i]);
    }

    return contours;
}
