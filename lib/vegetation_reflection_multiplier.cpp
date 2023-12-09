#include "vegetation_reflection_multiplier.h"

namespace RP = ReflectionCoefficients;

// This function calculates attenuation coefficient for vegetation
// function accepts:
// 1) incidence angle in radians
// 2) tabular coefficient a
// 3) tabular coefficient b
// 4) wave length in meters
// function returns:
// attenuation coefficient for vegetation
// formula (2.18) on the page 50

double RP::VegetationReflectionMultiplier::vegetation_coeff(double incidence_angle, double a, double b, double wave_l)
{
    double vegetation_coefficient = (1 - std::sqrt(a * wave_l)) * std::exp(-b * std::sin(incidence_angle) / wave_l) + std::sqrt(a * wave_l);
    return vegetation_coefficient;
}
