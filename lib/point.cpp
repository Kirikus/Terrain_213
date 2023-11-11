#include "point.h"

PointSpheric::PointSpheric(PointCartesian center, PointCartesian target)
{
    double x = target.get_x() - center.get_x();
    double y = target.get_y() - center.get_y();
    double h = target.get_h() - center.get_h();
    double xy = hypot(x, y);

    this->center = center;
    this->target = target;
    this->R = sqrt(pow(x, 2) + pow(y, 2) + pow(h, 2));

    //atan2 defined in the range from -180 to 180;
    //therefore, if the angle is negative,
    //then we will convert it to the corresponding positive
    this->azimuth = atan2(y, x) * 180 / M_PI;
    if (this->azimuth < 0) this->azimuth += 360;
    this->azimuth /= 180;
    this->azimuth *= M_PI;

    this->phi = atan(h/xy);
}

PointSpheric::PointSpheric(const PointSpheric& sp)
{
    this->center = sp.center;
    this->target = sp.target;
    this->azimuth = sp.azimuth;
    this->R = sp.R;
    this->phi = sp.phi;
}
