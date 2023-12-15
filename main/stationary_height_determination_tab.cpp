#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../lib/height_determination.h"

void MainWindow::on_stationary_height_apply_button_clicked(QAbstractButton *button)
{
    if (!ui->stationary_height_h->displayText().isEmpty())  // if there was any output
    {
        ui->stationary_height_h->clear();
        ui->stationary_height_graph->clearGraphs();
    }

    EM::ModelEarth* model;
    int i = ui->stationary_height_calculation_mode->checkedId();
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

    Map map(&geo_data, &vegetation, &const_dp, &c);

    double R = ui->stationary_height_R->value();
    double R1 = ui->stationary_height_R1->value();
    double R2 = ui->stationary_height_R2->value();

    double x = ui->stationary_height_x->value();
    double y = ui->stationary_height_y->value();

    RLS::Data rls_data;
    rls_data = ui->stationary_height_rls->get_all_data();
    PointCartesian rls = rls_data.position;

    PointCartesian reflection_point;

    double h = FindTargetHeight(&map, model, rls, Point2d(x ,y), &reflection_point, R, R1, R2, rls_data.radius);

    ui->stationary_height_h->insert(QString::number(std::round(h * 100) / 100));

    PointCartesian target(x ,y ,h);

    stationary_height_plot_graph(rls, target, reflection_point, &map);
}

void MainWindow::stationary_height_plot_graph(PointCartesian rls, PointCartesian target, PointCartesian reflection_point, Map* map)
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

    ui->stationary_height_graph->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->stationary_height_graph->legend->setFont(legendFont);
    ui->stationary_height_graph->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));

    ui->stationary_height_graph->addGraph();  // graph of surface
    ui->stationary_height_graph->graph(0)->setPen(QPen(QColor(32, 32, 32)));
    ui->stationary_height_graph->graph(0)->setData(x, y);
    ui->stationary_height_graph->graph(0)->setName("Поверхность");
    ui->stationary_height_graph->graph(0)->rescaleAxes();

    ui->stationary_height_graph->addGraph();  // graph of ray to surface
    ui->stationary_height_graph->graph(1)->setPen(QPen(Qt::blue));
    ui->stationary_height_graph->graph(1)->setData(x1, y1);
    ui->stationary_height_graph->graph(1)->setName("Луч к поверхности");

    ui->stationary_height_graph->addGraph();  // graph of ray from surface
    ui->stationary_height_graph->graph(2)->setPen(QPen(Qt::red));
    ui->stationary_height_graph->graph(2)->setData(x2, y2);
    ui->stationary_height_graph->graph(2)->setName("Луч от поверхности");

    ui->stationary_height_graph->addGraph();  // graph with rls point
    ui->stationary_height_graph->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::blue, 0), QColor(0, 0, 255, 200), 7));
    ui->stationary_height_graph->graph(3)->setData(QVector<double>{d_center}, QVector<double>{point.get_center().get_h()});
    ui->stationary_height_graph->graph(3)->setName("Положение РЛС");
    ui->stationary_height_graph->graph(3)->setLineStyle(QCPGraph::lsNone);

    ui->stationary_height_graph->addGraph();  // graph with reflection point
    ui->stationary_height_graph->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, QPen(Qt::black, 0), QColor(0, 0, 0, 200), 15));
    ui->stationary_height_graph->graph(4)->setData(QVector<double>{d_reflection}, QVector<double>{reflection_point.get_h()});
    ui->stationary_height_graph->graph(4)->setName("Положение точки отражения");
    ui->stationary_height_graph->graph(4)->setLineStyle(QCPGraph::lsNone);

    ui->stationary_height_graph->addGraph();  // graph with target point
    ui->stationary_height_graph->graph(5)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, QPen(Qt::black, 0), QColor(0, 0, 0, 90), 10));
    ui->stationary_height_graph->graph(5)->setData(QVector<double>{d_target}, QVector<double>{point.get_target().get_h()});
    ui->stationary_height_graph->graph(5)->setName("Положение цели");
    ui->stationary_height_graph->graph(5)->setLineStyle(QCPGraph::lsNone);

    // add subgrid
    ui->stationary_height_graph->xAxis->grid()->setSubGridVisible(true);
    ui->stationary_height_graph->yAxis->grid()->setSubGridVisible(true);

    // add labels
    ui->stationary_height_graph->xAxis->setLabel("Расстояние, м");
    ui->stationary_height_graph->yAxis->setLabel("Высота, м");

    ui->stationary_height_graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->stationary_height_graph->replot();
}
