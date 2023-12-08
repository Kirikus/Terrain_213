#ifndef VEGETATION_REFLECTION_MULTIPLIER_H
#define VEGETATION_REFLECTION_MULTIPLIER_H

#include <cmath>

namespace ReflectionCoefficients
{
class VegetationReflectionMultiplier
{
public:
    double vegetation_coeff(double incidence_angle, double a, double b, double wave_l);
};
}

#endif // VEGETATION_REFLECTION_MULTIPLIER_H
