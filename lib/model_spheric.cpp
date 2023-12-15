#include "models_earth.h"

namespace EM = EarthModels;

// This function calculates angle between two radii of earth
// function accepts:
// 1) center - point with center coordinates
// 2) target - point with target coordinates
// function returns:
// 1) Angle between two radii of earth

double EM::ModelSpheric::find_earth_angle(PointCartesian center, PointCartesian target)
{
    double x = center.get_x() - target.get_x();
    double y = center.get_y() - target.get_y();
    double distance = std::hypot(x, y);

    double earth_angle = std::acos(1 - std::pow(distance, 2)/(2 * std::pow(r_e, 2)));
    return earth_angle;
}

// This function calculates distance between center and target
// function accepts:
// 1) center - point with center coordinates
// 2) target - point with target coordinates
// function returns:
// 1) distance between these points

double EM::ModelSpheric::find_r(PointCartesian center, PointCartesian target)
{
    double h_t = r_e + target.get_h(); //height relative to the center of the earth
    double h_c = r_e + center.get_h();

    double earth_angle = find_earth_angle(center, target);

    double r = std::sqrt(std::pow(h_t, 2) + std::pow(h_c, 2) - 2*h_t*h_c*std::cos(earth_angle));
    return r;
}

// This function calculates elevation angle between center and target
// function accepts:
// 1) center - point with center coordinates
// 2) target - point with target coordinates
// function returns:
// 1) elevation angle

double EM::ModelSpheric::find_phi(PointCartesian center, PointCartesian target)
{
    double h_a = target.get_h();
    double h_b = center.get_h();
    double r = find_r(center, target);

    double phi = std::asin((h_a - h_b)*(2*r_e + h_a + h_b)/(2*r*(r_e + h_b)) - r/(2*(r_e + h_b)));
    return phi;
}
