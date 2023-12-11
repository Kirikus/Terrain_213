#include "frenel_multiplier.h"

namespace RP = ReflectionCoefficients;

//  This functions calculate Frenel Refraction Coefficient for different polarization.
//  functions accept:
//  1) pointer to the map;
//  2) a spherical point whose target is a reflection point;
//  3) incidence angle in radians.
//  functions return:
//  1) frenel coefficient, which
//  describes the ratio of the amplitude of the signal,
//  reflected from a smooth surface to the amplitude of the incident signal.
//  formulas (2.6) on the page 42

std::complex<double> RP::FrenelReflectionMultiplier::horizontal_polarization(Map* map, PointSpheric sp, double incidence_angle, double wave_l, double conductivity)
{
    PointCartesian relief_dot = sp.get_target();
    std::complex<double> eps = (map->dp(&relief_dot), -60 * wave_l * conductivity);
    std::complex<double> numerator = std::sin(incidence_angle) - std::sqrt(eps - std::pow(std::cos(incidence_angle), 2));
    std::complex<double> denumerator = std::sin(incidence_angle) + std::sqrt(eps - std::pow(std::cos(incidence_angle), 2));
    std::complex<double> frenel_coeff = numerator / denumerator;
    return frenel_coeff;
}

std::complex<double> RP::FrenelReflectionMultiplier::vertical_polarization(Map* map, PointSpheric sp, double incidence_angle, double wave_l, double conductivity)
{
    PointCartesian relief_dot = sp.get_target();
    std::complex<double> eps = (map->dp(&relief_dot), - 60 * wave_l * conductivity);
    std::complex<double> numerator = eps * std::sin(incidence_angle) - std::sqrt(eps - std::pow(std::cos(incidence_angle), 2));
    std::complex<double> denumerator = eps * std::sin(incidence_angle) + std::sqrt(eps - std::pow(std::cos(incidence_angle), 2));
    std::complex<double> frenel_coeff = numerator / denumerator;
    return frenel_coeff;
}

std::complex<double> RP::FrenelReflectionMultiplier::circular_polarization(Map* map, PointSpheric sp, double incidence_angle, double wave_l, double conductivity)
{
    std::complex<double> vertical = vertical_polarization(map, sp, incidence_angle, wave_l, conductivity);
    std::complex<double> horizontal = horizontal_polarization(map, sp, incidence_angle, wave_l, conductivity);
    return 0.5*(vertical + horizontal);
}

std::complex<double> RP::FrenelReflectionMultiplier::cross_polarization(Map* map, PointSpheric sp, double incidence_angle, double wave_l, double conductivity)
{
    std::complex<double> vertical = vertical_polarization(map, sp, incidence_angle, wave_l, conductivity);
    std::complex<double> horizontal = horizontal_polarization(map, sp, incidence_angle, wave_l, conductivity);
    return 0.5*(vertical - horizontal);
}
