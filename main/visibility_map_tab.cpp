# include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../lib/screenangle.h"
#include "visibility_map_classes.h"

#include <cmath>

void MainWindow::visibility_map_plot_default_map()
{
    ui->visibility_map->yAxis->setScaleRatio(ui->visibility_map->xAxis, 1.0);
    ui->visibility_map->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                        QCP::iSelectPlottables);

    std::pair<double, double> axes_range = std::pair<double, double>(200, 200);  // range of Ox and Oy for default map image
    ui->visibility_map->xAxis->setRange(-axes_range.first, axes_range.first);
    ui->visibility_map->yAxis->setRange(-axes_range.second, axes_range.second);

    ui->visibility_map->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(9);
    ui->visibility_map->legend->setFont(legendFont);
    ui->visibility_map->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));
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

void MainWindow::visibility_map_plot_angle_map(AngleMap angle_map, PointCartesian rls)
{
    QVector<QCPCurve*> contour_curves;

    size_t n = visibility_map_angles.size();
    for (size_t i = 0; i < n; i++)
    {
        contour_curves.push_back(new QCPCurve(ui->visibility_map->xAxis, ui->visibility_map->yAxis));
        if (i == 0)
            contour_curves.back()->setName(QString("Угол закрытия > ") + QString::number(0));
        else
            contour_curves.back()->setName(QString("Угол закрытия > ") + QString::number(std::round(visibility_map_angles[i - 1] * 100) / 100));
        contour_curves.back()->data()->set(angle_map.contours[i].get_data(), true);
        contour_curves.back()->setPen(QPen(visibility_map_colors[i]));

        QColor brush_color = visibility_map_colors[i];
        brush_color.setAlpha(80);
        contour_curves.back()->setBrush(QBrush(brush_color));
    }

    QVector<double> rls_x{rls.get_x()};
    QVector<double> rls_y{rls.get_y()};
    ui->visibility_map->graph(0)->addData(rls_x, rls_y);

    visibility_map_rls_contour_curves.push_back(contour_curves);
}

void MainWindow::visibility_map_get_angles()
{
    visibility_map_angles.clear();
    visibility_map_angles.push_back(ui->visibility_map_angle_1->value());
    visibility_map_angles.push_back(ui->visibility_map_angle_2->value());
    visibility_map_angles.push_back(ui->visibility_map_angle_3->value());
    visibility_map_angles.push_back(ui->visibility_map_angle_4->value());
    visibility_map_angles.push_back(M_PI / 2);
}

void MainWindow::on_visibility_map_add_RLS_clicked()
{
    RLS *NewRls = new RLS;
    std::string name = "РЛС " + std::to_string(++(this->visibility_map_rls_index));
    ui->visibility_map_RLS_widgets->addTab(NewRls, QString::fromStdString(name));
}

void MainWindow::on_visibility_map_RLS_widgets_tabCloseRequested(int index)
{ ui->visibility_map_RLS_widgets->removeTab(index); }

void MainWindow::on_visibility_map_apply_button_clicked(QAbstractButton *button)
{
    visibility_map_get_angles();

    if (!visibility_map_rls_contour_curves.empty())  // it's replot
    {
        for (auto rls : visibility_map_rls_contour_curves)
            for (auto contour : rls)
                ui->visibility_map->removePlottable(contour);

        visibility_map_rls_contour_curves.clear();
        ui->visibility_map->clearGraphs();
    }

    ui->visibility_map->addGraph();
    ui->visibility_map->graph(0)->setName("РЛС");
    ui->visibility_map->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::red, 0), QColor(255, 0, 0, 200), 10));
    ui->visibility_map->graph(0)->setLineStyle(QCPGraph::lsNone);

    for (int i = 0; i < ui->visibility_map_RLS_widgets->count(); ++i)
    {
        RLS* rls = (RLS*)ui->visibility_map_RLS_widgets->widget(i);
        RLS::Data data = rls->get_all_data();
        AngleMap angle_map = visibility_map_screen_angle_search(data);
        visibility_map_plot_angle_map(angle_map, data.position);
    }
    ui->visibility_map->replot();
}

AngleMap MainWindow::visibility_map_screen_angle_search(RLS::Data data)
{
    EM::ModelEarth* model;
    int i = ui->reflection_calculation_mode->checkedId();
    switch(i)
    {
    case -3:  // flat
        model = models[0];
        break;
    case -2:  // curve
        model = models[1];
        break;
    case -4:  // reflection
        model = models[2];
        break;
    default:
        break;
    }

    EL::GeoData geo_data;
    VG::None veg;
    DP::Constant dp(0);
    CD::Constant c(0);

    Map map(&geo_data, &veg, &dp, &c);

    double angle_iter = 1000;  // count of iteration in angle loop
    double R_iter = 100;  // count of iteration in radius loop
    double R_step = data.radius / R_iter;  // step for in cycle

    PointCartesian rls_position = data.position;
    PointSpheric current_point = PointSpheric(rls_position, PointCartesian(rls_position.get_x() + 1, rls_position.get_y() + 1, rls_position.get_h()));

    size_t n = visibility_map_angles.size();  // count_of_contours
    AngleMap angle_map(n, angle_iter, rls_position);

    int last_external_contour = -1;  // index of last contour which was external
    PointSpheric previous_external_point(current_point);
    PointSpheric previous_internal_point(current_point);  // prevoius internal point of external contour

    for (double azimuth = 0; azimuth < 2 * M_PI; azimuth += 2 * M_PI / angle_iter)
    {
        current_point.change_azimuth(azimuth);
        bool all_empty = true;

        std::vector<PointScreenAngle> max_points(n); // current max points (with max screening angle) for the i contour
        for (double R = R_step; R <= data.radius; R += R_step)
        {
            current_point.change_r(R);
            double screening_angle = FindScreeningAngle(&map, model, current_point, data.radius);

            for (int i = 0; i < n - 1; i++)
                if (screening_angle > visibility_map_angles[i])
                {
                    max_points[i + 1] = PointScreenAngle(current_point, screening_angle);
                    all_empty = false;
                    break;
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
