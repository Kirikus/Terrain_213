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
