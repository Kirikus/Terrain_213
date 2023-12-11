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

std::complex<double> RP::FrenelReflectionMultiplier::horizontal_polarization(Map* map, PointSpheric sp, double incidence_angle)
{
    Map1d map1d(map, sp.get_center(), sp.get_target());
    std::complex<double> eps = map1d.dielectric_permittivity(sp.get_d());
    std::complex<double> numerator = std::sin(incidence_angle) - std::sqrt(eps - std::pow(std::cos(incidence_angle), 2));
    std::complex<double> denumerator = std::sin(incidence_angle) + std::sqrt(eps - std::pow(std::cos(incidence_angle), 2));
    std::complex<double> frenel_coeff = numerator / denumerator;
    return frenel_coeff;
}

std::complex<double> RP::FrenelReflectionMultiplier::vertical_polarization(Map* map, PointSpheric sp, double incidence_angle)
{
    Map1d map1d(map, sp.get_center(), sp.get_target());
    std::complex<double> eps = map1d.dielectric_permittivity(sp.get_d());
    std::complex<double> numerator = eps * std::sin(incidence_angle) - std::sqrt(eps - std::pow(std::cos(incidence_angle), 2));
    std::complex<double> denumerator = eps * std::sin(incidence_angle) + std::sqrt(eps - std::pow(std::cos(incidence_angle), 2));
    std::complex<double> frenel_coeff = numerator / denumerator;
    return frenel_coeff;
}
