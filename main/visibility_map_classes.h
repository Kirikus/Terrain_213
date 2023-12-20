#ifndef VISIBILITY_MAP_CLASSES_H
#define VISIBILITY_MAP_CLASSES_H

#include <QMainWindow>
#include <QAbstractButton>

#include <qcustomplot.h>

#include "../lib/point.h"

struct Contour
{
public:
    Contour() = default;
    Contour(double angle_iter) : angle_iterations{angle_iter} {};

    void add_point(PointSpheric point, const bool external_point);
    void change_point(const double azimuth, PointSpheric point, const bool external_point);

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

#endif // VISIBILITY_MAP_CLASSES_H
