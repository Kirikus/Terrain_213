#include "point.h"

SphericPoint::SphericPoint(CartesianPoint center, CartesianPoint target)
{
    double r_x = target.get_x() - center.get_x();
    double r_y = target.get_y() - center.get_y();
    double r_h = target.get_h() - center.get_h();
    double r_xy = hypot(r_x, r_y);

    this->center = center;
    this->R = sqrt(pow(r_x, 2) + pow(r_y, 2) + pow(r_h, 2));
    this->azimuth = atan2(r_y, r_x) * 180 / M_PI;
    this->phi = atan2(r_h, r_xy) * 180 / M_PI;
}

SphericPoint::SphericPoint(const SphericPoint& sp)
{
    this->center = sp.center;
    this->azimuth = sp.azimuth;
    this->R = sp.R;
    this->phi = sp.phi;
}
