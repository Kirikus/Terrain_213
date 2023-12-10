#include "reflection_point.h"

namespace RP = ReflectionPoint;

// This function considers the derivative of z(d) by definition
// functions accepts:
// 1) PointSpheric rp - the associated coordinates of the center and the relief point;
// 2) Map* map - Pointer to the class of the map data.
// function returns:
// 1) The approximate value of the derivative at the point.

double RP::FindDerivative(PointSpheric rp, Map* map)
{
    double delta_d = 0.001;

    Point2d point1(rp.get_target().get_x(), rp.get_target().get_y());
    rp.change_d(rp.get_d() + delta_d);
    Point2d point2(rp.get_target().get_x(), rp.get_target().get_y());

    double h1 = map->h(&point1);
    double h2 = map->h(&point2);
    double derivative = (h2 - h1) / delta_d;

    return derivative;
}

// This function counts the angle between two straight lines
// function accepts:
// 1) k1 - the oblique coefficient of the first straight line;
// 2) k2 - the oblique coefficient of the second straight line.
// function returns:
// 1) The angle between two straight lines in radians.

double RP::FindPhi(double k1, double k2)
{
    return std::atan((k2 - k1) / (1 + k1 * k2));
}

// This function finds the coordinates of the reflection point
// function accepts:
// 1) PointSpheric sp - the associated coordinates of the center and the target;
// 2) Map* map - Pointer to the class of the map data.
// function returns:
// 1) PointCartesian reflection_point - the point of reflection if there is one;
// 2) PointCartesian center - if there is no reflection point.

PointCartesian RP::FindReflectionPoint(PointSpheric sp, Map* map)
{
    Map1d map1d(map, sp.get_center(), sp.get_target());
    PointCartesian reflection_point(sp.get_center());

    double epsilon = 0.001;
    double min_diff = 2 * M_PI;

    double step = 0.005;
    for (double curr_d = step; curr_d < sp.get_d(); curr_d += step)
    {
        double h = map1d.height(curr_d);
        double x = sp.get_center().get_x() + curr_d * std::cos(sp.get_azimuth());
        double y = sp.get_center().get_y() + curr_d * std::sin(sp.get_azimuth());
        PointCartesian relief_dot(x, y, h);
        PointSpheric rp1(sp.get_center(), relief_dot);
        PointSpheric rp2(sp.get_target(), relief_dot);

        double derivative = FindDerivative(rp1, map);
        double k1 = (h - sp.get_center().get_h()) / rp1.get_d();
        double k2 = - (h - sp.get_target().get_h()) / rp2.get_d();

        if (std::abs(std::abs(FindPhi(derivative, k1)) - std::abs(FindPhi(derivative, k2))) <= epsilon)
            if (std::abs(std::abs(FindPhi(derivative, k1)) - std::abs(FindPhi(derivative, k2))) < min_diff)
            {
                min_diff = std::abs(std::abs(FindPhi(derivative, k1)) - std::abs(FindPhi(derivative, k2)));
                reflection_point = relief_dot;
            }
    }

    return reflection_point;
}

// This function finds the angle of incidence at the reflection point
// function accepts:
// 1) PointSpheric sp - the associated coordinates of the center and the reflection point;
// 2) Map* map - Pointer to the class of the map data.
// function returns:
// 1) The approximate value of the incidence angle at the point.

double RP::FindIncidenceAngle(PointSpheric sp, Map* map)
{
    Point2d reflection_point2d(sp.get_target().get_x(), sp.get_target().get_y());

    double derivative = FindDerivative(sp, map);
    double h = map->h(&reflection_point2d);
    double k1 = (h - sp.get_center().get_h()) / sp.get_d();

    return std::abs(FindPhi(derivative, k1));
}
