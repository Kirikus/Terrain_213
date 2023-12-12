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
    ReflectionMultiplier(ElevationReflectionMultiplier* ElevationCoeff, FrenelReflectionMultiplier* FrenelCoeff, VegetationReflectionMultiplier* VegetationCoeff)
        :ElevationCoeff(ElevationCoeff), FrenelCoeff(FrenelCoeff), VegetationCoeff(VegetationCoeff)
    {}

    std::complex<double> reflection_multiplier(Map* map, PointSpheric sp, Polarization pol, double incidence_angle, double wave_l, double conductivity, double sko, Vegetation::VegetationType veg);
private:
    ElevationReflectionMultiplier* ElevationCoeff = nullptr;
    FrenelReflectionMultiplier* FrenelCoeff = nullptr;
    VegetationReflectionMultiplier* VegetationCoeff = nullptr;
};
}

#endif // REFLECTION_MULTIPLIER_H
