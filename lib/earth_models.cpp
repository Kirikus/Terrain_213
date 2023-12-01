#include "earth_models.h"

//This function calculates distance between center and target
double ModelFlat::find_r(PointCartesian center, PointCartesian target)
{
    double x = target.get_x() - center.get_x();
    double y = target.get_y() - center.get_y();
    double h = target.get_h() - center.get_h();

    double r = sqrt(pow(x, 2) + pow(y, 2) + pow(h, 2));
    return r;
}

//This function calculates elevation angle between center and target
double ModelFlat::find_phi(PointCartesian center, PointCartesian target)
{
    double h = target.get_h() - center.get_h();
    double r = find_r(center, target);

    double phi = asin(h/r);
    return phi;
}

///-------------------------------------------------------

//This function calculates angle between two radii of earth
double ModelSpheric::find_earth_angle(PointCartesian center, PointCartesian target)
{
    double x = center.get_x() - target.get_x();
    double y = center.get_y() - target.get_y();
    double distance = hypot(x, y);

    double earth_angle = acos(1 - pow(distance, 2)/(2 * pow(r_e, 2)));
    return earth_angle;
}

//This function calculates distance between center and target
double ModelSpheric::find_r(PointCartesian center, PointCartesian target)
{
    double h_t = r_e + target.get_h(); //height relative to the center of the earth
    double h_c = r_e + center.get_h();

    double earth_angle = find_earth_angle(center, target);

    double r = sqrt(pow(h_t, 2) + pow(h_c, 2) - 2*(h_t)*(h_c)*cos(earth_angle));
    return r;
}

//This function calculates elevation angle between center and target
double ModelSpheric::find_phi(PointCartesian center, PointCartesian target)
{
    double h_a = target.get_h();
    double h_b = center.get_h();
    double r = find_r(center, target);

    double phi = asin((h_a - h_b)*(2*r_e + h_a + h_b)/(2*r*(r_e + h_b)) - r/(2*(r_e + h_b)));
    return phi;
}

///-------------------------------------------------------

//This function calculates angle between two radii of earth
double ModelEffectiveRadius::find_earth_angle(PointCartesian center, PointCartesian target)
{
    double x = center.get_x() - target.get_x();
    double y = center.get_y() - target.get_y();
    double distance = hypot(x, y);

    double earth_angle = acos(1 - pow(distance, 2)/(2 * pow(k*r_e, 2)));
    return earth_angle;
}

//This function calculates distance between center and target
double ModelEffectiveRadius::find_r(PointCartesian center, PointCartesian target)
{
    double h_t = k*r_e + target.get_h(); //height relative to the center of the earth
    double h_c = k*r_e + center.get_h();

    double earth_angle = find_earth_angle(center, target);

    double r = sqrt(pow(h_t, 2) + pow(h_c, 2) - 2*(h_t)*(h_c)*cos(earth_angle));
    return r;
}

//This function calculates elevation angle between center and target
double ModelEffectiveRadius::find_phi(PointCartesian center, PointCartesian target)
{
    double h_a = target.get_h();
    double h_b = center.get_h();
    double r = find_r(center, target);

    double phi = asin((h_a - h_b)*(2*k*r_e + h_a + h_b)/(2*r*(k*r_e + h_b)) - r/(2*(k*r_e + h_b)));
    return phi;
}
