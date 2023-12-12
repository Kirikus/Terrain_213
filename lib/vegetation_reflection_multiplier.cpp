#include "vegetation_reflection_multiplier.h"

namespace RC = ReflectionCoefficients;
namespace VG = Vegetation;

// This function calculates attenuation coefficient for vegetation
// function accepts:
// 1) incidence angle in radians
// 2) tabular coefficient a
// 3) tabular coefficient b
// 4) wave length in meters
// function returns:
// attenuation coefficient for vegetation
// formula (2.18) on the page 50

double RC::VegetationReflectionMultiplier::vegetation_coeff(double incidence_angle, VG::VegetationType relief, double wave_l)
{
    double a{0}, b{0};

    switch(relief)
    {
    case(VG::VegetationType::None):
        a = 0;
        b = 0;
        break;
    case(VG::VegetationType::Grass):
        a = 3.2;
        b = 1;
        break;
    case(VG::VegetationType::Shrub):
        a = 0.32;
        b = 3;
        break;
    case(VG::VegetationType::Forest):
        a = 0.032;
        b = 5;
        break;
    }

    double vegetation_coefficient = (1 - std::sqrt(a * wave_l)) * std::exp(-b * std::sin(incidence_angle) / wave_l) + std::sqrt(a * wave_l);
    return vegetation_coefficient;
}
