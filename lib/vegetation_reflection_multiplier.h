#ifndef VEGETATION_REFLECTION_MULTIPLIER_H
#define VEGETATION_REFLECTION_MULTIPLIER_H

#include <cmath>
#include "vegetation.h"

namespace ReflectionCoefficients
{
class VegetationReflectionMultiplier
{
public:
    double vegetation_coeff(double incidence_angle, Vegetation::VegetationType relief, double wave_l);
};
}

#endif // VEGETATION_REFLECTION_MULTIPLIER_H
