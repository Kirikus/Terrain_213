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

    //atan defined in the range from -90 to 90
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

void PointSpheric::change_r(double new_R)
{
    this->R = new_R;
    double x = cos(azimuth)*cos(phi)*R;
    double y = sin(azimuth)*cos(phi)*R;
    double h = sin(phi)*R;
    move_target(x, y, h);
}

void PointSpheric::change_azimuth(double new_azimuth)
{
    this->azimuth = new_azimuth;
    double x = cos(azimuth)*cos(phi)*R;
    double y = sin(azimuth)*cos(phi)*R;
    double h = sin(phi)*R;
    move_target(x, y, h);
}

void PointSpheric::change_phi(double new_phi)
{
    this->phi = new_phi;
    double x = cos(azimuth)*cos(phi)*R;
    double y = sin(azimuth)*cos(phi)*R;
    double h = sin(phi)*R;
    move_target(x, y, h);
}
void PointSpheric::move_target(double new_x, double new_y, double new_h)
{
    this->target = PointCartesian(new_x, new_y, new_h);

    double x = target.get_x() - center.get_x();
    double y = target.get_y() - center.get_y();
    double h = target.get_h() - center.get_h();
    double xy = hypot(x, y);

    this->R = sqrt(pow(x, 2) + pow(y, 2) + pow(h, 2));

    //atan2 defined in the range from -180 to 180;
    //therefore, if the angle is negative,
    //then we will convert it to the corresponding positive
    this->azimuth = atan2(y, x) * 180 / M_PI;
    if (this->azimuth < 0) this->azimuth += 360;
    this->azimuth /= 180;
    this->azimuth *= M_PI;

    //atan defined in the range from -90 to 90
    this->phi = atan(h/xy);
}
