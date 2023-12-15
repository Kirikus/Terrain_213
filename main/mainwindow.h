#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>

#include <qcustomplot.h>
#include <rls.h>
#include "../lib/point.h"
#include "../lib/reflection_point.h"
#include <iostream>

class AngleMap;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void on_visibility_map_apply_button_clicked(QAbstractButton *button);
    void on_visibility_map_add_RLS_clicked();
    void on_visibility_map_RLS_widgets_tabCloseRequested(int index);

    void on_reflection_apply_button_clicked(QAbstractButton *button);

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    Ui::MainWindow *ui;
    size_t _rls_index = 0;  // index of last RLS in RLS_widget

    QVector<QVector<QCPCurve*>> rls_contour_curves;  // contours for screening angle map for each rls
    QVector<QColor> colors{QColor("#F2E750"), QColor("#F2B807"), QColor("#F28705"), QColor("#C52104"), QColor("#710301")};  // colors for graphs in angle map
    std::vector<double> angles;  // angles for the i contour (<= angles[i])
    void _plot_image();
    void _plot_angle_map(AngleMap angle_map, PointCartesian rls);
    AngleMap _screen_angle_search(RLS::Data data);
    void plot_reflection_graph(PointCartesian rls, PointCartesian target, PointCartesian reflection_point, Map* map);
    std::complex<double> find_reflection_coefficient(RLS::Data rls_data, Map* map, PointCartesian reflection_point, EarthModels::ModelEarth* model);
    void get_angles();
};

struct Contour
{
public:
    Contour() = default;
    Contour(double angle_iter) : angle_iterations{angle_iter} {};

    void add_point(PointSpheric point, const bool external_point)
    {
        double azimuth = point.get_azimuth();
        int key = int(azimuth * angle_iterations);
        if (external_point)
            if (points.find(key) != points.end())
                points[key].second = Point2d(point.get_target().get_x(), point.get_target().get_y());
            else
                points[key] = std::make_pair(rls_position, Point2d(point.get_target().get_x(), point.get_target().get_y()));
        else
            if (points.find(key) != points.end())
                points[key].first = Point2d(point.get_target().get_x(), point.get_target().get_y());
            else
                points[key] = std::make_pair(Point2d(point.get_target().get_x(), point.get_target().get_y()), rls_position);
        last_added_point = point;
    }

    void change_point(const double azimuth, PointSpheric point, const bool external_point)
    {
        int key = int(azimuth * angle_iterations);
        if (external_point)
            if (points.find(key) != points.end())
                points[key].second = Point2d(point.get_target().get_x(), point.get_target().get_y());
            else
                points[key] = std::make_pair(rls_position, Point2d(point.get_target().get_x(), point.get_target().get_y()));
        else
            if (points.find(key) != points.end())
                points[key].first = Point2d(point.get_target().get_x(), point.get_target().get_y());
            else
                points[key] = std::make_pair(Point2d(point.get_target().get_x(), point.get_target().get_y()), rls_position);
        last_added_point = point;
    }

    void set_angle_iter(const double angle_iter) { angle_iterations = angle_iter; };
    void set_rls_position(PointCartesian rls) { rls_position = Point2d(rls.get_x(), rls.get_y()); }
    bool if_zero() { return zero_contour; }
    void is_zero() { zero_contour = true; }
    PointSpheric back() { return last_added_point; }
    const PointSpheric external(const double azimuth)
    {
        Point2d point2d = points[int(azimuth)].second;
        PointCartesian rls(rls_position.get_x(), rls_position.get_y(), 0);
        PointCartesian point(point2d.get_x(), point2d.get_y(), 0);
        return PointSpheric(rls, point);
    }
    QVector<QCPCurveData> get_data();


private:
    std::map<int, std::pair<Point2d, Point2d>> points;  // points of a contour to draw visibility angle map
    double angle_iterations;  // count of angle iteration
    bool zero_contour = false;  // if it's contour with 0 number (it's special because it's only external)
    PointSpheric last_added_point;
    Point2d rls_position;

    QCPCurveData get_point(const int key,  const int i, const bool external);
    QVector<QCPCurveData> get_zero_data();
};


struct AngleMap
{
public:
    AngleMap(size_t contour_count, const double angle_iterations, PointCartesian rls_position)
    {
        contours.resize(contour_count);
        contours[0].is_zero();
        for (size_t i = 0; i < contour_count; i++)
        {
            contours[i].set_angle_iter(angle_iterations);
                contours[i].set_rls_position(rls_position);
        }
    }
    std::vector<Contour> contours;  // contours to draw visibility angle map
};

#endif  // MAINWINDOW_H
