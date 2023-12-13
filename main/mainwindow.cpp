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
//    ui->visibility_map->addGraph();
    ui->visibility_map->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->visibility_map->legend->setFont(legendFont);
    ui->visibility_map->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    ui->visibility_map->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    QPixmap pix(":map/map.PNG");
    QCPItemPixmap *MyImage = new QCPItemPixmap(ui->visibility_map);
    MyImage->setPixmap(pix);
    MyImage->topLeft->setType(QCPItemPosition::ptPlotCoords);
    MyImage->topLeft->setCoords(-axes_range.first * pix.width()/pix.height(), axes_range.second);
    MyImage->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    MyImage->bottomRight->setCoords(axes_range.first * pix.width()/pix.height(), -axes_range.second);
    MyImage->setScaled(true, Qt::AspectRatioMode::IgnoreAspectRatio);
}

void MainWindow::_plot_angle_map(AngleMap angle_map)
{
//    ui->visibility_map->graph()->setName("Bottom maxwell function");

    QVector<QCPCurve*> contour_curves;

    size_t n = angles.size();
    for (size_t i = 0; i < n; i++)
    {
//        ui->visibility_map->addGraph(ui->visibility_map->xAxis, ui->visibility_map->yAxis);

        contour_curves.push_back(new QCPCurve(ui->visibility_map->xAxis, ui->visibility_map->yAxis));
        contour_curves.back()->data()->set(angle_map.contours[i].get_data(), true);
        contour_curves.back()->setPen(QPen(colors[i]));

        QColor brush_color = colors[i];
        brush_color.setAlpha(80);
        contour_curves.back()->setBrush(QBrush(brush_color));
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
        ui->visibility_map->clearItems();
        ui->visibility_map->replot();
    }

    for (int i = 0; i < ui->RLS_widgets->count(); ++i)
    {
        RLS* rls = (RLS*)ui->RLS_widgets->widget(i);
        RLS::Data data = rls->get_all_data();
        AngleMap angle_map = _screen_angle_search(data);
        _plot_angle_map(angle_map);
    }
}

AngleMap MainWindow::_screen_angle_search(RLS::Data data)
{
    double angle_iter = 1000;  // count of iteration in angle loop
    double R_iter = 100;  // count of iteration in radius loop
    double R_step = data.radius / R_iter;  // step for in cycle

    PointCartesian rls_position = data.position;
    PointSpheric current_point = PointSpheric(rls_position, PointCartesian(rls_position.get_x() + 1, rls_position.get_y() + 1, rls_position.get_h()));

    size_t n = angles.size();  // count_of_contours
    AngleMap angle_map(n, angle_iter, rls_position);

    int last_external_contour = -1;  // index of last contour which was external
    PointSpheric previous_external_point(current_point);
//    PointSpheric previous_internal_point(current_point);

    PointSpheric previous_internal_point(current_point);  // prevoius internal point of external contour

    for (double azimuth = 0; azimuth < 2 * M_PI; azimuth += 2 * M_PI / angle_iter)
    {
        current_point.change_azimuth(azimuth);
        bool all_empty = true;

        std::vector<PointScreenAngle> max_points(n); // current max points (with max screening angle) for the i contour
        for (double R = R_step; R <= data.radius; R += R_step)
        {
            current_point.change_r(R);

            double screening_angle = FindScreeningAngle(current_point, data.radius);

            for (int i = n - 2; i >= 0; i--)
            {
                if (screening_angle > angles[i])
                {
                    max_points[i + 1] = PointScreenAngle(current_point, screening_angle);
                    all_empty = false;
                    break;
                }
            }
        }

        if (all_empty)
        {
            angle_map.contours[0].add_point(current_point, true);
            if (last_external_contour != 0 && last_external_contour != -1)
            {
                angle_map.contours[0].add_point(previous_external_point, true);
            }
            last_external_contour = 0;
            previous_external_point = current_point;
            continue;
        }

        int last_contour = -1;
        int external_contour;
        for (int i = n - 1; i > 0; i--)
        {
            if (!max_points[i].empty())  // any point for contour[i] exists
            {
                if (last_contour == -1)  // if haven't add any point before
                {
                    angle_map.contours[i].add_point(current_point, true);  // add external point
                    angle_map.contours[i].add_point(max_points[i].point_spheric, false);  // add own point of contours[i]

                    if (last_external_contour != i && last_external_contour != -1 && last_external_contour != 0)
                    {
                        angle_map.contours[last_external_contour].add_point(current_point, true);
                        if (max_points[last_external_contour].empty())
                            angle_map.contours[last_external_contour].change_point(azimuth, previous_internal_point, false);  // dublicate previous internal point but with current azimuth
                        else
                            angle_map.contours[last_external_contour].change_point(azimuth, max_points[last_external_contour].point_spheric, false);
                    }
                    external_contour = i;
                }
                else if (last_external_contour != i)  // it's an edge and we shouldn't rewrite internal and external point
                {
                    angle_map.contours[i].add_point(angle_map.contours[last_contour].back(), true);  // add point of previous contour
                    angle_map.contours[i].add_point(max_points[i].point_spheric, false);  // add own point of contours[i]
                }
                last_contour = i;
            }
        }
        if (last_external_contour == 0)
        {
            angle_map.contours[0].add_point(angle_map.contours[last_contour].back(), false);  // add internal point as external of last existed contour for this azimuth
            angle_map.contours[0].add_point(current_point, true);  // add external point
        }
        else
            angle_map.contours[0].add_point(angle_map.contours[last_contour].back(), false);  // add external point

        last_external_contour = external_contour;
        previous_external_point = current_point;
        previous_internal_point = max_points[external_contour].point_spheric;
    }

    return angle_map;
}

QCPCurveData Contour::get_point(const int key, const int i, const bool external)
{
    double x, y;
    if (external)
    {
        x = points[key].second.get_x();
        y = points[key].second.get_y();
    }
    else
    {
        x = points[key].first.get_x();
        y = points[key].first.get_y();
    }
    return QCPCurveData(i, x, y);
}

QVector<QCPCurveData> Contour::get_data()
{
    if (if_zero())
        return get_zero_data();

//    QVector<QCPCurveData> result(point_count);
    QVector<QCPCurveData> result;

    bool point_before = false;  // if there were point for previous azimuth value
    int i = 0;  // parameter for QCPCurveData
    int first_azimuth = 0;  // key which was the first when algorithms go in reverse
    int max_added_azimuth = -1;  // max azimuth of all added to result point
    for (double azimuth = 0; azimuth < 2 * M_PI * angle_iterations; azimuth += 2 * M_PI)
    {
        if (int(azimuth) == max_added_azimuth)
            break;

        if (points.find(int(azimuth)) != points.end())  // if there is a point for this azimuth
        {
            if (!point_before)
                first_azimuth = int(azimuth);

            // internal point for this azimuth
            result.push_back(get_point(int(azimuth), i, false));
            i++;
            point_before = true;
            continue;
        }

        // if there is no point for this azimuth
        if (!point_before)  // if there was no point before too, than we are beyond contour area
            continue;

        else  // means last time it was edge of contour area
        {
            point_before = false;
            for (double anticlockwise_azimuth = azimuth - 2 * M_PI; ; anticlockwise_azimuth -= 2 * M_PI)  // go behind (anticlockwise) for azimuth
            {
                if (points.find(int(anticlockwise_azimuth)) != points.end())  // if there is a point for this azimuth
                {
                    // external point for this azimuth
                    result.push_back(get_point(int(anticlockwise_azimuth), i, true));
                    i++;
                    if (int(anticlockwise_azimuth) == 0)
                        anticlockwise_azimuth = 2 * M_PI * angle_iterations;
                    continue;
                }

                // if there is no point for this azimuth
                max_added_azimuth = std::max(int(anticlockwise_azimuth + 2 * M_PI), max_added_azimuth);

                anticlockwise_azimuth += 2 * M_PI;
                if (int(anticlockwise_azimuth) == int(2 * M_PI * angle_iterations))
                    anticlockwise_azimuth = 0;

                for (double clockwise_azimuth = anticlockwise_azimuth; int(clockwise_azimuth) != first_azimuth; clockwise_azimuth += 2 * M_PI)
                {
                    if (int(clockwise_azimuth) == int(2 * M_PI * angle_iterations))
                    {
                        clockwise_azimuth = 0;
                        if (int(clockwise_azimuth) == first_azimuth)
                            break;
                    }
                    result.push_back(get_point(int(clockwise_azimuth), i, false));
                    i++;
                }
                result.push_back(get_point(first_azimuth, i, false));
                i++;
                break;
            }
        }
    }
    return result;
}


QVector<QCPCurveData> Contour::get_zero_data()
{
    QVector<QCPCurveData> result;

    int i = 0;  // parameter for QCPCurveData
    bool internal_before = false; // if there was inernal point in last iteration
    for (double azimuth = 0; azimuth < 2 * M_PI * angle_iterations; azimuth += 2 * M_PI)
    {
        QCPCurveData internal_point = get_point(int(azimuth), i, false);
        QCPCurveData external_point = get_point(int(azimuth), i, true);
        if (Point2d(internal_point.key, internal_point.value) != rls_position)  // if internal != default
        {
            if (Point2d(external_point.key, external_point.value) == rls_position)  // if external == default
            {
                internal_before = true;
                result.push_back(get_point(int(azimuth), i, false));   // internal point
                i++;
                continue;
            }

            // if external != default
            if (internal_before)
            {
                result.push_back(get_point(int(azimuth), i, false));  // internal point
                i++;
                result.push_back(get_point(int(azimuth), i, true));  // external point
                i++;
                internal_before = false;
            }
            else
            {
                result.push_back(get_point(int(azimuth), i, true));  // external point
                i++;
                result.push_back(get_point(int(azimuth), i, false));  // internal point
                i++;
                internal_before = true;
            }
        }
        else
        {
            result.push_back(get_point(int(azimuth), i, true));  // external point for this azimuth
            i++;
        }
    }
    QCPCurveData point = result[0];
    point.t = i;
    result.push_back(point);  // костыль
    return result;
}
