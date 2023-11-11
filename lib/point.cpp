#include "point.h"

PointSpheric::PointSpheric(PointCartesian center, PointCartesian target)
{
    double x = target.get_x() - center.get_x();
    double y = target.get_y() - center.get_y();
    double h = target.get_h() - center.get_h();
    double xy = hypot(x, y);

    this->center = center;
    this->R = sqrt(pow(x, 2) + pow(y, 2) + pow(h, 2));

    this->azimuth = atan2(y, x) * 180 / M_PI;
    this->azimuth *= M_PI;

    this->phi = atan(h/xy);
}

PointSpheric::PointSpheric(const PointSpheric& sp)
{
    this->center = sp.center;
    this->azimuth = sp.azimuth;
    this->R = sp.R;
    this->phi = sp.phi;
}
