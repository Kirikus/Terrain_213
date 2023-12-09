#include "reflection_point.h"

namespace RP = ReflectionPoint;

double RP::FindDerivative(PointSpheric rp, Map* map)
{
    double delta_d = 0.001;

    Point2d point1(rp.get_target().get_x(), rp.get_target().get_y());
    rp.change_d(rp.get_d() + delta_d);
    Point2d point2(rp.get_target().get_x(), rp.get_target().get_y());

    double h1 = map->h(point1);
    double h2 = map->h(point2);
    double derivative = (h2 - h1) / delta_d;

    return derivative;
}

double RP::FindPhi(double k1, double k2)
{
    return std::atan((k2 - k1) / (1 + k1 * k2));
}

PointCartesian RP::FindReflectionPoint(PointSpheric sp, Map* map)
{
    Map1d map1d(map, sp.get_center(), sp.get_target());
    double epsilon = 0.001;
    PointCartesian reflection_point(sp.get_center());
    double min_diff = 2 * M_PI;
    for (double curr_d = 0.005; curr_d < sp.get_d();)
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

        curr_d += 0.005;
    }

    return reflection_point;
}

double RP::FindIncidenceAngle(PointSpheric sp, Map* map)
{
    Point2d reflection_point2d(sp.get_target().get_x(), sp.get_target().get_y());

    double derivative = FindDerivative(sp, map);
    double h = map->h(reflection_point2d);
    double k1 = (h - sp.get_center().get_h()) / sp.get_d();

    return std::abs(FindPhi(derivative, k1));
}
