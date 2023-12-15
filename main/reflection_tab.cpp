#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "../lib/reflection_multiplier.h"
#include <cmath>

void MainWindow::on_reflection_apply_button_clicked(QAbstractButton *button)
{
    if (!ui->reflection_coefficient->displayText().isEmpty())  // if there was any output
    {
        ui->reflection_coefficient->clear();
        ui->reflection_point_x->clear();
        ui->reflection_point_y->clear();
        ui->reflection_point_z->clear();
        ui->reflection_point_graph->clearGraphs();
    }

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

    DP::Constant const_dp(4);
    VG::GeoData veg;
    EL::GeoData geo_data;
    CD::Constant c(5);

    Map map(&geo_data, &veg, &const_dp, &c);

    PointCartesian target;
    target.change_x(ui->reflection_object_x->value());
    target.change_y(ui->reflection_object_y->value());
    target.change_z(ui->reflection_object_z->value());

    RLS::Data rls_data;
    rls_data = ui->reflection_rls->get_all_data();
    PointCartesian rls = rls_data.position;

    PointCartesian reflection_point = RP::FindReflectionPoint(PointSpheric(rls, target), &map, model);
    ui->reflection_point_x->insert(QString::number(std::round(reflection_point.get_x() * 100) / 100));
    ui->reflection_point_y->insert(QString::number(std::round(reflection_point.get_y() * 100) / 100));
    ui->reflection_point_z->insert(QString::number(std::round(reflection_point.get_h() * 100) / 100));

    std::complex<double> reflection_coef = reflection_find_coefficient(rls_data, &map, reflection_point, model);
    if (reflection_coef.imag() >= 0)
        ui->reflection_coefficient->insert(QString::number(std::round(reflection_coef.real() * 100) / 100) + " + " +
                                           QString::number(std::round(reflection_coef.imag() * 100) / 100) + "i");
    else
        ui->reflection_coefficient->insert(QString::number(std::round(reflection_coef.real() * 100) / 100) + " " +
                                           QString::number(std::round(reflection_coef.imag() * 100) / 100) + "i");

    reflection_plot_graph(rls, target, reflection_point, &map);
}

std::complex<double> MainWindow::reflection_find_coefficient(RLS::Data rls_data, Map* map, PointCartesian reflection_point, EM::ModelEarth* model)
{
    PointSpheric point(rls_data.position, reflection_point);
    double incidence_angle = ReflectionPoint::FindIncidenceAngle(point, map, model);
    double wave_l = rls_data.wave_length;
    double conductivity = 5;  // to do
    double sko = wave_l / (16 * std::sin(incidence_angle));
    VG::VegetationType vegetation = VG::VegetationType::Forest;  // to do

    RC::ReflectionMultiplier reflection_multiplier;

    return reflection_multiplier.reflection_multiplier(map, point, rls_data.polarization, incidence_angle, wave_l, conductivity, sko, vegetation);
}

void MainWindow::reflection_plot_graph(PointCartesian rls, PointCartesian target, PointCartesian reflection_point, Map* map)
{
    PointSpheric point(rls, target);  // temporary value
    Map1d map1d(map, rls, target);

    const double step = 0.1;
    // fill relief dots
    QVector<double> x, y;
    for (double d = 0; d < point.get_d(); d += step) {
        y.push_back(map1d.height(d));
        x.push_back(d);
    }

    PointSpheric center_refl(point.get_center(), reflection_point);

    double d_center = 0;
    double d_reflection = center_refl.get_d();
    double d_target = point.get_d();

    // from center to reflection point
    QVector<double> x1, y1;
    x1.push_back(d_center);
    x1.push_back(d_reflection);
    y1.push_back(point.get_center().get_h());
    y1.push_back(reflection_point.get_h());

    // from reflection point to target
    QVector<double> x2, y2;
    x2.push_back(d_reflection);
    x2.push_back(d_target);
    y2.push_back(reflection_point.get_h());
    y2.push_back(point.get_target().get_h());

    ui->reflection_point_graph->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->reflection_point_graph->legend->setFont(legendFont);
    ui->reflection_point_graph->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));

    ui->reflection_point_graph->addGraph();  // graph of surface
    ui->reflection_point_graph->graph(0)->setPen(QPen(QColor(32, 32, 32)));
    ui->reflection_point_graph->graph(0)->setData(x, y);
    ui->reflection_point_graph->graph(0)->setName("Поверхность");
    ui->reflection_point_graph->graph(0)->rescaleAxes();

    ui->reflection_point_graph->addGraph();  // graph of ray to surface
    ui->reflection_point_graph->graph(1)->setPen(QPen(Qt::blue));
    ui->reflection_point_graph->graph(1)->setData(x1, y1);
    ui->reflection_point_graph->graph(1)->setName("Луч к поверхности");

    ui->reflection_point_graph->addGraph();  // graph of ray from surface
    ui->reflection_point_graph->graph(2)->setPen(QPen(Qt::red));
    ui->reflection_point_graph->graph(2)->setData(x2, y2);
    ui->reflection_point_graph->graph(2)->setName("Луч от поверхности");

    ui->reflection_point_graph->addGraph();  // graph with rls point
    ui->reflection_point_graph->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::blue, 0), QColor(0, 0, 255, 200), 7));
    ui->reflection_point_graph->graph(3)->setData(QVector<double>{d_center}, QVector<double>{point.get_center().get_h()});
    ui->reflection_point_graph->graph(3)->setName("Положение РЛС");
    ui->reflection_point_graph->graph(3)->setLineStyle(QCPGraph::lsNone);

    ui->reflection_point_graph->addGraph();  // graph with reflection point
    ui->reflection_point_graph->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, QPen(Qt::black, 0), QColor(0, 0, 0, 200), 15));
    ui->reflection_point_graph->graph(4)->setData(QVector<double>{d_reflection}, QVector<double>{reflection_point.get_h()});
    ui->reflection_point_graph->graph(4)->setName("Положение точки отражения");
    ui->reflection_point_graph->graph(4)->setLineStyle(QCPGraph::lsNone);

    ui->reflection_point_graph->addGraph();  // graph with target point
    ui->reflection_point_graph->graph(5)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, QPen(Qt::black, 0), QColor(0, 0, 0, 90), 10));
    ui->reflection_point_graph->graph(5)->setData(QVector<double>{d_target}, QVector<double>{point.get_target().get_h()});
    ui->reflection_point_graph->graph(5)->setName("Положение цели");
    ui->reflection_point_graph->graph(5)->setLineStyle(QCPGraph::lsNone);

    // add subgrid
    ui->reflection_point_graph->xAxis->grid()->setSubGridVisible(true);
    ui->reflection_point_graph->yAxis->grid()->setSubGridVisible(true);

    // add labels
    ui->reflection_point_graph->xAxis->setLabel("Расстояние, м");
    ui->reflection_point_graph->yAxis->setLabel("Высота, м");

    ui->reflection_point_graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->visibility_map->replot();
}
