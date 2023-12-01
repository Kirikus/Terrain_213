#include "models_earth.h"

namespace EM = EarthModels;

//This function calculates distance between center and target
double EM::ModelFlat::find_r(PointCartesian center, PointCartesian target)
{
    double x = target.get_x() - center.get_x();
    double y = target.get_y() - center.get_y();
    double h = target.get_h() - center.get_h();

    double r = std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(h, 2));
    return r;
}

//This function calculates elevation angle between center and target
double EM::ModelFlat::find_phi(PointCartesian center, PointCartesian target)
{
    double h = target.get_h() - center.get_h();
    double r = find_r(center, target);

    double phi = std::asin(h/r);
    return phi;
}
