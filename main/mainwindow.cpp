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
    ui->visibility_map->legend->setVisible(true);
    /*QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->visibility_map->legend->setFont(legendFont);
    ui->visibility_map->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    ui->visibility_map->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
*/
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
//    ui->visibility_map->graph()->setName("Bottom maxwell function");

    QVector<QCPCurve*> contour_curves;

    size_t counter = 0;
    for (auto contour : contours)
    {
        ui->visibility_map->addGraph(ui->visibility_map->xAxis, ui->visibility_map->yAxis);
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
        contour_curves.back()->setPen(QPen(colors[counter]));

        QColor brush_color = colors[counter];
//        brush_color.setAlpha(100 - 100 / angles.size() * counter);
        brush_color.setAlpha(80);
        contour_curves.back()->setBrush(QBrush(brush_color));

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
    double angle_iter = 100;  // count of iteration in angle loop
    double R_iter = 100;  // count of iteration in radius loop
    double R_step = data.radius / R_iter;  // step for in cycle

    PointCartesian rls_position = data.position;
    PointSpheric current_point = PointSpheric(rls_position, PointCartesian(rls_position.get_x() + 1, rls_position.get_y() + 1, rls_position.get_h()));

    size_t n = angles.size();  // count_of_contours
    std::vector<std::vector<Point2d>> contours(n);  // points of contours to draw visibility angle map

    int last_external_contour = -1;  // index of last contour which was external
    PointSpheric previous_external_point(current_point);

    for (double azimuth = 0; azimuth < 2 * M_PI; azimuth += 2 * M_PI / angle_iter)
    {
        current_point.change_azimuth(azimuth);
        bool all_empty = true;

        std::vector<PointScreenAngle> max_points(n); // current max points (with max screening angle) for the i contour
        for (double R = R_step; R <= data.radius; R += R_step)
        {
            current_point.change_r(R);

            double screening_angle = FindScreeningAngle(current_point, data.radius);

            for (size_t i = 0; i < n; i++)
            {
                if (screening_angle > angles[i])
                {
                    max_points[i + 1] = PointScreenAngle(current_point.get_target().get_x(), current_point.get_target().get_y(), screening_angle);
                    all_empty = false;
                }
            }
        }

        if (all_empty)
        {
            contours[0].push_back(Point2d(current_point.get_target().get_x(), current_point.get_target().get_y()));
            if (last_external_contour != 0 && last_external_contour != -1)
                contours[0].push_back(Point2d(previous_external_point.get_x(), previous_external_point.get_y()));
            last_external_contour = 0;
            previous_external_point = current_point;
            continue;
        }

        int last_contour = -1;
        for (int i = n - 1; i >= 0; i--)
        {
            if (!max_points[i].empty())  // any point for contour[i] exists
            {
                if (last_contour == -1)  // if haven't add any point before
                {
                    contours[i].push_back(Point2d(current_point.get_target().get_x(), current_point.get_target().get_y()));  // add external point
                    if (last_external_contour != i && last_external_contour != -1)
                        contours[i].push_back(Point2d(previous_external_point.get_x(), previous_external_point.get_y()));
                    last_external_contour = i;
                    previous_external_point = current_point;
                }
                else
                    contours[i].push_back(contours[last_contour].back());  // add point of previous contour

                contours[i].push_back(max_points[i].point2d);  // add own point of contours[i]
                last_contour = i;
            }
            else if (i == 0)  // no point for contours[i] but it's the last one
                contours[0].push_back(contours[last_contour].back());  // add
        }

//        if (all_empty)
//        {
//            contours[0].push_back(Point2d(current_point.get_target().get_x(), current_point.get_target().get_y()));
//            continue;
//        }
//        int last_contour = -1;
//        for (int i = n - 1; i >= 0; i--)
//        {
//            if (!max_points[i].empty())
//            {
//                contours[i].push_back(max_points[i].point2d);
//                last_contour = i;
//            }
//            else if (last_contour == -1)
//            {
//                contours[i].push_back(Point2d(current_point.get_target().get_x(), current_point.get_target().get_y()));
//                last_contour = i;
//            }
//            else
//                contours[i].push_back(contours[last_contour].back());
//        }
    }

    return contours;
}
