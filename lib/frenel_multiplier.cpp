#include "frenel_multiplier.h"

namespace RP = ReflectionCoefficients;

//  This functions calculate Frenel Refraction Coefficient for horizontal and
//  vetcical polarization.
//  functions accept:
//  1) pointer to the map;
//  2) a spherical point whose target is a reflection point;
//  3) incidence angle in radians.
//  functions return:
//  1) frenel coefficient, which
//  describes the ratio of the amplitude of the signal,
//  reflected from a smooth surface to the amplitude of the incident signal.
//  formulas (2.6) on the page 42

complex RP::FrenelReflectionMultiplier::horizontal_polarization(Map* map, PointSpheric sp, double incidence_angle)
{
    Map1d map1d(map, sp.get_center(), sp.get_target());
    double eps = map1d.dielectric_permittivity(sp.get_d());
    double teta = incidence_angle * 180 / M_PI;
    complex numerator = sin(teta) - sqrt(eps - pow(cos(teta), 2));
    complex denumerator = sin(teta) + sqrt(eps - pow(cos(teta), 2));
    complex frenel_coeff = numerator / denumerator;
    return frenel_coeff;
}

complex RP::FrenelReflectionMultiplier::vertical_polarization(Map* map, PointSpheric sp, double incidence_angle)
{
    Map1d map1d(map, sp.get_center(), sp.get_target());
    double eps = map1d.dielectric_permittivity(sp.get_d());
    double teta = incidence_angle * 180 / M_PI;
    complex numerator = eps * sin(teta) - sqrt(eps - pow(cos(teta), 2));
    complex denumerator = eps * sin(teta) + sqrt(eps - pow(cos(teta), 2));
    complex frenel_coeff = numerator / denumerator;
    return frenel_coeff;
}
