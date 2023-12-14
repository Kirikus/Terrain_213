#ifndef REFLECTION_MULTIPLIER_H
#define REFLECTION_MULTIPLIER_H

#include <cmath>
#include <complex>
#include "elevation_reflection_multiplier.h"
#include "frenel_multiplier.h"
#include "vegetation_reflection_multiplier.h"

namespace ReflectionCoefficients
{
class ReflectionMultiplier
{
public:
    ReflectionMultiplier() = default;

    std::complex<double> reflection_multiplier(Map* map, PointSpheric sp, Polarization pol, double incidence_angle, double wave_l, double conductivity, double sko, Vegetation::VegetationType veg);
private:
    ElevationReflectionMultiplier ElevationCoeff;
    FrenelReflectionMultiplier FrenelCoeff;
    VegetationReflectionMultiplier VegetationCoeff;
};
}

#endif // REFLECTION_MULTIPLIER_H
