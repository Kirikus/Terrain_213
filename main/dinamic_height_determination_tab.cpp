#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../lib/height_determination.h"

void MainWindow::on_dinamic_height_apply_button_clicked(QAbstractButton *button)
{
    if (!ui->dinamic_height_h->displayText().isEmpty())  // if there was any output
    {
        ui->dinamic_height_h->clear();
        ui->dinamic_height_graph->clearGraphs();
    }

    int i = ui->dinamic_height_calculation_mode->checkedId();
    switch(i)
    {
    case -3:  // flat
        dinamic_height_model = models[0];
        break;
    case -2:  // curve
        dinamic_height_model = models[1];
        break;
    case -4:  // reflection
        dinamic_height_model = models[2];
        break;
    default:
        break;
    }

    dinamic_height_map = Map(&geo_data, &vegetation, &const_dp, &c);

    dinamic_height_R = ui->dinamic_height_R->value();
    dinamic_height_R1 = ui->dinamic_height_R1->value();
    dinamic_height_R2 = ui->dinamic_height_R2->value();

    double x = ui->dinamic_height_x->value();
    double y = ui->dinamic_height_y->value();

    dinamic_height_rls_data = ui->dinamic_height_rls->get_all_data();
    PointCartesian rls_position = dinamic_height_rls_data.position;

    PointCartesian reflection_point;
    dinamic_height_object = PointSpheric(dinamic_height_rls_data.position, PointCartesian(x, y, 0));

    double h = FindTargetHeight(&dinamic_height_map, dinamic_height_model, rls_position, Point2d(x, y),
                                &reflection_point, dinamic_height_R, dinamic_height_R1, dinamic_height_R2, dinamic_height_rls_data.radius);

    dinamic_height_object.move_target(x, y, h);

    ui->dinamic_height_h->insert(QString::number(std::round(h * 100) / 100));

    PointCartesian target(x, y, h);
    dinamic_height_plot_graph(dinamic_height_rls_data.position, target, reflection_point, &dinamic_height_map);
}

void MainWindow::on_dinamic_height_slider_sliderMoved(int position)
{
    if (!ui->dinamic_height_h->displayText().isEmpty())  // if there was any output
    {
        ui->dinamic_height_h->clear();
        ui->dinamic_height_graph->clearGraphs();
        PointSpheric current_object = dinamic_height_object;  // copy default object position
        current_object.change_r(current_object.get_R() * (100 - position) / 100);

        PointCartesian reflection_point = ReflectionPoint::FindReflectionPoint(current_object, &dinamic_height_map, dinamic_height_model);
        double x = current_object.get_x();
        double y = current_object.get_y();

        PointSpheric rls_to_reflection_point(dinamic_height_rls_data.position, reflection_point);
        PointSpheric reflection_point_to_object(reflection_point, PointCartesian(x, y, current_object.get_h()));

        double R = current_object.get_R();
        double R1 = rls_to_reflection_point.get_R();
        double R2 = reflection_point_to_object.get_R();
        double h = FindTargetHeight(&dinamic_height_map, dinamic_height_model, dinamic_height_rls_data.position, Point2d(x, y), &reflection_point, R, R1, R2, dinamic_height_rls_data.radius);

        ui->dinamic_height_h->insert("h=" + QString::number(h) + " R=" + QString::number(R) + " R1=" + QString::number(R1) + " R2=" + QString::number(R2));
//        ui->dinamic_height_h->insert(QString::number(std::round(h * 100) / 100));

        PointCartesian target(current_object.get_target().get_x(), current_object.get_target().get_y(), current_object.get_target().get_h());
        dinamic_height_plot_graph(dinamic_height_rls_data.position, target, reflection_point, &dinamic_height_map);
    }
}

void MainWindow::dinamic_height_plot_graph(PointCartesian rls, PointCartesian target, PointCartesian reflection_point, Map* map)
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

    ui->dinamic_height_graph->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->dinamic_height_graph->legend->setFont(legendFont);
    ui->dinamic_height_graph->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));

    ui->dinamic_height_graph->addGraph();  // graph of surface
    ui->dinamic_height_graph->graph(0)->setPen(QPen(QColor(32, 32, 32)));
    ui->dinamic_height_graph->graph(0)->setData(x, y);
    ui->dinamic_height_graph->graph(0)->setName("Поверхность");
    ui->dinamic_height_graph->graph(0)->rescaleAxes();

    ui->dinamic_height_graph->addGraph();  // graph of ray to surface
    ui->dinamic_height_graph->graph(1)->setPen(QPen(Qt::blue));
    ui->dinamic_height_graph->graph(1)->setData(x1, y1);
    ui->dinamic_height_graph->graph(1)->setName("Луч к поверхности");

    ui->dinamic_height_graph->addGraph();  // graph of ray from surface
    ui->dinamic_height_graph->graph(2)->setPen(QPen(Qt::red));
    ui->dinamic_height_graph->graph(2)->setData(x2, y2);
    ui->dinamic_height_graph->graph(2)->setName("Луч от поверхности");

    ui->dinamic_height_graph->addGraph();  // graph with rls point
    ui->dinamic_height_graph->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::blue, 0), QColor(0, 0, 255, 200), 7));
    ui->dinamic_height_graph->graph(3)->setData(QVector<double>{d_center}, QVector<double>{point.get_center().get_h()});
    ui->dinamic_height_graph->graph(3)->setName("Положение РЛС");
    ui->dinamic_height_graph->graph(3)->setLineStyle(QCPGraph::lsNone);

    ui->dinamic_height_graph->addGraph();  // graph with reflection point
    ui->dinamic_height_graph->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, QPen(Qt::black, 0), QColor(0, 0, 0, 200), 15));
    ui->dinamic_height_graph->graph(4)->setData(QVector<double>{d_reflection}, QVector<double>{reflection_point.get_h()});
    ui->dinamic_height_graph->graph(4)->setName("Положение точки отражения");
    ui->dinamic_height_graph->graph(4)->setLineStyle(QCPGraph::lsNone);

    ui->dinamic_height_graph->addGraph();  // graph with target point
    ui->dinamic_height_graph->graph(5)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, QPen(Qt::black, 0), QColor(0, 0, 0, 90), 10));
    ui->dinamic_height_graph->graph(5)->setData(QVector<double>{d_target}, QVector<double>{point.get_target().get_h()});
    ui->dinamic_height_graph->graph(5)->setName("Положение цели");
    ui->dinamic_height_graph->graph(5)->setLineStyle(QCPGraph::lsNone);

    // add subgrid
    ui->dinamic_height_graph->xAxis->grid()->setSubGridVisible(true);
    ui->dinamic_height_graph->yAxis->grid()->setSubGridVisible(true);

    // add labels
    ui->dinamic_height_graph->xAxis->setLabel("Расстояние, м");
    ui->dinamic_height_graph->yAxis->setLabel("Высота, м");

    ui->dinamic_height_graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->dinamic_height_graph->replot();
}
