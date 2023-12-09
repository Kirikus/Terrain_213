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
    std::pair<double, double> axes_range = std::pair<double, double>(200, 200);  // range of Ox and Oy for default map image
    ui->visibility_map->xAxis->setRange(-axes_range.first, axes_range.first);
    ui->visibility_map->yAxis->setRange(-axes_range.second, axes_range.second);
    ui->visibility_map->addGraph();


    QPixmap pix(":map/map.PNG");
    QCPItemPixmap *MyImage = new QCPItemPixmap(ui->visibility_map);
    MyImage->setPixmap(pix);
    MyImage->topLeft->setType(QCPItemPosition::ptPlotCoords);
    MyImage->topLeft->setCoords(-axes_range.first * pix.width()/pix.height(), axes_range.second);
    MyImage->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    MyImage->bottomRight->setCoords(axes_range.first * pix.width()/pix.height(), -axes_range.second);
    MyImage->setScaled(true, Qt::AspectRatioMode::IgnoreAspectRatio);
}

void MainWindow::_plot_angle_map(std::vector<std::vector<Point2d>> contours)
{
    QVector<QCPCurve*> contour_curves;

    int counter = 1;  // to do
    for (auto contour : contours)
    {
        const size_t point_count = contour.size();
        QVector<QCPCurveData> data(point_count);
        for (size_t i = 0; i < point_count; i++)
        {
            double x = contour[i].get_x();
            double y = contour[i].get_y();
            data[i] = QCPCurveData(i, x, y);
        }

        contour_curves.push_back(new QCPCurve(ui->visibility_map->xAxis, ui->visibility_map->yAxis));
        contour_curves.back()->data()->set(data, true);
        contour_curves.back()->setPen(QPen(counter));
        if (counter == 1)
            contour_curves.back()->setBrush(QBrush(QColor(0, 0, 255, 40)));
        else
            contour_curves.back()->setBrush(QBrush(QColor(0, 255, 255, 40)));
        counter++;
    }

    rls_contour_curves.push_back(contour_curves);
}

MainWindow::~MainWindow()
{
    for (auto rls : rls_contour_curves)
        for (auto contour : rls)
            delete contour;
    delete ui;
}


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
    if (!rls_contour_curves.empty())  // it's replot
    {
        for (auto rls : rls_contour_curves)
            for (auto contour : rls)
                delete contour;
        rls_contour_curves.clear();
    }

    for (int i = 0; i < ui->RLS_widgets->count(); ++i)
    {
        RLS* rls = (RLS*)ui->RLS_widgets->widget(i);
        RLS::Data data = rls->get_all_data();
        std::vector<std::vector<Point2d>> contour_points = _screen_angle_search(data);
        _plot_angle_map(contour_points);
    }
}

std::vector<std::vector<Point2d>> MainWindow::_screen_angle_search(RLS::Data data)
{
    double angle_iter = 1000;  // count of iteration in angle loop
    double R_iter = 10;  // count of iteration in radius loop
    double R_step = data.radius / R_iter;  // step for in cycle

    PointCartesian rls_position = data.position;
    std::cout << data.position.get_x() << std::endl;
    PointSpheric current_point = PointSpheric(rls_position, PointCartesian(rls_position.get_x() + 1, rls_position.get_y() + 1, rls_position.get_h()));

    size_t n = 2;  // count_of_contours
    std::vector<std::vector<Point2d>> contours(n);  // points of contours to draw visibility angle map
    std::vector<double> angles = {(M_PI / 2) / 3, (M_PI / 2) / 3 * 2};  // angles for the i contour (<= angles[i])

    for (double azimuth = 0; azimuth < 2 * M_PI; azimuth += 2 * M_PI / angle_iter)
    {
        current_point.change_azimuth(azimuth);

        std::vector<PointScreenAngle> max_points(n); // current max points (with max screening angle) for the i contour
        for (double R = R_step; R <= data.radius; R += R_step)
        {
            current_point.change_r(R);

            double screening_angle = FindScreeningAngle(current_point, data.radius);

            for (size_t i = 0; i < n; i++)
            {
                if (screening_angle > angles[i])
                    max_points[i] = PointScreenAngle(current_point.get_target().get_x(), current_point.get_target().get_y(), screening_angle);
            }
        }

        size_t last_contour = -1;  // index of contour with last added point
        for (int i = n - 1; i >= 0; i--)
        {
            if (!max_points[i].empty())
            {
                contours[i].push_back(max_points[i].point2d);
                last_contour = i;
            }
            else if (last_contour == -1)
            {
                contours[i].push_back(Point2d(current_point.get_target().get_x(), current_point.get_target().get_y()));
                last_contour = i;
            }
            else
                contours[i].push_back(contours[last_contour].back());
        }
    }

    return contours;
}
